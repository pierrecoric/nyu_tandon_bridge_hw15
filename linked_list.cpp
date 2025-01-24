//////////Linked List Implementation

//Classes declarations.
template <class T>
class LinkedList;

template <class T>
class LinkedListNode;

//Class for the nodes.
template <class T>
class LinkedListNode {
	private:
		T data;
		LinkedListNode* next;
	public:
		//Constructor with a member initialization list. T newData = T() will create a new instance of T and assign it to data in the list.
		LinkedListNode(T newData = T(), LinkedListNode<T>* newnext = nullptr) : data(newData), next(newnext) {}
		friend class LinkedList<T>;
};

template <class T> 
class LinkedList{
	private:
		LinkedListNode<T>* head;
		//Function declaration of recursiveCopy to use in the copy constructor and the overloading of the = operator.
		LinkedListNode<T>* recursiveCopy(LinkedListNode<T>* rhs);
	public:
		//Constructor
		LinkedList() : head(nullptr) {}
		//Copy constryctor
		LinkedList(const LinkedList& rhs) : head(nullptr) {*this = rhs;}
		//operator = overloading.
		LinkedList<T>& operator=(const LinkedList<T>& rhs);
        //Destructor
		~LinkedList() { clear(); }
        //Insertion functions
		void insertAtHead(T newData);
		void insertAtEnd(T newData);
        void insertAtPoint(LinkedListNode<T>* ptr, T newData);
		T removeFromHead();
        //Return true if head points to nullptr
		bool isEmpty() const { return head == nullptr; }
		void clear();
		int size() const;
        void print() const;
};

//Implementation of recursiveCopy.
template <class T>
LinkedListNode<T>* LinkedList<T>::recursiveCopy(LinkedListNode<T>* rhs) {
	if (rhs == nullptr) {
		return nullptr;
	}
	return new LinkedListNode<T>(rhs -> data, recursiveCopy(rhs -> next));
}

//Overloading the operator
template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
	if(this == &rhs) {
		return *this;
	}
	clear();
	head = recursiveCopy(rhs.head);
	return *this;
}

//Implementation of insertAtHead
template <class T>
void LinkedList<T>::insertAtHead(T newData) {
    LinkedListNode<T>* temp = new LinkedListNode<T>(newData);
    temp -> next = head;
    head = temp;
}

//Implementation of insertAtEnd
template <class T>
void LinkedList<T>::insertAtEnd(T newData) {
	if(isEmpty()) {
		insertAtHead(newData);
		return;
	}
	LinkedListNode<T>* temp = new LinkedListNode<T>(newData);
	LinkedListNode<T>* end = head;
	while(end -> next != nullptr) {
		end = end -> next;
	}
	end -> next = temp;
}

//Implementation of insertAtPoint
//Inserts at the end if the point is not found.
template <class T>
void LinkedList<T>::insertAtPoint(LinkedListNode<T>* ptr, T newData) {
    //If the list is empty
    if(isEmpty()) {
        insertAtHead(newData);
        return;
    }
    //If ptr is null
    if(ptr == nullptr) {
        insertAtEnd(newData);
        return;
    }

    //New node to be inserted
    LinkedListNode<T>* temp = new LinkedListNode<T>(newData);

    //Pointer to find the position in the list
    LinkedListNode<T>* position = head;
    while(position -> next != ptr && position -> next != nullptr) {
        position = position -> next;
    }
    if(position != nullptr) {
        temp -> next = position -> next;
        position -> next = temp;
    }
    else {
        insertAtEnd(newData);
        //InsertAtEnd is called, so deletion of temp that has not been used.
        delete temp;
    } 
}

//Implementation of removeFromHead.
template <class T>
T LinkedList<T>::removeFromHead() {
    if(isEmpty()) {
        cout << "Error, cannot remove from empty list." << endl;
        exit(1);
    }
    T data = head -> data;
    LinkedListNode<T>* temp = head;
    head = head -> next;
    delete temp;
    return data;
}

//Implementation of the clear function.
template <class T>
void LinkedList<T>::clear() {
    while(head != nullptr) {
        LinkedListNode<T>* temp = head;
        head = head -> next;
        delete temp;
    }
}

//Implementation of size
template <class T>
int LinkedList<T>::size() const {
	int count = 0;
	LinkedListNode<T>* temp = head;
	while(temp != nullptr) {
		count ++;
		temp = temp -> next;
	}
	return count;
}

//Implementation of print
template <class T>
void LinkedList<T>::print() const {
    LinkedListNode<T>* temp = head;
    while(temp != nullptr) {
        cout << temp -> data << endl;
        temp = temp -> next;
    }
}

//////////