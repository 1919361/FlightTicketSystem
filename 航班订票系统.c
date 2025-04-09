#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include<conio.h>
#include <ctype.h>
#include <cstdio>


#define SIZE 40
#define NUM 150
#define FILENAME "./flight/password.txt"
#define MAX_PASSWORD_LENGTH 50
#define MAX_ORDER_SIZE 30
#pragma pack(push, 1)
typedef struct{
    char flightNum[SIZE];
    char take_off_Time[SIZE];
    char land_Time[SIZE];
    char take_off_City[SIZE];
    char land_City[SIZE];
    float pre_Price;
    float discount;
    float now_Price;
    int tickets;
}FLIGHT;
#pragma pack(pop)

typedef struct{
    char orderId[MAX_ORDER_SIZE];
    char name[SIZE];
    char userId[SIZE];
    struct {
        char flightNum[SIZE];
        char take_off_Time[SIZE];
        char land_Time[SIZE];
        char take_off_City[SIZE];
        char land_City[SIZE];
        float price;
    } order;
}ORDER;

typedef struct {
    char username[SIZE];
    char name[SIZE];       // ���������ֶ�
    char password[MAX_PASSWORD_LENGTH];
    char userId[SIZE];
    char gender[10];       // �����Ա��ֶ�
    char phone[12];      // �����ֻ������ֶ�
} USER, *PUSER;

char username_Global[SIZE];//ȫ���û���
// �����username_Global��������
char userID_Global[SIZE]; //ȫ���û�ID
char userRealname_Global[SIZE];//ȫ���û�����
char userPhone_Global[SIZE];//ȫ���û��ֻ���
char userGender_Global[SIZE];//ȫ���û��Ա�
int UsefulPayment_Global = 0;

void input_flight();

void user_register();
int user_login();
int verify_password();
char* Getpassword();
void change_password();
void user_change_password();
void custom();
void list_all_users();  // �û��б�������
int main();
void add_order(int showCheck);
void order();
int validateIDCard(const char *id);
void add_order_for_others(const char* name, const char* userId, const char* flightNum);

