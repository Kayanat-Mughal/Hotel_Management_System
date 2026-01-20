#include "Bill.h"

// ==================== BILLITEM IMPLEMENTATION ====================
BillItem::BillItem(const string& desc, double amt, int qty)
    : description(desc), amount(amt), quantity(qty) {
    if (desc.empty()) throw ValidationException("Description cannot be empty");
    if (amt <= 0) throw ValidationException("Amount must be positive");
    if (qty <= 0) throw ValidationException("Quantity must be positive");
}

string BillItem::getDescription() const { return description; }
double BillItem::getAmount() const { return amount; }
int BillItem::getQuantity() const { return quantity; }

double BillItem::getTotal() const {
    return amount * quantity;
}

void BillItem::display() const {
    cout << setw(30) << left << description 
         << setw(8) << right << quantity
         << " x " << setw(10) << left << Utils::formatCurrency(amount)
         << " = " << setw(12) << right << Utils::formatCurrency(getTotal()) << "\n";
}

// ==================== BILL IMPLEMENTATION ====================
Bill::Bill() : billId(0), reservationId(0), taxRate(0.10), discount(0.0),
               isPaid(false), paymentDate(0) {}

Bill::Bill(int id, int resId, double tax, double disc)
    : billId(id), reservationId(resId), taxRate(tax), discount(disc),
      isPaid(false), paymentDate(0) {
    
    if (id <= 0) throw ValidationException("Bill ID must be positive");
    if (resId <= 0) throw ValidationException("Reservation ID must be positive");
    if (tax < 0) throw ValidationException("Tax rate cannot be negative");
    if (disc < 0 || disc > 1) throw ValidationException("Discount must be between 0 and 1");
}

// ==================== GETTERS ====================
int Bill::getBillId() const { return billId; }
int Bill::getReservationId() const { return reservationId; }
double Bill::getTaxRate() const { return taxRate; }
double Bill::getDiscount() const { return discount; }
string Bill::getPaymentMethod() const { return paymentMethod; }

string Bill::getPaymentDate() const {
    if (paymentDate == 0) return "Not paid";
    return Utils::formatDate(paymentDate);
}

bool Bill::getIsPaid() const { return isPaid; }

// ==================== BILL ITEMS MANAGEMENT ====================
void Bill::addItem(const string& description, double amount, int quantity) {
    items.push_back(BillItem(description, amount, quantity));
}

void Bill::addRoomCharge(double amount, int nights) {
    string desc = "Room Charge (" + to_string(nights) + " nights)";
    addItem(desc, amount, nights);
}

void Bill::addFoodCharge(const string& item, double amount, int quantity) {
    string desc = "Food - " + item;
    addItem(desc, amount, quantity);
}

void Bill::addServiceCharge(const string& service, double amount) {
    string desc = "Service - " + service;
    addItem(desc, amount);
}

void Bill::removeItem(int index) {
    if (index < 0 || index >= (int)items.size()) {
        throw ValidationException("Invalid item index");
    }
    items.erase(items.begin() + index);
}

void Bill::clearItems() {
    items.clear();
}

// ==================== CALCULATIONS ====================
double Bill::calculateSubtotal() const {
    double subtotal = 0.0;
    for (const auto& item : items) {
        subtotal += item.getTotal();
    }
    return subtotal;
}

double Bill::calculateTax() const {
    return calculateSubtotal() * taxRate;
}

double Bill::calculateDiscountAmount() const {
    double subtotal = calculateSubtotal();
    double tax = calculateTax();
    return (subtotal + tax) * discount;
}

double Bill::calculateTotal() const {
    double subtotal = calculateSubtotal();
    double tax = calculateTax();
    double discountAmount = calculateDiscountAmount();
    return subtotal + tax - discountAmount;
}

// ==================== PAYMENT ====================
bool Bill::processPayment(const string& method) {
    if (method.empty()) throw ValidationException("Payment method cannot be empty");
    if (isPaid) throw ValidationException("Bill is already paid");
    
    paymentMethod = method;
    paymentDate = time(nullptr);
    isPaid = true;
    
    return true;
}

bool Bill::isPaymentComplete() const {
    return isPaid;
}

double Bill::getBalanceDue() const {
    return isPaid ? 0.0 : calculateTotal();
}

// ==================== DISPLAY ====================
void Bill::display() const {
    double subtotal = calculateSubtotal();
    double tax = calculateTax();
    double discountAmount = calculateDiscountAmount();
    double total = calculateTotal();
    
    cout << "\n";
    cout << "┌─────────────────────────────────────────────┐\n";
    cout << "│                 BILL SUMMARY                │\n";
    cout << "├─────────────────────────────────────────────┤\n";
    cout << "│ Bill ID:         " << setw(25) << billId << " │\n";
    cout << "│ Reservation ID:  " << setw(25) << reservationId << " │\n";
    cout << "│ Subtotal:        " << setw(25) << Utils::formatCurrency(subtotal) << " │\n";
    cout << "│ Tax (" << (taxRate * 100) << "%):      " << setw(25) << Utils::formatCurrency(tax) << " │\n";
    cout << "│ Discount:        " << setw(25) << Utils::formatCurrency(discountAmount) << " │\n";
    cout << "│ Total:           " << setw(25) << Utils::formatCurrency(total) << " │\n";
    cout << "│ Payment Status:  " << setw(25) << (isPaid ? "PAID" : "PENDING") << " │\n";
    if (isPaid) {
        cout << "│ Payment Method:  " << setw(25) << paymentMethod << " │\n";
        cout << "│ Payment Date:    " << setw(25) << getPaymentDate() << " │\n";
    }
    cout << "└─────────────────────────────────────────────┘\n";
}

void Bill::displayDetailed() const {
    display();
    
    if (!items.empty()) {
        cout << "\n══════════════════════════════════════════════\n";
        cout << "              ITEMIZED BREAKDOWN\n";
        cout << "══════════════════════════════════════════════\n";
        
        for (const auto& item : items) {
            item.display();
        }
        
        cout << "══════════════════════════════════════════════\n";
    }
}

// ==================== FILE OPERATIONS ====================
void Bill::saveToFile(ofstream& file) const {
    if (!file.is_open()) throw FileException("File is not open for writing");
    
    file << billId << " "
         << reservationId << " "
         << taxRate << " "
         << discount << " "
         << isPaid << " "
         << paymentMethod << " "
         << paymentDate << " "
         << items.size() << "\n";
    
    for (const auto& item : items) {
        file << item.getDescription() << "|"
             << item.getAmount() << "|"
             << item.getQuantity() << "\n";
    }
}

void Bill::loadFromFile(ifstream& file) {
    if (!file.is_open()) throw FileException("File is not open for reading");
    
    int itemCount;
    file >> billId >> reservationId >> taxRate >> discount 
         >> isPaid >> paymentMethod >> paymentDate >> itemCount;
    
    items.clear();
    for (int i = 0; i < itemCount; i++) {
        string line;
        getline(file, line);
        
        if (!line.empty()) {
            stringstream ss(line);
            string desc, amountStr, qtyStr;
            
            getline(ss, desc, '|');
            getline(ss, amountStr, '|');
            getline(ss, qtyStr, '|');
            
            double amount = stod(amountStr);
            int quantity = stoi(qtyStr);
            
            items.push_back(BillItem(desc, amount, quantity));
        }
    }
}

// ==================== VALIDATION ====================
bool Bill::isValid() const {
    return billId > 0 && reservationId > 0 && taxRate >= 0 &&
           discount >= 0 && discount <= 1;
}