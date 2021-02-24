/*

NAME - BIDYUT MONDAL, ROLL - 18MA20010

The code written is based on modified earclipping method.
The algorithm consists of finding such an ear/triangle, removing it from the polygon
which results in a new polygon that still meets the conditions and repeating until there is only one triangle left.
The running time complexity of the code is O(n*n).
In this code we go through each vertex of the polygon and check if we can cutoff the ear/triangle from the polygon.
We cut an ear from a polygon only when the angle between two edges is less than 180 (i.e that part of the polygon is convex)
and there doesnot lie any point inside the ear/triangle so that when we draw diagonal it does not cut any edge or diagonal.
The checking condition of each vertex take O(n) time, so for n vertices of a polygon the time complexity becomes O(n*n).
Lastly, I sorted the output diagonal array using quicksort which takes O(nlogn) time.
So the total time complexity still remains O(n*n).
Please note that the output diagonal is in sorted order based on starting vertex.

*/

#include <iostream>
using namespace std;

//function swap two numbers
void swap(int* a, int* b) { 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

//function to find the position of pivot element and rearrange the array with smaller element at left subarray and greater element in right subarray
int partition (int arr[][2], int low, int high) { 
    int pivot1 = arr[high][0], pivot2 = arr[high][1], i = low-1;        //choosing two pivot as we are sorting 2d array

    for (int j = low; j <= high- 1; j++) { 
        if (arr[j][0]<=pivot1) {                                        //If the current element is smaller we swap the elements
            if(arr[j][0]==pivot1 and arr[j][1]>pivot2) continue;
            i++;
            swap(&arr[i][0], &arr[j][0]);
            swap(&arr[i][1], &arr[j][1]); 
        } 
    } 
    //At last swapping the pivot element to its right position
    swap(&arr[i + 1][0], &arr[high][0]);
    swap(&arr[i + 1][1], &arr[high][1]);
    return (i + 1); 
} 

//Sort a 2d array recursively
void quickSort(int arr[][2], int low, int high) { 
    if (low < high) { 
        int pi = partition(arr, low, high); 
        //recursively calling quicksort arount the pivot element
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
}

//Node of a linked list
struct Node{
    float x,y;            //the (x,y) coordinates of the vertices
    int pos;              //the position the vertex in the polygon
    Node *prev;           //pointer to the previous node
    Node *next;           //pointer to the next node
};

//function to add a vertex to the linked list
void addVertex(Node** head, float x, float y, int p){
    //if the linked list is empty 
    if(*head == NULL){
        Node* node = new Node;        
        node->x = x;         //inserting the values
        node->y = y;         //inserting the values
        node->pos = p;       //inserting the values
        node->next = node->prev = node;
        *head = node;
        return;
    }
    //else add vertex after the last node of the linked list
    Node *last = (*head)->prev; //last node
    Node* node = new Node; 
    node->x = x;            //inserting the values
    node->y = y;            //inserting the values
    node->pos = p;          //inserting the values

    //pointing the node to the correct nodes
    node->next = *head;
    (*head)->prev = node;
    node->prev = last;
    last->next = node;
}

//function to delete a vertex from the linked list
void delVertex(Node** head, int p) {   
    Node *curr = *head, *prev = NULL;
    while(curr->pos != p){               //finding the node to be deleted
        if(curr->next == *head) return;
        prev = curr;
        curr = curr->next;
    }
    // If the node to be deleted is the only node in list
    if(curr->next == *head && prev == NULL){ 
        (*head) = NULL; 
        free(curr); 
        return; 
    } 
    //If the list has more than 1 node
    if (curr == *head) {                     //If head is to be deleted
        prev = (*head)->prev; 
        *head = (*head)->next; 
        prev->next = *head; 
        (*head)->prev = prev; 
        free(curr); 
    } 
    else if (curr->next == *head) {         //If tail is to be deleted
        prev->next = *head; 
        (*head)->prev = prev; 
        free(curr); 
    } 
    else { 
        Node* temp = curr->next; 
        prev->next = temp; 
        temp->prev = prev; 
        free(curr); 
    } 
} 

//function to check if a diagonal is possible from the adjacent vertices
bool isDiagonalPossible(Node* head, int p){
    Node *temp = head;
    while(temp->pos != p) temp = temp->next;
    Node *next = temp->next;
    Node *prev = temp->prev;

    //xi and yi (i=1,2,3) are the x coordinate and y coordinates of a vertex and its adjacent ones
    float x1=temp->x, x2=prev->x, x3=next->x, y1=temp->y, y2=prev->y, y3=next->y; 
    bool b1=(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))>0;           //checks if the angle between two lines is greater than 180 degree
    if(!b1) return false;                                   //if greater than 180 degre retuen false as it is concave
    Node* curr = next->next;
    while(curr->pos != prev->pos){                          //checks if any other vertex lie inside the trianlge
        float x=curr->x, y=curr->y, l1=(y-y1)*(x2-x1)-(x-x1)*(y2-y1), l2=(y-y2)*(x3-x2)-(x-x2)*(y3-y2), l3=(y-y3)*(x1-x3)-(x-x3)*(y1-y3);
        if(l1>=0 && l2>=0 && l3>=0) return false;           //if any vertex is found we return false immediately as diagonal cannot be drawn
        curr = curr->next; 
    }
    return true;                    //returns true if a diagonal can be drawn
}

int main(){

    float vertices[][2] = {{0,2},{1,3},{0,4},{1.6,4.6},{0.8,3.5},{4,3},{3,5},{5,5},{5,3},{4,4},{5,2},{4,1},{3,2},{3,3},{2,2},{3,1},{2,-1},{2.5,-1.5},{2,-2},{0.8,1},{1,2},{-2,0},{-2,-1},{-3,1}};
    int n = sizeof(vertices)/sizeof(vertices[0]);
    int len=n-3;
    int diagonal[len][2];
    Node* head = NULL;

    for(int i=0;i<n;i++) addVertex(&head,vertices[i][0],vertices[i][1],i);          //adding vertices to the linked list from the array
    Node* curr = head->next;

    for(int i=0; i<len; i++){
        if(isDiagonalPossible(curr,curr->pos)){         //checking if diagonal is possible or not
            diagonal[i][0] = curr->prev->pos;           //adding vertex no to the diagonal array
            diagonal[i][1] = curr->next->pos;           //adding vertex no to the diagonal array
            delVertex(&curr,curr->pos);                 //deleting the vertex from the linked list
        }
        else i--;
        curr = curr->next;             //if diagonal cannot be drawn with the adjacent vertices of the current vertex then we move on to the next point
    }

    quickSort(diagonal,0,len-1);        //Sorting the diagonal array
    //printing the array containing the vertices to be joined to get the triangulated polygon
    cout<<"\nThe polygon can be triangulated by joining these vertices : \n";
    cout<<"{";
    for(int i=0;i<len;i++){
        cout<<"{"<<diagonal[i][0]<<","<<diagonal[i][1]<<"}";
        if(i!=len-1) cout<<",";
    }
    cout<<"}\n\n";

    return 0;
}

