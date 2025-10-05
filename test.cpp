#include<iostream>
using namespace std;

struct Node{
    int data;
    Node* next;
};

Node* createList(int n) {
    Node* head = nullptr;
    Node* tail = nullptr;

    for(int i=0; i<n; i++) {
        cout<<"["<<i+1<<"]: ";
        Node* newNode2 = new Node();
        cin>>newNode2->data;
        newNode2->next = nullptr;

        if(head == nullptr) {
            head = newNode2;
            tail = newNode2;
        } else {
            tail->next = newNode2;
            tail = newNode2;
        }

    }
    return head;
}

Node* addlist(Node* l1, Node* l2) {
    Node* head = nullptr;
    Node* tail = nullptr;
    int sum = 0;

    Node* newNode = new Node();
    if((l1->data+l2->data) <10) {
        newNode->data = l1->data + l2->data;
    } else {
        newNode->data = (l1->data + l2->data)%10;
        sum = (l1->data + l2->data)/10;
    }
    newNode->next = nullptr;
    head = newNode;
    tail = newNode;
    l1 = l1->next;
    l2 = l2->next;

    while(l1 != nullptr && l2 != nullptr) {
        Node* newNode = new Node;
        if((l1->data+l2->data + sum) <10) {
            newNode->data = l1->data + l2->data + sum;
            sum = 0;

        } else {
            newNode->data = (sum + l1->data + l2->data)%10;
            sum = (l1->data + l2->data + sum)/10;
        }
        newNode->next = nullptr;
            
        tail->next = newNode;
        tail = newNode;
        
        l1 = l1->next;
        l2 = l2->next;

    }

    while(l1 != nullptr && l2 == nullptr) {
        Node* newNode = new Node;
        if((sum + l1->data) <10) {
            newNode->data = sum + l1->data;
            sum = 0;

        } else {
            newNode->data = (sum + l1->data)%10;
            sum = (sum + l1->data)/10;
        }
        newNode->next = nullptr;    
        tail->next = newNode;
        tail = newNode;
        l1 = l1->next; 
    }

    while(l1 == nullptr && l2 != nullptr) {
        Node* newNode = new Node;
        if((sum + l2->data) <10) {
            newNode->data = sum + l2->data;
            sum = 0;

        } else {
            newNode->data = (sum + l2->data)%10;
            sum = (sum + l2->data)/10;
        }
        newNode->next = nullptr;    
        tail->next = newNode;
        tail = newNode;
        l2 = l2->next; 
    }


    if(sum!=0) {
        Node* newNode = new Node;
        newNode->data = sum;
        newNode->next = nullptr;
        tail->next = newNode;
        tail = newNode;
    }

    return head;    
}

void printList(Node* list) {
    Node* temp = list;
    while(temp != nullptr) {
        cout<<temp->data<<" ";
        temp = temp->next;
    }
}

int main() {
    int n1,n2;
    cout<<"Number of Elements in l1: ";
    cin>>n1;
    Node* l1 = createList(n1);
    
    cout<<"Number of Elements in l2: ";
    cin>>n2;
    Node* l2 = createList(n2);

    cout<<"l1: ";
    printList(l1);
    cout<< "  l2: ";
    printList(l2);
    cout<<endl;

    Node* l3 = addlist(l1, l2);

    cout<<"Added list: ";
    printList(l3);

    return 0;
}