assembler: Main.o Tables.o Get_Input_Commands.o Assistance_Functions.o First_Iteration.o Second_Iteration.o Operands_Handle.o
	gcc -g -ansi -Wall -pedantic Main.o Tables.o Get_Input_Commands.o Assistance_Functions.o First_Iteration.o Second_Iteration.o Operands_Handle.o -o assembler -lm

Main.o: Main.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Main.c -o Main.o -lm

Tables.o: Tables.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Tables.c -o Tables.o -lm

Get_Input_Commands.o: Get_Input_Commands.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Get_Input_Commands.c -o Get_Input_Commands.o -lm

Assistance_Functions.o: Assistance_Functions.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Assistance_Functions.c -o Assistance_Functions.o -lm

First_Iteration.o: First_Iteration.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic First_Iteration.c -o First_Iteration.o -lm

Second_Iteration.o: Second_Iteration.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Second_Iteration.c -o Second_Iteration.o -lm

Operands_Handle.o: Operands_Handle.c Header.h Error_Handle.h Data_Structure.h Tables.h
	gcc -c -ansi -Wall -pedantic Operands_Handle.c -o Operands_Handle.o -lm
