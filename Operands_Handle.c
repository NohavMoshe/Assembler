#include "Header.h"

void second_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char* command, char file_name[], int* IC, int number_of_line) {	/*group with one operand*/
	char first_com_loc[LineLength], is_hashtag[LabelLength] = { "\0" }, is_ampersand[LabelLength] = { "\0" };	/*'first_com_loc' gives us the first location of the command, and the rest*/
	char* all_registers[NumOfRegisters] = { "r0" ,"r1" ,"r2" ,"r3" ,"r4" ,"r5" ,"r6", "r7" };
	char operand2[LineLength];
	int i = 0, j = 0;
	boolean exit = FALSE;						/*exit will get true when we will get error messeage*/

	strcpy(first_com_loc, strstr(input, command));
	while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0') 								/*keep going untill there is space*/
		i++;
	while ((isspace(first_com_loc[i]) || first_com_loc[i] == '\0') && !exit) {								/*keep going untill there is char that isn't space*/
		if (first_com_loc[i++] == '\0') {
			add_input_error_message(the_command_has_no_operands, number_of_line, file_name);
			exit = TRUE;
			--i;			/*to make sure that we will not access uallocated memory*/
		}
	}
	if (first_com_loc[i] == '&' && strcmp(command, "jmp") && strcmp(command, "bne") && strcmp(command, "jsr") && !exit) {
		/*if the command different from jmp or bne or jsr, and there is &, so exit*/
		add_input_error_message(the_command_is_written_illegally, number_of_line, file_name);
		exit = TRUE;
	}
	else if (first_com_loc[i] == '#' && strcmp(command, "prn") && !exit) {					/*if the operand is number, so exit*/
		add_input_error_message(destination_operand_cannot_be_number, number_of_line, file_name);
		exit = TRUE;
	}
	else if (isdigit(first_com_loc[i]) && !exit) {				/*in case there is a number without '#' before it*/
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	else if (first_com_loc[i] == '#' && !exit) {
		is_hashtag[0] = first_com_loc[i++];
		is_hashtag[1] = '\0';
	}
	else if (first_com_loc[i] == '&' && !exit) {
		is_ampersand[0] = first_com_loc[i++];
		is_ampersand[1] = '\0';
	}
	while (((isalnum(first_com_loc[i])) || first_com_loc[i] == '-' || first_com_loc[i] == '+') && !exit) {
		operand2[j++] = first_com_loc[i++];
	}
	operand2[j] = '\0';
	if (!isalnum(first_com_loc[i]) && !isspace(first_com_loc[i]) && first_com_loc[i] != '\0' && !exit) {			/*check if the operand has illegal character*/
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	while (isspace(first_com_loc[i]) != 0 && first_com_loc[i] != '\0' && !exit) 		/*keep going untill you get '\0' or character*/
		i++;
	if (first_com_loc[i] != ';' && first_com_loc[i] != '\0' && !exit) {				/*check if we get character that is not ; or '\0'*/
		add_input_error_message(extraneous_syntax, number_of_line, file_name);
		return;
	}
	if (exit)
		return;
	for (j = 0; j < NumOfRegisters && !exit; j++) {										/*check if the operand is register */
		if (!strcmp(all_registers[j], operand2)) {
			if (label[0])
				add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
			add_to_code_linked_list(hptr_code, input, commands_table, command, NULL, operand2, IC, 1, file_name);
			exit = TRUE;
		}
	}/*in case the operand isn't register*/
	if (!exit) {
		if (label[0])		/*check if there is label*/
			add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
		if (is_hashtag[0] != '\0') {		/*check if there is '#'*/
			add_to_code_linked_list(hptr_code, input, commands_table, command, NULL, strcat(is_hashtag, operand2), IC, 2, file_name);
			exit = TRUE;
		}
		if (!exit) {
			add_to_operand_label_list(hptr_operand_label, operand2, number_of_line, file_name);		/*comes here if there is only operand label*/
			if (is_ampersand[0] != '\0') {		/*check if there is '&'*/
				add_to_code_linked_list(hptr_code, input, commands_table, command, NULL, strcat(is_ampersand, operand2), IC, 2, file_name);
				exit = TRUE;
			}
		}
		if (!exit)
			add_to_code_linked_list(hptr_code, input, commands_table, command, NULL, operand2, IC, 2, file_name);	/*comes here if there is no '&'*/
	}
}

void first_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char command[], char file_name[], int* IC, int number_of_line) {	/*group with 2 operands*/
	char first_com_loc[LineLength];		/*'first_com_loc' gives us the first location of the command, and the rest*/
	char* all_registers[NumOfRegisters] = { "r0" ,"r1" ,"r2" ,"r3" ,"r4" ,"r5" ,"r6", "r7" };
	char operand1[LineLength], operand2[LineLength];				/*string that will contain the 2 operands*/
	int i = 0, j = 0, temp = 0;								/*temp is for saving us the '#'*/
	boolean exit = FALSE;									/*exit will get true when we will get error messeage*/

	strcpy(first_com_loc, strstr(input, command));
	while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0') 									/*keep going untill there is space*/
		i++;
	while ((isspace(first_com_loc[i]) || first_com_loc[i] == '\0') && !exit) {									/*keep going untill there is char that isn't space*/
		if (first_com_loc[i++] == '\0') {
			add_input_error_message(the_command_has_no_operands, number_of_line, file_name);
			exit = TRUE;
			--i;			/*to make sure that we will not access uallocated memory*/
		}
	}
	if (first_com_loc[i] == '&' && !exit) {
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	while (((isalnum(first_com_loc[i])) || first_com_loc[i] == '#') && first_com_loc[i] != ',' && !exit) { 	/*keep going untill there is no char or ','*/
		if (first_com_loc[i] == '#') 								/*if there is '#' we need to know that we dealing with number*/
			temp = '#';
		operand1[j++] = first_com_loc[i++];
		if (first_com_loc[i - 1] == '#' && (first_com_loc[i] == '-' || first_com_loc[i] == '+'))		/*check if there is a minus number, if yes, copy it*/
			operand1[j++] = first_com_loc[i++];
		if (temp == '#') {												/*if we have number, check if it written correctly*/
			if ((first_com_loc[i] - '0' > 9 || first_com_loc[i] - '0' < 0) && first_com_loc[i] != ',' && !isspace(first_com_loc[i])) {
				add_input_error_message(operand_is_illegal, number_of_line, file_name);
				exit = TRUE;
			}
		}
	}
	operand1[j] = '\0';
	j = 0;
	temp = 0;
	if (atoi(operand1) && !exit) {					/*in case the number has no '#' before him*/
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	while (isspace(first_com_loc[i]) && !exit) {
		if (first_com_loc[i++] == '\0') {								/*meaning there is only one operand*/
			add_input_error_message(operand_is_missing, number_of_line, file_name);
			exit = TRUE;
		}
	}
	if (!isalnum(first_com_loc[i]) && first_com_loc[i] != ',' && !exit) {
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	if (!exit) {
		if (first_com_loc[i++] != ',' && !exit) {
			add_input_error_message(no_comma_between_operands, number_of_line, file_name);
			exit = TRUE;
		}
	}
	while ((isspace(first_com_loc[i]) || first_com_loc[i] == '\0') && !exit) {		/*keep going untill there is no space*/
		if (first_com_loc[i++] == '\0') {	/*if the while entered to this if, its tells us that there is no second operand*/
			add_input_error_message(operand_is_missing, number_of_line, file_name);
			exit = TRUE;
		}
	}
	if (first_com_loc[i] == '&' && !exit) {
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	if (first_com_loc[i] == '#' && strcmp(command, "cmp") && !exit) {
		add_input_error_message(destination_operand_cannot_be_number, number_of_line, file_name);
		exit = TRUE;
	}
	if (!isalnum(first_com_loc[i]) && first_com_loc[i] != ',' && first_com_loc[i] != '#' && !exit) {
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	while (((isalnum(first_com_loc[i])) || first_com_loc[i] == '#' || first_com_loc[i] == '&') && !exit) {
		if (first_com_loc[i] == '#') 									/*if there is '#' we need to know that we dealing with number*/
			temp = '#';
		operand2[j++] = first_com_loc[i++];
		if (first_com_loc[i - 1] == '#' && (first_com_loc[i] == '-' || first_com_loc[i] == '+')) /*check if there is a minus number, if yes, copy it*/
			operand2[j++] = first_com_loc[i++];							/*attention: it may entered the 'if' only on command 'cmp'*/
		if (temp == '#') {												/*if we have number, check if it written correctly*/
			if (!isdigit(first_com_loc[i]) && first_com_loc[i] != ',' && first_com_loc[i] != '\0') {
				add_input_error_message(operand_is_illegal, number_of_line, file_name);
				exit = TRUE;
			}
		}
	}
	operand2[j] = '\0';
	if (atoi(operand2) && !exit) {
		add_input_error_message(operand_is_illegal, number_of_line, file_name);
		exit = TRUE;
	}
	while (isspace(first_com_loc[i]) && first_com_loc[i] != '\0' && !exit)			/*keep going untill there is no space or '\0'*/
		i++;
	if (first_com_loc[i] != ';' && first_com_loc[i] != '\0' && !exit) {				/*checking what is the char that not space*/
		add_input_error_message(extraneous_syntax, number_of_line, file_name);
		exit = TRUE;
	}
	if (exit)		/*if we got an error message, exit.*/
		return;
	for (j = 0; j < NumOfRegisters && !exit; j++) {
		if (!strcmp(all_registers[j], operand1)) {				/*check if first operand is register*/
			for (i = 0; i < NumOfRegisters && !exit; i++) {				/*if yes, check if the second operand is register*/
				if (!strcmp(all_registers[i], operand2)) {		/*if yes, enter the 'if' and allocate 1 word*/
					if (label[0])								/*it gets here only if there is 2 registers*/
						add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
					add_to_code_linked_list(hptr_code, input, commands_table, command, operand1, operand2, IC, 1, file_name);
					exit = TRUE;
				}
			}
			if (!exit) {/*gets here only if the second operand is not register*/
				if (operand2[0] != '#')		/*enter the if only if the operand is not a number*/
					add_to_operand_label_list(hptr_operand_label, operand2, number_of_line, file_name);	/*it gets here only if the second operand is operand label*/
				if (label[0])
					add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
				add_to_code_linked_list(hptr_code, input, commands_table, command, operand1, operand2, IC, 2, file_name);
				exit = TRUE;
			}
		}
	}						/*it gets here only if the first operand is not register*/
	for (j = 0; j < NumOfRegisters && !exit; j++) {
		if (!strcmp(all_registers[j], operand2)) {
			if (operand1[0] != '#')		/*enter the if only if the operand is not a number*/
				add_to_operand_label_list(hptr_operand_label, operand1, number_of_line, file_name);	/*it gets here only if the second operand is register*/
			if (label[0])
				add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
			add_to_code_linked_list(hptr_code, input, commands_table, command, operand1, operand2, IC, 2, file_name);
			exit = TRUE;
		}
	}
	if (!exit) {/*it gets here only if there is no registers in input*/
		if (operand1[0] != '#' && operand2[0] != '#') {		/*enter the if only if the 2 of the operands is not a number*/
			add_to_operand_label_list(hptr_operand_label, operand1, number_of_line, file_name);
			add_to_operand_label_list(hptr_operand_label, operand2, number_of_line, file_name);
		}
		else if (operand1[0] != '#')
			add_to_operand_label_list(hptr_operand_label, operand1, number_of_line, file_name);
		else if (operand2[0] != '#')
			add_to_operand_label_list(hptr_operand_label, operand2, number_of_line, file_name);
		if (label[0])
			add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
		add_to_code_linked_list(hptr_code, input, commands_table, command, operand1, operand2, IC, 3, file_name);
	}
}

void third_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char command[], char file_name[], int* IC, int number_of_line) {
	char first_com_loc[LineLength];		/*gives us the first location of the command, and the rest*/
	int i = 0;

	strcpy(first_com_loc, strstr(input, command));
	while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0') 									/*keep going untill there is space*/
		i++;
	while (first_com_loc[i] != '\0' && first_com_loc[i] != ';') {									/*keep going untill there is char that isn't space*/
		if (!isspace(first_com_loc[i++])) {
			add_input_error_message(extraneous_syntax, number_of_line, file_name);
			return;
		}
	}
	if (label[0])
		add_to_labels_list(hptr_label, label, IC, code, -1, file_name, number_of_line);
	add_to_code_linked_list(hptr_code, input, commands_table, command, NULL, NULL, IC, 1, file_name);
}
