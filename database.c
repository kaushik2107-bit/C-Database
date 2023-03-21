#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>


int verifyDatabase(char name[]) {
    struct stat st = {0};
    char directory[] = "databases/";
    strcat(directory, name);
    int check;
    if (stat(directory, &st) == -1) {
        return 0;
    }
    return 1;
}

int verifyTable(char name[], char name1[]) {
    struct stat st = {0};
    char directory[] = "databases/";
    strcat(directory, name);
    strcat(directory, "/");
    strcat(directory, name1);
    strcat(directory, ".tab");
    if (stat(directory, &st) == -1) {
        return 0;
    }
    return 1;
}

void printDatabase(char arr[], char arr1[]) {
    printf("Database > ");
    if (verifyDatabase(arr)) {
        printf("%s > ", arr);
    }
    if (verifyTable(arr, arr1)) {
        printf("%s > ", arr1);
    }
}

void newLine() {printf("\n");}

void commandToLower(char arr[]) {
    for (int i=0; i<strlen(arr); i++) arr[i] = tolower(arr[i]);
}

int strStartsWith(char arr1[], char arr2[]) {
    int i = 0, j = 0;
    while (i < strlen(arr1) && j < strlen(arr2) && arr1[i] == arr2[j]) {i++; j++;}
    if (j == strlen(arr2)) return 1;
    return 0;
}

int verifyName(char arr[]) {
    for (int i=0; i<strlen(arr); i++) if (arr[i] == ' ' || arr[i] == ']' || arr[i] == '[' || arr[i] == ',') return 0;
    return 1;
}

void createDatabase(char name[]) {
    struct stat st = {0};
    char directory[] = "databases/";
    int check;
    if (stat(directory, &st) == -1) {
        check = mkdir(directory, 0700);
        if (check == -1) {
            printf("Unknown error occurred. Try again!\n");
        }
    }
    strcat(directory, name);
    if (stat(directory, &st) == -1) {
        check = mkdir(directory, 0700);
        if (check == -1) {
            printf("Unknown error occurred. Try again!\n");
        } else {
            printf("Database successfully created!\n");
        }
    } else {
        printf("Database already exists\n");
    }
}

void showDatabases() {
    struct dirent *files;
    DIR *dir = opendir("databases");
    if (dir == NULL){
        printf("Directory cannot be opened!" );
        return;
    }
    int length = 0;
    char stored[100][100];
    while ((files = readdir(dir)) != NULL) {
        if (files->d_type == DT_DIR)
            if (strcmp(files->d_name,"..")!=0 && strcmp(files->d_name, ".")!=0) {
                strcpy(stored[length], files->d_name);
                length++;
            }
    }
    newLine();
    printf("%10s\t%-20s\n", "No", "Database");
    for (int i=0; i < length; i++) {
        printf("%10d\t%-20s\n", i+1, stored[i]);
    }
    newLine();
    closedir(dir);
}

void showTables(char arr[]) {
    struct dirent *files;
    char tmp[40] = "databases/";
    strcat(tmp, arr);
    DIR *dir = opendir(tmp);
    if (dir == NULL) {
        printf("Directory cannot be opened!");
        return;
    }

    int length = 0;
    char stored[100][100];
    while ((files = readdir(dir)) != NULL) {
        if (files->d_type != DT_DIR)
            if (strcmp(files->d_name,"..")!=0 && strcmp(files->d_name, ".")!=0) {
                char tmp[50];
                strcpy(tmp, files->d_name);
                int len = strlen(tmp);
                tmp[len-4] = '\0';
                strcpy(stored[length], tmp);
                length++;
            }
    }
    newLine();
    printf("%10s\t%-20s\n", "No", "Database");
    for (int i=0; i < length; i++) {
        printf("%10d\t%-20s\n", i+1, stored[i]);
    }
    newLine();
    closedir(dir);
}

