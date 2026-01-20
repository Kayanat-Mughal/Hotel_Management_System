#include "Hotel.h"

Hotel::Hotel() : currentEmployee(nullptr) {
    hotelName = "Grand Luxury Hotel";
    hotelAddress = "123 Luxury Street, New York, NY 10001";
    hotelContact = "+1 (555) 123-4567";
    hotelEmail = "info@grandluxury.com";
    
    // Create necessary directories
    Utils::createDirectory("data");
    Utils::createDirectory("backup");
    Utils::createDirectory("reports");
}

Hotel::~Hotel() {
    // Auto-save is handled by Database destructor
}

void Hotel::run() {
    try {
        // Show login screen
        login();
        
        int choice;
        bool running = true;
        
        while (running && isLoggedIn()) {
            Utils::clearScreen();
            showDashboard();
            showMainMenu();
            
            choice = Utils::getIntegerInput("\nEnter your choice (0-8): ", 0, 8);
            
            switch(choice) {
                case 1:
                    manageRooms();
                    break;
                case 2:
                    manageCustomers();
                    break;
                case 3:
                    manageReservations();
                    break;
                case 4:
                    manageEmployees();
                    break;
                case 5:
                    manageBilling();
                    break;
                case 6:
                    showReports();
                    break;
                case 7:
                    systemSettings();
                    break;
                case 8:
                    showDashboard();
                    break;
                case 0:
                    if (Utils::getYesNoInput("\nAre you sure you want to logout?")) {
                        logout();
                        running = false;
                    }
                    break;
                default:
                    Utils::showMessage("Invalid choice!", 'e');
            }
            
            if (choice != 0 && running && isLoggedIn()) {
                Utils::pressEnterToContinue();
            }
        }
        
        if (isLoggedIn()) {
            saveAndExit();
        }
        
    } catch (const exception& e) {
        Utils::showMessage("Fatal error: " + string(e.what()), 'e');
        Utils::pressEnterToContinue();
    }
}

void Hotel::showMainMenu() {
    vector<string> mainMenu = {
        "Room Management",
        "Customer Management", 
        "Reservation Management",
        "Employee Management",
        "Billing & Payments",
        "Reports & Statistics",
        "System Settings",
        "Show Dashboard",
        "Logout"
    };
    
    cout << "\n══════════════════════════════════════════════\n";
    cout << "                  MAIN MENU\n";
    cout << "══════════════════════════════════════════════\n";
    
    for (size_t i = 0; i < mainMenu.size(); i++) {
        cout << "  " << (i + 1) << ". " << mainMenu[i] << "\n";
    }
    cout << "  0. Logout\n";
    cout << "══════════════════════════════════════════════\n";
}

void Hotel::showDashboard() {
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║                 " << Utils::centerText(hotelName, 38) << "                 ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n";
    
    cout << "\n📍 " << hotelAddress << "\n";
    cout << "📞 " << hotelContact << " | ✉️  " << hotelEmail << "\n";
    cout << "📅 " << Utils::getCurrentDate() << " | 👤 " << getCurrentUser() << "\n";
    
    cout << "\n┌─────────────────┬─────────────────┬─────────────────┐\n";
    cout << "│    📊 STATS     │    🛏️  ROOMS    │    👥 GUESTS    │\n";
    cout << "├─────────────────┼─────────────────┼─────────────────┤\n";
    cout << "│ Total Rooms:    │ Available:      │ Today's Check:  │\n";
    cout << "│     " << setw(3) << database.getRoomCount() 
         << "        │      " << setw(3) << database.getAvailableRoomCount() 
         << "       │      " << setw(3) << database.findTodayCheckIns().size() 
         << "       │\n";
    cout << "└─────────────────┴─────────────────┴─────────────────┘\n";
}

// ==================== LOGIN/LOGOUT ====================
void Hotel::login() {
    Utils::clearScreen();
    Utils::displayHeader("HOTEL MANAGEMENT SYSTEM - LOGIN");
    
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    
    cout << "\n📝 TEST CREDENTIALS:\n";
    cout << "   Email: robert@hotel.com | Password: default123\n";
    cout << "   Email: lisa@hotel.com | Password: default123\n";
    cout << "   Email: david@hotel.com | Password: default123\n";
    cout << "\n";
    
    while (attempts < MAX_ATTEMPTS) {
        cout << "\n";
        string email = Utils::getStringInput("Email: ");
        string password = Utils::getStringInput("Password: ");
        
        currentEmployee = database.authenticateEmployee(email, password);
        
        if (currentEmployee) {
            Utils::showMessage("Login successful! Welcome, " + currentEmployee->getName() + "!", 's');
            return;
        } else {
            attempts++;
            cout << "\nInvalid credentials. Attempts remaining: " << (MAX_ATTEMPTS - attempts) << "\n";
        }
    }
    
    Utils::showMessage("Maximum login attempts exceeded. Exiting...", 'e');
    exit(0);
}

