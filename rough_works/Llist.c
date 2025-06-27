#include <stdio.h>   // Required for input/output operations (e.g., printf, scanf)
#include <stdlib.h>  // Required for dynamic memory allocation (e.g., malloc, free)

// ---------------------------------------------------
// 1. Node Structure Definition
// ---------------------------------------------------
// Defines the structure of a single node in the linked list.
// Each node contains two parts:
//    - 'data': An integer to store the actual value.
//    - 'next': A pointer to the next node in the sequence.
//              It's a self-referential structure.
typedef struct Node {
    int data;
    struct Node* next; // Pointer to the next node
} Node;

// ---------------------------------------------------
// 2. Function Prototypes (Declarations)
// ---------------------------------------------------
// Declaring functions before their definitions allows the compiler to know
// about them when they are called in main or other functions.
Node* createNode(int data);
void insertAtBeginning(Node** headRef, int data);
void insertAtEnd(Node** headRef, int data);
void insertAfterNode(Node* prevNode, int data);
void deleteNode(Node** headRef, int key);
void printList(Node* head);
int searchList(Node* head, int key);
void reverseList(Node** headRef);
void freeList(Node** headRef); // Function to free all allocated memory

// ---------------------------------------------------
// 3. Function Definitions
// ---------------------------------------------------

/**
 * @brief Creates a new node with the given data.
 * @param data The integer value to store in the new node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
Node* createNode(int data) {
    // Allocate memory for a new Node using malloc.
    // sizeof(Node) determines the amount of memory needed for one Node structure.
    Node* newNode = (Node*)malloc(sizeof(Node));

    // Check if memory allocation was successful.
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL; // Return NULL to indicate failure
    }

    // Initialize the data part of the new node.
    newNode->data = data;
    // Initialize the next pointer to NULL, as it's currently the last node or
    // its next link is not yet determined.
    newNode->next = NULL;

    return newNode; // Return the pointer to the new node
}

/**
 * @brief Inserts a new node at the beginning of the linked list.
 * @param headRef A pointer to the head pointer of the list.
 * Using a double pointer (pointer to pointer) allows the function
 * to modify the actual head pointer in the calling function (e.g., main).
 * @param data The integer value to insert.
 */
void insertAtBeginning(Node** headRef, int data) {
    Node* newNode = createNode(data); // Create the new node

    if (newNode == NULL) { // Handle memory allocation failure
        return;
    }

    // Link the new node to the current head of the list.
    // The 'next' of the new node now points to what 'head' was pointing to.
    newNode->next = *headRef;

    // Update the head of the list to point to the new node.
    // This effectively makes the new node the first node.
    *headRef = newNode;

    printf("Inserted %d at the beginning.\n", data);
}

/**
 * @brief Inserts a new node at the end of the linked list.
 * @param headRef A pointer to the head pointer of the list.
 * Allows modification of the head if the list was initially empty.
 * @param data The integer value to insert.
 */
void insertAtEnd(Node** headRef, int data) {
    Node* newNode = createNode(data); // Create the new node

    if (newNode == NULL) { // Handle memory allocation failure
        return;
    }

    // If the list is empty, the new node becomes the head.
    if (*headRef == NULL) {
        *headRef = newNode;
        printf("Inserted %d at the end (list was empty, now head).\n", data);
        return;
    }

    // Traverse the list to find the last node.
    Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }

    // Link the last node to the new node.
    // The 'next' of the old last node now points to the new node.
    current->next = newNode;
    printf("Inserted %d at the end.\n", data);
}

/**
 * @brief Inserts a new node after a given existing node.
 * @param prevNode A pointer to the node after which the new node should be inserted.
 * @param data The integer value to insert.
 */
void insertAfterNode(Node* prevNode, int data) {
    // Check if the previous node is NULL (invalid).
    if (prevNode == NULL) {
        printf("Previous node cannot be NULL for insertion after.\n");
        return;
    }

    Node* newNode = createNode(data); // Create the new node

    if (newNode == NULL) { // Handle memory allocation failure
        return;
    }

    // Link the new node to the node that was originally after prevNode.
    newNode->next = prevNode->next;

    // Link the previous node to the new node.
    prevNode->next = newNode;

    printf("Inserted %d after node with data %d.\n", data, prevNode->data);
}

/**
 * @brief Deletes the first occurrence of a node with the given key (data).
 * @param headRef A pointer to the head pointer of the list.
 * Allows modification of the head if the first node is deleted.
 * @param key The integer value to delete.
 */
void deleteNode(Node** headRef, int key) {
    Node* current = *headRef;
    Node* prev = NULL;

    // Case 1: If the head node itself holds the key to be deleted.
    if (current != NULL && current->data == key) {
        *headRef = current->next; // Change head
        free(current);             // Free old head node
        printf("Deleted %d from the beginning.\n", key);
        return;
    }

    // Case 2: Search for the key to be deleted, keep track of the previous node.
    // Traverse the list until the key is found or the end of the list is reached.
    while (current != NULL && current->data != key) {
        prev = current;
        current = current->next;
    }

    // If key was not present in the list.
    if (current == NULL) {
        printf("Key %d not found in the list. Nothing to delete.\n", key);
        return;
    }

    // If key was found, unlink the node from the linked list.
    // The 'next' of the previous node now points to the node after 'current'.
    prev->next = current->next;

    // Free the memory of the deleted node.
    free(current);
    printf("Deleted %d from the list.\n", key);
}

