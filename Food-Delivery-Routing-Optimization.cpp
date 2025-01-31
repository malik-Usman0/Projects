
#include <iostream>
#include <string>

using namespace std;

struct Rider {
    int id;
    int location;
};

struct Order {
    string order_name;
    int location;
    int time_limit;
    int assigned_rider;
    int travel_distance;
    Order(string name, int l, int t) : order_name(name), location(l), time_limit(t), assigned_rider(-1), travel_distance(0) {}
};

struct Restaurant {
    string Name;
    int location;
    int total_orders;
    int total_assigned_riders;
    Order** orders;
    Rider** Assigned_riders;

    Restaurant(string name, int location, int number_of_orders) {
        this->Name = name;
        this->location = location;
        this->total_orders = number_of_orders;
        this->total_assigned_riders = 0; // Initialize total_assigned_riders to 0

        // Allocate memory for the array of Order pointers
        orders = new Order * [total_orders];

        // Initialize each element to nullptr
        for (int i = 0; i < total_orders; ++i) {
            orders[i] = nullptr;
        }

        // Allocate memory for the array of Rider pointers
        Assigned_riders = new Rider * [total_assigned_riders];
    }

    void insertOrder(Order* order, int index) {
        if (index >= 0 && index < total_orders) {
            orders[index] = order;
        }
    }

    void showOrders() {
        for (int i = 0; i < total_orders; i++) {
            cout << "Order Details: \n";
            cout << "Order Name: " << orders[i]->order_name << "\n";
            cout << "Order Location: " << orders[i]->location << "\n";
            cout << "Order time_limit: " << orders[i]->time_limit << "\n";
        }
    }

    void assignRider(Rider* rider, int index) {
        if (index >= 0 && index < total_assigned_riders) {
            Assigned_riders[index] = rider;
        }
    }
};



// Function to relax edge in Dijkstra's algorithm
void relaxEdge(int dist[], int u, int v, int weight) {
    if (dist[u - 1] != INT_MAX && dist[u - 1] + weight < dist[v - 1]) {
        dist[v - 1] = dist[u - 1] + weight;
    }
}

// Function to find minimum distance vertex
int minimumDist(int dist[], bool visited[], int num_vertices) {
    int min_dist = INT_MAX, min_index;

    for (int v = 0; v < num_vertices; ++v) {
        if (!visited[v] && dist[v] <= min_dist) {
            min_dist = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Dijkstra's algorithm implementation
int Dijkstra(int size, int src, int dest) {
    int num_vertices = size * size;

    // Create a 2D grid representing the city
    if (src <= 0 || dest > (size * size)) {
        cout << "Out of range\n";
        return INT_MAX;
    }

    int** grid = new int* [size];
    for (int i = 0; i < size; ++i) {
        grid[i] = new int[size];
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            grid[i][j] = i * size + j + 1;
        }
    }

    // Initialize distances and visited array
    int* dist = new int[num_vertices];
    bool* visited = new bool[num_vertices];
    for (int i = 0; i < num_vertices; ++i) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }

    // Set distance of source vertex to 0
    dist[src - 1] = 0;

    // Perform Dijkstra's algorithm
    for (int count = 0; count < num_vertices - 1; ++count) {
        int u = minimumDist(dist, visited, num_vertices);
        visited[u] = true;

        // Update distances of adjacent vertices
        int row = (u / size);
        int col = (u % size);
        // Check adjacent vertices above, below, left, and right
        if (row > 0) // Check above
            relaxEdge(dist, grid[row][col], grid[row - 1][col], 1);
        if (row < size - 1) // Check below
            relaxEdge(dist, grid[row][col], grid[row + 1][col], 1);
        if (col > 0) // Check left
            relaxEdge(dist, grid[row][col], grid[row][col - 1], 1);
        if (col < size - 1) // Check right
            relaxEdge(dist, grid[row][col], grid[row][col + 1], 1);
    }

    // Print the shortest distance from source to destination
    cout << "Shortest distance from vertex " << src << " to vertex " << dest << " is: " << dist[dest - 1] << endl;
    return dist[dest - 1];
}


class Squared_Graph {
    int size;
    int** Squared_City;
    char** Orders;

public:
    Squared_Graph(int s) {
        this->size = s;
        Squared_City = new int* [size];
        Orders = new char* [size];
        for (int i = 0; i < size; i++) {
            Squared_City[i] = new int[size];
            Orders[i] = new char[size];
            for (int j = 0; j < size; j++) {
                Squared_City[i][j] = 0;
                Orders[i][j] = '.';
            }
        }
    }

    void Insert_restaurant(Restaurant r) {
        int row = (r.location - 1) / size;
        int col = (r.location - 1) % size;
        Squared_City[row][col] = 1;
    }

    void Insert_restaurant(Restaurant r, int index) {
        int row = (r.location - 1) / size;
        int col = (r.location - 1) % size;
        Squared_City[row][col] = index + 1;
    }

    void insert_orders_at_location(Order o) {
        int row = (o.location - 1) / size;
        int col = (o.location - 1) % size;
        Orders[row][col] = 'A' + o.location - 1; // Assigning unique characters to each order
    }

    void insert_orders_at_location(Restaurant r, int index) {
        for (int i = 0; i < r.total_orders; ++i) {
            Order* o = r.orders[i]; // Get the order from the orders array in the Restaurant object
            int row = (o->location - 1) / size;
            int col = (o->location - 1) % size;
            Orders[row][col] = 'A' + index; // Assigning unique characters to each order
        }
    }

    void printCity() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << Squared_City[i][j] << " ";
            }
            cout << endl;
        }
    }

    void print_Oders() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << Orders[i][j] << " ";
            }
            cout << endl;
        }
    }

    void assignRidersToRestaurants(int total_riders, Restaurant** restaurants, int total_restaurants);

    int calculate_minimum_distance(int total_riders, Restaurant** restaurants, int total_restaurants);
};


