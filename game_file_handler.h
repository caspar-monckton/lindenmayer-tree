#ifndef GAME_FILE_HANDLER_C_
#define GAME_FILE_HANDLER_C_
#include "plant.h"
#include "shared.h"

void save_plant(Plant *plant, int *species_linker, int index);

void load_plant(Plant *plant, Species **species_map_buffer, int **index_buffer, int plant_index);

void store_num_plant(int *amount);

void add_to_species_map(Species *species, int index);

void load_from_species_map(Species *species, int index);

int load_num_species(void);

int load_num_plants(void);

void change_num_species(int32_t amount);

void delete_from_species_map(int index);

#endif