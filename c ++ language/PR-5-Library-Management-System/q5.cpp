#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <ctime>
using namespace std;

// ============================================================
// UTILITY: Get current date string
// ============================================================
string getCurrentDate() {
    time_t now = time(0);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return string(buf);
}

string addDays(int days) {
    time_t now = time(0);
    now += (time_t)(days * 24 * 3600);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return string(buf);
}

// ============================================================
// PATRON CLASS (Encapsulation)
// ============================================================
class Patron {
private:
    string patronID;
    string name;
    string email;
    vector<string> borrowedItemIDs;

public:
    Patron(string id, string n, string e)
        : patronID(id), name(n), email(e) {}

    string getPatronID() const { return patronID; }
    string getName()     const { return name; }
    string getEmail()    const { return email; }

    void setName(string n)  { name = n; }
    void setEmail(string e) { email = e; }

    void borrowItem(const string& itemID) { borrowedItemIDs.push_back(itemID); }
    void returnItem(const string& itemID) {
        auto it = find(borrowedItemIDs.begin(), borrowedItemIDs.end(), itemID);
        if (it != borrowedItemIDs.end()) borrowedItemIDs.erase(it);
    }

    bool hasBorrowed(const string& itemID) const {
        return find(borrowedItemIDs.begin(), borrowedItemIDs.end(), itemID) != borrowedItemIDs.end();
    }

    void display() const {
        cout << "  Patron ID   : " << patronID << endl;
        cout << "  Name        : " << name << endl;
        cout << "  Email       : " << email << endl;
        cout << "  Items Held  : " << borrowedItemIDs.size() << endl;
        if (!borrowedItemIDs.empty()) {
            cout << "  Item IDs    : ";
            for (const auto& id : borrowedItemIDs) cout << id << " ";
            cout << endl;
        }
    }
};

// ============================================================
// ABSTRACT BASE CLASS: LibraryItem  (Abstraction + Encapsulation)
// ============================================================
class LibraryItem {
private:
    string itemID;
    string title;
    string author;
    string dueDate;

protected:
    bool checkedOut;
    string checkedOutBy; // patron ID

public:
    LibraryItem(string id, string t, string a)
        : itemID(id), title(t), author(a), dueDate(""), checkedOut(false), checkedOutBy("") {}

    virtual ~LibraryItem() {}

    // Getters (Encapsulation)
    string getItemID()   const { return itemID; }
    string getTitle()    const { return title; }
    string getAuthor()   const { return author; }
    string getDueDate()  const { return dueDate; }
    bool   isCheckedOut()const { return checkedOut; }
    string getBorrowedBy()const { return checkedOutBy; }

    // Setters
    void setTitle(string newTitle)     { title = newTitle; }
    void setAuthor(string newAuthor)   { author = newAuthor; }
    void setDueDate(string newDueDate) { dueDate = newDueDate; }
    void setCheckedOutBy(string pid)   { checkedOutBy = pid; }

    // Pure Virtual Functions (Abstraction + Polymorphism)
    virtual void checkOut()          = 0;
    virtual void returnItem()        = 0;
    virtual void displayDetails() const = 0;
    virtual string getItemType() const = 0;
};

// ============================================================
// DERIVED CLASS 1: Book  (Inheritance)
// ============================================================
class Book : public LibraryItem {
private:
    string isbn;
    string genre;
    int    pages;

public:
    Book(string id, string title, string author, string isbn, string genre, int pages)
        : LibraryItem(id, title, author), isbn(isbn), genre(genre), pages(pages) {
        // Validate ISBN length
        if (isbn.length() != 10 && isbn.length() != 13)
            throw invalid_argument("ISBN must be 10 or 13 digits. Got: " + isbn);
        if (pages <= 0)
            throw invalid_argument("Page count must be positive.");
    }

    string getISBN()  const { return isbn; }
    string getGenre() const { return genre; }
    int    getPages() const { return pages; }

    void checkOut() override {
        if (checkedOut)
            throw runtime_error("Book '" + getTitle() + "' is already checked out.");
        checkedOut = true;
        setDueDate(addDays(14)); // 2-week loan
        cout << "  [CHECKED OUT] Book: \"" << getTitle() << "\" | Due: " << getDueDate() << endl;
    }