void createTable(char arr[], char database[]) {
    FILE *file;
    char tmptmp[500] = "databases/";
    strcat(tmptmp, database);
    strcat(tmptmp, "/");
    strcat(tmptmp, arr);
    strcat(tmptmp, ".tab");
    // printf("%s\n", tmptmp);
    file = fopen(tmptmp, "r");
    if (file != NULL) {
        printf("Table %s already exists", arr);
        newLine();
        return;
    }
    file = fopen(tmptmp, "a");
    if (file == NULL) {
        printf("Some error occurred");
        newLine();
        return;
    }
    int columns;
    printf("Enter number of columns : ");
    scanf("%d", &columns);
    while ((getchar()) != '\n');
    newLine();
    int copy = columns;
    char save[400] = "[";
    while (copy--) {
        printf("Enter name of column %d: ", columns - copy);
        char col_name[40];
        scanf("%[^\n]%*c", col_name);
        verifyName(col_name);
        strcat(save, col_name);
        strcat(save, ",");
    }
    strcat(save, "]");
    fprintf(file, "%s", save);
    printf("\nTable Created Successfully\n");
    fclose(file);
}

void insertData(char database[], char table[]) {
    FILE *file;
    char tmptmp[500] = "databases/";
    strcat(tmptmp, database);
    strcat(tmptmp, "/");
    strcat(tmptmp, table);
    strcat(tmptmp, ".tab");
    // printf("%s\n", tmptmp);
    file = fopen(tmptmp, "r");
    if (file == NULL) {
        printf("Table does not exist");
        newLine();
        return;
    }
    fclose(file);
    file = fopen(tmptmp, "ra");
    if (file == NULL) {
        printf("Some error occurred");
        newLine();
        return;
    }

    char columns[400];
    int i = 0;
    fscanf(file, "%[^\n]", columns);
    int length = 0;
    for (int i=0; i<strlen(columns); i++) {
        if (columns[i] == ',') length++;
    }

    int cnt = 0, cc = 0;
    char dcrypt[length][100];
    for (int i=1; i<strlen(columns)-1; i++) {
        if (columns[i] == ',') {
            dcrypt[cnt][cc] = '\0';
            cc = 0;
            cnt++;
        } else {
            dcrypt[cnt][cc] = columns[i];
            cc++;
        }
    }

    char value[400] = "[";
    newLine();
    for (int i=0; i<length; i++) {
        printf("Enter value of column %s: ", dcrypt[i]);
        char tmp[100];
        scanf("%[^\n]%*c", tmp);
        strcat(value, tmp);
        strcat(value, ",");
    }
    strcat(value, "]");
    newLine();
    fclose(file);
    file = fopen(tmptmp, "a");
    fputs("\n", file);
    fputs(value, file);
    printf("Data added successfully\n");

    fclose(file);

}


void selectData(char database[]) {
    newLine();
    char table[50];
    printf("Enter table name: ");
    scanf("%[^\n]%*c", table);
    if (!verifyName(table)) {
        printf("Enter a valid table name\n");
        newLine();
        return;
    }
    FILE *file;
    char tmptmp[500] = "databases/";
    strcat(tmptmp, database);
    strcat(tmptmp, "/");
    strcat(tmptmp, table);
    strcat(tmptmp, ".tab");
    // printf("%s\n", tmptmp);
    file = fopen(tmptmp, "r");
    if (file == NULL) {
        printf("Table does not exist");
        newLine();
        return;
    }
    fclose(file);
    file = fopen(tmptmp, "ra");
    if (file == NULL) {
        printf("Some error occurred");
        newLine();
        return;
    }

    while (!feof(file)) {
        char columns[400];
        int i = 0;
        fscanf(file, "%[^\n]\n", columns);
        int length = 0;
        for (int i=0; i<strlen(columns); i++) {
            if (columns[i] == ',') length++;
        }

        int cnt = 0, cc = 0;
        char dcrypt[length][100];
        for (int i=1; i<strlen(columns)-1; i++) {
            if (columns[i] == ',') {
                dcrypt[cnt][cc] = '\0';
                cc = 0;
                cnt++;
            } else {
                dcrypt[cnt][cc] = columns[i];
                cc++;
            }
        }

        // printf("%10s\t%-20s\n", "No", "Database");
        newLine();
        for (int i=0; i<length; i++) {
            printf("%10s", dcrypt[i]);
        }
    }
    newLine();
    newLine();
}