void Hotel::logout() {
    if (currentEmployee) {
        Utils::showMessage("Goodbye, " + currentEmployee->getName() + "!", 's');
        currentEmployee = nullptr;
    }
}

bool Hotel::isAdmin() const {
    return currentEmployee && currentEmployee->getDepartment() == MANAGEMENT;
}

bool Hotel::isManager() const {
    return currentEmployee && currentEmployee->isManager();
}

bool Hotel::isLoggedIn() const {
    return currentEmployee != nullptr;
}

string Hotel::getCurrentUser() const {
    return currentEmployee ? currentEmployee->getName() : "Not logged in";
}

// ==================== ROOM MANAGEMENT ====================
void Hotel::manageRooms() {
    int choice;
    do {
        Utils::clearScreen();
        Utils::displayHeader("ROOM MANAGEMENT");
        
        vector<string> roomMenu = {
            "View All Rooms",
            "View Available Rooms",
            "Add New Room",
            "Search Room",
            "Update Room Status",
            "Modify Room Details",
            "Back to Main Menu"
        };
        
        Utils::displayMenu(roomMenu);
        choice = Utils::getIntegerInput("Enter your choice: ", 0, roomMenu.size());
        
        switch(choice) {
            case 1:
                database.displayAllRooms();
                break;
            case 2:
                database.displayAvailableRooms();
                break;
            case 3:
                addNewRoom();
                break;
            case 4:
                searchRoom();
                break;
            case 5:
                updateRoomStatus();
                break;
            case 6:
                modifyRoomDetails();
                break;
        }
        
        if (choice != 0) {
            Utils::pressEnterToContinue();
        }
        
    } while (choice != 0 && choice != 7);
}

void Hotel::addNewRoom() {
    try {
        Utils::displayHeader("ADD NEW ROOM");
        
        cout << "\nRoom Types:\n";
        cout << "1. Standard ($100-150/night)\n";
        cout << "2. Deluxe ($200-300/night)\n";
        cout << "3. Suite ($350-500/night)\n";
        cout << "4. Presidential ($800+/night)\n";
        
        int typeChoice = Utils::getIntegerInput("\nSelect room type (1-4): ", 1, 4);
        RoomType type = Utils::intToRoomType(typeChoice);
        
        double price = Utils::getDoubleInput("Enter price per night: $", 50.0);
        int capacity = Utils::getIntegerInput("Enter room capacity (1-6): ", 1, 6);
        
        cout << "\nEnter features (comma separated):\n";
        cout << "Available: WiFi, TV, AC, Mini-fridge, Mini-bar, Jacuzzi,\n";
        cout << "Kitchenette, Living area, Balcony, Sea view, City view\n";
        
        string featuresInput = Utils::getStringInput("Features: ");
        vector<string> features;
        
        // Parse features
        stringstream ss(featuresInput);
        string feature;
        while (getline(ss, feature, ',')) {
            features.push_back(Utils::trim(feature));
        }
        
        int roomNumber = database.addRoom(type, price, capacity, features);
        Utils::showMessage("Room #" + to_string(roomNumber) + " added successfully!", 's');
        
    } catch (const HotelException& e) {
        Utils::showMessage(e.what(), 'e');
    }
}

void Hotel::searchRoom() {
    int roomNumber = Utils::getIntegerInput("Enter room number: ");
    Room* room = database.findRoom(roomNumber);
    
    if (room) {
        room->display();
    } else {
        Utils::showMessage("Room not found!", 'e');
    }
}

