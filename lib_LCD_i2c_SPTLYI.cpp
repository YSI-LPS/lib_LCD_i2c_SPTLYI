/** LCD i2c SPTLYI class
*  Utilisée pour écrire sur l'afficheur i2c SPTLYI 2x16.
*
* Copyright (c) 2014, cstyles (http://mbed.org)
*
* Exemple:
* @code
* #include "mbed.h"
* #include "lib_LCD_i2c_SPTLYI.h"
*
* LCD_I2C LCD(p28, p27, p26, 0x7C);  //sda, scl, rst, only 4 slave address 0x7C 0x7D 0x7E 0x7F
* 
* int main() {
* int i=0;
*   while(1)
*   {
*       LCD.clear();
*       LCD.print(i++);
*       if(i>9999) i=0;
*       wait(0.25);
*   }
* }
* @endcode
* @file          lib_LCD_i2c_SPTLYI.h 
* @purpose       library for i2c LCD
* @date          Jan 2014
* @author        Yannic Simon
*/

#include "lib_LCD_i2c_SPTLYI.h"

#define I2C_Frequency 600000
#define Interrupts_OFF __disable_irq();    // Disable Interrupts
#define Interrupts_ON __enable_irq();     // Enable Interrupts
#define Last_Controle_Byte 0x00
#define First_Controle_Byte 0x80
#define Register_Select_CByte 0x40
#define Function_Set_IS0 0x38
#define Function_Set_IS1 0x39
#define Shift_Right_Cursor_Set 0x14
#define Shift_Left_Cursor_Set 0x10
#define Contrast_Set 0x79
#define Power_Icon_Set 0x50
#define Follower_Controle_Set 0x6F
#define Entry_Mode_Set 0x04
#define Display_ON_Set 0x0F
#define Display_OFF_Set 0x08
#define Cursor_ON_Set 0x0F
#define Cursor_OFF_Set 0x0C
#define Clear_Display 0x01
#define Return_Home 0x02



LCD_I2C::LCD_I2C(PinName pin_sda, PinName pin_scl, PinName pin_rst, int address) : I2C(pin_sda, pin_scl), m_pin_rst(pin_rst), m_address(address)
{
    reset();
    init();
}

int LCD_I2C::init(void)
{
    I2C::frequency(I2C_Frequency);

    char data[12]={First_Controle_Byte,Function_Set_IS0,Last_Controle_Byte,Function_Set_IS1,Shift_Right_Cursor_Set,Contrast_Set,Power_Icon_Set,Follower_Controle_Set,Display_ON_Set,Clear_Display,Return_Home,Entry_Mode_Set};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 12);
    Interrupts_ON
    wait_ms(1);
    return ack;
}

void LCD_I2C::reset(void)
{
    m_pin_rst.output();
    wait_ms(1);
    m_pin_rst.input();
    wait_ms(1);
}

int LCD_I2C::clear(void)
{
    char data[2]={Last_Controle_Byte,Clear_Display};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    X_position_cursor = 0;
    Y_position_cursor = 0;
    wait_ms(1);
    return ack;
}