/**
 * @brief Prints all the data elements in the linked list.
 * @param head A pointer to the head of the list.
 */
void printList(Node* head) {
    Node* current = head; // Start from the head
    printf("Linked List: ");
    if (current == NULL) {
        printf("List is empty.\n");
        return;
    }
    // Traverse the list until the end (current becomes NULL).
    while (current != NULL) {
        printf("%d -> ", current->data); // Print the data
        current = current->next;         // Move to the next node
    }
    printf("NULL\n"); // Indicate the end of the list
}

/**
 * @brief Searches for a given key in the linked list.
 * @param head A pointer to the head of the list.
 * @param key The integer value to search for.
 * @return 1 if the key is found, 0 otherwise.
 */
int searchList(Node* head, int key) {
    Node* current = head; // Start from the head
    int position = 0;

    // Traverse the list
    while (current != NULL) {
        if (current->data == key) {
            printf("Key %d found at position %d.\n", key, position);
            return 1; // Key found
        }
        current = current->next; // Move to the next node
        position++;
    }
    printf("Key %d not found in the list.\n", key);
    return 0; // Key not found
}

/**
 * @brief Reverses the linked list.
 * @param headRef A pointer to the head pointer of the list.
 * Allows modification of the head as the head changes after reversal.
 */
void reverseList(Node** headRef) {
    Node* prev = NULL;    // Pointer to the previous node (initially NULL)
    Node* current = *headRef; // Pointer to the current node (starts at head)
    Node* next = NULL;    // Pointer to the next node (temporary storage)

    // Traverse the list
    while (current != NULL) {
        next = current->next; // Store next node
        current->next = prev; // Reverse current node's pointer

        // Move pointers one position ahead
        prev = current;
        current = next;
    }
    *headRef = prev; // Update head to the new first node (old last node)
    printf("Linked List reversed.\n");
}

/**
 * @brief Frees all allocated memory in the linked list to prevent memory leaks.
 * @param headRef A pointer to the head pointer of the list.
 * Sets the head to NULL after freeing.
 */
void freeList(Node** headRef) {
    Node* current = *headRef;
    Node* nextNode;

    // Traverse the list and free each node.
    while (current != NULL) {
        nextNode = current->next; // Store the next node before freeing current
        free(current);            // Free the current node
        current = nextNode;       // Move to the next node
    }
    *headRef = NULL; // Set head to NULL to indicate an empty list
    printf("All memory freed. List is now empty.\n");
}


// ---------------------------------------------------
// 4. Main Function (Entry Point of the Program)
// ---------------------------------------------------
int main() {
    Node* head = NULL; // Initialize an empty linked list by setting head to NULL

    printf("--- Linked List Program for Analysis ---\n\n");

    // --- Insertion Operations ---
    printf("--- Insertion Tests ---\n");
    insertAtEnd(&head, 10);      // Insert 10 at the end (list: 10)
    printList(head);
    insertAtBeginning(&head, 5); // Insert 5 at the beginning (list: 5 -> 10)
    printList(head);
    insertAtEnd(&head, 20);      // Insert 20 at the end (list: 5 -> 10 -> 20)
    printList(head);
    insertAtBeginning(&head, 2); // Insert 2 at the beginning (list: 2 -> 5 -> 10 -> 20)
    printList(head);

    // Insert after a specific node (e.g., after node with data 5)
    printf("\nAttempting to insert 15 after node with data 5:\n");
    Node* nodeToInsertAfter = head->next; // Node with data 5 (assuming 2->5->...)
    if (nodeToInsertAfter != NULL && nodeToInsertAfter->data == 5) {
        insertAfterNode(nodeToInsertAfter, 15); // list: 2 -> 5 -> 15 -> 10 -> 20
        printList(head);
    } else {
        printf("Node with data 5 not found to insert after.\n");
    }

    // --- Traversal and Search Operations ---
    printf("\n--- Traversal and Search Tests ---\n");
    printList(head); // Print the current state of the list

    searchList(head, 10); // Search for an existing element
    searchList(head, 99); // Search for a non-existing element

    // --- Deletion Operations ---
    printf("\n--- Deletion Tests ---\n");
    deleteNode(&head, 10); // Delete existing element (list: 2 -> 5 -> 15 -> 20)
    printList(head);
    deleteNode(&head, 2);  // Delete the head element (list: 5 -> 15 -> 20)
    printList(head);
    deleteNode(&head, 99); // Attempt to delete non-existing element
    printList(head);
    deleteNode(&head, 20); // Delete last element (list: 5 -> 15)
    printList(head);

    // --- Reversal Operation ---
    printf("\n--- Reversal Test ---\n");
    reverseList(&head); // Reverse the list (list: 15 -> 5)
    printList(head);

    // --- Final Deletions to Empty List ---
    printf("\n--- Emptying List Tests ---\n");
    deleteNode(&head, 5); // Delete 5 (list: 15)
    printList(head);
    deleteNode(&head, 15); // Delete 15 (list: empty)
    printList(head);
    deleteNode(&head, 100); // Attempt to delete from empty list
    printList(head);

    // --- Re-populate and Free ---
    printf("\n--- Re-populating and Freeing List ---\n");
    insertAtEnd(&head, 1);
    insertAtEnd(&head, 2);
    insertAtEnd(&head, 3);
    printList(head);
    freeList(&head); // Free all memory and set head to NULL
    printList(head); // Verify list is empty after freeing

    return 0; // Indicate successful program execution
}

