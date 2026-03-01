# Keychron K5 Max ISO RGB (QMK) — Solid background with afterglow effect (VIA)

This repository contains a QMK firmware build for the **Keychron K5 Max** featuring a custom RGB matrix effect.
A **changeable solid background color** with a **reactive afterglow** on keypress. Capslock and Numlock are
kept on if active. 

It is **VIA-enabled**, so you can remap keys and adjust supported lighting parameters through VIA.

---

## Usage

The effect can be selected using the RGB-Mode keys on the keyboard, but it won't show up in the VIA effects list.

- **Hue** controls the background color.
- **Brightness** controls the background intensity.
- **Saturation** controls the color of the afterglow.
- **Speed** controls the *duration* (how long the afterglow persists).

---

## Build

Use `make keychron/k5_max/iso/rgb:via` to build the firmware. 
To flash the firmware, setup your keyboard in cable mode, run `make keychron/k5_max/iso/rgb:via:flash`, disconnect the 
cable and reconnect it while holding ESCAPE.

## VIA

This firmware is built with **VIA support** enabled.

- Use VIA to change key mappings.
- Use your usual QMK/VIA lighting controls to adjust hue/brightness/saturation/speed and see the effect respond.

---

## Notes

- This is a QMK-based firmware intended specifically for the **Keychron K5 Max**.
- If you’re looking for upstream QMK documentation, see: https://github.com/qmk/qmk_firmware
- Base for the Keychron K5 Max firmware: https://github.com/Keychron/qmk_firmware/tree/wireless_playground
---