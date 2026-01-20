#ifndef ROOM_H
#define ROOM_H

#include "Utils.h"
#include <vector>

class Room {
private:
    int roomNumber;
    RoomType type;
    RoomStatus status;
    double pricePerNight;
    int capacity;
    vector<string> features;
    
public:
    // Constructors
    Room();
    Room(int roomNo, RoomType t, double price, int cap, const vector<string>& feat = {});
    
    // Getters
    int getRoomNumber() const;
    RoomType getType() const;
    RoomStatus getStatus() const;
    double getPricePerNight() const;
    int getCapacity() const;
    vector<string> getFeatures() const;
    string getFeaturesString() const;
    string getTypeString() const;
    string getStatusString() const;
    
    // Setters
    void setStatus(RoomStatus newStatus);
    void setPrice(double newPrice);
    void addFeature(const string& feature);
    void setFeatures(const vector<string>& newFeatures);
    
    // Operations
    bool isAvailable() const;
    bool canAccommodate(int guests) const;
    double calculateStayCost(int nights) const;
    
    // Display
    void display() const;
    void displayBrief() const;
    
    // File operations
    void saveToFile(ofstream& file) const;
    void loadFromFile(ifstream& file);
    
    // Validation
    bool isValid() const;
};

#endif // ROOM_H