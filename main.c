

/**
 * main.c
 */

#include "Port.h"
#include "Priv.h"
#include "utils.h"
#include "DIO.h"
#include "std_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "UART_Config.h"
#include "UART_MemMap.h"
void InitializeArray(u8 * Array,u8 StartIndex,u8 EndIndex);
void delay(u32 max);

void Task1(void * pvParameters);
void Task2(void * pvParameters);
void Task3(void * pvParameters);

TaskHandle_t  First_handle,Second_handle,Third_handle;

TickType_t xLastWakeTime;

u8 RedLedStatusArr[81]={'R','e','d',' ','l','e','d',' ','o','n'};
u8 BlueLedStatusArr[81]={'B','l','u','e',' ','l','e','d',' ','o','n'};
void main(void)
{
    Port_Init();
    Port_SetPinMode(PORTB_PIN0,0x01);
    Port_SetPinMode(PORTB_PIN1,0x01);
    UART_init();
    SET_BIT(UARTCTL(UART1),8);
    SET_BIT(UARTCTL(UART1),9);
    xTaskCreate(Task1, "LED1",128, NULL,2, &First_handle);
    xTaskCreate(Task2, "LED2",128, NULL,1, &Second_handle);
    xTaskCreate(Task3, "main",128, NULL,0, &Third_handle);
    vTaskStartScheduler();
    while(1);

}


void Task1(void *pvParameters)
{
    //xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        Dio_WriteChannel( PORTF_PIN1, 1);
        Dio_WriteChannel( PORTF_PIN2, 0);
        InitializeArray(RedLedStatusArr,11,81);
        UART_enumRequestToSend(UART1,81,RedLedStatusArr);
        vTaskDelay(1000);
    }
}

void Task2(void *pvParameters)
{
    vTaskDelay(500);
    //TickType_t starttime = xLastWakeTime + 400;
    while(1)
    {
        Dio_WriteChannel( PORTF_PIN1, 0);
        Dio_WriteChannel( PORTF_PIN2, 1);
        InitializeArray(BlueLedStatusArr,12,81);
        UART_enumRequestToSend(UART1,81,BlueLedStatusArr);
        vTaskDelay(1000);
    }

}

void Task3(void * pvParameter)
{
    vTaskDelay(550);
    while(1)
    {
        UART_mainFunc ();
        vTaskDelay(1000);
    }
}

void delay(u32 max)
{
    u32 i=0;
    while(i<max) {i++;};
}
void InitializeArray(u8 * Array,u8 StartIndex,u8 EndIndex)
{
    u8 i;
    for(i=StartIndex;i<EndIndex;i++)
    {
        Array[i] = ' ';
    }
}
