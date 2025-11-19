/*
This is the core GUI library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

MIT License
*/

#include "LCDWIKI_font.c"
#include "LCDWIKI_GUI.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

//Constructor to set text color 
LCDWIKI_GUI::LCDWIKI_GUI(void)
{
	text_bgcolor = 0xF800; //default red
	text_color = 0x07E0; //default green
	draw_color = 0xF800; //default red
	text_size = 1;
	text_mode = 0;
}

//set 16bits draw color
void LCDWIKI_GUI::Set_Draw_color(uint16_t color)
{
	draw_color = color;
}

//set 8bits r,g,b color
void LCDWIKI_GUI::Set_Draw_color(uint8_t r, uint8_t g, uint8_t b)
{
	draw_color = Color_To_565(r, g, b);
}

//get draw color
uint16_t LCDWIKI_GUI::Get_Draw_color(void) const
{
	return draw_color;
}

//draw a pixel point
void LCDWIKI_GUI::Draw_Pixel(int16_t x, int16_t y)
{
	Draw_Pixe(x, y, draw_color);
}

void LCDWIKI_GUI::Draw_Pixel(int16_t x, int16_t y, int16_t draw_color)
{
	Draw_Pixe(x, y, draw_color);
}

//read color data for point(x,y)
uint16_t LCDWIKI_GUI::Read_Pixel(int16_t x, int16_t y)
{
	uint16_t colour;
	Read_GRAM(x, y, &colour, 1, 1);
	return colour;
}

//fill a rectangle
void LCDWIKI_GUI::Fill_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	int w = x2 - x1 + 1, h = y2 - y1 + 1;
   	if (w < 0) 
	{ 
		x1 = x2; 
		w = -w; 
	}
	if (h < 0) 
	{ 
		y1 = y2; 
		h = -h; 
	}
	Fill_Rect(x1, y1, w, h, draw_color);
}

//draw a vertical line 
void LCDWIKI_GUI::Draw_Fast_VLine(int16_t x, int16_t y, int16_t h)
{
	Fill_Rect(x, y, 1, h, draw_color);
}

//draw a horizontal line
void LCDWIKI_GUI::Draw_Fast_HLine(int16_t x, int16_t y, int16_t w)
{
	Fill_Rect(x, y, w, 1, draw_color);
}

//Fill the full screen with color
void LCDWIKI_GUI::Fill_Screen(uint16_t color)
{
	Fill_Rect(0, 0, Get_Width(), Get_Height(), color);
}

//Fill the full screen with r,g,b
void LCDWIKI_GUI::Fill_Screen(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = Color_To_565(r, g, b);
	Fill_Rect(0, 0, Get_Width(), Get_Height(), color);
}

//draw an arbitrary line from (x1,y1) to (x2,y2)
void LCDWIKI_GUI::Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	int16_t steep = abs(y2 - y1) > abs(x2 - x1);
  	if (steep) 
	{
    	swap(x1, y1);
    	swap(x2, y2);
	}
	if (x1 > x2) 
	{
    	swap(x1, x2);
    	swap(y1, y2);
  	}
	
  	int16_t dx, dy;
  	dx = x2 - x1;
  	dy = abs(y2 - y1);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y1 < y2) 
	{
    	ystep = 1;
  	} 
	else 
	{
    	ystep = -1;
	}

	for (; x1<=x2; x1++) 
	{
    	if (steep) 
		{
      		Draw_Pixel(y1, x1);
    	} 
		else 
		{
      		Draw_Pixel(x1, y1);
    	}
    	err -= dy;
    	if (err < 0) 
		{
			y1 += ystep;
			err += dx;
    	}
  	}
}

//draw a rectangle
void LCDWIKI_GUI::Draw_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{ 
	int16_t w = x2 - x1 + 1, h = y2 - y1 + 1;
	if (w < 0) 
	{ 
		x1 = x2; 
		w = -w; 
	}
	if (h < 0) 
	{ 
		y1 = y2; 
		h = -h; 
	}
	Draw_Fast_HLine(x1, y1, w);
  	Draw_Fast_HLine(x1, y2, w);
	Draw_Fast_VLine(x1, y1, h);
	Draw_Fast_VLine(x2, y1, h);
}

