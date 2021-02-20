/**
*@file DFRobot_ST7789.cpp
*@brief Define the DFRobot_ST7789 class infrastructure, the implementation of the underlying methods
*@copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
*@SKU DFRXXXX
*@licence The MIT License (MIT)
*@author [wangyanfang]<yanfang.wang@dfrobot.com>
*@version V1.0
*@date 2021-02-19
*@url  
*/

#include <DFRobot_ST7789.h>

// Program Space Utilities程序空间利用，使用PROGMEM关键字表示存储这个数组到Flash储存器而不是SRAM，flash里的东西一般不改变，加const      
// 7789屏幕的初始化命令
// 有一些命令不需要延迟却加上短延迟的原因是为了后面轮询结构的工整
// 因为它既没有参数也没有延迟，第二项就会空出，无法做判断，直接跳到下一个命令
static const uint8_t PROGMEM cmd_240x240[35] = {
    9,                                 // 列表中的9个命令：
  ST7789_SWRESET,   ST_CMD_DELAY,     // 1: Software reset, no args, w/delay
    150,           // 150 ms delay
  ST7789_SLPOUT ,   ST_CMD_DELAY,     // 2: Out of sleep mode, no args, w/delay
    255,           // 255 = 500 ms delay
  ST7789_COLMOD , 1 + ST_CMD_DELAY,   // 3: Set color mode, 1 arg + delay:
    0x55,          // 0x55:0101-0101 选中16bit/pixel 65k of RGB interface,16-bit color，对应RGB565
    10,            // 10 ms delay
  ST7789_MADCTL , 1,                  // 4: Memory date access ctrl (directions), 1 arg:
    0x00,          // 上下翻页，左到右，上到下，普通模式，RGB，左到右的方式刷新；该指令无时间限制
  ST7789_CASET  , 4,                  // 5: Column addr set, 4 args, no delay 如果收到RAMWR命令，就参考第2和第4个参数
    0x00, ST7789_240x240_XSTART,             // XSTART = 0
    (ST7789_TFT_WIDTH + ST7789_240x240_XSTART) >> 8,    // 0
    (ST7789_TFT_WIDTH + ST7789_240x240_XSTART) & 0xFF,  // 运算后0xf0 = 240
  ST7789_RASET  , 4,                  // 6: Row addr set, 4 args, no delay 如果收到RAMWR命令，就参考第1和第3个参数
    0x00, ST7789_240x240_YSTART,                       // YSTART = 0
    (ST7789_TFT_HEIGHT + ST7789_240x240_YSTART) >> 8,   //0x00
    (ST7789_TFT_HEIGHT + ST7789_240x240_YSTART) & 0xFF, // 运算后0xf0 = 240
  ST7789_INVON  ,   ST_CMD_DELAY,     // 7: Inversion ON
    10,           // 10 ms delay
  ST7789_NORON  ,   ST_CMD_DELAY,     // 8: Normal display on, no args, w/delay
    10,           // 10 ms delay
  ST7789_DISPON ,   ST_CMD_DELAY,     // 9: Main screen turn on, no args, w/delay
    255           // 255 = 500 ms delay
}; 

DFRobot_ST7789::DFRobot_ST7789()
{
}

void DFRobot_ST7789::st7789Init(int8_t dc, int8_t rst, int8_t cs)
{
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  
  _colstart = ST7789_240x240_XSTART;
  _rowstart = ST7789_240x240_YSTART;
  _height = 240;
  _width = 240;
  
  commonInit();//引脚设定，SPI初始设定
  displayInit(cmd_240x240);//初始化屏幕，调用flash里的默认屏幕设置数据
  
  writeData(ST7789_MADCTL_RGB);//0x00，表示颜色模式读取选定的为RGB
  
  _xstart = _colstart;
  _ystart = _rowstart;
}

void  DFRobot_ST7789::fillScreen(uint16_t color)
{
  fillRect(0, 0,  _width, _height, color);//全屏填充
}

void  DFRobot_ST7789::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color)
{
  if ((x >= _width) || (y >= _height)) return; //不填充
  if ((x + w - 1) >= _width)  w = _width  - x; //调整填充的矩形宽，确保矩形在屏幕/窗内
  if ((y + h - 1) >= _height) h = _height - y; //调整填充的矩形高，确保矩形在屏幕/窗内

  setAddrWindow(x, y, x + w - 1, y + h - 1); //视窗矩形大小及位置

  uint8_t hi = color >> 8, lo = color; ;//红色RGB转HEX：0xf800 1111-1000-0000-0000;hi:1111-1000;lo:1111-1000-0000-0000

  SPI.beginTransaction(mySPISettings); //开始通信

  digitalWrite(_dc, HIGH); //DC(1)写数据
  digitalWrite(_cs, LOW);  //片选拉低，选中状态
  
  //满屏填充色彩,transfer()只能传8位，从高到低传，以红色为例0xf800
  for (y = h; y > 0; y--) {
    for (x = w; x > 0; x--) {
      SPI.transfer(hi);
      SPI.transfer(lo);
    }
  }
  digitalWrite(_cs, HIGH);//片选拉高，取消选中
  SPI.endTransaction();//结束通信
}

