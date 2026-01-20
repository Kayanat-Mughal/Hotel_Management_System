#include "Employee.h"
#include <algorithm> // Add at top of file if not present
#include <cctype> 

// ==================== CONSTRUCTORS ====================
Employee::Employee() : employeeId(0), department(FRONT_DESK), shift(MORNING),
                      salary(0.0) {}

Employee::Employee(int id, const string& n, const string& pos, Department dept,
                   Shift sh, double sal, const string& contact, 
                   const string& addr, const string& joinDt)
    : employeeId(id), name(n), position(pos), department(dept), shift(sh),
      salary(sal), contactNumber(contact), address(addr), joinDate(joinDt) {
    
    if (id <= 0) throw ValidationException("Employee ID must be positive");
    if (name.empty()) throw ValidationException("Name cannot be empty");
    if (position.empty()) throw ValidationException("Position cannot be empty");
    if (salary <= 0) throw ValidationException("Salary must be positive");
    if (!Utils::isValidPhone(contact)) throw ValidationException("Invalid phone number");
    if (address.empty()) throw ValidationException("Address cannot be empty");
    if (joinDate.empty()) throw ValidationException("Join date cannot be empty");
    
    std::string lowerName = name.substr(0, name.find(' '));
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    email = lowerName + "@hotel.com";
    password = "default123";  // Default password
}

// ==================== GETTERS ====================
int Employee::getEmployeeId() const { return employeeId; }
string Employee::getName() const { return name; }
string Employee::getPosition() const { return position; }
Department Employee::getDepartment() const { return department; }
Shift Employee::getShift() const { return shift; }
double Employee::getSalary() const { return salary; }
string Employee::getContactNumber() const { return contactNumber; }
string Employee::getAddress() const { return address; }
string Employee::getJoinDate() const { return joinDate; }
string Employee::getEmail() const { return email; }

// ==================== DEPARTMENT AND SHIFT STRINGS ====================
string Employee::getDepartmentString() const {
    switch(department) {
        case FRONT_DESK: return "Front Desk";
        case HOUSEKEEPING: return "Housekeeping";
        case KITCHEN: return "Kitchen";
        case MANAGEMENT: return "Management";
        default: return "Unknown";
    }
}

string Employee::getShiftString() const {
    switch(shift) {
        case MORNING: return "Morning (8AM-4PM)";
        case AFTERNOON: return "Afternoon (4PM-12AM)";
        case NIGHT: return "Night (12AM-8AM)";
        default: return "Unknown";
    }
}

// ==================== SETTERS ====================
void Employee::setName(const string& newName) {
    if (newName.empty()) throw ValidationException("Name cannot be empty");
    name = newName;
}

void Employee::setPosition(const string& newPosition) {
    if (newPosition.empty()) throw ValidationException("Position cannot be empty");
    position = newPosition;
}

void Employee::setDepartment(Department newDept) {
    department = newDept;
}

void Employee::setShift(Shift newShift) {
    shift = newShift;
}

void Employee::setSalary(double newSalary) {
    if (newSalary <= 0) throw ValidationException("Salary must be positive");
    salary = newSalary;
}

void Employee::setContactNumber(const string& newContact) {
    if (!Utils::isValidPhone(newContact)) throw ValidationException("Invalid phone number");
    contactNumber = newContact;
}

void Employee::setAddress(const string& newAddress) {
    if (newAddress.empty()) throw ValidationException("Address cannot be empty");
    address = newAddress;
}

void Employee::setEmail(const string& newEmail) {
    if (!Utils::isValidEmail(newEmail)) throw ValidationException("Invalid email format");
    email = newEmail;
}

bool Employee::setPassword(const string& newPassword, const string& confirmPassword) {
    if (newPassword.empty()) {
        throw ValidationException("Password cannot be empty");
    }
    if (newPassword.length() < 6) {
        throw ValidationException("Password must be at least 6 characters");
    }
    if (newPassword != confirmPassword) {
        throw ValidationException("Passwords do not match");
    }
    
    password = newPassword;  // In real app, hash this password
    return true;
}

// ==================== AUTHENTICATION ====================
bool Employee::authenticate(const string& inputPassword) const {
    return inputPassword == password;  // In real app, compare hashes
}

// ==================== OPERATIONS ====================
double Employee::calculateMonthlySalary() const {
    return salary;  // Assuming monthly salary
}

bool Employee::isManager() const {
    return position.find("Manager") != string::npos || 
           position.find("Supervisor") != string::npos ||
           department == MANAGEMENT;
}

// ==================== DISPLAY ====================
void Employee::display() const {
    cout << "\n";
    cout << "┌─────────────────────────────────────────────┐\n";
    cout << "│            EMPLOYEE INFORMATION             │\n";
    cout << "├─────────────────────────────────────────────┤\n";
    cout << "│ Employee ID:  " << setw(27) << employeeId << " │\n";
    cout << "│ Name:         " << setw(27) << name << " │\n";
    cout << "│ Position:     " << setw(27) << position << " │\n";
    cout << "│ Department:   " << setw(27) << getDepartmentString() << " │\n";
    cout << "│ Shift:        " << setw(27) << getShiftString() << " │\n";
    cout << "│ Salary:       " << setw(27) << Utils::formatCurrency(salary) << " │\n";
    cout << "│ Contact:      " << setw(27) << contactNumber << " │\n";
    cout << "│ Email:        " << setw(27) << email << " │\n";
    cout << "│ Address:      " << setw(27) << address << " │\n";
    cout << "│ Join Date:    " << setw(27) << joinDate << " │\n";
    cout << "└─────────────────────────────────────────────┘\n";
}

void Employee::displayBrief() const {
    cout << "ID: " << setw(6) << employeeId
         << " | Name: " << setw(20) << left << name
         << " | Dept: " << setw(15) << left << getDepartmentString()
         << " | Position: " << setw(15) << left << position
         << " | Salary: " << Utils::formatCurrency(salary) << "\n";
}

// ==================== FILE OPERATIONS ====================
void Employee::saveToFile(ofstream& file) const {
    if (!file.is_open()) throw FileException("File is not open for writing");
    
    file << employeeId << "|"
         << name << "|"
         << position << "|"
         << static_cast<int>(department) << "|"
         << static_cast<int>(shift) << "|"
         << salary << "|"
         << contactNumber << "|"
         << address << "|"
         << joinDate << "|"
         << email << "|"
         << password << "\n";
}

void Employee::loadFromFile(ifstream& file) {
    if (!file.is_open()) throw FileException("File is not open for reading");
    
    string line;
    if (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 11) {
            employeeId = stoi(tokens[0]);
            name = tokens[1];
            position = tokens[2];
            department = static_cast<Department>(stoi(tokens[3]));
            shift = static_cast<Shift>(stoi(tokens[4]));
            salary = stod(tokens[5]);
            contactNumber = tokens[6];
            address = tokens[7];
            joinDate = tokens[8];
            email = tokens[9];
            password = tokens[10];
        }
    }
}

// ==================== VALIDATION ====================
bool Employee::isValid() const {
    return employeeId > 0 && !name.empty() && !position.empty() &&
           salary > 0 && Utils::isValidPhone(contactNumber) &&
           !address.empty() && !joinDate.empty() &&
           Utils::isValidEmail(email);
}