/*
 * This library will take care of writing to flash in update mode.
 */

#include "Stdio.h"
#include "stm32f4xx_hal.h"

int SetMode(uint8_t Mode);
enum BOOL_en updateTheFirmware(uint8_t *ptr);
HAL_StatusTypeDef Datawrite(uint8_t *Data , uint32_t Address, uint8_t Size);
void EraseFlash(void);

typedef enum CTRLMode_en
{
	RUN,
	UPDATE
}CTRLMode;

typedef enum BOOL_en
{
	false,
	true,
	close
}BOOL;

typedef enum FWPhase_en
{
	Firmwaredownloadstop,
	Firmwaredownloadstart,
	FirmwaredownloadOnGoing
}FWPhase;

typedef volatile struct INTELHEX
{
	uint8_t		Mode;
	uint8_t 	DataBytesNum;
	uint8_t 	Address_H;
	uint8_t		Address_L;
	uint8_t 	DataCode;
	uint8_t 	Crc;
	uint8_t		Data[16];


}INTEL_HEX_FORMAT;


