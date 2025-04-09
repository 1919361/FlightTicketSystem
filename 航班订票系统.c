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
    char name[SIZE];       // 新增姓名字段
    char password[MAX_PASSWORD_LENGTH];
    char userId[SIZE];
    char gender[10];       // 新增性别字段
    char phone[12];      // 新增手机号码字段
} USER, *PUSER;

char username_Global[SIZE];//全局用户名
// 添加在username_Global声明附近
char userID_Global[SIZE]; //全局用户ID
char userRealname_Global[SIZE];//全局用户姓名
char userPhone_Global[SIZE];//全局用户手机号
char userGender_Global[SIZE];//全局用户性别
int UsefulPayment_Global = 0;

void input_flight();

void user_register();
int user_login();
int verify_password();
char* Getpassword();
void change_password();
void user_change_password();
void custom();
void list_all_users();  // 用户列表函数声明
int main();
void add_order(int showCheck);
void order();
int validateIDCard(const char *id);
void add_order_for_others(const char* name, const char* userId, const char* flightNum);

void PaymentSystem(float price,int num){
    float totalPrice = price * num;
    if(totalPrice == 0) {
        printf("无效支付！\n");
        system("pause");
        return;	
    }
    printf("合计：%.2f 元\n", totalPrice);
    printf("1. 微信支付\n");
    printf("2. 支付宝支付\n");
    printf("3. 银行卡支付\n");
    printf("请选择支付方式:\n");
    int choice;
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("您已选择微信支付，支付金额为 %.2f 元\n", totalPrice);
            break; 	
        case 2:
            printf("您已选择支付宝支付，支付金额为 %.2f 元\n", totalPrice);
            break;
        case 3:
            printf("您已选择银行卡支付，支付金额为 %.2f 元\n", totalPrice);
            break;
        default:
            printf("无效的支付方式！\n");
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
    
    // 修复文件打开模式（合并为单次打开操作）
    if((fp=fopen("./flight/flight.csv","a+"))==NULL){
        printf("无法打开文件!系统将返回上一级菜单!\n");
        return;
    }
    
    // 修复文件头检测逻辑
    fseek(fp, 0, SEEK_SET);
    char first_line[256];
    if(!fgets(first_line, sizeof(first_line), fp)) { // 文件为空时写入表头
        fprintf(fp, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
    }
    fseek(fp, 0, SEEK_END);  // 重置到文件末尾准备追加

    printf("请输入预计要录入的航班数: ");
    if(scanf("%d",&n) != 1 || n <= 0) {  // 输入验证加强
        printf("输入无效，退出选项！");
        fclose(fp);
        system("pause");
        return;
    }

    // 修复输入缓冲区问题
    while(getchar() != '\n');  // 清空输入缓冲区

    printf("请按照顺序(航班号 起飞时间 降落时间 起飞机场 到达机场 原票价 折扣 剩余票数)依次输入...\n");
    printf("时间格式为01-01/00:00\n");
    while (i < n) {
        // 添加输入验证
        int read_count = scanf("%s%s%s%s%s%f%f%d", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].tickets);
        
        if(read_count != 8) {  // 检查输入完整性
            printf("输入格式错误，请重新输入本条数据！\n");
            while(getchar() != '\n');  // 清空错误输入
            continue;
        }
        
        // 修复数值范围验证
        if(f[i].discount < 0.1f || f[i].discount > 10.0f) {
            printf("折扣率应在0.1-10.0之间！\n");
            continue;
        }
        
        f[i].now_Price = f[i].pre_Price * f[i].discount;
        
        // 修复文件写入格式
        fprintf(fp, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, f[i].pre_Price, f[i].discount, f[i].now_Price, f[i].tickets);
        i++;
        
        if(i >= n) {
            // 修复继续输入逻辑
            printf("[当前已录入%d条] 是否继续录入？(继续数量/0退出): ", i);
            int x;
            if(scanf("%d", &x) == 1 && x > 0) {
                n += x;
            } else {
                break;
            }
        }
    }
    
    fclose(fp);
    printf("航班信息录入完成!\n");
    system("pause");
}

void check_flight(){  //录入航班
    FLIGHT f;
    FILE* fp;
    char header[256];
    printf("所有航班的信息如下所示:\n");
    if((fp=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    fgets(header, sizeof(header), fp); // 跳过标题行
    printf("%-8s %-12s %-12s %-25s %-25s %-10s %-5s %-10s %-5s\n","航班号","起飞时间","降落时间","起飞机场","到达机场","原价","折扣","现价","余票");
    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f.flightNum, f.take_off_Time, f.land_Time, f.take_off_City, f.land_City, &f.pre_Price, &f.discount, &f.now_Price, &f.tickets) != EOF) {
        printf("%-8s %-12s %-12s %-25s %-25s %-10.2f %-5.2f %-10.2f %-5d\n", f.flightNum, f.take_off_Time, f.land_Time,f.take_off_City, f.land_City,f.pre_Price,f.discount,f.now_Price,f.tickets);
    }
    fclose(fp);
	system("pause");
}

