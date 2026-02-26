# STM32WL33C_radio_drivers

This folder contains the Hardware Abstraction Layer (HAL) and custom primitives for managing wireless communication on the **STM32WL33C** platform. The system implements a dual-radio architecture: a high-speed main radio for data and an ultra-low-power receiver for remote wake-up.

## 📌 System Architecture
The driver is organized into three main components:
1.  **Primitives (`primitives.h`)**: An abstraction layer that coordinates both radio modules for high-level operations.
2.  **LPAWUR (`mw_lpawur.h`)**: Management of the *Low Power Autonomous Wake-up Receiver*.
3.  **MRSUBG (`my_mrsubg.h`)**: Management of the *Multi-Radio Sub-GHz* main radio, used for sending Wake-up signals (OOK) and standard data exchange (2FSK).

---

## 🛠 Main Primitives
The functions defined in `my_primitives.h` serve as main radio primitives:

| Function | Description |
| :--- | :--- |
| `send_wakeup()` | Configures the MRSUBG in OOK mode to send an activation packet to a remote node. |
| `recv_wakeup()` | Sets the LPAWUR to listen mode to receive a wake-up payload. |
| `send_data()` | Sends a standard data packet using 2FSK modulation. |
| `recv_main_radio()` | Opens a reception window on the main radio with a defined timeout. |

---

## ⚙️ Default Configurations

### Main Radio (MRSUBG)
* **Data Mode:** 868 MHz, 2FSK.
* **Wake-up Mode:** 900 MHz, OOK.

### Wake-up Receiver (LPAWUR)
* **Sync Pattern:** Variable length (8 or 16 bits) via `frame_sync_t`.

---

## MRSUBG timer example
The repository also contains a small example on how to use the MRSUBG radio timer to allow direct wake up of the system using the main radio when in DEEPSTOP mode, this technique could prove useful in a number of applications and was used during the board analysis
