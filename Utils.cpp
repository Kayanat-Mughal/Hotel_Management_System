#include "Utils.h"
#include <sys/stat.h>
#include <cstdlib>

// ==================== INPUT FUNCTIONS ====================
int Utils::getIntegerInput(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << "Please enter a value between " << min << " and " << max << ".\n";
            }
        } else {
            cout << "Invalid input. Please enter a valid integer.\n";
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double Utils::getDoubleInput(const string& prompt, double min) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << "Please enter a value greater than or equal to " << min << ".\n";
            }
        } else {
            cout << "Invalid input. Please enter a valid number.\n";
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string Utils::getStringInput(const string& prompt, bool allowEmpty) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        
        if (!allowEmpty && input.empty()) {
            cout << "Input cannot be empty. Please try again.\n";
        } else {
            return input;
        }
    }
}

bool Utils::getYesNoInput(const string& prompt) {
    char choice;
    while (true) {
        cout << prompt << " (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        choice = tolower(choice);
        if (choice == 'y' || choice == 'n') {
            return (choice == 'y');
        }
        cout << "Please enter 'y' for yes or 'n' for no.\n";
    }
}

// ==================== DATE FUNCTIONS ====================
string Utils::getCurrentDate() {
    time_t now = time(nullptr);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return string(buffer);
}

string Utils::getCurrentDateTime() {
    time_t now = time(nullptr);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buffer);
}

time_t Utils::parseDate(const string& dateStr) {
    tm tm = {};
    stringstream ss(dateStr);
    string token;
    
    getline(ss, token, '-');
    tm.tm_year = stoi(token) - 1900;
    
    getline(ss, token, '-');
    tm.tm_mon = stoi(token) - 1;
    
    getline(ss, token, '-');
    tm.tm_mday = stoi(token);
    
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    
    return mktime(&tm);
}

string Utils::formatDate(time_t time) {
    tm* tm = localtime(&time);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return string(buffer);
}

bool Utils::isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[4] != '-' || dateStr[7] != '-') return false;
    
    try {
        int year = stoi(dateStr.substr(0, 4));
        int month = stoi(dateStr.substr(5, 2));
        int day = stoi(dateStr.substr(8, 2));
        
        if (year < 2023 || year > 2030) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}

int Utils::daysBetween(time_t start, time_t end) {
    return (end - start) / (60 * 60 * 24);
}

// ==================== STRING FUNCTIONS ====================
string Utils::toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

string Utils::toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string Utils::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

// ==================== VALIDATION FUNCTIONS ====================
bool Utils::isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos && dotPos > atPos + 1);
}

bool Utils::isValidPhone(const string& phone) {
    return phone.length() >= 10 && 
           all_of(phone.begin(), phone.end(), [](char c) {
               return isdigit(c) || c == '+' || c == '-' || c == ' ' || c == '(' || c == ')';
           });
}

// ==================== FILE FUNCTIONS ====================
bool Utils::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void Utils::createDirectory(const string& dirname) {
    #ifdef _WIN32
        system(("mkdir " + dirname).c_str());
    #else
        system(("mkdir -p " + dirname).c_str());
    #endif
}

// ==================== DISPLAY FUNCTIONS ====================
void Utils::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Utils::displayHeader(const string& title) {
    cout << "\n";
    cout << string(60, '=') << "\n";
    cout << centerText(title, 60) << "\n";
    cout << string(60, '=') << "\n";
}

void Utils::displayMenu(const vector<string>& options) {
    for (size_t i = 0; i < options.size(); i++) {
        cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
    cout << "  0. Back/Exit\n";
    cout << string(40, '-') << "\n";
}

void Utils::showMessage(const string& message, char type) {
    cout << "\n";
    switch (type) {
        case 's': // Success
            cout << "✅ SUCCESS: " << message << "\n";
            break;
        case 'e': // Error
            cout << "❌ ERROR: " << message << "\n";
            break;
        case 'w': // Warning
            cout << "⚠️ WARNING: " << message << "\n";
            break;
        default: // Info
            cout << "ℹ️ INFO: " << message << "\n";
    }
}

// ==================== FORMATTING FUNCTIONS ====================
string Utils::formatCurrency(double amount) {
    stringstream ss;
    ss << "$" << fixed << setprecision(2) << amount;
    return ss.str();
}

string Utils::centerText(const string& text, int width) {
    if (text.length() >= width) return text;
    
    int padding = (width - text.length()) / 2;
    return string(padding, ' ') + text + string(width - text.length() - padding, ' ');
}

// ==================== OTHER FUNCTIONS ====================
void Utils::pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int Utils::generateId() {
    static int counter = 1000;
    return counter++;
}

// ==================== CONVERSION FUNCTIONS ====================
string Utils::roomTypeToString(RoomType type) {
    switch(type) {
        case STANDARD: return "Standard";
        case DELUXE: return "Deluxe";
        case SUITE: return "Suite";
        case PRESIDENTIAL: return "Presidential";
        default: return "Unknown";
    }
}

string Utils::roomStatusToString(RoomStatus status) {
    switch(status) {
        case AVAILABLE: return "Available";
        case OCCUPIED: return "Occupied";
        case RESERVED: return "Reserved";
        case MAINTENANCE: return "Maintenance";
        default: return "Unknown";
    }
}

RoomType Utils::intToRoomType(int value) {
    switch(value) {
        case 1: return STANDARD;
        case 2: return DELUXE;
        case 3: return SUITE;
        case 4: return PRESIDENTIAL;
        default: return STANDARD;
    }
}