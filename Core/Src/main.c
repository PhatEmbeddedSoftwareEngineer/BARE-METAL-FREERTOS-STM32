#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

#define RCC_BASE_ADDR			0x40023800
#define GPIOD_BASE_ADDR			0x40020C00


static void functionBlueLed(void *pvParameters);
static void functionRedLed(void *pvParameters);

int main(void)
{
	uint32_t *RCC_AHB1ENR = (uint32_t*)(RCC_BASE_ADDR + 0x30);
	*RCC_AHB1ENR |= (1U << 3);

	uint32_t *GPIOD_MODER = (uint32_t*)(GPIOD_BASE_ADDR);
	*GPIOD_MODER |= (0b01 << 30) | (0b01 << 28);

	int8_t status;
	status = xTaskCreate(functionBlueLed,"Blue",configMINIMAL_STACK_SIZE,NULL,configMAX_PRIORITIES-1,NULL);
	if(status == -1)
	{
		while(1);
	}
	status = xTaskCreate(functionRedLed,"Red",configMINIMAL_STACK_SIZE,NULL,configMAX_PRIORITIES-2,NULL);
	if(status == -1)
	{
		while(1);
	}
	vTaskStartScheduler();
	while(1);
}


static void functionBlueLed(void *pvParameters)
{
	uint32_t *GPIOD_ODR = (uint32_t*)(GPIOD_BASE_ADDR + 0x14);

	while(1)
	{
		*GPIOD_ODR ^= (1U << 15);
		vTaskDelay(500);
	}
}

static void functionRedLed(void *pvParameters)
{
	uint32_t *GPIOD_ODR = (uint32_t*)(GPIOD_BASE_ADDR + 0x14);

	while(1)
	{
		*GPIOD_ODR ^= (1U << 14);
		vTaskDelay(1000);
	}
}
