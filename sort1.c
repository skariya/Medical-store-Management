#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct record {
	char name[15];
	int age;
	float marks;
	} record;
int main() {
	record G[104];
	char temp[18];
	int i = 0, m =0, n;
	FILE *fp;
	fp = fopen("new.txt", "r");
	while(!feof(fp)) {
		fscanf(fp, " %s%d%f", G[i].name, &G[i].age, &G[i].marks);
		printf("%s %d\n", G[i].name, G[i].age);
		i++;
		}
	
	// sorting according to name
	for(i = 0; i < n; i++){
        	for(j = (i + 1); j < n; j++) {
 	        	if((strcmp(G[j].name, G[i].name)) < 0) {
	  	        	strcpy(temp, G[i].name);
			        strcpy(G[i].name, G[j].name);
				strcpy(G[j].name, temp);
			
			}
	}
	for(m = 0; m < i; m++) {
		printf("%s\n", G[m].name);		
	}
 // sorting according to age
 for(i = 0; i < n; i++) {
        	for(j = (i + 1); j < n; j++) {
 	        	if(G[j].age < G[i].age) {
	  	        	temp = G[i].age;
			        G[i].age = G[j].age;
				G[j].age = temp;
			} 
		}
 	for(m = 0; m < i; m++) {
		printf("%d\n", G[m].age);
		}
for(i = 0; i < n; i++) {
        	for(j = (i + 1); j < n; j++) {
 	        	if(G[j].marks < G[i].marks) {
	  	        	temp = G[i].marks;
			        G[i].marks = G[j].marks;
				G[j].marks = temp;
			} 
		}
}
 	for(m = 0; m < i; m++) {
		printf("%f\n", G[m].marks);
		}
