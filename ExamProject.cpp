
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

class User {
protected:
    string name;
    string phone;
    string password;
    string birthDate;

public:
    User() : name(""), phone(""), password(""), birthDate("") {}
    User(string name, string phone, string password, string birthDate)
        : name(name), phone(phone), password(password), birthDate(birthDate) {
    }

    User(const User& other)
        : name(other.name), phone(other.phone), password(other.password), birthDate(other.birthDate) {
    }

    User(User&& other) noexcept
        : name(move(other.name)), phone(move(other.phone)), password(move(other.password)), birthDate(move(other.birthDate)) {
    }

    User& operator=(const User& other) {
        if (this != &other) {
            name = other.name;
            phone = other.phone;
            password = other.password;
            birthDate = other.birthDate;
        }
        return *this;
    }

    virtual ~User() {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getPassword() const { return password; }
    string getBirthDate() const { return birthDate; }

    void setName(string n) { name = n; }
    void setPhone(string p) { phone = p; }
    void setPassword(string pw) { password = pw; }
    void setBirthDate(string bd) { birthDate = bd; }

    static bool isValidPhone(const string& phone) {
        return phone.rfind("+994", 0) == 0 && phone.length() == 13;
    }

    static bool isAdult(const string& birthDate) {
        int d, m, y;
        char dot;
        stringstream ss(birthDate);
        ss >> d >> dot >> m >> dot >> y;

        if (y < 2007) return true;
        if (y == 2007 && m < 5) return true;
        if (y == 2007 && m == 5 && d <= 1) return true;
        return false;
    }

    static bool isValidDate(const string& birthDate) {
        int d, m, y;
        char dot;
        stringstream ss(birthDate);
        ss >> d >> dot >> m >> dot >> y;

        if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900) return false;
        return true;
    }
};

class Client : public User {
private:
    double rating;

public:
    Client() : User(), rating(5.0) {}
    Client(string name, string phone, string password, string birthDate, double rating = 5.0)
        : User(name, phone, password, birthDate), rating(rating) {
    }

    Client(const Client& other) : User(other), rating(other.rating) {}
    Client(Client&& other) noexcept : User(move(other)), rating(other.rating) {}

    Client& operator=(const Client& other) {
        if (this != &other) {
            User::operator=(other);
            rating = other.rating;
        }
        return *this;
    }

    ~Client() {}

    double getRating() const { return rating; }
    void setRating(double r) { rating = r; }

    void updateRating(double newRating) {
        rating = newRating;
    }
};

class Driver : public User {
private:
    string licenseNumber;
    double rating;

public:
    Driver() : User(), licenseNumber(""), rating(5.0) {}
    Driver(string name, string phone, string password, string birthDate, string licenseNumber, double rating = 5.0)
        : User(name, phone, password, birthDate), licenseNumber(licenseNumber), rating(rating) {
    }

    Driver(const Driver& other)
        : User(other), licenseNumber(other.licenseNumber), rating(other.rating) {
    }

    Driver(Driver&& other) noexcept
        : User(move(other)), licenseNumber(move(other.licenseNumber)), rating(other.rating) {
    }

    Driver& operator=(const Driver& other) {
        if (this != &other) {
            User::operator=(other);
            licenseNumber = other.licenseNumber;
            rating = other.rating;
        }
        return *this;

    }

    ~Driver() {}

    string getLicenseNumber() const { return licenseNumber; }
    double getRating() const { return rating; }

    void setLicenseNumber(string ln) { licenseNumber = ln; }
    void setRating(double r) { rating = r; }

    void updateRating(double newRating) {
        rating = newRating;
    }
};

vector<Client> clients;
vector<Driver> drivers;

void saveClientsToFile() {
    ofstream outFile("clients.txt");
    for (const auto& client : clients) {
        outFile << client.getName() << "," << client.getPhone() << "," << client.getPassword() << ","
            << client.getBirthDate() << "," << client.getRating() << endl;
    }
    outFile.close();
}

