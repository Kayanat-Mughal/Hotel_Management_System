#ifndef BILL_H
#define BILL_H

#include "Utils.h"
#include "Reservation.h"
#include <vector>

class BillItem {
private:
    string description;
    double amount;
    int quantity;
    
public:
    BillItem(const string& desc, double amt, int qty = 1);
    
    // Getters
    string getDescription() const;
    double getAmount() const;
    int getQuantity() const;
    double getTotal() const;
    
    // Display
    void display() const;
};

class Bill {
private:
    int billId;
    int reservationId;
    vector<BillItem> items;
    double taxRate;
    double discount;
    string paymentMethod;
    time_t paymentDate;
    bool isPaid;
    
public:
    // Constructors
    Bill();
    Bill(int id, int resId, double tax = 0.10, double disc = 0.0);
    
    // Getters
    int getBillId() const;
    int getReservationId() const;
    double getTaxRate() const;
    double getDiscount() const;
    string getPaymentMethod() const;
    string getPaymentDate() const;
    bool getIsPaid() const;
    
    // Bill items management
    void addItem(const string& description, double amount, int quantity = 1);
    void addRoomCharge(double amount, int nights);
    void addFoodCharge(const string& item, double amount, int quantity = 1);
    void addServiceCharge(const string& service, double amount);
    void removeItem(int index);
    void clearItems();
    
    // Calculations
    double calculateSubtotal() const;
    double calculateTax() const;
    double calculateDiscountAmount() const;
    double calculateTotal() const;
    
    // Payment
    bool processPayment(const string& method);
    bool isPaymentComplete() const;
    double getBalanceDue() const;
    
    // Display
    void display() const;
    void displayDetailed() const;
    
    // File operations
    void saveToFile(ofstream& file) const;
    void loadFromFile(ifstream& file);
    
    // Validation
    bool isValid() const;
};

#endif // BILL_H