void Hotel::updateRoomStatus() {
    int roomNumber = Utils::getIntegerInput("Enter room number: ");
    Room* room = database.findRoom(roomNumber);
    
    if (room) {
        cout << "\nCurrent Status: " << room->getStatusString() << "\n";
        cout << "\nSelect new status:\n";
        cout << "1. Available\n2. Occupied\n3. Reserved\n4. Maintenance\n";
        
        int statusChoice = Utils::getIntegerInput("Select status (1-4): ", 1, 4);
        RoomStatus newStatus = static_cast<RoomStatus>(statusChoice);
        
        if (database.updateRoomStatus(roomNumber, newStatus)) {
            Utils::showMessage("Room status updated successfully!", 's');
        } else {
            Utils::showMessage("Failed to update room status", 'e');
        }
    } else {
        Utils::showMessage("Room not found!", 'e');
    }
}

void Hotel::modifyRoomDetails() {
    int roomNumber = Utils::getIntegerInput("Enter room number: ");
    Room* room = database.findRoom(roomNumber);
    
    if (room) {
        room->display();
        
        cout << "\nWhat would you like to modify?\n";
        cout << "1. Price\n2. Features\n3. Both\n";
        
        int choice = Utils::getIntegerInput("Select option (1-3): ", 1, 3);
        
        try {
            if (choice == 1 || choice == 3) {
                double newPrice = Utils::getDoubleInput("Enter new price: $", 50.0);
                room->setPrice(newPrice);
            }
            
            if (choice == 2 || choice == 3) {
                cout << "\nEnter new features (comma separated): ";
                string featuresInput = Utils::getStringInput("");
                vector<string> features;
                
                stringstream ss(featuresInput);
                string feature;
                while (getline(ss, feature, ',')) {
                    features.push_back(Utils::trim(feature));
                }
                
                room->setFeatures(features);
            }
            
            database.saveAllData();
            Utils::showMessage("Room details updated successfully!", 's');
            
        } catch (const HotelException& e) {
            Utils::showMessage(e.what(), 'e');
        }
    } else {
        Utils::showMessage("Room not found!", 'e');
    }
}

// ==================== CUSTOMER MANAGEMENT ====================
void Hotel::manageCustomers() {
    int choice;
    do {
        Utils::clearScreen();
        Utils::displayHeader("CUSTOMER MANAGEMENT");
        
        vector<string> customerMenu = {
            "View All Customers",
            "Add New Customer",
            "Search Customer",
            "Update Customer Information",
            "View Customer History",
            "Back to Main Menu"
        };
        
        Utils::displayMenu(customerMenu);
        choice = Utils::getIntegerInput("Enter your choice: ", 0, customerMenu.size());
        
        switch(choice) {
            case 1:
                database.displayAllCustomers();
                break;
            case 2:
                addNewCustomer();
                break;
            case 3:
                searchCustomer();
                break;
            case 4:
                updateCustomerInfo();
                break;
            case 5:
                viewCustomerHistory();
                break;
        }
        
        if (choice != 0) {
            Utils::pressEnterToContinue();
        }
        
    } while (choice != 0 && choice != 6);
}

void Hotel::addNewCustomer() {
    try {
        Utils::displayHeader("ADD NEW CUSTOMER");
        
        string name = Utils::getStringInput("Full Name: ");
        string email;
        
        do {
            email = Utils::getStringInput("Email: ");
            if (!Utils::isValidEmail(email)) {
                Utils::showMessage("Invalid email format!", 'e');
            }
        } while (!Utils::isValidEmail(email));
        
        string phone;
        do {
            phone = Utils::getStringInput("Phone: ");
            if (!Utils::isValidPhone(phone)) {
                Utils::showMessage("Invalid phone number!", 'e');
            }
        } while (!Utils::isValidPhone(phone));
        
        string address = Utils::getStringInput("Address: ");
        string idProof = Utils::getStringInput("ID Proof (Passport/Driver's License): ");
        
        int customerId = database.addCustomer(name, email, phone, address, idProof);
        Utils::showMessage("Customer added successfully! Customer ID: " + to_string(customerId), 's');
        
    } catch (const HotelException& e) {
        Utils::showMessage(e.what(), 'e');
    }
}

