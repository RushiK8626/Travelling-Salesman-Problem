//Included Required Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "config.h"

// Global variable definitions
Config config;
City city_database[MAX_CITIES];
int total_cities = 0;
int start_city;
int intermediate_cities[MAX_CITIES];
int num_intermediate_cities = 0;

// Global constant definitions
const char *city_prefixes[] = {
    "New", "Old", "North", "South", "East", "West", "Upper", "Lower",
    "Central", "Royal", "Grand", "Green", "Silver", "Golden", "Mountain",
    "River", "Lake", "Spring", "Pine", "Oak", "Stone", "Rock", "Valley",
    "Hill", "Bay", "Port", "Vista", "Glen", "Harbor", "Forest"};

const char *city_suffixes[] = {
    "ville", "town", "berg", "burg", "city", "field", "bridge",
    "ton", "port", "land", "view", "park", "springs", "heights",
    "valley", "ridge", "creek", "grove", "woods", "lake", "hills",
    "point", "center", "mesa", "bank", "falls", "island"};


// Intital Configurations
void initializeConfig(){
    config.maxCities = MAX_CITIES;
    config.maxPopulation = MAX_POPULATION;
    config.maxGenerations = MAX_GENERATIONS;
    config.mutationRate = MUTATION_RATE;
    config.crossoverRate = CROSSOVER_RATE;
}

// Utility Function to Display Help Munu when user passes -h argument in
// Command Line
void displayHelp() {
    printf("Usage: program [options]\n");
    printf("Options:\n");
    printf("  -c [value]    Set MAX_CITIES (default: %d)\n", MAX_CITIES);
    printf("  -p [value]    Set MAX_POPULATION (default: %d)\n", MAX_POPULATION);
    printf("  -g [value]    Set MAX_GENERATIONS (default: %d)\n", MAX_GENERATIONS);
    printf("  -m [value]    Set MUTATION_RATE (default: %.2f)\n", MUTATION_RATE);
    printf("  -x [value]    Set CROSSOVER_RATE (default: %.2f)\n", CROSSOVER_RATE);
    printf("  -h            Display this help menu\n");
}

// Function to parse command-line arguments
void parseArguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            config.maxCities = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config.maxPopulation = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            config.maxGenerations = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            config.mutationRate = atof(argv[++i]);
        } else if (strcmp(argv[i], "-x") == 0 && i + 1 < argc) {
            config.crossoverRate = atof(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0) {
            displayHelp();
            exit(0);
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            displayHelp();
            exit(1);
        }
    }
}

// Utility to load the city database
// It provides the choices for the user to either load the city database from a file
// or generate random cities using combination of predefined prefixes and suffixes
void load_city_database() {
    int choice;
    // Print the Choices for the user
    printf("\nCity Database Creation:\n");
    printf("1. Generate Random City Database\n");
    printf("2. Load City Database from File\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        // Generate Random City Database
        generate_random_city_database();
    } else {
        // Load City Database from File provides by the user
        char filename[100];
        printf("Enter filename for city database: ");
        scanf("%s", filename);

        FILE* file = fopen(filename, "r");
        if (!file) {
            // If file does not exist, print an error message and generate random cities instead
            printf("File not found. Generating random database.\n");
            generate_random_city_database();
            return;
        }

        // Reads the Data from the CSV File
        // Required CSV File Format: ID,CityName,X,Y
        total_cities = 0;
        while (fscanf(file, "%d,%49[^,],%lf,%lf", 
            &city_database[total_cities].id, 
            city_database[total_cities].name, 
            &city_database[total_cities].x_coordinate, 
            &city_database[total_cities].y_coordinate) == 4) {
            total_cities++;
            if (total_cities >= MAX_CITIES) break;
        }
        fclose(file);
    }
}

