
# Traveling Salesman Problem Solver Using Genetic Algorithm

This program solves the Traveling Salesman Problem (TSP) using a Genetic Algorithm (GA). The objective is to find the shortest route that visits all given cities exactly once and returns to the starting city.

The program is flexible, allowing users to:

- Use a city database loaded from a file or generate cities randomly.

- Customize genetic algorithm parameters like mutation rate and    population size.

- Visualize the solution as an SVG file.


## Features

- Random City Generation: Generate a random city database using prefixes and suffixes for city names
- File-Based City Database: Load city data from a user-provided CSV file.
- Genetic Algorithm:
  * Fitness-based route selection.
  * Mutation and crossover operations to improve the solution.
  * SVG Visualization: Export the best route as an SVG file for easy visualization.
  * Interactive Menu: Options to reselect cities, load a new database, or save the best route visualization.


## Compilation and Execution

### Prerequisites
Ensure you have a C compiler installed (e.g., GCC)

### Compilation

```bash
  gcc -Wall -o result ProgramMain.c tspSolver.c -lm
```

### Execution

```bash
  ./result [options]
```

## Command Line options

Use these options to customize the behavior of the genetic algorithm:

#### 1. `-c [value]` Set the maximum number of cities (default: 100). 

#### 2. `-p [value]` Set the population size for the algorithm (default: 500). 

#### 3. `-g [value]` Set the number of generations (default: 5000). 

#### 4. `-m [value]` Set the mutation rate (default: 0.08). 
 
#### 5. `-x [value]`) Set the crossover rate (default: 0.85). 

#### 6. `-h` Display the help menu. 







## City DataBase

### Random Generation
The program generates between 10 and 40 cities with random coordinates and names.

## File Format
If loading from a file, ensure it is in the following CSV format:

```bash
ID,CityName,X,Y
0,New York,40.7128,-74.0060
1,Tokyo,35.6895,139.6917
...
```

### Interactive Menu

After finding the best route, the program offers the following options:

- Back to City Selection: Modify the selected cities for the route.
- Back to Database Selection: Reload the city database from a file or generate new cities.
- Save Route Visualization: Save the best route as an SVG file.
- Exit the Program: Terminate the program.

## Output

- Best Route: Displays the total distance and the optimal path.
- Time Taken: Prints the execution time of the genetic algorithm.
- SVG File: Saves the best route visualization to best_route.svg.

```bash
Best Route Found:
Total Distance: 10345.67
Route: New York (0) -> Tokyo (1) -> Sydney (2) -> New York (0)
Time taken: 12.34 seconds

Options:
1. Back to City Selection
2. Back to Database Selection
3. Save Route Visualization
4. Exit the Program
```

## SVG Output

![Screenshot 2024-12-12 135947](https://github.com/user-attachments/assets/5bb30fc3-3467-465e-8df0-5c9dc15ce4a6)

## Key Functions

- `genetic_algorithm()`: Core implementation of the genetic algorithm.
- `initialize_population()`: Creates an initial population of routes.
- `mutate()`: Mutates a route for diversity.
- `crossover()`: Combines parent routes to produce offspring.
- `print_best_route()`: Displays the optimal route.
- `save_route_visualization()`: Exports the route to an SVG file.

## Enhacements

Future improvements could include:

- Support for multi-threading to optimize execution time.
- Dynamic SVG scaling for larger datasets.
- Configurable city coordinate ranges.

## Licence

This program is open-source and can be freely used and modified for educational purposes.