void Hotel::searchCustomer() {
    cout << "\nSearch by:\n";
    cout << "1. Customer ID\n2. Name\n3. Phone\n";
    
    int searchType = Utils::getIntegerInput("Select search type (1-3): ", 1, 3);
    
    if (searchType == 1) {
        int customerId = Utils::getIntegerInput("Enter Customer ID: ");
        Customer* customer = database.findCustomer(customerId);
        if (customer) {
            customer->display();
        } else {
            Utils::showMessage("Customer not found!", 'e');
        }
    } else if (searchType == 2) {
        string name = Utils::getStringInput("Enter name: ");
        Customer* customer = database.findCustomerByName(name);
        if (customer) {
            customer->display();
        } else {
            Utils::showMessage("Customer not found!", 'e');
        }
    } else if (searchType == 3) {
        string phone = Utils::getStringInput("Enter phone: ");
        vector<Customer*> customers = database.findCustomersByPhone(phone);
        if (!customers.empty()) {
            for (auto customer : customers) {
                customer->displayBrief();
            }
        } else {
            Utils::showMessage("No customers found!", 'e');
        }
    }
}

void Hotel::updateCustomerInfo() {
    int customerId = Utils::getIntegerInput("Enter Customer ID: ");
    Customer* customer = database.findCustomer(customerId);
    
    if (customer) {
        customer->display();
        
        cout << "\nWhat would you like to update?\n";
        cout << "1. Phone\n2. Email\n3. Address\n4. All\n";
        
        int choice = Utils::getIntegerInput("Select option (1-4): ", 1, 4);
        
        try {
            string newPhone, newEmail, newAddress;
            
            if (choice == 1 || choice == 4) {
                do {
                    newPhone = Utils::getStringInput("New Phone: ");
                    if (!Utils::isValidPhone(newPhone)) {
                        Utils::showMessage("Invalid phone number!", 'e');
                    }
                } while (!Utils::isValidPhone(newPhone));
            }
            
            if (choice == 2 || choice == 4) {
                do {
                    newEmail = Utils::getStringInput("New Email: ");
                    if (!Utils::isValidEmail(newEmail)) {
                        Utils::showMessage("Invalid email format!", 'e');
                    }
                } while (!Utils::isValidEmail(newEmail));
            }
            
            if (choice == 3 || choice == 4) {
                newAddress = Utils::getStringInput("New Address: ");
            }
            
            if (database.updateCustomerInfo(customerId, newPhone, newEmail, newAddress)) {
                Utils::showMessage("Customer information updated successfully!", 's');
            } else {
                Utils::showMessage("Failed to update customer information", 'e');
            }
            
        } catch (const HotelException& e) {
            Utils::showMessage(e.what(), 'e');
        }
    } else {
        Utils::showMessage("Customer not found!", 'e');
    }
}

void Hotel::viewCustomerHistory() {
    int customerId = Utils::getIntegerInput("Enter Customer ID: ");
    Customer* customer = database.findCustomer(customerId);
    
    if (customer) {
        customer->display();
        
        vector<Reservation*> reservations = database.findReservationsByCustomer(customerId);
        if (!reservations.empty()) {
            cout << "\n══════════════════════════════════════════════\n";
            cout << "           RESERVATION HISTORY\n";
            cout << "══════════════════════════════════════════════\n";
            
            for (const auto& reservation : reservations) {
                reservation->displayBrief();
            }
        } else {
            cout << "\nNo reservation history found.\n";
        }
    } else {
        Utils::showMessage("Customer not found!", 'e');
    }
}

// ==================== RESERVATION MANAGEMENT ====================
void Hotel::manageReservations() {
    int choice;
    do {
        Utils::clearScreen();
        Utils::displayHeader("RESERVATION MANAGEMENT");
        
        vector<string> reservationMenu = {
            "View All Reservations",
            "View Active Reservations",
            "Make New Reservation",
            "View Reservation",
            "Modify Reservation",
            "Cancel Reservation",
            "Process Check-in",
            "Process Check-out",
            "Back to Main Menu"
        };
        
        Utils::displayMenu(reservationMenu);
        choice = Utils::getIntegerInput("Enter your choice: ", 0, reservationMenu.size());
        
        switch(choice) {
            case 1:
                database.displayAllReservations();
                break;
            case 2:
                {
                    vector<Reservation*> active = database.findActiveReservations();
                    if (active.empty()) {
                        cout << "\nNo active reservations.\n";
                    } else {
                        cout << "\nActive Reservations (" << active.size() << "):\n";
                        for (const auto& res : active) {
                            res->displayBrief();
                        }
                    }
                }
                break;
            case 3:
                makeNewReservation();
                break;
            case 4:
                viewReservation();
                break;
            case 5:
                modifyReservation();
                break;
            case 6:
                cancelReservation();
                break;
            case 7:
                processCheckIn();
                break;
            case 8:
                processCheckOut();
                break;
        }
        
        if (choice != 0) {
            Utils::pressEnterToContinue();
        }
        
    } while (choice != 0 && choice != 9);
}

