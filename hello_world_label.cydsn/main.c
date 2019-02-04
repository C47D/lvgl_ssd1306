#include "project.h"

#include "lvgl.h"

#include "SSD1306.h"
#include "DBG.h"

/*
 * WELCOME TO THE LITTLEVGL TUTORIAL
 *
 *-----------------------------------------------------------------------------
 * Now you will learn how to port the Littlev Graphics Library to your system
 * ----------------------------------------------------------------------------
 *
 *Here are some info about the drawing modes in the library:
 *
 * BUFFERED DRAWING
 * The graphics library works with an internal buffering mechanism to
 * create advances graphics features with only one frame buffer.
 * The internal buffer is called VDB (Virtual Display Buffer) and its size can be adjusted in lv_conf.h.
 * When LV_VDB_SIZE not zero then the internal buffering is used and you have to provide a function
 * which flushes the buffers content to your display.
 *
 * UNBUFFERED DRAWING
 * It is possible to draw directly to a frame buffer when the internal buffering is disabled (LV_VDB_SIZE = 0).
 * Keep in mind this way during refresh some artifacts can be visible because the layers are drawn after each other.
 * And some high level graphics features like anti aliasing, opacity or shadows aren't available in this configuration.
 * In this mode two functions are required:  fill and area with a color AND write a color array to an area
 *
 * HARDWARE ACCELERATION (GPU)
 * If your MCU supports graphical acceleration (GPU) then you can use it with two interface functions:
 * blend two memories using opacity AND fill a memory with a color.
 * Use only one function or none of them is right as well because every GPU function is optional
 */

static int inline _load_frame_buffer(uint8_t *buf, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    // Convert to page
    y1 = y1 >> 3; // >> 3 es lo mismo que dividir entre 8
    y2 = y2 >> 3; // ditto, asi pasamos de coord 64 a pagina 8 (7, porque las paginas empiezan en 0)
    
    ssd_set_column_addr(x1, x2);
    ssd_set_page_addr(y1, y2);
    
    // https://github.com/littlevgl/lv_drivers/issues/29
    // https://github.com/littlevgl/lv_drivers/issues/29
    
    const uint16_t col_step = 16;
#if 0
    uint8_t col_len = (x2 - x1) + 1;
    uint8_t page_len = (y2 - y1) + 1;
    
    // Iteramos las paginas
    for (uint8_t page = 0; page < page_len; page++) {
        // Mandamos el contenido de las columnas
        for (uint8_t col = 0; col < col_len; /* col += col_step */) {
            if((col + col_step) <= col_len) {
                uint8_t *data = &buf[page * col_len + col];
                ssd_send_buffer(data, col_step);
                col += col_step;
            } else {
                // Last data for this page
                uint8_t *data = &buf[page * col_len + col];
                ssd_send_buffer(data, col_len - col);
                break;
            }
        }
    }
#else
    uint8_t col_len = x2 - x1;
    uint8_t page_len = y2 - y1;
    lv_coord_t area_w = x2 - x1 + 1;
    
    // Iteramos las paginas
    for (uint8_t page = 0; page < page_len; page++) {
        // Mandamos el contenido de las columnas
        for (uint8_t col = 0; col < col_len; col++) {
            uint8_t data = buf[page * area_w + col];
            ssd_send_data(data);
        }
    }
#endif

    return 0;
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
void ssd1306_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /* Return if the area is out the screen */
    if ((x2 < 0) || (y2 < 0) || (x1 > LV_HOR_RES - 1) || (y1 > (LV_VER_RES - 1))) {
        lv_flush_ready();
        return;
    }
    
    _load_frame_buffer((uint8_t *) color_p, x1, y1, x2, y2);
    
    lv_flush_ready();
}

void timer_handler(void)
{
    lv_tick_inc(5);
    
    Timer_ReadStatusRegister();
}

static void print_log(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
/*
#define LV_LOG_LEVEL_TRACE 0
#define LV_LOG_LEVEL_INFO  1
#define LV_LOG_LEVEL_WARN  2
#define LV_LOG_LEVEL_ERROR 3
#define _LV_LOG_LEVEL_NUM  4
*/
    (void) level;
    
    DBG_println("%s : %s : %s", file, line, dsc);
}

int main(void)
{
    isr_Timer_StartEx(timer_handler);
    
    CyGlobalIntEnable;
    
    UART_Log_Start();
    I2C_Start();
    DBG_clear_screen();
    
    lv_init();
    
    lv_log_register_print(print_log);
    
    ssd_init();
    
    Timer_Start();

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = ssd1306_flush;
    lv_disp_drv_register(&disp_drv);
    
#if 0
    /*Create a Label on the currently active screen*/
    lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);
    
    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello world!");

    /* NULL means align on parent (which is the screen now) */
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
#endif

    for(;;) {
        lv_task_handler();
        CyDelay(5);
    }
}

/* [] END OF FILE */