// Utility To Generate Random City Database
// It Generates 10 - 40 Cities Randomly
void generate_random_city_database() {
    srand(time(NULL));
    total_cities = 10 + rand() % 30;  // 10-40 cities
    int name_counter = 0;
    for (int i = 0; i < total_cities; i++) {
        city_database[i].id = i + 1;
        // sprintf(city_database[i].name, "City_%d", i + 1);
        
        int prefix_index = rand() % (sizeof(city_prefixes) / sizeof(city_prefixes[0]));
        int suffix_index = rand() % (sizeof(city_suffixes) / sizeof(city_suffixes[0]));

        snprintf(city_database[i].name, MAX_CITY_NAME_LENGTH, "%s%s %d",
             city_prefixes[prefix_index],
             city_suffixes[suffix_index],
             ++name_counter);
        city_database[i].x_coordinate = (rand() % 1000) / 10.0;
        city_database[i].y_coordinate = (rand() % 1000) / 10.0;
    }
}
// Display Randomly Generated Or File Read Cities
// Show User Available Cities
void display_cities() {
    printf("\nAvailable Cities:\n");
    for (int i = 0; i < total_cities; i++) {
        printf("%d. %s (%.2f, %.2f)\n", 
            city_database[i].id, 
            city_database[i].name, 
            city_database[i].x_coordinate, 
            city_database[i].y_coordinate
        );
    }
}

/* This Function Allows User To Select The Cities He/She Wants To Visit
*  Then It Asks User To Enter Intermediate Cities Seperated By Space
*/
void input_route_parameters() {
    //Dispplay Available Cities
    display_cities();

    // Input start city
    printf("\nEnter the start/end city ID: ");
    scanf("%d", &start_city);

    // Validate start city
    int start_exists = 0;
    for (int i = 0; i < total_cities; i++) {
        if (city_database[i].id == start_city) {
            start_exists = 1;
            break;
        }
    }
    
    //If Chosen Start City Is Invalid Automatically Choose The First City
    if (!start_exists) {
        printf("Invalid start city. Selecting first city.\n");
        start_city = city_database[0].id;
    }

    // Input intermediate cities
    printf("Enter the number of intermediate cities to visit: ");
    scanf("%d", &num_intermediate_cities);

    printf("Enter the IDs of intermediate cities (space-separated): ");
    for (int i = 0; i < num_intermediate_cities; i++) {
        scanf("%d", &intermediate_cities[i]);
    }

    // Validate intermediate cities
    for (int i = 0; i < num_intermediate_cities; i++) {
        int city_exists = 0;
        for (int j = 0; j < total_cities; j++) {
            if (city_database[j].id == intermediate_cities[i]) {
                city_exists = 1;
                break;
            }
        }
        
        if (!city_exists) {
            printf("Invalid city %d. Removing from route.\n", intermediate_cities[i]);
            // Remove invalid city
            for (int j = i; j < num_intermediate_cities - 1; j++) {
                intermediate_cities[j] = intermediate_cities[j + 1];
            }
            num_intermediate_cities--;
            i--;
        }
    }
}

// Main Algorithm Functions

/* Standard Function To Intitialize The Population Of Routes
*  Population is a 2D array of Route objects
*  Each Route object represents a possible solution
*  The size of the population is determined by the MAX_POPULATION variable
*  The Population Is Initialized By Placing Start And End Stated by the User
*  And Randomly Shuffling Intermediate Stops
*/
void initialize_population(Route* population) {
    srand(time(NULL));

    for (int pop = 0; pop < MAX_POPULATION; pop++) {
        Route* current_route = &population[pop];
        
        // First city is always start
        current_route->path[0] = start_city;
        current_route->path_length = 1;

        // Shuffle intermediate cities
        int temp_intermediate[MAX_CITIES];
        memcpy(temp_intermediate, intermediate_cities, num_intermediate_cities * sizeof(int));
        
        for (int i = num_intermediate_cities - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = temp_intermediate[i];
            temp_intermediate[i] = temp_intermediate[j];
            temp_intermediate[j] = temp;
        }

        // Add shuffled intermediate cities
        for (int i = 0; i < num_intermediate_cities; i++) {
            current_route->path[current_route->path_length++] = temp_intermediate[i];
        }

        // Last city is always start (round trip)
        current_route->path[current_route->path_length++] = start_city;

        // Calculate fitness
        current_route->total_distance = 0;
        for (int i = 1; i < current_route->path_length; i++) {
            City* city1 = NULL, *city2 = NULL;
            for (int j = 0; j < total_cities; j++) {
                if (city_database[j].id == current_route->path[i-1]) city1 = &city_database[j];
                if (city_database[j].id == current_route->path[i]) city2 = &city_database[j];
            }
            current_route->total_distance += calculate_distance(city1, city2);
        }

        current_route->fitness = 1.0 / (current_route->total_distance + 1);
    }
}