void  DFRobot_ST7789::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                    uint16_t y1) {

  writeCommand(ST7789_CASET);  //写入命令，设置列地址
  writeData(x0 >> 8);          //0x00
  writeData(x0 & 0xFF);        //与后还是X0
  writeData(x1 >> 8);          //0x00
  writeData(x1 & 0xFF);        //与后还是X1
                               
  writeCommand(ST7789_RASET);  //写入命令，设置行地址  
  writeData(y0 >> 8);          //0x00
  writeData(y0 & 0xFF);        //与后还是Y0
  writeData(y1 >> 8);          //0x00 
  writeData(y1 & 0xFF);        //与后还是Y1 

  writeCommand(ST7789_RAMWR);  //执行写入RAM的命令
}

void  DFRobot_ST7789::commonInit(void) {
  pinMode(_dc, OUTPUT);    //DC引脚输出
  pinMode(_cs, OUTPUT);    //片选引脚输出
  
  //SPI的相关默认初始化
  SPI.begin();  
  //SPI模式的初始化设定，speedMaximum, dataOrder, dataMode，最大传输速率为24000000
  //数据传输顺序选择:MSBFIRST 高位先传; LSBFIRST 低位先传
  //数据模式选择4种
  //Mode 0 CPOL=0, CPHA=0 
  //Mode 1 CPOL=0, CPHA=1
  //Mode 2 CPOL=1, CPHA=0 //空闲时高电平，即有效电平是低电平，即时钟线拉低开始传输，第一个边沿采样                       
  //Mode 3 CPOL=1, CPHA=1
  mySPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE2);
  digitalWrite(_cs, LOW);   //片选拉低，开始传输
  
  pinMode(_rst, OUTPUT);    //复位引脚输出
  digitalWrite(_rst, HIGH); //正常情况复位引脚置高
  delay(50);
  ////复位引脚先拉低再拉高，一个上升沿，实现复位
  digitalWrite(_rst, LOW);
  delay(50);
  digitalWrite(_rst, HIGH);
  delay(50);
  
}

//初始化显示屏幕
void  DFRobot_ST7789::displayInit(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;  //命令的个数，参数的个数
  uint16_t ms;                    //uint8_t 最大只能256，ms=500不够用
  
  digitalWrite(_dc, HIGH);        //DC(1)写数据
  //读取flash常量到RAM常量,读的是cmd_240x240[0],即9，即命令个数
  numCommands = pgm_read_byte(addr++);   
  //numCommands=9
  while (numCommands--) {
    writeCommand(pgm_read_byte(addr++)); //读的是cmd_240x240[1]=ST7789_SWRESET这种命令
    numArgs  = pgm_read_byte(addr++);    //读的是cmd_240x240[2]=ST_CMD_DELAY，命令后一项
    ms       = numArgs & ST_CMD_DELAY;   //判断numArgs里是否有ST_CMD_DELAY延迟，如果ms=0,则没有不需要延迟，如果ms=ST_CMD_DELAY，则需要延迟
    numArgs &= ~ST_CMD_DELAY;            //读的是参数个数 1 + ST_CMD_DELAY与~ST_CMD_DELAY得到参数个数：1
    
    //有几个参就读几个参
    while (numArgs--) {                  
      writeData(pgm_read_byte(addr++)); 
    }

    //ms为真，则需要延迟；为假，跳过即可
    if (ms) {
      ms = pgm_read_byte(addr++);
      if (ms == 255) ms = 500;
      delay(ms);
    }
  }
}

void  DFRobot_ST7789::writeData(uint8_t c) {            
  digitalWrite(_dc, HIGH);               //DC(1)写数据
  digitalWrite(_cs, LOW);                //片选拉低，选中状态
  //在使用SPI.transfer()或断言芯片选择引脚之前，这个函数用于获得对SPI总线的独占访问，并配置正确的设置。
  SPI.beginTransaction(mySPISettings);   
  SPI.transfer(c);                       //直接调用SPI类的函数transfer,c为要传输的数据
  digitalWrite(_cs, HIGH);               //片选拉高，取消选择
  SPI.endTransaction();                  //结束通信
}

void  DFRobot_ST7789::writeCommand(uint8_t c) {          
  digitalWrite(_dc, LOW);                //DC(0)写命令
  digitalWrite(_cs, LOW);                //片选拉低，选中状态
  //在使用SPI.transfer()或断言芯片选择引脚之前，这个函数用于获得对SPI总线的独占访问，并配置正确的设置。
  SPI.beginTransaction(mySPISettings);   
  SPI.transfer(c);                       //直接调用SPI类的函数transfer,c为要传输的数据-此处为命令寄存器
  digitalWrite(_cs, HIGH);               //片选拉高，取消选择
  SPI.endTransaction();                  //结束通信
}