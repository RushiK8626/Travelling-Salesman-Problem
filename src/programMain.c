/*Solution For The Travelling Salesman Problem Using The Genetic Algorithm Optimization*/ 


/*This program solves the Traveling Salesman Problem (TSP) using a Genetic Algorithm (GA). 
* The objective is to find the shortest route that visits all given cities exactly once and returns to the starting city.
* The program is flexible, allowing users to:
    - Use a city database loaded from a file or generate cities randomly.
    - Customize genetic algorithm parameters like mutation rate and population size.
    - Visualize the solution as an SVG file.
*/

//Included Required Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "config.h"

// Main function

int main(int argc, char* argv[]) {

    //Parse Command Line Arguments
    parseArguments(argc, argv);

    Route best_route;

    // Seed random number generator
    srand(time(NULL));

    // Load or generate city database
    loadCities: 
        load_city_database();

    while(1){
        // Get route parameters from user
        citySelection:
        input_route_parameters();

        // Start the Clock to measure the time taken by the algorithm
        clock_t start = clock();

        // Run genetic algorithm to find best route
        genetic_algorithm(&best_route);

        clock_t end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Validate the route
        validate_route(&best_route);

        // Print best route details
        print_best_route(&best_route);
        printf("Time taken: %.2f seconds\n", cpu_time_used);

        // Option to save route visualization
        // char save_choice;
        // printf("\nSave route visualization as SVG? (y/n): ");
        // scanf(" %c", &save_choice);
        // if (save_choice == 'y' || save_choice == 'Y') {
        //     save_route_visualization(&best_route);
        // }
        
        while(1){

            // Additional Options to User after finding the best route
            // Options to reselect the cities, change the city database
            // Save the best route y visualizing in the SVG format or exit the program
            
            displayMenu();

            int choice;
            printf("Enter Your Choice: ");
            scanf("%d", &choice);

            switch(choice){

                case 1:
                    goto citySelection;

                case 2:
                    goto loadCities;

                case 3:
                    save_route_visualization(&best_route);
                    break;

                case 4:
                    printf("Exiting The Program...\n");
                    return 0;      

                default:
                    printf("Invalid Choice\n");
                    break;
            }
        }
    }

    return 0;
}