//draw a round rectangle
void LCDWIKI_GUI::Draw_Round_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t radius)
{
	int w = x2 - x1 + 1, h = y2 - y1 + 1;
	if (w < 0) 
	{ 
		x1 = x2; 
		w = -w; 
	}
 	if (h < 0) 
	{ 
		y1 = y2; 
		h = -h; 
	}
	Draw_Fast_HLine(x1+radius, y1, w-2*radius); 
	Draw_Fast_HLine(x1+radius, y1+h-1, w-2*radius); 
	Draw_Fast_VLine(x1, y1+radius, h-2*radius); 
  	Draw_Fast_VLine(x1+w-1, y1+radius, h-2*radius);
	Draw_Circle_Helper(x1+radius, y1+radius, radius, 1);
	Draw_Circle_Helper(x1+w-radius-1, y1+radius, radius, 2);
	Draw_Circle_Helper(x1+w-radius-1, y1+h-radius-1, radius, 4);
	Draw_Circle_Helper(x1+radius, y1+h-radius-1, radius, 8);
}

//fill a round rectangle
void LCDWIKI_GUI::Fill_Round_Rectangle(int16_t x1, int16_t y1, int16_t x2,int16_t y2, int16_t radius)
{
	int w = x2 - x1 + 1, h = y2 - y1 + 1;
	if (w < 0) 
	{ 
		x1 = x2; 
		w = -w; 
	}
	if (h < 0) 
	{ 
		y1 = y2; 
		h = -h; 
	}
	Fill_Rect(x1+radius, y1, w-2*radius, h, draw_color);
	Fill_Circle_Helper(x1+w-radius-1, y1+radius, radius, 1, h-2*radius-1);
	Fill_Circle_Helper(x1+radius, y1+radius, radius, 2, h-2*radius-1);	
}

//draw a circle
void LCDWIKI_GUI::Draw_Circle(int16_t x, int16_t y, int16_t radius)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x1= 0;
	int16_t y1= radius;

	Draw_Pixel(x, y+radius);
 	Draw_Pixel(x, y-radius);
	Draw_Pixel(x+radius, y);
	Draw_Pixel(x-radius, y);

	while (x1<y1) 
	{
    	if (f >= 0) 
		{
      		y1--;
      		ddF_y += 2;
      		f += ddF_y;
    	}
    	x1++;
    	ddF_x += 2;
    	f += ddF_x;
  
		Draw_Pixel(x + x1, y + y1);
    	Draw_Pixel(x - x1, y + y1);
		Draw_Pixel(x + x1, y - y1);
		Draw_Pixel(x - x1, y - y1);
		Draw_Pixel(x + y1, y + x1);
		Draw_Pixel(x - y1, y + x1);
		Draw_Pixel(x + y1, y - x1);
		Draw_Pixel(x - y1, y - x1);
 	}
}

//draw a circular bead
void LCDWIKI_GUI::Draw_Circle_Helper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername)
{
	int16_t f     = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x     = 0;
	int16_t y     = radius;
	while (x<y) 
	{
    	if (f >= 0) 
		{
      		y--;
      		ddF_y += 2;
      		f += ddF_y;
    	}
	    x++;
	    ddF_x += 2;
	    f += ddF_x;
	    if (cornername & 0x4) 
		{
			Draw_Pixel(x0 + x, y0 + y);
			Draw_Pixel(x0 + y, y0 + x);
	    } 
	    if (cornername & 0x2) 
		{
			Draw_Pixel(x0 + x, y0 - y);
			Draw_Pixel(x0 + y, y0 - x);
	    }
	    if (cornername & 0x8) 
		{
			Draw_Pixel(x0 - y, y0 + x);
			Draw_Pixel(x0 - x, y0 + y);
	    }
	    if (cornername & 0x1)
		{
			Draw_Pixel(x0 - y, y0 - x);
	 		Draw_Pixel(x0 - x, y0 - y);
	    }
  	}
}

//fill a circle
void LCDWIKI_GUI::Fill_Circle(int16_t x, int16_t y, int16_t radius)
{
	Draw_Fast_VLine(x, y-radius, 2*radius+1);
	Fill_Circle_Helper(x, y, radius, 3, 0);
}

