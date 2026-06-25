#include <iostream>
#include <string>
using namespace std;
// ==================== ZONE BST ====================
struct Zone {
    int id;
    string name;
    Zone* left;
    Zone* right;
};
Zone* newZone(int id, string name) {
    Zone* z = new Zone();
    z->id = id;
    z->name = name;
    z->left = nullptr;
    z->right = nullptr;
    return z;
}
Zone* insertZone(Zone* root, int id, string name) {
    if (root == nullptr)
        return newZone(id, name);
    if (id < root->id)
        root->left = insertZone(root->left, id, name);
    else if (id > root->id)
        root->right = insertZone(root->right, id, name);
    else
        cout << "Zone ID already exists.\n";
    return root;
}
Zone* findMin(Zone* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}
Zone* deleteZone(Zone* root, int id) {
    if (root == nullptr) {
        cout << "Zone not found.\n";
        return root;
    }
    if (id < root->id)
        root->left = deleteZone(root->left, id);
    else if (id > root->id)
        root->right = deleteZone(root->right, id);
    else {
        if (root->left == nullptr) {
            Zone* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Zone* temp = root->left;
            delete root;
            return temp;
        }
        else {
            Zone* successor = findMin(root->right);
            root->id = successor->id;
            root->name = successor->name;
            root->right = deleteZone(root->right, successor->id);
        }
    }
    return root;
}
// FIX: Free all BST nodes to prevent memory leaks
void freeZones(Zone* root) {
    if (root == nullptr) return;
    freeZones(root->left);
    freeZones(root->right);
    delete root;
}
bool searchZone(Zone* root, int id) {
    if (root == nullptr) return false;
    if (id == root->id) return true;
    if (id < root->id) return searchZone(root->left, id);
    return searchZone(root->right, id);
}
string getZoneName(Zone* root, int id) {
    if (root == nullptr) return "";
    if (id == root->id) return root->name;
    if (id < root->id) return getZoneName(root->left, id);
    return getZoneName(root->right, id);
}
void inorder(Zone* root) {
    if (root == nullptr) return;
    inorder(root->left);
    cout << " [ID: " << root->id << "] " << root->name << "\n";
    inorder(root->right);
}
// ==================== ROBOT LINKED LIST ====================
struct RobotNode {
    int id;
    string sensorType;
    int battery;
    string status; // "Standby", "On Mission"
    int assignedZoneId; // FIX: track which zone robot is sent to
    RobotNode* next;
};
RobotNode* robotHead = nullptr;
void addRobot(int id, string sensor, int battery) {
    // Check for duplicate ID
    RobotNode* curr = robotHead;
    while (curr != nullptr) {
        if (curr->id == id) {
            cout << "Robot ID already exists.\n";
            return;
        }
        curr = curr->next;
    }
    RobotNode* r = new RobotNode();
    r->id = id;
    r->sensorType = sensor;
    r->battery = battery;
    r->status = "Standby";
    r->assignedZoneId = -1;
    r->next = robotHead;
    robotHead = r;
    cout << "Robot " << id << " added.\n";
}
void removeRobot(int id) {
    RobotNode* curr = robotHead;
    RobotNode* prev = nullptr;
    while (curr != nullptr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == nullptr) {
        cout << "Robot not found.\n";
        return;
    }
    if (prev == nullptr)
        robotHead = curr->next;
    else
        prev->next = curr->next;
    delete curr;
    cout << "Robot " << id << " removed.\n";
}
// FIX: Assign a robot to a zone and update its status
void assignRobotToZone(int robotId, int zoneId, string zoneName) {
    RobotNode* curr = robotHead;
    while (curr != nullptr) {
        if (curr->id == robotId) {
            if (curr->status == "On Mission") {
                cout << "Robot " << robotId << " is already on a mission in zone " << curr->assignedZoneId << ".\n";
                return;
            }
            curr->status = "On Mission";
            curr->assignedZoneId = zoneId;
            cout << "Robot " << robotId << " assigned to zone: " << zoneName << " (ID: " << zoneId << ")\n";
            return;
        }
        curr = curr->next;
    }
    cout << "Robot not found.\n";
}
// FIX: Mark robot as returned from mission
void returnRobot(int robotId) {
    RobotNode* curr = robotHead;
    while (curr != nullptr) {
        if (curr->id == robotId) {
            if (curr->status == "Standby") {
                cout << "Robot " << robotId << " is already on standby.\n";
                return;
            }
            cout << "Robot " << robotId << " returned from zone " << curr->assignedZoneId << ".\n";
            curr->status = "Standby";
            curr->assignedZoneId = -1;
            return;
        }
        curr = curr->next;
    }
    cout << "Robot not found.\n";
}
void displayRobots() {
    RobotNode* curr = robotHead;
    if (curr == nullptr) {
        cout << "No robots in roster.\n";
        return;
    }
    while (curr != nullptr) {
        cout << " Robot ID: " << curr->id
             << " | Sensor: " << curr->sensorType
             << " | Battery: " << curr->battery << "%"
             << " | Status: " << curr->status;
        if (curr->assignedZoneId != -1)
            cout << " (Zone " << curr->assignedZoneId << ")";
        cout << "\n";
        curr = curr->next;
    }
}
// FIX: Free all robot nodes on exit
void freeRobots() {
    while (robotHead != nullptr) {
        RobotNode* temp = robotHead;
        robotHead = robotHead->next;
        delete temp;
    }
}
// ==================== MISSION QUEUE ====================
struct QueueNode {
    int zoneId;
    string zoneName;
    bool priority;
    QueueNode* next;
};
QueueNode* qFront = nullptr;
QueueNode* qRear = nullptr;
void enqueue(int zoneId, string zoneName, bool priority = false) {
    QueueNode* q = new QueueNode();
    q->zoneId = zoneId;
    q->zoneName = zoneName;
    q->priority = priority;
    q->next = nullptr;
    if (priority) {
        q->next = qFront;
        qFront = q;
        if (qRear == nullptr) qRear = q;
    }
    else {
        if (qRear == nullptr) {
            qFront = qRear = q;
        }
        else {
            qRear->next = q;
            qRear = q;
        }
    }
    cout << "Zone \"" << zoneName << "\" added to mission queue";
    if (priority) cout << " [PRIORITY]";
    cout << ".\n";
}
// FIX: Dequeue now links with robot assignment
void dequeue(Zone* bstRoot) {
    if (qFront == nullptr) {
        cout << "Mission queue is empty.\n";
        return;
    }
    int zoneId = qFront->zoneId;
    string zoneName = qFront->zoneName;
    cout << "Dispatching robot to zone: \"" << zoneName << "\" (ID: " << zoneId << ")\n";
    // Find a standby robot to assign
    RobotNode* curr = robotHead;
    bool assigned = false;
    while (curr != nullptr) {
        if (curr->status == "Standby") {
            assignRobotToZone(curr->id, zoneId, zoneName);
            assigned = true;
            break;
        }
        curr = curr->next;
    }
    if (!assigned)
        cout << " Warning: No standby robots available to assign!\n";
    QueueNode* temp = qFront;
    qFront = qFront->next;
    if (qFront == nullptr) qRear = nullptr;
    delete temp;
}
void displayQueue() {
    QueueNode* curr = qFront;
    if (curr == nullptr) {
        cout << "Mission queue is empty.\n";
        return;
    }
    cout << " Mission Queue (front to back):\n";
    while (curr != nullptr) {
        cout << " Zone ID: " << curr->zoneId << " | \"" << curr->zoneName << "\"";
        if (curr->priority) cout << " [PRIORITY]";
        cout << "\n";
        curr = curr->next;
    }
}
// FIX: Free all queue nodes on exit
void freeQueue() {
    while (qFront != nullptr) {
        QueueNode* temp = qFront;
        qFront = qFront->next;
        delete temp;
    }
    qRear = nullptr;
}
// ==================== PATH STACK ====================
struct StackNode {
    int zoneId;
    string zoneName;
    StackNode* next;
};
StackNode* stackTop = nullptr;
void pushPath(int zoneId, string zoneName) {
    StackNode* s = new StackNode();
    s->zoneId = zoneId;
    s->zoneName = zoneName;
    s->next = stackTop;
    stackTop = s;
    cout << "Robot advanced into zone: \"" << zoneName << "\"\n";
}
void popPath() {
    if (stackTop == nullptr) {
        cout << "No path to retreat from.\n";
        return;
    }
    cout << "Robot retreating from zone: \"" << stackTop->zoneName << "\"\n";
    StackNode* temp = stackTop;
    stackTop = stackTop->next;
    delete temp;
}
void displayPath() {
    StackNode* curr = stackTop;
    if (curr == nullptr) {
        cout << "Path is empty.\n";
        return;
    }
    cout << " Robot Path (current to start):\n";
    while (curr != nullptr) {
        cout << " Zone ID: " << curr->zoneId << " | \"" << curr->zoneName << "\"\n";
        curr = curr->next;
    }
}
// FIX: Free all stack nodes on exit
void freeStack() {
    while (stackTop != nullptr) {
        StackNode* temp = stackTop;
        stackTop = stackTop->next;
        delete temp;
    }
}
// ==================== MAIN ====================
int main() {
    Zone* bstRoot = nullptr;
    int choice;
    do {
        cout << "\n========= VOLCANO EXPLORATION ROBOT =========\n";
        cout << "--- Zone Manager (BST) ---\n";
        cout << "1. Insert Zone\n";
        cout << "2. Search Zone\n";
        cout << "3. Delete Zone\n";
        cout << "4. Display All Zones (InOrder)\n";
        cout << "--- Robot Manager (Linked List) ---\n";
        cout << "5. Add Robot\n";
        cout << "6. Remove Robot\n";
        cout << "7. Display All Robots\n";
        cout << "8. Mark Robot as Returned\n";
        cout << "--- Mission Queue ---\n";
        cout << "9. Add Zone to Queue\n";
        cout << "10. Add Priority Zone to Queue\n";
        cout << "11. Dispatch Robot (Dequeue)\n";
        cout << "12. Display Queue\n";
        cout << "--- Path Tracker (Stack) ---\n";
        cout << "13. Advance into Zone (Push)\n";
        cout << "14. Retreat (Pop)\n";
        cout << "15. Display Path\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // FIX: clear newline before getline
        if (choice == 1) {
            int id; string name;
            cout << "Enter Zone ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Zone Name: ";
            getline(cin, name); // FIX: supports spaces
            bstRoot = insertZone(bstRoot, id, name);
            cout << "Zone inserted.\n";
        }
        else if (choice == 2) {
            int id;
            cout << "Enter Zone ID to search: ";
            cin >> id;
            if (searchZone(bstRoot, id))
                cout << "Zone found: \"" << getZoneName(bstRoot, id) << "\"\n";
            else
                cout << "Zone not found.\n";
        }
        else if (choice == 3) {
            int id;
            cout << "Enter Zone ID to delete: ";
            cin >> id;
            bstRoot = deleteZone(bstRoot, id);
            cout << "Zone deleted (if it existed).\n";
        }
        else if (choice == 4) {
            cout << "All Zones (safest to most dangerous):\n";
            inorder(bstRoot);
        }
        else if (choice == 5) {
            int id, battery; string sensor;
            cout << "Enter Robot ID: "; cin >> id; cin.ignore();
            cout << "Enter Sensor Type: ";
            getline(cin, sensor); // FIX: supports spaces
            cout << "Enter Battery %: "; cin >> battery;
            addRobot(id, sensor, battery);
        }
        else if (choice == 6) {
            int id;
            cout << "Enter Robot ID to remove: "; cin >> id;
            removeRobot(id);
        }
        else if (choice == 7) {
            displayRobots();
        }
        else if (choice == 8) {
            int id;
            cout << "Enter Robot ID to mark as returned: "; cin >> id;
            returnRobot(id);
        }
        else if (choice == 9) {
            int id; string name;
            cout << "Enter Zone ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Zone Name: ";
            getline(cin, name);
            enqueue(id, name, false);
        }
        else if (choice == 10) {
            int id; string name;
            cout << "Enter Zone ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Zone Name: ";
            getline(cin, name);
            enqueue(id, name, true);
        }
        else if (choice == 11) {
            dequeue(bstRoot); // FIX: passes BST so dispatch can link to robot
        }
        else if (choice == 12) {
            displayQueue();
        }
        else if (choice == 13) {
            int id; string name;
            cout << "Enter Zone ID: "; cin >> id;
            cin.ignore();
            cout << "Enter Zone Name: ";
            getline(cin, name);
            pushPath(id, name);
        }
        else if (choice == 14) {
            popPath();
        }
        else if (choice == 15) {
            displayPath();
        }
        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    // FIX: Clean up all dynamically allocated memory before exit
    freeZones(bstRoot);
    freeRobots();
    freeQueue();
    freeStack();
    cout << "Mission control shutting down.\n";
    return 0;
}