void PaymentSystem(float price,int num){
    float totalPrice = price * num;
    if(totalPrice == 0) {
        printf("��Ч֧����\n");
        system("pause");
        return;	
    }
    printf("�ϼƣ�%.2f Ԫ\n", totalPrice);
    printf("1. ΢��֧��\n");
    printf("2. ֧����֧��\n");
    printf("3. ���п�֧��\n");
    printf("��ѡ��֧����ʽ:\n");
    int choice;
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("����ѡ��΢��֧����֧�����Ϊ %.2f Ԫ\n", totalPrice);
            break; 	
        case 2:
            printf("����ѡ��֧����֧����֧�����Ϊ %.2f Ԫ\n", totalPrice);
            break;
        case 3:
            printf("����ѡ�����п�֧����֧�����Ϊ %.2f Ԫ\n", totalPrice);
            break;
        default:
            printf("��Ч��֧����ʽ��\n");
            break;
    }
    system("pause");
    return;
}
void input_flight(){
    FILE* fp;
    int i=0;
    int n;
    FLIGHT f[NUM];
    
    // �޸��ļ���ģʽ���ϲ�Ϊ���δ򿪲�����
    if((fp=fopen("./flight/flight.csv","a+"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ���˵�!\n");
        return;
    }
    
    // �޸��ļ�ͷ����߼�
    fseek(fp, 0, SEEK_SET);
    char first_line[256];
    if(!fgets(first_line, sizeof(first_line), fp)) { // �ļ�Ϊ��ʱд���ͷ
        fprintf(fp, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
    }
    fseek(fp, 0, SEEK_END);  // ���õ��ļ�ĩβ׼��׷��

    printf("������Ԥ��Ҫ¼��ĺ�����: ");
    if(scanf("%d",&n) != 1 || n <= 0) {  // ������֤��ǿ
        printf("������Ч���˳�ѡ�");
        fclose(fp);
        system("pause");
        return;
    }

    // �޸����뻺��������
    while(getchar() != '\n');  // ������뻺����

    printf("�밴��˳��(����� ���ʱ�� ����ʱ�� ��ɻ��� ������� ԭƱ�� �ۿ� ʣ��Ʊ��)��������...\n");
    printf("ʱ���ʽΪ01-01/00:00\n");
    while (i < n) {
        // ���������֤
        int read_count = scanf("%s%s%s%s%s%f%f%d", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].tickets);
        
        if(read_count != 8) {  // �������������
            printf("�����ʽ�������������뱾�����ݣ�\n");
            while(getchar() != '\n');  // ��մ�������
            continue;
        }
        
        // �޸���ֵ��Χ��֤
        if(f[i].discount < 0.1f || f[i].discount > 10.0f) {
            printf("�ۿ���Ӧ��0.1-10.0֮�䣡\n");
            continue;
        }
        
        f[i].now_Price = f[i].pre_Price * f[i].discount;
        
        // �޸��ļ�д���ʽ
        fprintf(fp, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, f[i].pre_Price, f[i].discount, f[i].now_Price, f[i].tickets);
        i++;
        
        if(i >= n) {
            // �޸����������߼�
            printf("[��ǰ��¼��%d��] �Ƿ����¼�룿(��������/0�˳�): ", i);
            int x;
            if(scanf("%d", &x) == 1 && x > 0) {
                n += x;
            } else {
                break;
            }
        }
    }
    
    fclose(fp);
    printf("������Ϣ¼�����!\n");
    system("pause");
}

void check_flight(){  //¼�뺽��
    FLIGHT f;
    FILE* fp;
    char header[256];
    printf("���к������Ϣ������ʾ:\n");
    if((fp=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    fgets(header, sizeof(header), fp); // ����������
    printf("%-8s %-12s %-12s %-25s %-25s %-10s %-5s %-10s %-5s\n","�����","���ʱ��","����ʱ��","��ɻ���","�������","ԭ��","�ۿ�","�ּ�","��Ʊ");
    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f.flightNum, f.take_off_Time, f.land_Time, f.take_off_City, f.land_City, &f.pre_Price, &f.discount, &f.now_Price, &f.tickets) != EOF) {
        printf("%-8s %-12s %-12s %-25s %-25s %-10.2f %-5.2f %-10.2f %-5d\n", f.flightNum, f.take_off_Time, f.land_Time,f.take_off_City, f.land_City,f.pre_Price,f.discount,f.now_Price,f.tickets);
    }
    fclose(fp);
	system("pause");
}

void change_flight(){  //�޸ĺ�����Ϣ
    FLIGHT f[NUM];
    ORDER o[NUM];
    FILE* fp1;
    FILE* fp2;
    FILE* op1;
    FILE* op2; 
    char str[SIZE];
    char str1[SIZE];
    int i=0;
    int a=0;
    int x = -1;
    int flag=0;

    // ��ȡ����CSV
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);

    // ��ȡ����CSV
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[a].orderId, o[a].name, o[a].userId, o[a].order.flightNum, o[a].order.take_off_Time, o[a].order.land_Time, o[a].order.take_off_City, o[a].order.land_City, &o[a].order.price) != EOF) {
        a++;
    }
    fclose(op1);

    check_flight();
    printf("��������Ҫ�޸ĺ���ĺ����: ");
    scanf("%s",str);

    for(int j=0;j<i;j++){
        if(strcmp(str,f[j].flightNum)==0){
            printf("���ҵ��ú���:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n",
                "�����","���ʱ��","����ʱ��","��ɻ���","�������","ԭ��","�ۿ�","�ּ�","��Ʊ");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", 
                f[j].flightNum, f[j].take_off_Time, f[j].land_Time,
                f[j].take_off_City, f[j].land_City,
                f[j].pre_Price,f[j].discount,f[j].now_Price,f[j].tickets);
            
            printf("����������ú������Ϣ;\n�밴��(���ʱ�� ����ʱ�� ��ɻ��� ������� ԭƱ�� �ۿ� ʣ��Ʊ��)˳������������Ӧ��Ϣ;\n������ɺͽ���ʱ���밴������\"2020-01-01/12:45\"����ʽ����,�����ۿ�������1.0:\n");
            scanf("%s%s%s%s%f%f%d", f[j].take_off_Time,f[j].land_Time, f[j].take_off_City,f[j].land_City, &f[j].pre_Price,&f[j].discount,&f[j].tickets);
            f[j].now_Price=f[j].pre_Price*f[j].discount;
            x=j;
            flag=1;
        }
    }

    if (flag == 0) { 
        printf("����Ҫ�޸ĵĺ��ಢ������!\n"); 
        system("pause");
        return;
    }

    // ���º���CSV
    if((fp2=fopen("./flight/flight.csv","w"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    fprintf(fp2, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
    for(int j=0;j<i;j++){
        fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[j].flightNum, f[j].take_off_Time, f[j].land_Time, f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
    }
    fclose(fp2);

    // ���¶���CSV
    if((op2=fopen("./flight/order.csv","w"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    fprintf(op2, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
    for(int b=0;b<a;b++){
        if(strcmp(str,o[b].order.flightNum)==0){
            strcpy(o[b].order.take_off_Time,f[x].take_off_Time);
            strcpy(o[b].order.land_Time,f[x].land_Time);
            strcpy(o[b].order.take_off_City,f[x].take_off_City);
            strcpy(o[b].order.land_City,f[x].land_City);
            snprintf(o[b].orderId, MAX_ORDER_SIZE, "%s%s", f[x].flightNum, o[b].userId);
            o[b].order.price=f[x].now_Price;
        }
        fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[b].orderId, o[b].name, o[b].userId, o[b].order.flightNum, o[b].order.take_off_Time, o[b].order.land_Time, o[b].order.take_off_City, o[b].order.land_City, o[b].order.price);
    }
    fclose(op2);

    printf("\n�޸�����ɣ�\n");
    system("pause");
}

void delete_flight(){  //ɾ��������Ϣ
    FLIGHT f[NUM];
    ORDER o[NUM];
    FILE* fp1;
    FILE* fp2;
    FILE* op1;
    FILE* op2;
    char str[SIZE];
    int i=0, a=0, flag=0, c=0;
    
    // �޸�ΪCSV��ȡģʽ
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    
    // ����CSV������
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    
    // ��ȡ��������
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);

    // ��ȡ�������ݣ�CSV��ʽ��
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n",o[a].orderId, o[a].name, o[a].userId, o[a].order.flightNum,o[a].order.take_off_Time, o[a].order.land_Time,o[a].order.take_off_City, o[a].order.land_City, &o[a].order.price) != EOF) {
        a++;
    }
    fclose(op1);

    check_flight();
    printf("������Ҫɾ���ĺ����: ");
    scanf("%s", str);
    
    // ���Һ��ಢȷ��
    int x = -1;
    for (int k = 0; k < i; k++) {
        if (strcmp(str, f[k].flightNum) == 0) {
            x = k;
            printf("�ҵ�����:\n%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", f[k].flightNum, f[k].take_off_Time, f[k].land_Time,f[k].take_off_City, f[k].land_City, f[k].pre_Price,f[k].discount, f[k].now_Price, f[k].tickets);
                
            printf("ȷ��ɾ���ú�����(1=ȷ��/0=ȡ��): ");
            scanf("%d", &c);
            break;
        }
    }

    if (x == -1) {
        printf("δ�ҵ�����: %s\n", str);
        system("pause");
        return;
    }

    // ���º���CSV�ļ�
    if(c==1){
        if((fp2=fopen("./flight/flight.csv","w"))==NULL){
            printf("�޷����ļ�!ϵͳ��������һ��!\n");
            return;
        }
        // д��CSV����
        fprintf(fp2, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
        // д��ʣ�ຽ��
        for(int k=0; k<i; k++){
            if(k != x){
                fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n",f[k].flightNum, f[k].take_off_Time, f[k].land_Time,f[k].take_off_City, f[k].land_City, f[k].pre_Price,f[k].discount, f[k].now_Price, f[k].tickets);
            }
        }
        fclose(fp2);
    }

    // ���¶���CSV�ļ�
    if(c==1){
        if((op2=fopen("./flight/order.csv","w"))==NULL){
            printf("�޷����ļ�!ϵͳ��������һ��!\n");
            return;
        }
        // д��CSV����
        fprintf(op2, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
        // д��ʣ�ඩ��
        for(int k=0; k<a; k++){
            if(strcmp(str, o[k].order.flightNum) != 0){
                fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n",o[k].orderId, o[k].name, o[k].userId, o[k].order.flightNum,o[k].order.take_off_Time, o[k].order.land_Time,o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            }
        }
        fclose(op2);
    }
}

void searchByFligtNum(){  //�����������
    FLIGHT f[NUM];
    char str[SIZE];
    FILE* fp;
    int i = 0;
    int flag = 0;
    int n;
    float price;
    
    printf("����������Ҫ��ѯ�ĺ����: ");
    scanf("%s", str);
    
    // �޸�ΪCSV�ļ���ȡ
    if ((fp = fopen("./flight/flight.csv", "r")) == NULL) {
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    
    // ����CSV������
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // �޸�ΪCSV��ʽ����
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // ����ԭ�в�ѯ�߼�
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            printf("��ѯ�������Ϣ,������ʾ:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n","�����", "���ʱ��", "����ʱ��", "��ɻ���", "�������", "ԭ��", "�ۿ�", "�ּ�", "��Ʊ");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.02f %-5d\n",f[j].flightNum, f[j].take_off_Time, f[j].land_Time,f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            flag = 1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // �޸�Ϊ֧�ֶ��˶�Ʊ
        printf("\n������Ԥ��Ҫ�����Ļ�Ʊ��: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("������Ч!\n");
            system("pause");
            return;
        }
        
        // ������뻺����
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // ��Ϊ�Լ���Ʊ
        if(n >= 1) {
            // ֱ��ʹ�ò�ѯ���ĺ����Ϊ�Լ���Ʊ
            add_order_for_others(userRealname_Global, userID_Global, str);
        }
        
        // �����Ʊ��������1������Ϊ���˶�Ʊ
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\nΪ���˶�Ʊ #%d:\n", others);
                printf("������˿�����: ");
                scanf("%s", other_name);
                
                // ������뻺����
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("������˿����֤��: ");
                    scanf("%s", other_userId);
                    
                    // ������뻺����
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("���֤�Ÿ�ʽ����ȷ�����������룡\n");
                    }
                }
                
                // ʹ����ͬ�ĺ����Ϊ���˶�Ʊ
                add_order_for_others(other_name, other_userId, str);
            }
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("δ�ҵ�ƥ�亽��!\n");
        system("pause");
    };
    printf("\n");
    
}

// �������ߺ�������
void toLowerCase(char* str) {
    for(; *str; ++str) *str = tolower(*str);
}

void searchByAddr(){  //����ɻ����͵����������,֧��ģ������
    FLIGHT f[NUM];
    char str1[SIZE];
    char str2[SIZE];
    FILE* fp;
    int i=0;
    int flag=0;
    int n;
    float price;
    system("cls");
    
    printf("��������ɻ����͵�������ؼ��ʣ�֧��ģ��������: ");
    scanf("%s%s",str1,str2);
    
    // ת������ΪСд
    toLowerCase(str1);
    toLowerCase(str2);

    if((fp=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        return;
    }
    
    // ���������б��ֲ���
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // ����ԭ��CSV�����߼�
    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // �޸ĺ��ģ��ƥ���߼�
    printf("��ѯ�������Ϣ,������ʾ:\n");
    printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n", "�����","���ʱ��","����ʱ��","��ɻ���","�������","ԭ��","�ۿ�","�ּ�","��Ʊ");
    
    // �洢ƥ��ĺ���
    FLIGHT matched_flights[NUM];
    int match_count = 0;
    
    for(int j=0;j<i;j++){
        // ������ʱ��������Сдת��
        char take_off_lower[SIZE], land_lower[SIZE];
        strcpy(take_off_lower, f[j].take_off_City);
        strcpy(land_lower, f[j].land_City);
        toLowerCase(take_off_lower);
        toLowerCase(land_lower);

        // ʹ��strstr���в���ƥ��
        if(strstr(take_off_lower, str1) && strstr(land_lower, str2)){
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", f[j].flightNum, f[j].take_off_Time, f[j].land_Time, f[j].take_off_City,f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            // ����ƥ��ĺ���
            memcpy(&matched_flights[match_count], &f[j], sizeof(FLIGHT));
            match_count++;
            flag=1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // �޸�Ϊ֧�ֶ��˶�Ʊ
        char selected_flight[SIZE];
        
        printf("\n��ѡ�񺽰��: ");
        scanf("%s", selected_flight);
        
        // ��֤ѡ��ĺ����Ƿ���ƥ������
        int valid_flight = 0;
        for(int j=0; j<match_count; j++) {
            if(strcmp(selected_flight, matched_flights[j].flightNum) == 0) {
                valid_flight = 1;
                break;
            }
        }
        
        if(!valid_flight) {
            printf("ѡ��ĺ������Ч!\n");
            system("pause");
            return;
        }
        
        printf("������Ԥ��Ҫ�����Ļ�Ʊ��: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("������Ч!\n");
            system("pause");
            return;
        }
        
        // ������뻺����
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // ��Ϊ�Լ���Ʊ
        if(n >= 1) {
            // ʹ��ѡ��ĺ����Ϊ�Լ���Ʊ
            add_order_for_others(userRealname_Global, userID_Global, selected_flight);
        }
        
        // �����Ʊ��������1������Ϊ���˶�Ʊ
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\nΪ���˶�Ʊ #%d:\n", others);
                printf("������˿�����: ");
                scanf("%s", other_name);
                
                // ������뻺����
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("������˿����֤��: ");
                    scanf("%s", other_userId);
                    
                    // ������뻺����
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("���֤�Ÿ�ʽ����ȷ�����������룡\n");
                    }
                }
                
                // ʹ����ͬ�ĺ����Ϊ���˶�Ʊ
                add_order_for_others(other_name, other_userId, selected_flight);
            }
            
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("δ�ҵ�ƥ�亽��!\n");
        system("pause");
    };
    printf("\n");
    
}

void order(){  //���Һ���
	int i;
	while(1){
		system("cls");
		printf("                 @@@@@****************************�� ѡ �� �� Ʊ �� ʽ**************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###����������������������������������������������������������������������------------��������������������������������������������������������������###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 ��ɵ� ������ض�Ʊ                           ###\n");
		printf("                 @@@                              2 ��  ��  ��  ��  Ʊ                            @@@\n");
		printf("                 ###                              3 �� �� ʱ �� �� �� Ʊ                          ###\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###��������������������������������������������������������������---------------��--------------| 0 �����ϼ� |������###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("��ѡ��: ");
		scanf("%d",&i);
		switch(i){
			case 1:searchByAddr();break;
			case 2:searchByFligtNum();break; 
			case 3:add_order(1);break;
            case 0:return;
			default:printf("����Ƿ���");
		}
	}
}

int validateNameAndID(const char* name, const char* userId) {
    FILE* fp = fopen("./flight/users.csv", "r");
    if (!fp) {
        return 0; // �ļ������ڣ���֤ʧ��
    }
    
    char line[256];
    fgets(line, sizeof(line), fp); // ����������
    
    int found = 0;
    while(fgets(line, sizeof(line), fp)) {
        char username[SIZE], password[MAX_PASSWORD_LENGTH], storedName[SIZE], storedId[SIZE], gender[10], phone[12];
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", username, password, storedName, storedId, gender, phone);
        
        if(strcmp(storedName, name) == 0 && strcmp(storedId, userId) == 0) {
            found = 1;
            break;
        }
    }
    
    fclose(fp);
    return found;
}

void add_order_for_others(const char* name, const char* userId, const char* flightNum) { //Ϊ���˶�Ʊ��ָ������ţ�
    if (!validateNameAndID(name, userId)) {
        char newName[SIZE];
        char newUserId[SIZE];
        int valid = 0;
        
        printf("�ṩ�����������֤�Ų�ƥ��򲻴�����ϵͳ�У�\n");
        
        while (!valid) {
            printf("����������˿�����: ");
            scanf("%s", newName);
            
            // ������뻺����
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("������˿����֤��: ");
            scanf("%s", newUserId);
            
            // ������뻺����
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (validateIDCard(newUserId)) {
                if (validateNameAndID(newName, newUserId)) {
                    // ʹ���µ���Ч���������֤�ŵݹ����
                    add_order_for_others(newName, newUserId, flightNum);
                    return;
                } else {
                    printf("���������֤�Ų�ƥ��򲻴�����ϵͳ�У������ԣ�\n");
                }
            } else {
                printf("���֤�Ÿ�ʽ����ȷ�����������룡\n");
            }
        }
        return;
    }

    FILE* op;
    FILE* fp1;
    FILE* fp2;
    ORDER o;
    FLIGHT f[NUM];
    int i=0;
    int found = 0;
    int already_booked = 0; // ����Ƿ��Ѿ������ú���

    
    // �򿪶����ļ���׷��ģʽ��
    if((op=fopen("./flight/order.csv","a+"))==NULL){
        printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    
    // �����û��Ƿ��Ѿ��������ú���
    fseek(op, 0, SEEK_SET);
    char first_line[256];
    if(fgets(first_line, sizeof(first_line), op)) { // ����������
        ORDER temp_order;
        while(fscanf(op, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", 
                    temp_order.orderId, temp_order.name, temp_order.userId, 
                    temp_order.order.flightNum, temp_order.order.take_off_Time, 
                    temp_order.order.land_Time, temp_order.order.take_off_City, 
                    temp_order.order.land_City, &temp_order.order.price) != EOF) {
            if(strcmp(temp_order.userId, userId) == 0 && strcmp(temp_order.order.flightNum, flightNum) == 0) {
                already_booked = 1;
                break;
            }
        }
    }
    
    if(already_booked) {
        printf("�ó˿��Ѿ����������� %s�������ظ���Ʊ!\n", flightNum);
        fclose(op);
        system("pause");
        return;
    }
    
    // ���û�ж���������������Ʊ����
    fseek(op, 0, SEEK_SET);
    if(!fgets(first_line, sizeof(first_line), op)) { // �ļ�Ϊ��ʱд���ͷ
        fprintf(op, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
    }
    fseek(op, 0, SEEK_END);  // �ص��ļ�ĩβ׼��׷��

    // ��ȡ�������ݣ�CSV��ʽ��
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷��򿪺����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        fclose(op);
        return;
    }
    
    // ����������
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    
    // ��ȡ������Ϣ
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",
                f[i].flightNum, f[i].take_off_Time, f[i].land_Time,
                f[i].take_off_City, f[i].land_City, 
                &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);
    
    // ��ʼ��������Ϣ
    strcpy(o.name, name);
    strcpy(o.userId, userId);
    strcpy(o.order.flightNum, flightNum);
    
    // ����ָ������
    for(int j=0; j<i; j++){
        if(strcmp(flightNum, f[j].flightNum)==0){
            if(f[j].tickets >= 1){
                // ���ɶ�����Ϣ
                strcpy(o.order.take_off_Time, f[j].take_off_Time);
                strcpy(o.order.land_Time, f[j].land_Time);
                strcpy(o.order.take_off_City, f[j].take_off_City);
                strcpy(o.order.land_City, f[j].land_City);
                snprintf(o.orderId, MAX_ORDER_SIZE, "%s%s", f[j].flightNum, o.userId);
                o.order.price = f[j].now_Price;
                
                // ������Ʊ
                f[j].tickets--;
                found = 1;
                
                // ����д���ļ�
                fprintf(op, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", 
                        o.orderId, o.name, o.userId, o.order.flightNum, 
                        o.order.take_off_Time, o.order.land_Time, 
                        o.order.take_off_City, o.order.land_City, o.order.price);
                fflush(op);  // ȷ������д�����
                break;
            }
            else {
                printf("���� %s ��Ʊ����!��ǰ��Ʊ: %d\n", flightNum, f[j].tickets);
                fclose(op);
                system("pause");
                return;
            }
        }
    }
    
    if(!found){
        printf("û���ҵ������Ϊ %s �ĺ���!\n", flightNum);
        fclose(op);
        system("pause");
        return;
    }
    
    // ���º����ļ�
    if((fp2=fopen("./flight/flight.csv","w"))==NULL){
        printf("�޷��򿪺����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        fclose(op);
        return;
    }
    fprintf(fp2, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
    for(int j=0; j<i; j++){
        // ���Ʊ����֤
        if(f[j].tickets < 0) f[j].tickets = 0;
        fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", 
                f[j].flightNum, f[j].take_off_Time, f[j].land_Time, 
                f[j].take_off_City, f[j].land_City, 
                f[j].pre_Price, f[j].discount, f[j].now_Price, f[j].tickets);
    }
    
    printf("�ɹ�Ϊ %s �����˺��� %s �Ļ�Ʊ!\n", name, flightNum);
    UsefulPayment_Global +=1;
    fclose(fp2);
    fclose(op);
    system("pause");
}

void add_order(int showCheck){ //��Ʊ
    FLIGHT f[NUM];
    char str[SIZE];
    FILE* fp;
    int i = 0;
    int flag = 0;
    int n;
    float price;
    
    if(showCheck==1) check_flight();
    
    printf("����������Ҫ�����ĺ����: ");
    scanf("%s", str);
    
    // �޸�ΪCSV�ļ���ȡ
    if ((fp = fopen("./flight/flight.csv", "r")) == NULL) {
        printf("�޷����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    
    // ����CSV������
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // �޸�ΪCSV��ʽ����
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // ��Ӻ���������ж�
    int flightExists = 0;
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            flightExists = 1;
            break;
        }
    }
    
    if (!flightExists) {
        printf("����� %s ������!\n", str);
        system("pause");
        return;
    }
    
    // ����ԭ�в�ѯ�߼�
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            printf("��ѯ�������Ϣ,������ʾ:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n","�����", "���ʱ��", "����ʱ��", "��ɻ���", "�������", "ԭ��", "�ۿ�", "�ּ�", "��Ʊ");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.02f %-5d\n",f[j].flightNum, f[j].take_off_Time, f[j].land_Time,f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            flag = 1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // �޸�Ϊ֧�ֶ��˶�Ʊ
        printf("\n������Ԥ��Ҫ�����Ļ�Ʊ��: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("������Ч!\n");
            system("pause");
            return;
        }
        
        // ������뻺����
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // ��Ϊ�Լ���Ʊ
        if(n >= 1) {
            // ֱ��ʹ�ò�ѯ���ĺ����Ϊ�Լ���Ʊ
            add_order_for_others(userRealname_Global, userID_Global, str);
        }
        
        // �����Ʊ��������1������Ϊ���˶�Ʊ
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\nΪ���˶�Ʊ #%d:\n", others);
                printf("������˿�����: ");
                scanf("%s", other_name);
                
                // ������뻺����
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("������˿����֤��: ");
                    scanf("%s", other_userId);
                    
                    // ������뻺����
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("���֤�Ÿ�ʽ����ȷ�����������룡\n");
                    }
                }
                
                // ʹ����ͬ�ĺ����Ϊ���˶�Ʊ
                add_order_for_others(other_name, other_userId, str);
            }
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("δ�ҵ�ƥ�亽��!\n");
        system("pause");
    };
    printf("\n");
}

void check_all(){
    ORDER o[NUM];
    FILE* op;
    int i=0;
    
    // �޸��ļ�·��Ϊ order.csv
    if((op=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    
    // ����CSV������
    char header[512]; // ���ӻ�������С
    if (!fgets(header, sizeof(header), op)) {
        printf("�����ļ�Ϊ�ջ��ʽ����!\n");
        fclose(op);
        system("pause");
        return;
    }
    
    printf("���ж�������Ϣ������ʾ:\n");
    printf("%-25s %-10s %-13s %-13s %-22s %-22s %-5s", "������", "����", "���ʱ��", "����ʱ��", "��ɻ���", "�������", "�۸�");
    
    // ʹ�ø��ɿ����ж�ȡ��ʽ
    char line[512];
    while(fgets(line, sizeof(line), op)) {
        // ����ȥ�����п��ܵĻ��з��ͻس���
        char* p = line;
        while(*p) {
            if(*p == '\r' || *p == '\n') {
                *p = 0;
                break;
            }
            p++;
        }
        
        // ʹ��sscanf�������У�����fscanf�ĸ�ʽ����
        char orderId[MAX_ORDER_SIZE], name[SIZE], userId[SIZE], flightNum[SIZE];
        char takeOffTime[SIZE], landTime[SIZE], takeOffCity[SIZE], landCity[SIZE];
        float price;
        
        if(sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", orderId, name, userId, flightNum, takeOffTime, landTime, takeOffCity, landCity, &price) == 9) {
            
            // �޸�Ϊ���Զ���ӻ��з��������ʽ
            printf("\n%-25s %-10s %-13s %-13s %-22s %-22s %-7.2f", orderId, name, takeOffTime, landTime, takeOffCity, landCity, price);
            i++;
        }
    }
    
    if (i == 0) {
        printf("\nû���ҵ��κζ�����¼��");
    } else {
        printf("\n\n���ҵ� %d ��������¼��", i);
    }
    
    printf("\n");
    fclose(op);
    system("pause");
}
////////2025-3-17 �޸� ��������dat��ʽת��Ϊ�ı�csv��ʽ,���ϴ��������bug������Ʊ�������ܵ��³���ѭ���쳣����Ҫ��������ѽ��
void check_person(){
    ORDER o[NUM];
    FILE* op;
    int i = 0;
    int found = 0; // ��ӱ�־����¼�Ƿ��ҵ�����
    
    // �޸��ļ�·���ʹ�ģʽ
    if((op=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    
    // ����CSV������
    char header[256];
    fgets(header, sizeof(header), op);
    
    printf("��ѯ�� ��%s�� �����Ϣ��������ʾ:\n", userRealname_Global);
    printf("%-25s %-7s %-13s %-13s %-22s %-22s %-10s\n", "������","�����","���ʱ��","����ʱ��","��ɻ���","�������","�۸�");
    
    // ����CSV������ʽ - �޸��ֶ�˳��Ͷ�ȡ��ʽ
    char line[512]; // ���ӻ�������С����ֹ���б��ض�
    
    while(fgets(line, sizeof(line), op)) {
        // ȥ����β���з�
        line[strcspn(line, "\n")] = 0;
        
        // ʹ�ø��ɿ��ķ�ʽ����CSV
        char orderId[MAX_ORDER_SIZE], name[SIZE], userId[SIZE], flightNum[SIZE];
        char takeOffTime[SIZE], landTime[SIZE], takeOffCity[SIZE], landCity[SIZE];
        float price;
        
        // ʹ��sscanf�������У�ȷ����ʽƥ��
        if(sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", 
                 orderId, name, userId, flightNum, takeOffTime, landTime, 
                 takeOffCity, landCity, &price) == 9) {
            
            // ��ȷ�Ƚ��û�ID
            if(strcmp(userId, userID_Global) == 0) {
                printf("%-25s %-7s %-13s %-13s %-22s %-22s %-10.2f\n", 
                       orderId, flightNum, takeOffTime, landTime, 
                       takeOffCity, landCity, price);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("δ�ҵ����Ķ�����¼��\n");
    }
    
    printf("\n");
    fclose(op);
}

void check_order(){
	int i;
	system("cls");
	check_person();
	system("pause");
	//system("cls");
	}


void change_order(){
    FILE* fp1;
    FILE* fp2;
    FILE* op1;
    FILE* op2;
    FLIGHT f[NUM];
    ORDER o[NUM];
    char str1[SIZE];
    char str2[SIZE];
    char str3[SIZE];
    int i=0;
    int j=0;
    int k;
    int x;
    int flag=0;
    int fflag=0;
    
    // ��CSV�ļ�
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷��򿪺����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        fclose(op1);
        return;
    }
    
    // ��ȡ��������
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF){
        i++;
    }
    
    // �޸���1�����������ֶζ�ȡ˳��
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[j].orderId, o[j].name, o[j].userId, o[j].order.flightNum, o[j].order.take_off_Time, o[j].order.land_Time, o[j].order.take_off_City, o[j].order.land_City, &o[j].order.price) != EOF){
        j++;
    }
    
    fclose(fp1);
    fclose(op1);
    
    // �û���������
    check_order();
    printf("��������Ҫ�޸Ķ����Ķ�����: ");
    scanf("%s",str1);
    
    // �޸���2��������뻺����
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // ���Ҷ���
    for(k=0;k<j;k++){
        if(strcmp(str1,o[k].orderId)==0){
            system("cls");
            printf("���ҵ��ö���,��Ϣ����:\n");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10s\n", "������","�����","���ʱ��","����ʱ��","��ɻ���","�������","�۸�");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10.2f\n", o[k].orderId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            strcpy(str2,o[k].order.flightNum);
            flag=1;
            x=k;
        }
    }
    
    if(flag==1){
        printf("\n�������º���ĺ����: ");
        scanf("%s",str3);
        
        // �ٴ���ջ�����
        while ((c = getchar()) != '\n' && c != EOF);

        // �����º���
        for(int a=0;a<i;a++){
            if(strcmp(str3,f[a].flightNum)==0){
                if(f[a].tickets>=1){
                    // ���¶�����Ϣ
                    strcpy(o[x].order.flightNum, f[a].flightNum);
                    strcpy(o[x].order.take_off_Time,f[a].take_off_Time);
                    strcpy(o[x].order.land_Time,f[a].land_Time);
                    strcpy(o[x].order.take_off_City,f[a].take_off_City);
                    strcpy(o[x].order.land_City,f[a].land_City);
                    snprintf(o[x].orderId, MAX_ORDER_SIZE, "%s%s", f[a].flightNum, o[x].userId);
                    o[x].order.price=f[a].now_Price;
                    f[a].tickets--;
                    fflag=1;
                }
                else {
                    printf("��Ʊ����!\n\n");
                    system("pause");
                    return;
                }
            }
        }
        
        if(fflag==1){
            // �ָ��ɺ���Ʊ��
            for(int b=0;b<i;b++){
                if(strcmp(str2,f[b].flightNum)==0){
                    f[b].tickets++;
                }
            }
        }
        
        if(fflag==0){
            printf("�ú��಻����!\n\n");
            system("pause");
            return;
        }
    }
    
    if(flag==0){
        printf("δ�ҵ��ö���!\n\n");
        system("pause");
        return;
    }
    
    // �޸���3��д���ļ���ˢ�»�����
    if(flag==1 && fflag==1){
        // ���º����ļ�
        if((fp2=fopen("./flight/flight.csv","w"))==NULL){
            printf("�޷��򿪺����ļ�!ϵͳ��������һ��!\n");
            system("pause");
            return;
        }
        fprintf(fp2, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
        for(int c=0;c<i;c++){
            fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[c].flightNum, f[c].take_off_Time, f[c].land_Time, f[c].take_off_City, f[c].land_City, f[c].pre_Price, f[c].discount, f[c].now_Price, f[c].tickets);
        }
        fflush(fp2);
        fclose(fp2);
        
        // ���¶����ļ�
        if((op2=fopen("./flight/order.csv","w"))==NULL){
            printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
            system("pause");
            return;
        }
        fprintf(op2, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
        for(int c=0;c<j;c++){
            fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[c].orderId, o[c].name, o[c].userId, o[c].order.flightNum, o[c].order.take_off_Time, o[c].order.land_Time, o[c].order.take_off_City, o[c].order.land_City, o[c].order.price);
        }
        fflush(op2);
        fclose(op2);
        
        printf("\n�����޸ĳɹ�!\n");
        system("pause");
    }
}

void return_order(){
    FILE* op1;
    FILE* op2;
    FILE* fp1;
    FILE* fp2;
    char str[SIZE];
    ORDER o[NUM];
    FLIGHT f[NUM];
    int i=0;
    int j=0;
    int c=0;
    int flag=0; 

    // �޸�ΪCSV�ļ���ȡ
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("�޷��򿪶����ļ�!ϵͳ��������һ��!\n");
        system("pause");
        return;
    }
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("�޷��򿪺����ļ�!ϵͳ��������һ��!\n");
        fclose(op1);
        system("pause");
        return;
    }

    // ��ȡ��������
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF){
        i++;
    }

    // ��ȡ��������
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[j].orderId, o[j].name, o[j].userId, o[j].order.flightNum, o[j].order.take_off_Time, o[j].order.land_Time, o[j].order.take_off_City, o[j].order.land_City, &o[j].order.price) != EOF){
        j++;
    }

    fclose(fp1);
    fclose(op1);

    check_person();
    printf("�����붩����: ");
    scanf("%s",str);
    
    // ������뻺����
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    for(int k=0;k<j;k++){
        if(strcmp(str,o[k].orderId)==0){
            printf("\n���ҵ��ö�����Ϣ:\n");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10s\n", "������","�����","���ʱ��","����ʱ��","��ɻ���","�������","�۸�");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10.2f\n", o[k].orderId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            
            printf("ȷ��Ҫ��Ʊ��(1 ȷ��/0 ȡ��):");
            scanf("%d",&c);
            if(c==1){
                // ���º�����Ʊ
                for(int x=0;x<i;x++){
                    if(strcmp(o[k].order.flightNum,f[x].flightNum)==0){
                        f[x].tickets++;
                        break;
                    }
                }
                
                // ɾ��������¼
                for(int x=k;x<j-1;x++){
                    o[x] = o[x+1];
                }
                j--;

                // д����º�ĺ����ļ�
                if((fp2=fopen("./flight/flight.csv","w"))==NULL){
                    printf("�޷����º����ļ�!\n");
                    system("pause");
                    return;
                }
                fprintf(fp2, "�����,���ʱ��,����ʱ��,��ɻ���,�������,ԭ��,�ۿ�,�ּ�,��Ʊ\n");
                for(int x=0;x<i;x++){
                    fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[x].flightNum, f[x].take_off_Time, f[x].land_Time, f[x].take_off_City, f[x].land_City, f[x].pre_Price, f[x].discount, f[x].now_Price, f[x].tickets);
                }
                fclose(fp2);

                // д����º�Ķ����ļ�
                if((op2=fopen("./flight/order.csv","w"))==NULL){
                    printf("�޷����¶����ļ�!\n");
                    system("pause");
                    return;
                }
                fprintf(op2, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
                for(int x=0;x<j;x++){
                    fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[x].orderId, o[x].name, o[x].userId, o[x].order.flightNum, o[x].order.take_off_Time, o[x].order.land_Time, o[x].order.take_off_City, o[x].order.land_City, o[x].order.price);
                }
                fclose(op2);

                printf("\n��Ʊ�ɹ�!\n");
                
                // ��¼ɾ����������ʷ�ļ�
                FILE* history_fp = fopen("./flight/deleted_orders.csv", "a+");
                if(history_fp) {
                    if(ftell(history_fp) == 0) { // ��������ļ�д�����
                        fprintf(history_fp, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
                    }
                    fprintf(history_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[k].orderId, o[k].name, o[k].userId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
                    fclose(history_fp);
                }
                
                flag=1;
                system("pause");
                break;
            }
            else {
                printf("ȡ����Ʊ!\n");
                system("pause");
                return;
            }
        }
    }

    if(flag==0){
        printf("δ�ҵ��ö���!\n");
        system("pause");
    }
}



