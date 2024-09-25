/**
 * @file lv_tft_espi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "display/driver/lv_skdk.h"
#include "display/display_task.h"

#include "LGFX_SKDK.hpp"

#include <LovyanGFX.hpp>

/*********************
 *      DEFINES
 *********************/

#define LV_TICK_PERIOD_MS 1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);

/**********************
 *  STATIC VARIABLES
 **********************/
static LGFX lcd;                 // Instance of LGFX
static LGFX_Sprite sprite(&lcd); // Instance of LGFX_Sprite when using sprites

#define TFT_HOR_RES 240
#define TFT_VER_RES 240

static const uint32_t DISP_BUF_SIZE = ((TFT_HOR_RES * TFT_HOR_RES) * sizeof(uint16_t));

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_skdk_create() {
    lcd.init();
    lcd.initDMA();
    lcd.setRotation(SK_DISPLAY_ROTATION);
    lcd.setBrightness(255);
    lcd.setSwapBytes(true);
    lcd.setColorDepth(16);
}

LGFX *lv_skdk_get_lcd() { return &lcd; }
