## Protokół Sterowania CAN

Urządzenie nasłuchuje na ramki CAN ze standardowym identyfikatorem (StdId). Poniżej znajduje się opis struktury ramki sterującej.

### Parametry Transmisji
| Parametr | Wartość |
| :--- | :--- |
| **CAN ID** | `0x096` (Hex) / `150` (Dec) |
| **DLC** | 8 bajtów (używane bajty 0-6) |

### Mapa Ramki (Data Frame)

Ramka podzielona jest na trzy sekcje sterujące różnymi podzespołami:

| Bajt (Index) | Funkcja | Opis |
| :---: | :--- | :--- |
| **0** | **Tryb Serwa (AX12)** | `1` = Prawo (+), `2` = Lewo (-), Inne = Brak zmiany |
| **1** | **Pozycja MSB** | Starszy bajt zmiany pozycji (delta) |
| **2** | **Pozycja LSB** | Młodszy bajt zmiany pozycji (delta) |
| **3** | **Tryb Silników DC** | Wybór silnika i kierunku (Mieszadło/Wiertło) |
| **4** | **Prędkość DC** | PWM dla silników DC (0-255) |
| **5** | **Tryb Wiertła (ESC)** | Sterowanie obrotami wiertła (BLDC/ESC) |
| **6** | **Prędkość Wiertła** | Moc obrotów wiertła (0-255) |
| **7** | *Nieużywany* | - |

---

### Szczegółowy Opis Bajtów

#### 1. Sterowanie Serwem AX12 (Bajty 0-2)
Zmienia aktualną pozycję o zadaną wartość (delta). Wynikowa pozycja jest limitowana do zakresu 0-1023.

* **Bajt 0 (Komenda):**
    * `0x01`: Zwiększ pozycję (Ruch w prawo).
    * `0x02`: Zmniejsz pozycję (Ruch w lewo).
* **Bajty 1 i 2 (Wartość):** 16-bitowa liczba określająca o ile zmienić pozycję.
    * Wartość = `(Bajt[1] << 8) + Bajt[2]`

#### 2. Sterowanie Silnikami DC (Bajty 3-4)
Steruje silnikami pomocniczymi (Mieszadło - Motor2, Posuw Wiertła - Motor3).

* **Bajt 3 (Tryb):**
    * `0`: **STOP** (Zatrzymuje Motor2 i Motor3).
    * `1`: Mieszadło (Motor2) w lewo (CCW).
    * `2`: Mieszadło (Motor2) w prawo (CW).
    * `3`: Posuw wiertła (Motor3) w lewo (CCW).
    * `4`: Posuw wiertła (Motor2*) w prawo (CW).
* **Bajt 4 (Prędkość):** Wartość PWM (0-255).

*\*Uwaga: W kodzie dla trybu 4 sterowany jest Motor2, mimo że logicznie (jako powrót wiertła) sugerowany byłby Motor3.*

#### 3. Sterowanie Wiertłem Głównym / ESC (Bajty 5-6)
Steruje sygnałem PWM dla regulatora ESC (Timer 4 Channel 2).

* **Bajt 5 (Kierunek):**
    * `0`: **STOP** (Neutral, PWM = 4800).
    * `1`: Obroty Normalne (PWM = 4800 + Speed).
    * `2`: Obroty Wsteczne (PWM = 4800 - Speed).
* **Bajt 6 (Moc):** Wartość wejściowa 0-255, skalowana do zakresu timera (0-1600).

---

### Przykładowe Ramki

**Przykład 1: Wszystko STOP**
Zatrzymuje silniki DC i wiertło.
`ID: 0x96` -> `[ 00, 00, 00, 00, 00, 00, 00, 00 ]`

**Przykład 2: Ruch serwa i włączenie mieszadła**
Serwo w prawo o 50 jednostek, Mieszadło CCW z pełną mocą.
* Serwo: Tryb 1, Val 50 -> Hex: `00 32`
* DC: Tryb 1, Speed 255 -> Hex: `FF`
`ID: 0x96` -> `[ 01, 00, 32, 01, FF, 00, 00, 00 ]`

**Przykład 3: Wiertło na 50% mocy**
Serwo bez zmian, DC stop, Wiertło tryb 1, moc ok. 128.
`ID: 0x96` -> `[ 00, 00, 00, 00, 00, 01, 80, 00 ]`
