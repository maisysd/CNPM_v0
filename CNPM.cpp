#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMPLOYEES 100
#define DATA_FILE "employees.dat"
#define REPORT_FILE "salary_report.txt"

// Cau truc nhan vien
typedef struct {
    int id;
    char name[50];
    double base_salary;
    double bonus;
    double tax;
    double insurance;
    double allowance;
    char membership_card[20];
    int dependents;
    int region;
} Employee;

Employee employees[MAX_EMPLOYEES];
int employee_count = 0;

// Muc luong toi thieu theo vung
const double MIN_SALARY[] = {4680000, 4160000, 3640000, 3250000};

// Ham tinh thue thu nhap ca nhan
double calculate_tax(double income, int dependents) {
    double taxable_income = income - 11000000 - (dependents * 4400000);
    if (taxable_income <= 0) return 0;
    if (taxable_income <= 5000000) return taxable_income * 0.05;
    if (taxable_income <= 10000000) return taxable_income * 0.1 - 250000;
    if (taxable_income <= 18000000) return taxable_income * 0.15 - 750000;
    if (taxable_income <= 32000000) return taxable_income * 0.2 - 1650000;
    if (taxable_income <= 52000000) return taxable_income * 0.25 - 3250000;
    if (taxable_income <= 80000000) return taxable_income * 0.3 - 5850000;
    return taxable_income * 0.35 - 9850000;
}

// Ham tinh bao hiem
double calculate_insurance(double salary) {
    return salary * 0.105;
}

// Ham ghi du lieu vao file
void save_data() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file) {
        fwrite(&employee_count, sizeof(int), 1, file);
        fwrite(employees, sizeof(Employee), employee_count, file);
        fclose(file);
    }
}

// Ham doc du lieu tu file
void load_data() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file) {
        fread(&employee_count, sizeof(int), 1, file);
        fread(employees, sizeof(Employee), employee_count, file);
        fclose(file);
    }
}

// Ham them nhan vien
void add_employee() {
    if (employee_count >= MAX_EMPLOYEES) {
        printf("Danh sach nhan vien da day!\n");
        return;
    }
    Employee emp;
    printf("Nhap ID: ");
    scanf("%d", &emp.id);
    getchar();
    printf("Nhap ten: ");
    fgets(emp.name, sizeof(emp.name), stdin);
    emp.name[strcspn(emp.name, "\n")] = 0;
    printf("Nhap vung (1-4): ");
    scanf("%d", &emp.region);
    printf("Nhap luong co ban: ");
    scanf("%lf", &emp.base_salary);
    if (emp.base_salary < MIN_SALARY[emp.region - 1]) {
        printf("Canh bao: Luong thap hon muc luong toi thieu vung!\n");
    }
    printf("Nhap thuong: ");
    scanf("%lf", &emp.bonus);
    printf("Nhap so nguoi phu thuoc: ");
    scanf("%d", &emp.dependents);
    emp.insurance = calculate_insurance(emp.base_salary);
    emp.tax = calculate_tax(emp.base_salary + emp.bonus, emp.dependents);
    printf("Nhap tro cap: ");
    scanf("%lf", &emp.allowance);
    getchar();
    printf("Nhap loai the thanh vien: ");
    fgets(emp.membership_card, sizeof(emp.membership_card), stdin);
    emp.membership_card[strcspn(emp.membership_card, "\n")] = 0;
    
    employees[employee_count++] = emp;
    save_data();
    printf("Da them nhan vien!\n");
}

// Ham hien thi danh sach nhan vien
void display_employees() {
    printf("\n===== DANH SACH NHAN VIEN =====\n");
    for (int i = 0; i < employee_count; i++) {
        double net_salary = employees[i].base_salary + employees[i].bonus - employees[i].tax - employees[i].insurance + employees[i].allowance;
        printf("ID: %d\nTen: %s\n", employees[i].id, employees[i].name);
        printf("Luong co ban: %.2lf\nThuong: %.2lf\nTro cap: %.2lf\n", employees[i].base_salary, employees[i].bonus, employees[i].allowance);
        printf("Bao hiem: %.2lf\nThue: %.2lf\nLuong thuc nhan: %.2lf\n\n", employees[i].insurance, employees[i].tax, net_salary);
    }
}

// Ham tao bao cao luong
void generate_salary_report() {
    FILE *file = fopen(REPORT_FILE, "w");
    if (file) {
        fprintf(file, "===== BAO CAO LUONG NHAN VIEN =====\n");
        for (int i = 0; i < employee_count; i++) {
            double net_salary = employees[i].base_salary + employees[i].bonus - employees[i].tax - employees[i].insurance + employees[i].allowance;
            fprintf(file, "ID: %d\nTen: %s\n", employees[i].id, employees[i].name);
            fprintf(file, "Luong co ban: %.2lf\nThuong: %.2lf\nTro cap: %.2lf\n", employees[i].base_salary, employees[i].bonus, employees[i].allowance);
            fprintf(file, "Bao hiem: %.2lf\nThue: %.2lf\nLuong thuc nhan: %.2lf\n\n", employees[i].insurance, employees[i].tax, net_salary);
        }
        fclose(file);
        printf("Da tao bao cao luong: %s\n", REPORT_FILE);
    } else {
        printf("Loi khi tao bao cao luong!\n");
    }
}

// Main function
int main() {
    int choice;
    load_data();
    printf("\n===== HE THONG TINH LUONG VA PHUC LOI =====\n");
    do {
        printf("\n1. Them nhan vien\n2. Hien thi danh sach\n3. Tao bao cao luong\n4. Thoat\nChon: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: add_employee(); break;
            case 2: display_employees(); break;
            case 3: generate_salary_report(); break;
        }
    } while (choice != 4);
    return 0;
}

