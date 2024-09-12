#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <random>

using namespace std;

// Forward declarations
class Restaurant;
class Customer;
class Order;
class DeliveryAgent;
class OrderItem;

// OrderStatus enum
enum class OrderStatus {
    PENDING,
    CONFIRMED,
    PREPARING,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

// MenuItem class
class MenuItem {
public:
    MenuItem(string id, string name, string description, double price)
        : id(id), name(name), description(description), price(price), available(true) {}

    void setAvailable(bool available) { this->available = available; }

    string getId() const { return id; }

private:
    string id;
    string name;
    string description;
    double price;
    bool available;
};

// Customer class
class Customer {
public:
    Customer(string id, string name, string email, string phone)
        : id(id), name(name), email(email), phone(phone) {}

    string getId() const { return id; }

private:
    string id;
    string name;
    string email;
    string phone;
};

// DeliveryAgent class
class DeliveryAgent {
public:
    DeliveryAgent(string id, string name, string phone)
        : id(id), name(name), phone(phone), available(true) {}

    void setAvailable(bool available) { this->available = available; }
    bool isAvailable() const { return available; }

    string getId() const { return id; }

private:
    string id;
    string name;
    string phone;
    bool available;
};

// OrderItem class
class OrderItem {
public:
    OrderItem(const shared_ptr<MenuItem>& menuItem, int quantity)
        : menuItem(menuItem), quantity(quantity) {}

private:
    shared_ptr<MenuItem> menuItem;
    int quantity;
};

// Order class
class Order {
public:
    Order(string id, shared_ptr<Customer> customer, shared_ptr<Restaurant> restaurant)
        : id(id), customer(customer), restaurant(restaurant), status(OrderStatus::PENDING) {}

    void addItem(const shared_ptr<OrderItem>& item) { items.push_back(item); }
    void removeItem(const shared_ptr<OrderItem>& item) { items.erase(remove(items.begin(), items.end(), item), items.end()); }

    void setStatus(OrderStatus status) { this->status = status; }
    void assignDeliveryAgent(const shared_ptr<DeliveryAgent>& agent) { this->deliveryAgent = agent; }

    string getId() const { return id; }
    OrderStatus getStatus() const { return status; }

private:
    string id;
    shared_ptr<Customer> customer;
    shared_ptr<Restaurant> restaurant;
    vector<shared_ptr<OrderItem>> items;
    OrderStatus status;
    shared_ptr<DeliveryAgent> deliveryAgent;
};

// Restaurant class
class Restaurant {
public:
    Restaurant(string id, string name, string address, vector<shared_ptr<MenuItem>> menu)
        : id(id), name(name), address(address), menu(menu) {}

    void addMenuItem(const shared_ptr<MenuItem>& item) { menu.push_back(item); }
    void removeMenuItem(const shared_ptr<MenuItem>& item) { menu.erase(remove(menu.begin(), menu.end(), item), menu.end()); }

    string getId() const { return id; }
    vector<shared_ptr<MenuItem>> getMenu() const { return menu; }

private:
    string id;
    string name;
    string address;
    vector<shared_ptr<MenuItem>> menu;
};

// FoodDeliveryService class (Singleton)
class FoodDeliveryService {
public:
    static FoodDeliveryService& getInstance() {
        static FoodDeliveryService instance;
        return instance;
    }

    void registerCustomer(const shared_ptr<Customer>& customer) { customers[customer->getId()] = customer; }
    void registerRestaurant(const shared_ptr<Restaurant>& restaurant) { restaurants[restaurant->getId()] = restaurant; }
    void registerDeliveryAgent(const shared_ptr<DeliveryAgent>& agent) { deliveryAgents[agent->getId()] = agent; }

    vector<shared_ptr<Restaurant>> getAvailableRestaurants() {
        vector<shared_ptr<Restaurant>> result;
        for (auto& [id, restaurant] : restaurants) {
            result.push_back(restaurant);
        }
        return result;
    }

    shared_ptr<Order> placeOrder(const string& customerId, const string& restaurantId, const vector<shared_ptr<OrderItem>>& items) {
        auto customer = customers[customerId];
        auto restaurant = restaurants[restaurantId];
        if (customer && restaurant) {
            auto order = make_shared<Order>(generateOrderId(), customer, restaurant);
            for (auto& item : items) {
                order->addItem(item);
            }
            orders[order->getId()] = order;
            notifyRestaurant(order);
            cout << "Order placed: " << order->getId() << endl;
            return order;
        }
        return nullptr;
    }