int main() {
    printf("Welcome to Database\n");
    printf("Type 'exit' to leave\n");
    printf("\n");
    printf("Available Commands\n");
    printf("1. create database\n");
    printf("2. select database\n");
    printf("3. show databases\n");
    printf("4. create table\n");
    printf("5. show tables\n");
    printf("6. select table\n");
    printf("7. insert data\n");
    printf("8. select data\n");
    printf("\n");

    char command[500];
    char currentDatabase[40];
    char currentTable[40];
    currentDatabase[39] = '\0';
    currentTable[39] = '\0';
    while(strcmp(command, "exit")) {
        printDatabase(currentDatabase, currentTable);
        scanf("%[^\n]%*c", command);
        commandToLower(command);
        
        if (strStartsWith(command, "create database;")) {
            char database[40];
            printDatabase(currentDatabase, currentTable);
            printf("Enter name of the database: ");
            scanf("%[^\n]%*c", database);
            int check = verifyName(database);
            if (check == 1) {
                createDatabase(database);
            } else {
                printf("Error in your database name. (No space is allowed)");
                newLine();
                continue;
            }
        } else if (strStartsWith(command, "select database;")) {
            char database[40];
            printDatabase(currentDatabase, currentTable);
            printf("Enter name of the database: ");
            scanf("%[^\n]%*c", database);
            int check = verifyName(database);
            if (check == 0) {
                printf("Error in your database name. (No spaces is allowed)");
                newLine();
                continue;
            }
            check = verifyDatabase(database);
            if (check == 0) {
                printf("Database do not exists");
                newLine();
                continue;
            }
            strcpy(currentDatabase, database);
            printf("Database selected\n");
        } else if (strStartsWith(command, "show databases;")) {
            showDatabases();
        } else if (strStartsWith(command, "create table;")) {
            if (!verifyDatabase(currentDatabase)) {
                printf("No database selected. To select database run 'select database;'");
                newLine();
                continue;
            }
            char tableName[40];
            printDatabase(currentDatabase, currentTable);
            printf("Enter name of the table: ");
            scanf("%[^\n]%*c", tableName);
            int check = verifyName(tableName);
            if (check == 0) {
                printf("Error in your table name. (No spaces is allowed)");
                newLine();
                continue;
            }
            createTable(tableName, currentDatabase);
        } else if (strStartsWith(command, "select table;")) {
            char table[40];
            printDatabase(currentDatabase, currentTable);
            printf("Enter name of the table: ");
            scanf("%[^\n]%*c", table);
            int check = verifyName(table);
            if (check == 0) {
                printf("Error in your table name. (No spaces is allowed)");
                newLine();
                continue;
            }
            check = verifyTable(currentDatabase, table);
            if (check == 0) {
                printf("Table do not exists");
                newLine();
                continue;
            }
            strcpy(currentTable, table);
            printf("Table selected\n");
        } else if (strStartsWith(command, "show tables;")) {
            if (!verifyDatabase(currentDatabase)) {
                printf("No database selected. To select database run 'select database;'");
                newLine();
                continue;
            }
            showTables(currentDatabase);
        } else if (strStartsWith(command, "insert data;")) {
            if (!verifyDatabase(currentDatabase)) {
                printf("No database selected. To select database run 'select database;'");
                newLine();
                continue;
            }
            if (!verifyTable(currentDatabase, currentTable)) {
                printf("No table selected. To select table run 'select table;'");
                newLine();
                continue;
            }
            insertData(currentDatabase, currentTable);
        } else if (strStartsWith(command, "select data;")) {
            if (!verifyDatabase(currentDatabase)) {
                printf("No database selected. To select database run 'select database;'");
                newLine();
                continue;
            }
            if (!verifyTable(currentDatabase, currentTable)) {
                printf("No table selected. To select table run 'select table;'");
                newLine();
                continue;
            }
            selectData(currentDatabase);
        }
    }
}