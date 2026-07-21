#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define FILE_NAME "inventory.dat"

typedef enum {
    MISC,
    FOOD,
    ELECTRONICS,
    CLOTHING,
    // Additional categories can be added here
} Category;

typedef union {
    float nutritionalValue; // NEW FEATURE: Only relevant for FOOD category items
    struct {
        float warrantyPeriod; // Placeholder for future ELECTRONICS category attributes
        float screenSize;     // Placeholder for future ELECTRONICS category attributes
    };
    char size[10]; // Placeholder for future CLOTHING category attributes
    // Future category-specific attributes can be added here
} CategoryAttributes;

// Define the blueprint structure for our inventory items
typedef struct {
    int id;
    char name[50]; // Storage array allocated for string variables
    int quantity;
    float weight; // NEW FEATURE: Individual item weight for logistics calculations
    float totalWeight; // NEW FEATURE: Tracks cumulative weight for logistics
    float price;
    Category category;
    CategoryAttributes attributes; // NEW FEATURE: Union to hold category-specific attributes
} Item;

// Global memory array acts as our runtime database cache
Item inventory[MAX_ITEMS];
int itemCount = 0; // Tracks the current active number of records loaded

// Main UI & Flow Control Prototypes
void showMenu();
void addItem();
void displayInventory();
void searchItem();
void editItemQuantity(); // NEW FEATURE: Restock or reduce inventory stock
void editItemPrice();    // NEW FEATURE: Adjust pricing for items
void deleteItem();       // NEW FEATURE: Completely purge an item from system records

// File Management Prototypes
void loadFromFile();
void saveToFile();

// Advanced Sorting Comparator Prototypes (Updated for multi-direction toggles)
int sortDirection = 1; // 1 for Ascending, -1 for Descending
int compareById(const void *a, const void *b);
int compareByName(const void *a, const void *b);
int compareByPrice(const void *a, const void *b);

int main() {
    int choice;

    // Execute an initial hard drive scan immediately upon software initiation
    loadFromFile();

    do {
        showMenu();
        printf("Enter your choice (1-7): ");

        // Robust numeric checking guards loop against breaking on unexpected character entries
        if (scanf("%d", &choice) != 1) {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Invalid key signature. Please enter a valid menu number.\n");
            while (getchar() != '\n'); // Flushes the input buffer stream completely
            continue;
        }

        switch (choice) {
            case 1: addItem(); break;
            case 2: displayInventory(); break;
            case 3: searchItem(); break;
            case 4: editItemQuantity(); break; // Direct modification pipeline
            case 5: editItemPrice(); break;    // Price modification pipeline
            case 6: deleteItem(); break;       // Target elimination pipeline
            case 7: 
                printf("\nExiting the inventory system safely. Goodbye!\n"); 
                break;
            default: 
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Out of option boundaries. Select between 1 and 7.\n");
        }
    } while (choice != 7);

    return 0;
}

// Renders the main user control interface panel
void showMenu() {
 printf("\n==================================");
    printf("\n    INVENTORY MANAGEMENT SYSTEM   ");
 printf("\n==================================");
    printf("\n1. Add New Item");
    printf("\n2. Display & Sort Items");
    printf("\n3. Search Item by ID");
    printf("\n4. Edit Item Quantity (Restock)");
    printf("\n5. Edit Item Price");
    printf("\n6. Delete Item from Records");
    printf("\n7. Exit");
 printf("\n==================================\n");
}

// Adds a new product securely into the primary cache array matrices
void addItem() {
// Array safety boundary check
    if (itemCount >= MAX_ITEMS) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Inventory memory limits hit! Delete items first.\n");
        return;
    }

    Item newItem;

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\nEnter Unique Item ID (Integer): ");
    scanf("%d", &newItem.id);