// �������ܺ���
void caesar_encrypt(char* str, int key) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] = (str[i] - 'a' + key) % 26 + 'a';
		}
		else if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = (str[i] - 'A' + key) % 26 + 'A';
		}
	}
}

// �������ܺ���
void caesar_decrypt(char* str, int key) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] = (str[i] - 'a' - key + 26) % 26 + 'a';
		}
		else if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] = (str[i] - 'A' - key + 26) % 26 + 'A';
		}
	}
}

// �޸����������߼���ʾ���޸� New_password ������
void New_password() {
    FILE* file = fopen(FILENAME, "w");
    char* new_password = Getpassword(); // ��ȡ��̬�ڴ�
    char encrypted_password[MAX_PASSWORD_LENGTH];
    
    if (!file) {
        free(new_password); // �쳣ʱ�ͷ��ڴ�
        printf("�޷����ļ�����д�롣\n");
        return;
    }

    strncpy(encrypted_password, new_password, MAX_PASSWORD_LENGTH); // ��ȫ����
    free(new_password); // ʹ�ú������ͷ�
    
    caesar_encrypt(encrypted_password, 3);
    fprintf(file, "%s\n", encrypted_password);
    fclose(file);
    printf("�����ѳɹ��޸ġ�\n");
    system("pause");
}
// ��֤����
int verify_password() {
	FILE* file = fopen(FILENAME, "r");
	if (!file) {
		printf("�����ļ������ڣ������������롣\n");
		system("pause");
		New_password();
		return 2;
	}

	
	char stored_password[MAX_PASSWORD_LENGTH];
	char input[MAX_PASSWORD_LENGTH];
	if (fgets(stored_password, sizeof(stored_password), file)) {
		stored_password[strcspn(stored_password, "\n")] = '\0'; // ȥ�����з�
		caesar_decrypt(stored_password, 3); // ���ܴ洢������
		fclose(file);
		int i=5;
		do {
			printf("���������룺");
			//scanf("%19s", input);  // �������볤�ȣ����⻺�������
			strcpy(input, Getpassword());
			if (strcmp(input, stored_password) != 0) {  // ����������벻������ʵ����
				if (i > 1) {  // �������ʣ�ೢ�Ի���
					printf("��������㻹��%d��������ᡣ\n\n", i - 1);
				}
				else {  // ���û��ʣ�ೢ�Ի���
					printf("���������û��ʣ�ೢ�Ի����ˡ�\n\n");
				}
			}
			else {  // �������������ȷ
				fclose(file);
				printf("������ȷ��\n\n");
				//system("cls");
				return 0;  // �ɹ��˳�����
			}

			i--;  // ���ٳ��Դ���
		} while (i > 0);  // �������ʣ�ೢ�Դ���������ѭ��
		fclose(file);
		printf("�����������������࣬�����˳���\n\n");
		system("pause");
		return 1;

		
	}
}