void assignRidersAndCalculateDistances(Rider* riders, int num_riders, Restaurant** restaurants, int num_restaurants, int size) {
    int total_travel_time = 0;

    // Assign riders to orders from their corresponding restaurants
    for (int i = 0; i < num_restaurants; i++) {
        int total_assigned_riders = restaurants[i]->total_assigned_riders;
        int total_orders = restaurants[i]->total_orders;

        // Assign riders to orders in round-robin fashion
        for (int j = 0; j < total_orders; j++) {
            Order* order = restaurants[i]->orders[j];

            // Find the rider index using modulo to ensure round-robin assignment
            int rider_index = j % total_assigned_riders;
            Rider* rider = restaurants[i]->Assigned_riders[rider_index];

            // Calculate distance from rider's location to order's location
            int distance = Dijkstra(size, rider->location, order->location);

            // Update order's assigned rider and travel distance
            order->assigned_rider = rider->id;
            order->travel_distance = distance;

            // Update total travel time
            total_travel_time += distance;

            // Update rider's location
            rider->location = order->location;

            cout << "Order " << order->order_name << " assigned to rider " << order->assigned_rider << " with travel distance " << order->travel_distance << endl;
        }
    }

    cout << "\nTotal travel time for all orders: " << total_travel_time << endl;
}

void Squared_Graph::assignRidersToRestaurants(int total_riders, Restaurant** restaurants, int total_restaurants) {
    // Assign riders to restaurants
    if (total_riders >= total_restaurants) {
        int riders_per_restaurant = total_riders / total_restaurants;
        for (int i = 0; i < total_restaurants; i++) {
            for (int j = 0; j < riders_per_restaurant; j++) {
                Rider* rider = new Rider();
                rider->id = (i * riders_per_restaurant) + j + 1; // Unique identifier for the rider
                rider->location = 1; // Set initial location of the rider
                restaurants[i]->total_assigned_riders++;
                restaurants[i]->assignRider(rider, restaurants[i]->total_assigned_riders - 1);
                cout << "Assigned rider " << rider->id << " to Restaurant " << i + 1 << endl;
            }
        }
        // Assign additional riders to the restaurant with the most orders
        int remaining_riders = total_riders % total_restaurants;
        if (remaining_riders > 0) {
            int max_orders = 0;
            int max_orders_index = 0;
            // Find the restaurant with the most orders
            for (int j = 0; j < total_restaurants; j++) {
                if (restaurants[j]->total_orders > max_orders) {
                    max_orders = restaurants[j]->total_orders;
                    max_orders_index = j;
                }
            }
            // Assign the remaining riders to the restaurant with the most orders
            for (int i = 0; i < remaining_riders; i++) {
                Rider* rider = new Rider();
                rider->id = total_riders - remaining_riders + i + 1; // Unique identifier for the rider
                rider->location = 1; // Set initial location of the rider
                restaurants[max_orders_index]->total_assigned_riders++;
                restaurants[max_orders_index]->assignRider(rider, restaurants[max_orders_index]->total_assigned_riders - 1);
                cout << "Assigned additional rider " << rider->id << " to Restaurant " << max_orders_index + 1 << endl;
            }
        }
    }
    else {
        cout << "Number of riders is less than total restaurants. Assigning riders to restaurants with most orders." << endl;
        int max_orders = 0;
        int max_orders_index = 0;
        // Find the restaurant with the most orders
        for (int j = 0; j < total_restaurants; j++) {
            if (restaurants[j]->total_orders > max_orders) {
                max_orders = restaurants[j]->total_orders;
                max_orders_index = j;
            }
        }
        // Assign the riders to the restaurant with the most orders
        for (int i = 0; i < total_riders; i++) {
            Rider* rider = new Rider();
            rider->id = i + 1; // Unique identifier for the rider
            rider->location = 1; // Set initial location of the rider
            restaurants[max_orders_index]->total_assigned_riders++;
            restaurants[max_orders_index]->assignRider(rider, restaurants[max_orders_index]->total_assigned_riders - 1);
            cout << "Assigned rider " << rider->id << " to Restaurant " << max_orders_index + 1 << endl;
        }
    }
}