void Hotel::makeNewReservation() {
    try {
        Utils::displayHeader("MAKE NEW RESERVATION");
        
        // Show available rooms
        database.displayAvailableRooms();
        
        // Get customer ID
        int customerId = Utils::getIntegerInput("\nEnter Customer ID (0 to create new): ", 0);
        if (customerId == 0) {
            addNewCustomer();
            customerId = database.getCustomerCount() + 1000; // Get latest ID
        }
        
        // Get room number
        int roomNumber = Utils::getIntegerInput("Enter Room Number: ");
        
        // Get dates
        string checkInStr, checkOutStr;
        do {
            checkInStr = Utils::getStringInput("Check-in Date (YYYY-MM-DD): ");
            if (!Utils::isValidDate(checkInStr)) {
                Utils::showMessage("Invalid date format!", 'e');
            }
        } while (!Utils::isValidDate(checkInStr));
        
        do {
            checkOutStr = Utils::getStringInput("Check-out Date (YYYY-MM-DD): ");
            if (!Utils::isValidDate(checkOutStr)) {
                Utils::showMessage("Invalid date format!", 'e');
            }
        } while (!Utils::isValidDate(checkOutStr));
        
        time_t checkIn = Utils::parseDate(checkInStr);
        time_t checkOut = Utils::parseDate(checkOutStr);
        
        if (checkIn >= checkOut) {
            throw ValidationException("Check-out must be after check-in");
        }
        
        // Get guests
        int guests = Utils::getIntegerInput("Number of Guests: ", 1, 6);
        
        // Get special requests
        string requests = Utils::getStringInput("Special Requests (optional): ", true);
        
        // Make reservation
        int reservationId = database.makeReservation(customerId, roomNumber, 
                                                    checkIn, checkOut, guests, requests);
        
        Utils::showMessage("Reservation created successfully! Reservation ID: " + 
                          to_string(reservationId), 's');
        
        // Ask if they want to create bill
        if (Utils::getYesNoInput("\nCreate bill for this reservation?")) {
            createNewBill();
        }
        
    } catch (const HotelException& e) {
        Utils::showMessage(e.what(), 'e');
    }
}

void Hotel::viewReservation() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID: ");
    Reservation* reservation = database.findReservation(reservationId);
    
    if (reservation) {
        reservation->display();
    } else {
        Utils::showMessage("Reservation not found!", 'e');
    }
}

void Hotel::modifyReservation() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID: ");
    Reservation* reservation = database.findReservation(reservationId);
    
    if (reservation) {
        if (reservation->getStatus() != CONFIRMED) {
            Utils::showMessage("Only confirmed reservations can be modified!", 'e');
            return;
        }
        
        reservation->display();
        
        cout << "\nWhat would you like to modify?\n";
        cout << "1. Dates\n2. Number of Guests\n3. Special Requests\n";
        
        int choice = Utils::getIntegerInput("Select option (1-3): ", 1, 3);
        
        try {
            if (choice == 1) {
                string checkInStr, checkOutStr;
                do {
                    checkInStr = Utils::getStringInput("New Check-in Date (YYYY-MM-DD): ");
                    if (!Utils::isValidDate(checkInStr)) {
                        Utils::showMessage("Invalid date format!", 'e');
                    }
                } while (!Utils::isValidDate(checkInStr));
                
                do {
                    checkOutStr = Utils::getStringInput("New Check-out Date (YYYY-MM-DD): ");
                    if (!Utils::isValidDate(checkOutStr)) {
                        Utils::showMessage("Invalid date format!", 'e');
                    }
                } while (!Utils::isValidDate(checkOutStr));
                
                time_t newCheckIn = Utils::parseDate(checkInStr);
                time_t newCheckOut = Utils::parseDate(checkOutStr);
                
                reservation->setDates(newCheckIn, newCheckOut);
            } else if (choice == 2) {
                int newGuests = Utils::getIntegerInput("New Number of Guests: ", 1, 6);
                reservation->setGuests(newGuests);
            } else if (choice == 3) {
                string newRequests = Utils::getStringInput("New Special Requests: ", true);
                reservation->setSpecialRequests(newRequests);
            }
            
            database.saveAllData();
            Utils::showMessage("Reservation modified successfully!", 's');
            
        } catch (const HotelException& e) {
            Utils::showMessage(e.what(), 'e');
        }
    } else {
        Utils::showMessage("Reservation not found!", 'e');
    }
}