void saveDriversToFile() {
    ofstream outFile("drivers.txt");
    for (const auto& driver : drivers) {
        outFile << driver.getName() << "," << driver.getPhone() << "," << driver.getPassword() << ","
            << driver.getBirthDate() << "," << driver.getLicenseNumber() << "," << driver.getRating() << endl;
    }
    outFile.close();
}

void loadClientsFromFile() {
    ifstream inFile("clients.txt");
    string name, phone, password, birthDate;
    double rating;

    while (getline(inFile, name, ',')) {
        getline(inFile, phone, ',');
        getline(inFile, password, ',');
        getline(inFile, birthDate, ',');
        inFile >> rating;
        inFile.ignore();
        clients.push_back(Client(name, phone, password, birthDate, rating));
    }
    inFile.close();
}

void loadDriversFromFile() {
    ifstream inFile("drivers.txt");
    string name, phone, password, birthDate, licenseNumber;
    double rating;

    while (getline(inFile, name, ',')) {
        getline(inFile, phone, ',');
        getline(inFile, password, ',');
        getline(inFile, birthDate, ',');
        getline(inFile, licenseNumber, ',');
        inFile >> rating;
        inFile.ignore();
        drivers.push_back(Driver(name, phone, password, birthDate, licenseNumber, rating));
    }
    inFile.close();
}

// Регистрация клиента
void registerClient() {
    string name, phone, password, birthDate;

    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter phone number (+994...): ";
    getline(cin, phone);

    if (!User::isValidPhone(phone)) {
        cout << "Invalid phone number. It must start with +994 and have 13 digits.\n";
        return;
    }

    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter birthdate (DD.MM.YYYY): ";
    getline(cin, birthDate);

    if (!User::isValidDate(birthDate) || !User::isAdult(birthDate)) {
        cout << "Invalid birthdate or age under 18.\n";
        return;
    }

    clients.push_back(Client(name, phone, password, birthDate));
    cout << "Client registered successfully!\n";
}

// Регистрация водителя
void registerDriver() {
    string name, phone, password, birthDate, licenseNumber;

    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter phone number (+994...): ";
    getline(cin, phone);

    if (!User::isValidPhone(phone)) {
        cout << "Invalid phone number. It must start with +994 and have 13 digits.\n";
        return;
    }

    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter birthdate (DD.MM.YYYY): ";
    getline(cin, birthDate);

    if (!User::isValidDate(birthDate) || !User::isAdult(birthDate)) {
        cout << "Invalid birthdate or age under 18.\n";
        return;
    }

    cout << "Enter license number: ";
    getline(cin, licenseNumber);

    drivers.push_back(Driver(name, phone, password, birthDate, licenseNumber));
    cout << "Driver registered successfully!\n";
}

// Вход клиента
Client* loginClient() {
    string phone, password;
    cout << "Enter phone number: ";
    getline(cin, phone);
    cout << "Enter password: ";
    getline(cin, password);

    for (auto& client : clients) {
        if (client.getPhone() == phone && client.getPassword() == password) {
            cout << "Login successful!\n";
            return &client;
        }
    }

    cout << "Invalid phone or password.\n";
    return nullptr;
}

// Вход водителя
Driver* loginDriver() {
    string phone, password;
    cout << "Enter phone number: ";
    getline(cin, phone);
    cout << "Enter password: ";
    getline(cin, password);

    for (auto& driver : drivers) {
        if (driver.getPhone() == phone && driver.getPassword() == password) {
            cout << "Login successful!\n";
            return &driver;
        }
    }

    cout << "Invalid phone or password.\n";
    return nullptr;
}

void displayMenu() {
    int choice;
    while (true) {
        cout << "\n1. Registerыы as Client\n";
        cout << "2. Register as Driver\n";
        cout << "3. Login as Client\n";
        cout << "4. Login as Driver\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // To ignore the newline character after input

        switch (choice) {
        case 1:
            registerClient();
            break;
        case 2:
            registerDriver();
            break;
        case 3:
            loginClient();
            break;
        case 4:
            loginDriver();
            break;
        case 5:
            saveClientsToFile();
            saveDriversToFile();
            cout << "Exiting...\n";
            return;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }
}

int main() {
    loadClientsFromFile();
    loadDriversFromFile();
    displayMenu();
    return 0;
}

