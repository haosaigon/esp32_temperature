#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "onewire.h"
#include "ds18x20.h"

#define TAG "DS18X20"

// GPIO where the DS18B20 is connected
#define DS18B20_GPIO 8

// Maximum number of devices expected on the bus
#define MAX_SENSORS 8

void app_main(void)
{
    // Array for storing detected sensor addresses
    ds18x20_addr_t addrs[MAX_SENSORS];
    size_t sensor_count = 0;
    esp_err_t status;

    // Scan for sensors on the bus
    ESP_LOGI(TAG, "Scanning for DS18x20 sensors...");
    
    if (onewire_reset(DS18B20_GPIO)) {
        ESP_LOGI(TAG, "OneWire bus reset successful. Devices detected.");
    } else {
        ESP_LOGE(TAG, "No devices detected on the OneWire bus!");
        return;  // Exit if no devices are detected
    }

    do {
        // Scan for devices on the bus
        status = ds18x20_scan_devices(DS18B20_GPIO, addrs, MAX_SENSORS, &sensor_count);
        if (sensor_count < 1) {
            ESP_LOGW(TAG, "No DS18x20 sensors detected!");
            ESP_LOGW(TAG, "Status: %04x", (unsigned int)status);
            vTaskDelay(pdMS_TO_TICKS(1000));  // Retry after 1 second
        }
    } while (sensor_count < 1);

    ESP_LOGI(TAG, "Found %d DS18x20 sensor(s).", sensor_count);

    while (1) {
        // Loop through all detected sensors
        for (size_t i = 0; i < sensor_count; i++) {
            float temperature;
            // Read temperature from the sensor
            status = ds18x20_measure_and_read(DS18B20_GPIO, addrs[i], &temperature);
            if (status == ESP_OK) {
                ESP_LOGI(TAG, "Sensor %d Temperature (°C): %.2f", i, temperature);
            } else {
                ESP_LOGE(TAG, "Error reading from sensor %d. Status: %04x", i, (unsigned int)status);
            }
        }
        // Wait 2 seconds before the next reading
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