    void returnItem() override {
        if (!checkedOut)
            throw runtime_error("Book '" + getTitle() + "' is not checked out.");
        checkedOut = false;
        checkedOutBy = "";
        setDueDate("");
        cout << "  [RETURNED] Book: \"" << getTitle() << "\" returned successfully." << endl;
    }

    void displayDetails() const override {
        cout << "  Type        : " << getItemType() << endl;
        cout << "  Item ID     : " << getItemID() << endl;
        cout << "  Title       : " << getTitle() << endl;
        cout << "  Author      : " << getAuthor() << endl;
        cout << "  ISBN        : " << isbn << endl;
        cout << "  Genre       : " << genre << endl;
        cout << "  Pages       : " << pages << endl;
        cout << "  Status      : " << (checkedOut ? "Checked Out (By: " + checkedOutBy + ")" : "Available") << endl;
        if (checkedOut) cout << "  Due Date    : " << getDueDate() << endl;
    }

    string getItemType() const override { return "Book"; }
};

// ============================================================
// DERIVED CLASS 2: DVD  (Inheritance)
// ============================================================
class DVD : public LibraryItem {
private:
    int    duration;  // in minutes
    string director;
    string rating;    // G, PG, PG-13, R

public:
    DVD(string id, string title, string director, int duration, string rating)
        : LibraryItem(id, title, director), duration(duration), director(director), rating(rating) {
        if (duration <= 0)
            throw invalid_argument("DVD duration must be positive.");
    }

    int    getDuration() const { return duration; }
    string getDirector() const { return director; }
    string getRating()   const { return rating; }

    void checkOut() override {
        if (checkedOut)
            throw runtime_error("DVD '" + getTitle() + "' is already checked out.");
        checkedOut = true;
        setDueDate(addDays(7)); // 1-week loan
        cout << "  [CHECKED OUT] DVD: \"" << getTitle() << "\" | Due: " << getDueDate() << endl;
    }

    void returnItem() override {
        if (!checkedOut)
            throw runtime_error("DVD '" + getTitle() + "' is not checked out.");
        checkedOut = false;
        checkedOutBy = "";
        setDueDate("");
        cout << "  [RETURNED] DVD: \"" << getTitle() << "\" returned successfully." << endl;
    }

    void displayDetails() const override {
        cout << "  Type        : " << getItemType() << endl;
        cout << "  Item ID     : " << getItemID() << endl;
        cout << "  Title       : " << getTitle() << endl;
        cout << "  Director    : " << director << endl;
        cout << "  Duration    : " << duration << " mins" << endl;
        cout << "  Rating      : " << rating << endl;
        cout << "  Status      : " << (checkedOut ? "Checked Out (By: " + checkedOutBy + ")" : "Available") << endl;
        if (checkedOut) cout << "  Due Date    : " << getDueDate() << endl;
    }

    string getItemType() const override { return "DVD"; }
};

// ============================================================
// DERIVED CLASS 3: Magazine  (Inheritance)
// ============================================================
class Magazine : public LibraryItem {
private:
    int    issueNumber;
    string publisher;
    string publishDate;

public:
    Magazine(string id, string title, string publisher, int issueNum, string pubDate)
        : LibraryItem(id, title, publisher), issueNumber(issueNum),
          publisher(publisher), publishDate(pubDate) {
        if (issueNumber <= 0)
            throw invalid_argument("Issue number must be positive.");
    }

    int    getIssueNumber()  const { return issueNumber; }
    string getPublisher()    const { return publisher; }
    string getPublishDate()  const { return publishDate; }

    void checkOut() override {
        if (checkedOut)
            throw runtime_error("Magazine '" + getTitle() + "' is already checked out.");
        checkedOut = true;
        setDueDate(addDays(3)); // 3-day loan
        cout << "  [CHECKED OUT] Magazine: \"" << getTitle() << "\" | Due: " << getDueDate() << endl;
    }

    void returnItem() override {
        if (!checkedOut)
            throw runtime_error("Magazine '" + getTitle() + "' is not checked out.");
        checkedOut = false;
        checkedOutBy = "";
        setDueDate("");
        cout << "  [RETURNED] Magazine: \"" << getTitle() << "\" returned successfully." << endl;
    }

