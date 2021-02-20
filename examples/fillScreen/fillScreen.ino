/*!
 * @file fillScreen.ino
 * @brief 用一个彩色填充屏幕
 * @n 实验现象：屏幕全屏显示一个彩色
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [wangyanfang]<yanfang.wang@dfrobot.com>
 * @version  V1.0
 * @date  2021-02-19
 * @get from https://www.dfrobot.com
 * @url 
 */
#include <DFRobot_ST7789.h>

// 定义三个引脚的连线PIN脚
// TFT_DC 高，发送数据；低，发送命令
// TFT_CS 片选，拉低则选中，可以通信；拉高不能通信
// TFT_RST 复位引脚
#define TFT_DC    2
#define TFT_CS    3
#define TFT_RST   4

#define RGB_565_RED 0xf800  //红色
#define RGB_565_DARKRED 0x7800  //深红色
#define RGB_565_OLIVE 0x7BE0  //橄榄色

DFRobot_ST7789 st7789;

void setup(void) {
  Serial.begin(115200);
  st7789.st7789Init(TFT_DC, TFT_RST, TFT_CS);
  st7789.fillScreen(RGB_565_RED);//用XX色填充屏幕
}

void loop() {

}

