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
    printf("%s\n", tmptmp);
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


    int check;
    char code[10];
    int index = 0;
    printf("Select all columns(1/0): ");
    scanf("%d", &check);
    while ((getchar()) != '\n');

    char colcol[400];
    fscanf(file, "%[^\n]\n", colcol);
    fclose(file);
    file = fopen(tmptmp, "r");
    int ll = 0;
    for (int i=0; i<strlen(colcol); i++) {
        if (colcol[i] == ',') ll++;
    }
    char cols_name[ll][100];

    if (check == 0) {
        code[index] = '0'; index++;
        char col_details[400];
        printf("Name the columns comma separated without space: ");
        scanf("%s", col_details);
        while ((getchar()) != '\n');
        int cc = 0, cnt = 0;
        for (int i=0; i<strlen(col_details); i++) {
            if (col_details[i] == ',') {
                cols_name[cnt][cc] = '\0';
                cc = 0;
                cnt++;
            } else {
                cols_name[cnt][cc] = col_details[i];
                cc++;
            }
        }
    }
    else if (check == 1) {code[index] = '1'; index++;}

    code[index] = '\0';
    // printf("%s", code);
    if (!strcmp(code, "0")) {
        char colscols[400];
        fscanf(file, "%[^\n]\n", colscols);
        fclose(file);
        file = fopen(tmptmp, "r");
        int cntt = 0, ccc = 0;
        char cols_all[ll][100];
        for (int i=1; i<strlen(colscols)-1; i++) {
            if (colscols[i] == ',') {
                cols_all[cntt][ccc] = '\0';
                ccc = 0;
                cntt++;
            } else {
                cols_all[cntt][ccc] = colscols[i];
                ccc++;
            }
        }

        char col_code[40];
        int count = 0;

        for (int i=0; i<ll; i++) {
            int pt = 1;
            for (int j=0; j<ll; j++) {
                if (!strcmp(cols_all[i], cols_name[j])) {
                    col_code[count] = '1';
                    count++;
                    col_code[count] = '\0';
                    pt = 0;
                }
            }
            if (pt) {
                col_code[count] = '0';
                count++;
                col_code[count] = '\0';
            }
        }

        // printf("%s\n", col_code);

        while (!feof(file)) {
            char columns[400];
            int i = 0;
            fscanf(file, "%[^\n]\n", columns);
            int length = 0;
            for (int i=0; i<strlen(columns); i++) {
                if (columns[i] == ',') length++;
            }

            int cnt = 0, cc = 0, code_check = 0;
            char dcrypt[length][100];
            for (int i=1; i<strlen(columns)-1; i++) {
                int cp = 0;
                if (columns[i] == ',') {
                    dcrypt[cnt][cc] = '\0';
                    cc = 0;
                    cnt++;
                    code_check++;
                } else {
                    if (col_code[code_check] == '1') {
                        dcrypt[cnt][cc] = columns[i];
                        cc++;
                    }
                }
            }

            newLine();
            for (int i=0; i<length; i++) {
                printf("%20s", dcrypt[i]);
            }
        }
    } else if (!strcmp(code, "1")) {
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
                printf("%20s", dcrypt[i]);
            }
        }
    }
    newLine();
    newLine();
}

void countData(char database[]) {
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

    printf("Enter any conditions (e.g. and(equal('column', 'value'),lessthan('column', 'value')): ");
    char condition[400];
    scanf("%[^\n]%*c", condition);

    if (!strcmp(condition, "null")) {
        int length = -1;
        char tmp[100];
        while(!feof(file)) {
            fscanf(file, "%s", tmp);
            length++;
        }
        
        printf("%20s\n", "count(rows)");
        printf("%20d\n", length);

    } else {
        
    }

    newLine();
    newLine();
}