// �޸�����
void change_password() {
	
	int state= verify_password();
	if (state ==1) return;//����֤ԭ����
	else if (state == 2) ;

	FILE* file = fopen(FILENAME, "w");
	char new_password[MAX_PASSWORD_LENGTH];
	printf("�����������룺");
	//scanf("%s",new_password);
	
	strcpy(new_password,Getpassword());
	
	if (!file) {
		printf("�޷����ļ�����д�롣\n");
		return;
	}

	char encrypted_password[MAX_PASSWORD_LENGTH];
	strcpy(encrypted_password, new_password);//�Ƚ�����
	caesar_encrypt(encrypted_password, 3); // ����������
	fprintf(file, "%s\n", encrypted_password);//д��������

	fclose(file);
	printf("�����ѳɹ��޸ġ�\n");
	system("pause");
}

// ... ���д��� ...

char* Getpassword()//�����Ǻű��ܴ���
{
    char* password = (char*)malloc(MAX_PASSWORD_LENGTH * sizeof(char)); // ��Ϊ��̬�ڴ����
    memset(password, 0, MAX_PASSWORD_LENGTH);
    int i = 0;
    char c;
    
    while (1) {
        c = _getch();
        if (c == '\r') {
            break;
        } else if (c == '\b') {
            if (i > 0) {  // ��ӱ߽���
                printf("\b \b");
                --i;
            }
        } else if (i < MAX_PASSWORD_LENGTH - 1) {  // ��ֹ���������
            password[i++] = c;
            printf("*");
        }
    }
    printf("\n");
    return password;
}

