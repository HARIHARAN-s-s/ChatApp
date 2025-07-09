# SPDX-License-Identifier: Apache-2.0

zephyr_include_directories(${SOC_FAMILY})
zephyr_include_directories(${SOC_FAMILY}/${SOC_SERIES})

# If you have common code for all Silabs SoCs, you can include it here
# add_subdirectory(common)

# Add subdirectories for this SoC series if needed
# (Uncomment and adapt the following line if you have subfolders)
# add_subdirectory_ifdef(CONFIG_SOC_SERIES_BGM220P bgm220p)
