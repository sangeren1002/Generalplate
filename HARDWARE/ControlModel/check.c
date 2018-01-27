#include "check.h"

uint16_t CalcFieldCRC(uint16_t* pDataArray, uint16_t numberOfWords)
{
	uint16_t shifter, c;
	uint16_t carry;
	uint16_t CRC_MAXON=0;
	//Calculate pDataArray Word by Word
	while(numberOfWords--)
	{
		shifter = 0x8000;
		c = *pDataArray++;
		do
		{
			carry = CRC_MAXON & 0x8000;
			CRC_MAXON <<= 1;
			if(c & shifter) 
				CRC_MAXON++;
			if(carry)
				CRC_MAXON ^= 0x1021;
			shifter >>=1;
		} 
		while(shifter);
	}
		return CRC_MAXON;
}

uint8_t BccCheckSum(uint8_t buf[], uint8_t lenth)
{
	uint8_t checksum;
	uint16_t i;
	for( i=0; i < lenth; i++)
	{
		checksum ^= buf[i];
	}
	
	return checksum;
}

uint16_t UART_CRC16_Work(uint8_t *CRC_Buf,uint8_t CRC_Leni)
{
  uint8_t i,j;
  uint16_t CRC_Sumx;
	
  CRC_Sumx = 0xFFFF;
	
  for(i=0; i<CRC_Leni; i++)
	{
		CRC_Sumx ^= *(CRC_Buf+i);
		
		for(j=0;j<8;j++)
		{
			if(CRC_Sumx & 0x01)
			{
				CRC_Sumx >>= 1;	
				CRC_Sumx ^= 0xA001;
			}
			else
			{
				CRC_Sumx >>= 1;
			}
		}	
	}
  return (CRC_Sumx);
}




