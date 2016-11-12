#include <stdio.h>


#include <ev3.h>
#include <ev3_light.h>

#include <brick.h>


#ifdef __WIN32__
    #include <windows.h>
#else
    #include <unistd.h>
    #define Sleep(usec) usleep((usec) * 1000)
#endif

#define SPEED_LINEAR    100  /* Motor speed for linear motion, in percents */
#define SPEED_CIRCULAR  100  /* ... for circular motion */

#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )


int max_speed;              /* Motor maximal speed (will be detected) */
int speed_linear;
int speed_circular;

typedef enum {
    STOP,
    FORTH,
    BACK,
    LEFT,
    RIGHT
} DIRECTION;

typedef enum {
    OFF,
    RED,
    GREEN,
    ORANGE,
} LIGHT;


int connect()
{
    if (ev3_init() < 1) {
        printf("An error occurred trying to initiate brick connection.\n");
        return 0;
    }

    while (ev3_tacho_init() < 1)
        Sleep(1000);

    return 1;
}

int setup()
{
    if (tacho_is_plugged(MOTOR_BOTH, TACHO_TYPE__NONE_)) {
        max_speed = tacho_get_max_speed(MOTOR_LEFT, 0);
        tacho_reset(MOTOR_BOTH);
        speed_linear = max_speed * SPEED_LINEAR / 100;
        speed_circular = max_speed * SPEED_CIRCULAR / 100;
        return 1;
    } else {
        printf("An error occurred trying to detect plugged motors.\n");
        return 0;
    }
}

void light(LIGHT l)
{
    switch (l) {
    case OFF:
        set_light(LIT_LEFT, LIT_OFF);
        set_light(LIT_RIGHT, LIT_OFF);
        break;
    case RED:
        set_light(LIT_LEFT, LIT_RED);
        set_light(LIT_RIGHT, LIT_RED);
        break;
    case GREEN:
        set_light(LIT_LEFT, LIT_GREEN);
        set_light(LIT_RIGHT, LIT_GREEN);
        break;
    case ORANGE:
        set_light(LIT_LEFT, LIT_AMBER);
        set_light(LIT_RIGHT, LIT_AMBER);
        break;
    }
}

void move(DIRECTION d)
{
    switch (d) {
    case STOP:
        tacho_stop(MOTOR_BOTH);
        light(ORANGE);
        break;
    case FORTH:
        tacho_set_speed_sp(MOTOR_BOTH, -speed_linear);
        tacho_run_forever(MOTOR_BOTH);
        light(GREEN);
        break;
    case BACK:
        tacho_set_speed_sp(MOTOR_BOTH, speed_linear);
        tacho_run_forever(MOTOR_BOTH);
        light(RED);
        break;
    case RIGHT:
        tacho_set_speed_sp(MOTOR_LEFT, -speed_circular);
        tacho_set_speed_sp(MOTOR_RIGHT, speed_circular);
        tacho_run_forever(MOTOR_BOTH);
        break;
    case LEFT:
        tacho_set_speed_sp(MOTOR_LEFT, speed_circular);
        tacho_set_speed_sp(MOTOR_RIGHT, -speed_circular);
        tacho_run_forever(MOTOR_BOTH);
        break;
    }
}

void poweroff()
{
    tacho_stop(MOTOR_BOTH);
    light(OFF);
    ev3_poweroff();
}
