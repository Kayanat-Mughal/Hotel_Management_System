#include "Room.h"

// ==================== CONSTRUCTORS ====================
Room::Room() : roomNumber(0), type(STANDARD), status(AVAILABLE), 
               pricePerNight(0.0), capacity(1) {}

Room::Room(int roomNo, RoomType t, double price, int cap, const vector<string>& feat)
    : roomNumber(roomNo), type(t), status(AVAILABLE), 
      pricePerNight(price), capacity(cap), features(feat) {
    if (roomNo <= 0) throw ValidationException("Room number must be positive");
    if (price <= 0) throw ValidationException("Price must be positive");
    if (cap <= 0) throw ValidationException("Capacity must be positive");
}

// ==================== GETTERS ====================
int Room::getRoomNumber() const { return roomNumber; }
RoomType Room::getType() const { return type; }
RoomStatus Room::getStatus() const { return status; }
double Room::getPricePerNight() const { return pricePerNight; }
int Room::getCapacity() const { return capacity; }
vector<string> Room::getFeatures() const { return features; }

string Room::getFeaturesString() const {
    string result;
    for (size_t i = 0; i < features.size(); i++) {
        result += features[i];
        if (i != features.size() - 1) result += ", ";
    }
    return result.empty() ? "None" : result;
}

string Room::getTypeString() const {
    return Utils::roomTypeToString(type);
}

string Room::getStatusString() const {
    return Utils::roomStatusToString(status);
}

// ==================== SETTERS ====================
void Room::setStatus(RoomStatus newStatus) {
    status = newStatus;
}

void Room::setPrice(double newPrice) {
    if (newPrice <= 0) throw ValidationException("Price must be positive");
    pricePerNight = newPrice;
}

void Room::addFeature(const string& feature) {
    if (!feature.empty()) {
        features.push_back(feature);
    }
}

void Room::setFeatures(const vector<string>& newFeatures) {
    features = newFeatures;
}

// ==================== OPERATIONS ====================
bool Room::isAvailable() const {
    return status == AVAILABLE;
}

bool Room::canAccommodate(int guests) const {
    return guests <= capacity;
}

double Room::calculateStayCost(int nights) const {
    if (nights <= 0) throw ValidationException("Nights must be positive");
    return pricePerNight * nights;
}

// ==================== DISPLAY ====================
void Room::display() const {
    cout << "\n";
    cout << "┌─────────────────────────────────────────────┐\n";
    cout << "│               ROOM INFORMATION              │\n";
    cout << "├─────────────────────────────────────────────┤\n";
    cout << "│ Room Number: " << setw(30) << roomNumber << " │\n";
    cout << "│ Type:        " << setw(30) << getTypeString() << " │\n";
    cout << "│ Status:      " << setw(30) << getStatusString() << " │\n";
    cout << "│ Price/Night: " << setw(30) << Utils::formatCurrency(pricePerNight) << " │\n";
    cout << "│ Capacity:    " << setw(30) << capacity << " persons │\n";
    cout << "│ Features:    " << setw(30) << getFeaturesString() << " │\n";
    cout << "└─────────────────────────────────────────────┘\n";
}

void Room::displayBrief() const {
    cout << "Room #" << setw(4) << roomNumber 
         << " | " << setw(12) << left << getTypeString()
         << " | " << setw(12) << left << getStatusString()
         << " | " << setw(10) << left << Utils::formatCurrency(pricePerNight)
         << " | Capacity: " << capacity << "\n";
}

// ==================== FILE OPERATIONS ====================
void Room::saveToFile(ofstream& file) const {
    if (!file.is_open()) throw FileException("File is not open for writing");
    
    file << roomNumber << " "
         << static_cast<int>(type) << " "
         << static_cast<int>(status) << " "
         << pricePerNight << " "
         << capacity << " "
         << features.size() << " ";
    
    for (const auto& feature : features) {
        file << feature << " ";
    }
    file << "\n";
}

void Room::loadFromFile(ifstream& file) {
    if (!file.is_open()) throw FileException("File is not open for reading");
    
    int typeInt, statusInt, featureCount;
    file >> roomNumber >> typeInt >> statusInt >> pricePerNight >> capacity >> featureCount;
    
    type = static_cast<RoomType>(typeInt);
    status = static_cast<RoomStatus>(statusInt);
    
    features.clear();
    for (int i = 0; i < featureCount; i++) {
        string feature;
        file >> feature;
        features.push_back(feature);
    }
}

// ==================== VALIDATION ====================
bool Room::isValid() const {
    return roomNumber > 0 && pricePerNight > 0 && capacity > 0;
}