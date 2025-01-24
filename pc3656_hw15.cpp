//pc3656
//hw15

#include<iostream>
#include<fstream>
using namespace std;

//Classes declarations.
class LinkedList;
class LinkedListNode;
class Person;

//Anoying Person class. 
//(The person is annoying, not the class).
class Person {
    private:
        double amountSpent;
        string name;
        bool mustTrade;
    public:
        Person() : name(""), amountSpent(0), mustTrade(true){}
        Person(string fullName, double d);
        ~Person(){}
        double getamountSpent() {return amountSpent;}
        string getName() {return name;}
        bool getmustTrade() {return mustTrade;}
        void setamountSpent(double w) {amountSpent = w;}
        void setName(string n) {name = n;}
        void setmustTrade(bool c) {mustTrade = c;}
};

//Constructor definition.
Person::Person(string fullName, double d) {
    mustTrade = true;
    amountSpent = d;
    name = fullName;
}


//Function prototype to set the debts between two people.
void debtPaidIsFriendKept(Person& A, Person& B, double target);

//Class for the nodes.
class LinkedListNode {
	private:
		Person person;
		LinkedListNode* next;
	public:
		//Constructor.
		LinkedListNode(Person newPerson = Person(), LinkedListNode* newnext = nullptr) : person(newPerson), next(newnext) {}
		friend class LinkedList;
}; 

//Linked list class
class LinkedList{
	private:
		LinkedListNode* head;
		//Function declaration of recursiveCopy to use in the copy constructor and the overloading of the = operator.
		LinkedListNode* recursiveCopy(LinkedListNode* rhs);
        double averageSpent;
	public:
		//Constructor
		LinkedList() : head(nullptr) {}
		//Copy constryctor
		LinkedList(const LinkedList& rhs) : head(nullptr) {*this = rhs;}
		//operator = overloading.
		LinkedList& operator=(const LinkedList& rhs);
        //Destructor
		~LinkedList() { clear(); }
        //Insertion functions
		void insertAtHead(Person newPerson);
		void insertAtEnd(Person newPerson);
        void insertAtPoint(LinkedListNode* ptr,  Person newPerson);
		Person removeFromHead();
        //Return true if head points to nullptr
		bool isEmpty() const { return head == nullptr; }
		void clear();
		int size() const;
        void print() const;
        double getAverage() const {return averageSpent;}
        void computeAverage();
        void settleDebts();
};

//Implementation of recursiveCopy.
LinkedListNode* LinkedList::recursiveCopy(LinkedListNode* rhs) {
	if (rhs == nullptr) {
		return nullptr;
	}
	return new LinkedListNode(rhs -> person, recursiveCopy(rhs -> next));
}

//Overloading the operator
LinkedList& LinkedList::operator=(const LinkedList& rhs) {
	if(this == &rhs) {
		return *this;
	}
	clear();
	head = recursiveCopy(rhs.head);
	return *this;
}

//Implementation of insertAtHead
void LinkedList::insertAtHead(Person newPerson) {
    LinkedListNode* temp = new LinkedListNode(newPerson);
    temp -> next = head;
    head = temp;
}

//Implementation of insertAtEnd
void LinkedList::insertAtEnd(Person newPerson) {
	if(isEmpty()) {
		insertAtHead(newPerson);
		return;
	}
	LinkedListNode* temp = new LinkedListNode(newPerson);
	LinkedListNode* end = head;
	while(end -> next != nullptr) {
		end = end -> next;
	}
	end -> next = temp;
}

//Implementation of insertAtPoint
//Inserts at the end if the point is not found.
void LinkedList::insertAtPoint(LinkedListNode* ptr, Person newPerson) {
    //If the list is empty
    if(isEmpty()) {
        insertAtHead(newPerson);
        return;
    }
    //If ptr is null
    if(ptr == nullptr) {
        insertAtEnd(newPerson);
        return;
    }

    //New node to be inserted
    LinkedListNode* temp = new LinkedListNode(newPerson);

    //Pointer to find the position in the list
    LinkedListNode* position = head;
    while(position -> next != ptr && position -> next != nullptr) {
        position = position -> next;
    }
    if(position != nullptr) {
        temp -> next = position -> next;
        position -> next = temp;
    }
    else {
        insertAtEnd(newPerson);
        //InsertAtEnd is called, so deletion of temp that has not been used.
        delete temp;
    } 
}

