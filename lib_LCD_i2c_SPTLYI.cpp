#include "lib_LCD_i2c_SPTLYI.h"


LCD_I2C::LCD_I2C(PinName pin_sda, PinName pin_scl, PinName pin_rst, int address) : I2C(pin_sda, pin_scl), m_pin_rst(pin_rst), m_address(address)
{
    reset();
    init();
}

int LCD_I2C::init(void)
{
    I2C::frequency(600000);

    char data[12]={0x80,0x38,0x00,0x39,0x14,0x79,0x50,0x6F,0x0F,0x01,0x02,0x04};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 12);
    __enable_irq();     // Enable Interrupts
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
    char data[2]={0x00,0x01};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    wait_ms(1);
    return ack;
}

int LCD_I2C::turn_on_cursor(void)
{
    char data[2]={0x00,0x0F};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::turn_off_cursor(void)
{
    char data[2]={0x00,0x0C};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::turn_on_display(void)
{
    char data[2]={0x00,0x0F};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::turn_off_display(void)
{
    char data[2]={0x00,0x08};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::return_home_cursor(void)
{
    char data[2]={0x00,0x02};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

void LCD_I2C::shift_left_cursor(int n)
{
    char data[3]={0x00,0x38,0x10};
    int i=0;
    for(i=0;i<n;i++)
    {
        __disable_irq();    // Disable Interrupts
        I2C::write(m_address, &data[0], 3);
        __enable_irq();     // Enable Interrupts
    }
}

void LCD_I2C::shift_right_cursor(int n)
{
    char data[3]={0x00,0x38,0x14};
    int i=0;
    for(i=0;i<n;i++)
    {
        __disable_irq();    // Disable Interrupts
        I2C::write(m_address, &data[0], 3);
        __enable_irq();     // Enable Interrupts
    }
}

int LCD_I2C::shift_line_cursor(void)
{
    char data[4]={0x00,0x38,0x40,0xC0};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 4);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::set_position_cursor(int position)
{
    int CGRAM=0x00;
    int DDRAM=0x00;

    if(position < 0x00)
    {
        CGRAM = 0x00;
        DDRAM = 0x00;
    }else{
        if(position > 0x7F)
        {
            CGRAM = 0x7F;
            DDRAM = 0x67;
        }else{
            CGRAM = position;
            if(position > 0x27)
            {
                DDRAM = (position - 0x27) + 0x3F;
            }else{
                DDRAM = position;
            }
        }
    }
    char data[4]={0x00,0x38,0x40+CGRAM,0x80+DDRAM};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 4);
    __enable_irq();     // Enable Interrupts
    return ack;
}

void LCD_I2C::shift_left_display(int n)
{
    char data[3]={0x00,0x38,0x18};
    int i=0;
    for(i=0;i<n;i++)
    {
        __disable_irq();    // Disable Interrupts
        I2C::write(m_address, &data[0], 3);
        __enable_irq();     // Enable Interrupts
    }
}

void LCD_I2C::shift_right_display(int n)
{
    char data[3]={0x00,0x38,0x1C};
    int i=0;
    for(i=0;i<n;i++)
    {
        __disable_irq();    // Disable Interrupts
        I2C::write(m_address, &data[0], 3);
        __enable_irq();     // Enable Interrupts
    }
}

int LCD_I2C::enable_auto_shift_right_display(void)
{
    char data[2]={0x00,0x05};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::enable_auto_shift_left_display(void)
{
    char data[2]={0x00,0x07};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::disable_auto_shift_display(void)
{
    char data[2]={0x00,0x06};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::auto_shift_right_cursor(void)
{
    char data[2]={0x00,0x06};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::auto_shift_left_cursor(void)
{
    char data[2]={0x00,0x04};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::putnc(char *s,int n)
{
    int ack=0, i=0;
    char data[n+1];
    data[0]=0x40;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    __disable_irq();    // Disable Interrupts
    ack = I2C::write(m_address, &data[0], n+1);
    __enable_irq();     // Enable Interrupts
    return ack;  
}

int LCD_I2C::print(char c)
{
    char data[2]={0x40,c};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 2);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::print(char c1, char c2)
{
    char data[3]={0x40,c1,c2};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 3);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::print(char c1, char c2, char c3)
{
    char data[4]={0x40,c1,c2,c3};
    __disable_irq();    // Disable Interrupts
    int ack = I2C::write(m_address, &data[0], 4);
    __enable_irq();     // Enable Interrupts
    return ack;
}

int LCD_I2C::print(char *s)
{
    int ack=0, i=0, n=0;
    while(s[n]!='\0') n++;
    char data[n+1];
    data[0]=0x40;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    __disable_irq();    // Disable Interrupts
    ack = I2C::write(m_address, &data[0], n+1);
    __enable_irq();     // Enable Interrupts
    return ack;  
}

int LCD_I2C::print(int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%d\0",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(double nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],"%.2f\0",nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, double nb)
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

int LCD_I2C::print(char *s, double nb1, double nb2, int nb3)
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


int LCD_I2C::print(char *s, double nb1, int nb2, int nb3)
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


int LCD_I2C::print(char *s, int nb1, double nb2, int nb3)
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


int LCD_I2C::print(char *s, int nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    return putnc(&buffer[0],n);
}

int LCD_I2C::print(char *s, char c1)
{
    int ack=0, i=0, n=0;
    while(s[n]!='\0') n++;
    char data[n+2];
    data[0]=0x40;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    data[n+1]=c1;
    __disable_irq();    // Disable Interrupts
    ack = I2C::write(m_address, &data[0], n+2);
    __enable_irq();     // Enable Interrupts
    return ack;  
}

int LCD_I2C::print(char *s, char c1, int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, double nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, int nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, int nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, double nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, double nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, double nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, double nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, double nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, double nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, int nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, int nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, int nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}


int LCD_I2C::print(char *s, char c1, int nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    return putnc(&buffer[0],n+1);
}

int LCD_I2C::print(char *s, char c1, char c2)
{
    int ack=0, i=0, n=0;
    while(s[n]!='\0') n++;
    char data[n+3];
    data[0]=0x40;
    for(i=0;i<n;i++)    data[i+1]=s[i];
    data[n+1]=c1;
    data[n+2]=c2;
    __disable_irq();    // Disable Interrupts
    ack = I2C::write(m_address, &data[0], n+3);
    __enable_irq();     // Enable Interrupts
    return ack;  
}

int LCD_I2C::print(char *s, char c1, char c2, int nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, double nb)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, int nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, int nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, double nb1, int nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, double nb1, double nb2)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, double nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}

int LCD_I2C::print(char *s, char c1, char c2, double nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, double nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, double nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, int nb1, double nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, int nb1, double nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, int nb1, int nb2, double nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
}


int LCD_I2C::print(char *s, char c1, char c2, int nb1, int nb2, int nb3)
{
    int n=0;
    char buffer[100]={0x00};
    n = sprintf(&buffer[0],&s[0],nb1,nb2,nb3);
    buffer[n]=c1;
    buffer[n+1]=c2;
    return putnc(&buffer[0],n+2);
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
