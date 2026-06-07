# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c ui_manager.c wifi_app.c http_client.c esp_now_app.c
    INCLUDE_DIRS "."      # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES esp_driver_gpio esp_wifi nvs_flash esp_http_client esp-tls   # optional, list the private requirements
)