//Implementation of removeFromHead.
Person LinkedList::removeFromHead() {
    if(isEmpty()) {
        cout << "Error, cannot remove from empty list." << endl;
        exit(1);
    }
    Person person = head -> person;
    LinkedListNode* temp = head;
    head = head -> next;
    delete temp;
    return person;
}

//Implementation of the clear function.
void LinkedList::clear() {
    while(head != nullptr) {
        LinkedListNode* temp = head;
        head = head -> next;
        delete temp;
    }
}

//Implementation of size
int LinkedList::size() const {
	int count = 0;
	LinkedListNode* temp = head;
	while(temp != nullptr) {
		count ++;
		temp = temp -> next;
	}
	return count;
}

//Implementation of print
void LinkedList::print() const {
    LinkedListNode* temp = head;
    while(temp != nullptr) {
        Person p = temp -> person;
        cout << p.getName() << endl;
        cout << p.getamountSpent() << endl;
        temp = temp -> next;
    }
}

//Settle debts. At the end of the execution everybody should have paid the target.
void LinkedList::settleDebts() {
    if(isEmpty()) {
        cout << "Nobody here, no debt to settle." << endl;  
    }
    

    //Iterate once over the list and remove the people who already have paid exactly the target.
    LinkedListNode* maybeFine = head;
    while(maybeFine != nullptr) {
        Person X = maybeFine -> person;
        if(X.getamountSpent() == averageSpent) {
            cout << X.getName() << ", you don't need to do anything" << endl;
            X.setmustTrade(false);
        }
        maybeFine = maybeFine -> next;
    }

    bool settled(false);

    do {
        settled = false;
        
        //Get a person A to iterate with.
        LinkedListNode* currentA = head;
        while(currentA != nullptr) {
            Person& A = currentA -> person;
            //if A already spent the right amount, continue to the next person.
            if(A.getmustTrade() == false) {
                currentA = currentA -> next;
                continue;
            }

            LinkedListNode* currentB = currentA -> next;
            while (currentB != nullptr) {
                Person& B = currentB -> person;

                //if B already spent the right amount, continue to the next person.
                if(B.getmustTrade() == false) {
                    currentB = currentB -> next;
                    continue;
                }

                double aBefore = A.getamountSpent();
                double bBefore = B.getamountSpent();

                //Settle the debts between A and B
                debtPaidIsFriendKept(A, B, averageSpent);
                
                //If debts have been settled
                if (A.getamountSpent() != aBefore || B.getamountSpent() != bBefore) {
                    settled = true;
                }

                currentB = currentB -> next;
            }
            currentA = currentA -> next;
        }

    } while(settled);

    cout << "In the end, you should all have spent around $" << averageSpent << endl;
}

//Compute the average spent by the group.
void LinkedList::computeAverage() {
    LinkedListNode* temp = head;
    double total;
    while(temp != nullptr) {
        Person p = temp -> person;
        total += p.getamountSpent();
        temp = temp -> next;
    }
    averageSpent = total / size();
}

//////////




//Function prototypes:
//Helper function to get a string from the user.
string getString();
//Helper function to print the file.
void printFile(istream & f);

//Helper functions to deal with string representing double:
//Return true if the string represents a valid double.
bool validDoubleString(string s);
//Return a double from a string.
double stringToDouble(string s);
//Function to populate the list from the file.
void populateListFromFile(istream & f, LinkedList& list);
//Set the debts between two people
void debtPaidIsFriendKept(Person& A, Person& B, double target);


int main() {
    //String to querry the file name.
    
    //string querryFile;
    //cout << "Enter the filnemane: ";
    //querryFile = getString();

    //Declaring and opening the file.
    string querryFile("data.txt");
    ifstream inFile;
    inFile.open(querryFile);
    
    //Declaring a list to represent the group of friends.
    LinkedList group;
    //Populate the list.
    populateListFromFile(inFile, group);

    group.computeAverage();

    group.settleDebts();

    //Closing the file and exiting the program.
    inFile.close();
    return 0;
}

//Helper function to get a string from the user.
string getString() {
    string s;
    char ans = '\0';
    while(ans != '\n') {
        cin.get(ans);
        if(ans != '\n') {
            s += ans;
        }
    }
    return s;
}

//Function to print the file.
void printFile(istream & f) {
    char next;
    while(f.get(next)) {
        cout << next;
    }
}

