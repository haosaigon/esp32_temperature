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
    if(onewire_reset(DS18B20_GPIO))
		printf("Devices detected\n");
    else
		printf("NO Devices detected\n");
	do {
		status = ds18x20_scan_devices(DS18B20_GPIO, addrs, MAX_SENSORS,&sensor_count);
        if(sensor_count < 1)
        {
			ESP_LOGW(TAG, "No DS18x20 sensors detected!");
            ESP_LOGW(TAG, "status: %04x", (unsigned int)status);
            vTaskDelay(pdMS_TO_TICKS(1000));
		}
	} while (sensor_count < 1);
    // now there is at least one sensor detected
	while(1) {
		// Get data
		float temperature = read_temperature();
		        // Log the temperature
        if (temperature != -127.0) {
            ESP_LOGI(TAG, "Temperature (°C): %.2f", temperature);
		}
        // ...fill in here
        // Wait 2 sec before next measurement
        vTaskDelay(pdMS_TO_TICKS(2000));
	}
}