// ... �������� ...

//Getpassword�������������磬�����Ӵ������룬�������ӣ�https://zhuanlan.zhihu.com/p/109099710

int validateIDCard(const char *id) { //���֤��֤����
    // ϵ����
    const int coefficient[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    // У�����
    const char checkCode[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
    
    // ��鳤���Ƿ�Ϊ18
    if (strlen(id) != 18) {
        return 0;
    }
    
    // ���ǰ17λ�Ƿ�������
    for (int i = 0; i < 17; i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }
    
    // ���У����
    int sum = 0;
    for (int i = 0; i < 17; i++) {
        sum += (id[i] - '0') * coefficient[i];
    }
    
    int remainder = sum % 11;
    char correctCheckCode = checkCode[remainder];
    
    // ��18λ������X(��������10)��x����Ҫת��Ϊ��д�Ƚ�
    char lastChar = toupper(id[17]);
    
    if (lastChar != correctCheckCode) {
        return 0;
    }
    
    // ��������Ƿ�Ϸ�
    char yearStr[5] = {0};
    char monthStr[3] = {0};
    char dayStr[3] = {0};
    
    strncpy(yearStr, id + 6, 4);
    strncpy(monthStr, id + 10, 2);
    strncpy(dayStr, id + 12, 2);
    
    int year = atoi(yearStr);
    int month = atoi(monthStr);
    int day = atoi(dayStr);
    
    // �򵥵�������֤
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // ����ض��·ݵ�����
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return 0;
    } else if (month == 2) {
        // �������ж�
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear) {
            if (day > 29) return 0;
        } else {
            if (day > 28) return 0;
        }
    }
    
    return 1;
}

