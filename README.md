# LCDWIKI-drivers-idf
Use LCDWIKI's TFT drivers on ESP-IDF.

## 基本信息

This is a library for the SPI lcd(TFT) display. Include LCDWIKI_SPI & LCDWIKI_GUI & their examples.

LCDWIKI_SPI的官方开源地址是：[LCDWIKI_SPI](https://github.com/lcdwiki/LCDWIKI_SPI) ，仅支持Arduino IDE；LCDWIKI_GUI的官方开源地址是：[LCDWIKI_GUI](https://github.com/lcdwiki/LCDWIKI_gui) ，仅支持Arduino IDE;

我对LCDWIKI_SPI和LCDWIKI_GUI进行了移植，使其支持ESP-IDF 5.1.4以及ESP32，并补充了他们的examples；我使用深圳秦唐盛世科技的2.0寸屏幕（ST7789V+FT6336，240x320 IPS）进行了demo测试，就效果良好；其中库和demo使用方法见下方“基本使用”所介绍。

This library support these lcd controller:
- ILI9325 
- ILI9328 
- ILI9341 
- HX8357D 
- HX8347G 
- HX8347I 
- ILI9486 
- ST7735S 
- SSD1283A
- ST7789
- ST7789V

Check out the file of LCDWIKI SPI lib Requirements for our tutorials and wiring diagrams.

These displays use spi bus to communicate, 5 pins are required to interface (MISO is no need), include SDA(MOSI)、SCK(SCLK)、CS、CD(D/C)、RST.

Basic functionally of this library was origianlly based on the demo-code of Adafruit GFX lib and Adafruit SPITFT lib.  

MIT license, all text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder LCDWIKI_SPI. Check that the LCDWIKI_SPI folder contains LCDWIKI_SPI.cpp and LCDWIKI_SPI.h

Place the LCDWIKI_SPI library folder your <arduinosketchfolder>/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE

## 基本使用

### Step 1

将整个LCDWIKI-drivers-idf文件夹复制到你的IDF项目的components文件夹中（或者用git获取），在项目main目录的CMakeLists.txt中添加对LCDWIKI-drivers-idf的依赖；值得注意的是，本项目的前置需求是[pschatzmann](https://github.com/pschatzmann)的[arduino-audio-tools](https://github.com/pschatzmann/arduino-audio-tools)，其内置的NoArduino.h使用ESP-IDF方法重构了Arduino基本函数，使得非Arduino项目能在不引入Arduino组件的情况下使用Arduino的基本函数，例如常用的延时、IO操作、串口打印等；下面是main目录的CMakeLists.txt示例：

```cmake
    idf_component_register(SRCS "main.cpp"
                            INCLUDE_DIRS "."
                            REQUIRES arduino-audio-tools LCDWIKI-drivers-idf)
```

### Step 2

假如有的examples需要其自身目录的头文件，只需要将LCDWIKI-drivers-idf的CMakeLists.txt的INCLUDE_DIRS添加其对应目录即可；

### Step 3

参考demo的ino文件，将其所用的头文件放到到你的main.cpp中，demo的setup()的内容放到main.cpp的app_main()中即可，如果loop()有内容，那就创建一个task()，把loop()的内容放到task()里，并在app_main()末尾调用启动任务调度器启动此task()即可。

### 注意事项

在NverUseArduino.h中有两个开关，**NoArduino**和**USE_GxEPD**；

开关**NoArduino**控制**是否不使用Arduino**，默认置1是不使用Arduino，其会调用arduino-audio-tools的NoArduino.h，否则会调用Arduino.h；

开关**USE_GxEPD**控制**是否使用GoodDisplay的ESP32-L开发板**，其默认置1是启用GoodDisplay ESP32-L，如使用其它开发板如微雪的[电子墨水屏无线网络驱动板](https://www.waveshare.net/shop/e-Paper-ESP32-Driver-Board.htm)，需要将开关**USE_GxEPD**置0，否则会影响SPI和墨水屏控制IO的定义。同样的，如果你没有使用上述开发板，使用的是自定义板子，那请修改为实际IO值；

LCDWIKI_SPI定义了多组函数，分软件SPI（8个参数）和硬件SPI（5个参数），通过不同的参数数量进行区分，不过我设置了ESP32默认仅使用硬件SPI（SPI3_HOST，ESP32对应VSPI），此时只需要为LCDWIKI_SPI传入“MODEL, CS, CD, RST, LED”这5个参数，其余的MOSI和SCLK已在LCDWIKI_SPI.cpp预定义，假如强行传入大于5个参数，将会触发函数未定义的错误；假如你不喜欢这样，你希望同时传入所有参数来启用软件SPI（即使你用的是硬件SPI引脚），那你只需要禁用USE_HWSPI_ONLY宏定义即可，这将会开放软件SPI条件编译，你可以通过传入“MODEL,CS,CD,-1,SDA,RST,SCK,LED”这8个参数来启用软件SPI函数，这将开放SPI引脚的选择；软件SPI与硬件SPI本质只是有没有预定义MOSI和SCLK，性能取决于你实际接的引脚；


## 字体

目前使用的是LCDWIKI_GUI的默认字体，等我后续更新；

## FT6336触摸驱动

常见的FT6336触摸屏驱动详见[mlx-ft6336-drivers](https://github.com/sprlightning/mlx-ft6336-drivers)，同样支持ESP-IDF 5.1.4环境。

## LCDWIKI版本更新记录

2025年11月19日确认新增ST7789(V)的支持。


