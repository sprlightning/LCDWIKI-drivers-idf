# -- CMAKE for Espressif IDF
# -- author MLX
# -- copyright GPLv3

idf_component_register(
    SRC_DIRS LCDWIKI_GUI LCDWIKI_SPI Fonts
    INCLUDE_DIRS "." LCDWIKI_GUI LCDWIKI_SPI Fonts
    REQUIRES esp_common freertos hal log nvs_flash driver esp_timer arduino-audio-tools
)

target_compile_options(${COMPONENT_LIB} PUBLIC -DESP32_CMAKE=1 -Wno-error -Wno-format -fpermissive -Wno-narrowing)
