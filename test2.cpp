#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>


using namespace std;

// Function to center-align text
void centerText(const string& text) {
    int width = 80;
    int len = text.length();
    int pad = (width - len) / 2;
    cout << string(pad, ' ') << text << endl;
}

// Function to print a decorative line
void printLine(char c = '-', int length = 80) {
    cout << string(length, c) << endl;
}


class User {
protected:
    string name;
    string email;
    string number;
    string address;

public:
    User() {}

    void inputDetails() {
        system("cls");
        printLine('=');
        centerText("USER DETAILS INPUT");
        printLine('=');
        centerText("Press Enter to continue...");
        cin.ignore();
        
        centerText("Enter Name: ");
        getline(cin, name);
        centerText("Enter Email: ");
        getline(cin, email);
        centerText("Enter Phone Number: ");
        getline(cin, number);
        centerText("Enter Address: ");
        getline(cin, address);
    }


    void displayDetails() {
        system("cls");
        printLine('=');
        centerText("USER DETAILS");
        printLine('=');
        centerText("Name: " + name);
        centerText("Email: " + email);
        centerText("Phone Number: " + number);
        centerText("Address: " + address);
        printLine('-');
    }

    string getName() {
        return name;
    }

    void saveToFile() {
        ofstream file("users.txt", ios::app);
        if (file.is_open()) {
            file << name << "," << email << "," << number << "," << address << endl;
            file.close();
            centerText("User details saved successfully!");
        } else {
            centerText("Error: Unable to save user details.");
        }
    }

	void updateFile(const string& oldName) {
        ifstream inFile("users.txt");
        ofstream outFile("users_temp.txt");
        string line;
        
        while (getline(inFile, line)) {
            istringstream iss(line);
            string userName;
            getline(iss, userName, ',');
            
            if (userName != oldName) {
                outFile << line << endl;
            }
        }
        
        outFile << name << "," << email << "," << number << "," << address << endl;
        
        inFile.close();
        outFile.close();
        
        remove("users.txt");
        rename("users_temp.txt", "users.txt");
        centerText("User details updated successfully!");
    }


    static bool isDuplicate(const string& name, User &sms) {
        ifstream file("users.txt");
        string line;
	    while (getline(file, line)) {
	        istringstream iss(line);
	        string userName;
	        getline(iss, userName, ',');

	        if (userName == name) {
	        	sms.name=userName;
	            getline(iss, sms.email, ',');
	            getline(iss, sms.number, ',');
	            getline(iss, sms.address);
	            file.close();
	            return true;
	        }
	    }
        return false;
    }
};

class Share {
private:
    string username;
    string companyName;
    int numberOfShares;
    double ltp;
    double rateOfChange;


public:
    Share() {}
    Share(const string& user) : username(user) {}


    void inputShareDetails() {
    	system("cls");
        printLine('=');
        centerText("SHARE DETAILS INPUT");
        printLine('=');
    	
        cout << "Enter Company Name: ";
        cin.ignore();
        getline(cin, companyName);
        cout << "Enter Number of Shares: ";
        cin >> numberOfShares;
        cout << "Enter Latest LTP: ";
        cin >> ltp;
        cout << "Enter Rate of Change: ";
        cin >> rateOfChange;
    }

    void displayShareDetails() const {
        printLine('-');
        centerText("Company Name: " + companyName);
        centerText("Number of Shares: " + to_string(numberOfShares));
        centerText("Latest LTP: " + to_string(ltp));
        centerText("Rate of Change: " + to_string(rateOfChange) + "%");
        printLine('-');
    }

    void saveShareToFile() {
        ofstream file(username + "_shares.txt", ios::app);
        if (file.is_open()) {
            file << companyName << "," << numberOfShares << "," << ltp << "," << rateOfChange << endl;
            file.close();
            centerText("Share details saved successfully!");
        } else {
            centerText("Error: Unable to save share details.");
        }
    }

    void updateShareDetails() {
        system("cls");
        printLine('=');
        centerText("UPDATE SHARE DETAILS");
        printLine('=');
        
        centerText("Current details:");
        displayShareDetails();
        
        cin.ignore();
        
        centerText("Enter new Company Name (or press Enter to keep current): ");
        string newCompanyName;
        getline(cin, newCompanyName);
        if (!newCompanyName.empty()) {
            companyName = newCompanyName;
        }
        
        centerText("Enter new Number of Shares: ");
        cin >> numberOfShares;
        
        centerText("Enter new Latest LTP: ");
        cin >> ltp;
        
        centerText("Enter new Rate of Change: ");
        cin >> rateOfChange;
        
    }

    void updateToFile() const {
        ofstream file(username + "_shares.txt", ios::app);
        if (file.is_open()) {
            file << companyName << "," << numberOfShares << "," << ltp << "," << rateOfChange << endl;
            file.close();
        }
    }

