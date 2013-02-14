Kim Cooperrider
CS 1550
Project 1
Fall 2013

README

--COMPILATION INSTRUCTIONS--
To compile this code, just run the following command:
	gcc myrecs.c -o myrecs


--RUNNING INSTRUCTIONS--
Run program with following command: 
	./myrecs
You should see a prompt asking for a command. Valid commands include:
	ins <studId> <courseId> <courseName> <grade>
	load <inputFile>
	find <studId>
	gpa <studId>
	gpa_range <studentId_a> <studentId_b>
	top <numCourses>
	range <studentId_a> <studentId_b>
	exit
	print (this will print the leftmost child)
	printtree (will print the tree)
	leaves (will traverse from leftmost leaf node to right most, printing out keys)

--OTHER FILES INCLUDED--
Some test files have been included. Samples include:
	few_records.txt
	test_data.txt
	more_records.txt


