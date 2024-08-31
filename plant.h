#include "shared.h"

#ifndef PLANT_C_
#define PLANT_C_

void update_plant(Plant *plant);

void init_species(Species *species, int seed, int instruction_length);

void add_species_rule(Species *species, int index, int rule_size, int8_t *rule);

void init_plant(Plant *plant, float seed_x, float seed_y, float seed_z, Species *species);

void print_plant_form(Plant *plant);

void print_species_rule(Species *species);

void get_plant_nodes(Plant plant, PlantEdge *edge_buffer, int *edge_buffer_length);

void free_plant(Plant *plant);

void free_species(Species *species);
#endif