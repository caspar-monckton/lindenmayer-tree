#ifndef DEFS_H_
#define DEFS_H_

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRAME_RATE_CAP 60

// keyboard and input
#define NUM_KEYS 6
#define key_W 0
#define key_A 1
#define key_S 2
#define key_D 3
#define key_SPACE 4
#define key_ESC 5

#define CAMERA_DEPTH 500
#define PLAYER_VELOCITY 3
#define PLAYER_HEIGHT 100
#define CAMERA_SCALE_FACTOR 1

#define PI 3.1415926535
#define SQRT_3 1.7321
#define SQRT_2 1.4142

#define LINE_DETAIL 1800
#define DEFAULT_DOT_SIZE 4
#define LINE_DOT_SIZE 2

#define PLANT_FORWARD_AMOUNT 20

#define ANGLE_MASK 0b01100000
#define THETA_MASK 0b00100000
#define ANGLE_MULTIPLIER_MASK 0b00011111
#define PHI_MASK 0b01000000

#define MAX_PLANT_FORM_SIZE 4000
#define MAX_EDGE_BUFFER_SIZE 1000

#define NUM_SYMBOLS 127
#define THETA (PI / 16)
#define PHI (PI / 16)
#define MAX_STACK_CAPACITY 64
#define MAX_NUM_PLANTS 2000
#define PLANT_UPDATE_TICK_CHANCE 100

#define INITIAL_SPECIES_NUMBER 10
#define INITIAL_PLANT_NUMBER 64

#define LEFT 0b01011110
#define RIGHT 0b01000100
#define COIL_RIGHT 0b00100001
#define COIL_LEFT 0b00111111
#define PUSH_POS 2
#define POP_POS 3
#define PUSH_VERT 6
#define POP_VERT 7
#define PUSH_HOR 4
#define POP_HOR 5
#endif