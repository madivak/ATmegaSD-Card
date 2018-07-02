/*
 * SD_Card_ATmega1284p.c
 *
 * Created: 7/2/2018 4:19:22 PM
 * Author : madiv
 */ 

#include <avr/io.h>
#include "sdcard_raw.h"
#define CHOSEN_SECTOR 0
#define BUFFER_SIZE 10
int main(void)
{
	/* LED for result feedback PA3 (active High) */
	DDRA |= (1 << 3);
	PORTA |= (1 << 3);
	_delay_us(1000);
	
	/* Max clock frequency 8MHz (prescaler 0) */
	CLKPR = (1 << CLKPCE);	/* Enabled clock prescaler change */
	CLKPR = 0; /* Write prescaler to 0 */
	/* Initialize SD card */
	DSTATUS status;
	do{
		status = disk_initialize();
	} while (status);
	/* Write something */
	uint8_t write_from_here[BUFFER_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	disk_write_start(CHOSEN_SECTOR);
	disk_write_continue(write_from_here, BUFFER_SIZE);
	disk_write_stop();
	/* Read back */
	uint8_t read_to_here[BUFFER_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	disk_read_start(CHOSEN_SECTOR);
	disk_read_continue(read_to_here, BUFFER_SIZE);
	disk_read_stop();
	/* Break anywhere after here to inspect buffers */
	
	/* Compare buffers */
	status = 1;
	uint8_t i;
	for(i = 0; i < BUFFER_SIZE; i++){
		if(read_to_here[i] != write_from_here[i]){
			status = 0;
		}
	}
	/* If success, turn Off LED */
	if(status){
		PORTA &= ~(1 << 3);
	}

	while (1) {}
}

