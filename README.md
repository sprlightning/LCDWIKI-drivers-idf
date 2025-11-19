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
下面是demo display_string在main.cpp的使用示例：
```cpp
/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * 
 * 屏幕驱动：https://github.com/sprlightning/LCDWIKI-drivers-idf
 * 触屏驱动：https://github.com/sprlightning/mlx-ft6336-drivers
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"

// tft
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

//paramters define
#define MODEL ST7789
#define CS   27    
#define CD   14
#define RST  12
#define SDA  23
#define SCK  18
#define LED  13   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//the definiens of software spi mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
// LCDWIKI_SPI mylcd(MODEL,CS,CD,-1,SDA,RST,SCK,LED); //model,cs,dc,sdo,sda,reset,sck,led
LCDWIKI_SPI mylcd(MODEL, CS, CD, RST, LED); // hardware spi

// tft_test_task
void tft_test_task(void *pvParameters) {
  //
  while (1) {
    mylcd.Set_Text_Mode(0);

    mylcd.Set_Rotation(3);
  
    mylcd.Fill_Screen(WHITE);
    mylcd.Set_Text_colour(RED);
    mylcd.Set_Text_Back_colour(WHITE);
    mylcd.Set_Text_Size(1);
    mylcd.Print_String("Hello World!", 0, 0);
    mylcd.Print_Number_Float(1234.56, 2, 0, 8, '.', 0, ' ');  
    mylcd.Print_Number_Int(0xDEADBEF, 0, 16, 0, ' ',16);

    mylcd.Set_Text_colour(GREEN);
    mylcd.Set_Text_Size(2);
    mylcd.Print_String("Hello", 0, 28);
    mylcd.Print_Number_Float(1234.56, 2, 0, 44, '.', 0, ' ');  
    mylcd.Print_Number_Int(0xDEADBEF, 0, 60, 0, ' ',16);

    mylcd.Set_Text_colour(BLUE);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("Hello", 0, 80);
    mylcd.Print_Number_Float(1234.56, 2, 0, 104, '.', 0, ' ');  
    mylcd.Print_Number_Int(0xDEADBEF, 0, 128, 0, ' ',16);

    mylcd.Print_String_EN(0, 160, "Hello World!", &Font12, CYAN, WHITE);
    mylcd.Print_String_CN(0, 180, "木炉星TFT测试123", &Font12CJK_B, MAGENTA, WHITE);

    delay(3000);
  }
}


extern "C" void app_main(void)
{
    //  ____  ____ ___   _     ____ ____    _____         _
    // / ___||  _ \_ _| | |   / ___|  _ \  |_   _|__  ___| |_
    // \___ \| |_) | |  | |  | |   | | | |   | |/ _ \/ __| __|
    //  ___) |  __/| |  | |__| |___| |_| |   | |  __/\__ \ |_
    // |____/|_|  |___| |_____\____|____/    |_|\___||___/\__|
    printf(" ____  ____ ___   _     ____ ____    _____         _\r\n");
    printf("/ ___||  _ \\_ _| | |   / ___|  _ \\  |_   _|__  ___| |_\r\n");
    printf("\\___ \\| |_) | |  | |  | |   | | | |   | |/ _ \\/ __| __|\r\n");
    printf(" ___) |  __/| |  | |__| |___| |_| |   | |  __/\\__ \\ |_\r\n");
    printf("|____/|_|  |___| |_____\\____|____/    |_|\\___||___/\\__|\r\n");
    
    // 关闭看门狗，避免测试过程中复位
    esp_task_wdt_deinit();
    
    // 初始化
    mylcd.Init_LCD();
    mylcd.Fill_Screen(BLACK); 

    xTaskCreatePinnedToCore(tft_test_task, "tft_test_task", 4096, NULL, 10, NULL, 1);
    
    // 测试完成后挂起，避免程序退出
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

```

### 注意事项

在NverUseArduino.h中有两个开关，**NoArduino**和**USE_GxEPD**；

开关**NoArduino**控制**是否不使用Arduino**，默认置1是不使用Arduino，其会调用arduino-audio-tools的NoArduino.h，否则会调用Arduino.h；

开关**USE_GxEPD**控制**是否使用GoodDisplay的ESP32-L开发板**，其默认置1是启用GoodDisplay ESP32-L，如使用其它开发板如微雪的[电子墨水屏无线网络驱动板](https://www.waveshare.net/shop/e-Paper-ESP32-Driver-Board.htm)，需要将开关**USE_GxEPD**置0，否则会影响SPI和墨水屏控制IO的定义。同样的，如果你没有使用上述开发板，使用的是自定义板子，那请修改为实际IO值；

LCDWIKI_SPI定义了多组函数，分软件SPI（8个参数）和硬件SPI（5个参数），通过不同的参数数量进行区分，不过我设置了ESP32默认仅使用硬件SPI（SPI3_HOST，ESP32对应VSPI），此时只需要为LCDWIKI_SPI传入“MODEL, CS, CD, RST, LED”这5个参数，其余的MOSI和SCLK已在LCDWIKI_SPI.cpp预定义，假如强行传入大于5个参数，将会触发函数未定义的错误；假如你不喜欢这样，你希望同时传入所有参数来启用软件SPI（即使你用的是硬件SPI引脚），那你只需要禁用USE_HWSPI_ONLY宏定义即可，这将会开放软件SPI条件编译，你可以通过传入“MODEL,CS,CD,-1,SDA,RST,SCK,LED”这8个参数来启用软件SPI函数，这将开放SPI引脚的选择；软件SPI与硬件SPI本质只是有没有预定义MOSI和SCLK，性能取决于你实际接的引脚；


## 字体

我在src/Fonts目录中增加了CJK字体，适用于大多数使用场景，可用**Print_String_CN**函数绘制；

使用STC-ISP的字库生成工具制作，水平扫描，从左到右从上到下，高位在前；

其中**Font12CJK**使用新宋体，12号，常规；点阵宽x高=16x21，字体宽x高=16x16；水平调整0，垂直调整2；

**Font12CJK_B**使用新宋体，12号，粗体；点阵宽x高=16x21，字体宽x高=16x16；水平调整0，垂直调整2；

值得注意的是，字库占用空间较大，可能单个CJK就需要1MB空间，为此可参考我提供的**music_fonts_cjk.txt**按照**stc_isp_char_config.ini**定义的规则生成字库bin文件放在独立的flash中；或者确保ESP32 Flash的factory分区有足够的空间存放带字库的ESP32 Image，这通常意味着需要4MB及以上的ESP32 Flash；

SIC-ISP工具位于extras/Font-tools目录，版本6.96A，来自STC官网；解压后运行，其菜单的工具选项可看到“字库生成工具”。

## FT6336触摸驱动

常见的FT6336触摸屏驱动详见[mlx-ft6336-drivers](https://github.com/sprlightning/mlx-ft6336-drivers)，同样支持ESP-IDF 5.1.4环境。

## LCDWIKI版本更新记录

2025年11月19日确认新增ST7789(V)的支持。


