/***********************************************************************************
 Name: Josiah Varghese
 StudentID: 1043468
 Date: October 22, 2022
 Assignment Name: Stack
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Car Car;
typedef struct List List;

typedef enum
{
    false,
    true
} bool;

struct Car
{
    char *plate;
    int mileage;
    int return_date;
    Car *next;
};

struct List
{
    Car *head;
    Car *tail;
    int size;
};

// lists
List *available;
List *rented;
List *repair;

double total_income = 0;

// decalration functions
void init();
void print_menu();
Car *create_car(char *, int, int);
void add_car_available(Car *);
void add_returned_car(Car *, List *, int);
void add_car_rented(Car *);
void add_car_repair(struct Car *);
int check_duplicate(char *);
void transfer_car_available(Car *);
Car *rent_car();
void print_list(List *);
void read_data();
void write_data();

// initalizes the list structure
void init()
{
    available = (List *)malloc(sizeof(List));
    available->head = NULL;
    available->tail = NULL;
    available->size = 0;

    rented = (List *)malloc(sizeof(List));
    rented->head = NULL;
    rented->tail = NULL;
    rented->size = 0;

    repair = (List *)malloc(sizeof(List));
    repair->head = NULL;
    repair->tail = NULL;
    repair->size = 0;
}

// prints the menu to the user
void print_menu()
{
    printf("Please select one of the following options: \n");
    printf("1: Add a new car to the available-for-rent list.\n");
    printf("2: Add a returned car to the available-for-rent list.\n");
    printf("3: Add a returned car to the repair list.\n");
    printf("4: Transfer a car from the repair list to the available-for-rent list.\n");
    printf("5: Rent the first available car.\n");
    printf("6: Print all the lists.\n");
    printf("7: Quit.\n");
}

// creates a car struct and initializes the values
Car *create_car(char *plate, int mileage, int return_date)
{
    Car *car = (Car *)malloc(sizeof(Car));
    car->plate = (char *)malloc(sizeof(char) * 8 + 1);
    strcpy(car->plate, plate);
    car->mileage = mileage;
    car->return_date = return_date;
    car->next = NULL;

    return car;
}

// adds a car to the available for rent list
void add_car_available(Car *car)
{
    if (available->head == NULL)
    {
        available->head = car;
        available->tail = car;
    }
    else
    {
        Car *current = available->head;
        if (current->mileage > car->mileage)
        {
            car->next = current;
            available->head = car;
        }
        else
        {
            while (current->next != NULL && current->next->mileage < car->mileage)
            {
                current = current->next;
            }
            car->next = current->next;
            current->next = car;
        }
    }
    available->size++;
}

// adds returned car to the repair or available for rent  list
void add_returned_car(Car *car, List *list, int type)
{
    Car *current = list->head;
    Car *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->plate, car->plate) == 0)
        {
            car->return_date = current->return_date;
            double charge = 80;
            if (car->mileage - current->mileage > 200)
            {
                charge += (double)((car->mileage - current->mileage) - 200) * 0.15;
            }
            total_income += charge;
            printf("Charge: $%.2lf\n", charge);
            break;
        }
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("No such car found.\n");
        return;
    }
    if (type == 2)
    {
        if (current == rented->head)
        {
            rented->head = current->next;
        }
        else
        {
            prev->next = current->next;
        }
        printf("Returning Car to Available List from Rented List\n\n");
        add_car_available(car);
    }
    if (type == 3)
    {
        if (current == repair->head)
        {
            repair->head = current->next;
        }
        else
        {
            prev->next = current->next;
        }
        printf("Added Car to Repair List from Rented List\n\n");
        add_car_repair(car);
    }
    rented->size--;
    free(current);
}

// adds a car to the rented list
void add_car_rented(Car *car)
{
    if (rented->head == NULL)
    {
        rented->head = car;
        rented->tail = car;
    }
    else
    {
        Car *current = rented->head;
        if (current->return_date > car->return_date)
        {
            car->next = current;
            rented->head = car;
        }
        else
        {
            while (current->next != NULL && current->next->return_date < car->return_date)
            {
                current = current->next;
            }
            car->next = current->next;
            current->next = car;
        }
    }
    rented->size++;
}

// adds car to repair list
void add_car_repair(Car *car)
{
    if (repair->head == NULL)
    {
        repair->head = car;
        repair->tail = car;
    }
    else
    {
        Car *current = repair->head;
        if (current->plate > car->plate)
        {
            car->next = current;
            repair->head = car;
        }
        else
        {
            while (current->next != NULL && current->next->plate < car->plate)
            {
                current = current->next;
            }
            car->next = current->next;
            current->next = car;
        }
    }
    repair->size++;
}

// checks if plate number has duplicates in all lists
int check_duplicate(char *plate)
{
    int i, j, z;
    Car *a = available->head;
    Car *b = repair->head;
    Car *c = rented->head;
    if (available->head == NULL && repair->head == NULL && rented->head == NULL)
    {
        return 1;
    }
    for (i = 0; i < available->size; i++)
    {
        if (strcmp(a->plate, plate) == 0)
        {
            printf("Duplicate found on Available List\n");
            return 0;
        }
        a = a->next;
    }

    for (j = 0; j < repair->size; j++)
    {
        if (strcmp(b->plate, plate) == 0)
        {
            printf("Duplicate found on Repair List\n");
            return 0;
        }
        b = b->next;
    }

    for (z = 0; z < rented->size; z++)
    {
        if (strcmp(c->plate, plate) == 0)
        {
            printf("Duplicate found on Rented List\n");
            return 0;
        }
        c = c->next;
    }

    return 1;
}

// transfers a car from the repair list to the available for rent list
void transfer_car_available(Car *car)
{
    Car *current = repair->head;
    Car *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->plate, car->plate) == 0)
        {
            car->mileage = current->mileage;
            car->return_date = current->return_date;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("No such car found.\n");
        return;
    }

    if (current == repair->head)
    {
        repair->head = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    free(current);
    repair->size--;
    printf("Transfering Car to Avaible List from Repair List\n\n");
    add_car_available(car);
}

// rents the first available to rent car
Car *rent_car()
{
    if (available->head == NULL)
    {
        return NULL;
    }
    else
    {
        Car *car = available->head;
        available->head = available->head->next;
        car->next = NULL;
        printf("Renting Car from Available List\n\n");
        add_car_rented(car);
        available->size--;
        return car;
    }
}

// prints the list
void print_list(List *list)
{
    Car *current = list->head;
    while (current != NULL)
    {
        printf("Plate: %s\nMileage: %d\nReturn date: %d\n\n", current->plate, current->mileage, current->return_date);
        current = current->next;
    }
}

// reads data from disk files
void read_data()
{
    FILE *file1 = fopen("available.txt", "r");
    if (file1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    char *plate = (char *)malloc(sizeof(char) * 8 + 1);
    int mileage, return_date, check;

    while (fscanf(file1, "%s\n %d\n %d\n\n", plate, &mileage, &return_date) == 3)
    {
        check = check_duplicate(plate);
        if (check == 0)
        {
            printf("Error on Disk file\n");
            exit(1);
        }
        else
        {
            add_car_available(create_car(plate, mileage, return_date));
        }
    }
    fclose(file1);

    FILE *file2 = fopen("repair.txt", "r");
    if (file2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }
    while (fscanf(file2, "%s\n %d\n %d\n\n", plate, &mileage, &return_date) == 3)
    {
        check = check_duplicate(plate);
        if (check == 0)
        {
            printf("Error on Disk file\n");
            exit(1);
        }
        add_car_repair(create_car(plate, mileage, return_date));
    }
    fclose(file2);

    FILE *file3 = fopen("rented.txt", "r");
    if (file3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }
    while (fscanf(file3, "%s\n %d\n %d\n\n", plate, &mileage, &return_date) == 3)
    {
        check = check_duplicate(plate);
        if (check == 0)
        {
            printf("Error on Disk file\n");
            exit(1);
        }

        add_car_rented(create_car(plate, mileage, return_date));
    }
    fclose(file3);
}

// writes data to disk files
void write_data()
{
    FILE *file1 = fopen("available.txt", "w");
    if (file1 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    Car *current = available->head;
    while (current != NULL)
    {
        fprintf(file1, "%s\n %d\n %d\n\n", current->plate, current->mileage, current->return_date);
        current = current->next;
    }
    fclose(file1);

    FILE *file2 = fopen("repair.txt", "w");
    if (file2 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }
    current = repair->head;
    while (current != NULL)
    {
        fprintf(file2, "%s\n %d\n %d\n\n", current->plate, current->mileage, current->return_date);
        current = current->next;
    }
    fclose(file2);

    FILE *file3 = fopen("rented.txt", "w");
    if (file3 == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    current = rented->head;
    while (current != NULL)
    {
        fprintf(file3, "%s\n %d\n %d\n\n", current->plate, current->mileage, current->return_date);
        current = current->next;
    }
    fclose(file3);
}

int main()
{
    init();
    read_data();
    int transaction_code;
    char *plate = (char *)malloc(sizeof(char) * 8 + 1);
    int mileage, return_date;
    bool quit = false;
    while (!quit)
    {
        print_menu();
        printf("\nEnter transaction code: ");
        scanf("%d", &transaction_code);
        switch (transaction_code)
        {
        case 1:
            printf("Enter plate number: ");
            scanf("%s", plate);
            if (check_duplicate(plate) == 0)
            {
                printf("Duplicate plate in files");
                break;
            }
            printf("Enter mileage: ");
            scanf("%d", &mileage);
            printf("Enter return date: ");
            scanf("%d", &return_date);
            add_car_available(create_car(plate, mileage, return_date));
            printf("Added Car to Available List\n\n");

            break;
        case 2:
            if (rented->head == NULL)
            {
                printf("No rented cars\n");
            }
            else
            {
                printf("Enter plate number: ");
                scanf("%s", plate);
                printf("Enter mileage: ");
                scanf("%d", &mileage);

                add_returned_car(create_car(plate, mileage, return_date), rented, 2);
                printf("Added Car to Available List\n\n");
            }
            break;
        case 3:
            if (rented->head == NULL)
            {
                printf("No rented cars\n");
            }
            else
            {
                printf("Enter plate number: ");
                scanf("%s", plate);
                printf("Enter mileage: ");
                scanf("%d", &mileage);

                add_returned_car(create_car(plate, mileage, return_date), rented, 3);
                printf("Added Car to Repair List\n\n");
            }
            break;
        case 4:
            if (repair->head == NULL)
            {
                printf("No cars in repair\n");
            }
            else
            {
                printf("Enter plate number: ");
                scanf("%s", plate);

                transfer_car_available(create_car(plate, mileage, return_date));
                printf("Added Car to Repair List\n\n");
            }
            break;
        case 5:
            if (available->head == NULL)
            {
                printf("No available cars\n");
            }
            else
            {
                Car *car = rent_car();
                printf("Expected return date: ");
                scanf("%d", &car->return_date);
                printf("Added Car to Rented List\n\n");
            }
            break;
        case 6:
            printf("Available cars\n*-------------*\n");
            print_list(available);
            printf("Rented cars\n*-------------*\n");
            print_list(rented);
            printf("Cars in repair\n*-------------*\n");
            print_list(repair);
            break;
        case 7:
            quit = true;
            break;
        default:
            break;
        }
    }
    printf("Total income: $%.2lf\n", total_income);
    write_data();
}