/**
*@file DFRobot_ST7789.h
*@brief Define the infrastructure for the DFRobot_ST7789 class
*@n The functions are as follows:
*@n Function 1: set addr Window
*@n Function 2: fill rectangle
*@n Function 3: fill screen with one color
*@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
*@SKU DFRXXXX
*@licence The MIT License (MIT)
*@author [wangyanfang]<yanfang.wang@dfrobot.com>
*@version V1.0
*@date 2021-02-19
*@url  
*/ 
#ifndef _DFROBOT_ST7789_H_
#define _DFROBOT_ST7789_H_

//做兼容，版本号不同则希望导入不同的库
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//根据芯片手册与该屏幕的引出引脚，该驱动采用标准4线SPI，可直接使用标准SPI库
#include <SPI.h>


class DFRobot_ST7789{
public:
  // 定义三个引脚的连线PIN脚
  // TFT_DC 高，发送数据；低，发送命令
  // TFT_CS 片选，拉低则选中，可以通信；拉高不能通信
  // TFT_RST 复位引脚
  #define TFT_DC    2
  #define TFT_CS    3
  #define TFT_RST   4
  
  // 定义屏幕的宽和高（尺寸）
  #define ST7789_TFT_WIDTH   240
  #define ST7789_TFT_HEIGHT  240
  
  // 定义屏幕的起始点坐标
  #define ST7789_240x240_XSTART 0 //起始点横坐标
  #define ST7789_240x240_YSTART 0 //起始点纵坐标
  
  #define ST_CMD_DELAY   0x80  //
  
  // 指令的寄存器定义，参考芯片手册第9章
  #define ST7789_NOP     0x00  // empty command 空指令
  #define ST7789_SWRESET 0x01  // Software Reset 软复位 
  #define ST7789_RDDID   0x04  // Read Display ID 读取显示id 
  #define ST7789_RDDST   0x09  // Read Display Status 读取显示状态 
  
  #define ST7789_SLPIN   0x10  // Sleep in 睡眠模式 
  #define ST7789_SLPOUT  0x11  // Sleep Out 退出睡眠模式
  #define ST7789_PTLON   0x12  // Partial Display Mode On 部分显示模式开启
  #define ST7789_NORON   0x13  // Normal Display Mode On 正常显示模式开启
  
  #define ST7789_INVOFF  0x20  // Display Inversion 显示反转关闭
  #define ST7789_INVON   0x21  // Display Inversion 显示反转开启
  #define ST7789_DISPOFF 0x28  // Display 显示关闭
  #define ST7789_DISPON  0x29  // Display 显示开启
  #define ST7789_CASET   0x2A  // Column Address Set 设置列地址 
  #define ST7789_RASET   0x2B  // Row Address Set 设置行地址 
  #define ST7789_RAMWR   0x2C  // RAM Write 内存写入 
  #define ST7789_RAMRD   0x2E  // RAM Read 内存读取（此例程未使用）
  
  #define ST7789_PTLAR   0x30  // Partial Area 局部区域 
  #define ST7789_COLMOD  0x3A  // Set color mode 颜色模式
  
  // Memory Data Access Control内存数据访问控制，通过该寄存器可以设定图像的刷新方式
  #define ST7789_MADCTL  0x36     
  #define ST7789_MADCTL_MY  0x80  //D7:1000-0000 页面地址顺序：底到顶,相应位上为1
  #define ST7789_MADCTL_MX  0x40  //D6:0100-0000 右边到左边,相应位上为1
  #define ST7789_MADCTL_MV  0x20  //D5:0010-0000 翻转模式,相应位上为1
  #define ST7789_MADCTL_ML  0x10  //D4:0001-0000 线地址顺序：底到顶,相应位上为1
  #define ST7789_MADCTL_RGB 0x00  //D3:0000-0000 选定的RGB模式
  
  
  #define RGB_565_RED 0xf800  //红色

public:
  /**
   * @brief  构造函数
   */
  DFRobot_ST7789();
  
  
  /**
   * @brief ST7789初始化
   * @param dc  DC引脚号   1：发送数据; 0：发送命令
   * @param rst 复位引脚号 复位引脚先拉低再拉高完成一个上升沿即可成功复位，即先置0再置1
   * @param cs  片选引脚号 1:取消选中无法通信; 0:选中可以通信
   */
  void st7789Init(int8_t dc, int8_t rst, int8_t cs);
  
  /**
   * @brief 用色彩填充全屏幕
   * @param color 填充的颜色 RGB565值
   */
  void  fillScreen(uint16_t color);
  
protected:
  
  /**
   * @brief 通用初始化设定：引脚设定，SPI基础设定
   */
  void  commonInit(void);
  
  /**
   * @brief 写入命令
   * @param c 要写入的命令（寄存器）
   */
  void  writeCommand(uint8_t c);
  
  /**
   * @brief 写入数据
   * @param c 要写入的数据
   */
  void  writeData(uint8_t c);
  
  /**
   * @brief 初始化显示屏幕
   * @param addr 命令集
   */
  void  displayInit(const uint8_t *addr);
  
  
  /**
   * @brief 设置显示窗的位置及大小
   * @param x0 矩形窗起始点横坐标
   * @param y0 矩形窗起始点纵坐标
   * @param x1 矩形窗结束点横坐标
   * @param y1 矩形窗结束点纵坐标
   */
  void  setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1);
  
  /**
   * @brief 用色彩填充矩形
   * @param x 矩形窗起始点横坐标
   * @param y 矩形窗起始点纵坐标
   * @param w 矩形窗的宽
   * @param h 矩形窗的高
   * @param color 填充的颜色 RGB565值
   */
  void  fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
  
private:
  //类的实例化，这里只用到了初始化设定几个值
  SPISettings mySPISettings;
  
  uint8_t  _colstart, _rowstart, _xstart, _ystart; // some displays need this changed
  int8_t  _cs, _dc, _rst;
  uint16_t rotation, _width, _height;
  
};
#endif