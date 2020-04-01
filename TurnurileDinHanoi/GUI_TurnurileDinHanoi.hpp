
#define NO_BETE                         3
#define NO_DISCURI                      5
#define NO_MAX_DISCURI                  10
#define NO_SECUNDE                      1
#define ITEM_RESET                      20
#define ITEM_REZOLVA                    21
#define ITEM_STOP                       22

#define CEAS_W                          200
#define CEAS_H                          20

#define WIN_L                           400
#define WIN_T                           200
#define WIN_W                           1000
#define WIN_H                           500

#define WIN_MIN_W                       600
#define WIN_MIN_H                       300

#define BAZA_L                          100
#define BAZA_T                          350
#define BAZA_W                          800
#define BAZA_H                          30

#define BAT_L(x)                        (size_t)(BAZA_L+(x+1)*BAZA_W/(NO_BETE+1))
#define BAT_H                           200
#define BAT_T                           BAZA_T-BAT_H
#define BAT_W                           4

#define DISC_CLEARENCE                  BAZA_W/((NO_DISCURI+1)*(NO_BETE+1))
#define DISC_W(x)                       (size_t)(BAZA_W/(NO_BETE+1) - (NO_DISCURI-x-1)*DISC_CLEARENCE)
#define DISC_H                          (size_t)((BAT_H-50)/NO_DISCURI)

#define PROXIMITY_W                     (size_t)(BAZA_W/8 - 10)
#define COEF_CULOARE                    100
