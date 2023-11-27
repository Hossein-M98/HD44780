idf_component_register(
  SRCS "main.c" "./main.c" "../../../../src/HD44780.c" "../../../../port/ESP32-IDF/HD44780_platform.c"
  INCLUDE_DIRS "../../../../src/include" "../../../../port/ESP32-IDF"
  )