//Returns true if the string represent a valid double.
bool validDoubleString(string s) {
    //Return true if the string represent a double.
    //- or digit in first position
    if(s[0] != '-' && !isdigit(s[0])) {
        return false;
    }
    //only digit in the last position
    if(!isdigit(s[s.size() - 1])) {
        return false;
    }

    //contains a coma or a dot somewhere inside (only one)
    bool decimal(false);
    int begining(0);
    if (s[0] == '-') {
        begining = 1;
    }

    //Iterate and check that the format is correct.
    for (int i = begining; i < s.size(); i++) {
        //check if the character is not a digit;
        if(!isdigit(s[i])) {
            if(s[i] == '.' || s[i] == ',') {
                if(!decimal) {
                    decimal = true;
                } else return false;
            } else return false;
        }
    }

    return true;
}

//Return a double from a string.
double stringToDouble(string s) {
    //Initializes the result to 0.
    double result(0);

    //Check for empty string
    if(s.size() == 0) {
        return 0;
    }

    //Assess wheter the string represents a negative number.
    int begining(0);
    if(s[0] == '-') {
        //If yes, the begining of the string representing the number is 1.
        begining = 1;
    }

    //Finds in which position is the decimal point and if there is one
    bool decimal(false);
    int decimalIndex(0);
    //Iterates to determine this.
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == '.' || s[i] == ',') {
            decimal = true;
            decimalIndex = i;
            break;
        }
    }

    //If the number does not contain decimal point, the decimal index is equal to the size of the string, so the second loop does not run.
    if(!decimal) {
        decimalIndex = s.size();
    }
    
    //Initialize the factor to 1.
    double factor = 1;

    //Iterate from the decimal index to the begining of the string. 
    //Add to the result and multiply the factor by 10.
    for(int i = decimalIndex - 1; i >= begining; i--) {
        double n = s[i] - '0';
        result += n * factor;
        factor *= 10;
    }

    factor = 0.1;

    //Iterate from the decimal index of the string to the end of the string.
    //Add to the result and divide the factor by 10.
    for(int i = decimalIndex + 1; i < s.size(); i ++) {
        double n = s[i] - '0';
        result += n * factor;
        factor /= 10;
    }

    //In case of negative string, return the result negatively.
    if (begining == 1) {
        result = -result;
    }

    return result;
}

void populateListFromFile(istream & f, LinkedList& list) {
    char next;
    string numberString(""), nameString("");
    double n;
    bool numberFound(false);
    while(f.get(next)) {
        if(!numberFound && next == ' ') {
            numberFound = true;
        }
        else if(!numberFound){
            numberString += next;
        }
        else {
            if (next != '\n') {
                nameString += next;
            }
        }
        //If new Line
        if(next == '\n') {
            Person p(nameString, stringToDouble(numberString));
            list.insertAtEnd(p);
            //Reseting everything for the next line.
            numberFound = false;
            numberString = "";
            nameString = "";
        }
    }
    //Accounting for the last person.
    Person p(nameString, stringToDouble(numberString));
    list.insertAtEnd(p);
}

//Set the debts between two people
void debtPaidIsFriendKept(Person& A, Person& B, double target) {
    double a = A.getamountSpent();
    double b = B.getamountSpent();
    double refund(0), maxPossible(0);

    //Nobody owes anything.
    if(a >= target && b >= target) {
        return;
    }

    //Nothing to do here.
    if (a == target || b == target) {
        return;
    }

    

    //If we are here, someone owes someone.

    //If A spent less than B
    //A owes B
    if(a < b) {
        //figure out how much
        maxPossible = target - a;

        //Calculate what is the max that b can get without reaching the target.
        double bMaxRefund = b - target;

        if(maxPossible <= bMaxRefund) {
            refund = maxPossible;
        } else refund = bMaxRefund;

        //Update the wallet and output the transaction.
        A.setamountSpent(a + refund);
        B.setamountSpent(b - refund);
        cout << A.getName() << ", you give " << B.getName() << " $" << refund << endl;
    }
        
    //If B spent lsee than A
    //B owes A
    else {
        //figure out how much.
        maxPossible = target - b;

        //Calculate what is the max that b can get without reaching the target.
        double aMaxRefund = a - target;


        if(maxPossible <= aMaxRefund) {
            refund = maxPossible;
        } else refund = aMaxRefund;
        

        //Update the wallet and output the transaction.
        cout << B.getName() << ", you give " << A.getName() << " $" << refund << endl;
        A.setamountSpent(a - refund);
        B.setamountSpent(b + refund);
    }

    if(A.getamountSpent() == target) {
        A.setmustTrade(false);
    }
    if(B.getamountSpent() == target) {
        B.setmustTrade(false);
    }
        
}