char* getGenderFromID(const char* id) { //�Ա��жϺ���
    static char gender[10];
    // ���֤��17λ������Ϊ���ԣ�ż��ΪŮ��
    int genderCode = id[16] - '0';
    if (genderCode % 2 == 1) {
        strcpy(gender, "��");
    } else {
        strcpy(gender, "Ů");
    }
    return gender;
}

void user_register() {
    FILE* fp = fopen("./flight/users.csv", "a+"); // ��ΪCSV�ļ�
    USER newUser = {0};

    // �û��������߼�
    do {
        printf("�������û�����");
        scanf("%39s", newUser.username);
        // ������뻺����
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // ����û�������
        if(strlen(newUser.username) >= SIZE-1) {
            printf("�û�������(���39�ַ�)������������!\n");
            continue;
        }

        // CSV��ʽ����
        int duplicate = 0;
        FILE* checkFp = fopen("./flight/users.csv", "r");
        if(checkFp) {
            char line[256];
            fgets(line, sizeof(line), checkFp); // ����������
            while(fgets(line, sizeof(line), checkFp)) {
                char existingName[SIZE] = {0};
                sscanf(line, "%[^,],", existingName); // ֻ��ȡ�û����ֶ�
                if(strcmp(newUser.username, existingName) == 0) {
                    printf("���û����Ѵ��ڣ����������룡\n");
                    duplicate = 1;
                    break;
                }
            }
            fclose(checkFp);
        }
        if(!duplicate) break;
    } while(1);

    // ��ȡ����
    printf("���������룺");
    char* pwd = Getpassword();
    strncpy(newUser.password, pwd, MAX_PASSWORD_LENGTH);
    

    printf("���ٴ��������룺");
    while(1) {
        char* confirmPwd = Getpassword();
        if(strcmp(pwd, confirmPwd) == 0) {
            free(confirmPwd);
            free(pwd);
            break;
        } else {
            printf("������������벻һ�£����������룡\n");
            free(confirmPwd);
        }
    }

    printf("������11λ�ֻ��ţ�");
    while(1){
        scanf("%s", newUser.phone);
        if(strlen(newUser.phone)==11){
            break;
        }else{
            printf("�ֻ��ŷǷ������������룡\n");
        }

    }
    
    printf("�������ֻ���֤�루6λ����");
    while(1){
        char input_code[7]={0};
        scanf("%s", input_code);
        if(strcmp(input_code, "")!=0 && strlen(input_code)==6){
            printf("��֤����ȷ��\n");
            break;
        }else{
            printf("��֤������Ƿ������������룡\n");
        }

    }

    // ��ȡ������Ϣ
    printf("������������");
    scanf("%s", newUser.name);
    
    while(1) {
        printf("���������֤�ţ�");
        scanf("%s", newUser.userId);
        if(validateIDCard(newUser.userId)) {
            strcpy(newUser.gender, getGenderFromID(newUser.userId));
            printf("ϵͳ��⵽�����Ա�Ϊ��%s\n", newUser.gender);
            break;
        } else {
            printf("���֤�Ÿ�ʽ����ȷ�����������룡\n");
        }
    }
    

    // д��CSV�ļ�
    if(fp) {
        // ��������ļ���д�����
        fseek(fp, 0, SEEK_SET);
        char first_line[256];
        if(!fgets(first_line, sizeof(first_line), fp)) { // �ļ�Ϊ��ʱд���ͷ
            fprintf(fp, "�û���,����,����,���֤��,�Ա�,�ֻ���\n");
        }
        fseek(fp, 0, SEEK_END);  // ���õ��ļ�ĩβ׼��׷��

        // д���û����ݣ������Ա��ֶΣ�
        fprintf(fp, "%s,%s,%s,%s,%s,%s\n", newUser.username, newUser.password, newUser.name, newUser.userId, newUser.gender,newUser.phone);
        fclose(fp);
    } else {
        printf("�޷����û����ݿ⣡\n");
    }

    printf("ע��ɹ���\n");
    system("pause");
}

int user_login() {
    FILE* fp = fopen("./flight/users.csv", "r"); // ��ΪCSV�ļ�
    USER currentUser = {0};
    char inputName[SIZE] = {0};
    char* inputPwd = NULL;
    int found = 0;

    printf("�������û�����");
    scanf("%s", inputName);
    
    // ������뻺����
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if(fp) {
        char line[256];
        fgets(line, sizeof(line), fp); // ����������
        
        // CSV��ʽ��ѯ
        while(fgets(line, sizeof(line), fp)) {
            char storedName[SIZE] = {0};
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", storedName, currentUser.password, currentUser.name, currentUser.userId,currentUser.gender,currentUser.phone);

            if(strcmp(inputName, storedName) == 0) {
                found = 1;
                printf("���������룺");
                inputPwd = Getpassword();
                
                if(strcmp(inputPwd, currentUser.password) == 0) {
                    free(inputPwd);
                    fclose(fp);
                    printf("��¼�ɹ���\n");
                    
                    // �洢ȫ����Ϣ
                    strcpy(username_Global, inputName);
                    strcpy(userID_Global, currentUser.userId);
                    strcpy(userRealname_Global, currentUser.name);
                    strcpy(userGender_Global, currentUser.gender);
                    strcpy(userPhone_Global, currentUser.phone);
                    
                    custom();
                    system("pause");
                    return 1;
                }
                free(inputPwd);
                break;
            }
        }
        fclose(fp);
    }

    if(!found) {
        printf("�û������ڣ��Ƿ�����ע�᣿(y/n): ");
        char choice = getchar();
        while((c = getchar()) != '\n' && c != EOF); // ��ջ�����

        if(choice == 'y' || choice == 'Y') {
            user_register();
            printf("��ʹ�����˺����µ�¼��\n");
            system("pause");
            return 0;
        }
    } else {
        printf("�������\n");
        system("pause");
    }
    return 0;
}

