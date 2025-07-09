# Zephyr Porting for Silicon Labs BGM220P

This repository contains a custom board port for the **Silicon Labs BGM220P** module to run applications using the **Zephyr RTOS**. The BGM220P is based on the EFR32BG22 SoC and is part of the Gecko Bluetooth LE product family.

## 📌 Overview

The goal of this project is to enable Zephyr RTOS support on the BGM220P, allowing development of applications such as `blinky` and other embedded demos directly on the board using Zephyr's build system (`west`).

## ✅ Status

* [x] Custom board definition created
* [x] SoC series and part number integrated
* [x] Device tree files created and verified
* [x] Toolchain detection issue resolved
* [ ] Configuration phase builds successfully
* [ ] Application runtime testing *(in progress)*

## 🏗️ Project Structure

```
zephyrproject/
├── zephyr/
│   ├── boards/arm/bgm220p/
│   │   ├── bgm220p.dts
│   │   ├── bgm220p.dtsi
│   │   ├── pinctrl.dtsi
│   │   ├── bgm220p_defconfig
│   │   ├── board.c
│   │   ├── board.yaml
│   │   ├── Kconfig
│   │   ├── CMakeLists.txt
│   │   └── board.cmake
│   └── soc/silabs/silabs_s2/efr32bg22/
│       ├── Kconfig
│       ├── Kconfig.soc
│       └── Kconfig.defconfig
├── blinky_app/
│   ├── src/main.c
│   └── prj.conf
```

## ⚙️ Build Instructions

Ensure you have the Zephyr SDK installed and the environment set up:

```bash
export ZEPHYR_SDK_INSTALL_DIR=/opt/toolchains/zephyr-sdk-0.17.2
west build -b bgm220p -s blinky_app -d build_blinky --pristine
```

## 🧰 Toolchain Setup

Toolchain path is defined in `toolchain.cmake`:

```cmake
set(CC gcc)
set(CROSS_COMPILE ${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
```

Ensure that:

```bash
echo $ZEPHYR_SDK_INSTALL_DIR
# should output: /opt/toolchains/zephyr-sdk-0.17.2
```

## 🔧 Common Issues & Fixes

### 1. **Dependency Loops in Kconfig**

Avoid using `select` on SoC symbols. Use `default` and `depends on` instead.

### 2. **Invalid Assignment to Hidden Symbols**

Only set `CONFIG_SOC_PART_NUMBER_...` in the defconfig.

### 3. **Toolchain Not Detected**

Ensure:

* `CC` is defined
* `CROSS_COMPILE` has the correct path
* The `arm-zephyr-eabi-gcc` binary exists

## 📄 License

This project is licensed under the [Apache-2.0](LICENSE) License.

## 🙋‍♂️ Maintainer

**Hariharan S**
Email: [hariharansitsme@gmail.com](mailto:hariharansitsme@gmail.com)
GitHub: [HARIHARAN-s-s](https://github.com/HARIHARAN-s-s)

---

This is a custom board port effort for BGM220P in Zephyr RTOS. Contributions or suggestions are welcome.
