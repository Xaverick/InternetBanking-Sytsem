#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
FILE *fptr;
int stringcomp(char str1[], char str2[])
{
    int i = 0;
    for (int i = 0; i < 6; i++)
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
    }
    return 1;
}

struct date{
    int day;
    int month;
    int year;
}date;

struct statement{
    char cust_id[6];
    struct date dates;
    int withdarwal;
    int deposit;    
    int current_balance;
} statement;


struct login_credential{                       // Fields of the record file in sequence manner
    char cust_id[6];
    int password;
    long long int debit_card;
    char name[50];
    int balance;
    long long int account_no;
    int pin;

} login_credential;

void fordelay(int j)
{
    int k;
    for (int i = 0; i < j; i++){
        k = i;
    }    
}


char *find_cust_id(int debit_card)
{
    fptr = fopen("record.txt", "r");    
    while (fscanf(fptr, "%s %*d %d", &login_credential.cust_id, &login_credential.debit_card) != EOF)
    {
        if (debit_card == login_credential.debit_card)
        {
            fclose(fptr);
            return login_credential.cust_id;
        }
    }
    
}


char *account_no_to_cust_id(int account_no)
{
    fptr = fopen("record.txt", "r");
    while (fscanf(fptr, "%s %*d %*d %*s %*d %d", &login_credential.cust_id, &login_credential.account_no) != EOF)
    {
        if (account_no == login_credential.account_no)
        {
            fclose(fptr);
            return login_credential.cust_id;
        }
    }
    fclose(fptr);
}


int cust_check1(char cust_id[],int password)
{
    fptr = fopen("record.txt", "a+");
    int flag = 0;
    while (fscanf(fptr, "%s %d", &login_credential.cust_id, &login_credential.password) != EOF)
    {
        if (stringcomp(cust_id,login_credential.cust_id ) && password == login_credential.password)
        {
            flag = 1;
            break;
        }
    }
    fclose(fptr);
    return flag;
}


int cust_check2(int debit_card, int password)
{
    fptr = fopen("record.txt", "a+");
    int flag = 0;
    while (fscanf(fptr, "%*s %d %d", &login_credential.password ,&login_credential.debit_card) != EOF)
    {
        if(login_credential.debit_card == 0){
            return flag;
        }

        if (debit_card == login_credential.debit_card && password == login_credential.password)
        {
            flag = 1;
            break;
        }
    }
    fclose(fptr);
    return flag;
}


int acc_balance(char cust_id[]){
    fptr = fopen("record.txt", "r");
    int balance = 0;
    while (fscanf(fptr, "%s %*d %*d %*s %d", &login_credential.cust_id, &login_credential.balance) != EOF)
    {
        if (stringcomp(cust_id,login_credential.cust_id))
        {
            fclose(fptr);
            return login_credential.balance;
        }
    }    
    
}