// Scan loop ensures Primary Key (ID uniqueness constraint) is strictly protected
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == newItem.id) {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Primary Key violation. ID %d already exists.\n", newItem.id);
            return;
        }
    }

    printf("Enter Item Name: ");
    scanf(" %[^\n]", newItem.name); // Specifier scans for text strings containing spacing values

    printf("Enter Quantity: ");
    scanf("%d", &newItem.quantity);

    printf("Enter Item Weight: ");
    scanf("%f", &newItem.weight);

    newItem.totalWeight = newItem.quantity * newItem.weight; // Calculate total weight based on quantity
    printf("Total Weight for this item: %.2f\n", newItem.totalWeight);

    printf("Enter Price: ");
    scanf("%f", &newItem.price);

    printf("Select Category (0: MISC, 1: FOOD, 2: ELECTRONICS, 3: CLOTHING): ");
    int categoryChoice;
    scanf("%d", &categoryChoice);
    if (categoryChoice < 0 || categoryChoice > 3) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Invalid category selection. Defaulting to MISC.\n");
        newItem.category = MISC;
    } else {
        newItem.category = (Category)categoryChoice;
        switch (newItem.category) {
            case FOOD:
                printf("Category set to FOOD.\n");
                printf("Enter Nutritional Value: ");
                scanf("%f", &newItem.attributes.nutritionalValue); // NEW FEATURE: Capture nutritional value for food items
                break;
            case ELECTRONICS:
                printf("Category set to ELECTRONICS.\n");
                printf("Enter Warranty Period (in months): ");
                scanf("%f", &newItem.attributes.warrantyPeriod);
                printf("Enter Screen Size (in inches): ");
                scanf("%f", &newItem.attributes.screenSize);
                break;
            case CLOTHING:
                printf("Category set to CLOTHING.\n");
                printf("Enter the size (e.g., S, M, L, XL, UNKNOWN): ");
                int sizeInput;
                scanf("%d", &sizeInput);
                if (sizeInput < 0 || sizeInput > 3) {
                    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Invalid category selection. Defaulting to UNKNOWN.\n");
                    strcpy(newItem.attributes.size, "UNKNOWN");
                } else {
                    scanf(" %s", newItem.attributes.size); // Assuming size is a string attribute for clothing
                }
                break;
            default:
                printf("Category set to MISC.\n");
        }
    }

// Save item structurally into current available index slot
    inventory[itemCount] = newItem;
    itemCount++;

    // Instantly sync RAM arrays to file system storage to protect data against power failures
    saveToFile();
    printf("\n[SUCCESS] Entry logged into system and synchronized to local storage.\n");
}
// Display mechanism featuring advanced multi-attribute and dual-direction sorting options
void displayInventory() {
    if (itemCount == 0) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[INFO] Database is blank. No records available to display.\n");
        return;
    }

    int sortChoice, dirChoice;
    printf("\n--- Step 1: Choose Sorting Metric ---");
    printf("\n1. Unsorted (Native Order)");
    printf("\n2. Sort by ID");
    printf("\n3. Sort by Name (Alphabetical)");
    printf("\n4. Sort by Price");
    printf("\nEnter selection (1-4): ");
    scanf("%d", &sortChoice);

// Only ask for direction parameters if user chooses to actively sort the records
    if (sortChoice >= 2 && sortChoice <= 4) {
        printf("\n--- Step 2: Choose Order Velocity ---");
        printf("\n1. Ascending  (Low to High / A-Z)");
        printf("\n2. Descending (High to Low / Z-A)");
        printf("\nEnter selection (1-2): ");
        scanf("%d", &dirChoice);

        // Multiplier flag switches comparator mathematical equations
        sortDirection = (dirChoice == 2) ? -1 : 1;
    }

    // Allocate a non-destructive temporary workspace to keep original array unchanged
    Item displayList[MAX_ITEMS];
    memcpy(displayList, inventory, sizeof(Item) * itemCount);

    // Standard QuickSort call engine utilizing function pointer switches
    switch (sortChoice) {
        case 2: qsort(displayList, itemCount, sizeof(Item), compareById); break;
        case 3: qsort(displayList, itemCount, sizeof(Item), compareByName); break;
        case 4: qsort(displayList, itemCount, sizeof(Item), compareByPrice); break;
        default: break; // Unsorted configuration leaves memory array raw
    }

    // Layout reporting headers
    printf("\n%-10s %-25s %-10s %-10s\n", "Item ID", "Item Name", "Quantity", "Price");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < itemCount; i++) {
        printf("%-10d %-25s %-10d $%-9.2f\n", 
               displayList[i].id, 
               displayList[i].name, 
               displayList[i].quantity, 
               displayList[i].price);
    }
}

// Performs a linear array search scanning key IDs
void searchItem() {
    if (itemCount == 0) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[INFO] Search canceled. Database is empty.\n");
        return;
    }

    int searchId;
    printf("\nEnter the Item ID to locate: ");
    scanf("%d", &searchId);

    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == searchId) {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n=== Record Retrieved! ===");
            printf("\nID:       %d", inventory[i].id);
            printf("\nName:     %s", inventory[i].name);
            printf("\nQuantity: %d", inventory[i].quantity);
            printf("\nPrice:    $%.2f\n", inventory[i].price);
            printf("\nCategory: ");
            switch (inventory[i].category) {
                case FOOD:
                    printf("FOOD\n");
                    printf("Nutritional Value: %.2f\n", inventory[i].attributes.nutritionalValue);
                    break;
                case ELECTRONICS:
                    printf("ELECTRONICS\n");
                    printf("Warranty Period: %.2f months\n", inventory[i].attributes.warrantyPeriod);
                    printf("Screen Size: %.2f inches\n", inventory[i].attributes.screenSize);
                    break;
                case CLOTHING:
                    printf("CLOTHING\n");
                    printf("Size: %s\n", inventory[i].attributes.size);
                    break;
                default:
                    printf("MISC\n"); break;
            }
            return; // Target found, break context out early
        }
    }
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Profile search failed. ID %d does not exist.\n", searchId);
}

