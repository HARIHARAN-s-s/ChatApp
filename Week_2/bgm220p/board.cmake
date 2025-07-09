#board_runner_args(jlink "--device=EFR32BG22C224F512IM40" "--reset-after-load")
#include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
# Set the architecture and toolchain
set(ARCH arm)
set(SOC_SERIES efr32bg22)
set(SOC efr32bg22c224f512im40)
set(COMPILER zephyr)  # Use Zephyr SDK

# Optional: set TOOLCHAIN_ROOT explicitly (not always needed)
# set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(CROSS_COMPILE ${TOOLCHAIN_ROOT}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
# toolchain-override.cmake
#set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
#set(CROSS_COMPILE ${TOOLCHAIN_ROOT}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
set(CC gcc)  # <-- THIS LINE FIXES THE ERROR


# J-Link debug settings
board_runner_args(jlink "--device=EFR32BG22C224F512IM40" "--reset-after-load")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)

