/** LCD i2c SPTLYI class
*
* @purpose       library for i2c LCD
*
* Utilisée pour écrire sur l'afficheur i2c SPTLYI 2x16.
*
* http://fr.farnell.com/midas/mccog21605b6w-sptlyi/lcd-cog-2x16-stn-vert-b-l-i2c/dp/2063208
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
* int main()
* {
*     while(1)
*     {
*         for(int i = 0; i < 9999; i++)
*         {
*             LCD.clear();
*             LCD.print(i);
*             #if MBED_MAJOR_VERSION > 5
*             ThisThread::sleep_for(250ms);
*             #else
*             ThisThread::sleep_for(2500);
*             #endif
*         }
*     }
* }
* @endcode
* @file          lib_LCD_i2c_SPTLYI.h 
* @date          Jan 2014
* @author        Yannic Simon
*/
#ifndef DEF_lib_LCD_i2c_SPTLYI_H
#define DEF_lib_LCD_i2c_SPTLYI_H

#include "mbed.h"
#include <cstdarg>

/** LCD_I2C class
 */
class LCD_I2C : public I2C
{
public:
    /** Creer une instance LCD_I2C
    * sur les pins sda, scl, rst avec 1 des 4 adresses possiblent 0x7C 0x7D 0x7E 0x7F
    *
    * @param pin_sda par defaut p28
    * @param pin_scl par defaut p27
    * @param pin_rst par defaut p26
    * @param address par defaut 0x7C
    */
    LCD_I2C(PinName pin_sda, PinName pin_scl, PinName pin_rst, int address = 0x7C);

    /** Efface l'afficheur et renvoie le curseur en position X = 0 et Y = 0
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int clear(void);
    
    /** renvoie le curseur en position X = 0 et Y = 0
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int return_home_cursor(void);
    
    /** positionne le curseur sur la ligne (0 à 39)
    *
    * @param X est la position sur la ligne
    * @returns acknoledge write i2c
    */
    int set_position_cursor(int X);
    
    /** positionne le curseur horizontalement (0 à 39) et verticalement (0 à 1)
    *
    * @param X est la position sur la ligne
    * @param Y est la position sur la colone
    * @returns acknoledge write i2c
    */
    int set_position_cursor(int X, int Y);
    
    /** renvoie la position horizontale du curseur
    *
    * @param aucun
    * @returns X est la position sur la ligne (0 à 39)
    */
    int get_X_position_cursor(void);
    
    /** renvoie la position verticale du curseur
    *
    * @param aucun
    * @returns Y est la position sur la colone (0 à 1)
    */
    int get_Y_position_cursor(void);
    
    /** positionne le curseur au début de la ligne suivante
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int shift_line_cursor(void);
    
    /** active l'affichage du curseur
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int turn_on_cursor(void);
    
    /** désactive l'affichage du curseur
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int turn_off_cursor(void);
    
    /** active l'affichage
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int turn_on_display(void);
    
    /** désactive l'affichage
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int turn_off_display(void);
    
    /** déplace le curseur vers la gauche
    *
    * @param n est le nombre de déplacement du curseur
    * @returns aucun
    */
    void shift_left_cursor(int n=1);
    
    /** déplace le curseur vers la droite
    *
    * @param n est le nombre de déplacement du curseur
    * @returns aucun
    */
    void shift_right_cursor(int n=1);
    
    /** déplace l'affichage vers la gauche
    *
    * @param n est le nombre de déplacement de l'affichage
    * @returns aucun
    */
    void shift_left_display(int n=1);
    
    /** déplace l'affichage vers la droite
    *
    * @param n est le nombre de déplacement de l'affichage
    * @returns aucun
    */
    void shift_right_display(int n=1);
    
    /** active le déplacement automatique de l'affichage vers la droite
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int enable_auto_shift_right_display(void);
    
    /** active le déplacement automatique de l'affichage vers la gauche
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int enable_auto_shift_left_display(void);
    
    /** désactive le déplacement automatique de l'affichage
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int disable_auto_shift_display(void);
    
    /** configure le déplacement automatique du curseur vers la droite
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int auto_shift_right_cursor(void);
    
    /** configure le déplacement automatique du curseur vers la droite
    *
    * @param aucun
    * @returns acknoledge write i2c
    */
    int auto_shift_left_cursor(void);
    
    /** renvoie le caractere se trouvant à la position du curseur
    *
    * @param aucun
    * @returns le caractere se trouvant à position du curseur
    */   
    char read(void);
    
    /** renvoie le caractere se trouvant à la position spécifiée
    *
    * @param X est la position sur la ligne
    * @param Y est la position sur la colone
    * @returns le caractere se trouvant à position X et Y
    */   
    char read(int X, int Y);
    
    /** affiche une variable sur l'afficheur
    *
    * @param nb la variable à afficher
    * @returns acknoledge write i2c
    */
    int print(int nb);
    int print(unsigned int nb);
    int print(short nb);
    int print(unsigned short nb);
    int print(long long nb);
    int print(unsigned long long nb);
    int print(float nb);
    int print(double nb);
    int print(char c);
    int print(char *s);
    
    /** affiche une chaine formaté sur l'afficheur
    *
    * @param format la chaine de format à afficher
    * @param ... les arguments à afficher
    * @returns acknoledge write i2c
    */
    int print(const char *format, ... );
    
    private:
    DigitalInOut m_pin_rst;
    int m_address;
    
    int init(void);
    void reset(void);
    int putnc(char *s, int n);
    
    void X_move_position(int n);
    int X40_position_cursor;
    int X80_position_cursor;
    int Y2_position_cursor;
    char LCD_Data_Register[80];
};

#endif
