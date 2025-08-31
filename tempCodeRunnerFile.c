#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct
{
int id;
char name[50];
int quantity;
float price;
}Product;

typedef struct Shipment
{
int id;
int productid;
char destination[50];
struct Shipment*next;
}Shipment;

typedef struct 
{
int id;
char customer[50];
int productid;
int quantity;
}Order;

typedef struct History
{
char action[30];
Product product;
struct History*next;
}History;

Product inventory[100];
int productCount=0;

Shipment*shipments=NULL;
Order orders[100];
int front=-1,rear=-1;

History*history=NULL;

void addProduct();
void addShipment();
void placeOrder();
void undo();
void showProducts();
void clearScreen();
void cleanup();
void addHistory(const char*action,Product p);

// === Utility Functions ===
void clearScreen()
{
system("clear||cls");
}
void addHistory(const char*action,Product p){
History*newItem=(History*)malloc(sizeof(History));
if(!newItem)
{
printf("Memory allocation failed for history!\n");
return;
}
strcpy(newItem->action,action);
newItem->product=p;
newItem->next=history;
history=newItem;
}

void cleanup()
{
Shipment*s=shipments;
while(s)
{
Shipment*temp=s;
s=s->next;
free(temp);
}
    History*h = history;
    while (h) {
        History* temp = h;
        h = h->next;
        free(temp);
    }
}

// === Feature Implementations ===

void addProduct() {
    Product p;
    printf("\nEnter Product ID: ");
    scanf("%d", &p.id);

    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == p.id) {
            printf("Error: A product with this ID already exists!\n");
            return;
        }
    }

    printf("Enter Product Name: ");
    getchar();  // flush newline
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);

    printf("Enter Price: ");
    scanf("%f", &p.price);

    inventory[productCount++] = p;
    addHistory("ADDED PRODUCT", p);

    printf("Product successfully added!\n");
}

void addShipment() {
    Shipment* newShip = (Shipment*)malloc(sizeof(Shipment));
    if (!newShip) {
        printf("Error: Unable to allocate memory for shipment.\n");
        return;
    }

    printf("\nEnter Shipment ID: ");
    scanf("%d", &newShip->id);
    printf("Enter Product ID: ");
    scanf("%d", &newShip->productid);
    printf("Enter Destination: ");
    getchar(); // flush newline
    fgets(newShip->destination, sizeof(newShip->destination), stdin);
    newShip->destination[strcspn(newShip->destination, "\n")] = '\0';

    newShip->next = shipments;
    shipments = newShip;

    printf("Shipment successfully added!\n");
}

void placeOrder() {
    if (rear == 99) {
        printf("The order queue is full. Cannot add a new order.\n");
        return;
    }

    if (front == -1) front = 0;

    Order newOrder;
    printf("\nEnter Order ID: ");
    scanf("%d", &newOrder.id);
    printf("Enter Customer Name: ");
    getchar(); // flush newline
    fgets(newOrder.customer, sizeof(newOrder.customer), stdin);
    newOrder.customer[strcspn(newOrder.customer, "\n")] = '\0';

    printf("Enter Product ID: ");
    scanf("%d", &newOrder.productid);
    printf("Enter Quantity: ");
    scanf("%d", &newOrder.quantity);

    orders[++rear] = newOrder;
    printf("Order successfully placed!\n");
}

void undo() {
    if (history == NULL) {
        printf("Nothing to undo!\n");
        return;
    }

    History* lastAction = history;
    history = history->next;

    if (strcmp(lastAction->action, "ADDED PRODUCT") == 0) {
        for (int i = 0; i < productCount; i++) {
            if (inventory[i].id == lastAction->product.id) {
                for (int j = i; j < productCount - 1; j++) {
                    inventory[j] = inventory[j + 1];
                }
                productCount--;
                break;
            }
        }
    }

    printf("Undid: %s (Product ID %d)\n", lastAction->action, lastAction->product.id);
    free(lastAction);
}

void showProducts() {
    printf("\n%-5s %-20s %-10s %-10s\n", "ID", "NAME", "QUANTITY", "PRICE");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < productCount; i++) {
        printf("%-5d %-20s %-10d %-10.2f\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].quantity,
               inventory[i].price);
    }

    if (productCount == 0) {
        printf("No products available in inventory.\n");
    }
}

// === Main Menu ===
int main() {
    int choice;

    while (1) {
        clearScreen();
        printf("\n===== WAREHOUSE MANAGEMENT SYSTEM =====\n");
        printf("1. Product Management\n");
        printf("2. Shipment Management\n");
        printf("3. Order Management\n");
        printf("4. Undo Last Action\n");
        printf("5. View All Products\n");
        printf("0. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                addShipment();
                break;
            case 3:
                placeOrder();
                break;
            case 4:
                undo();
                break;
            case 5:
                showProducts();
                break;
            case 0:
                cleanup();
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); // flush newline
        getchar(); // wait for enter
    }

    return 0;
}