//fill a semi-circle
void LCDWIKI_GUI::Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta)
{
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) 
	{
    	if (f >= 0) 
		{
      		y--;
      		ddF_y += 2;
      		f += ddF_y;
    	}
    	x++;
    	ddF_x += 2;
    	f += ddF_x;

    	if (cornername & 0x1) 
		{
      		Draw_Fast_VLine(x0+x, y0-y, 2*y+1+delta);
      		Draw_Fast_VLine(x0+y, y0-x, 2*x+1+delta);
    	}
    	if (cornername & 0x2) 
		{
      		Draw_Fast_VLine(x0-x, y0-y, 2*y+1+delta);
      		Draw_Fast_VLine(x0-y, y0-x, 2*x+1+delta);
    	}
  	}
}

//draw a triangle
void LCDWIKI_GUI::Draw_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2)
{
	Draw_Line(x0, y0, x1, y1);
	Draw_Line(x1, y1, x2, y2);
  	Draw_Line(x2, y2, x0, y0);
}

//fill a triangle
void LCDWIKI_GUI::Fill_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2)
{
	int16_t a, b, y, last;
  	if (y0 > y1) 
	{
    	swap(y0, y1); 
		swap(x0, x1);
  	}
  	if (y1 > y2) 
	{
    	swap(y2, y1); 
		swap(x2, x1);
  	}
  	if (y0 > y1) 
	{
    	swap(y0, y1); 
		swap(x0, x1);
  	}

	if(y0 == y2) 
	{ 
    	a = b = x0;
    	if(x1 < a)
    	{
			a = x1;
    	}
    	else if(x1 > b)
    	{
			b = x1;
    	}
    	if(x2 < a)
    	{
			a = x2;
    	}
    	else if(x2 > b)
    	{
			b = x2;
    	}
    	Draw_Fast_HLine(a, y0, b-a+1);
    	return;
	}
  	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;
	if(y1 == y2)
	{
		last = y1; 
	}
  	else
  	{
		last = y1-1; 
  	}

  	for(y=y0; y<=last; y++) 
	{
    	a   = x0 + sa / dy01;
    	b   = x0 + sb / dy02;
    	sa += dx01;
    	sb += dx02;
    	if(a > b)
    	{
			swap(a,b);
    	}
    	Draw_Fast_HLine(a, y, b-a+1);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
    	a   = x1 + sa / dy12;
    	b   = x0 + sb / dy02;
    	sa += dx12;
    	sb += dx02;
    	if(a > b)
    	{
			swap(a,b);
    	}
		Draw_Fast_HLine(a, y, b-a+1);
	}
}

//uint8_t LCDKIWI_GUI::Get_Screen_Rotation(void) const
//{
//	return LCDKIWI_KBV::Get_Rotation();
//}

//void LCDKIWI_GUI::Push_Colors(uint16_t * block, int16_t n, boolean first, uint8_t flags)
//{
//	LCDKIWI_KBV::Push_Any_Color(block, n, first, flags);
//}

//void LCDKIWI_GUI::Push_Colors(uint8_t * block, int16_t n, boolean first, uint8_t flags)
//{
//	LCDKIWI_KBV::Push_Any_Color(block, n, first, flags);
//}

//draw a bit map
void LCDWIKI_GUI::Draw_Bit_Map(int16_t x, int16_t y, int16_t sx, int16_t sy, const uint16_t *data, int16_t scale)
{
	int16_t color;
	Set_Addr_Window(x, y, x + sx*scale - 1, y + sy*scale - 1); 
	if(1 == scale)
	{

		Push_Any_Color((uint16_t *)data, sx * sy, 1, 0);
	}
	else 
	{
		for (int16_t row = 0; row < sy; row++) 
		{
			for (int16_t col = 0; col < sx; col++) 
			{
				color = *(data + (row*sx + col)*1);//pgm_read_word(data + (row*sx + col)*1);
				Fill_Rect(x+col*scale, y+row*scale, scale, scale, color);
			}
		}
	}
}

//set text coordinate
void LCDWIKI_GUI::Set_Text_Cousur(int16_t x, int16_t y)
{
	text_x = x;
	text_y = y;
}

