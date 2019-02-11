#include "project.h"

#include "lvgl.h"

#include "SSD1306.h"
#include "DBG.h"

#define SSD1306_COLS        128
#define SSD1306_PAGES       8
#define SSD1306_COL_SIZE    128

#define SSD1306_VER_RES     64
#define SSD1306_HOR_RES     128

uint8_t ssd_buffer[SSD1306_COLS * SSD1306_PAGES] = {0};

void ssd_set_pixel(uint8_t x, uint8_t y, bool set)
{
    if (set) {
        ssd_buffer[x + ((y / SSD1306_PAGES) * SSD1306_COLS)] |= 1 << (y & 7);        
    } else {
        ssd_buffer[x + ((y / SSD1306_PAGES) * SSD1306_COLS)] &= ~(1 << (y & 7));
    }
}

void ssd_update(void)
{
    ssd_send_buffer(ssd_buffer, sizeof ssd_buffer);
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/

// https://github.com/littlevgl/lv_drivers/issues/29
// https://github.com/littlevgl/lvgl/issues/801
void ssd1306_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    DBG_println("x1: %d, y1: %d, x2: %d, y2: %d, pixel: %s", x1, y1, x2, y2, color_p->full ? "full" : "empty");
    
    if (0 > x2) {
        return;
    }
    
    if (0 > y2) {
        return;
    }
    
    if (SSD1306_HOR_RES - 1 < x1) {
        return;
    }
    
    if (SSD1306_VER_RES - 1 < y1) {
        return;
    }
    
    uint8_t act_x1 = x1 < 0 ? 0 : x1;
    uint8_t act_y1 = y1 < 0 ? 0 : y1;
    uint8_t act_x2 = x2 > SSD1306_HOR_RES - 1 ? SSD1306_HOR_RES - 1 : x2;
    uint8_t act_y2 = y2 > SSD1306_VER_RES - 1 ? SSD1306_VER_RES - 1 : y2;
    
    for (uint8_t y = act_y1; y <= act_y2; y++ ) {
        for (uint8_t x = act_x1; x <= act_x2; x++) {
            DBG_println("x: %d, y: %d, %s", x, y, color_p->full != 0 ? "set" : "clear");
            if (color_p->full != 0) {
                ssd_set_pixel(x, y, 1);
            } else {
                ssd_set_pixel(x, y, 0);
            }
            
            color_p++;
        }
        
        color_p += 128;
    }
    
    ssd_send_buffer(ssd_buffer, sizeof ssd_buffer);
    lv_flush_ready();
}

void timer_handler(void)
{
    lv_tick_inc(1);
    Timer_ReadStatusRegister();
}

static void print_log(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{
    (void) level;
    (void) file;
    (void) line;
    
    DBG_println("%s", dsc);
}

int main(void)
{
    isr_Timer_StartEx(timer_handler);
    
    CyGlobalIntEnable;
    
    UART_Log_Start();
    I2C_Start();
    DBG_clear_screen();
    
    ssd_init();
    
    lv_init();
    lv_log_register_print(print_log);
    
    DBG_println("VDB size in bytes: %d", LV_VDB_SIZE_IN_BYTES);
    
    Timer_Start();

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = ssd1306_flush;
    lv_disp_drv_register(&disp_drv);
    
    /*Create a Label on the currently active screen*/
    lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);
    
    /*Modify the Label's text*/
    lv_label_set_text(label1, "lvgl");

    /* NULL means align on parent (which is the screen now) */
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 5);
    
    for(;;) {
        CyDelay(3);
        lv_task_handler();
    }
}

/* [] END OF FILE */
