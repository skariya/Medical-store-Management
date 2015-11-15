all: project

project: 
	gcc function.c -o project
clear:
	rm -rf *o project