/* Standard Function to Fitness of the Route
*  It Calculates the total distance of Route and returns the fitness values of the Route
*  Fitness value of the Route is 1 / (Total Distance + 1)
*/
double calculate_route_fitness(Route* route) {
    // Massive penalty if not all intermediate cities are visited
    if (!all_intermediate_cities_visited(route)) {
        return 1.0 / MASSIVE_PENALTY;
    }

    // Calculate total distance
    route->total_distance = 0;
    for (int i = 1; i < route->path_length; i++) {
        City* city1 = NULL, *city2 = NULL;
        for (int j = 0; j < total_cities; j++) {
            if (city_database[j].id == route->path[i-1]) city1 = &city_database[j];
            if (city_database[j].id == route->path[i]) city2 = &city_database[j];
        }
        route->total_distance += calculate_distance(city1, city2);
    }

    // Fitness is inverse of total distance, with a small constant
    return 1.0 / (route->total_distance + 1);
}

/* This Function verifies if all the intermediate cities are visited in the Route
*  It returns 1 if all the intermediate cities are visited, else returns 0
*/
int all_intermediate_cities_visited(Route* route) {
    // Create a check array for intermediate cities
    int visited[MAX_CITIES] = {0};

    // Mark intermediate cities as found in route
    for (int i = 1; i < route->path_length - 1; i++) {
        for (int j = 0; j < num_intermediate_cities; j++) {
            if (route->path[i] == intermediate_cities[j]) {
                visited[j] = 1;
            }
        }
    }

    // Check if all intermediate cities are visited
    for (int j = 0; j < num_intermediate_cities; j++) {
        if (!visited[j]) return 0;
    }

    return 1;
}

// Guaranteed CrossOver Method to Better Results
/*
* Creates child route by combining parent routes
* Ensures all intermediate cities are included
* Steps:
  a) Start and end with start city
  b) First add intermediate cities from parent1
  c) Fill remaining slots with cities from parent2
  d) Randomly add any missing cities
  e) Recalculate fitness*/
void guaranteed_crossover(Route* parent1, Route* parent2, Route* child) {
    // Reset child route
    memset(child->path, 0, sizeof(child->path));
    child->path_length = 0;
    child->total_distance = 0;
    child->fitness = 0;

    // Always start and end with start city
    child->path[0] = start_city;
    child->path_length = 1;

    // Create a set of available intermediate cities
    int available_intermediate[MAX_CITIES];
    int num_available = num_intermediate_cities;
    memcpy(available_intermediate, intermediate_cities, sizeof(intermediate_cities));

    // First, add cities from parent1's intermediate section
    for (int i = 1; i < parent1->path_length - 1; i++) {
        int current_city = parent1->path[i];
        
        // Check if this is an intermediate city
        for (int j = 0; j < num_available; j++) {
            if (current_city == available_intermediate[j]) {
                // Add to child if not already added
                int already_exists = 0;
                for (int k = 1; k < child->path_length; k++) {
                    if (child->path[k] == current_city) {
                        already_exists = 1;
                        break;
                    }
                }

                if (!already_exists) {
                    child->path[child->path_length++] = current_city;
                    
                    // Remove from available list
                    available_intermediate[j] = available_intermediate[num_available - 1];
                    num_available--;
                }
                break;
            }
        }
    }

    // Fill remaining slots from parent2
    for (int i = 1; i < parent2->path_length - 1; i++) {
        int current_city = parent2->path[i];
        
        // Check if this is an available intermediate city
        for (int j = 0; j < num_available; j++) {
            if (current_city == available_intermediate[j]) {
                // Add to child if not already added
                int already_exists = 0;
                for (int k = 1; k < child->path_length; k++) {
                    if (child->path[k] == current_city) {
                        already_exists = 1;
                        break;
                    }
                }

                if (!already_exists) {
                    child->path[child->path_length++] = current_city;
                    
                    // Remove from available list
                    available_intermediate[j] = available_intermediate[num_available - 1];
                    num_available--;
                }
                break;
            }
        }

        // Stop if all intermediate cities are added
        if (num_available == 0) break;
    }

    // If still missing cities, add them randomly
    while (num_available > 0) {
        int random_index = rand() % num_available;
        child->path[child->path_length++] = available_intermediate[random_index];
        
        // Remove from available list
        available_intermediate[random_index] = available_intermediate[num_available - 1];
        num_available--;
    }

    // Always end with start city
    child->path[child->path_length++] = start_city;

    // Recalculate fitness
    child->fitness = calculate_route_fitness(child);
}