int LCD_I2C::turn_on_cursor(void)
{
    char data[2]={Last_Controle_Byte,Cursor_ON_Set};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::turn_off_cursor(void)
{
    char data[2]={Last_Controle_Byte,Cursor_OFF_Set};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::turn_on_display(void)
{
    char data[2]={Last_Controle_Byte,Display_ON_Set};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::turn_off_display(void)
{
    char data[2]={Last_Controle_Byte,Display_OFF_Set};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::return_home_cursor(void)
{
    char data[2]={Last_Controle_Byte,Return_Home};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    X_position_cursor = 0;
    Y_position_cursor = 0;
    return ack;
}

void LCD_I2C::shift_left_cursor(int n)
{
    char data[3]={Last_Controle_Byte,Function_Set_IS0,Shift_Left_Cursor_Set};
    int i=0;
    for(i=0;i<n;i++)
    {
        Interrupts_OFF
        I2C::write(m_address, &data[0], 3);
        Interrupts_ON
    }
    X_move_position(-n);
}

void LCD_I2C::shift_right_cursor(int n)
{
    char data[3]={Last_Controle_Byte,Function_Set_IS0,Shift_Right_Cursor_Set};
    int i=0;
    for(i=0;i<n;i++)
    {
        Interrupts_OFF
        I2C::write(m_address, &data[0], 3);
        Interrupts_ON
    }
    X_move_position(n);
}

int LCD_I2C::shift_line_cursor(void)
{
    char data[4]={Last_Controle_Byte,Function_Set_IS0,0x40+0x00,0x80+0x40};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 4);
    Interrupts_ON
    X_position_cursor = 0;
    Y_position_cursor = 1;
    return ack;
}

int LCD_I2C::set_position_cursor(int X)
{
    int CGRAM=0x00;
    int DDRAM=0x00;
    
    if(X < 0)    X = 0;
    if(X > 39)   X = 39;    //0x27
    
    
    if(Y_position_cursor == 1)
    {
        CGRAM = X + 39;     //0x27
        DDRAM = X + 64;     //0x40
    }else{
        CGRAM = X;
        DDRAM = X;
    }

    X_position_cursor = X;

    char data[4]={Last_Controle_Byte,Function_Set_IS0,0x40+CGRAM,0x80+DDRAM};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 4);
    Interrupts_ON
    return ack;
}

int LCD_I2C::set_position_cursor(int X, int Y)
{
    int CGRAM=0x00;
    int DDRAM=0x00;
    
    if(X < 0)    X = 0;
    if(X > 39)   X = 39;    //0x27
    if(Y < 0)    Y = 0;
    if(Y > 1)    Y = 1;
    
    if(Y == 1)
    {
        CGRAM = X + 39;     //0x27
        DDRAM = X + 64;     //0x40
    }else{
        CGRAM = X;
        DDRAM = X;
    }

    X_position_cursor = X;
    Y_position_cursor = Y;

    char data[4]={Last_Controle_Byte,Function_Set_IS0,0x40+CGRAM,0x80+DDRAM};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 4);
    Interrupts_ON
    return ack;
}

void LCD_I2C::shift_left_display(int n)
{
    char data[3]={Last_Controle_Byte,Function_Set_IS0,0x18};
    int i=0;
    for(i=0;i<n;i++)
    {
        Interrupts_OFF
        I2C::write(m_address, &data[0], 3);
        Interrupts_ON
    }
}

void LCD_I2C::shift_right_display(int n)
{
    char data[3]={Last_Controle_Byte,Function_Set_IS0,0x1C};
    int i=0;
    for(i=0;i<n;i++)
    {
        Interrupts_OFF
        I2C::write(m_address, &data[0], 3);
        Interrupts_ON
    }
}

int LCD_I2C::enable_auto_shift_right_display(void)
{
    char data[2]={Last_Controle_Byte,0x05};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::enable_auto_shift_left_display(void)
{
    char data[2]={Last_Controle_Byte,0x07};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::disable_auto_shift_display(void)
{
    char data[2]={Last_Controle_Byte,0x06};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::auto_shift_right_cursor(void)
{
    char data[2]={Last_Controle_Byte,0x06};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::auto_shift_left_cursor(void)
{
    char data[2]={Last_Controle_Byte,0x04};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

void LCD_I2C::X_move_position(int n)
{
    X_position_cursor += n;
    while(X_position_cursor > 39)
    {
        X_position_cursor -= 40;
        Y_position_cursor += 1;
        if(Y_position_cursor == 2) Y_position_cursor = 0;
    }
    while(X_position_cursor < 0)
    {
        X_position_cursor += 40;
        Y_position_cursor -= 1;
        if(Y_position_cursor == -1) Y_position_cursor = 1;
    }
}

int LCD_I2C::get_X_position_cursor(void)
{
    return X_position_cursor;
}

int LCD_I2C::get_Y_position_cursor(void)
{
    return Y_position_cursor;
}

int LCD_I2C::putnc(char *s,int n)
{
    int ack=0, i=0;
    char data[n+1];
    data[0]=Register_Select_CByte;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    Interrupts_OFF
    ack = I2C::write(m_address, &data[0], n+1);
    Interrupts_ON
    return ack;  
}

int LCD_I2C::print(char c)
{
    char data[2]={Register_Select_CByte,c};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 2);
    Interrupts_ON
    return ack;
}

int LCD_I2C::print(char c1, char c2)
{
    char data[3]={Register_Select_CByte,c1,c2};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 3);
    Interrupts_ON
    return ack;
}

int LCD_I2C::print(char c1, char c2, char c3)
{
    char data[4]={Register_Select_CByte,c1,c2,c3};
    Interrupts_OFF
    int ack = I2C::write(m_address, &data[0], 4);
    Interrupts_ON
    return ack;
}

int LCD_I2C::print(short nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%d",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(unsigned short nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%u",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%d",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(unsigned int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%u",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(long long nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%lld",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(unsigned long long nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%llu",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(float nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%f",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(double nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%lf",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s)
{
    int ack=0, i=0, n=0;
    while(s[n]!='\0') n++;
    char data[n+1];
    data[0]=Register_Select_CByte;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    Interrupts_OFF
    ack = I2C::write(m_address, &data[0], n+1);
    Interrupts_ON
    return ack;  
}

int LCD_I2C::print(char *s, short nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, int nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, short nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned int nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, short nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, long long nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, short nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned long long nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, short nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, float nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, short nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, short nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, double nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, short nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, short nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, short nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned short nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, int nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned short nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned int nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned short nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, long long nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned short nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned long long nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned short nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, float nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned short nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, unsigned short nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, double nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned short nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned short nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned short nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, unsigned short nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, long long nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, int nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned long long nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, int nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, float nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, int nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, long long nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned int nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned long long nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned int nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, float nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned int nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, unsigned int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, double nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned int nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned int nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, unsigned int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, float nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, long long nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, long long nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, double nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, long long nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, long long nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, long long nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned long long nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, float nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, unsigned long long nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, unsigned long long nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, double nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, unsigned long long nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned long long nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, unsigned long long nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, unsigned long long nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, float nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, float nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, double nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, float nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, float nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, float nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, float nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::putnb2(int nb)
{
    int temp=0, signe=0, exp=1;
    int i=0, j=0;
    int n=0, nb_c=nb;
    char buffer[100]={0x00};

    if(nb<0)
    {
        buffer[0]='-';
        signe = 1;
    }

    while(nb_c!=0)
    {
        nb_c /= 10;
        n++;
    }
    nb_c=nb;
    if(signe)   nb_c *= -1;

    for(i=0;i<n;i++)
    {
        exp = 1;
        for(j=(n-i);j>1;j--)    exp *= 10;
        temp = nb_c/exp;
        nb_c = nb_c-(temp*exp);

        buffer[signe+i]=0x30+temp;
        buffer[signe+i+1]='\0';
    }
    if(signe) n++;
    return putnc(&buffer[0],n);
}