void user_change_password() {
    FILE* fp = fopen("./flight/users.csv", "r+");
    USER users[100];
    char username[SIZE] = {0};
    char userId[SIZE] = {0};
    char newPwd[MAX_PASSWORD_LENGTH];
    int count = 0, found = 0;

    // ��ȡCSV����
    if(fp) {
        char line[256];
        fgets(line, sizeof(line), fp); // ����������
        while(fgets(line, sizeof(line), fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%s", users[count].username, users[count].password, users[count].name, users[count].userId);
         count++;
        }
        fclose(fp);
    } else {
        printf("�޷����û��ļ���\n");
        system("pause");
        return;
    }

    // ��ȡ�û�����
    printf("�������û�����");
    scanf("%s", username);
    printf("���������֤�ţ�");
    scanf("%19s", userId);
   
    
    // �������뻺����
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // ���Ҳ��޸�����
    for(int i = 0; i < count; i++) {
        if(strcmp(username, users[i].username) == 0 && 
           strcmp(userId, users[i].userId) == 0) 
        {
            printf("�����������룺");
            char* tmp = Getpassword();
            strncpy(newPwd, tmp, MAX_PASSWORD_LENGTH);
            free(tmp);

            strncpy(users[i].password, newPwd, MAX_PASSWORD_LENGTH);
            found = 1;
            break;
        }
    }

    // д����º������
    if(found) {
        fp = fopen("./flight/users.csv", "w");
        if(fp) {
            fprintf(fp, "�û���,����,����,���֤��\n");
            for(int i = 0; i < count; i++) {
                fprintf(fp, "%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId);
            }
            fclose(fp);
            printf("�����޸ĳɹ���\n");
        } else {
            printf("�ļ�����ʧ�ܣ�\n");
        }
    } else {
        printf("�û���֤ʧ�ܣ�\n");
    }
    system("pause");
}

void list_all_users() {
    FILE* fp = fopen("./flight/users.csv", "r");
    if (!fp) {
        printf("�����û����ݣ�\n");
        system("pause");
        return;
    }

    printf("\n%-40s %-20s %-20s %-5s %-13s\n", "�û���", "����", "���֤��", "�Ա�", "�ֻ���");
    printf("----------------------------------------------------------\n");
    printf("----------------------------------------------------------\n");

    char line[256];
    fgets(line, sizeof(line), fp); // ����������
    
    while(fgets(line, sizeof(line), fp)) {
        USER user = {0};
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", user.username, user.password, user.name, user.userId, user.gender, user.phone);
        
        printf("%-40s %-20s %-20s %-5s %-13s\n", user.username, user.name, user.userId, user.gender, user.phone);
    }
    fclose(fp);
    system("pause");
}

// ����ע���˻�����
void delete_account() {
    FILE* user_fp = fopen("./flight/users.csv", "r+");
    FILE* order_fp = fopen("./flight/order.csv", "r+");
    USER users[100];
    ORDER orders[100];
    int user_count = 0, order_count = 0;
    char confirm[10];

    // ��ȡCSV�û�����
    if(user_fp) {
        char line[256];
        fgets(line, sizeof(line), user_fp); // ����������
        while(fgets(line, sizeof(line), user_fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", users[user_count].username, users[user_count].password, users[user_count].name, users[user_count].userId,users[user_count].gender,users[user_count].phone);
            user_count++;
        }
        fclose(user_fp);
    }

    // ��ȡCSV��������
    if(order_fp) {
        char line[512];
        fgets(line, sizeof(line), order_fp); // ����������
        while(fgets(line, sizeof(line), order_fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", orders[order_count].orderId, orders[order_count].name, orders[order_count].userId, orders[order_count].order.flightNum, orders[order_count].order.take_off_Time, orders[order_count].order.land_Time, orders[order_count].order.take_off_City, orders[order_count].order.land_City, &orders[order_count].order.price);
            order_count++;
        }
        fclose(order_fp);
    }

    // ȷ�ϲ���
    printf("ȷ��Ҫ����ɾ���˻�%s��(y/n): ", username_Global);
    scanf("%s", confirm);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if(strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0) {
        return;
    }

    // ɾ���û�����
    int new_user_count = 0;
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].username, username_Global) != 0) {
            users[new_user_count++] = users[i];
        }
    }

    // �޸���1��ʹ����ȷ�Ķ�����ʷ�ļ�ͷ
    FILE* history_fp = fopen("./flight/deleted_orders.csv", "a+");
    if(history_fp) {
        // ����ļ��Ƿ�Ϊ��
        fseek(history_fp, 0, SEEK_END);
        if(ftell(history_fp) == 0) {
            fprintf(history_fp, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
        }
        fseek(history_fp, 0, SEEK_END);
        
        for(int i = 0; i < order_count; i++) {
            if(strcmp(orders[i].userId, userID_Global) == 0) {
                fprintf(history_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", orders[i].orderId, orders[i].name, orders[i].userId, orders[i].order.flightNum, orders[i].order.take_off_Time, orders[i].order.land_Time, orders[i].order.take_off_City, orders[i].order.land_City, orders[i].order.price);
            }
        }
        fclose(history_fp);
    }

    // ɾ����������
    int new_order_count = 0;
    for(int i = 0; i < order_count; i++) {
        if(strcmp(orders[i].userId, userID_Global) != 0) {
            orders[new_order_count++] = orders[i];
        }
    }

    // �޸���2��ʹ����ȷ���û��ļ�ͷ
    user_fp = fopen("./flight/users.csv", "w");
    if(user_fp) {
        fprintf(user_fp, "�û���,����,����,���֤��,�Ա�,�ֻ���\n"); // ���������ȷ��ͷ
        for(int i = 0; i < new_user_count; i++) {
            fprintf(user_fp, "%s,%s,%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId,users[i].gender,users[i].phone);
        }
        fclose(user_fp);
    }

    // д����º�Ķ�������
    order_fp = fopen("./flight/order.csv", "w");
    if(order_fp) {
        fprintf(order_fp, "������,����,�û�ID,�����,���ʱ��,����ʱ��,��ɻ���,�������,�۸�\n");
        for(int i = 0; i < new_order_count; i++) {
            fprintf(order_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", orders[i].orderId, orders[i].name, orders[i].userId, orders[i].order.flightNum, orders[i].order.take_off_Time, orders[i].order.land_Time, orders[i].order.take_off_City, orders[i].order.land_City, orders[i].order.price);
        }
        fclose(order_fp);
    }

    // �޸���3��ʹ����ȷ��ע���û���ʷ�ļ�ͷ
    FILE* del_fp = fopen("./flight/deleted_users.csv", "a+");
    if(del_fp) {
        fseek(del_fp, 0, SEEK_END);
        if(ftell(del_fp) == 0) {
            fprintf(del_fp, "�û���,����,����,���֤��,�Ա�,�ֻ���\n"); // ���������ȷ��ͷ
        }
        for(int i = 0; i < user_count; i++) {
            if(strcmp(users[i].username, username_Global) == 0) { fprintf(del_fp, "%s,%s,%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId,users[i].gender,users[i].phone);
                break;
            }
        }
        fclose(del_fp);
    }

    // ���ȫ�ֱ���
    memset(username_Global, 0, SIZE);
    memset(userID_Global, 0, SIZE);
    memset(userRealname_Global, 0, SIZE);
    system("pause");
    system("cls");
    main();
}

void user_information_change(){
    FILE* fp = fopen("./flight/users.csv", "r");
    USER users[100];
    int count = 0;
    int userIndex = -1;
    char choice;
    
    if(!fp) {
        printf("�޷����û������ļ���\n");
        system("pause");
        return;
    }
    
    // ��ȡ�����û�����
    char line[256];
    fgets(line, sizeof(line), fp); // ����������
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", 
               users[count].username, 
               users[count].password, 
               users[count].name, 
               users[count].userId,
               users[count].gender,
               users[count].phone);
        
        // �ҵ���ǰ��¼�û�
        if(strcmp(users[count].username, username_Global) == 0) {
            userIndex = count;
        }
        count++;
    }
    fclose(fp);
    
    if(userIndex == -1) {
        printf("δ�ҵ���ǰ�û���Ϣ�������µ�¼��\n");
        system("pause");
        return;
    }
    
    // ��ʾ��ǰ�û���Ϣ
    system("cls");
    printf("=== ��ǰ�û���Ϣ ===\n");
    printf("�û���: %s\n", users[userIndex].username);
    printf("����: %s\n", users[userIndex].name);
    printf("�Ա�: %s\n", users[userIndex].gender);
    printf("�ֻ���: %s\n", users[userIndex].phone);
    printf("���֤��: %s\n", users[userIndex].userId);
    printf("=====================\n\n");
    printf("\n�������֤�ŵ�������ԣ������޸���ע���˺�����ע��\n");
    
    printf("��ѡ��Ҫ�޸ĵ���Ϣ��\n");
    printf("1. ����\n");
    printf("2. �ֻ���\n");
    printf("3. ����\n");
    printf("0. ����\n");
    printf("��ѡ��: ");
    
    // ������뻺����
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    choice = getchar();
    while ((c = getchar()) != '\n' && c != EOF); // �ٴ���ջ�����
    
    switch(choice) {
        case '1': {
            // �޸�����
            char newName[SIZE];
            printf("������������: ");
            scanf("%s", newName);
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("ȷ�Ͻ������� \"%s\" �޸�Ϊ \"%s\"��(y/n): ", users[userIndex].name, newName);
            char confirm = getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(confirm == 'y' || confirm == 'Y') {
                strcpy(users[userIndex].name, newName);
                strcpy(userRealname_Global, newName); // ����ȫ�ֱ���
                printf("�����޸ĳɹ���\n");
            } else {
                printf("��ȡ���޸ġ�\n");
            }
            break;
        }
        case '2': {
            // �޸��ֻ���
            char newPhone[12];
            printf("���������ֻ���(11λ): ");
            scanf("%s", newPhone);
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(strlen(newPhone) != 11) {
                printf("�ֻ��ű���Ϊ11λ��\n");
                system("pause");
                return;
            }
            
            printf("ȷ�Ͻ��ֻ��Ŵ� \"%s\" �޸�Ϊ \"%s\"��(y/n): ", users[userIndex].phone, newPhone);
            char confirm = getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(confirm == 'y' || confirm == 'Y') {
                strcpy(users[userIndex].phone, newPhone);
                strcpy(userPhone_Global, newPhone); // ����ȫ�ֱ���
                printf("�ֻ����޸ĳɹ���\n");
            } else {
                printf("��ȡ���޸ġ�\n");
            }
            break;
        }
        case '3': {
            // �޸�����
            printf("�����뵱ǰ����: ");
            char* currentPwd = Getpassword();
            
            if(strcmp(currentPwd, users[userIndex].password) != 0) {
                printf("��ǰ�������\n");
                free(currentPwd);
                system("pause");
                return;
            }
            free(currentPwd);
            
            printf("������������: ");
            char* newPwd = Getpassword();
            
            printf("���ٴ�����������: ");
            char* confirmPwd = Getpassword();
            
            if(strcmp(newPwd, confirmPwd) != 0) {
                printf("������������벻һ�£�\n");
                free(newPwd);
                free(confirmPwd);
                system("pause");
                return;
            }
            
            strcpy(users[userIndex].password, newPwd);
            printf("�����޸ĳɹ���\n");
            
            free(newPwd);
            free(confirmPwd);
            break;
        }case '0':return;
        default:
            printf("��Чѡ��\n");
            system("pause");
            return;
    }
    
    // �����º���û���Ϣд���ļ�
    fp = fopen("./flight/users.csv", "w");
    if(!fp) {
        printf("�޷����û������ļ�����д�룡\n");
        system("pause");
        return;
    }
    
    // д��CSV��ͷ
    fprintf(fp, "�û���,����,����,���֤��,�Ա�,�ֻ���\n");
    
    // д�������û�����
    for(int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%s\n", 
                users[i].username, 
                users[i].password, 
                users[i].name, 
                users[i].userId,
                users[i].gender,
                users[i].phone);
    }
    
    fclose(fp);
    system("pause");
}

void administor() {
	char i;
	//��ӡ�������
	if (verify_password()) return;
	while (1) {
		system("cls");
		printf("                 @@@@@***************************��ӭʹ�ú������ϵͳ***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###�������������������������������������������������������������������� �� �� Ա ����������������---����������������������������������������������###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 @@@                              A �� �� �� �� �� Ϣ                             @@@\n");
		printf("                 ###                              B ɾ �� �� �� �� Ϣ                             ###\n");
		printf("                 @@@                              C ¼ �� �� �� �� Ϣ                             @@@\n");
		printf("                 @@@                              D �� �� �� �� �� ��                             @@@\n");
		printf("                 @@@                              E �� �� �� �� �� ��                             @@@\n");
		printf("                 @@@                              F �� �� �� �� �� ��                             @@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###��������������������������������������������������������������--------------��---------------| 0 �˳����� |������###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("��ѡ��: ");

		scanf("%c", &i);
		if (i >= 97 && i <= 122) {
			i -= 32;
		}
		switch (i) {
		case 'A':change_flight(); break;
		case 'B':delete_flight(); break;
		case 'C':input_flight(); break;
		case 'D':change_password(); break;
		case 'E':check_all(); break;
		case 'F':list_all_users(); break;
		case '0':return;
		default:break;
	}
	
	}
}

void custom(){
	char i;
	//��ӡ������
	while(1){
		system("cls");
		printf("                 | �û�����%s |---| ������%s |---| �ֻ��ţ�%s |\n",username_Global,userRealname_Global,userPhone_Global);
        printf("                 ###����������������������������������������������������������������������------------��������| ��Ϣ���󣿰���*���޸� |����###\n");
        printf("                 ###����������������������������������������������������������������������------------��������| \033[31m!������\033[0m����#��ע���˻�|����###\n");
        printf("                                                                                                     \n");
		printf("                 @@@@@***************************��ӭʹ�ú��ඩƱϵͳ***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###����������������������������������������������������������������������------------��������������������������������������������������������������###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 ��            Ʊ                              ###\n");
		printf("                 @@@                              2 �� �� ȫ �� �� ��                             @@@\n");
		printf("                 @@@                              3 ��   ��   ��   ��                             @@@\n");
		printf("                 ###                              4 ��   ��   ��   ��                             ###\n");
		printf("                 @@@                              5 ��            Ʊ                              @@@\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###                                                                              ###\n");
		printf("                 ###��������������������������������������������������������������---------------��------------- | 0 �˳���¼ |������###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("��ѡ��: ");
		
		scanf("%c", &i);
		//����ѡ��
		switch(i){
			case '1':order(); break;
			case '2':check_flight();break;
	 		case '3':check_order();break;
	 		case '4':change_order();break;
	 		case '5':return_order();break;
			case '#':delete_account();break;
			case '*':user_information_change(); break;
	 		case '0':return;
	 		default:break;
		}
	}
	return;
}

int main(){
	char i;
	strcpy(username_Global, "");  //�˳���¼���ʼ����Ϣ
	strcpy(userID_Global, "");    
	strcpy(userRealname_Global, "");
	//��ӡ������
	while(1){
		system("cls");
		printf("                 @@@@@***************************��ӭ��¼���ඩƱϵͳ***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###����������������������������������������������������������������������------------��������������������������������������������������������������###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 ��            ¼                              ###\n");
		printf("                 @@@                              2 ע            ��                              @@@\n");
		printf("                 ###                              3 ��   ��   ��   ��                             ###\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###��������������������������������������������������������������--------------��| * ����Աѡ�� | 0 �˳����� |������###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("��ѡ��: ");
		
		scanf("%c", &i);
		//����ѡ��
		switch(i){
			case '1':user_login(); break;
			case '2':user_register();break;
			case '3':user_change_password();break;
			case '*':administor(); break;
	 		case '0':exit(0);
	 		default:break;
		}
	}
	return 0;
}