int fund_transfer(char cust_id[] ,int account_no,int amount,struct date date){
    FILE *oldrec, *newrec;
    FILE *fp1 , *fp2;

    int balance1;
    int balance2;   

    char cust_id2[6]; 
    oldrec = fopen("record.txt", "r");

    char new[10];
    strcpy(new, cust_id);
    strcat(new, ".txt");
    fp1 = fopen(new, "a+");

    // fp2 = fopen(reciever, "a+");

    int flag = 0;
    int amountflag = 0;
    while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
    {

        if(account_no == login_credential.account_no){
            balance2 = login_credential.balance;                 //unUpdated balance
            strcpy(cust_id2, login_credential.cust_id);
            flag = 1;
            break;
        }
    }
    fclose(oldrec);

    oldrec = fopen("record.txt", "r");
    while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
    {
        if (stringcomp(cust_id, login_credential.cust_id))
        {
            if (amount > login_credential.balance)
            {
                printf("\t\t\t\tBALANCE IS LESS THAN THE AMOUNT\n");
                fclose(oldrec);
                return 2;
                break;
            }
            else
            {
                balance1 = login_credential.balance;
                amountflag = 1;
            }
        }

    }
    fclose(oldrec);

    if (flag == 0 || amountflag == 0){
        return 0;
    }

    oldrec = fopen("record.txt", "r");
    newrec = fopen("new.txt", "w");
    //updating balance for the customer_id guy
    if (flag == 1 && amountflag == 1)
    {

        int balance = balance1 - amount;
        fprintf(fp1, "\n%s %d/%d/%d %d %d %d", cust_id, date.day, date.month, date.year, amount, 0, balance);
        fclose(fp1);
        
        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {

            if (stringcomp(cust_id, login_credential.cust_id))
            {
                login_credential.balance -= amount;
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
            else{
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
        }
    
    }

    fclose(oldrec);
    fclose(newrec);
    remove("record.txt");
    rename("new.txt", "record.txt");

    // updating balance for who is receiving the money AND UPDATING OR CREATING A STATEMENT FILE

    char reciever[10];
    strcpy(reciever, cust_id2);
    strcat(reciever, ".txt");
    fp2 = fopen(reciever, "a+");

    oldrec = fopen("record.txt", "r");
    newrec = fopen("new.txt", "w");
    if (flag == 1 && amountflag == 1)
    {
        int balance = balance2 + amount;        
        fprintf(fp2,"\n%s %d/%d/%d %d %d %d", cust_id2, date.day, date.month, date.year, 0, amount, balance);
        fclose(fp2);

        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {

            if (login_credential.account_no == account_no)
            {
                login_credential.balance += amount;
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
            else
            {
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
        }
    }

    fclose(oldrec);
    fclose(newrec);                                                                
    remove("record.txt");
    rename("new.txt", "record.txt");
    return 1;
}


void statementprint(char cust_id[]){
    char new[10];
    strcpy(new, cust_id);
    strcat(new, ".txt");    
    fptr = fopen(new, "r");
    printf("\t\t\t\t DATE      WITHDRAWL    DEPOSIT     BALANCE\n");
    while (fscanf(fptr, "%s %d/%d/%d %d %d %d", &statement.cust_id, &statement.dates.day, &statement.dates.month, &statement.dates.year, &statement.withdarwal,&statement.deposit, &statement.current_balance) != EOF){      

        if (stringcomp(cust_id, statement.cust_id))
        {

            printf("\t\t\t\t%d/%d/%d      %d          %d          %d\n", statement.dates.day, statement.dates.month, statement.dates.year, statement.withdarwal, statement.deposit, statement.current_balance);
        }
    }
    fclose(fptr);
}



void card_function(char cust_id[]){
    FILE *oldrec, *newrec;
    int change;
    int flag = 0;
    oldrec = fopen("record.txt", "r");
    while (fscanf(fptr, "%s %d %*s %*d %d", &login_credential.cust_id, &login_credential.password, &login_credential.pin) != EOF){

        if (stringcomp(cust_id, login_credential.cust_id))
        {
            retry:
            printf("\t\t\t\tWhat will you like do you with your card \n");
            printf("\t\t\t\t[1]. Change the pin \n");
            printf("\t\t\t\t[2]. block the card \n");
            printf("\t\t\t\t[3]. return to main menu\n");
            int choice;
            printf("\t\t\t\tEnter the Choice ");
            scanf("%d", &choice);

            if(choice == 1){               
                int new_pin;
                int confirm_pin;
                printf("\t\t\t\tenter the new pin ");
                scanf("%d", &new_pin);

                if(!(new_pin >= 1000 && new_pin < 10000)){
                    printf("\t\t\t\tPin must be of 4 digit try again \n");
                    system("cls");
                    Sleep(500);
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    goto retry;
                }

                printf("\t\t\t\tconfirm PIN by Re entering the pin ");
                scanf("%d", &confirm_pin);

                if(new_pin == confirm_pin){
                    change = confirm_pin;
                    flag = 1;
                    printf("\t\t\t\tPIN changed succesfully\n");
                }
                else{
                    printf("\t\t\t\tthe pin does not match. please try again\n");
                    system("cls");
                    Sleep(500);
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    goto retry;
                }

            }

            else if(choice == 2){
                char ch;
                printf("\t\t\t\tDo you really want to block your card? Y/N ");
                scanf(" %c", &ch);

                if(ch == 'Y' || ch == 'y'){
                    start:
                    printf("\t\t\t\tEnter your Password ");
                    int password;
                    scanf("%d", &password);
                    if(password == login_credential.password){
                        change = 0;
                        flag = 2;
                        printf("\t\t\t\tYour Card has been successfully Blocked. Now you cannot use it anywhere \n");
                    }
                    else{
                        printf("\t\t\t\tpassword entered is wrong. please try again\n");
                        system("cls");
                        Sleep(500);
                        goto start;
                    }
                }

                else{
                    system("cls");
                    Sleep(500);
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    printf("\n");
                    goto retry;
                }
            }

            else{
                break;
            }            
        }
    }
    fclose(oldrec);


    if (flag == 1)
    {
        oldrec = fopen("record.txt", "r");
        newrec = fopen("new.txt", "w");
        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {
            if(stringcomp(cust_id, login_credential.cust_id)){
                login_credential.pin = change;
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
            else{
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
        }

        fclose(oldrec);
        fclose(newrec);
        remove("record.txt");
        rename("new.txt", "record.txt");
    }

    else if (flag == 2)
    {
        oldrec = fopen("record.txt", "r");
        newrec = fopen("new.txt", "w");
        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {
            if (stringcomp(cust_id, login_credential.cust_id))
            {
                login_credential.debit_card = change;
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
            else
            {
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
        }
        fclose(oldrec);
        fclose(newrec);
        remove("record.txt");
        rename("new.txt", "record.txt");
    }

}

void investRD_FD(char cust_id[]){
    FILE *oldrec, *newrec;
    FILE *fp1;

    char new[10];
    strcpy(new, cust_id);
    strcat(new, ".txt");

    fp1 = fopen(new, "a+");
    int choice;
    printf("\t\t\t\t[1]. Invest In FD \n");
    printf("\t\t\t\t[2]. To return to main menu \n");
    printf("\t\t\t\tEnter choice ");
    scanf("%d", &choice);
    int FD_amount;
    float FD_months;
    float interest_earned;
    int amountflag = 0;
    if( choice == 1){
        retry:
        printf("\t\t\t\tThe interest amount in FD is 4% \n");
        printf("\t\t\t\tEnter the amonut you want to invest. (minimum amount $1000): $");
        scanf("%d", &FD_amount);

        if(FD_amount < 1000){
            printf("\t\t\t\tMinimum amount is $1000");
            system("cls");
            goto retry;
        }

        oldrec = fopen("record.txt", "r");
        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {
            if (stringcomp(cust_id, login_credential.cust_id))
            {
                if (FD_amount > login_credential.balance)
                {
                    printf("\t\t\t\tBALANCE IS LESS THAN THE AMOUNT\n");
                    return ;
                    
                }
                else
                {                    
                    amountflag = 1;
                    break;
                }
            }
        }
        fclose(oldrec);

        printf("\t\t\t\tEnter the date in dd/mm/yyyy ");
        scanf("%d/%d/%d", &date.day, &date.month, &date.year);

        printf("\t\t\t\tEnter the time period in months: ");
        scanf("%f", &FD_months);

        float total = FD_amount + (FD_amount*4*FD_months)/1200;
        interest_earned = total - FD_amount;

        printf("\t\t\t\tYour Interest earned will be %f\n", interest_earned);
        printf("\t\t\t\tDo you want to invest? Y/N ");
        Sleep(1000);
        char ans;
        scanf(" %c", &ans);

        if((ans == 'Y' || ans == 'y') && amountflag == 1){

            oldrec = fopen("record.txt", "r");
            newrec = fopen("new.txt", "w");
            while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
            {
                if (stringcomp(cust_id, login_credential.cust_id))
                {
                    login_credential.balance -= FD_amount;           
                    fprintf(fp1, "\n%s %d/%d/%d %d %d %d", cust_id, date.day, date.month, date.year, FD_amount, 0, login_credential.balance);
                    fclose(fp1);
                    fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
                }
                else
                {
                    fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
                }
            }

            fclose(oldrec);
            fclose(newrec);
            remove("record.txt");
            rename("new.txt", "record.txt");
        }

    }

    else if( choice == 2){
        return ;
    }
    
}

void change_password(char cust_id[]){
    FILE *oldrec, *newrec;

    int new_pass;
    int confirm_pass;
    retry:
    printf("\t\t\t\tEnter the new password ");
    scanf("%d" , &new_pass);

    printf("\t\t\t\tConfirm the new password ");
    scanf("%d", &confirm_pass);

    if(new_pass == confirm_pass){

        oldrec = fopen("record.txt", "r");
        newrec = fopen("new.txt", "w");
        while (fscanf(oldrec, "%s %d %d %s %d %d %d", &login_credential.cust_id, &login_credential.password, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no, &login_credential.pin) != EOF)
        {
            if (stringcomp(cust_id, login_credential.cust_id))
            {

                login_credential.password = confirm_pass;
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
            else
            {
                fprintf(newrec, "%s %d %d %s %d %d %d\n", login_credential.cust_id, login_credential.password, login_credential.debit_card, login_credential.name, login_credential.balance, login_credential.account_no, login_credential.pin);
            }
        }

        fclose(oldrec);
        fclose(newrec);
        remove("record.txt");
        rename("new.txt", "record.txt");
        Sleep(1000);
        printf("\n\t\t\t\tPassword changed Successfully");
    }

    else{
        printf("\t\t\t\tPassword does not match. Please try again");
        system("cls");
        Sleep(500);
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        goto retry;
    }
}

void account_detail(char cust_id[]){
    fptr = fopen("record.txt", "r");
    while (fscanf(fptr, "%s %*d %d %s %d %d", &login_credential.cust_id, &login_credential.debit_card, &login_credential.name, &login_credential.balance, &login_credential.account_no) != EOF)
    {
        if (stringcomp(cust_id, login_credential.cust_id))
        {
            printf("\t\t\t\tYour Customer Id is:   %s\n", login_credential.cust_id);
            printf("\t\t\t\tYour Debit Card no is:   %d\n", login_credential.debit_card);
            printf("\t\t\t\tYour Name is:  %s\n", login_credential.name);
            printf("\t\t\t\tYour Balance is:   $%d\n", login_credential.balance);
            printf("\t\t\t\tYour Account No is:   %d\n", login_credential.account_no);
        }
    }
    fclose(fptr);

}


void mainbody(char cust_id[])                  //using after successful login of the customer
{
    int flag = 1;
    while(flag){
        start:
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");        
        printf("\t\t\t\tCHOOSE A OPTION\n");
        printf("\n");
        printf("\n");
        printf("\t\t\t\t[1]. CHECK BALANCE\n");                     //DONE
        printf("\t\t\t\t[2]. FUND TRANSFER\n");                     //DONE
        printf("\t\t\t\t[3]. VIEW YOUR BANK STATEMENT\n");          //DONE
        printf("\t\t\t\t[4]. INVEST IN FD\n");                      //DONE                        
        printf("\t\t\t\t[5]. MANAGE YOUR DEBIT/CREDIT CARDS\n");    //DONE
        printf("\t\t\t\t[6]. VIEW ACCOUNT DETAILS\n");              //DONE
        printf("\t\t\t\t[7]. CHANGE PASSWORD\n");                   //DONE
        printf("\t\t\t\t[8]. EXIT\n");                              //DONE

        int choice;
        printf("\t\t\t\tEnter Your Choice ");
        scanf("%d", &choice);

        if(choice == 1){
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\t\t\t\tYour Balance is $%d\n", acc_balance(cust_id));
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if(choice == 1){
                system("cls");
                goto start;
            }
            else{
                break;
            }
        }

        else if(choice == 2){
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            long long int account_no;
            printf("\t\t\t\tEnter the Account No ");
            scanf("%d", &account_no);

            int amount;
            printf("\t\t\t\tEnter the amount ");
            scanf("%d", &amount);

            printf("\t\t\t\tEnter the date in dd/mm/yyyy ");
            scanf("%d/%d/%d", &date.day, &date.month, &date.year);

            int flag = fund_transfer(cust_id, account_no,amount,date);
            Sleep(1000);
            if(flag == 1){
                printf("\t\t\t\tFUND TRANSFER SUCCESSFUL\n");
            }
            else if(flag == 0){
                printf("\t\t\t\tInvalid Account No. Please try again\n");
            }

            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }
        }


        else if(choice == 3){
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            statementprint(cust_id);
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }
        }

        else if( choice == 4){

            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            investRD_FD(cust_id);
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }
        }


        else if(choice == 5){
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            card_function(cust_id);
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }

        }


        else if(choice == 6)
        {
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            account_detail(cust_id);
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }
        }

        else if (choice == 7)
        {
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            change_password(cust_id);
            printf("\n");
            printf("\t\t\t\tEnter 1 to return to menu or 0 to exit ");
            scanf("%d", &choice);
            if (choice == 1)
            {
                system("cls");
                Sleep(500);
                goto start;
            }
            else
            {
                system("cls");
                Sleep(500);
                break;
            }
        }

        else if (choice == 8){
            system("cls");
            Sleep(1000);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\t\t\t\tThank You For Banking With Us \n");
            system("pause");
            Sleep(1000);
            system("cls");
            Sleep(1000);
            break;
        }

        else{
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\t\t\t\tEnter a Valid choice ");
            Sleep(1000);
            system("cls");
            goto start;
        }
    }
}


int main(){    
    start:
    system("color 71");
    int password, debit_card;
    char cust_id[6];
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf(" \t\t\t\t\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2  WELCOME TO INTERNET BANKING SYSTEM  \xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\tHow would you like to login \n");
    printf("\n");
    printf("\t\t\t\tChoice [1]. Using Customer ID And Password \n");
    printf("\t\t\t\tChoice [2]. Using Debit Card And Password \n");
    printf("\t\t\t\tchoice [3]. To exit \n");
    printf("\n");
    printf("\n");
    int choice;
    while(1){
        printf("\t\t\t\tEnter Choice ");
        scanf("%d", &choice);
        if (choice == 1){
            system("cls");
            Sleep(500);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\t\t\t\tEnter the customer id ");
            scanf("%s", &cust_id);
            printf("\t\t\t\tEnter the password ");
            scanf("%d", &password);

            
            if (cust_check1(cust_id,password))
            {
                printf("\n\n\t\t\t\tDeatils Matched!\n\t\t\t\tLOADING");
                for (int i = 0; i <= 6; i++)
                {
                    fordelay(100000000);
                    printf(". ");
                }
                printf("\n");
                system("pause");
                printf("\t\t\t\tWelcome to internet banking. login succsseful\n");
                system("cls");
                mainbody(cust_id);
            }
            else
            {
                printf("\t\t\t\tInvalid Login Credential Please try again\a\a\a");
                Sleep(2000);
                system("cls");
            }
            
        }

        else if(choice == 2){
            system("cls");
            Sleep(500);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\t\t\t\tEnter the Debit Card Number ");
            scanf("%d", &debit_card);
            printf("\t\t\t\tEnter the password ");
            scanf("%d", &password);
            
            if (cust_check2(debit_card, password))
            {
                printf("\n\n\t\t\t\tDeatils Matched!\n\t\t\t\tLOADING");
                for (int i = 0; i <= 6; i++)
                {
                    fordelay(100000000);
                    printf(". ");
                }
                printf("\n");
                system("pause");
                printf("\t\t\t\tWelcome to internet banking. login succsseful\n");
                system("cls");
                //finding customer id of the person logged in with debit card            
                mainbody(find_cust_id(debit_card));
            }
            else
            {
                printf("\t\t\t\tInvalid Login Credential Please try again");
                Sleep(2000);
                system("cls");
            }
        }

        else if(choice == 3){
            break;
        }

        else{
            printf("\t\t\t\tEnter a valid choice");
            Sleep(1000);
            system("cls");
        }
        goto start;

    }

}




