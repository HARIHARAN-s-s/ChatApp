# zephyr/boards/arm/bgm220p/toolchain.cmake

set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(CROSS_COMPILE ${TOOLCHAIN_ROOT}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
set(CC gcc)  # 🔥 This is the missing piece
message(STATUS "Using compiler: ${CROSS_COMPILE}${CC}")

