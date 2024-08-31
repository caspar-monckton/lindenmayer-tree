#include "game_file_handler.h"

#include <sys/stat.h>

void store_num_plant(int *amount) {
    FILE *fp = fopen("./game_objects/num_plants.bin", "wb");
    fwrite(amount, sizeof(int32_t), 1, fp);
    fclose(fp);
}

int load_num_plants() {
    int num_plants;
    struct stat buffer;

    if (stat("./game_objects/num_plants.bin", &buffer) != -1) {
        FILE *fp = fopen("./game_objects/num_plants.bin", "rb");
        fread(&num_plants, sizeof(int32_t), 1, fp);
        fclose(fp);
    } else {
        num_plants = 0;
    }
    return num_plants;
}

void save_plant(Plant *plant, int *species_linker, int index) {
    /* ints (16 bit):
     * 1. current_size
     * 2. current_form_index
     * floats (32 bit):
     * 3. seed_x
     * 4. seed_y
     * 5. seed_z
     * int8_t *(current_size * 8 bits)
     * 6. link to the species map
     * 7. form i for i in current_form
     */
    char plant_file_name[22];
    sprintf(plant_file_name, "./game_objects/plant_%d.bin", index);
    FILE *fp = fopen(plant_file_name, "wb");

    fwrite(&(plant->current_size), sizeof(int32_t), 1, fp);
    fwrite(&(plant->current_form_index), sizeof(int32_t), 1, fp);
    fwrite(&(plant->seed_x), sizeof(float), 1, fp);
    fwrite(&(plant->seed_y), sizeof(float), 1, fp);
    fwrite(&(plant->seed_z), sizeof(float), 1, fp);
    fwrite(species_linker, sizeof(int32_t), 1, fp);
    fwrite(plant->current_form, sizeof(int8_t), plant->current_size, fp);

    fclose(fp);
}

void load_plant(Plant *plant, Species **species_map_buffer, int **index_buffer, int plant_index) {
    char plant_file_name[22];
    int32_t current_size, current_form_index, species_linker;
    float seed_x, seed_y, seed_z;
    int8_t *saved_form = malloc(MAX_NUM_PLANTS * sizeof(int8_t));
    sprintf(plant_file_name, "./game_objects/plant_%d.bin", plant_index);
    FILE *fp = fopen(plant_file_name, "rb");

    fread(&current_size, sizeof(int32_t), 1, fp);
    fread(&current_form_index, sizeof(int32_t), 1, fp);
    fread(&seed_x, sizeof(float), 1, fp);
    fread(&seed_y, sizeof(float), 1, fp);
    fread(&seed_z, sizeof(float), 1, fp);
    fread(&species_linker, sizeof(int32_t), 1, fp);
    fread(saved_form, sizeof(int8_t), current_size, fp);

    init_plant(plant, seed_x, seed_y, seed_z, &((*species_map_buffer)[species_linker]));
    (*index_buffer)[plant_index] = species_linker;
    plant->current_form_index = current_form_index;
    plant->current_size = current_size;

    for (int i = 0; i < plant->current_size; i++) {
        (plant->current_form)[i] = saved_form[i];
    }
    fclose(fp);
    free(saved_form);
}

int load_num_species() {
    int num_species;
    struct stat buffer;
    if (stat("./game_objects/num_species.bin", &buffer) != -1) {
        FILE *fp = fopen("./game_objects/num_species.bin", "rb");
        fread(&num_species, sizeof(int32_t), 1, fp);
        fclose(fp);
    } else {
        num_species = 0;
    }
    return num_species;
}

void change_num_species(int32_t amount) {
    int num_species = load_num_species();
    num_species += amount;
    FILE *fp = fopen("./game_objects/num_species.bin", "wb");
    fwrite(&num_species, sizeof(int32_t), 1, fp);
    fclose(fp);
}

void add_to_species_map(Species *species, int index) {
    char species_map_file_name[22];
    sprintf(species_map_file_name, "./game_objects/species_map_%d.bin", index);

    FILE *file_ptr;
    file_ptr = fopen(species_map_file_name, "wb");
    /*
     * ints (16 bit):
     * 1. seed
     * 2. intruction_length
     * int * (instruction_length * 16 bits)
     * 3. instruction_lengths
     * 4. int8_t ** ((sum(instruction_length i for i in instruction_lengths)) * 8 bits)
     */
    fwrite(&(species->seed), sizeof(int32_t), 1, file_ptr);
    fwrite(&(species->instruction_length), sizeof(int32_t), 1, file_ptr);

    for (int i = 0; i < species->instruction_length; i++) {
        fwrite(&((species->instruction_lengths)[i]), sizeof(int32_t), 1, file_ptr);
    }

    for (int i = 0; i < species->instruction_length; i++) {
        for (int j = 0; j < (species->instruction_lengths)[i]; j++) {
            fwrite(&((species->instructions)[i][j]), sizeof(int8_t), 1, file_ptr);
        }
    }

    fclose(file_ptr);
}

void load_from_species_map(Species *species, int index) {
    char species_map_file_name[22];
    sprintf(species_map_file_name, "./game_objects/species_map_%d.bin", index);
    FILE *file_ptr;
    file_ptr = fopen(species_map_file_name, "rb");

    if (file_ptr == NULL) {
        printf("Failed to load species data.\n");
        return;
    }
    /*
     * ints (16 bit):
     * 1. seed
     * 2. intruction_length
     * int * (instruction_length * 16 bits)
     * 3. instruction_lengths
     * 4. int8_t ** ((sum(instruction_length i for i in instruction_lengths)) * 8 bits)
     */
    fread(&(species->seed), sizeof(int32_t), 1, file_ptr);
    fread(&(species->instruction_length), sizeof(int32_t), 1, file_ptr);

    for (int i = 0; i < species->instruction_length; i++) {
        fread(&((species->instruction_lengths)[i]), sizeof(int32_t), 1, file_ptr);
    }

    for (int i = 0; i < species->instruction_length; i++) {
        int8_t *current_species_rule = malloc(((species->instruction_lengths)[i]) * sizeof(int8_t));
        fread(&(*current_species_rule), sizeof(int8_t), (species->instruction_lengths)[i],
              file_ptr);
        add_species_rule(species, i, (species->instruction_lengths)[i], current_species_rule);
    }

    fclose(file_ptr);
}