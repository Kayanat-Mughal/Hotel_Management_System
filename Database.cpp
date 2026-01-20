#include "Database.h"
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <vector>


Database::Database() : nextRoomNumber(101), nextCustomerId(1001), 
                      nextReservationId(10001), nextEmployeeId(201), 
                      nextBillId(5001) {
    createDataDirectory();
    loadAllData();
    if (rooms.empty() && customers.empty() && employees.empty()) {
        initializeSampleData();
    }
}

Database::~Database() {
    saveAllData();
}

void Database::createDataDirectory() {
    Utils::createDirectory("data");
}

// ==================== ROOM OPERATIONS ====================
int Database::addRoom(RoomType type, double price, int capacity, const vector<string>& features) {
    try {
        Room newRoom(nextRoomNumber++, type, price, capacity, features);
        rooms.push_back(newRoom);
        saveRooms();
        return newRoom.getRoomNumber();
    } catch (const HotelException&) {
        throw;
    }
}

Room* Database::findRoom(int roomNumber) {
    for (auto& room : rooms) {
        if (room.getRoomNumber() == roomNumber) {
            return &room;
        }
    }
    return nullptr;
}

// Correct implementation should match header declaration
std::vector<Room*> Database::findAvailableRooms(RoomType type, int capacity) {
    std::vector<Room*> availableRooms;
    
    // Use traditional for loop instead of range-based
    for (auto& room : rooms) {  // room is Room& reference
        if (room.getType() == type && 
            room.getCapacity() >= capacity && 
            room.isAvailable()) {
            availableRooms.push_back(&room);  // Get address of object
        }
    }
    
    return availableRooms;
}
bool Database::updateRoomStatus(int roomNumber, RoomStatus status) {
    Room* room = findRoom(roomNumber);
    if (room) {
        room->setStatus(status);
        saveRooms();
        return true;
    }
    return false;
}

bool Database::modifyRoom(int roomNumber, double newPrice, const vector<string>& newFeatures) {
    Room* room = findRoom(roomNumber);
    if (room) {
        try {
            room->setPrice(newPrice);
            room->setFeatures(newFeatures);
            saveRooms();
            return true;
        } catch (const HotelException& e) {
            throw;
        }
    }
    return false;
}

