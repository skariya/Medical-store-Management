#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct medical {
	char medi_name[20];
	int quantity;
	float cost;
	char prod_date[15];
	char exp_date[15];
	int rack;
}temp;

struct record{
	char cust_name[16];
	float rup;	
}r;

void getcust_id(char *);
void stock_up();
void medi_sale();
void check_stock();
void medi_search(char *);
float bill(struct medical *, int);
void cust_record(char *, float);
void check_record();
void medi_purchase();

int main() {
	char str[16];
	int x;
	printf("	Welcome to KARIYA Medical Store	\n");

	while(1){
		printf("1.purchase new medicine\n2.sale medicine\n3.update stock of medicine\n4.search medicine\n5.check stock of medicines\n6.search for medicine\n7.old customer's record\n8.exit\n");
		scanf("%d", &x);
		switch(x) {
			case 1:
				medi_purchase();
				break;
			case 2:
				medi_sale();
				break;
			case 3:
				stock_up();
				break;
			case 4:
				break;
			case 5:
				check_stock();
				break;
			case 6:
				printf("enter name of medicine\n");
				scanf(" %[^\n]s", str);
				medi_search(str);
				break;
			case 7:
				check_record();
				break;
			case 8:
				return 0;
		}
	}
	return 0;
}

void getcust_id(char *str) {
	FILE *fp;
	fp = fopen("custinfo.txt", "a");
	fwrite(str, sizeof(str), 1, fp);
	fclose(fp);
	fp = fopen("custinfo.txt", "r");
	/*while(fread(str, sizeof(str), 1, fp) == 1) {
		printf("%s\n", str);
	}*/
	fclose(fp);
}

void stock_up() {
	FILE *fp;
	int t = 0;
	struct medical data;
/*enter information about medicine*/
	printf("enter name of medicine\n");
	scanf(" %[^\n]s", data.medi_name);
	printf("enter quantity of medicines\n");
	scanf("%d", &data.quantity);
	printf("enter cost of each medicine packet\n");
	scanf("%f", &data.cost);
	printf("enter production date\n");
	scanf(" %[^\n]s", data.prod_date);
	printf("enter expire date\n");
	scanf(" %[^\n]", data.exp_date);
	printf("enter rack no in which medicine get putted\n");
	scanf("%d", &data.rack);
	
	fp = fopen("stock.txt", "a+");
	fclose(fp);
	fp = fopen("stock.txt", "rb+");
		
	while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
		if((strcmp(data.medi_name, temp.medi_name)) == 0) {
			data.quantity = data.quantity + temp.quantity;
			fseek(fp, -sizeof(temp), SEEK_CUR);
			fwrite(&data, sizeof(struct medical), 1, fp);
			t = -1;
			break;
		}
	}

	fclose(fp);

	if(t != -1) {
		fp = fopen("stock.txt", "a");
		fwrite(&data, sizeof(struct medical), 1, fp);
		fclose(fp);
	}
	
	check_stock();
} 

void medi_sale() {
	float total;
	char str[28];
	int n, i = 0, j = 0, c = 0, q = 0, quantity;
	
	printf("enter your full name\n");
	scanf(" %[^\n]s", str);
	getcust_id(str);

	struct data{
		char medi_name[24];
		int quantity;	
	}d[10];
	
	struct medical x[10];

	FILE *fp;
	fp = fopen("stock.txt", "rb+");
	printf("enter the name of medicines you want & no. of packets of the medicine\n");
		while((scanf("%s%d", d[i].medi_name, &d[i].quantity)) != EOF)
			i++;
	for(n = 0; n < i; n++)
		printf("%s	%d\n", d[n].medi_name, d[n].quantity);
	

	for(n = 0; n < i; n++) {
		while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
			if((strcmp(d[n].medi_name, temp.medi_name)) == 0) {
				if(temp.quantity < d[n].quantity) {
					printf("Sorry, can give only %d %s medicines\n", temp.quantity, d[n].medi_name);
					break;
				}
				printf("%s medicine is available in rack no. %d\n", d[n].medi_name, temp.rack);
				temp.quantity = temp.quantity - d[n].quantity;
				q = -1;
				x[j] = temp;
				x[j].quantity = d[n].quantity;				
				fseek(fp, -sizeof(temp), SEEK_CUR);
				fwrite(&temp, sizeof(temp), 1, fp);
				j++;
				if(temp.quantity == 0) {
					printf("%s medicine is not available now\n", d[n].medi_name);
					break;
				}
				break;
			}			
		}
		if(q != -1) {
			printf("%s medicine is not available", d[n].medi_name);
			q = 0;
		}
		rewind(fp);
	}	
	rewind(fp);
	total = bill(x, j);
	cust_record(str, total);

}

void check_stock() {
	FILE *fp;	
	fp = fopen("stock.txt", "r");
	
	while(fread(&temp, sizeof(struct medical), 1, fp) == 1) {
		printf("%s, %d, %f, %s, %s, %d\n", temp.medi_name, temp.quantity, temp.cost, temp.prod_date, temp.exp_date, temp.rack);
	}	
	fclose(fp);
	
}


float bill(struct medical *x, int j) {
	int i;
	float total = 0;
	i = 0;
		
	for(i = 0; i < j; i++) {
		printf("%s	%d	%f	%f\n", x[i].medi_name, x[i].quantity, x[i].cost, x[i].quantity * x[i].cost);
		total = total + x[i].quantity * x[i].cost;
	}
	printf("bill is of Rs.%f\n", total);
	return total;
}

void medi_search(char *str) {
		int q = 0;
		FILE *fp;
		fp = fopen("stock.txt", "r");
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

void cust_record(char *str, float total) {
	
	strcpy(r.cust_name, str);
	r.rup = total;
	FILE *fp;
	fp = fopen("record.txt", "a");
	fwrite(&r, sizeof(r), 1, fp);
	fclose(fp);

}

void check_record() {
	FILE *fp;
	fp = fopen("record.txt", "r");
	while((fread(&r, sizeof(struct record), 1, fp)) == 1)
		printf("%s	Rs=%f\n", r.cust_name, r.rup);
}

void medi_purchase() {
	char str[16];
	printf("enter name of seller");
	scanf(" %[^\n]s", str);
		
}