    void updateOrderStatus(const string& orderId, OrderStatus status) {
        auto order = orders[orderId];
        if (order) {
            order->setStatus(status);
            notifyCustomer(order);
            if (status == OrderStatus::CONFIRMED) {
                assignDeliveryAgent(order);
            }
        }
    }

    void cancelOrder(const string& orderId) {
        auto order = orders[orderId];
        if (order && order->getStatus() == OrderStatus::PENDING) {
            order->setStatus(OrderStatus::CANCELLED);
            notifyCustomer(order);
            notifyRestaurant(order);
            cout << "Order cancelled: " << order->getId() << endl;
        }
    }

private:
    unordered_map<string, shared_ptr<Customer>> customers;
    unordered_map<string, shared_ptr<Restaurant>> restaurants;
    unordered_map<string, shared_ptr<Order>> orders;
    unordered_map<string, shared_ptr<DeliveryAgent>> deliveryAgents;

    FoodDeliveryService() = default;

    void notifyCustomer(const shared_ptr<Order>& order) {
        // Notify customer about order status
    }

    void notifyRestaurant(const shared_ptr<Order>& order) {
        // Notify restaurant about new order
    }

    void assignDeliveryAgent(const shared_ptr<Order>& order) {
        for (auto& [id, agent] : deliveryAgents) {
            if (agent->isAvailable()) {
                agent->setAvailable(false);
                order->assignDeliveryAgent(agent);
                notifyDeliveryAgent(order);
                break;
            }
        }
    }

    void notifyDeliveryAgent(const shared_ptr<Order>& order) {
        // Notify delivery agent about assigned order
    }

    string generateOrderId() {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        return "ORD" + to_string(dis(gen));
    }
};

// Demo Function
void runDemo() {
    auto& deliveryService = FoodDeliveryService::getInstance();

    // Register customers
    auto customer1 = make_shared<Customer>("C001", "John Doe", "john@example.com", "1234567890");
    auto customer2 = make_shared<Customer>("C002", "Jane Smith", "jane@example.com", "9876543210");
    deliveryService.registerCustomer(customer1);
    deliveryService.registerCustomer(customer2);

    // Register restaurants
    vector<shared_ptr<MenuItem>> restaurant1Menu = {
        make_shared<MenuItem>("M001", "Burger", "Delicious burger", 9.99),
        make_shared<MenuItem>("M002", "Pizza", "Cheesy pizza", 12.99)
    };
    auto restaurant1 = make_shared<Restaurant>("R001", "Restaurant 1", "Address 1", restaurant1Menu);
    deliveryService.registerRestaurant(restaurant1);

    vector<shared_ptr<MenuItem>> restaurant2Menu = {
        make_shared<MenuItem>("M003", "Sushi", "Fresh sushi", 15.99),
        make_shared<MenuItem>("M004", "Ramen", "Delicious ramen", 10.99)
    };
    auto restaurant2 = make_shared<Restaurant>("R002", "Restaurant 2", "Address 2", restaurant2Menu);
    deliveryService.registerRestaurant(restaurant2);

    // Register delivery agents
    auto agent1 = make_shared<DeliveryAgent>("D001", "Agent 1", "9999999999");
    auto agent2 = make_shared<DeliveryAgent>("D002", "Agent 2", "8888888888");
    deliveryService.registerDeliveryAgent(agent1);
    deliveryService.registerDeliveryAgent(agent2);

    // Place an order
    vector<shared_ptr<OrderItem>> orderItems = {
        make_shared<OrderItem>(restaurant1Menu[0], 2),
        make_shared<OrderItem>(restaurant1Menu[1], 1)
    };
    auto order = deliveryService.placeOrder(customer1->getId(), restaurant1->getId(), orderItems);

    // Update order status
    deliveryService.updateOrderStatus(order->getId(), OrderStatus::CONFIRMED);
    cout << "Order status updated: " << static_cast<int>(order->getStatus()) << endl;

    // Cancel an order
    auto order2 = deliveryService.placeOrder(customer2->getId(), restaurant2->getId(), {
        make_shared<OrderItem>(restaurant2Menu[0], 1)
    });
    deliveryService.cancelOrder(order2->getId());
}

int main() {
    runDemo();
    return 0;
}