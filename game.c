#include "game.h"

#include <math.h>

#include "camera.h"
#include "game_file_handler.h"
#include "plant.h"
#include "quaternions.h"
// testing
#define MAIN_INSTRUCTION_LENGTH 3
#define SECONDARY_INSTRUCTION_LENGTH 24

Player player;
Quat camera_angle;
Camera cam;

Species *species_book;
int num_species;

Plant *plants;
int num_plants;

int *linker_map;

PlantEdge edge_buffers[MAX_NUM_PLANTS][MAX_EDGE_BUFFER_SIZE];
int edge_buffer_lengths[MAX_NUM_PLANTS];
float horizontal_rotation;

void init_game() {
    cam.rotation = &camera_angle;
    horizontal_rotation = 0;
    set_camera_angle(&cam, PI / 0.5, 0, 1, 0);
    set_camera_position(&cam, 0, 0, 0);
    player.x = 0;
    player.y = PLAYER_HEIGHT;
    player.z = 0;
    player.xvel = 0;
    player.yvel = 0;
    player.zvel = 0;

    num_species = load_num_species();
    num_plants = load_num_plants();

    /*initialise the size of each edge buffer to 1*/
    for (int i = 0; i < num_plants; i++) {
        edge_buffer_lengths[i] = 1;
    }

    /*initialise species book*/
    if (!num_species) {
        num_species = INITIAL_SPECIES_NUMBER;
        change_num_species(INITIAL_SPECIES_NUMBER);
        species_book = malloc(INITIAL_SPECIES_NUMBER * sizeof(Species));

        for (int i = 0; i < INITIAL_SPECIES_NUMBER; i++) {
            init_species(&species_book[i], 1, NUM_SYMBOLS);
            int8_t rule_0[MAIN_INSTRUCTION_LENGTH] = {0, 0, COIL_LEFT};
            int8_t rule_1[SECONDARY_INSTRUCTION_LENGTH] = {
                0,         RIGHT, PUSH_POS, PUSH_VERT, PUSH_POS, PUSH_VERT, 1,     POP_VERT,
                POP_POS,   LEFT,  1,        POP_VERT,  POP_POS,  LEFT,      0,     PUSH_POS,
                PUSH_VERT, LEFT,  0,        1,         POP_VERT, POP_POS,   RIGHT, 1};
            add_species_rule(&species_book[i], 0, MAIN_INSTRUCTION_LENGTH, rule_0);
            add_species_rule(&species_book[i], 1, SECONDARY_INSTRUCTION_LENGTH, rule_1);
        }

    } else {
        species_book = malloc(num_species * sizeof(Species));
        for (int i = 0; i < num_species; i++) {
            init_species(&species_book[i], 1, NUM_SYMBOLS);
            load_from_species_map(&species_book[i], i);
        }
    }

    /*initialise plants*/
    if (!num_plants) {
        plants = malloc(INITIAL_PLANT_NUMBER * sizeof(Plant));
        linker_map = malloc(INITIAL_PLANT_NUMBER * sizeof(int));
        num_plants = INITIAL_PLANT_NUMBER;
        store_num_plant(&num_plants);
        int plant_grid_side_length = (int)sqrt(INITIAL_PLANT_NUMBER);
        for (int i = 0; i < plant_grid_side_length; i++) {
            for (int j = 0; j < plant_grid_side_length; j++) {
                int index = i * plant_grid_side_length + j;
                init_plant(&(plants[index]), i * 300, 0, j * 400,
                           &species_book[(index + num_species - 1) % num_species]);
                get_plant_nodes(plants[index], edge_buffers[index], &edge_buffer_lengths[index]);
                linker_map[index] = 0;
            }
        }
    } else {
        plants = malloc(num_plants * sizeof(Plant));
        linker_map = malloc(num_plants * sizeof(int));
        for (int i = 0; i < num_plants; i++) {
            load_plant(&(plants[i]), &species_book, &linker_map, i);
            get_plant_nodes(plants[i], edge_buffers[i], &edge_buffer_lengths[i]);
        }
    }
}

void quit_game() {
    for (int i = 0; i < num_species; i++) {
        add_to_species_map(&species_book[0], i);
    }

    for (int i = 0; i < num_plants; i++) {
        save_plant(&plants[i], &linker_map[i], i);
        free_plant(&plants[i]);
    }
}

Plant *get_plant(int index) {
    return &(plants[index]);
}

