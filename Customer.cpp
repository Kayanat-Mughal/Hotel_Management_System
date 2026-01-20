#include "Customer.h"

// ==================== CONSTRUCTORS ====================
Customer::Customer() : customerId(0), registrationDate(time(nullptr)), 
                       totalVisits(0), totalSpent(0.0) {}

Customer::Customer(int id, const string& n, const string& e, const string& p, 
                   const string& addr, const string& proof)
    : customerId(id), name(n), email(e), phone(p), address(addr), idProof(proof),
      registrationDate(time(nullptr)), totalVisits(0), totalSpent(0.0) {
    
    if (id <= 0) throw ValidationException("Customer ID must be positive");
    if (name.empty()) throw ValidationException("Name cannot be empty");
    if (!Utils::isValidEmail(email)) throw ValidationException("Invalid email format");
    if (!Utils::isValidPhone(phone)) throw ValidationException("Invalid phone number");
    if (address.empty()) throw ValidationException("Address cannot be empty");
    if (proof.empty()) throw ValidationException("ID proof cannot be empty");
}

// ==================== GETTERS ====================
int Customer::getCustomerId() const { return customerId; }
string Customer::getName() const { return name; }
string Customer::getEmail() const { return email; }
string Customer::getPhone() const { return phone; }
string Customer::getAddress() const { return address; }
string Customer::getIdProof() const { return idProof; }

string Customer::getRegistrationDate() const {
    return Utils::formatDate(registrationDate);
}

int Customer::getTotalVisits() const { return totalVisits; }
double Customer::getTotalSpent() const { return totalSpent; }

// ==================== SETTERS ====================
void Customer::setName(const string& newName) {
    if (newName.empty()) throw ValidationException("Name cannot be empty");
    name = newName;
}

void Customer::setEmail(const string& newEmail) {
    if (!Utils::isValidEmail(newEmail)) throw ValidationException("Invalid email format");
    email = newEmail;
}

void Customer::setPhone(const string& newPhone) {
    if (!Utils::isValidPhone(newPhone)) throw ValidationException("Invalid phone number");
    phone = newPhone;
}

void Customer::setAddress(const string& newAddress) {
    if (newAddress.empty()) throw ValidationException("Address cannot be empty");
    address = newAddress;
}

void Customer::setIdProof(const string& newProof) {
    if (newProof.empty()) throw ValidationException("ID proof cannot be empty");
    idProof = newProof;
}

// ==================== OPERATIONS ====================
void Customer::addVisit(double amount) {
    if (amount < 0) throw ValidationException("Amount cannot be negative");
    totalVisits++;
    totalSpent += amount;
}

void Customer::updateInfo(const string& newPhone, const string& newEmail, 
                         const string& newAddress) {
    setPhone(newPhone);
    setEmail(newEmail);
    setAddress(newAddress);
}

// ==================== DISPLAY ====================
void Customer::display() const {
    cout << "\n";
    cout << "┌─────────────────────────────────────────────┐\n";
    cout << "│            CUSTOMER INFORMATION             │\n";
    cout << "├─────────────────────────────────────────────┤\n";
    cout << "│ Customer ID: " << setw(29) << customerId << " │\n";
    cout << "│ Name:        " << setw(29) << name << " │\n";
    cout << "│ Email:       " << setw(29) << email << " │\n";
    cout << "│ Phone:       " << setw(29) << phone << " │\n";
    cout << "│ Address:     " << setw(29) << address << " │\n";
    cout << "│ ID Proof:    " << setw(29) << idProof << " │\n";
    cout << "│ Registered:  " << setw(29) << getRegistrationDate() << " │\n";
    cout << "│ Total Visits:" << setw(29) << totalVisits << " │\n";
    cout << "│ Total Spent: " << setw(29) << Utils::formatCurrency(totalSpent) << " │\n";
    cout << "└─────────────────────────────────────────────┘\n";
}

void Customer::displayBrief() const {
    cout << "ID: " << setw(6) << customerId 
         << " | Name: " << setw(20) << left << name
         << " | Phone: " << setw(15) << phone
         << " | Visits: " << totalVisits 
         << " | Spent: " << Utils::formatCurrency(totalSpent) << "\n";
}

// ==================== FILE OPERATIONS ====================
void Customer::saveToFile(ofstream& file) const {
    if (!file.is_open()) throw FileException("File is not open for writing");
    
    file << customerId << " "
         << name << " "
         << email << " "
         << phone << " "
         << address << " "
         << idProof << " "
         << registrationDate << " "
         << totalVisits << " "
         << totalSpent << "\n";
}

void Customer::loadFromFile(ifstream& file) {
    if (!file.is_open()) throw FileException("File is not open for reading");
    
    file >> customerId;
    file.ignore();
    getline(file, name, '|');
    getline(file, email, '|');
    getline(file, phone, '|');
    getline(file, address, '|');
    getline(file, idProof, '|');
    file >> registrationDate >> totalVisits >> totalSpent;
}

// ==================== VALIDATION ====================
bool Customer::isValid() const {
    return customerId > 0 && !name.empty() && Utils::isValidEmail(email) &&
           Utils::isValidPhone(phone) && !address.empty() && !idProof.empty();
}