//get text x coordinate
int16_t LCDWIKI_GUI::Get_Text_X_Cousur(void) const
{
	return text_x;
}

//get text y coordinate
int16_t LCDWIKI_GUI::Get_Text_Y_Cousur(void) const
{
	return text_y;
}

//set text colour with 16bit color
void LCDWIKI_GUI::Set_Text_colour(uint16_t color)
{
	text_color = color;
}

//set text colour with 8bits r,g,b
void LCDWIKI_GUI::Set_Text_colour(uint8_t r, uint8_t g, uint8_t b)
{
	text_color = Color_To_565(r, g, b);
}

//get text colour
uint16_t LCDWIKI_GUI::Get_Text_colour(void) const
{
	return text_color;
}

//set text background colour with 16bits color
void LCDWIKI_GUI::Set_Text_Back_colour(uint16_t color)
{
	text_bgcolor = color;	
}

//set text background colour with 8bits r,g,b
void LCDWIKI_GUI::Set_Text_Back_colour(uint8_t r, uint8_t g, uint8_t b)
{
	text_bgcolor = Color_To_565(r, g, b);
}

//get text background colour
uint16_t LCDWIKI_GUI::Get_Text_Back_colour(void) const
{
	return text_bgcolor;
}

//set text size
void LCDWIKI_GUI::Set_Text_Size(uint8_t s)
{
	text_size = s;
}

//get text size
uint8_t LCDWIKI_GUI::Get_Text_Size(void) const
{
	return text_size;
}

/******************************************************************************
function:	Set Text Mode
parameter:
    mode           ：Text Mode
note     : if set, text_bgcolor is invalid
         : 0，字符背景色有效
         : 1，字符背景色无效
******************************************************************************/
void LCDWIKI_GUI::Set_Text_Mode(bool mode)
{
	text_mode = mode;
}

//get text mode
bool LCDWIKI_GUI::Get_Text_Mode(void) const
{
	return text_mode;
}

//draw a char - 默认使用lcd_font字库
void LCDWIKI_GUI::Draw_Char(int16_t x, int16_t y, uint8_t c, uint16_t color,uint16_t bg, uint8_t size, bool mode)
{
	if((x >= Get_Width()) || (y >= Get_Height()) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0))
	{
    	return;
	}		
  	if(c >= 176)
  	{
		c++; 
  	}
	for (int8_t i=0; i<6; i++) 
	{
    	uint8_t line;
    	if (i == 5)
    	{
      		line = 0x0;
    	}
    	else
    	{
      		line = lcd_font[(c*5)+i]; // 直接访问数组（假设字体数据在RAM中）
    	}
    	for (int8_t j = 0; j<8; j++) 
		{
      		if (line & 0x1) 
			{
        		if (size == 1)
        		{
        			Draw_Pixe(x+i, y+j, color);
        		}
        		else 
				{  
					Fill_Rect(x+(i*size), y+(j*size), size, size, color);
        		}
        	} 
			else if (bg != color) 				
			{
				if(!mode)
				{
	        		if (size == 1) 
	        		{
	        			Draw_Pixe(x+i, y+j, bg);
	        		}
	        		else 
					{  
						Fill_Rect(x+i*size, y+j*size, size, size, bg);
					}
				}
			}
      		line >>= 1;
    	}
    }
}

//print string
size_t LCDWIKI_GUI::Print(uint8_t *st, int16_t x, int16_t y)
{
	int16_t pos;
	uint16_t len;
	const char * p = (const char *)st;
	size_t n = 0;
	if (x == CENTER || x == RIGHT) 
	{
		len = strlen((const char *)st) * 6 * text_size;		
		pos = (Get_Display_Width() - len); 
		if (x == CENTER)
		{
			x = pos/2;
		}
		else
		{
			x = pos - 1;
		}
	}
    Set_Text_Cousur(x, y);
	while(1)
	{
		unsigned char ch = *(p++);//pgm_read_byte(p++);
		if(ch == 0)
		{
			break;
		}
		if(write(ch))
		{
			n++;
		}
		else
		{
			break;
		}
	}	
	return n;
}

