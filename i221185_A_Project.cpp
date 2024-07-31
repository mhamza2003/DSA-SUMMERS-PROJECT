#include <iostream>
#include <string>
using namespace std;



// Patient Node
class Patient 
{
public:
    string Name;
    int Age;
    string Illness;
    int PriorityNumber;
    Patient* Next;
    bool isScheduled;

    Patient(string n, int a, string ill, int p) 
    {
        Name = n;
        Age = a;
        Illness = ill;
        PriorityNumber = p;
        Next = NULL;
        isScheduled = false; 
    }
};



// Priority Queue Node
class PriorityNode 
{
public:
    Patient* PatientPN;
    PriorityNode* Next;

    PriorityNode(Patient* p) 
    {
        PatientPN = p;
        Next = NULL;
    }
};



// Priority Queue For Scheduling Appointments Based on Priority Number
class AppointmentQueue 
{
public:
    Patient* head;
    PriorityNode* Front;
    Patient* emergencyStack; 

    AppointmentQueue() 
    {
        head = NULL;
        Front = NULL;
        emergencyStack = NULL;
    }

    void addPatient(string n, int a, string ill, int p) 
    {
        Patient* newPatient = new Patient(n, a, ill, p);

        if (!head) 
        {
            head = newPatient;
        } 
        else 
        {
            Patient* temp = head;

            while (temp->Next) 
            {
                temp = temp->Next;
            }

            temp->Next = newPatient;

        }
    }

    bool isPriorityUnique(int p) 
    {
        Patient* temp = head;

        while (temp) 
        {
            if (temp->PriorityNumber == p) 
            {
                return false; 
            }

            temp = temp->Next;
        }

        return true; 
    }

    Patient* findPatient(string name) 
    {
        Patient* temp = head;

        while (temp) 
        {
            if (temp->Name == name) 
            {
                return temp;
            }

            temp = temp->Next;
        }

        return NULL;
    }

    void pushEmergencyPatient(Patient* patient) 
    {
        patient->Next = emergencyStack;
        emergencyStack = patient;
    }

    Patient* popEmergencyPatient() 
    {
        if (!emergencyStack) return NULL;

        Patient* topPatient = emergencyStack;
        emergencyStack = emergencyStack->Next;

        topPatient->Next = NULL;

        return topPatient;
    }

    void scheduleAppointment(Patient* patient) 
    {
        if (patient->isScheduled) return; // Skip if already scheduled

        PriorityNode* newNode = new PriorityNode(patient);

        if (!Front || Front->PatientPN->PriorityNumber > patient->PriorityNumber) 
        {
            newNode->Next = Front;
            Front = newNode;
        } 
        else 
        {
            PriorityNode* temp = Front;

            while (temp->Next && temp->Next->PatientPN->PriorityNumber <= patient->PriorityNumber) 
            {
                temp = temp->Next;
            }

            newNode->Next = temp->Next;
            temp->Next = newNode;
        }

        patient->isScheduled = true; // Mark the patient as scheduled
    }

    void displayPatients() 
    {
        Patient* temp = head;

        while (temp) 
        {
            cout << "Name: " << temp->Name << ", Age: " << temp->Age << ", Illness: " << temp->Illness << ", Priority Number: " << temp->PriorityNumber << endl;
            temp = temp->Next;
        }

        cout << endl;
    }

    void displayAppointments() 
    {
        PriorityNode* temp = Front;

        while (temp) 
        {
            cout << "Name: " << temp->PatientPN->Name << ", Age: " << temp->PatientPN->Age << ", Illness: " << temp->PatientPN->Illness << ", Priority Number: " << temp->PatientPN->PriorityNumber << endl;
            temp = temp->Next;
        }

        cout << endl;
    }

    void handleEmergency(string n, int a, string ill) 
    {
        Patient* emergencyPatient = findPatient(n);
        if (!emergencyPatient) 
        {
            emergencyPatient = new Patient(n, a, ill, 0);
            addPatient(n, a, ill, 0); // Add to the list of patients if not already present
        }

        // Push the emergency patient onto the stack
        if (!emergencyPatient->isScheduled) 
        {
            pushEmergencyPatient(emergencyPatient);
        }
    }

