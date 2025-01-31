#include <iostream>
using namespace std;

// Base class for all menu categories
class MenuCategory {
public:
    virtual void display() = 0; // pure virtual function for displaying menu category
};

// Takeaway category class
class TakeawayCategory : public MenuCategory {
public:
    void display() {
        cout << "#######---->Welcome to Takeaway menu <----#######" << endl;
    }
};

// Dine-in category class
class DineCategory : public MenuCategory {
public:
    void display() {
        cout << "#######---->Welcome to Dine away menu <----#######" << endl;
    }
};

// Delivery category class
class DeliveryCategory : public MenuCategory {
public:
    void display() {
        cout << "#######---->Welcome to Dilivery menu <----#######" << endl;
    }
};

// Base class for all food types
class FoodType {
public:
    virtual void display() = 0; // pure virtual function for displaying food type
    double getPrice(int item_number) {
        return item_prices[item_number - 1];
    }

protected:
    double item_prices[4];
};

// Fast food type class
class FastFood : public FoodType {
public:
    FastFood() {
        item_prices[0] = 499;
        item_prices[1] = 299;
        item_prices[2] = 199;
        item_prices[3] = 399;
    }

    void display() {
        cout << "Available Items: " << endl;
        cout << "[1] Zinger Burger (RS:499)" << endl;
        cout << "[2] Loaded Fries (RS:299) " << endl;
        cout << "[3] Shawarma(Rs:199)" << endl;
        cout << "[4] Chicken Cheese Burger(Rs:399)" << endl;
    }
};

// Pakistani food type class
class PakistaniFood : public FoodType {
public:
    PakistaniFood() {
        item_prices[0] = 1499;
        item_prices[1] = 299;
        item_prices[2] = 249;
        item_prices[3] = 599;
    }

    void display() {
        cout << "Available Pakistani Food Items: " << endl;
        cout << "[1]Chicken Karahi(Rs:1499)" << endl;
        cout << "[2]Biryani(Rs:299)" << endl;
        cout << "[3]Haleem(Rs:249)" << endl;
        cout << "[4]Nehari(Rs:599)" << endl;
    }
};

// Order class for tracking orders
class Order {
private:
    double total_cost;
    double item_prices[4];

public:
    Order() {
        total_cost = 0.0;
    }

    void addToTotal(double cost, int item_number) {
        total_cost += cost;
        item_prices[item_number - 1] = cost;
    }

    double getTotal() {
        return total_cost;
    }

    void displayItems() {
        cout << "Items and Prices: " << endl;
        for (int i = 0; i < 4; i++) {
            if (item_prices[i] > 0) {
                cout << "Item " << i + 1 << ": " << item_prices[i] << endl;
            }
        }
    }

       double getDiscount() {
    double total = getTotal();
    if (total <= 1000) {
        return total * 0.05;
    } else if (total > 1000 && total <= 1500) {
        return total * 0.10;
    } else if (total > 1500 && total < 2000) {
        return total * 0.15;
    } else if (total > 2000 && total < 2500) {
        return total * 0.20;
    } else if (total > 2500 && total < 3000) {
        return total * 0.30;
    } else {
        return total * 0.50;
    }
}
};