//print string
void LCDWIKI_GUI::Print_String(const uint8_t *st, int16_t x, int16_t y)
{
	Print((uint8_t *)st, x, y);
}

//print string
void LCDWIKI_GUI::Print_String(uint8_t *st, int16_t x, int16_t y)
{
	Print(st, x, y);
}

//print string
void LCDWIKI_GUI::Print_String(const char* st, int16_t x, int16_t y)
{
	Print((uint8_t *)st, x, y);
}

/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void LCDWIKI_GUI::Draw_Char(int16_t Xpoint, int16_t Ypoint, const char Acsii_Char,
                    sFONT* Font, int16_t Color_Foreground, int16_t Color_Background)
{
    int16_t Page, Column;

    if (Xpoint > Get_Width() || Ypoint > Get_Height()) {
        printf("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    Draw_Pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    Draw_Pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    Draw_Pixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void LCDWIKI_GUI::Print_String_EN(int16_t Xstart, int16_t Ystart, const char * pString,
                         sFONT* Font, int16_t Color_Foreground, int16_t Color_Background)
{
    int16_t Xpoint = Xstart;
    int16_t Ypoint = Ystart;

    if (Xstart > Get_Width() || Ystart > Get_Height()) {
        printf("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > Get_Width() ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > Get_Height() ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        // 绘制英文字符
        Draw_Char(Xpoint, Ypoint, * pString, Font, Color_Foreground, Color_Background);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}

/******************************************************************************
function: Display the string
parameter:
    Xstart  ：X coordinate
    Ystart  ：Y coordinate
    pString ：The first address of the Chinese string and English
              string to be displayed
    Font    ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void LCDWIKI_GUI::Print_String_CN(int16_t Xstart, int16_t Ystart, const char * pString, cFONT* font,
                        int16_t Color_Foreground, int16_t Color_Background)
{
    const char* p_text = pString;
    int x = Xstart, y = Ystart;
    int i, j,Num;

    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        if(*p_text <= 0x7F) {  //ASCII < 126
            for(Num = 0; Num < font->size; Num++) {
                if(*p_text== font->table[Num].index[0]) {
                    const char* ptr = &font->table[Num].matrix[0];

                    for (j = 0; j < font->Height; j++) {
                        for (i = 0; i < font->Width; i++) {
                            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                                if (*ptr & (0x80 >> (i % 8))) {
                                    Draw_Pixel(x + i, y + j, Color_Foreground);
                                    // Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            } else {
                                if (*ptr & (0x80 >> (i % 8))) {
                                    Draw_Pixel(x + i, y + j, Color_Foreground);
                                    // Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                } else {
                                    Draw_Pixel(x + i, y + j, Color_Background);
                                    // Paint_DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            }
                            if (i % 8 == 7) {
                                ptr++;
                            }
                        }
                        if (font->Width % 8 != 0) {
                            ptr++;
                        }
                    }
                    break;
                }
            }
            /* Point on the next character */
            p_text += 1;
            /* Decrement the column position by 16 */
            x += font->ASCII_Width;
        } else {        //Chinese
            for(Num = 0; Num < font->size; Num++) {
                if ((*p_text == font->table[Num].index[0]) && \
                    (*(p_text + 1) == font->table[Num].index[1]) && \
                    (*(p_text + 2) == font->table[Num].index[2])) {
                    const char* ptr = &font->table[Num].matrix[0];

                    for (j = 0; j < font->Height; j++) {
                        for (i = 0; i < font->Width; i++) {
                            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                                if (*ptr & (0x80 >> (i % 8))) {
                                    Draw_Pixel(x + i, y + j, Color_Foreground);
                                    // Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            } else {
                                if (*ptr & (0x80 >> (i % 8))) {
                                    Draw_Pixel(x + i, y + j, Color_Foreground);
                                    // Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                } else {
                                    Draw_Pixel(x + i, y + j, Color_Background);
                                    // Paint_DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            }
                            if (i % 8 == 7) {
                                ptr++;
                            }
                        }
                        if (font->Width % 8 != 0) {
                            ptr++;
                        }
                    }
                    break;
                }
            }
            /* Point on the next character */
            p_text += 3;
            /* Decrement the column position by 16 */
            x += font->Width;
        }
    }
}

//print int number
void LCDWIKI_GUI::Print_Number_Int(long num, int16_t x, int16_t y, int16_t length, uint8_t filler, int16_t system)
{
	uint8_t st[27] = {0};
	uint8_t *p = st+26;
	bool flag = false;
	int16_t len = 0,nlen = 0,left_len = 0,i = 0;
	*p = '\0';
	if(0 == num)
	{
		*(--p) = '0';
		len = 1;
	}
	else
	{
		if(num < 0)
		{
			num = -num;
			flag = true;
		}		
	}
	while((num > 0) && (len < 10))
	{
		if(num%system > 9)
		{
			*(--p) = 'A' + (num%system-10);
		}
		else
		{
			*(--p) = '0' + num%system;
		}
		num = num/system;
		len++;
	}
	if(flag)
	{
		*(--p) = '-';
	}
	if(length > (len + flag + 1))
	{
		if(length > sizeof(st))
		{
			nlen = sizeof(st) - len - flag - 1;
		}
		else
		{
			nlen = length - len - flag - 1;
		}
		for(i = 0;i< nlen;i++)
		{
			*(--p) = filler;
		}
		left_len = sizeof(st) - nlen - len - flag - 1;
	}	
	else
	{
		left_len = sizeof(st) - len - flag - 1;
	}
	for(i = 0; i < (sizeof(st) - left_len);i++)
	{
		st[i] = st[left_len + i];
	}
	st[i] = '\0';
	Print(st, x, y);
}

//print float number
void LCDWIKI_GUI::Print_Number_Float(double num, uint8_t dec, int16_t x, int16_t y, uint8_t divider, int16_t length, uint8_t filler)
{
	uint8_t st[27] = {0};
	uint8_t * p = st;
	bool flag = false;
	int16_t i = 0;
	if(dec<1)
	{
		dec=1;
	}
	else if(dec>5)
	{
		dec=5;
	}
	if(num<0)
	{
		flag = true;
	}
	snprintf((char *)st, sizeof(st), "%*.*f", length, dec, num); // 格式化浮点数（宽度length，小数位dec）
	if(divider != '.')
	{
		while(i < sizeof(st))
		{
			if('.' == *(p+i))
			{
				*(p+i) = divider;
			}
			i++;
		}	
	}
	if(filler != ' ')
	{
		if(flag)
		{
			*p = '-';
			i = 1;
			while(i < sizeof(st))
			{
				if((*(p+i) == ' ') || (*(p+i) == '-'))
				{
					*(p+i) = filler;
				}
				i++;
			}
		}
		else
		{
			i = 0;
			while(i < sizeof(st))
			{
				if(' ' == *(p+i))
				{
					*(p+i) = filler;
				}
			}
		}
	}
	Print(st, x, y);
}

//write a char
size_t LCDWIKI_GUI::write(uint8_t c) 
{
	if (c == '\n') 
	{
    	text_y += text_size*8;
    	text_x  = 0;
 	} 
	else if(c == '\r')
	{
	}
	else 
	{
    	Draw_Char(text_x, text_y, c, text_color, text_bgcolor, text_size,text_mode);
    	text_x += text_size*6;		
    }	
  	return 1;	
}

//get lcd width
int16_t LCDWIKI_GUI::Get_Display_Width(void) const
{
	return Get_Width();
}

//get lcd height 
int16_t LCDWIKI_GUI::Get_Display_Height(void) const
{
	return Get_Height();
}

//uint16_t LCDKIWI_GUI::Change_To_565(uint8_t r, uint8_t g, uint8_t b)
//{
//	return LCDKIWI_KBV::Color_To_565(r, g, b);
//}

//uint16_t LCDKIWI_GUI::Read_Dev_ID(void)
//{
//	return LCDKIWI_KBV::Read_ID();

//}

//void LCDKIWI_GUI::Dev_Invert_Display(boolean i)
//{
//	LCDKIWI_KBV::Invert_Display(i);
//}

//void LCDKIWI_GUI::Dev_Vert_Scroll(int16_t top, int16_t scrollines, int16_t offset)
//{
//	LCDKIWI_KBV::Vert_Scroll(top, scrollines, offset);
//}
