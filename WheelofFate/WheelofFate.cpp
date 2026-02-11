#include <iostream>
#include <stack>
#include <unordered_map>

using namespace std;

// Basic node structure
// data : stored value
// skip : how many steps to move forward
// next : pointer for circular list
struct FateNode {
    int data;
    int skip;
    FateNode* next;
};

// WheelOfFate Class
// Hybrid Structure: Circular Linked List + Stack + Hash Table
class WheelOfFate {
private:
    FateNode* head;                 // Start of the circular list
    FateNode* current;              // Current position
    int size;                       // Number of nodes
    int visitCount;                 // How many spins occurred

    stack<FateNode*> history;       // Rollback support
    unordered_map<int, int> fate;   // Data -> signature table

public:
    // Start with an empty structure
    WheelOfFate() {
        head = nullptr;
        current = nullptr;
        size = 0;
        visitCount = 0;
    }

    // Clean up all allocated nodes
    ~WheelOfFate() {
        if (!head) return;

        FateNode* temp = head->next;
        while (temp != head) {
            FateNode* del = temp;
            temp = temp->next;
            delete del;
        }
        delete head;
    }

    // Add a new node to the circular list
    void insert(int data) {
        FateNode* newNode = new FateNode;
        newNode->data = data;
        newNode->skip = 1;

        // Simple deterministic signature
        fate[data] = (data * 31 + 7) % 97 + 1;

        if (head == nullptr) {
            head = newNode;
            newNode->next = newNode;
            current = newNode;
        } else {
            FateNode* temp = head;
            while (temp->next != head)
                temp = temp->next;

            temp->next = newNode;
            newNode->next = head;
        }
        size++;
    }

    // Perform a single spin step
    void spinOnce() {
        if (!current || size == 0) return;

        history.push(current);
        visitCount++;

        int previousData = 0;
        if (history.size() > 1) {
            FateNode* temp = history.top();
            history.pop();
            previousData = history.top()->data;
            history.push(temp);
        }

        current->skip =
            (fate[current->data] ^ visitCount ^ previousData) % size + 1;

        int i = 0;
        while (i < current->skip) {
            current = current->next;
            i++;
        }
    }

    // Run spin multiple times
    void spinUntil(int k) {
        int i = 0;
        while (i < k) {
            spinOnce();
            i++;
        }
    }

    // Roll back k steps if possible
    void rollback(int k) {
        while (k > 0 && !history.empty()) {
            current = history.top();
            history.pop();
            visitCount--;
            k--;
        }
    }

    // Remove the current node from the list
    void deleteCurrent() {
        if (!current || size == 0) return;

        FateNode* temp = current;
        FateNode* prev = current;

        while (prev->next != current)
            prev = prev->next;

        if (current == head)
            head = head->next;

        prev->next = current->next;
        current = current->next;

        fate.erase(temp->data);
        delete temp;
        size--;
    }

    // Print current node value
    void printCurrent() const {
        if (current)
            cout << "Current Node: " << current->data << endl;
    }

    // Debug helper to show step-by-step movement
    void traverseWithTrace(int steps) const {
        if (!current) return;

        FateNode* temp = current;
        cout << "Step | Data | Signature | Skip | Next\n";
        cout << "------------------------------------\n";

        int i = 1;
        while (i <= steps) {
            int signature = fate.at(temp->data);
            int skipVal = (signature ^ i) % size + 1;

            FateNode* nextNode = temp;
            int j = 0;
            while (j < skipVal) {
                nextNode = nextNode->next;
                j++;
            }

            cout << i << "    | "
                 << temp->data << "    | "
                 << signature << "        | "
                 << skipVal << "    | "
                 << nextNode->data << endl;

            temp = nextNode;
            i++;
        }
    }
};

// Main function (demo)
int main() {
    WheelOfFate wheel;

    wheel.insert(10);
    wheel.insert(25);
    wheel.insert(7);
    wheel.insert(42);
    wheel.insert(18);

    wheel.spinUntil(5);
    wheel.printCurrent();

    wheel.rollback(2);
    wheel.printCurrent();

    cout << "\nFate Trace:\n";
    wheel.traverseWithTrace(8);

    return 0;
}