    static Share* loadSharesForUser(const string& username, int& count) {
        ifstream file(username + "_shares.txt");
        string line;
        count = 0;
        Share* shares = new Share[100]; // Assuming a maximum of 100 shares
        while (getline(file, line) && count < 100) {
            Share share(username);
            istringstream iss(line);
            getline(iss, share.companyName, ',');
            iss >> share.numberOfShares;
            iss.ignore();
            iss >> share.ltp;
            iss.ignore();
            iss >> share.rateOfChange;
            shares[count++] = share;
        }
        return shares;
    }

    string getCompanyName() const { return companyName; }
    int getNumberOfShares() const { return numberOfShares; }
    double getLTP() const { return ltp; }
    double getRateOfChange() const { return rateOfChange; }
    void setCompanyName(const string& name) { companyName = name; }
    void setNumberOfShares(int num) { numberOfShares = num; }
    void setLTP(double price) { ltp = price; }
    void setRateOfChange(double rate) { rateOfChange = rate; }

};

class ShareManagementSystem : public User {
private:
    static const int MAX_SHARES = 100;
    Share shares[MAX_SHARES];
    int shareCount;

public:
    ShareManagementSystem(){
        shareCount=0;
    }
    ShareManagementSystem(const string& username) : shareCount(0) {
        name = username;
        loadShares();
    }

    void loadShares() {
        Share* loadedShares;
        int loadedCount;
        loadedShares = Share::loadSharesForUser(name, loadedCount);
        for (int i = 0; i < loadedCount && i < MAX_SHARES; i++) {
            shares[i] = loadedShares[i];
        }
        shareCount = (loadedCount < MAX_SHARES) ? loadedCount : MAX_SHARES;
        delete[] loadedShares;
    }

    bool menu() {
        int choice;
        do {
            system("cls");
            printLine('=');
            centerText("SHARE MANAGEMENT SYSTEM");
            printLine('=');
            centerText("1. Add Share");
            centerText("2. Transfer Share");
            centerText("3. Update Profile");
            centerText("4. See All Shares");
            centerText("5. Update a Share");
            centerText("6. Logout");
            printLine('-');
            centerText("Enter your choice: ");
            cin >> choice;

            switch (choice) {
                case 1:
                    addShare();
                    break;
                case 2:
                    transferShare();
                    break;
                case 3:
                    updateProfile();
                    loadShares();
                    break;
                case 4:
                    displayAllShares();
                    break;
                case 5:
                    updateShareDetails();
                    break;
                case 6:
                    logout();
                    return false;
                default:
                    centerText("Invalid choice. Please try again.");
            }
            if (choice != 5 && choice != 6) {
                centerText("Press Enter to continue...");
                cin.ignore();
                cin.get();
            }
        } while (choice != 6);
        return true;
    }

    void logout() {
        centerText("Logging out...");
        shareCount = 0;
        centerText("Press Enter to return to main menu...");
        cin.ignore();
        cin.get();
    }

    void addShare() {
        Share share(name);
        share.inputShareDetails();
        shares[shareCount++] = share;
        share.saveShareToFile();
        
        system("cls");
        printLine('=');
        centerText("SHARE ADDED SUCCESSFULLY");
        printLine('=');
        share.displayShareDetails();
        
        return;
    }

    void updateProfile() {
        system("cls");
        printLine('*');
        centerText("UPDATE PROFILE");
        printLine('*');
        
        // Display current details
        centerText("Current Details:");
        User::displayDetails();
        printLine('-');

        string oldName = getName();
        string newName, newEmail, newNumber, newAddress;

        cin.ignore();

        centerText("Enter new Name (or press Enter to keep current): ");
        getline(cin, newName);
        if (newName.empty()) newName = name;

        centerText("Enter new Email (or press Enter to keep current): ");
        getline(cin, newEmail);
        if (newEmail.empty()) newEmail = email;

        centerText("Enter new Phone Number (or press Enter to keep current): ");
        getline(cin, newNumber);
        if (newNumber.empty()) newNumber = number;

        centerText("Enter new Address (or press Enter to keep current): ");
        getline(cin, newAddress);
        if (newAddress.empty()) newAddress = address;

        name = newName;
        email = newEmail;
        number = newNumber;
        address = newAddress;

        if (name != oldName && shareCount!=0) {
            renameSharesFile(oldName, newName);
        }
        updateFile(oldName);
    }

    void renameSharesFile(const string& oldName, const string& newName) {
        string oldFileName = oldName + "_shares.txt";
        string newFileName = newName + "_shares.txt";
        
        if (rename(oldFileName.c_str(), newFileName.c_str()) != 0) {
            centerText("Error renaming shares file.");
        } else {
            remove(oldFileName.c_str());
            centerText("Shares file renamed successfully.");
        }
    }