/* Crossover Function which uses guarenteed crossover
*  This function performs crossover between two parents to produce a child
*  It uses the "order crossover" method, where a random subset of cities from one parent is selected
*  and the remaining cities are taken from the other parent in order
*  The child's fitness is then recalculated
*  The child is then added to the population
*/
void crossover(Route* parent1, Route* parent2, Route* child) {
    if ((double)rand() / RAND_MAX > CROSSOVER_RATE) {
        // Simple copy if no crossover
        memcpy(child, parent1, sizeof(Route));
        return;
    }

    guaranteed_crossover(parent1, parent2, child);
}

/* Mutation with city preservation
*  This function performs a mutation on a route by swapping two random cities
*  The mutation rate is set to 0.01, meaning there is a 1% chance of
*  A city being swapped with another random city
*/
void mutate(Route* route) {
    if ((double)rand() / RAND_MAX > MUTATION_RATE) return;

    // Identify indices of intermediate cities in the route
    int inter_indices[MAX_CITIES];
    int num_inter_indices = 0;

    for (int i = 1; i < route->path_length - 1; i++) {
        for (int j = 0; j < num_intermediate_cities; j++) {
            if (route->path[i] == intermediate_cities[j]) {
                inter_indices[num_inter_indices++] = i;
                break;
            }
        }
    }

    // If less than 2 intermediate cities, no meaningful mutation
    if (num_inter_indices < 2) return;

    // Swap two intermediate cities
    int idx1 = inter_indices[rand() % num_inter_indices];
    int idx2 = inter_indices[rand() % num_inter_indices];

    int temp = route->path[idx1];
    route->path[idx1] = route->path[idx2];
    route->path[idx2] = temp;

    // Recalculate fitness
    route->fitness = calculate_route_fitness(route);
}

// Main genetic algorithm function
/* Main function
 * Initializes the algorithm, runs the main loop, and handles result output
*/
void genetic_algorithm(Route* best_route) {
    Route population[MAX_POPULATION];
    Route next_generation[MAX_POPULATION];

    // Initialize first population
    initialize_population(population);

    // Copy first route as initial best route
    memcpy(best_route, &population[0], sizeof(Route));

    // Evolution process
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        // Elitism - keep best route
        memcpy(&next_generation[0], best_route, sizeof(Route));

        // Generate next generation
        for (int i = 1; i < MAX_POPULATION; i++) {
            // Tournament selection
            Route* parent1 = &population[rand() % MAX_POPULATION];
            Route* parent2 = &population[rand() % MAX_POPULATION];

            // Create child through crossover
            crossover(parent1, parent2, &next_generation[i]);
            
            // Potential mutation
            mutate(&next_generation[i]);
        }

        // Replace population
        memcpy(population, next_generation, sizeof(population));

        // Update best route
        for (int i = 0; i < MAX_POPULATION; i++) {
            if (population[i].fitness > best_route->fitness) {
                memcpy(best_route, &population[i], sizeof(Route));
            }
        }
    }
}

// Utility Function to Display the Best Route found
void print_best_route(Route* best_route) {
    printf("\nBest Route Found:\n");
    printf("Total Distance: %.2f\n", best_route->total_distance);
    printf("Route: ");
    
    // Check if the path length is greater than 0
    if (best_route->path_length > 0) {
        // Print the first city
        City* previous_city = NULL;
        for (int i = 0; i < best_route->path_length; i++) {
            City* current_city = NULL;
            
            // Find the current city from the city database
            for (int j = 0; j < total_cities; j++) {
                if (city_database[j].id == best_route->path[i]) {
                    current_city = &city_database[j];
                    break;
                }
            }
            
            // Skip printing the current city if it's the same as the previous city
            if (previous_city != NULL && previous_city->id == current_city->id) {
                continue;
            }
            
            // Print the current city
            printf("%s (%d)", current_city->name, current_city->id);
            previous_city = current_city; // Update the previous city
            
            // Print arrow if this is not the last city in the path
            if (i < best_route->path_length - 1) {
                printf(" -> ");
            } else {
                printf("\n");
            }
        }
    }
    printf("\n");
}