// Main class for the program
class UNHCrewCafe {
private:
    MenuCategory* category_menu;
    FoodType* food_type;
    Order order;
public:
    UNHCrewCafe() {
        int category_input, food_input;
        cout << "########---> Welcome to UNH Crew Cafe<--- #########" << endl;
        cout << "Please select the option " << endl;
        cout << "[0] Take away" << endl;
        cout << "[1] Dine in" << endl;
        cout << "[2] Home Delivery" << endl;
        cout << "Select category (0 , 1 , 2): ";
        cin >> category_input;
        switch (category_input) {
            case 0:
                category_menu = new TakeawayCategory();
                break;
            case 1:
                category_menu = new DineCategory();
                break;
            case 2:
                category_menu = new DeliveryCategory();
                break;
            default:
                cout << "Please Enter Correct Category" << endl;
                return;
        }
        category_menu->display();
        cout << "Select food Type: " << endl;
        cout << "[0] Fast Food " << endl;
        cout << "[1] Pakistani Food " << endl;
        cout << "Select category: ";
        cin >> food_input;
        switch (food_input) {
            case 0:
                food_type = new FastFood();
                break;
            case 1:
                food_type = new PakistaniFood();
                break;
            default:
                cout << "Please Enter Correct Category" << endl;
                return;
        }
        food_type->display();
        int food_choice[10];
        cout << "Select food item: ";
        cin>> food_choice;
        switch (food_choice) {
            case 1:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 2:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 3:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 4:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            default:
                cout << "Please Enter Correct Food Item" << endl;
                return;
        }
        int checking_number;
        cout << "Do you want to order something  OR You want total: " << endl;
        cout << "[0] To order Something Again" << endl;
        cout << "[1] To Calculate total" << endl;
        cin >> checking_number;
        if (checking_number == 0) {
            UNHCrewCafe cafe;
            cafe.run();
        } else {
            double total = order.getTotal();
                        double discount = order.getDiscount();
            cout << "Items and Prices: " << endl;
            order.displayItems();
            cout << "Total Bill: " << total << " PKR." << endl;
            cout << "Discount Applied: " << discount << " PKR." << endl;
            cout << "Your Total Bill after discount is: " << total - discount << " PKR." << endl;
            cout << "**************Thanks for Visiting************************" << endl;
        }
    }
    void run() {
        UNHCrewCafe cafe;
        cafe.start();
    }
    void start() {
        int category_input, food_input;
        cout << "########---> Welcome to UNH Crew Cafe<--- #########" << endl;
        cout << "Please select the option " << endl;
        cout << "[0] Take away" << endl;
        cout << "[1] Dine in" << endl;
        cout << "[2] Home Delivery" << endl;
        cout << "Select category (1 , 2 , 3): ";
        cin >> category_input;
        switch (category_input) {
            case 0:
                category_menu = new TakeawayCategory();
                break;
            case 1:
                category_menu = new DineCategory();
                break;
            case 2:
                category_menu = new DeliveryCategory();
                break;
            default:
                cout << "Please Enter Correct Category" << endl;
                return;
        }
        category_menu->display();
        cout << "Select food Type: " << endl;
        cout << "[0] Fast Food " << endl;
        cout << "[1] Pakistani Food " << endl;
        cout << "Select category: ";
        cin >> food_input;
        switch (food_input) {
            case 0:
                food_type = new FastFood();
                break;
            case 1:
                food_type = new PakistaniFood();
                break;
            default:
                cout << "Please Enter Correct Category" << endl;
                return;
        }
        food_type->display();
        int food_choice;
        cout << "Select food item: ";
        cin >> food_choice;
        switch (food_choice) {
            case 1:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 2:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 3:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            case 4:
                order.addToTotal(food_type->getPrice(food_choice), food_choice);
                break;
            default:
                cout << "Please Enter Correct Food Item" << endl;
                return;
        }
        int checking_number;
        cout << "Do you want to order something  OR You want total: " << endl;
        cout << "[0] To order Something Again" << endl;
        cout << "[1] To Calculate total" << endl;
        cin >> checking_number;
        if (checking_number == 0) {
            UNHCrewCafe cafe;
            cafe.run();
        } else {
            double total = order.getTotal();
            double discount = order.getDiscount();
            cout << "Items and Prices: " << endl;
            order.displayItems();
            cout << "Total Bill: " << total << " PKR." << endl;
            cout << "Discount Applied: " << discount << " PKR." << endl;
            cout << "Your Total Bill after discount is: " << total - discount << " PKR." << endl;
            cout << "**************Thanks for Visiting************************" << endl;
        }
    }
};

int main() {
    UNHCrewCafe cafe;
    cafe.start();
    return 0;
}
