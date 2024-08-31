#include "plant.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "process_3d.h"
#include "quaternions.h"
#include "stack_handler.h"

/* Mutates original form, size, and instruction pointer */
void apply_instruction(int8_t **current_form_pointer, int8_t **instructions, int *current_size,
                       int *current_form_index, int *instruction_lengths) {
    if (!current_form_pointer || !*current_form_pointer || !instructions || !current_size ||
        !current_form_index || !instruction_lengths) {
        printf("Invalid pointers provided\n");
        return;
    }

    int end_size = *current_size - *current_form_index - 1;
    int replacement_size = instruction_lengths[(*current_form_pointer)[*current_form_index]];

    // Check for valid replacement size
    if (replacement_size < 0) {
        printf("Invalid replacement size\n");
        return;
    }

    // Ensure we do not exceed the maximum size of the plant form
    if (*current_size + replacement_size - 1 >= MAX_PLANT_FORM_SIZE) {
        printf("MAX FORM SIZE REACHED\n");
        return;
    }

    // Move the elements at the end of the array forward by the length of the expanded symbol
    memmove(&(*current_form_pointer)[*current_form_index + replacement_size],
            &(*current_form_pointer)[*current_form_index + 1], end_size * sizeof(int8_t));

    // Apply the replacement rule to the current element of the form
    int8_t index = (*current_form_pointer)[*current_form_index];
    memcpy(&(*current_form_pointer)[*current_form_index], instructions[index],
           replacement_size * sizeof(int8_t));

    // Update parameters
    *current_size += replacement_size - 1;
    *current_form_index += replacement_size;

    // Wrap around the index if it reaches the end
    if (*current_form_index >= *current_size) {
        *current_form_index = 0;
    }
}

/*allocates a spot for each instruction and initialises them to map to themselves*/
void init_species(Species *species, int seed, int instruction_length) {
    species->seed = seed;
    species->instruction_length = instruction_length;
    species->instruction_lengths = malloc(instruction_length * sizeof(int));
    for (int i = 0; i < instruction_length; i++) {
        (species->instruction_lengths)[i] = 1;
    }
    species->instructions = malloc(instruction_length * sizeof(int8_t *));
    for (int i = 0; i < instruction_length; i++) {
        (species->instructions)[i] = malloc(sizeof(int8_t));
        (species->instructions)[i][0] = i;
    }
}

void free_species(Species *species) {
    free(species->instruction_lengths);
    for (int i = 0; i < species->instruction_length; i++) {
        free((species->instructions)[i]);
    }
    free(species->instructions);
}

void add_species_rule(Species *species, int index, int rule_size, int8_t *rule) {
    (species->instruction_lengths)[index] = rule_size;
    (species->instructions)[index] =
        realloc((species->instructions)[index], rule_size * sizeof(int8_t));
    for (int i = 0; i < rule_size; i++) {
        ((species->instructions)[index])[i] = rule[i];
    }
}

void init_plant(Plant *plant, float seed_x, float seed_y, float seed_z, Species *species) {
    plant->current_form_index = 0;
    plant->current_size = 1;
    plant->seed_x = seed_x;
    plant->seed_y = seed_y;
    plant->seed_z = seed_z;
    plant->current_form = malloc(MAX_PLANT_FORM_SIZE * sizeof(int8_t));
    (plant->species) = species;
    (plant->current_form)[0] = species->seed;
}

void free_plant(Plant *plant) {
    free(plant->current_form);
}

void print_plant_form(Plant *plant) {
    printf("current form: ");
    for (int i = 0; i < plant->current_size; i++) {
        if (i == plant->current_form_index) {
            printf("[");
        }
        printf(" %d ", (plant->current_form)[i]);
        if (i == plant->current_form_index) {
            printf("]");
        }
    }
    printf("\n");
}

void print_species_rule(Species *species) {
    printf("species: \n");
    for (int i = 0; i < 2; i++) {
        printf("rule %d: \n", i);
        for (int j = 0; j < (species->instruction_lengths)[i]; j++) {
            printf("%d ", (species->instructions)[i][j]);
        }
        printf("\n");
    }
}

