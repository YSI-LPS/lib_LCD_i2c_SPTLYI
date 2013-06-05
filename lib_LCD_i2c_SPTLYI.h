#ifndef DEF_lib_LCD_i2c_SPTLYI_H
#define DEF_lib_LCD_i2c_SPTLYI_H

#include "mbed.h"


class LCD_I2C : public I2C
{
public:
    LCD_I2C(PinName pin_sda, PinName pin_scl, PinName pin_rst, int address);

    int clear(void);
    int return_home_cursor(void);
    int turn_on_cursor(void);
    int turn_off_cursor(void);
    int turn_on_display(void);
    int turn_off_display(void);
    int set_position_cursor(int position);
    void shift_left_cursor(int n);
    void shift_right_cursor(int n);
    int shift_line_cursor(void);
    void shift_left_display(int n);
    void shift_right_display(int n);
    int enable_auto_shift_right_display(void);
    int enable_auto_shift_left_display(void);
    int disable_auto_shift_display(void);
    int auto_shift_right_cursor(void);
    int auto_shift_left_cursor(void);
    
    int putnc(char *s, int n);
    int print(char c);
    int print(char c1, char c2);
    int print(char c1, char c2, char c3);
    int print(int nb);
    int print(double nb);
    int print(char *s);
    int print(char *s, int nb);
    int print(char *s, double nb);
    int print(char *s, int nb1, int nb2);
    int print(char *s, int nb1, double nb2);
    int print(char *s, double nb1, int nb2);
    int print(char *s, double nb1, double nb2);
    int print(char *s, double nb1, double nb2, double nb3);
    int print(char *s, double nb1, double nb2, int nb3);
    int print(char *s, double nb1, int nb2, double nb3);
    int print(char *s, double nb1, int nb2, int nb3);
    int print(char *s, int nb1, double nb2, double nb3);
    int print(char *s, int nb1, double nb2, int nb3);
    int print(char *s, int nb1, int nb2, double nb3);
    int print(char *s, int nb1, int nb2, int nb3);
    int print(char *s, char c1);
    int print(char *s, char c1, int nb);
    int print(char *s, char c1, double nb);
    int print(char *s, char c1, int nb1, int nb2);
    int print(char *s, char c1, int nb1, double nb2);
    int print(char *s, char c1, double nb1, int nb2);
    int print(char *s, char c1, double nb1, double nb2);
    int print(char *s, char c1, double nb1, double nb2, double nb3);
    int print(char *s, char c1, double nb1, double nb2, int nb3);
    int print(char *s, char c1, double nb1, int nb2, double nb3);
    int print(char *s, char c1, double nb1, int nb2, int nb3);
    int print(char *s, char c1, int nb1, double nb2, double nb3);
    int print(char *s, char c1, int nb1, double nb2, int nb3);
    int print(char *s, char c1, int nb1, int nb2, double nb3);
    int print(char *s, char c1, int nb1, int nb2, int nb3);
    int print(char *s, char c1, char c2);
    int print(char *s, char c1, char c2, int nb);
    int print(char *s, char c1, char c2, double nb);
    int print(char *s, char c1, char c2, int nb1, int nb2);
    int print(char *s, char c1, char c2, int nb1, double nb2);
    int print(char *s, char c1, char c2, double nb1, int nb2);
    int print(char *s, char c1, char c2, double nb1, double nb2);
    int print(char *s, char c1, char c2, double nb1, double nb2, double nb3);
    int print(char *s, char c1, char c2, double nb1, double nb2, int nb3);
    int print(char *s, char c1, char c2, double nb1, int nb2, double nb3);
    int print(char *s, char c1, char c2, double nb1, int nb2, int nb3);
    int print(char *s, char c1, char c2, int nb1, double nb2, double nb3);
    int print(char *s, char c1, char c2, int nb1, double nb2, int nb3);
    int print(char *s, char c1, char c2, int nb1, int nb2, double nb3);
    int print(char *s, char c1, char c2, int nb1, int nb2, int nb3);
private:
    DigitalInOut m_pin_rst;
    int m_address;
    
    int init(void);
    void reset(void);
    int putnb2(int nb);
};

#endif
