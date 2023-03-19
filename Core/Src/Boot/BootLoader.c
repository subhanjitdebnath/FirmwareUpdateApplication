#include "BootLoader.h"

static uint32_t FlashingAddress=0x08020000;

int SetMode(uint8_t Mode)
{
	int ModeSel=0;
	if(Mode==1)
	{
		ModeSel=1;
		printf("RUN MODE \n\r");
	}
	else if (Mode==2)
	{
		ModeSel=2;
		printf("UPDATE MODE \n\r");
	}
	else
	printf("Command Not Recognized");

	return ModeSel;
}

enum BOOL_en updateTheFirmware(uint8_t *ptr)
{

	BOOL Status=true;
	HAL_StatusTypeDef FlashState;
	INTEL_HEX_FORMAT *RxFram = (INTEL_HEX_FORMAT*)ptr;

	uint32_t Address=0,Address_H=0,Address_L=0;

	if(RxFram->DataCode==4)
	{
		EraseFlash();
		Status=true;
		HAL_FLASH_Unlock();
	}
	else if(RxFram->DataCode==0)
	{
		Address_H=(uint32_t)(RxFram->Address_H);
		Address_L=(uint32_t)(RxFram->Address_L);
		Address = FlashingAddress+(Address_H<<8)+ Address_L;
		FlashState=Datawrite( RxFram->Data ,Address , RxFram->DataBytesNum);
		while(FlashState!=HAL_OK)
		{
			if(FlashState==HAL_BUSY || FlashState==HAL_TIMEOUT)
					{
						FlashState=Datawrite( RxFram->Data ,Address , RxFram->DataBytesNum);
						Status=true;
					}
			else if(FlashState==HAL_ERROR)
					{
						printf("Flash Error \r\n");
						Status=false;
						break;
					}
			else
					{
						Status=true;
						break;
					}
		}

	}
	else if(RxFram->DataCode==5)
	{
		HAL_FLASH_Lock();
		Status=close;
	}
	else
		Status=false;

	return Status;

}

HAL_StatusTypeDef Datawrite(uint8_t *Data , uint32_t Address, uint8_t Size)
{
  HAL_StatusTypeDef status;
  HAL_FLASH_Unlock();
  for(int i=0;i<Size;i++)
  {
	  status=HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, Data[i]);
	  Address++;
  }
  //HAL_FLASH_Lock();
  return status;
}

void EraseFlash(void)
{
	FLASH_EraseInitTypeDef FlashEraseDefination;
	uint32_t FlashEraseFault=0;
	HAL_FLASH_Unlock();

	FlashEraseDefination.TypeErase = FLASH_TYPEERASE_SECTORS;
	FlashEraseDefination.Banks= FLASH_BANK_1;
	FlashEraseDefination.NbSectors=2;
	FlashEraseDefination.Sector=FLASH_SECTOR_5;
	FlashEraseDefination.VoltageRange=FLASH_VOLTAGE_RANGE_3;

	HAL_FLASHEx_Erase(&FlashEraseDefination, &FlashEraseFault);

	HAL_FLASH_Lock();

}
