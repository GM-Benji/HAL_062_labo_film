# Dokumentacja Protokołu Sterowania CAN

System sterowania oparty jest na magistrali CAN. Urządzenie nasłuchuje na dwa identyfikatory (Standard ID):
1. **0x096** (Główna ramka sterująca: Serwo 1, Silniki DC, Wiertło)
2. **0x097** (Ramka pomocnicza: Serwo 2)

## Parametry Magistrali
* **Baudrate:** 500 kbps (zależnie od prescalera)
* **Format:** Standard Frame (11-bit ID)
* **DLC:** 8 bajtów

---

## 1. Ramka Główna (ID: 0x096)
Obsługuje pierwsze ramię (Serwo 1), silniki pomocnicze (Mieszadło/Posuw) oraz główne wrzeciono wiertła.

| Bajt | Funkcja | Zakres / Opis |
| :---: | :--- | :--- |
| **0** | **Tryb Serwa 1** | `1` = Prawo (+), `2` = Lewo (-), `0` = Brak zmian |
| **1** | **Pozycja S1 (MSB)** | Starszy bajt wartości zmiany pozycji (Delta) |
| **2** | **Pozycja S1 (LSB)** | Młodszy bajt wartości zmiany pozycji (Delta) |
| **3** | **Tryb Silników DC** | Wybór silnika i kierunku (patrz sekcja *Silniki DC*) |
| **4** | **Prędkość DC** | PWM dla silnika DC (`0` - `255`) |
| **5** | **Tryb Wiertła** | `0` = Stop, `1` = Prawo (CW), `2` = Lewo (CCW) |
| **6** | **Prędkość Wiertła** | Moc wiertła (`0` - `255`) |
| **7** | *Rezerwa* | Nieużywany |

### Szczegóły sterowania

#### A. Serwo 1 (Dynamixel ID: 0x1E)
Wartość w bajtach 1 i 2 określa **zmianę pozycji** (delta), a nie pozycję absolutną.
* **Wzór:** `Delta = (Bajt[1] << 8) | Bajt[2]`
* **Ruch:** Aktualna pozycja jest zwiększana lub zmniejszana o `Delta`. Wynik jest limitowany do zakresu 0-1023.

#### B. Silniki DC (Bajty 3 i 4) – Zabezpieczenia
System posiada flagę **Current Limit**. Tylko jeden silnik DC (Mieszadło lub Posuw) może pracować w danej chwili.
Aby zmienić kierunek lub silnik, należy najpierw wysłać komendę STOP (`0`), co resetuje blokadę.

| Wartość (Bajt 3) | Akcja | Warunki uruchomienia |
| :--- | :--- | :--- |
| **0** | **STOP / RESET** | Zatrzymuje silniki, zeruje flagę `current_limit`. |
| **1** | **Mieszadło (CCW)** | Wymagany brak blokady (`current_limit == 0`). |
| **2** | **Mieszadło (CW)** | Wymagany brak blokady (`current_limit == 0`). |
| **3** | **Posuw Wiertła (CCW)** | Wymagany brak blokady ORAZ nieaktywna krańcówka górna (`!up_limit`). |
| **4** | **Powrót Wiertła (CW)** | Wymagany brak blokady ORAZ nieaktywna krańcówka dolna (`!down_limit`). |

> **Uwaga:** Krańcówki (`up_limit`, `down_limit`) blokują ruch wiertła w danym kierunku. Ruch w przeciw stronę zeruje flagę odpowiedniej krańcówki.

#### C. Wiertło Główne (Bajty 5 i 6)
Sterowanie ESC poprzez sygnał PWM.
* **Stop:** Bajt 5 = `0` (PWM ustawiane na wartość neutralną 4800).
* **Praca:** Bajt 5 = `1` lub `2`. Prędkość skalowana: `Input(0-255) -> Output(0-1600)`.

---

## 2. Ramka Pomocnicza (ID: 0x097)
Obsługuje drugie ramię (Serwo 2). Struktura jest uproszczona.

| Bajt | Funkcja | Zakres / Opis |
| :---: | :--- | :--- |
| **0** | **Tryb Serwa 2** | `1` = Prawo (+), `2` = Lewo (-), `0` = Brak zmian |
| **1** | **Pozycja S2 (MSB)** | Starszy bajt wartości zmiany pozycji |
| **2** | **Pozycja S2 (LSB)** | Młodszy bajt wartości zmiany pozycji |
| **3-7** | *Nieużywane* | - |

#### Serwo 2 (Dynamixel ID: 0x01)
Działa analogicznie do Serwa 1. Steruje pozycją absolutną poprzez dodawanie/odejmowanie wartości delta.

---

## Przykłady Ramek (Hex)

### 1. Start Mieszadła (10% mocy)
Należy ustawić Bajt 3 na `1` (Mieszadło) i Bajt 4 na `25` (Moc). Serwa i Wiertło wyłączone.
* **ID:** `0x096`
* **Data:** `00 00 00 01 80 00 00 00`

### 2. Zatrzymanie Mieszadła (Wymagane przed zmianą na Wiertło)
Wysłanie zera w Bajcie 3 resetuje flagę `current_limit`.
* **ID:** `0x096`
* **Data:** `00 00 00 00 00 00 00 00`

### 3. Ruch Wiertła (Posuw) + Włączenie Wiertła
Posuw w dół (Tryb 3) z pełną mocą (255), Wiertło kręci się w prawo (Tryb 1) z połową mocy (128).
* **ID:** `0x096`
* **Data:** `00 00 00 03 FF 01 80 00`

### 4. Ruch obu serw
Wymaga wysłania dwóch ramek jedna po drugiej.
* **Ruch Serwa 1 (Prawo o 100):**
  ID: `0x096` -> `01 00 64 00 00 00 00 00`
* **Ruch Serwa 2 (Lewo o 50):**
  ID: `0x097` -> `02 00 32 00 00 00 00 00`
