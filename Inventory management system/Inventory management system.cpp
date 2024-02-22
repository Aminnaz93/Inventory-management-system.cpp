#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Product {
    protected:
    std::string name;
    double price;
    int quantity;

public:
    Product(std::string name, double price, int quantity)
        : name(name), price(price), quantity(quantity) {}

    virtual double calculateTotalPrice() const {
        return price * quantity;
    }

    virtual void inputDetails() {
        std::cout << "Enter product details:\n";
        std::cout << "Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Price: ";
        std::cin >> price;
        std::cout << "Quantity: ";
        std::cin >> quantity;
    }

    virtual void displayDetails() const {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Price: " << price << " kr" << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
    }

    virtual void writeToFile(std::ofstream& file) const {
        file << name << " " << price << " " << quantity << " ";
    }

    virtual void readFromFile(std::ifstream& file) {
        file >> name >> price >> quantity;
    }
};

class Electronics : public Product {
    private:
    int warranty;

public:
    Electronics(std::string name, double price, int quantity, int warranty)
        : Product(name, price, quantity), warranty(warranty) {}

    double calculateTotalPrice() const override {
        return price * quantity;
    }

    void inputDetails() override {
        Product::inputDetails();
        std::cout << "Warranty (in months): ";
        std::cin >> warranty;
    }

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << "Warranty: " << warranty << " months\n";
    }

    void writeToFile(std::ofstream& file) const override {
        file << "E ";
        Product::writeToFile(file);
        file << warranty << "\n";
    }

    void readFromFile(std::ifstream& file) override {
        Product::readFromFile(file);
        file >> warranty;
    }

};

class Clothing : public Product {
    private:
    std::string size;

public:
    Clothing(std::string name, double price, int quantity, std::string size)
        : Product(name, price, quantity), size(size) {}

    double calculateTotalPrice() const override {
        return price * quantity;
    }

    void inputDetails() override {
        Product::inputDetails();
        std::cout << "Size: ";
        std::cin >> size;
    }

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << "Size: " << size << "\n";
    }

    void writeToFile(std::ofstream& file) const override {
        file << "C ";
        Product::writeToFile(file);
        file << size << "\n";
    }

    void readFromFile(std::ifstream& file) override {
        Product::readFromFile(file);
        file >> size;
    }

};

class Food : public Product {
    private:
    std::string expirationDate;

public:
    Food(std::string name, double price, int quantity, std::string expirationDate)
        : Product(name, price, quantity), expirationDate(expirationDate) {}

    double calculateTotalPrice() const override {
        return price * quantity;
    }

    void inputDetails() override {
        Product::inputDetails();
        std::cout << "Expiration Date: ";
        std::cin >> expirationDate;
    }

    void displayDetails() const override {
        Product::displayDetails();
        std::cout << "Expiration Date: " << expirationDate << "\n";
    }

    void writeToFile(std::ofstream& file) const override {
        file << "F ";
        Product::writeToFile(file);
        file << expirationDate << "\n";
    }

    void readFromFile(std::ifstream& file) override {
        Product::readFromFile(file);
        file >> expirationDate;
    }

};

class InventoryManager {
public:
    // Konstruktor som laddar produkter från filen vid programmets start
    InventoryManager() {
        loadFromFile();
    }

    void addProduct() {
        char choice;
        std::cout << "Select product type to add (E for Electronics, C for Clothing, F for Food): ";
        std::cin >> choice;

        // Konvertera valet till små bokstäver
        choice = std::tolower(choice);

        Product* newProduct = nullptr;

        switch (choice) {
            case 'e':
                newProduct = new Electronics("", 0.0, 0, 0);
                break;
            case 'c':
                newProduct = new Clothing("", 0.0, 0, "");
                break;
            case 'f':
                newProduct = new Food("", 0.0, 0, "");
                break;
            default:
                std::cout << "Invalid choice\n";
                return;
        }

        newProduct->inputDetails();
        products.push_back(newProduct);
        std::cout << "Product added successfully!\n";

        // Spara endast produkten som nyligen lagts till
        saveToFile();
    }

    void displayInventory() const {
        if (products.empty()) {
            std::cout << "Inventory is empty\n";
            return;
        }

        for (const auto& product : products) {
            product->displayDetails();
            std::cout << "Total Price: " << product->calculateTotalPrice() << " kr" << std::endl;
            std::cout << "------------------------\n";
        }
    }

    void saveToFile() const {
        std::ofstream file("inventory.txt");

        if (!file.is_open()) {
            std::cerr << "Error opening file for writing\n";
            return;
        }

        for (const auto& product : products) {
            product->writeToFile(file);
        }

        file.close();
        std::cout << "Inventory saved to file\n";
    }

    void loadFromFile() {
        std::ifstream file("inventory.txt");

        if (!file.is_open()) {
            // Om filen inte kan öppnas, skriv bara ut ett felmeddelande
            std::cerr << "Error opening file for reading\n";
            return;
        }

        char type;
        while (file >> type) {
            Product* newProduct = nullptr;

            switch (type) {
                case 'E':
                    newProduct = new Electronics("", 0.0, 0, 0);
                    break;
                case 'C':
                    newProduct = new Clothing("", 0.0, 0, "");
                    break;
                case 'F':
                    newProduct = new Food("", 0.0, 0, "");
                    break;
                default:
                    std::cerr << "Invalid type in file\n";
                    delete newProduct; // Cleanup
                    continue; // Skip to the next iteration
            }

            newProduct->readFromFile(file);
            products.push_back(newProduct);
        }

        file.close();
        std::cout << "Inventory loaded from file\n";
    }

    // Destruktor som tar hand om att rensa upp produkterna när programmet avslutas
    ~InventoryManager() {
        for (auto& product : products) {
            delete product;
        }
    }

private:
    std::vector<Product*> products;
};

int main() {
    InventoryManager inventory;

    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Add Product\n";
        std::cout << "2. Display Inventory\n";
        std::cout << "3. Exit\n";

        char choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                inventory.addProduct();
                break;
            case '2':
                inventory.displayInventory();
                break;
            case '3':
                // Spara produkterna till filen innan du avslutar programmet
                inventory.saveToFile();
                std::cout << "Exiting program\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
