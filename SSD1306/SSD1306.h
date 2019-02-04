#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// MCU I2C INTERFACE
// SLAVE_ADDRESS + R/W + ACK + Co + D/C# + Control byte + ACK + Data byte + ACK + Co + D/C# + Control byte + ACK + Data byte + ACK + P
// If the Co bit is set as logic 0, the transmission of the following information will contain data bytes only
// If D/C# determines the next data byte is acted as a command or a data. D/C# bit is set to logic 1, it defines the following data byte
// as a command. If D/C# is set to logic 1, it defines the following data byte as a data which will be stored at the GDDRAM. The GDDRAM
// column address pointer will be increased by one automatically after each data write.

    
// Fundamental command table
#define SET_CONTRAST_CONTROL    0x81

#define SET_ENTIRE_DISPLAY_ON   0xA4 // Resume to RAM content display
#define SET_ENTIRE_DISPLAY_ON_2 0xA5 // Entire display ON, output ignores RAM content

#define SET_NORMAL_DISPLAY      0xA6 // Normal display 0 in RAM: OFF in display panel, 1 in RAM: ON in display panel
#define SET_INVERSE_DISPLAY     0xA7 // Inverse display 0 in RAM: ON in display panel, 1 in RAM: OFF in display panel

#define SET_DISPLAY_OFF         0xAE
#define SET_DISPLAY_ON          0xAF

// Scrolling command table

#define ACTIVATE_SCROLL         0x2E
#define DEACTIVATE_SCROLL       0x2E


    
#define NOP                     0xE3

// Charge pump settings
#define SET_CHARGE_PUMP         0x8D
#define SET_CHARGE_PUMP_ENABLE  0x14
#define SET_CHARGE_PUMP_DISABLE 0x10
    
// Addressing setting command table
#define SET_LOWER_COLUMN_START_ADDRESS // 0x00 ~ 0x0F 0b0000XXXX
#define SET_HIGHER_COLUMN_START_ADDRESS // 0x10 ~ 0x1F 0b0001XXXX
#define SET_MEMORY_ADDRESSING_MODE  0x20
#define SET_COLUMN_ADDRESS          0x21
#define SET_PAGE_ADDRESS 0x22 // AAA (0 - 7d) page start address, BBB (0 - 7d) page end address
#define SET_PAGE_START 0xB0 // 0xB0 ~ 0xB7
// Hardware configuration command table
#define SET_DISPLAY_START_LINE  0x40 // 0x40 | 0bXXXXXX
#define SET_SEGMENT_REMAP0      0xA0 // Column adress 0 is mapped to SEG0
#define SET_SEGMENT_REMAP1      0xA1 // Column address 127 is mapped to SEG0
#define SET_MULTIPLEX_RATIO     0xA8
#define SET_COM_OUTPUT_NORMAL_MODE 0xC0 // Scan from COM0 to COM[N-1]
#define SET_COM_OUTPUT_REMAPPED_MODE 0xC8 // Scan from COM[N-1] to COM0 where N is the MULTIPLEX_RATIO
#define SET_COM_PINS_HW_CONF    0xDA
#define SET_DISPLAY_OFFSET      0xd3
    
// Timing and driving scheme setting command table
#define SET_DISPLAY_CLOCK       0xD5
#define SET_PRECHARGE_PERIOD    0xD9
#define SET_DESELECT_LVL        0xDB

void ssd_init(void);
void ssd_set_column_addr(uint8_t x1, uint8_t x2);
void ssd_set_page_addr(uint8_t y1, uint8_t y2);
void ssd_send_command(uint8_t cmd);
void ssd_send_data(uint8_t data);
void ssd_send_buffer(uint8_t *data, size_t len);
#endif /* SSD1306_H_ */

/* [] END OF FILE */