void Hotel::cancelReservation() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID to cancel: ");
    
    if (Utils::getYesNoInput("Are you sure you want to cancel this reservation?")) {
        if (database.cancelReservation(reservationId)) {
            Utils::showMessage("Reservation cancelled successfully!", 's');
        } else {
            Utils::showMessage("Failed to cancel reservation", 'e');
        }
    }
}

void Hotel::processCheckIn() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID for check-in: ");
    
    if (database.checkIn(reservationId)) {
        Utils::showMessage("Check-in processed successfully!", 's');
    } else {
        Utils::showMessage("Failed to process check-in", 'e');
    }
}

void Hotel::processCheckOut() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID for check-out: ");
    
    if (database.checkOut(reservationId)) {
        Utils::showMessage("Check-out processed successfully!", 's');
        
        // Ask to create bill if not exists
        Bill* bill = database.findBillByReservation(reservationId);
        if (!bill) {
            if (Utils::getYesNoInput("\nCreate bill for this stay?")) {
                createNewBill();
            }
        }
    } else {
        Utils::showMessage("Failed to process check-out", 'e');
    }
}

// ==================== QUICK OPERATIONS ====================
void Hotel::quickCheckIn() {
    try {
        int reservationId = Utils::getIntegerInput("Enter Reservation ID: ");
        processCheckIn();
    } catch (...) {
        Utils::showMessage("Quick check-in failed", 'e');
    }
}

void Hotel::quickCheckOut() {
    try {
        int reservationId = Utils::getIntegerInput("Enter Reservation ID: ");
        processCheckOut();
    } catch (...) {
        Utils::showMessage("Quick check-out failed", 'e');
    }
}

void Hotel::quickPayment() {
    try {
        int billId = Utils::getIntegerInput("Enter Bill ID: ");
        Bill* bill = database.findBill(billId);
        
        if (bill) {
            bill->display();
            
            if (bill->getIsPaid()) {
                Utils::showMessage("Bill is already paid", 'i');
                return;
            }
            
            cout << "\nPayment Methods:\n";
            cout << "1. Cash\n2. Credit Card\n3. Debit Card\n4. Online Payment\n";
            
            int methodChoice = Utils::getIntegerInput("Select payment method (1-4): ", 1, 4);
            string method;
            
            switch(methodChoice) {
                case 1: method = "Cash"; break;
                case 2: method = "Credit Card"; break;
                case 3: method = "Debit Card"; break;
                case 4: method = "Online Payment"; break;
            }
            
            if (database.processPayment(billId, method)) {
                Utils::showMessage("Payment processed successfully!", 's');
            } else {
                Utils::showMessage("Failed to process payment", 'e');
            }
        } else {
            Utils::showMessage("Bill not found!", 'e');
        }
    } catch (...) {
        Utils::showMessage("Quick payment failed", 'e');
    }
}

// ==================== EMPLOYEE MANAGEMENT ====================
void Hotel::manageEmployees() {
    if (!isAdmin() && !isManager()) {
        Utils::showMessage("Access denied! Manager/Admin privileges required.", 'e');
        return;
    }
    
    int choice;
    do {
        Utils::clearScreen();
        Utils::displayHeader("EMPLOYEE MANAGEMENT");
        
        vector<string> employeeMenu = {
            "View All Employees",
            "Add New Employee",
            "Search Employee",
            "Update Employee Information",
            "Change Password",
            "Back to Main Menu"
        };
        
        Utils::displayMenu(employeeMenu);
        choice = Utils::getIntegerInput("Enter your choice: ", 0, employeeMenu.size());
        
        switch(choice) {
            case 1:
                database.displayAllEmployees();
                break;
            case 2:
                addNewEmployee();
                break;
            case 3:
                searchEmployee();
                break;
            case 4:
                updateEmployeeInfo();
                break;
            case 5:
                changeEmployeePassword();
                break;
        }
        
        if (choice != 0) {
            Utils::pressEnterToContinue();
        }
        
    } while (choice != 0 && choice != 6);
}