void get_plant_nodes(Plant plant, PlantEdge *edge_buffer, int *edge_buffer_length) {
    Quat horizontal_rotation, vertical_rotation, overall_rotation;

    float x = plant.seed_x, y = plant.seed_y, z = plant.seed_z, theta = 0, phi = PI / 2;
    int current_edge_index = 0;

    FloatStack x_stack, y_stack, z_stack, theta_stack, phi_stack;
    init_float_stack(&x_stack, MAX_STACK_CAPACITY);
    init_float_stack(&y_stack, MAX_STACK_CAPACITY);
    init_float_stack(&z_stack, MAX_STACK_CAPACITY);
    init_float_stack(&theta_stack, MAX_STACK_CAPACITY);
    init_float_stack(&phi_stack, MAX_STACK_CAPACITY);

    /*interpret symbol as drawing instruction and apply the appropriate transformations
     * to positions and angles.*/
    /*0 --> draw line in direction of angles and update position
     *1 --> placeholder
     *2 --> push position
     *3 --> pop position
     *4 --> push horizontal angle
     *5 --> pop horizontal angle
     *6 --> push vertical angle
     *7 --> pop vertical angle
     *32[_____] --> rotate horizontal angle by 5 bit integer
     *64[_____] --> rotate vertical angle by 5 bit integer
     */
    for (int i = 0; i < plant.current_size; i++) {
        float new_x = x, new_y = y, new_z = z;
        int drawing = 0;
        if (plant.current_form[i] == 0) {
            new_z += PLANT_FORWARD_AMOUNT;
            drawing = 1;
        } else if (plant.current_form[i] & ANGLE_MASK) {
            if ((plant.current_form[i] & ANGLE_MASK) == THETA_MASK) {
                theta += ((plant.current_form[i] & ANGLE_MULTIPLIER_MASK) * THETA);
                if (theta >= 2 * PI) {
                    theta -= (2 * PI);
                }
                // printf("theta %f\n", theta);
            } else if ((plant.current_form[i] & ANGLE_MASK) == PHI_MASK) {
                phi += ((plant.current_form[i] & ANGLE_MULTIPLIER_MASK) * PHI);

                if (phi > 2 * PI) {
                    phi -= (2 * PI);
                }
                // printf("phi %f\n", phi);
                //  printf("angle multiplier: %d\n", (plant.current_form[i] &
                //  ANGLE_MULTIPLIER_MASK)); printf("vertical angle: %f\n", phi);
            }
        } else if (plant.current_form[i] == 2) {
            push_float_stack(&x_stack, x);
            push_float_stack(&y_stack, y);
            push_float_stack(&z_stack, z);

        } else if (plant.current_form[i] == 3) {
            x = pop_float_stack(&x_stack);
            y = pop_float_stack(&y_stack);
            z = pop_float_stack(&z_stack);

        } else if (plant.current_form[i] == 4) {
            push_float_stack(&theta_stack, theta);

        } else if (plant.current_form[i] == 5) {
            theta = pop_float_stack(&theta_stack);

        } else if (plant.current_form[i] == 6) {
            push_float_stack(&phi_stack, phi);

        } else if (plant.current_form[i] == 7) {
            phi = pop_float_stack(&phi_stack);
        }

        /*save energy on drawing and rotating new point if no movement has occurred*/
        if (drawing) {
            /*apply any rotations*/
            euler_to_quat(&horizontal_rotation, theta, 0, 1, 0);
            euler_to_quat(&vertical_rotation, phi, 1, 0, 0);
            q_mul(&overall_rotation, &vertical_rotation, &horizontal_rotation);

            float temp_x, temp_y, temp_z;

            rotate_point(new_x, new_y, new_z, &temp_x, &temp_y, &temp_z, x, y, z,
                         &overall_rotation);
            new_x = temp_x;
            new_y = temp_y;
            new_z = temp_z;
            (edge_buffer[current_edge_index]).x1 = x;
            (edge_buffer[current_edge_index]).y1 = y;
            (edge_buffer[current_edge_index]).z1 = z;
            (edge_buffer[current_edge_index]).x2 = new_x;
            (edge_buffer[current_edge_index]).y2 = new_y;
            (edge_buffer[current_edge_index]).z2 = new_z;
            current_edge_index++;

            // printf("=> (%f, %f, %f) -> (%f, %f, %f)", x, y, z, new_x, new_y, new_z);
            x = new_x;
            y = new_y;
            z = new_z;
        }
    }
    *edge_buffer_length = current_edge_index;
}

void update_plant(Plant *plant) {
    apply_instruction(&(plant->current_form), (plant->species)->instructions,
                      &(plant->current_size), &(plant->current_form_index),
                      (*(plant->species)).instruction_lengths);
}
