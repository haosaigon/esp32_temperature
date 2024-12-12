idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES onewire
                    REQUIRES ds18x20)