    void updateShareDetails() {
        if (shareCount == 0) {
            centerText("No shares to update.");
            cin.ignore();
            cin.get();
            return;
        }

        system("cls");
        printLine('=');
        centerText("UPDATE SHARE DETAILS");
        printLine('=');
        centerText("Select a share to update:");
        
        for (size_t i = 0; i < shareCount; ++i) {
            centerText(to_string(i + 1) + ".");
            shares[i].displayShareDetails();
        }

        size_t choice;
        centerText("Enter the number of the share to update: ");
        cin >> choice;

        if (choice > 0 && choice <= shareCount) {
            shares[choice - 1].updateShareDetails();
            saveAllSharesToFile();
            centerText("Share details updated successfully!");
            centerText("Press Enter to continue...");
            cin.ignore();
            cin.get();
            return;
        } else {
            centerText("Invalid choice.");
            centerText("Press Enter to continue...");
            cin.ignore();
            cin.get();
            return;
        }
    }

    void saveAllSharesToFile() {
        ofstream file(name + "_shares.txt");
        if (file.is_open()) {
            for (int i = 0; i < shareCount; i++) {
                file << shares[i].getCompanyName() << "," 
                     << shares[i].getNumberOfShares() << "," 
                     << shares[i].getLTP() << "," 
                     << shares[i].getRateOfChange() << endl;
            }
            file.close();
        } else {
            centerText("Error: Unable to save share details.");
        }
    }

    void displayAllShares() {
        system("cls");
        printLine('=');
        centerText("ALL SHARES FOR " + name);
        printLine('=');

        if (shareCount==0) {
            centerText("You don't have any shares yet.");
        } else {
            for (size_t i = 0; i < shareCount; ++i) {
                centerText("Share #" + to_string(i + 1));
                shares[i].displayShareDetails();
            }
        }
    }

    void transferShare() {
        if (shareCount==0) {
            centerText("You don't have any shares to transfer.");
            return;
        }
        system("cls");
        printLine('=');
        centerText("TRANSFER SHARE");
        printLine('=');
        centerText("Select a share to transfer: ");
        
        for (size_t i = 0; i < shareCount; ++i) {
            centerText(to_string(i + 1) + ".");
            shares[i].displayShareDetails();
        }

        size_t choice;
        centerText("Enter the S.N. of the share to transfer: ");
        cin >> choice;

        if (choice > 0 && choice <= shareCount) {
            Share& selectedShare = shares[choice - 1];
            
            string recipientName;
            int sharesToTransfer;

            cin.ignore(); 
            centerText("Enter the name of the user to transfer to: ");
            getline(cin, recipientName);

            ShareManagementSystem recipient(recipientName);
            if (!User::isDuplicate(recipientName, recipient)) {
                centerText("Recipient user not found.");
                return;
            }

            centerText("Enter the number of shares to transfer: ");
            cin >> sharesToTransfer;

            if (sharesToTransfer <= 0 || sharesToTransfer > selectedShare.getNumberOfShares()) {
                centerText("Invalid number of shares.");
                return;
            }

            Share transferredShare(recipientName);
            transferredShare.setCompanyName(selectedShare.getCompanyName());
            transferredShare.setNumberOfShares(sharesToTransfer);
            transferredShare.setLTP(selectedShare.getLTP());
            transferredShare.setRateOfChange(selectedShare.getRateOfChange());

            transferredShare.saveShareToFile();

            if (sharesToTransfer == selectedShare.getNumberOfShares()) {
                for (int i = choice - 1; i < shareCount - 1; i++) {
                    shares[i] = shares[i + 1];
                }
                shareCount--;
            } else {
                selectedShare.setNumberOfShares(selectedShare.getNumberOfShares() - sharesToTransfer);
            }

            saveAllSharesToFile();

            centerText("Shares transferred successfully!");
        } else {
            centerText("Invalid choice.");
        }
}

};

int main() {

    string name;
    int choice;

    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 800, 600, TRUE);

while (true) {
    system("cls");
    printLine('=');
    centerText("SHARE MANAGEMENT SYSTEM");
    printLine('=');
    centerText("Welcome to Share Management System!");
    printLine('-');
    printLine('=');
    centerText("1. Login");
    centerText("2. Register");
    centerText("3. Exit");
    printLine('-');

    centerText("Enter your choice: ");
    cin >> choice;
    
    if (choice == 1) {
        system("cls");
        printLine('=');
        printLine('-');
        centerText("LOGIN");
        printLine('-');
        centerText("Enter your name: ");
        cin.ignore();
        getline(cin, name);
        ShareManagementSystem sms(name);
        if (User::isDuplicate(name,sms)){
            centerText("User found, Logging in...");
            cin.get();
            sms.menu();
        }else{
            centerText("User not found, Returning to main menu..");
            cin.get();
            continue;
        }
    }else if(choice==2){
        ShareManagementSystem sms;
        printLine('-');
        centerText("REGISTER");
        printLine('-');
        cin.get();
        sms.inputDetails();
        sms.saveToFile();
        sms.loadShares();
        centerText("Registration Successful...");
        centerText("Press Enter to Continue...");
        cin.get();
        sms.menu();

    }else if(choice == 3){
        centerText("Thank you for using Share Management System. Goodbye!");
        break; 
    }else{
        centerText("Invalid choice. Please try again.");
        centerText("Press any key to continue...");
        cin.ignore();
        cin.get();
    }
}

    return 0;
}