void Database::displayAllRooms() const {
    if (rooms.empty()) {
        cout << "\nNo rooms available.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "                 ALL ROOMS (" << rooms.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (const auto& room : rooms) {
        room.displayBrief();
    }
}

void Database::displayAvailableRooms() const {
    auto available = const_cast<Database*>(this)->findAvailableRooms(RoomType::STANDARD, 2);  
    
    if (available.empty()) {
        cout << "\nNo available rooms at the moment.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "          AVAILABLE ROOMS (" << available.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (const auto& room : available) {
        room->displayBrief();
    }
}

int Database::getRoomCount() const {
    return rooms.size();
}

int Database::getAvailableRoomCount() const {
    int count = 0;
    for (const auto& room : rooms) {
        if (room.isAvailable()) count++;
    }
    return count;
}

// ==================== CUSTOMER OPERATIONS ====================
int Database::addCustomer(const string& name, const string& email, const string& phone,
                         const string& address, const string& idProof) {
    try {
        Customer newCustomer(nextCustomerId++, name, email, phone, address, idProof);
        customers.push_back(newCustomer);
        saveCustomers();
        return newCustomer.getCustomerId();
    } catch (const HotelException& e) {
        throw;
    }
}

Customer* Database::findCustomer(int customerId) {
    for (auto& customer : customers) {
        if (customer.getCustomerId() == customerId) {
            return &customer;
        }
    }
    return nullptr;
}

Customer* Database::findCustomerByName(const string& name) {
    string searchName = Utils::toLower(name);
    for (auto& customer : customers) {
        if (Utils::toLower(customer.getName()).find(searchName) != string::npos) {
            return &customer;
        }
    }
    return nullptr;
}

vector<Customer*> Database::findCustomersByPhone(const string& phone) {
    vector<Customer*> result;
    for (auto& customer : customers) {
        if (customer.getPhone().find(phone) != string::npos) {
            result.push_back(&customer);
        }
    }
    return result;
}

void Database::displayAllCustomers() const {
    if (customers.empty()) {
        cout << "\nNo customers registered.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "              ALL CUSTOMERS (" << customers.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (const auto& customer : customers) {
        customer.displayBrief();
    }
}

bool Database::updateCustomerInfo(int customerId, const string& phone,
                                 const string& email, const string& address) {
    Customer* customer = findCustomer(customerId);
    if (customer) {
        try {
            customer->updateInfo(phone, email, address);
            saveCustomers();
            return true;
        } catch (const HotelException& e) {
            throw;
        }
    }
    return false;
}

int Database::getCustomerCount() const {
    return customers.size();
}

// ==================== RESERVATION OPERATIONS ====================
int Database::makeReservation(int customerId, int roomNumber, time_t checkIn,
                             time_t checkOut, int guests, const string& requests) {
    try {
        // Check if customer exists
        Customer* customer = findCustomer(customerId);
        if (!customer) {
            throw ValidationException("Customer not found");
        }
        
        // Check if room exists and is available
        Room* room = findRoom(roomNumber);
        if (!room) {
            throw ValidationException("Room not found");
        }
        if (!room->isAvailable()) {
            throw RoomException("Room is not available");
        }
        if (!room->canAccommodate(guests)) {
            throw ValidationException("Room cannot accommodate " + to_string(guests) + " guests");
        }
        
        // Calculate room rate
        double roomRate = room->getPricePerNight();
        
        // Create reservation
        Reservation newReservation(nextReservationId++, customerId, roomNumber, 
                                  checkIn, checkOut, guests, roomRate);
        
        if (!requests.empty()) {
            newReservation.setSpecialRequests(requests);
        }
        
        // Update room status
        room->setStatus(RESERVED);
        
        // Save changes
        reservations.push_back(newReservation);
        saveReservations();
        saveRooms();
        
        return newReservation.getReservationId();
        
    } catch (const HotelException& e) {
        throw;
    }
}

Reservation* Database::findReservation(int reservationId) {
    for (auto& reservation : reservations) {
        if (reservation.getReservationId() == reservationId) {
            return &reservation;
        }
    }
    return nullptr;
}

vector<Reservation*> Database::findReservationsByCustomer(int customerId) {
    vector<Reservation*> result;
    for (auto& reservation : reservations) {
        if (reservation.getCustomerId() == customerId) {
            result.push_back(&reservation);
        }
    }
    return result;
}

vector<Reservation*> Database::findActiveReservations() const {
    vector<Reservation*> result;
    for (const auto& reservation : reservations) {
        if (reservation.isActive()) {
            result.push_back(const_cast<Reservation*>(&reservation));
        }
    }
    return result;
}

vector<Reservation*> Database::findTodayCheckIns() const {
    vector<Reservation*> result;
    time_t today = Utils::parseDate(Utils::getCurrentDate());
    
    for (const auto& reservation : reservations) {
        if (reservation.getStatus() == CONFIRMED && 
            Utils::daysBetween(today, reservation.getRawCheckIn()) == 0) {
            result.push_back(const_cast<Reservation*>(&reservation));
        }
    }
    return result;
}

vector<Reservation*> Database::findTodayCheckOuts() const {
    vector<Reservation*> result;
    time_t today = Utils::parseDate(Utils::getCurrentDate());
    
    for (const auto& reservation : reservations) {
        if (reservation.getStatus() == CHECKED_IN && 
            Utils::daysBetween(today, reservation.getRawCheckOut()) == 0) {
            result.push_back(const_cast<Reservation*>(&reservation));
        }
    }
    return result;
}

bool Database::cancelReservation(int reservationId) {
    Reservation* reservation = findReservation(reservationId);
    if (reservation && reservation->cancel()) {
        // Free the room
        Room* room = findRoom(reservation->getRoomNumber());
        if (room) {
            room->setStatus(AVAILABLE);
            saveRooms();
        }
        saveReservations();
        return true;
    }
    return false;
}

bool Database::checkIn(int reservationId) {
    Reservation* reservation = findReservation(reservationId);
    if (reservation && reservation->checkIn()) {
        Room* room = findRoom(reservation->getRoomNumber());
        if (room) {
            room->setStatus(OCCUPIED);
            saveRooms();
        }
        saveReservations();
        return true;
    }
    return false;
}

bool Database::checkOut(int reservationId) {
    Reservation* reservation = findReservation(reservationId);
    if (reservation && reservation->checkOut()) {
        Room* room = findRoom(reservation->getRoomNumber());
        if (room) {
            room->setStatus(AVAILABLE);
            saveRooms();
        }
        saveReservations();
        return true;
    }
    return false;
}

int Database::getReservationCount() const {
    return reservations.size();
}

int Database::getActiveReservationCount() const {
    return findActiveReservations().size();
}

// ==================== EMPLOYEE OPERATIONS ====================
int Database::addEmployee(const string& name, const string& position, Department dept,
                         Shift shift, double salary, const string& contact,
                         const string& address, const string& joinDate) {
    try {
        Employee newEmployee(nextEmployeeId++, name, position, dept, shift, 
                           salary, contact, address, joinDate);
        employees.push_back(newEmployee);
        saveEmployees();
        return newEmployee.getEmployeeId();
    } catch (const HotelException& e) {
        throw;
    }
}

Employee* Database::findEmployee(int employeeId) {
    for (auto& employee : employees) {
        if (employee.getEmployeeId() == employeeId) {
            return &employee;
        }
    }
    return nullptr;
}

Employee* Database::authenticateEmployee(const string& email, const string& password) {
    for (auto& employee : employees) {
        if (employee.getEmail() == email && employee.authenticate(password)) {
            return &employee;
        }
    }
    return nullptr;
}

vector<Employee*> Database::findEmployeesByDepartment(Department dept) {
    vector<Employee*> result;
    for (auto& employee : employees) {
        if (employee.getDepartment() == dept) {
            result.push_back(&employee);
        }
    }
    return result;
}

void Database::displayAllEmployees() const {
    if (employees.empty()) {
        cout << "\nNo employees registered.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "              ALL EMPLOYEES (" << employees.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (const auto& employee : employees) {
        employee.displayBrief();
    }
}

bool Database::updateEmployeeInfo(int employeeId, const string& position,
                                 Department dept, Shift shift, double salary) {
    Employee* employee = findEmployee(employeeId);
    if (employee) {
        try {
            employee->setPosition(position);
            employee->setDepartment(dept);
            employee->setShift(shift);
            employee->setSalary(salary);
            saveEmployees();
            return true;
        } catch (const HotelException& e) {
            throw;
        }
    }
    return false;
}

int Database::getEmployeeCount() const {
    return employees.size();
}

// ==================== BILLING OPERATIONS ====================
int Database::createBill(int reservationId, double taxRate, double discount) {
    try {
        Reservation* reservation = findReservation(reservationId);
        if (!reservation) {
            throw ValidationException("Reservation not found");
        }
        
        Bill newBill(nextBillId++, reservationId, taxRate, discount);
        bills.push_back(newBill);
        saveBills();
        return newBill.getBillId();
    } catch (const HotelException& e) {
        throw;
    }
}

Bill* Database::findBill(int billId) {
    for (auto& bill : bills) {
        if (bill.getBillId() == billId) {
            return &bill;
        }
    }
    return nullptr;
}

Bill* Database::findBillByReservation(int reservationId) {
    for (auto& bill : bills) {
        if (bill.getReservationId() == reservationId) {
            return &bill;
        }
    }
    return nullptr;
}

bool Database::addBillItem(int billId, const string& description, double amount, int quantity) {
    Bill* bill = findBill(billId);
    if (bill) {
        try {
            bill->addItem(description, amount, quantity);
            saveBills();
            return true;
        } catch (const HotelException& e) {
            throw;
        }
    }
    return false;
}

bool Database::processPayment(int billId, const string& paymentMethod) {
    Bill* bill = findBill(billId);
    if (bill) {
        try {
            bill->processPayment(paymentMethod);
            saveBills();
            return true;
        } catch (const HotelException& e) {
            throw;
        }
    }
    return false;
}

void Database::displayAllBills() const {
    if (bills.empty()) {
        cout << "\nNo bills available.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "                 ALL BILLS (" << bills.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    double totalRevenue = 0;
    for (const auto& bill : bills) {
        cout << "Bill ID: " << bill.getBillId() 
             << " | Reservation: " << bill.getReservationId()
             << " | Amount: " << Utils::formatCurrency(bill.calculateTotal())
             << " | Status: " << (bill.getIsPaid() ? "PAID" : "PENDING") << "\n";
        totalRevenue += bill.calculateTotal();
    }
    
    cout << "\nTotal Revenue: " << Utils::formatCurrency(totalRevenue) << "\n";
}

void Database::displayUnpaidBills() const {
    vector<Bill*> unpaid;
    for (const auto& bill : bills) {
        if (!bill.getIsPaid()) {
            unpaid.push_back(const_cast<Bill*>(&bill));
        }
    }
    
    if (unpaid.empty()) {
        cout << "\nNo unpaid bills.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "              UNPAID BILLS (" << unpaid.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    double totalDue = 0;
    for (const auto& bill : unpaid) {
        cout << "Bill ID: " << bill->getBillId() 
             << " | Reservation: " << bill->getReservationId()
             << " | Due: " << Utils::formatCurrency(bill->getBalanceDue()) << "\n";
        totalDue += bill->getBalanceDue();
    }
    
    cout << "\nTotal Amount Due: " << Utils::formatCurrency(totalDue) << "\n";
}

double Database::calculateTotalRevenue() const {
    double total = 0;
    for (const auto& bill : bills) {
        if (bill.getIsPaid()) {
            total += bill.calculateTotal();
        }
    }
    return total;
}

double Database::calculateTodayRevenue() const {
    double total = 0;
    string today = Utils::getCurrentDate();
    
    // This is simplified - in real app, track payment dates
    for (const auto& bill : bills) {
        if (bill.getIsPaid()) {
            total += bill.calculateTotal();
        }
    }
    return total * 0.1; // Assume 10% of total is today's revenue for demo
}

int Database::getBillCount() const {
    return bills.size();
}

// ==================== FILE OPERATIONS ====================
bool Database::saveAllData() {
    try {
        return saveRooms() && saveCustomers() && saveReservations() &&
               saveEmployees() && saveBills();
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::loadAllData() {
    try {
        bool success = true;
        success = loadRooms() && success;
        success = loadCustomers() && success;
        success = loadReservations() && success;
        success = loadEmployees() && success;
        success = loadBills() && success;
        
        // Update ID counters
        if (!rooms.empty()) {
            nextRoomNumber = rooms.back().getRoomNumber() + 1;
        }
        if (!customers.empty()) {
            nextCustomerId = customers.back().getCustomerId() + 1;
        }
        if (!reservations.empty()) {
            nextReservationId = reservations.back().getReservationId() + 1;
        }
        if (!employees.empty()) {
            nextEmployeeId = employees.back().getEmployeeId() + 1;
        }
        if (!bills.empty()) {
            nextBillId = bills.back().getBillId() + 1;
        }
        
        return success;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::backupData() const {
    // Simplified backup - just copy files
    try {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localTime);
        
        string backupDir = "backup_" + string(timestamp);
        Utils::createDirectory(backupDir);
        
        // Copy all data files
        ifstream src;
        ofstream dst;
        
        vector<pair<string, string>> files = {
            {ROOMS_FILE, backupDir + "/rooms.dat"},
            {CUSTOMERS_FILE, backupDir + "/customers.dat"},
            {RESERVATIONS_FILE, backupDir + "/reservations.dat"},
            {EMPLOYEES_FILE, backupDir + "/employees.dat"},
            {BILLS_FILE, backupDir + "/bills.dat"}
        };
        
        for (const auto& file : files) {
            src.open(file.first, ios::binary);
            dst.open(file.second, ios::binary);
            dst << src.rdbuf();
            src.close();
            dst.close();
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::restoreData(const string& backupDir) {
    // Implementation for restore
    return false; // Simplified
}

// ==================== PRIVATE FILE OPERATIONS ====================
bool Database::saveRooms() {
    try {
        ofstream file(ROOMS_FILE);
        if (!file.is_open()) throw FileException("Cannot open rooms file");
        
        file << rooms.size() << "\n";
        for (const auto& room : rooms) {
            room.saveToFile(file);
        }
        
        file.close();
        return true;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::saveCustomers() {
    try {
        ofstream file(CUSTOMERS_FILE);
        if (!file.is_open()) throw FileException("Cannot open customers file");
        
        file << customers.size() << "\n";
        for (const auto& customer : customers) {
            customer.saveToFile(file);
        }
        
        file.close();
        return true;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::saveReservations() {
    try {
        ofstream file(RESERVATIONS_FILE);
        if (!file.is_open()) throw FileException("Cannot open reservations file");
        
        file << reservations.size() << "\n";
        for (const auto& reservation : reservations) {
            reservation.saveToFile(file);
        }
        
        file.close();
        return true;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::saveEmployees() {
    try {
        ofstream file(EMPLOYEES_FILE);
        if (!file.is_open()) throw FileException("Cannot open employees file");
        
        file << employees.size() << "\n";
        for (const auto& employee : employees) {
            employee.saveToFile(file);
        }
        
        file.close();
        return true;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::saveBills() {
    try {
        ofstream file(BILLS_FILE);
        if (!file.is_open()) throw FileException("Cannot open bills file");
        
        file << bills.size() << "\n";
        for (const auto& bill : bills) {
            bill.saveToFile(file);
        }
        
        file.close();
        return true;
    } catch (const FileException& e) {
        throw;
    }
}

bool Database::loadRooms() {
    try {
        ifstream file(ROOMS_FILE);
        if (!file.is_open()) return false;
        
        int count;
        file >> count;
        file.ignore();
        
        rooms.clear();
        for (int i = 0; i < count; i++) {
            Room room;
            room.loadFromFile(file);
            rooms.push_back(room);
        }
        
        file.close();
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::loadCustomers() {
    try {
        ifstream file(CUSTOMERS_FILE);
        if (!file.is_open()) return false;
        
        int count;
        file >> count;
        file.ignore();
        
        customers.clear();
        for (int i = 0; i < count; i++) {
            Customer customer;
            customer.loadFromFile(file);
            customers.push_back(customer);
        }
        
        file.close();
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::loadReservations() {
    try {
        ifstream file(RESERVATIONS_FILE);
        if (!file.is_open()) return false;
        
        int count;
        file >> count;
        file.ignore();
        
        reservations.clear();
        for (int i = 0; i < count; i++) {
            Reservation reservation;
            reservation.loadFromFile(file);
            reservations.push_back(reservation);
        }
        
        file.close();
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::loadEmployees() {
    try {
        ifstream file(EMPLOYEES_FILE);
        if (!file.is_open()) return false;
        
        int count;
        file >> count;
        file.ignore();
        
        employees.clear();
        for (int i = 0; i < count; i++) {
            Employee employee;
            employee.loadFromFile(file);
            employees.push_back(employee);
        }
        
        file.close();
        return true;
    } catch (...) {
        return false;
    }
}

bool Database::loadBills() {
    try {
        ifstream file(BILLS_FILE);
        if (!file.is_open()) return false;
        
        int count;
        file >> count;
        file.ignore();
        
        bills.clear();
        for (int i = 0; i < count; i++) {
            Bill bill;
            bill.loadFromFile(file);
            bills.push_back(bill);
        }
        
        file.close();
        return true;
    } catch (...) {
        return false;
    }
}

// ==================== STATISTICS ====================
void Database::showStatistics() const {
    cout << "\n══════════════════════════════════════════════\n";
    cout << "              HOTEL STATISTICS\n";
    cout << "══════════════════════════════════════════════\n";
    cout << "Total Rooms:           " << setw(10) << getRoomCount() << "\n";
    cout << "Available Rooms:       " << setw(10) << getAvailableRoomCount() << "\n";
    cout << "Occupancy Rate:        " << setw(10) 
         << fixed << setprecision(1) 
         << (getRoomCount() > 0 ? 
             (1.0 - (double)getAvailableRoomCount() / getRoomCount()) * 100 : 0)
         << "%\n";
    cout << "Total Customers:       " << setw(10) << getCustomerCount() << "\n";
    cout << "Active Reservations:   " << setw(10) << getActiveReservationCount() << "\n";
    cout << "Total Employees:       " << setw(10) << getEmployeeCount() << "\n";
    cout << "Total Revenue:         " << setw(10) << Utils::formatCurrency(calculateTotalRevenue()) << "\n";
    cout << "Today's Revenue:       " << setw(10) << Utils::formatCurrency(calculateTodayRevenue()) << "\n";
    cout << "══════════════════════════════════════════════\n";
}

map<string, double> Database::getDailyRevenue(int days) const {
    map<string, double> revenue;
    // Simplified implementation
    for (int i = 0; i < days; i++) {
        string date = Utils::formatDate(time(nullptr) - i * 24 * 60 * 60);
        revenue[date] = calculateTodayRevenue() * 0.8; // Sample data
    }
    return revenue;
}

map<string, int> Database::getOccupancyRate() const {
    map<string, int> occupancy;
    // Simplified implementation
    occupancy["Standard"] = 75;
    occupancy["Deluxe"] = 85;
    occupancy["Suite"] = 60;
    occupancy["Presidential"] = 40;
    return occupancy;
}

vector<pair<string, int>> Database::getPopularRooms() const {
    vector<pair<string, int>> popular;
    // Simplified implementation
    popular.push_back({"Standard", 45});
    popular.push_back({"Deluxe", 30});
    popular.push_back({"Suite", 15});
    popular.push_back({"Presidential", 10});
    return popular;
}

// ==================== SEARCH OPERATIONS ====================
vector<Room*> Database::searchRooms(double maxPrice, int minCapacity, RoomType type) {
    vector<Room*> result;
    for (auto& room : rooms) {
        if (room.isAvailable() &&
            room.getPricePerNight() <= maxPrice &&
            room.canAccommodate(minCapacity) &&
            (type == STANDARD || room.getType() == type)) {
            result.push_back(&room);
        }
    }
    return result;
}

vector<Customer*> Database::searchCustomers(const string& keyword) {
    vector<Customer*> result;
    string searchKey = Utils::toLower(keyword);
    
    for (auto& customer : customers) {
        if (Utils::toLower(customer.getName()).find(searchKey) != string::npos ||
            Utils::toLower(customer.getEmail()).find(searchKey) != string::npos ||
            customer.getPhone().find(keyword) != string::npos) {
            result.push_back(&customer);
        }
    }
    return result;
}

vector<Reservation*> Database::searchReservationsByDateRange(time_t start, time_t end) {
    vector<Reservation*> result;
    for (auto& reservation : reservations) {
        if (reservation.getRawCheckIn() >= start && reservation.getRawCheckOut() <= end) {
            result.push_back(&reservation);
        }
    }
    return result;
}

void Database::displayAllReservations() const {
    if (reservations.empty()) {
        cout << "\nNo reservations available.\n";
        return;
    }
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "           ALL RESERVATIONS (" << reservations.size() << ")\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (const auto& reservation : reservations) {
        reservation.displayBrief();
    }
}
// ==================== SAMPLE DATA ====================
void Database::initializeSampleData() {
    try {
        // Add sample rooms
        addRoom(STANDARD, 100.0, 2, {"WiFi", "TV", "AC"});
        addRoom(STANDARD, 120.0, 3, {"WiFi", "TV", "AC", "Mini-fridge"});
        addRoom(DELUXE, 200.0, 2, {"WiFi", "Smart TV", "AC", "Mini-bar", "Jacuzzi"});
        addRoom(DELUXE, 250.0, 4, {"WiFi", "Smart TV", "AC", "Mini-bar", "Living area"});
        addRoom(SUITE, 350.0, 3, {"WiFi", "Smart TV", "AC", "Kitchenette", "Dining area"});
        addRoom(PRESIDENTIAL, 800.0, 2, {"WiFi", "4K TV", "AC", "Full kitchen", "Butler service", "Private pool"});
        
        // Add sample customers
        addCustomer("John Smith", "john.smith@email.com", "+1-555-0101", 
                   "123 Main Street, New York", "PASSPORT-XYZ123");
        addCustomer("Emma Johnson", "emma.j@email.com", "+1-555-0102", 
                   "456 Oak Avenue, Los Angeles", "DL-AB123456");
        addCustomer("Michael Brown", "michael.b@email.com", "+1-555-0103", 
                   "789 Pine Road, Chicago", "PASSPORT-ABC789");
        
        // Add sample employees
        addEmployee("Robert Wilson", "Front Desk Manager", MANAGEMENT, MORNING,
                   3500.0, "+1-555-0201", "101 Maple Blvd, New York", "2023-03-15");
        addEmployee("Lisa Taylor", "Receptionist", FRONT_DESK, MORNING,
                   2500.0, "+1-555-0202", "202 Cedar Lane, New York", "2023-01-10");
        addEmployee("David Miller", "Housekeeping Supervisor", HOUSEKEEPING, MORNING,
                   2800.0, "+1-555-0203", "303 Birch St, New York", "2023-06-20");
        
        // Add sample reservations
        time_t now = time(nullptr);
        time_t checkIn1 = now + 2 * 24 * 60 * 60;  // 2 days from now
        time_t checkOut1 = checkIn1 + 3 * 24 * 60 * 60;  // 3 nights
        
        makeReservation(1001, 101, checkIn1, checkOut1, 2, "Early check-in requested");
        makeReservation(1002, 102, checkIn1, checkOut1, 3, "Extra bed needed");
        
        // Add sample bills
        int bill1 = createBill(10001);
        addBillItem(bill1, "Room Charge", 100.0, 3);
        addBillItem(bill1, "Food - Breakfast", 15.0, 3);
        addBillItem(bill1, "Laundry Service", 25.0);
        
        int bill2 = createBill(10002, 0.10, 0.05); // 10% tax, 5% discount
        addBillItem(bill2, "Room Charge", 120.0, 3);
        addBillItem(bill2, "Food - Dinner", 30.0, 2);
        
        // Process one payment
        processPayment(bill1, "Credit Card");
        
        Utils::showMessage("Sample data initialized successfully", 's');
        
    } catch (const HotelException& e) {
        Utils::showMessage("Error initializing sample data: " + string(e.what()), 'e');
    }
}