set(TOOLCHAIN_ROOT $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(CROSS_COMPILE ${TOOLCHAIN_ROOT}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
set(CC gcc)  # <-- THIS LINE FIXES THE ERROR
