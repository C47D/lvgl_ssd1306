#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "project.h"

#include "SSD1306.h"

#define SSD1306_SLAVE_ADDR  0x3C

void ssd_init(void)
{
    CyDelay(10);
    
    // ssd1306_display_on(dev, false)
    ssd_send_command(SET_DISPLAY_OFF);
    // ssd1306_set_osc_freq(dev, 0x80)
    ssd_send_command(SET_DISPLAY_CLOCK);
    ssd_send_command(0x80);
    // ssd1306_set_mux_ratio(dev, dev->height - 1)
    ssd_send_command(SET_MULTIPLEX_RATIO);
    ssd_send_command(63);
    // ssd1306_set_display_offset(dev, 0x00)
    ssd_send_command(SET_DISPLAY_OFFSET);
    ssd_send_command(0x00);
    // ssd1306_set_display_start_line(dev, 0x00)
    ssd_send_command(SET_DISPLAY_START_LINE | 0x00);
    // ssd1306_set_charge_pump_enabled(dev, true)
    ssd_send_command(SET_CHARGE_PUMP);
    ssd_send_command(SET_CHARGE_PUMP_ENABLE);
    // ssd1306_set_mem_addr_mode(dev, SSD_ADDR_MODE_HORIZONTAL)
    ssd_send_command(SET_MEMORY_ADDRESSING_MODE);
    ssd_send_command(0x00); // 0x00 horizontal, 0x01 vertical, 0x10 page addressing mode, 0x11 invalid
    
    /* Para empezar a escribir desde lo amarillo debemos de voltear los segmentos y columnas */
    ssd_send_command(SET_SEGMENT_REMAP1);
    ssd_send_command(SET_COM_OUTPUT_REMAPPED_MODE);
    
    /* Para empezar normal */
    // ssd_send_command(SET_SEGMENT_REMAP0);
    // ssd_send_command(SET_COM_OUTPUT_NORMAL_MODE);
    
    // ssd1306_set_com_pin_hw_config(dev, pin_cfg)
    ssd_send_command(SET_COM_PINS_HW_CONF);
    ssd_send_command(0x12); // alternative com configuration (RESET)
    // ssd1306_set_contrast(dev, 0x9F)
    ssd_send_command(SET_CONTRAST_CONTROL);
    ssd_send_command(0x0f);
    // ssd1306_set_precharge_period(dev, 0xF1)
    ssd_send_command(SET_PRECHARGE_PERIOD);
    ssd_send_command(0xf1);
    // ssd1306_set_deseltct_lvl(dev, 0x40)
    ssd_send_command(SET_DESELECT_LVL);
    ssd_send_command(0x40);
    // ssd1306_set_whole_display_lighting(dev, true)
    ssd_send_command(SET_ENTIRE_DISPLAY_ON);
    // ssd1306_set_inversion(dev, false)
    ssd_send_command(SET_NORMAL_DISPLAY);
    
    ssd_set_column_addr(0, 127);
    ssd_set_page_addr(0, 7);
    
    ssd_send_command(SET_DISPLAY_ON);
    
    const uint8_t zero_buffer[128 * 8] = {0};
    ssd_send_buffer(zero_buffer, sizeof zero_buffer);
}

void ssd_set_column_addr(uint8_t x1, uint8_t x2)
{
    ssd_send_command(SET_COLUMN_ADDRESS);
    ssd_send_command(x1);
    ssd_send_command(x2);
}

void ssd_set_page_addr(uint8_t y1, uint8_t y2)
{
    ssd_send_command(SET_PAGE_ADDRESS);
    ssd_send_command(y1);
    ssd_send_command(y2);
}

void ssd_send_command(uint8_t cmd)
{
    uint8_t dummy[] = {
        0x00, cmd
    };
    I2C_MasterClearStatus();
    I2C_MasterWriteBuf(SSD1306_SLAVE_ADDR, dummy, sizeof dummy, I2C_MODE_COMPLETE_XFER);
    while (I2C_MSTAT_WR_CMPLT != I2C_MasterStatus()) {
        CyDelay(1);
    }
}

void ssd_send_data(uint8_t data)
{
    uint8_t dummy[] = {
        0x40, data
    };
    I2C_MasterClearStatus();
    I2C_MasterWriteBuf(SSD1306_SLAVE_ADDR, dummy, sizeof dummy, I2C_MODE_COMPLETE_XFER);
    while (I2C_MSTAT_WR_CMPLT != I2C_MasterStatus()) {
        CyDelay(1);
    }
}

void ssd_send_buffer(uint8_t *data, size_t len)
{
#if 0
    for (size_t i = 0; i < len; i++) {
        ssd_send_data(data[i]);
    }
#else
    uint8_t dummy[len + 1];
    dummy[0] = 0x40;
    memcpy(&dummy[1], data, len);
    I2C_MasterClearStatus();
    I2C_MasterWriteBuf(SSD1306_SLAVE_ADDR, dummy, sizeof dummy, I2C_MODE_COMPLETE_XFER);
    while (I2C_MSTAT_WR_CMPLT != I2C_MasterStatus()) {
        CyDelay(1);
    }
#endif
}

/* [] END OF FILE */