    void displayDetails() const override {
        cout << "  Type        : " << getItemType() << endl;
        cout << "  Item ID     : " << getItemID() << endl;
        cout << "  Title       : " << getTitle() << endl;
        cout << "  Publisher   : " << publisher << endl;
        cout << "  Issue No.   : " << issueNumber << endl;
        cout << "  Publish Date: " << publishDate << endl;
        cout << "  Status      : " << (checkedOut ? "Checked Out (By: " + checkedOutBy + ")" : "Available") << endl;
        if (checkedOut) cout << "  Due Date    : " << getDueDate() << endl;
    }

    string getItemType() const override { return "Magazine"; }
};

// ============================================================
// LIBRARY MANAGEMENT SYSTEM CLASS
// Uses dynamic memory + LibraryItem* (Polymorphism)
// ============================================================
class LibraryManagementSystem {
private:
    static const int MAX_ITEMS = 100;
    LibraryItem* libraryItems[MAX_ITEMS]; // Array of base class pointers (Polymorphism)
    int itemCount;

    vector<Patron*> patrons;
    int nextItemNum;
    int nextPatronNum;

    string generateItemID()   { return "ITM" + to_string(100 + nextItemNum++); }
    string generatePatronID() { return "PAT" + to_string(100 + nextPatronNum++); }

    LibraryItem* findItem(const string& id) {
        for (int i = 0; i < itemCount; i++)
            if (libraryItems[i]->getItemID() == id) return libraryItems[i];
        return nullptr;
    }

    int findItemIndex(const string& id) {
        for (int i = 0; i < itemCount; i++)
            if (libraryItems[i]->getItemID() == id) return i;
        return -1;
    }

    Patron* findPatron(const string& id) {
        for (auto& p : patrons)
            if (p->getPatronID() == id) return p;
        return nullptr;
    }

public:
    LibraryManagementSystem() : itemCount(0), nextItemNum(1), nextPatronNum(1) {
        for (int i = 0; i < MAX_ITEMS; i++) libraryItems[i] = nullptr;
    }

    ~LibraryManagementSystem() {
        for (int i = 0; i < itemCount; i++) delete libraryItems[i];
        for (auto& p : patrons) delete p;
    }