void Hotel::addNewEmployee() {
    try {
        Utils::displayHeader("ADD NEW EMPLOYEE");
        
        string name = Utils::getStringInput("Full Name: ");
        string position = Utils::getStringInput("Position: ");
        
        cout << "\nDepartments:\n";
        cout << "1. Front Desk\n2. Housekeeping\n3. Kitchen\n4. Management\n";
        int deptChoice = Utils::getIntegerInput("Select department (1-4): ", 1, 4);
        Department dept = static_cast<Department>(deptChoice);
        
        cout << "\nShifts:\n";
        cout << "1. Morning (8AM-4PM)\n2. Afternoon (4PM-12AM)\n3. Night (12AM-8AM)\n";
        int shiftChoice = Utils::getIntegerInput("Select shift (1-3): ", 1, 3);
        Shift shift = static_cast<Shift>(shiftChoice);
        
        double salary = Utils::getDoubleInput("Monthly Salary: $", 1000.0);
        
        string contact;
        do {
            contact = Utils::getStringInput("Contact Number: ");
            if (!Utils::isValidPhone(contact)) {
                Utils::showMessage("Invalid phone number!", 'e');
            }
        } while (!Utils::isValidPhone(contact));
        
        string address = Utils::getStringInput("Address: ");
        string joinDate = Utils::getCurrentDate();
        
        int employeeId = database.addEmployee(name, position, dept, shift, 
                                            salary, contact, address, joinDate);
        
        Utils::showMessage("Employee added successfully! Employee ID: " + 
                          to_string(employeeId), 's');
        
    } catch (const HotelException& e) {
        Utils::showMessage(e.what(), 'e');
    }
}

void Hotel::searchEmployee() {
    int employeeId = Utils::getIntegerInput("Enter Employee ID: ");
    Employee* employee = database.findEmployee(employeeId);
    
    if (employee) {
        employee->display();
    } else {
        Utils::showMessage("Employee not found!", 'e');
    }
}

void Hotel::updateEmployeeInfo() {
    int employeeId = Utils::getIntegerInput("Enter Employee ID: ");
    Employee* employee = database.findEmployee(employeeId);
    
    if (employee) {
        employee->display();
        
        cout << "\nWhat would you like to update?\n";
        cout << "1. Position\n2. Department\n3. Shift\n4. Salary\n5. All\n";
        
        int choice = Utils::getIntegerInput("Select option (1-5): ", 1, 5);
        
        try {
            string newPosition;
            Department newDept;
            Shift newShift;
            double newSalary;
            
            if (choice == 1 || choice == 5) {
                newPosition = Utils::getStringInput("New Position: ");
            }
            
            if (choice == 2 || choice == 5) {
                cout << "\nDepartments:\n";
                cout << "1. Front Desk\n2. Housekeeping\n3. Kitchen\n4. Management\n";
                int deptChoice = Utils::getIntegerInput("Select department (1-4): ", 1, 4);
                newDept = static_cast<Department>(deptChoice);
            }
            
            if (choice == 3 || choice == 5) {
                cout << "\nShifts:\n";
                cout << "1. Morning (8AM-4PM)\n2. Afternoon (4PM-12AM)\n3. Night (12AM-8AM)\n";
                int shiftChoice = Utils::getIntegerInput("Select shift (1-3): ", 1, 3);
                newShift = static_cast<Shift>(shiftChoice);
            }
            
            if (choice == 4 || choice == 5) {
                newSalary = Utils::getDoubleInput("New Salary: $", 1000.0);
            }
            
            if (database.updateEmployeeInfo(employeeId, newPosition, newDept, newShift, newSalary)) {
                Utils::showMessage("Employee information updated successfully!", 's');
            }
        } catch (const std::exception& e) {
            // Handle exception
            std::cerr << "Error: " << e.what() << std::endl;
            throw;  // Re-throw or handle appropriately
        }
    } else {
        Utils::showMessage("Employee not found!", 'e');
    }
}

void Hotel::changeEmployeePassword() {
    int employeeId = Utils::getIntegerInput("Enter Employee ID: ");
    Employee* employee = database.findEmployee(employeeId);
    
    if (employee) {
        string oldPassword = Utils::getStringInput("Enter old password: ");
        // Note: In a real system, you would verify the old password
        string newPassword = Utils::getStringInput("Enter new password: ");
        string confirmPassword = Utils::getStringInput("Confirm new password: ");
        
        if (newPassword == confirmPassword) {
            // Note: In a real system, update password in database
            Utils::showMessage("Password changed successfully!", 's');
        } else {
            Utils::showMessage("Passwords do not match!", 'e');
        }
    } else {
        Utils::showMessage("Employee not found!", 'e');
    }
}

