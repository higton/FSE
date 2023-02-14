#ifndef NVS_HANDLE_H
#define NVS_HANDLE_H

#include "nvs.h"
#include "nvs_flash.h"

int32_t read_nvs_value(char *atributo);
void write_value_to_nvs(char *atributo, int32_t valor);

#endif