void runQueries(char database[]) {
    int commandcommand;
    printf("\n");
    printf("Available Commands\n");
    printf("1. Display count of patient\n");
    printf("2. Display the count of patient based on gender\n");
    printf("3. Display the age wise count of patient\n");
    printf("4. Display the count of patient areawise\n");
    printf("5. Admitted on same date\n");
    printf("6. Discharged on same date\n");
    printf("7. Patients treated under the doctor\n");
    printf("8. Patients treated under the male doctor\n");
    printf("9. Patients treated under the female doctor area wise\n");
    printf("\n");

    newLine();
    printf("Enter the number of the command to execute: ");
    scanf("%d", &commandcommand);
    while ((getchar()) != '\n');

    if (commandcommand == 1) {
        FILE *file;
        char tmptmp[500] = "databases/DBMS2/patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];
        int ccpc = -1;
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
            ccpc++;
            // printf("%10s\t%-20s\n", "No", "Database");
        }
        newLine();
        printf("The number of patients are: ");
        printf("%d", ccpc);
        newLine();

    } else if (commandcommand == 2) {
        FILE *file;
        char tmptmp[500] = "databases/DBMS2/patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];
        int ccpc1 = 0, ccpc2 = 0;
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

            for (int i=0; i<length; i++) {
                if (i%4 == 3) {
                    if (dcrypt[i][0] == 'M') {
                        ccpc1++;
                    } else if (dcrypt[i][0] == 'F') {
                        ccpc2++;
                    }
                }
            }

            
            // printf("%10s\t%-20s\n", "No", "Database");
        }
        newLine();
        printf("The number of Male patients: ");
        printf("%d", ccpc1);
        newLine();
        printf("The number of Female patients: ");
        printf("%d", ccpc2);
        newLine();
 
    } else if (commandcommand == 3) {
        int age[100] = {0};
        while ((getchar()) != '\n');

        FILE *file;
        char tmptmp[500] = "databases/DBMS2/patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];
        
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

            for (int i=0; i<length; i++) {
                if (i%4 == 2) {
                    int ageage = atoi(dcrypt[i]);
                    age[ageage]++;
                }
            }
            // printf("%10s\t%-20s\n", "No", "Database");
        }
        newLine();
        printf("\tAge\tValue\n");
        for (int i=0; i<100; i++) {
            if (i != 0 && age[i] !=0) {
                printf("\t%d -> ", i);
                printf("\t%d\n", age[i]);
            }
        }

    } else if (commandcommand == 4) {
        printf("%d", commandcommand);
    } else if (commandcommand == 5) {
        FILE *file;
        char tmptmp[500] = "databases/DBMS2/doctor-patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];

        char date[10];
        printf("Enter date in yyyy-mm-dd format: ");
        scanf("%s", date);
        while ((getchar()) != '\n');
        int chcceck = 1;
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

            if (chcceck) {
                for (int i=0; i<length; i++) {
                    chcceck = 0;
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }

            if (!strcmp(dcrypt[2], date)) {
                for (int i=0; i<length; i++) {
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }
            
            // printf("%10s\t%-20s\n", "No", "Database");
        }
    } else if (commandcommand == 6) {
        FILE *file;
        char tmptmp[500] = "databases/DBMS2/doctor-patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];

        char date[10];
        printf("Enter date in yyyy-mm-dd format: ");
        scanf("%s", date);
        while ((getchar()) != '\n');
        int chcceck = 1;
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

            if (chcceck) {
                for (int i=0; i<length; i++) {
                    chcceck = 0;
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }

            if (!strcmp(dcrypt[3], date)) {
                for (int i=0; i<length; i++) {
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }
            
            // printf("%10s\t%-20s\n", "No", "Database");
        }
    } else if (commandcommand == 7) {
        FILE *file;
        char tmptmp[500] = "databases/DBMS2/doctor-patient.tab";
        // printf("%s", tmptmp);
        file = fopen(tmptmp, "r");
        if (file == NULL) {
            printf("Table does not exist");
            newLine();
            return;
        }

        char colcol[400];
        fscanf(file, "%[^\n]\n", colcol);
        fclose(file);
        file = fopen(tmptmp, "r");
        int ll = 0;
        for (int i=0; i<strlen(colcol); i++) {
            if (colcol[i] == ',') ll++;
        }
        char cols_name[ll][100];

        char date[10];
        printf("Enter doctorId: ");
        scanf("%s", date);
        while ((getchar()) != '\n');
        int chcceck = 1;
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

            if (chcceck) {
                for (int i=0; i<length; i++) {
                    chcceck = 0;
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }

            if (!strcmp(dcrypt[0], date)) {
                for (int i=0; i<length; i++) {
                    printf("%20s", dcrypt[i]);
                }
                newLine();
            }
            
            // printf("%10s\t%-20s\n", "No", "Database");
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
    printf("9. run queries\n");
    printf("\n");

    char command[500];
    char currentDatabase[40];
    char currentTable[40];
    currentDatabase[39] = '\0';
    currentTable[39] = '\0';
    while(strcmp(command, "exit")) {
        printDatabase(currentDatabase, currentTable);
        fflush(stdin);
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
        } else if (strStartsWith(command, "count data;")) {
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
            countData(currentDatabase);
        } else if (strStartsWith(command, "run queries;")) {
            if (!verifyDatabase(currentDatabase)) {
                printf("No database selected. To select database run 'select database;'");
                newLine();
                continue;
            }
            runQueries(currentDatabase);
        }
    }
}