int Squared_Graph::calculate_minimum_distance(int total_riders, Restaurant** restaurants, int total_restaurants) {
    int min_distance = INT_MAX;

    for (int i = 0; i < total_restaurants; ++i) {
        for (int j = 0; j < restaurants[i]->total_orders; ++j) {
            Order* order = restaurants[i]->orders[j];
            for (int k = 0; k < total_riders; ++k) {
                int distance = Dijkstra(size, order->location, restaurants[i]->Assigned_riders[k]->location);
                if (distance < min_distance) {
                    min_distance = distance;
                }
            }
        }
    }

    return min_distance;
}



int main() {
    int number_of_test_cases;
    cout << "Enter the number of test cases: ";
    cin >> number_of_test_cases;
    if (number_of_test_cases <= 0) {
        cout << "Invalid number of test cases\n";
        return 1; // Exit with error
    }
    Squared_Graph** Graphs = new Squared_Graph * [number_of_test_cases];

    for (int i = 0; i < number_of_test_cases; i++) {
        int size_of_graph;
        int number_of_riders;
        int total_restaurants;
        cout << "Enter the size of graph, numbers of riders and Number of Restaurants: ";
        cin >> size_of_graph >> number_of_riders >> total_restaurants;

        cin.ignore();

        Graphs[i] = new Squared_Graph(size_of_graph);
        Restaurant** restaurants = new Restaurant * [total_restaurants];

        Rider* riders = new Rider[number_of_riders];
        for (int j = 0; j < number_of_riders; j++) {
            riders[j].id = j + 1;
            riders[j].location = 0;  // Assuming initial location is 0
        }

        for (int j = 0; j < total_restaurants; j++) {
            int restaurant_location;
            string Restaurant_name;
            int number_of_orders;

            cout << "Enter the name of Restaurant " << j + 1 << ": ";
            cin.ignore();
            getline(cin, Restaurant_name);
            cout << "Enter the location of Restaurant " << j + 1 << ": ";
            cin >> restaurant_location;
            cout << "Enter the number of orders for Restaurant " << j + 1 << ": ";
            cin >> number_of_orders;

            restaurants[j] = new Restaurant(Restaurant_name, restaurant_location, number_of_orders);
            restaurants[j]->orders = new Order * [number_of_orders];

            for (int k = 0; k < number_of_orders; k++) {
                cout << "Enter the name of order " << k + 1 << ": ";
                string order_name;
                int location, time_limit;
                cin.ignore();
                getline(cin, order_name);
                cout << "Enter the location: ";
                cin >> location;
                cout << "Enter the time limit: ";
                cin >> time_limit;
                Order* order = new Order(order_name, location, time_limit);
                restaurants[j]->insertOrder(order, k);
            }

            Graphs[i]->insert_orders_at_location(*restaurants[j], j);
            Graphs[i]->Insert_restaurant(*restaurants[j], j);
        }
        Graphs[i]->printCity();
        Graphs[i]->print_Oders();
        Graphs[i]->assignRidersToRestaurants(number_of_riders, restaurants, total_restaurants);

        // Call the function to assign riders and calculate distances
        //assignRidersAndCalculateDistances(riders, number_of_riders, restaurants, total_restaurants, size_of_graph);

        // Cleanup memory
        for (int j = 0; j < total_restaurants; j++) {
            /*for (int k = 0; k < restaurants[j]->total_orders; k++) {
                delete restaurants[j]->orders[k];
            }*/
            delete[] restaurants[j]->orders;
            delete restaurants[j];
        }
        delete[] restaurants;

        for (int j = 0; j < number_of_riders; j++) {
            delete[] riders;
        }


        delete Graphs[i];
    }

    delete[] Graphs;

    return 0;
}