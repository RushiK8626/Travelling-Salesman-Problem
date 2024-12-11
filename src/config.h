#ifndef CONFIG_H
#define CONFIG_H

// Defined Named Constants
#define MAX_CITIES 100
#define MAX_POPULATION 500
#define MAX_GENERATIONS 5000
#define MUTATION_RATE 0.08
#define CROSSOVER_RATE 0.85
#define MASSIVE_PENALTY 1000000.0
#define MAX_CITY_NAME_LENGTH 100

// City structure to store detailed information
typedef struct {
    int id;
    char name[MAX_CITY_NAME_LENGTH];
    double x_coordinate;
    double y_coordinate;
} City;

// Route representation
typedef struct {
    int path[MAX_CITIES];
    int path_length;
    double total_distance;
    double fitness;
} Route;

// Config structure
typedef struct {
    int maxCities;
    int maxPopulation;
    int maxGenerations;
    double mutationRate;
    double crossoverRate;
} Config;

// prototypes of All the Functions which are used thoughout the Program
void initializeConfig();
void displayHelp();
void parseArguments(int argc, char *argv[]);
void load_city_database();
void display_cities();
double calculate_distance(City* city1, City* city2);
void generate_random_city_database();
void input_route_parameters();
void initialize_population(Route* population);
double calculate_route_fitness(Route* route);
void crossover(Route* parent1, Route* parent2, Route* child);
void mutate(Route* route);
void genetic_algorithm(Route* best_route);
void print_best_route(Route* best_route);
void save_route_visualization(Route* best_route);
void validate_route(Route* route);
int all_intermediate_cities_visited(Route* route);
void guaranteed_crossover(Route* parent1, Route* parent2, Route* child);
void displayMenu();

#endif // CONFIG_H
