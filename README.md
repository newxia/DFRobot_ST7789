# DFRobot_ST7789
DFRobot_ST7789是一个LCD屏幕驱动库。

## 产品链接（链接到英文商城）
    SKU：DFR0649
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

- 画矩形
- 给矩形填充单色
- 给屏幕填充单色

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @brief ST7789初始化
   * @param dc DC引脚号 1：发送数据; 0：发送命令
   * @param rst 复位引脚号
   * @param cs 片选引脚号 1:取消选中无法通信; 0:选中可以通信
   */
  void st7789_init(int8_t dc, int8_t rst, int8_t cs);
  
  /**
   * @brief 用色彩填充全屏幕
   * @param color 填充的颜色 RGB565值
   */
  void  fillScreen(uint16_t color);
  
  /**
   * @brief 通用初始化设定：引脚设定，SPI基础设定
   */
  void  commonInit(void);
  
  /**
   * @brief 写入命令
   * @param c 要写入的命令（寄存器）
   */
  void  writecommand(uint8_t c);
  
  /**
   * @brief 写入数据
   * @param c 要写入的数据
   */
  void  writedata(uint8_t c);
  
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
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 



## History

- data 2021-02-20
- version V0.1


## Credits

Written by wangyanfangr(yanfang.wang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