// Modifies quantity parameters for an item
void editItemQuantity() {
    if (itemCount == 0) {
        printf("\n[INFO] Edit canceled. Database is empty.\n");
        return;
    }

    int targetId, foundIndex = -1;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\nEnter target Item ID to update stock quantity: ");
    scanf("%d", &targetId);

    // Locate structural array coordinate matching ID
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ERROR] Item ID not found.\n");
        return;
    }

    // Output live parameters to operator
    printf("\nCurrent status of \"%s\": %d items in stock.\n", inventory[foundIndex].name, inventory[foundIndex].quantity);

    printf("Enter new total stock quantity value: ");
    scanf("%d", &inventory[foundIndex].quantity);

    // Sync structural adjustments safely to disk file
    saveToFile();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[SUCCESS] Quantity metrics updated and synchronized to storage file.\n");
}

void editItemPrice() {
    if (itemCount == 0) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[INFO] Edit canceled. Database is empty.\n");
        return;
    }

    int targetId, foundIndex = -1;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\nEnter target Item ID to update price: ");
    scanf("%d", &targetId);

    // Locate structural array coordinate matching ID
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("[ERROR] Item ID not found.\n");
        return;
    }

    // Output live parameters to operator
    printf("\nCurrent price of \"%s\": $%.2f\n", inventory[foundIndex].name, inventory[foundIndex].price);

    printf("Enter new price value: ");
    scanf("%f", &inventory[foundIndex].price);

    // Sync structural adjustments safely to disk file
    saveToFile();
    printf("\n[SUCCESS] Price metrics updated and synchronized to storage file.\n");
}
// Drops record entirely and shifts structural metrics inward to compress arrays safely
void deleteItem() {
    if (itemCount == 0) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[INFO] Deletion canceled. Database is empty.\n");
        return;
    }

    int targetId, foundIndex = -1;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\nEnter Item ID to permanently purge from logs: ");
    scanf("%d", &targetId);

    // Pinpoint address matching unique entry criteria
    for (int i = 0; i < itemCount; i++) {
        if (inventory[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("[ERROR] Item ID not found. No modifications executed.\n");
        return;
    }

    // Shift subsequent items back by 1 slot to keep array continuous without gaps
    for (int i = foundIndex; i < itemCount - 1; i++) {
        inventory[i] = inventory[i + 1];
    }

    // Decrement the record count indicator metrics globally
    itemCount--;

    // Overwrite old storage records to complete the deletion process on disk
    saveToFile();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[SUCCESS] Item removed. Local storage updated.\n");
}

// Writes continuous binary system blocks cleanly into local file architectures
void saveToFile() {
    // Open using Write Binary ("wb") settings which creates or overwrites clean targets
    FILE *file = fopen(FILE_NAME, "wb"); 
    if (file == NULL) {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n[CRITICAL ERROR] OS rejected storage write handle context assignment.\n");
        return;
    }

    // Block Step 1: Stamp current item array counts directly into file header
    fwrite(&itemCount, sizeof(int), 1, file);

    // Block Step 2: Stream structural payload arrays directly into sequential memory tracking
    fwrite(inventory, sizeof(Item), itemCount, file);

    fclose(file); // Relinquish locks on file handles immediately
}

// Re-reads structured memory logs on setup initialization
void loadFromFile() {
    // Open utilizing Read Binary ("rb") setup options
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        // Log safe bypass execution warning; means program execution loop runs inside virgin sandbox environment
        printf("[SYSTEM] Notice: No pre-existing storage files recovered. Constructing fresh database.\n");
        return;
    }
    // Read header count integer to scale incoming buffer expectations
    fread(&itemCount, sizeof(int), 1, file);

    // Hydrate runtime array matrices using streaming properties matching structure width specifications
    fread(inventory, sizeof(Item), itemCount, file);

    fclose(file);
    printf("[SYSTEM] Success: Recovered %d operational product files from storage.\n", itemCount);
}

// QuickSort Integer Comparator Execution Engine
    int compareById(const void *a, const void *b) {
    int idA = ((Item *)a)->id;
    int idB = ((Item *)b)->id;
    // Returns negative value if structure order satisfies parameters, otherwise positivereturn (idA - idB) sortDirection;
}

// QuickSort String Alphabetical Comparator Execution Engine
int compareByName(const void *a, const void *b) {
    // Utilizes native strcmp and flips outcomes against direction modifier tags
    return strcmp(((Item *)a)->name, ((Item *)b)->name) * sortDirection;
}

// QuickSort Floating Point Price Comparator Execution Engine
int compareByPrice(const void *a, const void *b) {
    float priceA = ((Item *)a)->price;
    float priceB = ((Item *)b)->price;

    if (priceA < priceB) return -1 * sortDirection;
    if (priceA > priceB) return 1 * sortDirection;
return 0;
}