// Standaard Function To Calculate the Distance between two Cities
// It uses standard Euclidean Distance Formula
// a(X₁,Y₁), b(X₂,Y₂)
// Distance(a,b) = √((X₁-Y₁)²+(X₂-Y₂)²)
double calculate_distance(City* city1, City* city2) {
    double dx = city1->x_coordinate - city2->x_coordinate;
    double dy = city1->y_coordinate - city2->y_coordinate;
    return sqrt(dx * dx + dy * dy);
}


// Save route visualization as SVG (continued)
void save_route_visualization(Route* best_route) {
    FILE* svg_file = fopen("best_route.svg", "w");
    if (!svg_file) {
        printf("Could not create SVG file.\n");
        return;
    }

    // SVG header
    fprintf(svg_file, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svg_file, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\">\n");
    
    // Background
    fprintf(svg_file, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");

    // Normalize coordinates
    double max_x = -DBL_MAX, max_y = -DBL_MAX;
    double min_x = DBL_MAX, min_y = DBL_MAX;
    for (int i = 0; i < total_cities; i++) {
        max_x = fmax(max_x, city_database[i].x_coordinate);
        max_y = fmax(max_y, city_database[i].y_coordinate);
        min_x = fmin(min_x, city_database[i].x_coordinate);
        min_y = fmin(min_y, city_database[i].y_coordinate);
    }

    // Scale and padding
    double scale_x = 800 / (max_x - min_x);
    double scale_y = 800 / (max_y - min_y);
    double padding = 100;

    // Draw route lines
    fprintf(svg_file, "<path d=\"M");
    for (int i = 0; i < best_route->path_length; i++) {
        City* current_city = NULL;
        for (int j = 0; j < total_cities; j++) {
            if (city_database[j].id == best_route->path[i]) {
                current_city = &city_database[j];
                break;
            }
        }
        
        double x = padding + (current_city->x_coordinate - min_x) * scale_x;
        double y = padding + (current_city->y_coordinate - min_y) * scale_y;
        fprintf(svg_file, "%.2f,%.2f ", x, y);
    }
    fprintf(svg_file, "\" fill=\"none\" stroke=\"blue\" stroke-width=\"2\"/>\n");

    // Draw cities
    for (int i = 0; i < best_route->path_length; i++) {
        City* current_city = NULL;
        for (int j = 0; j < total_cities; j++) {
            if (city_database[j].id == best_route->path[i]) {
                current_city = &city_database[j];
                break;
            }
        }
        
        double x = padding + (current_city->x_coordinate - min_x) * scale_x;
        double y = padding + (current_city->y_coordinate - min_y) * scale_y;
        
        // Highlight start/end city
        if (current_city->id == start_city) {
            fprintf(svg_file, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"8\" fill=\"red\"/>\n", x, y);
        } else {
            fprintf(svg_file, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"5\" fill=\"green\"/>\n", x, y);
        }
        
        // City labels
        fprintf(svg_file, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"10\" fill=\"black\">%s</text>\n", 
                x + 10, y + 10, current_city->name);
    }

    // SVG footer
    fprintf(svg_file, "</svg>");
    fclose(svg_file);
    printf("Route visualization saved as best_route.svg\n");
}

// Validate route (optional, can be expanded)
void validate_route(Route* route) {
    // Ensure no duplicate cities (except start/end)
    for (int i = 1; i < route->path_length - 1; i++) {
        for (int j = i + 1; j < route->path_length - 1; j++) {
            if (route->path[i] == route->path[j]) {
                printf("Invalid route: Duplicate city found.\n");
                return;
            }
        }
    }
}

void displayMenu() {
    printf("\n\n--------------------Options---------------------\n");
    printf("1. Back to City selection\n");
    printf("2. Back to Database selection\n");
    printf("3. Save route visualization as SVG\n");
    printf("4. Exit the Program\n");
}