int get_num_plants() {
    return num_plants;
}

void update_plant_on_random_tick(int index) {
    int num = rand() % (PLANT_UPDATE_TICK_CHANCE + 1);
    if (num == 0) {
        update_plant(&plants[index]);
        get_plant_nodes(plants[index], edge_buffers[index], &edge_buffer_lengths[index]);
    }
}

void update_plants() {
    for (int i = 0; i < num_plants; i++) {
        update_plant_on_random_tick(i);
    }
}

void update_game() {
    update_plants();
}

PlantEdge *get_edge_buffer(int index) {
    return edge_buffers[index];
}

int get_edge_buffer_length(int index) {
    return edge_buffer_lengths[index];
}

void move_camera(float x, float y, float z, float angle, float axis_x, float axis_y, float axis_z) {
    translate_camera(&cam, x, y, z);
    rotate_camera(&cam, angle, axis_x, axis_z, axis_y);
}

void set_horizontal_rotation(float angle) {
    horizontal_rotation = angle;
}

Camera *get_camera() {
    return &cam;
}

void move_player() {
    player.x += player.xvel;
    player.y += player.yvel;
    player.z += player.zvel;
    cam.x = player.x * CAMERA_SCALE_FACTOR;
    cam.y = player.y * CAMERA_SCALE_FACTOR;
    cam.z = player.z * CAMERA_SCALE_FACTOR;
}

void set_game_camera_angle(float angle, float x, float y, float z) {
    set_camera_angle(&cam, angle, x, y, z);
}

/*left-right, forward-backward in relation to axis of camera rotation and y as absolute with
 * respect to coordinate system.*/
void set_player_velocity(int d_lr, int dy, int d_fb, float speed) {
    float normalisation_factor;
    float norm = sqrt((d_lr * d_lr) + (d_fb * d_fb));
    if (norm > 0) {
        normalisation_factor = 1 / norm;
    } else {
        normalisation_factor = 0;
    }

    player.yvel = dy;
    /*right handed coordinate system so moving forwards is in the negative z direction.*/
    if ((d_fb == 1) && (d_lr == 0)) {
        player.xvel = -normalisation_factor * sin(horizontal_rotation) * speed;
        player.zvel = -normalisation_factor * cos(horizontal_rotation) * speed;
    } else if ((d_fb == -1) && (d_lr == 0)) {
        player.xvel = normalisation_factor * sin(horizontal_rotation) * speed;
        player.zvel = normalisation_factor * cos(horizontal_rotation) * speed;
    } else if ((d_lr == -1) && (d_fb == 0)) {
        player.xvel = normalisation_factor * cos(horizontal_rotation) * speed;
        player.zvel = -normalisation_factor * sin(horizontal_rotation) * speed;
    } else if ((d_lr == 1) && (d_fb == 0)) {
        player.xvel = -normalisation_factor * cos(horizontal_rotation) * speed;
        player.zvel = normalisation_factor * sin(horizontal_rotation) * speed;
    }
    /*diagonal movement*/
    else if ((d_fb == 1) && (d_lr == 1)) {
        player.xvel =
            -normalisation_factor * (sin(horizontal_rotation) + cos(horizontal_rotation)) * speed;
        player.zvel =
            -normalisation_factor * (cos(horizontal_rotation) - sin(horizontal_rotation)) * speed;
    } else if ((d_fb == -1) && (d_lr == 1)) {
        player.xvel =
            normalisation_factor * (sin(horizontal_rotation) - cos(horizontal_rotation)) * speed;
        player.zvel =
            normalisation_factor * (cos(horizontal_rotation) + sin(horizontal_rotation)) * speed;
    } else if ((d_fb == 1) && (d_lr == -1)) {
        player.xvel =
            -normalisation_factor * (sin(horizontal_rotation) - cos(horizontal_rotation)) * speed;
        player.zvel =
            -normalisation_factor * (cos(horizontal_rotation) + sin(horizontal_rotation)) * speed;
    } else if ((d_fb == -1) && (d_lr == -1)) {
        player.xvel =
            normalisation_factor * (sin(horizontal_rotation) + cos(horizontal_rotation)) * speed;
        player.zvel =
            normalisation_factor * (cos(horizontal_rotation) - sin(horizontal_rotation)) * speed;
    } else {
        player.xvel = 0;
        player.zvel = 0;
    }
}

Player *get_player() {
    return &player;
}