    void scheduleAppointments() 
    {
        // First, schedule all emergency cases from the stack
        Patient* emergencyPatient;
        while ((emergencyPatient = popEmergencyPatient()) != NULL) 
        {
            scheduleAppointment(emergencyPatient);
        }

        // Then, schedule all regular patients
        Patient* temp = head;

        while (temp) 
        {
            if (!temp->isScheduled && temp->PriorityNumber != 0) 
            {
                scheduleAppointment(temp);
            }

            temp = temp->Next;
        }
    }
};



// Resonance Healthcare Clinic Appointment Management System
class HealthcareClinic 
{
public:
    AppointmentQueue queue;

    void addPatient(string n, int a, string ill, int p) 
    {
        if (p == 0) 
        {
            cout << "Error: Priority number 0 is reserved for emergency cases. Please choose a different priority number.\n";
            return;
        }

        if (!queue.isPriorityUnique(p)) 
        {
            cout << "Error: Priority number " << p << " is already in use. Please choose a different priority number.\n";
            return;
        }

        queue.addPatient(n, a, ill, p);
    }

    void scheduleAppointments() 
    {
        queue.scheduleAppointments();
        cout << "Appointments scheduled successfully.\n";
    }

    void displayAllPatients() 
    {
        cout << "List of Patients:\n";
        queue.displayPatients();
    }

    void displayAllAppointments() 
    {
        cout << "Scheduled Appointments:\n";
        queue.displayAppointments();
    }

    void searchPatientByName(string name) 
    {
        Patient* patient = queue.findPatient(name);

        if (patient) 
        {
            cout << "Name: " << patient->Name << ", Age: " << patient->Age << ", Illness: " << patient->Illness << ", Priority Number: " << patient->PriorityNumber << endl;
        } 
        else 
        {
            cout << "Patient not found.\n";
        }
    }

    void handleEmergency(string n, int a, string ill) 
    {
        queue.handleEmergency(n, a, ill);
    }
};

int main() 
{
    HealthcareClinic clinic;

    int choice;
    do 
    {
        cout << "\n<------- RESONANCE HEALTHCARE CLINIC APPOINTMENT MANAGEMENT SYSTEM ------->\n";
        cout << "1. Add Patient\n";
        cout << "2. Display All Patients\n";
        cout << "3. Schedule Appointments\n";
        cout << "4. Display Scheduled Appointments\n";
        cout << "5. Search Patient by Name\n";
        cout << "6. ADD EMERGENCY PATIENT\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
        case 1: 
        {
            string name, illness;
            int age, priority;

            cout << "Enter patient name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter patient age: ";
            cin >> age;

            cout << "Enter patient illness: ";
            cin.ignore();
            getline(cin, illness);

            do 
            {
                cout << "Enter priority (lower number means higher priority, non-zero): ";
                cin >> priority;

                if (priority == 0) 
                {
                    cout << "Error: Priority number 0 is reserved for emergency cases. Please choose a different priority number.\n";
                }
                else if (!clinic.queue.isPriorityUnique(priority)) 
                {
                    cout << "Error: Priority number " << priority << " is already in use. Please choose a different priority number.\n";
                }

            } while (priority == 0 || !clinic.queue.isPriorityUnique(priority));

            clinic.addPatient(name, age, illness, priority);

            break;
        }
        case 2:
        {
            clinic.displayAllPatients();

            break;
        }
        case 3:
        {    
            clinic.scheduleAppointments();

            break;
        }
        case 4:
        {
            clinic.displayAllAppointments();

            break;
        }
        case 5: 
        {
            string name;

            cout << "Enter patient name to search: ";
            cin.ignore();
            getline(cin, name);

            clinic.searchPatientByName(name);

            break;
        }
        case 6: 
        {
            string name, illness;
            int age;

            cout << "Enter emergency patient name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter emergency patient age: ";
            cin >> age;

            cout << "Enter emergency patient illness: ";
            cin.ignore();
            getline(cin, illness);

            clinic.handleEmergency(name, age, illness);

            break;
        }
        case 7:
        {    
            cout << "Exiting.......\n";
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);



    return 0;
}