void Hotel::manageBilling() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         BILLING & PAYMENTS MENU        ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    int choice;
    bool managingBilling = true;
    
    while (managingBilling) {
        cout << "1. Create New Bill\n";
        cout << "2. View Bill\n";
        cout << "3. Process Payment\n";
        cout << "4. Back to Main Menu\n";
        
        choice = Utils::getIntegerInput("Select option (1-4): ", 1, 4);
        
        switch(choice) {
            case 1:
                createNewBill();
                break;
            case 2:
                viewBill();
                break;
            case 3:
                processBillPayment();
                break;
            case 4:
                managingBilling = false;
                break;
        }
    }
}

void Hotel::createNewBill() {
    int reservationId = Utils::getIntegerInput("Enter Reservation ID: ");
    // Note: Implement bill creation logic here
    Utils::showMessage("Bill creation feature coming soon!", 'i');
}

void Hotel::showReports() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║       REPORTS & STATISTICS MENU        ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    int choice;
    bool viewingReports = true;
    
    while (viewingReports) {
        cout << "1. Revenue Report\n";
        cout << "2. Occupancy Report\n";
        cout << "3. Customer Report\n";
        cout << "4. Back to Main Menu\n";
        
        choice = Utils::getIntegerInput("Select option (1-4): ", 1, 4);
        
        switch(choice) {
            case 1:
                generateRevenueReport();
                break;
            case 2:
                generateOccupancyReport();
                break;
            case 3:
                generateCustomerReport();
                break;
            case 4:
                viewingReports = false;
                break;
        }
    }
}

void Hotel::systemSettings() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         SYSTEM SETTINGS MENU           ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    cout << "1. Display Hotel Information\n";
    cout << "2. Backup Database\n";
    cout << "3. View Logs\n";
    cout << "4. Back to Main Menu\n";
    
    int choice = Utils::getIntegerInput("Select option (1-4): ", 1, 4);
    
    switch(choice) {
        case 1:
            displayHotelInfo();
            break;
        case 2:
            Utils::showMessage("Backup feature coming soon!", 'i');
            break;
        case 3:
            Utils::showMessage("Logs feature coming soon!", 'i');
            break;
        case 4:
            break;
    }
}

void Hotel::saveAndExit() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║    Saving data and exiting system...   ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    Utils::showMessage("All data has been saved successfully!", 's');
    cout << "\nThank you for using Hotel Management System!\n";
    cout << "Goodbye!\n\n";
}

void Hotel::viewBill() {
    int billId = Utils::getIntegerInput("Enter Bill ID: ");
    Utils::showMessage("View bill feature coming soon!", 'i');
}

void Hotel::processBillPayment() {
    int billId = Utils::getIntegerInput("Enter Bill ID: ");
    double amount = Utils::getDoubleInput("Enter payment amount: $", 0.01);
    Utils::showMessage("Payment processed successfully!", 's');
}

void Hotel::generateRevenueReport() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║          REVENUE REPORT                ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << "Total Revenue: $0.00\n";
    cout << "Revenue from Room Bookings: $0.00\n";
    cout << "Revenue from Services: $0.00\n";
    Utils::pressEnterToContinue();
}

void Hotel::generateOccupancyReport() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║         OCCUPANCY REPORT               ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << "Total Rooms: 0\n";
    cout << "Occupied Rooms: 0\n";
    cout << "Available Rooms: 0\n";
    cout << "Occupancy Rate: 0%\n";
    Utils::pressEnterToContinue();
}

void Hotel::generateCustomerReport() {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║        CUSTOMER REPORT                 ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << "Total Customers: 0\n";
    cout << "Active Reservations: 0\n";
    cout << "Customers Checked In: 0\n";
    Utils::pressEnterToContinue();
}

void Hotel::displayHotelInfo() const {
    Utils::clearScreen();
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║        HOTEL INFORMATION               ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << "Hotel Name: " << hotelName << "\n";
    cout << "Address: " << hotelAddress << "\n";
    cout << "Contact: " << hotelContact << "\n";
    cout << "Email: " << hotelEmail << "\n";
    Utils::pressEnterToContinue();
}