    // ---- ADD ITEMS ----
    void addBook() {
        try {
            string title, author, isbn, genre;
            int pages;
            cin.ignore();
            cout << "  Enter Title    : "; getline(cin, title);
            cout << "  Enter Author   : "; getline(cin, author);
            cout << "  Enter ISBN (10 or 13 digits): "; cin >> isbn;
            cout << "  Enter Genre    : "; cin.ignore(); getline(cin, genre);
            cout << "  Enter Pages    : "; cin >> pages;

            if (itemCount >= MAX_ITEMS) throw runtime_error("Library catalog is full!");

            string id = generateItemID();
            libraryItems[itemCount++] = new Book(id, title, author, isbn, genre, pages);
            cout << "\n  [ADDED] Book added | ID: " << id << endl;
        } catch (const invalid_argument& e) {
            cout << "\n  [INVALID INPUT] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    void addDVD() {
        try {
            string title, director, rating;
            int duration;
            cin.ignore();
            cout << "  Enter Title    : "; getline(cin, title);
            cout << "  Enter Director : "; getline(cin, director);
            cout << "  Enter Duration (mins): "; cin >> duration;
            cout << "  Enter Rating (G/PG/PG-13/R): "; cin >> rating;

            if (itemCount >= MAX_ITEMS) throw runtime_error("Library catalog is full!");

            string id = generateItemID();
            libraryItems[itemCount++] = new DVD(id, title, director, duration, rating);
            cout << "\n  [ADDED] DVD added | ID: " << id << endl;
        } catch (const invalid_argument& e) {
            cout << "\n  [INVALID INPUT] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    void addMagazine() {
        try {
            string title, publisher, pubDate;
            int issueNum;
            cin.ignore();
            cout << "  Enter Title      : "; getline(cin, title);
            cout << "  Enter Publisher  : "; getline(cin, publisher);
            cout << "  Enter Issue No.  : "; cin >> issueNum;
            cout << "  Enter Publish Date (YYYY-MM-DD): "; cin >> pubDate;

            if (itemCount >= MAX_ITEMS) throw runtime_error("Library catalog is full!");

            string id = generateItemID();
            libraryItems[itemCount++] = new Magazine(id, title, publisher, issueNum, pubDate);
            cout << "\n  [ADDED] Magazine added | ID: " << id << endl;
        } catch (const invalid_argument& e) {
            cout << "\n  [INVALID INPUT] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    // ---- PATRON MANAGEMENT ----
    void registerPatron() {
        try {
            string name, email;
            cin.ignore();
            cout << "  Enter Name  : "; getline(cin, name);
            cout << "  Enter Email : "; getline(cin, email);
            if (name.empty()) throw invalid_argument("Name cannot be empty.");

            string id = generatePatronID();
            patrons.push_back(new Patron(id, name, email));
            cout << "\n  [REGISTERED] Patron registered | ID: " << id << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    // ---- CHECKOUT ----
    void checkOutItem() {
        try {
            string itemID, patronID;
            cout << "  Enter Item ID   : "; cin >> itemID;
            cout << "  Enter Patron ID : "; cin >> patronID;

            LibraryItem* item = findItem(itemID);
            if (!item) throw runtime_error("Item ID '" + itemID + "' not found.");

            Patron* patron = findPatron(patronID);
            if (!patron) throw runtime_error("Patron ID '" + patronID + "' not found.");

            item->setCheckedOutBy(patronID);
            item->checkOut();  // Polymorphic call
            patron->borrowItem(itemID);

        } catch (const runtime_error& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    // ---- RETURN ----
    void returnItem() {
        try {
            string itemID, patronID;
            cout << "  Enter Item ID   : "; cin >> itemID;
            cout << "  Enter Patron ID : "; cin >> patronID;

            LibraryItem* item = findItem(itemID);
            if (!item) throw runtime_error("Item ID '" + itemID + "' not found.");

            Patron* patron = findPatron(patronID);
            if (!patron) throw runtime_error("Patron ID '" + patronID + "' not found.");

            if (!patron->hasBorrowed(itemID))
                throw runtime_error("Patron '" + patronID + "' did not borrow this item.");

            item->returnItem(); // Polymorphic call
            patron->returnItem(itemID);

        } catch (const runtime_error& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    // ---- SEARCH ----
    void searchByTitle() {
        cin.ignore();
        string keyword;
        cout << "  Enter title keyword: "; getline(cin, keyword);
        string kw = keyword;
        transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

        bool found = false;
        cout << "\n  ===== Search Results =====" << endl;
        for (int i = 0; i < itemCount; i++) {
            string t = libraryItems[i]->getTitle();
            string tl = t;
            transform(tl.begin(), tl.end(), tl.begin(), ::tolower);
            if (tl.find(kw) != string::npos) {
                cout << "\n  [" << libraryItems[i]->getItemType() << "] "
                     << libraryItems[i]->getItemID() << " - "
                     << libraryItems[i]->getTitle()
                     << " (" << (libraryItems[i]->isCheckedOut() ? "Checked Out" : "Available") << ")" << endl;
                found = true;
            }
        }
        if (!found) cout << "  No items found matching '" << keyword << "'." << endl;
    }

    // ---- DISPLAY ----
    void displayAllItems() {
        if (itemCount == 0) { cout << "\n  No items in catalog." << endl; return; }
        cout << "\n  ===== Library Catalog (" << itemCount << " items) =====" << endl;
        cout << "  " << left << setw(10) << "ID"
             << setw(12) << "Type"
             << setw(30) << "Title"
             << setw(20) << "Author/Director"
             << "Status" << endl;
        cout << "  " << string(85, '-') << endl;
        for (int i = 0; i < itemCount; i++) {
            cout << "  " << left << setw(10) << libraryItems[i]->getItemID()
                 << setw(12) << libraryItems[i]->getItemType()
                 << setw(30) << libraryItems[i]->getTitle().substr(0,28)
                 << setw(20) << libraryItems[i]->getAuthor().substr(0,18)
                 << (libraryItems[i]->isCheckedOut() ? "Checked Out" : "Available") << endl;
        }
    }

    void displayItemDetails() {
        string id;
        cout << "  Enter Item ID: "; cin >> id;
        LibraryItem* item = findItem(id);
        if (!item) { cout << "  [ERROR] Item not found." << endl; return; }
        cout << "\n  ===== Item Details =====" << endl;
        item->displayDetails(); // Polymorphic call
    }

    void displayAllPatrons() {
        if (patrons.empty()) { cout << "\n  No patrons registered." << endl; return; }
        cout << "\n  ===== Registered Patrons (" << patrons.size() << ") =====" << endl;
        for (const auto& p : patrons) {
            cout << "\n  "; p->display();
        }
    }

    // ---- REMOVE ITEM (with delete - memory management) ----
    void removeItem() {
        try {
            string id;
            cout << "  Enter Item ID to remove: "; cin >> id;
            int idx = findItemIndex(id);
            if (idx == -1) throw runtime_error("Item ID '" + id + "' not found.");

            if (libraryItems[idx]->isCheckedOut())
                throw runtime_error("Cannot remove a checked-out item. Return it first.");

            string title = libraryItems[idx]->getTitle();
            delete libraryItems[idx];  // Proper memory release
            // Shift array
            for (int i = idx; i < itemCount - 1; i++)
                libraryItems[i] = libraryItems[i + 1];
            libraryItems[--itemCount] = nullptr;

            cout << "  [REMOVED] \"" << title << "\" removed from catalog." << endl;
        } catch (const exception& e) {
            cout << "\n  [ERROR] " << e.what() << endl;
        }
    }

    // ---- POLYMORPHISM DEMO ----
    void polymorphismDemo() {
        if (itemCount == 0) { cout << "\n  No items for demo." << endl; return; }
        cout << "\n  ===== Polymorphism Demo: displayDetails() via LibraryItem* =====" << endl;
        for (int i = 0; i < itemCount; i++) {
            cout << "\n  [Item " << i+1 << "]" << endl;
            cout << "  " << string(40, '-') << endl;
            libraryItems[i]->displayDetails(); // Runtime polymorphism via base pointer
        }
    }
};

// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {
    LibraryManagementSystem lms;
    int choice;

    cout << "============================================" << endl;
    cout << "    LIBRARY MANAGEMENT SYSTEM (C++)         " << endl;
    cout << "  Abstraction | Encapsulation               " << endl;
    cout << "  Inheritance | Polymorphism                " << endl;
    cout << "============================================" << endl;
    cout << "  Today: " << getCurrentDate() << endl;

    do {
        cout << "\n  ========== MAIN MENU ==========" << endl;
        cout << "  --- Items ---" << endl;
        cout << "  1.  Add Book" << endl;
        cout << "  2.  Add DVD" << endl;
        cout << "  3.  Add Magazine" << endl;
        cout << "  4.  Display All Items" << endl;
        cout << "  5.  Display Item Details" << endl;
        cout << "  6.  Search by Title" << endl;
        cout << "  7.  Remove Item" << endl;
        cout << "  --- Patrons ---" << endl;
        cout << "  8.  Register Patron" << endl;
        cout << "  9.  Display All Patrons" << endl;
        cout << "  --- Transactions ---" << endl;
        cout << "  10. Check Out Item" << endl;
        cout << "  11. Return Item" << endl;
        cout << "  --- Demo ---" << endl;
        cout << "  12. Polymorphism Demo" << endl;
        cout << "  13. Exit" << endl;
        cout << "  Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:  lms.addBook(); break;
            case 2:  lms.addDVD(); break;
            case 3:  lms.addMagazine(); break;
            case 4:  lms.displayAllItems(); break;
            case 5:  lms.displayItemDetails(); break;
            case 6:  lms.searchByTitle(); break;
            case 7:  lms.removeItem(); break;
            case 8:  lms.registerPatron(); break;
            case 9:  lms.displayAllPatrons(); break;
            case 10: lms.checkOutItem(); break;
            case 11: lms.returnItem(); break;
            case 12: lms.polymorphismDemo(); break;
            case 13: cout << "\n  Thank you! Goodbye!" << endl; break;
            default: cout << "\n  [ERROR] Invalid choice!" << endl;
        }
    } while (choice != 13);

    return 0;
}