void change_flight(){  //修改航班信息
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

    // 读取航班CSV
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);

    // 读取订单CSV
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[a].orderId, o[a].name, o[a].userId, o[a].order.flightNum, o[a].order.take_off_Time, o[a].order.land_Time, o[a].order.take_off_City, o[a].order.land_City, &o[a].order.price) != EOF) {
        a++;
    }
    fclose(op1);

    check_flight();
    printf("请输入需要修改航班的航班号: ");
    scanf("%s",str);

    for(int j=0;j<i;j++){
        if(strcmp(str,f[j].flightNum)==0){
            printf("已找到该航班:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n",
                "航班号","起飞时间","降落时间","起飞机场","到达机场","原价","折扣","现价","余票");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", 
                f[j].flightNum, f[j].take_off_Time, f[j].land_Time,
                f[j].take_off_City, f[j].land_City,
                f[j].pre_Price,f[j].discount,f[j].now_Price,f[j].tickets);
            
            printf("请重新输入该航班的信息;\n请按照(起飞时间 降落时间 起飞机场 到达机场 原票价 折扣 剩余票数)顺序依次输入相应信息;\n其中起飞和降落时间请按照形如\"2020-01-01/12:45\"的形式输入,若无折扣请输入1.0:\n");
            scanf("%s%s%s%s%f%f%d", f[j].take_off_Time,f[j].land_Time, f[j].take_off_City,f[j].land_City, &f[j].pre_Price,&f[j].discount,&f[j].tickets);
            f[j].now_Price=f[j].pre_Price*f[j].discount;
            x=j;
            flag=1;
        }
    }

    if (flag == 0) { 
        printf("您所要修改的航班并不存在!\n"); 
        system("pause");
        return;
    }

    // 更新航班CSV
    if((fp2=fopen("./flight/flight.csv","w"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    fprintf(fp2, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
    for(int j=0;j<i;j++){
        fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[j].flightNum, f[j].take_off_Time, f[j].land_Time, f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
    }
    fclose(fp2);

    // 更新订单CSV
    if((op2=fopen("./flight/order.csv","w"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    fprintf(op2, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
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

    printf("\n修改已完成！\n");
    system("pause");
}

void delete_flight(){  //删除航班信息
    FLIGHT f[NUM];
    ORDER o[NUM];
    FILE* fp1;
    FILE* fp2;
    FILE* op1;
    FILE* op2;
    char str[SIZE];
    int i=0, a=0, flag=0, c=0;
    
    // 修改为CSV读取模式
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    
    // 处理CSV标题行
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    
    // 读取航班数据
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);

    // 读取订单数据（CSV格式）
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n",o[a].orderId, o[a].name, o[a].userId, o[a].order.flightNum,o[a].order.take_off_Time, o[a].order.land_Time,o[a].order.take_off_City, o[a].order.land_City, &o[a].order.price) != EOF) {
        a++;
    }
    fclose(op1);

    check_flight();
    printf("请输入要删除的航班号: ");
    scanf("%s", str);
    
    // 查找航班并确认
    int x = -1;
    for (int k = 0; k < i; k++) {
        if (strcmp(str, f[k].flightNum) == 0) {
            x = k;
            printf("找到航班:\n%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", f[k].flightNum, f[k].take_off_Time, f[k].land_Time,f[k].take_off_City, f[k].land_City, f[k].pre_Price,f[k].discount, f[k].now_Price, f[k].tickets);
                
            printf("确认删除该航班吗？(1=确认/0=取消): ");
            scanf("%d", &c);
            break;
        }
    }

    if (x == -1) {
        printf("未找到航班: %s\n", str);
        system("pause");
        return;
    }

    // 更新航班CSV文件
    if(c==1){
        if((fp2=fopen("./flight/flight.csv","w"))==NULL){
            printf("无法打开文件!系统将返回上一级!\n");
            return;
        }
        // 写入CSV标题
        fprintf(fp2, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
        // 写入剩余航班
        for(int k=0; k<i; k++){
            if(k != x){
                fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n",f[k].flightNum, f[k].take_off_Time, f[k].land_Time,f[k].take_off_City, f[k].land_City, f[k].pre_Price,f[k].discount, f[k].now_Price, f[k].tickets);
            }
        }
        fclose(fp2);
    }

    // 更新订单CSV文件
    if(c==1){
        if((op2=fopen("./flight/order.csv","w"))==NULL){
            printf("无法打开文件!系统将返回上一级!\n");
            return;
        }
        // 写入CSV标题
        fprintf(op2, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
        // 写入剩余订单
        for(int k=0; k<a; k++){
            if(strcmp(str, o[k].order.flightNum) != 0){
                fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n",o[k].orderId, o[k].name, o[k].userId, o[k].order.flightNum,o[k].order.take_off_Time, o[k].order.land_Time,o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            }
        }
        fclose(op2);
    }
}

void searchByFligtNum(){  //按航班号搜索
    FLIGHT f[NUM];
    char str[SIZE];
    FILE* fp;
    int i = 0;
    int flag = 0;
    int n;
    float price;
    
    printf("请输入您想要查询的航班号: ");
    scanf("%s", str);
    
    // 修改为CSV文件读取
    if ((fp = fopen("./flight/flight.csv", "r")) == NULL) {
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    
    // 跳过CSV标题行
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // 修改为CSV格式解析
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // 保持原有查询逻辑
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            printf("查询到相关信息,如下所示:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n","航班号", "起飞时间", "降落时间", "起飞机场", "到达机场", "原价", "折扣", "现价", "余票");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.02f %-5d\n",f[j].flightNum, f[j].take_off_Time, f[j].land_Time,f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            flag = 1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // 修改为支持多人订票
        printf("\n请输入预计要订购的机票数: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("输入无效!\n");
            system("pause");
            return;
        }
        
        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // 先为自己订票
        if(n >= 1) {
            // 直接使用查询到的航班号为自己订票
            add_order_for_others(userRealname_Global, userID_Global, str);
        }
        
        // 如果订票数量大于1，处理为他人订票
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\n为他人订票 #%d:\n", others);
                printf("请输入乘客姓名: ");
                scanf("%s", other_name);
                
                // 清空输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("请输入乘客身份证号: ");
                    scanf("%s", other_userId);
                    
                    // 清空输入缓冲区
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("身份证号格式不正确，请重新输入！\n");
                    }
                }
                
                // 使用相同的航班号为他人订票
                add_order_for_others(other_name, other_userId, str);
            }
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("未找到匹配航班!\n");
        system("pause");
    };
    printf("\n");
    
}

// 新增工具函数声明
void toLowerCase(char* str) {
    for(; *str; ++str) *str = tolower(*str);
}

void searchByAddr(){  //按起飞机场和到达机场搜索,支持模糊搜索
    FLIGHT f[NUM];
    char str1[SIZE];
    char str2[SIZE];
    FILE* fp;
    int i=0;
    int flag=0;
    int n;
    float price;
    system("cls");
    
    printf("请输入起飞机场和到达机场关键词（支持模糊搜索）: ");
    scanf("%s%s",str1,str2);
    
    // 转换输入为小写
    toLowerCase(str1);
    toLowerCase(str2);

    if((fp=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开文件!系统将返回上一级!\n");
        return;
    }
    
    // 跳过标题行保持不变
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // 保持原有CSV解析逻辑
    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // 修改后的模糊匹配逻辑
    printf("查询到相关信息,如下所示:\n");
    printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n", "航班号","起飞时间","降落时间","起飞机场","到达机场","原价","折扣","现价","余票");
    
    // 存储匹配的航班
    FLIGHT matched_flights[NUM];
    int match_count = 0;
    
    for(int j=0;j<i;j++){
        // 创建临时副本进行小写转换
        char take_off_lower[SIZE], land_lower[SIZE];
        strcpy(take_off_lower, f[j].take_off_City);
        strcpy(land_lower, f[j].land_City);
        toLowerCase(take_off_lower);
        toLowerCase(land_lower);

        // 使用strstr进行部分匹配
        if(strstr(take_off_lower, str1) && strstr(land_lower, str2)){
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.2f %-5d\n", f[j].flightNum, f[j].take_off_Time, f[j].land_Time, f[j].take_off_City,f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            // 保存匹配的航班
            memcpy(&matched_flights[match_count], &f[j], sizeof(FLIGHT));
            match_count++;
            flag=1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // 修改为支持多人订票
        char selected_flight[SIZE];
        
        printf("\n请选择航班号: ");
        scanf("%s", selected_flight);
        
        // 验证选择的航班是否在匹配结果中
        int valid_flight = 0;
        for(int j=0; j<match_count; j++) {
            if(strcmp(selected_flight, matched_flights[j].flightNum) == 0) {
                valid_flight = 1;
                break;
            }
        }
        
        if(!valid_flight) {
            printf("选择的航班号无效!\n");
            system("pause");
            return;
        }
        
        printf("请输入预计要订购的机票数: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("输入无效!\n");
            system("pause");
            return;
        }
        
        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // 先为自己订票
        if(n >= 1) {
            // 使用选择的航班号为自己订票
            add_order_for_others(userRealname_Global, userID_Global, selected_flight);
        }
        
        // 如果订票数量大于1，处理为他人订票
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\n为他人订票 #%d:\n", others);
                printf("请输入乘客姓名: ");
                scanf("%s", other_name);
                
                // 清空输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("请输入乘客身份证号: ");
                    scanf("%s", other_userId);
                    
                    // 清空输入缓冲区
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("身份证号格式不正确，请重新输入！\n");
                    }
                }
                
                // 使用相同的航班号为他人订票
                add_order_for_others(other_name, other_userId, selected_flight);
            }
            
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("未找到匹配航班!\n");
        system("pause");
    };
    printf("\n");
    
}

void order(){  //查找航班
	int i;
	while(1){
		system("cls");
		printf("                 @@@@@****************************请 选 择 订 票 方 式**************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###———————————————————————————————————------------———————————————————————————————###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 起飞地 、降落地订票                           ###\n");
		printf("                 @@@                              2 航  班  号  订  票                            @@@\n");
		printf("                 ###                              3 航 班 时 刻 表 订 票                          ###\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###———————————————————————————————---------------—--------------| 0 返回上级 |———###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("请选择: ");
		scanf("%d",&i);
		switch(i){
			case 1:searchByAddr();break;
			case 2:searchByFligtNum();break; 
			case 3:add_order(1);break;
            case 0:return;
			default:printf("输入非法！");
		}
	}
}

int validateNameAndID(const char* name, const char* userId) {
    FILE* fp = fopen("./flight/users.csv", "r");
    if (!fp) {
        return 0; // 文件不存在，验证失败
    }
    
    char line[256];
    fgets(line, sizeof(line), fp); // 跳过标题行
    
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

void add_order_for_others(const char* name, const char* userId, const char* flightNum) { //为他人订票（指定航班号）
    if (!validateNameAndID(name, userId)) {
        char newName[SIZE];
        char newUserId[SIZE];
        int valid = 0;
        
        printf("提供的姓名和身份证号不匹配或不存在于系统中！\n");
        
        while (!valid) {
            printf("请重新输入乘客姓名: ");
            scanf("%s", newName);
            
            // 清空输入缓冲区
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("请输入乘客身份证号: ");
            scanf("%s", newUserId);
            
            // 清空输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (validateIDCard(newUserId)) {
                if (validateNameAndID(newName, newUserId)) {
                    // 使用新的有效姓名和身份证号递归调用
                    add_order_for_others(newName, newUserId, flightNum);
                    return;
                } else {
                    printf("姓名和身份证号不匹配或不存在于系统中，请重试！\n");
                }
            } else {
                printf("身份证号格式不正确，请重新输入！\n");
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
    int already_booked = 0; // 标记是否已经订过该航班

    
    // 打开订单文件（追加模式）
    if((op=fopen("./flight/order.csv","a+"))==NULL){
        printf("无法打开订单文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    
    // 检查该用户是否已经订购过该航班
    fseek(op, 0, SEEK_SET);
    char first_line[256];
    if(fgets(first_line, sizeof(first_line), op)) { // 跳过标题行
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
        printf("该乘客已经订购过航班 %s，不能重复订票!\n", flightNum);
        fclose(op);
        system("pause");
        return;
    }
    
    // 如果没有订过，继续正常订票流程
    fseek(op, 0, SEEK_SET);
    if(!fgets(first_line, sizeof(first_line), op)) { // 文件为空时写入表头
        fprintf(op, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
    }
    fseek(op, 0, SEEK_END);  // 回到文件末尾准备追加

    // 读取航班数据（CSV格式）
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开航班文件!系统将返回上一级!\n");
        system("pause");
        fclose(op);
        return;
    }
    
    // 跳过标题行
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    
    // 读取航班信息
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",
                f[i].flightNum, f[i].take_off_Time, f[i].land_Time,
                f[i].take_off_City, f[i].land_City, 
                &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp1);
    
    // 初始化订单信息
    strcpy(o.name, name);
    strcpy(o.userId, userId);
    strcpy(o.order.flightNum, flightNum);
    
    // 查找指定航班
    for(int j=0; j<i; j++){
        if(strcmp(flightNum, f[j].flightNum)==0){
            if(f[j].tickets >= 1){
                // 生成订单信息
                strcpy(o.order.take_off_Time, f[j].take_off_Time);
                strcpy(o.order.land_Time, f[j].land_Time);
                strcpy(o.order.take_off_City, f[j].take_off_City);
                strcpy(o.order.land_City, f[j].land_City);
                snprintf(o.orderId, MAX_ORDER_SIZE, "%s%s", f[j].flightNum, o.userId);
                o.order.price = f[j].now_Price;
                
                // 更新余票
                f[j].tickets--;
                found = 1;
                
                // 立即写入文件
                fprintf(op, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", 
                        o.orderId, o.name, o.userId, o.order.flightNum, 
                        o.order.take_off_Time, o.order.land_Time, 
                        o.order.take_off_City, o.order.land_City, o.order.price);
                fflush(op);  // 确保数据写入磁盘
                break;
            }
            else {
                printf("航班 %s 余票不足!当前余票: %d\n", flightNum, f[j].tickets);
                fclose(op);
                system("pause");
                return;
            }
        }
    }
    
    if(!found){
        printf("没有找到航班号为 %s 的航班!\n", flightNum);
        fclose(op);
        system("pause");
        return;
    }
    
    // 更新航班文件
    if((fp2=fopen("./flight/flight.csv","w"))==NULL){
        printf("无法打开航班文件!系统将返回上一级!\n");
        system("pause");
        fclose(op);
        return;
    }
    fprintf(fp2, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
    for(int j=0; j<i; j++){
        // 添加票数验证
        if(f[j].tickets < 0) f[j].tickets = 0;
        fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", 
                f[j].flightNum, f[j].take_off_Time, f[j].land_Time, 
                f[j].take_off_City, f[j].land_City, 
                f[j].pre_Price, f[j].discount, f[j].now_Price, f[j].tickets);
    }
    
    printf("成功为 %s 订购了航班 %s 的机票!\n", name, flightNum);
    UsefulPayment_Global +=1;
    fclose(fp2);
    fclose(op);
    system("pause");
}

void add_order(int showCheck){ //订票
    FLIGHT f[NUM];
    char str[SIZE];
    FILE* fp;
    int i = 0;
    int flag = 0;
    int n;
    float price;
    
    if(showCheck==1) check_flight();
    
    printf("请输入您想要订购的航班号: ");
    scanf("%s", str);
    
    // 修改为CSV文件读取
    if ((fp = fopen("./flight/flight.csv", "r")) == NULL) {
        printf("无法打开文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    
    // 跳过CSV标题行
    char header[256];
    fgets(header, sizeof(header), fp);
    
    // 修改为CSV格式解析
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n",f[i].flightNum, f[i].take_off_Time, f[i].land_Time,f[i].take_off_City, f[i].land_City, &f[i].pre_Price,&f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF) {
        i++;
    }
    fclose(fp);
    
    // 添加航班存在性判断
    int flightExists = 0;
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            flightExists = 1;
            break;
        }
    }
    
    if (!flightExists) {
        printf("航班号 %s 不存在!\n", str);
        system("pause");
        return;
    }
    
    // 保持原有查询逻辑
    for (int j = 0; j < i; j++) {
        if (strcmp(str, f[j].flightNum) == 0) {
            printf("查询到相关信息,如下所示:\n");
            printf("%-10s %-13s %-13s %-22s %-22s %-10s %-5s %-10s %-5s\n","航班号", "起飞时间", "降落时间", "起飞机场", "到达机场", "原价", "折扣", "现价", "余票");
            printf("%-10s %-13s %-13s %-22s %-22s %-10.2f %-5.2f %-10.02f %-5d\n",f[j].flightNum, f[j].take_off_Time, f[j].land_Time,f[j].take_off_City, f[j].land_City, f[j].pre_Price,f[j].discount, f[j].now_Price, f[j].tickets);
            flag = 1;
            price = f[j].now_Price;
        }
    }
    
    if(flag==1){
        // 修改为支持多人订票
        printf("\n请输入预计要订购的机票数: ");
        if(scanf("%d",&n) != 1 || n <= 0){
            printf("输入无效!\n");
            system("pause");
            return;
        }
        
        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        // 先为自己订票
        if(n >= 1) {
            // 直接使用查询到的航班号为自己订票
            add_order_for_others(userRealname_Global, userID_Global, str);
        }
        
        // 如果订票数量大于1，处理为他人订票
        if(n > 1) {
            for(int others = 1; others < n; others++) {
                char other_name[SIZE];
                char other_userId[SIZE];
                int valid = 0;
                
                printf("\n为他人订票 #%d:\n", others);
                printf("请输入乘客姓名: ");
                scanf("%s", other_name);
                
                // 清空输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                
                while(!valid) {
                    printf("请输入乘客身份证号: ");
                    scanf("%s", other_userId);
                    
                    // 清空输入缓冲区
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                    if(validateIDCard(other_userId)) {
                        valid = 1;
                    } else {
                        printf("身份证号格式不正确，请重新输入！\n");
                    }
                }
                
                // 使用相同的航班号为他人订票
                add_order_for_others(other_name, other_userId, str);
            }
        }
        PaymentSystem(price,UsefulPayment_Global);
        UsefulPayment_Global=0;
    }else if(flag==0) {
        printf("未找到匹配航班!\n");
        system("pause");
    };
    printf("\n");
}

void check_all(){
    ORDER o[NUM];
    FILE* op;
    int i=0;
    
    // 修改文件路径为 order.csv
    if((op=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开订单文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    
    // 跳过CSV标题行
    char header[512]; // 增加缓冲区大小
    if (!fgets(header, sizeof(header), op)) {
        printf("订单文件为空或格式错误!\n");
        fclose(op);
        system("pause");
        return;
    }
    
    printf("所有订单的信息如下所示:\n");
    printf("%-25s %-10s %-13s %-13s %-22s %-22s %-5s", "订单号", "姓名", "起飞时间", "降落时间", "起飞机场", "到达机场", "价格");
    
    // 使用更可靠的行读取方式
    char line[512];
    while(fgets(line, sizeof(line), op)) {
        // 彻底去除所有可能的换行符和回车符
        char* p = line;
        while(*p) {
            if(*p == '\r' || *p == '\n') {
                *p = 0;
                break;
            }
            p++;
        }
        
        // 使用sscanf解析整行，避免fscanf的格式问题
        char orderId[MAX_ORDER_SIZE], name[SIZE], userId[SIZE], flightNum[SIZE];
        char takeOffTime[SIZE], landTime[SIZE], takeOffCity[SIZE], landCity[SIZE];
        float price;
        
        if(sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", orderId, name, userId, flightNum, takeOffTime, landTime, takeOffCity, landCity, &price) == 9) {
            
            // 修改为不自动添加换行符的输出格式
            printf("\n%-25s %-10s %-13s %-13s %-22s %-22s %-7.2f", orderId, name, takeOffTime, landTime, takeOffCity, landCity, price);
            i++;
        }
    }
    
    if (i == 0) {
        printf("\n没有找到任何订单记录。");
    } else {
        printf("\n\n共找到 %d 条订单记录。", i);
    }
    
    printf("\n");
    fclose(op);
    system("pause");
}
////////2025-3-17 修改 将二进制dat格式转化为文本csv格式,以上代码测试无bug。（订票函数可能导致程序循环异常，需要解决！）已解决
void check_person(){
    ORDER o[NUM];
    FILE* op;
    int i = 0;
    int found = 0; // 添加标志，记录是否找到订单
    
    // 修改文件路径和打开模式
    if((op=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开订单文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    
    // 跳过CSV标题行
    char header[256];
    fgets(header, sizeof(header), op);
    
    printf("查询到 「%s」 相关信息，如下所示:\n", userRealname_Global);
    printf("%-25s %-7s %-13s %-13s %-22s %-22s %-10s\n", "订单号","航班号","起飞时间","降落时间","起飞机场","到达机场","价格");
    
    // 修正CSV解析格式 - 修复字段顺序和读取方式
    char line[512]; // 增加缓冲区大小，防止长行被截断
    
    while(fgets(line, sizeof(line), op)) {
        // 去除行尾换行符
        line[strcspn(line, "\n")] = 0;
        
        // 使用更可靠的方式解析CSV
        char orderId[MAX_ORDER_SIZE], name[SIZE], userId[SIZE], flightNum[SIZE];
        char takeOffTime[SIZE], landTime[SIZE], takeOffCity[SIZE], landCity[SIZE];
        float price;
        
        // 使用sscanf解析整行，确保格式匹配
        if(sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", 
                 orderId, name, userId, flightNum, takeOffTime, landTime, 
                 takeOffCity, landCity, &price) == 9) {
            
            // 精确比较用户ID
            if(strcmp(userId, userID_Global) == 0) {
                printf("%-25s %-7s %-13s %-13s %-22s %-22s %-10.2f\n", 
                       orderId, flightNum, takeOffTime, landTime, 
                       takeOffCity, landCity, price);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("未找到您的订单记录。\n");
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
    
    // 打开CSV文件
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开订单文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开航班文件!系统将返回上一级!\n");
        system("pause");
        fclose(op1);
        return;
    }
    
    // 读取航班数据
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF){
        i++;
    }
    
    // 修复点1：修正订单字段读取顺序
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[j].orderId, o[j].name, o[j].userId, o[j].order.flightNum, o[j].order.take_off_Time, o[j].order.land_Time, o[j].order.take_off_City, o[j].order.land_City, &o[j].order.price) != EOF){
        j++;
    }
    
    fclose(fp1);
    fclose(op1);
    
    // 用户交互流程
    check_order();
    printf("请输入需要修改订单的订单号: ");
    scanf("%s",str1);
    
    // 修复点2：清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // 查找订单
    for(k=0;k<j;k++){
        if(strcmp(str1,o[k].orderId)==0){
            system("cls");
            printf("已找到该订单,信息如下:\n");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10s\n", "订单号","航班号","起飞时间","降落时间","起飞机场","到达机场","价格");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10.2f\n", o[k].orderId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            strcpy(str2,o[k].order.flightNum);
            flag=1;
            x=k;
        }
    }
    
    if(flag==1){
        printf("\n请输入新航班的航班号: ");
        scanf("%s",str3);
        
        // 再次清空缓冲区
        while ((c = getchar()) != '\n' && c != EOF);

        // 查找新航班
        for(int a=0;a<i;a++){
            if(strcmp(str3,f[a].flightNum)==0){
                if(f[a].tickets>=1){
                    // 更新订单信息
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
                    printf("余票不足!\n\n");
                    system("pause");
                    return;
                }
            }
        }
        
        if(fflag==1){
            // 恢复旧航班票数
            for(int b=0;b<i;b++){
                if(strcmp(str2,f[b].flightNum)==0){
                    f[b].tickets++;
                }
            }
        }
        
        if(fflag==0){
            printf("该航班不存在!\n\n");
            system("pause");
            return;
        }
    }
    
    if(flag==0){
        printf("未找到该订单!\n\n");
        system("pause");
        return;
    }
    
    // 修复点3：写入文件并刷新缓冲区
    if(flag==1 && fflag==1){
        // 更新航班文件
        if((fp2=fopen("./flight/flight.csv","w"))==NULL){
            printf("无法打开航班文件!系统将返回上一级!\n");
            system("pause");
            return;
        }
        fprintf(fp2, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
        for(int c=0;c<i;c++){
            fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[c].flightNum, f[c].take_off_Time, f[c].land_Time, f[c].take_off_City, f[c].land_City, f[c].pre_Price, f[c].discount, f[c].now_Price, f[c].tickets);
        }
        fflush(fp2);
        fclose(fp2);
        
        // 更新订单文件
        if((op2=fopen("./flight/order.csv","w"))==NULL){
            printf("无法打开订单文件!系统将返回上一级!\n");
            system("pause");
            return;
        }
        fprintf(op2, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
        for(int c=0;c<j;c++){
            fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[c].orderId, o[c].name, o[c].userId, o[c].order.flightNum, o[c].order.take_off_Time, o[c].order.land_Time, o[c].order.take_off_City, o[c].order.land_City, o[c].order.price);
        }
        fflush(op2);
        fclose(op2);
        
        printf("\n订单修改成功!\n");
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

    // 修改为CSV文件读取
    if((op1=fopen("./flight/order.csv","r"))==NULL){
        printf("无法打开订单文件!系统将返回上一级!\n");
        system("pause");
        return;
    }
    if((fp1=fopen("./flight/flight.csv","r"))==NULL){
        printf("无法打开航班文件!系统将返回上一级!\n");
        fclose(op1);
        system("pause");
        return;
    }

    // 读取航班数据
    char flight_header[256];
    fgets(flight_header, sizeof(flight_header), fp1);
    while(fscanf(fp1, "%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%d\n", f[i].flightNum, f[i].take_off_Time, f[i].land_Time, f[i].take_off_City, f[i].land_City, &f[i].pre_Price, &f[i].discount, &f[i].now_Price, &f[i].tickets) != EOF){
        i++;
    }

    // 读取订单数据
    char order_header[256];
    fgets(order_header, sizeof(order_header), op1);
    while(fscanf(op1, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n", o[j].orderId, o[j].name, o[j].userId, o[j].order.flightNum, o[j].order.take_off_Time, o[j].order.land_Time, o[j].order.take_off_City, o[j].order.land_City, &o[j].order.price) != EOF){
        j++;
    }

    fclose(fp1);
    fclose(op1);

    check_person();
    printf("请输入订单号: ");
    scanf("%s",str);
    
    // 清空输入缓冲区
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    for(int k=0;k<j;k++){
        if(strcmp(str,o[k].orderId)==0){
            printf("\n已找到该订单信息:\n");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10s\n", "订单号","航班号","起飞时间","降落时间","起飞机场","到达机场","价格");
            printf("%-25s %-7s %-18s %-18s %-10s %-10s %-10.2f\n", o[k].orderId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
            
            printf("确认要退票吗？(1 确认/0 取消):");
            scanf("%d",&c);
            if(c==1){
                // 更新航班余票
                for(int x=0;x<i;x++){
                    if(strcmp(o[k].order.flightNum,f[x].flightNum)==0){
                        f[x].tickets++;
                        break;
                    }
                }
                
                // 删除订单记录
                for(int x=k;x<j-1;x++){
                    o[x] = o[x+1];
                }
                j--;

                // 写入更新后的航班文件
                if((fp2=fopen("./flight/flight.csv","w"))==NULL){
                    printf("无法更新航班文件!\n");
                    system("pause");
                    return;
                }
                fprintf(fp2, "航班号,起飞时间,降落时间,起飞机场,到达机场,原价,折扣,现价,余票\n");
                for(int x=0;x<i;x++){
                    fprintf(fp2, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%d\n", f[x].flightNum, f[x].take_off_Time, f[x].land_Time, f[x].take_off_City, f[x].land_City, f[x].pre_Price, f[x].discount, f[x].now_Price, f[x].tickets);
                }
                fclose(fp2);

                // 写入更新后的订单文件
                if((op2=fopen("./flight/order.csv","w"))==NULL){
                    printf("无法更新订单文件!\n");
                    system("pause");
                    return;
                }
                fprintf(op2, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
                for(int x=0;x<j;x++){
                    fprintf(op2, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[x].orderId, o[x].name, o[x].userId, o[x].order.flightNum, o[x].order.take_off_Time, o[x].order.land_Time, o[x].order.take_off_City, o[x].order.land_City, o[x].order.price);
                }
                fclose(op2);

                printf("\n退票成功!\n");
                
                // 记录删除订单到历史文件
                FILE* history_fp = fopen("./flight/deleted_orders.csv", "a+");
                if(history_fp) {
                    if(ftell(history_fp) == 0) { // 如果是新文件写入标题
                        fprintf(history_fp, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
                    }
                    fprintf(history_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", o[k].orderId, o[k].name, o[k].userId, o[k].order.flightNum, o[k].order.take_off_Time, o[k].order.land_Time, o[k].order.take_off_City, o[k].order.land_City, o[k].order.price);
                    fclose(history_fp);
                }
                
                flag=1;
                system("pause");
                break;
            }
            else {
                printf("取消退票!\n");
                system("pause");
                return;
            }
        }
    }

    if(flag==0){
        printf("未找到该订单!\n");
        system("pause");
    }
}



// 凯撒加密函数
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

// 凯撒解密函数
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

// 修改密码设置逻辑（示例修改 New_password 函数）
void New_password() {
    FILE* file = fopen(FILENAME, "w");
    char* new_password = Getpassword(); // 获取动态内存
    char encrypted_password[MAX_PASSWORD_LENGTH];
    
    if (!file) {
        free(new_password); // 异常时释放内存
        printf("无法打开文件进行写入。\n");
        return;
    }

    strncpy(encrypted_password, new_password, MAX_PASSWORD_LENGTH); // 安全拷贝
    free(new_password); // 使用后立即释放
    
    caesar_encrypt(encrypted_password, 3);
    fprintf(file, "%s\n", encrypted_password);
    fclose(file);
    printf("密码已成功修改。\n");
    system("pause");
}
// 验证密码
int verify_password() {
	FILE* file = fopen(FILENAME, "r");
	if (!file) {
		printf("密码文件不存在，请先设置密码。\n");
		system("pause");
		New_password();
		return 2;
	}

	
	char stored_password[MAX_PASSWORD_LENGTH];
	char input[MAX_PASSWORD_LENGTH];
	if (fgets(stored_password, sizeof(stored_password), file)) {
		stored_password[strcspn(stored_password, "\n")] = '\0'; // 去掉换行符
		caesar_decrypt(stored_password, 3); // 解密存储的密码
		fclose(file);
		int i=5;
		do {
			printf("请输入密码：");
			//scanf("%19s", input);  // 限制输入长度，避免缓冲区溢出
			strcpy(input, Getpassword());
			if (strcmp(input, stored_password) != 0) {  // 如果输入密码不等于真实密码
				if (i > 1) {  // 如果还有剩余尝试机会
					printf("密码错误！你还有%d次输入机会。\n\n", i - 1);
				}
				else {  // 如果没有剩余尝试机会
					printf("密码错误！你没有剩余尝试机会了。\n\n");
				}
			}
			else {  // 如果输入密码正确
				fclose(file);
				printf("密码正确！\n\n");
				//system("cls");
				return 0;  // 成功退出程序
			}

			i--;  // 减少尝试次数
		} while (i > 0);  // 如果还有剩余尝试次数，继续循环
		fclose(file);
		printf("密码输入错误次数过多，程序退出。\n\n");
		system("pause");
		return 1;

		
	}
}

// 修改密码
void change_password() {
	
	int state= verify_password();
	if (state ==1) return;//先验证原密码
	else if (state == 2) ;

	FILE* file = fopen(FILENAME, "w");
	char new_password[MAX_PASSWORD_LENGTH];
	printf("请输入新密码：");
	//scanf("%s",new_password);
	
	strcpy(new_password,Getpassword());
	
	if (!file) {
		printf("无法打开文件进行写入。\n");
		return;
	}

	char encrypted_password[MAX_PASSWORD_LENGTH];
	strcpy(encrypted_password, new_password);//比较密码
	caesar_encrypt(encrypted_password, 3); // 加密新密码
	fprintf(file, "%s\n", encrypted_password);//写入新密码

	fclose(file);
	printf("密码已成功修改。\n");
	system("pause");
}

// ... 已有代码 ...

char* Getpassword()//密码星号保密处理
{
    char* password = (char*)malloc(MAX_PASSWORD_LENGTH * sizeof(char)); // 改为动态内存分配
    memset(password, 0, MAX_PASSWORD_LENGTH);
    int i = 0;
    char c;
    
    while (1) {
        c = _getch();
        if (c == '\r') {
            break;
        } else if (c == '\b') {
            if (i > 0) {  // 添加边界检查
                printf("\b \b");
                --i;
            }
        } else if (i < MAX_PASSWORD_LENGTH - 1) {  // 防止缓冲区溢出
            password[i++] = c;
            printf("*");
        }
    }
    printf("\n");
    return password;
}

// ... 后续代码 ...

//Getpassword函数引用自网络，做连接处理后放入，引用链接：https://zhuanlan.zhihu.com/p/109099710

int validateIDCard(const char *id) { //身份证验证函数
    // 系数表
    const int coefficient[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    // 校验码表
    const char checkCode[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
    
    // 检查长度是否为18
    if (strlen(id) != 18) {
        return 0;
    }
    
    // 检查前17位是否都是数字
    for (int i = 0; i < 17; i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }
    
    // 检查校验码
    int sum = 0;
    for (int i = 0; i < 17; i++) {
        sum += (id[i] - '0') * coefficient[i];
    }
    
    int remainder = sum % 11;
    char correctCheckCode = checkCode[remainder];
    
    // 第18位可能是X(罗马数字10)或x，需要转换为大写比较
    char lastChar = toupper(id[17]);
    
    if (lastChar != correctCheckCode) {
        return 0;
    }
    
    // 检查日期是否合法
    char yearStr[5] = {0};
    char monthStr[3] = {0};
    char dayStr[3] = {0};
    
    strncpy(yearStr, id + 6, 4);
    strncpy(monthStr, id + 10, 2);
    strncpy(dayStr, id + 12, 2);
    
    int year = atoi(yearStr);
    int month = atoi(monthStr);
    int day = atoi(dayStr);
    
    // 简单的日期验证
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // 检查特定月份的天数
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return 0;
    } else if (month == 2) {
        // 简单闰年判断
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear) {
            if (day > 29) return 0;
        } else {
            if (day > 28) return 0;
        }
    }
    
    return 1;
}

char* getGenderFromID(const char* id) { //性别判断函数
    static char gender[10];
    // 身份证第17位，奇数为男性，偶数为女性
    int genderCode = id[16] - '0';
    if (genderCode % 2 == 1) {
        strcpy(gender, "男");
    } else {
        strcpy(gender, "女");
    }
    return gender;
}

void user_register() {
    FILE* fp = fopen("./flight/users.csv", "a+"); // 改为CSV文件
    USER newUser = {0};

    // 用户名查重逻辑
    do {
        printf("请输入用户名：");
        scanf("%39s", newUser.username);
        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // 检查用户名长度
        if(strlen(newUser.username) >= SIZE-1) {
            printf("用户名过长(最大39字符)，请重新输入!\n");
            continue;
        }

        // CSV格式查重
        int duplicate = 0;
        FILE* checkFp = fopen("./flight/users.csv", "r");
        if(checkFp) {
            char line[256];
            fgets(line, sizeof(line), checkFp); // 跳过标题行
            while(fgets(line, sizeof(line), checkFp)) {
                char existingName[SIZE] = {0};
                sscanf(line, "%[^,],", existingName); // 只读取用户名字段
                if(strcmp(newUser.username, existingName) == 0) {
                    printf("该用户名已存在，请重新输入！\n");
                    duplicate = 1;
                    break;
                }
            }
            fclose(checkFp);
        }
        if(!duplicate) break;
    } while(1);

    // 获取密码
    printf("请输入密码：");
    char* pwd = Getpassword();
    strncpy(newUser.password, pwd, MAX_PASSWORD_LENGTH);
    

    printf("请再次输入密码：");
    while(1) {
        char* confirmPwd = Getpassword();
        if(strcmp(pwd, confirmPwd) == 0) {
            free(confirmPwd);
            free(pwd);
            break;
        } else {
            printf("两次输入的密码不一致，请重新输入！\n");
            free(confirmPwd);
        }
    }

    printf("请输入11位手机号：");
    while(1){
        scanf("%s", newUser.phone);
        if(strlen(newUser.phone)==11){
            break;
        }else{
            printf("手机号非法，请重新输入！\n");
        }

    }
    
    printf("请输入手机验证码（6位）：");
    while(1){
        char input_code[7]={0};
        scanf("%s", input_code);
        if(strcmp(input_code, "")!=0 && strlen(input_code)==6){
            printf("验证码正确！\n");
            break;
        }else{
            printf("验证码错误或非法，请重新输入！\n");
        }

    }

    // 获取其他信息
    printf("请输入姓名：");
    scanf("%s", newUser.name);
    
    while(1) {
        printf("请输入身份证号：");
        scanf("%s", newUser.userId);
        if(validateIDCard(newUser.userId)) {
            strcpy(newUser.gender, getGenderFromID(newUser.userId));
            printf("系统检测到您的性别为：%s\n", newUser.gender);
            break;
        } else {
            printf("身份证号格式不正确，请重新输入！\n");
        }
    }
    

    // 写入CSV文件
    if(fp) {
        // 如果是新文件，写入标题
        fseek(fp, 0, SEEK_SET);
        char first_line[256];
        if(!fgets(first_line, sizeof(first_line), fp)) { // 文件为空时写入表头
            fprintf(fp, "用户名,密码,姓名,身份证号,性别,手机号\n");
        }
        fseek(fp, 0, SEEK_END);  // 重置到文件末尾准备追加

        // 写入用户数据（增加性别字段）
        fprintf(fp, "%s,%s,%s,%s,%s,%s\n", newUser.username, newUser.password, newUser.name, newUser.userId, newUser.gender,newUser.phone);
        fclose(fp);
    } else {
        printf("无法打开用户数据库！\n");
    }

    printf("注册成功！\n");
    system("pause");
}

int user_login() {
    FILE* fp = fopen("./flight/users.csv", "r"); // 改为CSV文件
    USER currentUser = {0};
    char inputName[SIZE] = {0};
    char* inputPwd = NULL;
    int found = 0;

    printf("请输入用户名：");
    scanf("%s", inputName);
    
    // 清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if(fp) {
        char line[256];
        fgets(line, sizeof(line), fp); // 跳过标题行
        
        // CSV格式查询
        while(fgets(line, sizeof(line), fp)) {
            char storedName[SIZE] = {0};
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", storedName, currentUser.password, currentUser.name, currentUser.userId,currentUser.gender,currentUser.phone);

            if(strcmp(inputName, storedName) == 0) {
                found = 1;
                printf("请输入密码：");
                inputPwd = Getpassword();
                
                if(strcmp(inputPwd, currentUser.password) == 0) {
                    free(inputPwd);
                    fclose(fp);
                    printf("登录成功！\n");
                    
                    // 存储全局信息
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
        printf("用户不存在，是否立即注册？(y/n): ");
        char choice = getchar();
        while((c = getchar()) != '\n' && c != EOF); // 清空缓冲区

        if(choice == 'y' || choice == 'Y') {
            user_register();
            printf("请使用新账号重新登录！\n");
            system("pause");
            return 0;
        }
    } else {
        printf("密码错误！\n");
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

    // 读取CSV数据
    if(fp) {
        char line[256];
        fgets(line, sizeof(line), fp); // 跳过标题行
        while(fgets(line, sizeof(line), fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%s", users[count].username, users[count].password, users[count].name, users[count].userId);
         count++;
        }
        fclose(fp);
    } else {
        printf("无法打开用户文件！\n");
        system("pause");
        return;
    }

    // 获取用户输入
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入身份证号：");
    scanf("%19s", userId);
   
    
    // 清理输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // 查找并修改密码
    for(int i = 0; i < count; i++) {
        if(strcmp(username, users[i].username) == 0 && 
           strcmp(userId, users[i].userId) == 0) 
        {
            printf("请输入新密码：");
            char* tmp = Getpassword();
            strncpy(newPwd, tmp, MAX_PASSWORD_LENGTH);
            free(tmp);

            strncpy(users[i].password, newPwd, MAX_PASSWORD_LENGTH);
            found = 1;
            break;
        }
    }

    // 写入更新后的数据
    if(found) {
        fp = fopen("./flight/users.csv", "w");
        if(fp) {
            fprintf(fp, "用户名,密码,姓名,身份证号\n");
            for(int i = 0; i < count; i++) {
                fprintf(fp, "%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId);
            }
            fclose(fp);
            printf("密码修改成功！\n");
        } else {
            printf("文件更新失败！\n");
        }
    } else {
        printf("用户验证失败！\n");
    }
    system("pause");
}

void list_all_users() {
    FILE* fp = fopen("./flight/users.csv", "r");
    if (!fp) {
        printf("暂无用户数据！\n");
        system("pause");
        return;
    }

    printf("\n%-40s %-20s %-20s %-5s %-13s\n", "用户名", "姓名", "身份证号", "性别", "手机号");
    printf("----------------------------------------------------------\n");
    printf("----------------------------------------------------------\n");

    char line[256];
    fgets(line, sizeof(line), fp); // 跳过标题行
    
    while(fgets(line, sizeof(line), fp)) {
        USER user = {0};
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", user.username, user.password, user.name, user.userId, user.gender, user.phone);
        
        printf("%-40s %-20s %-20s %-5s %-13s\n", user.username, user.name, user.userId, user.gender, user.phone);
    }
    fclose(fp);
    system("pause");
}

// 新增注销账户函数
void delete_account() {
    FILE* user_fp = fopen("./flight/users.csv", "r+");
    FILE* order_fp = fopen("./flight/order.csv", "r+");
    USER users[100];
    ORDER orders[100];
    int user_count = 0, order_count = 0;
    char confirm[10];

    // 读取CSV用户数据
    if(user_fp) {
        char line[256];
        fgets(line, sizeof(line), user_fp); // 跳过标题行
        while(fgets(line, sizeof(line), user_fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", users[user_count].username, users[user_count].password, users[user_count].name, users[user_count].userId,users[user_count].gender,users[user_count].phone);
            user_count++;
        }
        fclose(user_fp);
    }

    // 读取CSV订单数据
    if(order_fp) {
        char line[512];
        fgets(line, sizeof(line), order_fp); // 跳过标题行
        while(fgets(line, sizeof(line), order_fp)) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f", orders[order_count].orderId, orders[order_count].name, orders[order_count].userId, orders[order_count].order.flightNum, orders[order_count].order.take_off_Time, orders[order_count].order.land_Time, orders[order_count].order.take_off_City, orders[order_count].order.land_City, &orders[order_count].order.price);
            order_count++;
        }
        fclose(order_fp);
    }

    // 确认操作
    printf("确定要永久删除账户%s吗？(y/n): ", username_Global);
    scanf("%s", confirm);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if(strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0) {
        return;
    }

    // 删除用户数据
    int new_user_count = 0;
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].username, username_Global) != 0) {
            users[new_user_count++] = users[i];
        }
    }

    // 修复点1：使用正确的订单历史文件头
    FILE* history_fp = fopen("./flight/deleted_orders.csv", "a+");
    if(history_fp) {
        // 检查文件是否为空
        fseek(history_fp, 0, SEEK_END);
        if(ftell(history_fp) == 0) {
            fprintf(history_fp, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
        }
        fseek(history_fp, 0, SEEK_END);
        
        for(int i = 0; i < order_count; i++) {
            if(strcmp(orders[i].userId, userID_Global) == 0) {
                fprintf(history_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", orders[i].orderId, orders[i].name, orders[i].userId, orders[i].order.flightNum, orders[i].order.take_off_Time, orders[i].order.land_Time, orders[i].order.take_off_City, orders[i].order.land_City, orders[i].order.price);
            }
        }
        fclose(history_fp);
    }

    // 删除关联订单
    int new_order_count = 0;
    for(int i = 0; i < order_count; i++) {
        if(strcmp(orders[i].userId, userID_Global) != 0) {
            orders[new_order_count++] = orders[i];
        }
    }

    // 修复点2：使用正确的用户文件头
    user_fp = fopen("./flight/users.csv", "w");
    if(user_fp) {
        fprintf(user_fp, "用户名,密码,姓名,身份证号,性别,手机号\n"); // 修正后的正确表头
        for(int i = 0; i < new_user_count; i++) {
            fprintf(user_fp, "%s,%s,%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId,users[i].gender,users[i].phone);
        }
        fclose(user_fp);
    }

    // 写入更新后的订单数据
    order_fp = fopen("./flight/order.csv", "w");
    if(order_fp) {
        fprintf(order_fp, "订单号,姓名,用户ID,航班号,起飞时间,降落时间,起飞机场,到达机场,价格\n");
        for(int i = 0; i < new_order_count; i++) {
            fprintf(order_fp, "%s,%s,%s,%s,%s,%s,%s,%s,%.2f\n", orders[i].orderId, orders[i].name, orders[i].userId, orders[i].order.flightNum, orders[i].order.take_off_Time, orders[i].order.land_Time, orders[i].order.take_off_City, orders[i].order.land_City, orders[i].order.price);
        }
        fclose(order_fp);
    }

    // 修复点3：使用正确的注销用户历史文件头
    FILE* del_fp = fopen("./flight/deleted_users.csv", "a+");
    if(del_fp) {
        fseek(del_fp, 0, SEEK_END);
        if(ftell(del_fp) == 0) {
            fprintf(del_fp, "用户名,密码,姓名,身份证号,性别,手机号\n"); // 修正后的正确表头
        }
        for(int i = 0; i < user_count; i++) {
            if(strcmp(users[i].username, username_Global) == 0) { fprintf(del_fp, "%s,%s,%s,%s,%s,%s\n", users[i].username, users[i].password, users[i].name, users[i].userId,users[i].gender,users[i].phone);
                break;
            }
        }
        fclose(del_fp);
    }

    // 清空全局变量
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
        printf("无法打开用户数据文件！\n");
        system("pause");
        return;
    }
    
    // 读取所有用户数据
    char line[256];
    fgets(line, sizeof(line), fp); // 跳过标题行
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%s", 
               users[count].username, 
               users[count].password, 
               users[count].name, 
               users[count].userId,
               users[count].gender,
               users[count].phone);
        
        // 找到当前登录用户
        if(strcmp(users[count].username, username_Global) == 0) {
            userIndex = count;
        }
        count++;
    }
    fclose(fp);
    
    if(userIndex == -1) {
        printf("未找到当前用户信息，请重新登录！\n");
        system("pause");
        return;
    }
    
    // 显示当前用户信息
    system("cls");
    printf("=== 当前用户信息 ===\n");
    printf("用户名: %s\n", users[userIndex].username);
    printf("姓名: %s\n", users[userIndex].name);
    printf("性别: %s\n", users[userIndex].gender);
    printf("手机号: %s\n", users[userIndex].phone);
    printf("身份证号: %s\n", users[userIndex].userId);
    printf("=====================\n\n");
    printf("\n由于身份证号的特殊绑定性，如需修改请注销账号重新注册\n");
    
    printf("请选择要修改的信息：\n");
    printf("1. 姓名\n");
    printf("2. 手机号\n");
    printf("3. 密码\n");
    printf("0. 返回\n");
    printf("请选择: ");
    
    // 清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    choice = getchar();
    while ((c = getchar()) != '\n' && c != EOF); // 再次清空缓冲区
    
    switch(choice) {
        case '1': {
            // 修改姓名
            char newName[SIZE];
            printf("请输入新姓名: ");
            scanf("%s", newName);
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("确认将姓名从 \"%s\" 修改为 \"%s\"？(y/n): ", users[userIndex].name, newName);
            char confirm = getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(confirm == 'y' || confirm == 'Y') {
                strcpy(users[userIndex].name, newName);
                strcpy(userRealname_Global, newName); // 更新全局变量
                printf("姓名修改成功！\n");
            } else {
                printf("已取消修改。\n");
            }
            break;
        }
        case '2': {
            // 修改手机号
            char newPhone[12];
            printf("请输入新手机号(11位): ");
            scanf("%s", newPhone);
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(strlen(newPhone) != 11) {
                printf("手机号必须为11位！\n");
                system("pause");
                return;
            }
            
            printf("确认将手机号从 \"%s\" 修改为 \"%s\"？(y/n): ", users[userIndex].phone, newPhone);
            char confirm = getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            
            if(confirm == 'y' || confirm == 'Y') {
                strcpy(users[userIndex].phone, newPhone);
                strcpy(userPhone_Global, newPhone); // 更新全局变量
                printf("手机号修改成功！\n");
            } else {
                printf("已取消修改。\n");
            }
            break;
        }
        case '3': {
            // 修改密码
            printf("请输入当前密码: ");
            char* currentPwd = Getpassword();
            
            if(strcmp(currentPwd, users[userIndex].password) != 0) {
                printf("当前密码错误！\n");
                free(currentPwd);
                system("pause");
                return;
            }
            free(currentPwd);
            
            printf("请输入新密码: ");
            char* newPwd = Getpassword();
            
            printf("请再次输入新密码: ");
            char* confirmPwd = Getpassword();
            
            if(strcmp(newPwd, confirmPwd) != 0) {
                printf("两次输入的密码不一致！\n");
                free(newPwd);
                free(confirmPwd);
                system("pause");
                return;
            }
            
            strcpy(users[userIndex].password, newPwd);
            printf("密码修改成功！\n");
            
            free(newPwd);
            free(confirmPwd);
            break;
        }case '0':return;
        default:
            printf("无效选择！\n");
            system("pause");
            return;
    }
    
    // 将更新后的用户信息写回文件
    fp = fopen("./flight/users.csv", "w");
    if(!fp) {
        printf("无法打开用户数据文件进行写入！\n");
        system("pause");
        return;
    }
    
    // 写入CSV表头
    fprintf(fp, "用户名,密码,姓名,身份证号,性别,手机号\n");
    
    // 写入所有用户数据
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
	//打印管理界面
	if (verify_password()) return;
	while (1) {
		system("cls");
		printf("                 @@@@@***************************欢迎使用航班管理系统***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###—————————————————————————————————— 管 理 员 ————————---———————————————————————###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 @@@                              A 修 改 航 班 信 息                             @@@\n");
		printf("                 ###                              B 删 除 航 班 信 息                             ###\n");
		printf("                 @@@                              C 录 入 航 班 信 息                             @@@\n");
		printf("                 @@@                              D 修 改 管 理 密 码                             @@@\n");
		printf("                 @@@                              E 查 看 所 有 订 单                             @@@\n");
		printf("                 @@@                              F 查 看 所 有 用 户                             @@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###———————————————————————————————--------------—---------------| 0 退出管理 |———###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("请选择: ");

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
	//打印主界面
	while(1){
		system("cls");
		printf("                 | 用户名：%s |---| 姓名：%s |---| 手机号：%s |\n",username_Global,userRealname_Global,userPhone_Global);
        printf("                 ###———————————————————————————————————------------————| 信息有误？按（*）修改 |——###\n");
        printf("                 ###———————————————————————————————————------------————| \033[31m!不可逆\033[0m按（#）注销账户|——###\n");
        printf("                                                                                                     \n");
		printf("                 @@@@@***************************欢迎使用航班订票系统***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###———————————————————————————————————------------———————————————————————————————###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 订            票                              ###\n");
		printf("                 @@@                              2 查 看 全 部 航 班                             @@@\n");
		printf("                 @@@                              3 查   看   订   单                             @@@\n");
		printf("                 ###                              4 修   改   订   单                             ###\n");
		printf("                 @@@                              5 退            票                              @@@\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###                                                                              ###\n");
		printf("                 ###———————————————————————————————---------------—------------- | 0 退出登录 |———###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("请选择: ");
		
		scanf("%c", &i);
		//功能选择
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
	strcpy(username_Global, "");  //退出登录后初始化信息
	strcpy(userID_Global, "");    
	strcpy(userRealname_Global, "");
	//打印主界面
	while(1){
		system("cls");
		printf("                 @@@@@***************************欢迎登录航班订票系统***************************@@@@@\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###———————————————————————————————————------------———————————————————————————————###\n");
		printf("                 ###                                                                              ###\n");
		printf("                 ###                              1 登            录                              ###\n");
		printf("                 @@@                              2 注            册                              @@@\n");
		printf("                 ###                              3 忘   记   密   码                             ###\n");
		printf("                 ###                                                                              ###\n");	
		printf("                 ###———————————————————————————————--------------—| * 管理员选项 | 0 退出程序 |———###\n");
		printf("                 @@@@@**************************************************************************@@@@@\n");
		printf("请选择: ");
		
		scanf("%c", &i);
		//功能选择
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
