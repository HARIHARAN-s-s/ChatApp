# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(board.c)
# this line tells Zephyr to pick up <board>_defconfig automatically
zephyr_board_defconfig(bgm220p_defconfig)
set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(CROSS_COMPILE ${TOOLCHAIN_ROOT}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)

board_runner_args(jlink "--device=EFR32BG22C224F512IM40" "--reset-after-load")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)


