/*after returning the medicine mark * on the old bill*/

/*****************************************************************************
 * Copyright (C) SHAMBHOO A. KARIYA shambhoo123.sk@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct medical {
	char medi_name[20];
	int quantity;
	float cost;
	int exp_day;
	int exp_month;
	int exp_year;
	int rack;
}temp;

struct record{
	char medi_name[20];
	int quantity;
	float cost;
	float total;	
}r;
	
struct date {
	int d;
	int m;
	int y;	
}dt;

int ID;	

void getcust_id(char *);
void stock_up(char *);
void medi_sale();
void check_stock();
void medi_search(char *);
float bill(char *, struct medical *, int, struct date);
void cust_record(char *, struct medical *, int, float, struct date); 
void check_record();
void medi_return();
void search_record(char *);
void exp_remainder();
void stock_remainder();
void medi_remainder();
void medi_purchase();
void check_rack();
void dealer_record(char *);
void medi_remove(char *);
int check_exp(int, int, int);

int main() {
	int ID = 0, q= 0;
	
	char str[16], name[16], adr[48];
	int x, y;
	
	printf("\n-----------------------------------------------------------------\n	Welcome to KARIYA MEDICAL STORE\n		AAJ NAGAD KAL UDHAR\n\n");
	printf("enter Today's Date\n");
	scanf("%d%d%d", &dt.d, &dt.m, &dt.y);
	exp_remainder();
	medi_remainder();
	while(1){
		printf("1.purchase new medicine\n2.sale medicine\n3.check records of dealers\n4.search medicine\n5.check stock of medicines\n6.Diary of old customer's record\n7.Check medicine in rack no.\n8.return the medicine\n9.Check record of old customer in the Diary\n10.exit\n");
		scanf("%d", &x);
		switch(x) {
			case 1:
				medi_purchase();
				break;
			case 2:
				medi_sale();
				break;
			case 3:
				printf("enter the name of dealer\n");
				scanf(" %[^\n]s", name);
				printf("enter adress of dealer\n");
				scanf(" %[^\n]s", adr);
				strcat(name, adr);
				strcat(name, ".txt");
				dealer_record(name);
				break;
			case 4:
				printf("enter name of medicine\n");
				scanf(" %[^\n]s", str);
				medi_search(str);
				break;

			case 5:
				check_stock();
				break;
			case 6:
				check_record();
				break;
			case 7:
				printf("enter rack no.\n");
				scanf("%d", &y);
				check_rack(y);
				break;
			case 8:
				medi_return();
				break;
			case 9:
				printf("enter customer's name\n");
				scanf(" %[^\n]s", str);
				search_record(str);
				break;
			case 10:
				return 0;
		}
	}
	return 0;
}

void medi_purchase() {
	char name[16];
	char adr[48];
	printf("enter the name of dealer\n");
	scanf(" %[^\n]s", name);
	printf("enter adress\n");
	scanf(" %[^\n]s", adr);
	FILE *fp;
	strcat(name, adr);
	strcat(name, ".txt");
	fp = fopen(name, "a+");
	fclose(fp);	
	fp = fopen(name, "a");
	fwrite(&dt, sizeof(dt), 1, fp);
	fclose(fp);
	stock_up(name);
}

void dealer_record(char *name) {
	FILE *fp;
	int count, i;
	float total = 0;
	struct date day;
	fp = fopen(name, "r");
	if(fp == NULL) {
		printf("No record with this dealer\n");
		return;	
	}
	while((fread(&day, sizeof(struct date), 1, fp)) == 1) {
		fread(&count, sizeof(count), 1, fp);
		printf("-------------------------------------------------------------------------------------\n");
		printf("\n			DATE-%d-%d-%d\n", day.d, day.m, day.y);
		for(i = 0; i < count; i++) {
			fread(&r, sizeof(r), 1, fp);
			printf("%s	%d	%f	%f\n", r.medi_name, r.quantity, r.cost, r.total);
			total = total + r.total;
		}
		printf("				TOTAL = Rs.%f\n\n", total);
		printf("---------------------------------------------------------------------------------------\n");
		total = 0;
	}	
	fclose(fp);
}
void getcust_id(char *str) {
	FILE *fp;
	fp = fopen("custinfo.txt", "a");
	fwrite(str, sizeof(str), 1, fp);
	fclose(fp); 
	
}

void stock_up(char *name) {
	FILE *fp, *fd;
	fp = fopen("stock.txt", "a+");
	fclose(fp);
	fd = fopen(name, "a");
	int t = 0, i = 0, x = 0, y, count = 0, z = 1;
	struct medical data;
	struct record rec[100];

	printf("enter information about medicine in the following format\n\n");
	printf("name of medicine\n");
	printf("quantity of medicines\n");
	printf("cost of each medicine packet\n");
	printf("expire date in day month year\n");
	printf("rack no in which medicine get putted\n");

	while(1) {
		
		CONT :

		scanf(" %[^\n]", data.medi_name);
		scanf("%d", &data.quantity);
		scanf("%f", &data.cost);
		scanf("%d%d%d", &data.exp_day, &data.exp_month, &data.exp_year);
		z = check_exp(data.exp_day, data.exp_month, data.exp_year);
		if(z == 0) { 
			printf("*Cant update stock*, This Medicine is expired!!\n");
			printf("1.If you have same medicine which is not expired\n2.continue with other medicine\n3.exit\n");
			scanf("%d", &x);
		}
 				
		if(x == 1 || x == 2) {
			if(x == 1) 
				printf("reenter all information, start from medicine name\n");
			if(x == 2)
				printf("continue with the information, start from name\n");
			x = 0;
			goto CONT;
			
		}
		
		strcpy(rec[count].medi_name, data.medi_name);
		rec[count].quantity = data.quantity;
		rec[count].cost = data.cost;
		rec[count].total = data.quantity * data.cost;
		count++;
	
		fp = fopen("stock.txt", "rb+");	
		while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
			if((strcmp(data.medi_name, temp.medi_name)) == 0) {
				printf("(Put the medicine in Rack no- %d)\nenter?\n", temp.rack);
				scanf("%d", &data.rack);
				data.quantity = data.quantity + temp.quantity;
				fseek(fp, -sizeof(temp), SEEK_CUR);
				fwrite(&data, sizeof(struct medical), 1, fp);
				t = -1;
				break;
			}
		}	
		if(t != -1) {
			printf("(Put the medicine in Rack no- [%d])\nenter?\n", temp.rack + 1);
			scanf("%d", &data.rack);
			fclose(fp);
			fp = fopen("stock.txt", "a");
			fwrite(&data, sizeof(struct medical), 1, fp);
			fclose(fp);
			
		}
		else
			fclose(fp);
		t = 0;
		printf("enter 1 to continue & 2 to stop");
		scanf("%d", &y);
		if(y == 2) {
			fwrite(&count, sizeof(count), 1, fd);
			for(i = 0; i < count; i++)
				fwrite(&rec[i], sizeof(struct record), 1, fd);
			fclose(fd);
			return;
		}
	}
	
} 

void medi_sale() {
	float total;
	char str[16];
	int n, i = 0, j = 0, c = 0, q = 0, z, quantity;
	int day, month, year;
	
	
	printf("enter your full name\n");
	scanf(" %[^\n]s", str);
	getcust_id(str);

	struct data{
		char medi_name[20];
		int quantity;	
	}d[10];
	
	struct medical x[10];

	FILE *fp;
	fp = fopen("stock.txt", "rb+");
	printf("enter the name of medicines you want & no. of packets of the medicine\n");
		while((scanf("%s%d", d[i].medi_name, &d[i].quantity)) != EOF)
			i++;
	for(n = 0; n < i; n++)
		printf("[%s]	[%d]\n", d[n].medi_name, d[n].quantity);
	

	for(n = 0; n < i; n++) {
		while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
			if((strcmp(d[n].medi_name, temp.medi_name)) == 0) {
				if(temp.quantity <= d[n].quantity) {
					if(temp.quantity < d[n].quantity) {
						z = check_exp(temp.exp_day, temp.exp_month, temp.exp_year);
						if(z == 0) {
							printf("Sorry, Cant give [%s] medicine, as it is expired", temp.medi_name);
							break;
						}
						check_exp(temp.exp_day, temp.exp_month, temp.exp_year);	
						printf("%s medicine is available in rack no. %d\n", d[n].medi_name, temp.rack);
						printf("Sorry, can give only [%d]-packets of %s medicines\n", temp.quantity, d[n].medi_name);
						x[j] = temp;
						x[j].quantity = temp.quantity;
						temp.quantity = 0;				
					}
					else {
						printf("Sorry, stock of [%s] medicine is not available", d[n].medi_name);
						break;	
					}
				}
				else {
					z = check_exp(temp.exp_day, temp.exp_month, temp.exp_year);
					if(z == 0) {
						printf("Sorry, Cant give [%s] medicine, as it is expired", temp.medi_name);
						break;
					}	
					printf("[%s] medicine is available in rack no. [%d]\n", d[n].medi_name, temp.rack);
					temp.quantity = temp.quantity - d[n].quantity;
					q = -1;
					x[j] = temp;
					x[j].quantity = d[n].quantity;
				}				
				fseek(fp, -sizeof(temp), SEEK_CUR);
				fwrite(&temp, sizeof(temp), 1, fp);
				j++;
				if(temp.quantity == 0) {
					printf("[%s] medicine is not available now\n", d[n].medi_name);
					break;
				}
				break;
			}			
		}
		if(q != -1) {
			printf("[%s] medicine is not available", d[n].medi_name);
			q = 0;
		}
		rewind(fp);
	}	
	rewind(fp);
	fclose(fp);
	total = bill(str, x, j, dt);
	cust_record(str, x, j, total, dt);

}

void check_stock() {
	FILE *fp;	
	fp = fopen("stock.txt", "r");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}
	printf("\n\n			STOCK OF MEDICINES\n-------------------------------------------------------------------------------\n");
	while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
		printf("[%s]	%d-Packets	Cost-[Rs.%f]	EXP-%d/%d/%d	Rack-[%d]\n", temp.medi_name, temp.quantity, temp.cost, temp.exp_day, temp.exp_month, temp.exp_year, temp.rack);
	}
	printf("-------------------------------------------------------------------------------\n\n");	
	fclose(fp);
	
}


float bill(char *name, struct medical *x, int j, struct date dte) {
	int i, ID = 0, q = 0;
	float total = 0;
	FILE *fp;
	fp = fopen("id.txt", "a+");
	fread(&ID, sizeof(ID), 1, fp);
	fclose(fp);
	fp = fopen("id.txt", "w");
	if(ID == 0) {
		ID = 1000;
		fwrite(&ID, sizeof(ID), 1, fp);	
	}
	else{
		ID = ID + 1;
		fwrite(&ID, sizeof(ID), 1, fp);	
	}
	fclose(fp);

	printf("-------------------------------------------------------------------------------------\n");
	printf("	KARIYA MEDICAL STORES \n");
	printf("\n			DATE-%d-%d-%d\n", dte.d, dte.m, dte.y);
	printf("		CUSTOMER'S ID - %d\n", ID);
	printf("CUSTOMER'S NAME - %s\n", name);	
	for(i = 0; i < j; i++) {
		printf("[%s]	[%d]	Rs.%f	Rs.%f\n", x[i].medi_name, x[i].quantity, x[i].cost, x[i].quantity * x[i].cost);
		total = total + x[i].quantity * x[i].cost;
	}
	printf("bill is of				Rs.%f\n\n", total);
	printf("---------------------------------------------------------------------------------------\n");
	return total;
}

void medi_search(char *str) {
		int q = 0;
		FILE *fp;
		fp = fopen("stock.txt", "r");
		if(fp == NULL) {
			printf("stock is empty\n");
			return;		
		}
		while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
			if(strcmp(str, temp.medi_name) == 0) {
				if(temp.quantity == 0) {
					printf("%s medicine is not available\n", str);
					break;
				}
				printf("%s medicine is available in rack no. %d\n", str, temp.rack);
				q = -1;
				break;
			}			
		}

		if(q != -1) {
			printf("%s medicine is not available", str);
			q = 0;
		}
		fclose(fp);	
}

void cust_record(char *str, struct medical *x, int n, float total, struct date dte) {
	int i = 0, ID;
	char name[16];
	strcpy(name, str);
		
	FILE *fd;
	fd = fopen("id.txt", "r");
	if(fd == NULL) {
		return;	
	}
	fread(&ID, sizeof(ID), 1, fd);
	fclose(fd);

	FILE *fp;	
	fp = fopen("record.txt", "a");
	
	fwrite(&ID, sizeof(ID), 1, fp);
	fwrite(name, sizeof(name), 1, fp);
	fwrite(&n, sizeof(n), 1, fp);
	fwrite(&dte, sizeof(struct date), 1, fp);
	
	for(i = 0; i < n; i++) {
		strcpy(r.medi_name, x[i].medi_name);
		r.quantity = x[i].quantity;
		r.cost = x[i].cost;
		r.total = x[i].quantity * x[i].cost;
		fwrite(&r, sizeof(struct record), 1, fp);
	}

	fwrite(&total, sizeof(total), 1, fp);
	
	fclose(fp);
}

void check_record() {
	char name[16];
	int n, i, ID;
	float total;
	FILE *fp;
	fp = fopen("record.txt", "r");
	if(fp == NULL) {
		printf("No record\n");
		return;	
	}
	while((fread(&ID, sizeof(ID), 1, fp)) == 1) {
		fread(name, sizeof(name), 1, fp);
		printf("-----------------------------------------------------------------------------\n");
		printf("	KARIYA MEDICAL STORES \n");
		fread(&n, sizeof(n), 1, fp);
		fread(&dt, sizeof(struct date), 1, fp);
		printf("\n			DATE-%d-%d-%d\n", dt.d, dt.m, dt.y);
		printf("		CUSTOMER'S ID - %d\n", ID);
		printf("CUSTOMER'S NAME - %s\n", name);
		for(i = 0; i < n; i++) {
			fread(&r, sizeof(struct record), 1, fp);
			printf("[%s]	[%d]	Rs.%f	Rs.%f\n", r.medi_name, r.quantity, r.cost, r.total);
		}
		fread(&total, sizeof(total), 1, fp);
		printf("bill is of 				Rs.%f\n\n", total);
		printf("-----------------------------------------------------------------------------\n");
		
	}
	fclose(fp);
	
}

void check_rack(int rack) {
	FILE *fp;
	int x = 0;
	fp = fopen("stock.txt", "r");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}
	while(fread(&temp, sizeof(temp), 1, fp) == 1) {
		if(rack == temp.rack) {
			printf("%d %s medicine is available in rack no. %d\n", temp.quantity, temp.medi_name, temp.rack);
			x = -1;
			break;
		}
	}
	if(x != -1)
		printf("No medicine is available in the rack\n");
}

void search_record(char *str) {
	char name[16];
	FILE *fp;
	fp = fopen("record.txt", "r");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}
	int q = 0, n, i;
	float total;
	while((fread(&ID, sizeof(ID), 1, fp)) == 1) {
		fread(name, sizeof(name), 1, fp);
		if(strcmp(name, str) == 0) {
			printf("-----------------------------------------------------------------------------\n");
			printf("	KARIYA MEDICAL STORES \n");
			fread(&n, sizeof(n), 1, fp);
			fread(&dt, sizeof(struct date), 1, fp);
			printf("\n			DATE-%d-%d-%d\n", dt.d, dt.m, dt.y);
			printf("		CUSTOMER'S ID - %d\n", ID);
			printf("CUSTOMER'S NAME - %s\n", name);
			for(i = 0; i < n; i++) {
				fread(&r, sizeof(struct record), 1, fp);
				printf("[%s]	[%d]	Rs.%f	Rs.%f\n", r.medi_name, r.quantity, r.cost, r.total);
			}
			fread(&total, sizeof(total), 1, fp);
			printf("bill is of				Rs.%f\n\n", total);
			printf("-----------------------------------------------------------------------------\n\n");
			q = -1;
	
		}
		else {
			fread(&n, sizeof(n), 1, fp);
			fread(&dt, sizeof(struct date), 1, fp);	
			for(i = 0; i < n; i++) 
				fread(&r, sizeof(struct record), 1, fp);
			fread(&total, sizeof(total), 1, fp);
		}
	}

	if(q == 0)
		printf("Customer's name is not found in the Diary\n\n");
	fclose(fp);	
}

void medi_return() {
	int ID, id, i, j, k, y, n, q = 0, m = 0, p = 0;
	char name[16], str[16], medi_name[20];
	float total, t;
	printf("Please enter your Customer's ID\n");
	scanf("%d", &id);

	struct data{
		char medi_name[20];
		int quantity;	
	}d[10];
	struct record{
		char medi_name[20];
		int quantity;
		float cost;
		float total;	
	}rec[10];

	struct medical x[10];

	FILE *fp;
	fp = fopen("record.txt", "r");
	if(fp == NULL) {
		printf("No record exist\n");
		return;	
	}

	while((fread(&ID, sizeof(ID), 1, fp)) == 1) {
		if(ID == id) {
			printf("Customer's ID matches succesfully\n");
			printf("Enter Customer's name on the bill you have\n");
			scanf(" %[^\n]", str);
			fread(name, sizeof(name), 1, fp);	
			if(strcmp(str, name) == 0) {
				printf("Customer's name also matches\nYou are our customer\n\n");
			}
			else {
				printf("Sorry, Name on the bill is different\nYour bill is fake\n");
				return;
			}
			printf("-----------------------------------------------------------------------------\n");
			printf("	KARIYA MEDICAL STORES \n");
			
			fread(&n, sizeof(n), 1, fp);
			fread(&dt, sizeof(struct date), 1, fp);
			printf("\n			DATE-%d-%d-%d\n", dt.d, dt.m, dt.y);
			printf("		CUSTOMER'S ID - %d\n", ID);
			printf("CUSTOMER'S NAME - %s\n", name);
			for(i = 0; i < n; i++) {
				fread(&r, sizeof(struct record), 1, fp);
				printf("%s	%d	%f	%f\n", r.medi_name, r.quantity, r.cost, r.total);
			}
			fread(&total, sizeof(total), 1, fp);
			printf("bill is of Rs.			%f\n\n", total);
			printf("-----------------------------------------------------------------------------\n");
			t = total;
			q = -1;
			break;
			
		}
		else {
			fread(name, sizeof(name), 1, fp);
			fread(&n, sizeof(n), 1, fp);
			fread(&dt, sizeof(struct date), 1, fp);
			for(i = 0; i < n; i++) 
				fread(&r, sizeof(struct record), 1, fp);
			fread(&total, sizeof(total), 1, fp);
		}
			
	}

	if(q == 0) {
		printf("Customer's ID is not found in the Diary\nSorry, you are not our customer\n\n");
		return;
	}
	if(q = -1) {
		fseek(fp, -sizeof(total), SEEK_CUR);
		for(i = 0; i < n; i++) 
			fseek(fp, -sizeof(r), SEEK_CUR);

		printf("Enter the name of medicine & no. of packets you want to return\n");
		i = 0;
		while((scanf("%s%d", d[i].medi_name, &d[i].quantity)) != EOF)
			i++;
		y = i;
		for(j = 0; j < n; j++) {
			fread(&r, sizeof(struct record), 1, fp);
			for(k = 0; k < i; k++) {
				if(strcmp(d[k].medi_name, r.medi_name) == 0) {
					if(d[k].quantity <= r.quantity) {
						p = -1;
						break;
					}
					else
						printf("quantity exceeded\n");
				}
			}
			if(p == -1) {
				strcpy(rec[m].medi_name , r.medi_name);
				rec[m].quantity = r.quantity - d[k].quantity;
				rec[m].cost = r.cost;
				m++;
			}
			if(p == 0) {
				strcpy(rec[m].medi_name , r.medi_name);
				rec[m].quantity = r.quantity;
				rec[m].cost = r.cost;
				m++;
			}
			p = 0;
		}		
	}

	for(i = 0; i < m; i++) {
		strcpy(x[i].medi_name, rec[i].medi_name);
		x[i].quantity = rec[i].quantity;
		x[i].cost = rec[i].cost;
	}
	
	total = bill(name, x, m, dt);
	cust_record(name, x, j, total, dt);
	t = t - total;
	printf("Give Rs.%f back to customer\n", t);	
	fclose(fp);	
	fp = fopen("stock.txt", "rb+");
	for(i = 0; i < y; i++) {
		while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
			if((strcmp(d[i].medi_name, temp.medi_name)) == 0) {
				temp.quantity = d[i].quantity + temp.quantity;
				fseek(fp, -sizeof(temp), SEEK_CUR);
				fwrite(&temp, sizeof(struct medical), 1, fp);
				rewind(fp);
				break;
			}
		}
	}	
	fclose(fp);
}

void exp_remainder() {
	int x = 0;
	FILE *fp;
	fp = fopen("stock.txt", "r");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}

	while(fread(&temp, sizeof(struct medical), 1, fp)) {
		if(dt.y > temp.exp_year) {
			printf("Medicine [%s] is expired!!\n", temp.medi_name);
			printf("1.Remove it\n2.Not now\n");
			scanf("%d", &x);
			if(x == 1) {
				medi_remove(temp.medi_name);
				x = 0;
			}
		}
		if(dt.y == temp.exp_year) {
			if(dt.m > temp.exp_month) {
				printf("Medicine [%s] is expired!!\n", temp.medi_name);
				printf("1.Remove it\n2.Not now\n");
				scanf("%d", &x);
				if(x == 1) {
					medi_remove(temp.medi_name);
					x = 0;
				}
			}
			else if((dt.m + 4) > temp.exp_month)
				printf("Medicine [%s] is going to expire in 4 months!!\n", temp.medi_name);
		}	
	}
	fclose(fp);
}

void medi_remainder() {
	int x = 0;
	FILE *fp;
	fp = fopen("stock.txt", "r");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}

	while(fread(&temp, sizeof(struct medical), 1, fp)) {
		if(temp.quantity < 5) {
			if(temp.quantity == 0) {
				printf("Stock of [%s] medicine is over\n", temp.medi_name);
				printf("1.Purchase [%s] medicines?\n2.Not now\n", temp.medi_name);
				scanf("%d", &x);
				if(x == 1) {
					medi_purchase();
					printf("stock get uodated\n");
					x = 0;
				}
			}
			else {
				printf("Only [%d]-packets of [%s] medicine are available now!!\n", temp.quantity, temp.medi_name);
				printf("1.Purchase [%s] medicines?\n2.Not now\n", temp.medi_name);
				scanf("%d", &x);
				if(x == 1) {
					medi_purchase();
					printf("stock get uodated\n");
					x = 0;
				}
			}
		}
		
	}
	fclose(fp);
}

void medi_remove(char *str) {
	int q = 0;
	FILE *fp;
	char medi[20];
	strcpy(medi, str); 	
	fp = fopen("stock.txt", "rb+");
	if(fp == NULL) {
		printf("stock is empty\n");
		return;	
	}
				
	while(fread(&temp, sizeof(temp), 1, fp) == 1) {
		if((strcmp(medi, temp.medi_name)) == 0) {
			printf("%s\n", temp.medi_name);
			temp.quantity = 0;
			fseek(fp, -sizeof(temp), SEEK_CUR);
			fwrite(&temp, sizeof(temp), 1, fp);
			printf("Medicine %s is removed\n", str);
			q = -1;
			break;
		}
	}
	if(q != -1) {
		printf("%s medicine is not in available", str);
		q = 0;
	}
	fclose(fp);
}

int check_exp(int d, int m, int y) {
	if(dt.y >= y) { 
			if(dt.y > y) {
				return 0;
			}
 			else if(dt.y == y) {
				if(dt.m >= m) 
					return 0;	
			}
			else 
				return 1;
	}
	else 
		return 1;
		
}

