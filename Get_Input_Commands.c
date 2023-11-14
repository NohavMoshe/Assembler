#include "Header.h"

boolean get_code_command(char input[], char* command, char* label, all_commands_table commands_table[], char file_name[], int number_of_line) {
	int i = 0, j = 0;

	if (label[0] == '\0') {							/*in case there is no label*/
		while (isspace(input[i]))						/*go untill there is chars*/
			i++;
		while (isalnum(input[i]) != 0) 				/*copy the command to the 'command' string*/
			command[j++] = input[i++];
		command[j] = '\0';
	}
	else {											/*in case there is label*/
		while (isspace(input[i]))					/*go untill there is char*/
			i++;
		while (input[i] != ':')						/*go untill there is ':'*/
			i++;
		i++;											/*go after the ':'*/
		while (isspace(input[i]))						/*go untill there is chars*/
			i++;
		while (isalnum(input[i]) != 0) 				/*copy the command to the 'command' string*/
			command[j++] = input[i++];
		command[j] = '\0';
	}
	for (i = 0; i < NumberOfCommands; i++) {
		if (!strcmp(command, commands_table[i]->name)) {		/*check if there is command in the input*/
			strcpy(command, commands_table[i]->name);
			return TRUE;
		}
	}
	add_input_error_message(not_exists, number_of_line, file_name);
	return FALSE;
}

boolean get_data_command(OperandLabelPointer* hptr_operand_label, DataPointer* hptr_data, LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, char* label, char* command, char input[], all_commands_table commands_table[], char file_name[], int* IC, int* DC, int number_of_line) {
	int i = 0, j = 0;

	if (label[0] == '\0') {							/*in case there is no label*/
		while (isspace(input[i]))						/*go untill there is '.'*/
			i++;
	}
	else {											/*in case there is label*/
		while (!isspace(input[i]) && input[i] != ':')						/*go untill there is space or after the ':'*/
			i++;
		i++;											/*go after the space or ':'*/
		while (isspace(input[i]))						/*go untill there is chars*/
			i++;
	}
	if (input[i] == '.') {					/*if there is '.' before a string - it is data command. so copy it while there is no space*/
		while (!isspace(input[i]) && input[i] != '\0') {
			command[j++] = input[i++];		/*copy the data command*/
		}
		command[j] = '\0';
		check_if_data_command_lligal(hptr_operand_label, hptr_data, hptr_label, hptr_label_for_data, label, input, command, file_name, IC, DC, number_of_line);
		return TRUE;
	}
	return FALSE;
}

void check_if_data_command_lligal(OperandLabelPointer* hptr_operand_label, DataPointer* hptr_data, LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, char* label, char input[], char command[], char file_name[], int* IC, int* DC, int number_of_line) {
	char first_com_loc[LineLength], number[LineLength], temp[LineLength];					/*gives us the first location of the command, and the the rest*/
	int i = 0, j = 0;
	char* all_data_commands[NumOfDataCommands] = { ".data", ".string", ".entry", ".extern" }; /*the variable will count the number of addresses a label with data command is taking*/
	boolean data_command = FALSE, exit = FALSE , success = FALSE;		/*exit will get true when we will get error messeage, data_command is whether there is data command, success if whether there is no errors*/
	int number_of_entries = 0;

	strcpy(first_com_loc, strstr(input, command));
	while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0')								/*keep going untill there is space*/
		i++;
	while (isspace(first_com_loc[i]))								/*keep going untill there is char that isn't space*/
		i++;
	while (all_data_commands[j] != '\0' && !success && !exit) {
		if (!strcmp(command, all_data_commands[j])) {
			data_command = TRUE;
			switch (j) {
			case 0:		/*in case of data*/
				while (first_com_loc[i] != '\0' && !exit) {
					j = 0;
					if (!isdigit(first_com_loc[i]) && first_com_loc[i] != '-' && first_com_loc[i] != '+') {
						add_input_error_message(data_command_illegal, number_of_line, file_name);	
						exit = TRUE;
					}
					if ((first_com_loc[i] == '-' || first_com_loc[i] == '+') && !exit) {
						number[j++] = first_com_loc[i++];
					}
					while (isdigit(first_com_loc[i]) && !exit) {
						number[j++] = first_com_loc[i++];
					}
					number[j] = '\0';
					while (isspace(first_com_loc[i]) && !exit) {		/*go untill there is ','*/
						i++;
					}
					if (first_com_loc[i] != ',' && first_com_loc[i] != '\0' && !exit) {
						add_input_error_message(data_command_illegal, number_of_line, file_name);
						exit = TRUE;
					}
					if (exit)
						break;
					add_to_data_linked_list(hptr_data, atoi(number), IC, DC, file_name, label);
					number_of_entries++;
					if ((first_com_loc[i] == '\0' || first_com_loc[i] == ';') && !exit) {
						/*if (label[0])*/
						linked_list_for_real_data(hptr_label_for_data, label, number_of_entries, data, -1, file_name, number_of_line);
						success = TRUE;
						break;
					}
					i++;									/*go after the ','*/
					while (isspace(first_com_loc[i]) && !exit)		/*go untill the next number*/
						i++;
					if (!isdigit(first_com_loc[i]) && first_com_loc[i] != '-' && first_com_loc[i] != '+' && !exit) {
						add_input_error_message(data_command_illegal, number_of_line, file_name);
						exit = TRUE;
					}
				}
				if(!exit && !success)
					add_input_error_message(data_command_has_no_opernads, number_of_line, file_name);
				break;
			case 1:		/*in case of string*/
				j = 0;
				if (first_com_loc[i++] == '"') {		/*check that there is " between the string of .string*/
					while (first_com_loc[i] != '"' && first_com_loc[i] != '\0')
						temp[j++] = first_com_loc[i++];	/*copy the string that between ""*/
					temp[j] = '\0';
					if (first_com_loc[i++] == '"') {
						while (isspace(first_com_loc[i]))	/*check if there is something after the second "*/
							i++;
						if (first_com_loc[i] == '\0' || first_com_loc[i] == ';') {
							j = 0;
							while (temp[j]) {
								add_to_data_linked_list(hptr_data, temp[j], IC, DC, file_name, label);
								number_of_entries++;
								j++;
							}
							add_to_data_linked_list(hptr_data, temp[j], IC, DC, file_name, label);		/*adding the \0 to the data link list*/
							number_of_entries++;
							/*if (label[0])*/
							linked_list_for_real_data(hptr_label_for_data, label, number_of_entries, data, -1, file_name, number_of_line);
							success = TRUE;
							break;
						}
					}
				}
				add_input_error_message(missing_apostrophes, number_of_line, file_name);
				break;
			case 2:		/*in case of entry*/
				j = 0;
				while (isspace(first_com_loc[i]))								/*keep going untill there is char that isn't space*/
					i++;
				while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0')
					temp[j++] = first_com_loc[i++];
				temp[j] = '\0';
				while (isspace(first_com_loc[i]))								/*keep going untill there is char that isn't space*/
					i++;
				if (first_com_loc[i] != ';' && first_com_loc[i] != '\0') {
					add_input_error_message(extraneous_syntax, number_of_line, file_name);
					exit = TRUE;
				}
				if (!exit) {
					add_to_operand_label_list(hptr_operand_label, temp, number_of_line, file_name);
					add_to_labels_list(hptr_label, temp, IC, -1, entry, file_name, number_of_line);
					success = TRUE;
				}
				break;
			case 3:		/*in case of external*/
				j = 0;
				while (isspace(first_com_loc[i]))								/*keep going untill there is char that isn't space*/
					i++;
				while (!isspace(first_com_loc[i]) && first_com_loc[i] != '\0')
					temp[j++] = first_com_loc[i++];
				temp[j] = '\0';
				while (isspace(first_com_loc[i]))								/*keep going untill there is char that isn't space*/
					i++;
				if (first_com_loc[i] != ';' && first_com_loc[i] != '\0') {
					add_input_error_message(extraneous_syntax, number_of_line, file_name);
					exit = TRUE;
				}
				if (!exit) {
					add_to_operand_label_list(hptr_operand_label, temp, number_of_line, file_name);
					add_to_labels_list(hptr_label, temp, IC, -1, external, file_name, number_of_line);
					success = TRUE;
				}
				break;
			}
		}
		j++;
	}
	if(!data_command)
		add_input_error_message(not_exists, number_of_line, file_name);
}

boolean get_label(all_commands_table commands_table[], char input[], char* label, char file_name[], int number_of_line) {
	int i = 0, j = 0, temp = 0;
	char* all_registers[NumOfRegisters] = { "r0" ,"r1" ,"r2" ,"r3" ,"r4" ,"r5" ,"r6", "r7" };
	char* data_commands[NumOfDataCommands] = { ".entry", ".extern" };

	while (input[i]) {
		if (input[i++] == ':') {
			i--;
			temp = i;								/*'temp' is for saving the latest place of 'i', in order to come back to him later after next loop*/
			i = 0;									/*beggin from the start of the input*/
			if (isspace(input[i]))					/*checking if there is white character before the label*/
				i++;
			while (isalnum(input[i]) && i <= temp) {
				label[j++] = input[i++];			/*copy the label*/
			}
			if (!isalnum(input[i]) && input[i] != ':') {				/*check if there is illegal character on the label*/
				add_input_error_message(the_label_is_written_illegally, number_of_line, file_name);
				return FALSE;
			}
			label[j] = '\0';
			for (j = 0; j < NumOfRegisters; j++) {					/*check if the the label is register */
				if (!strcmp(all_registers[j], label)) {
					add_input_error_message(label_cannot_be_named_by_register, number_of_line, file_name);
					label[0] = '\0';
					j = NumOfRegisters;
					return FALSE;
				}
			}
			j = 0;
			while (data_commands[j]) {				/*if the input contain entry or extern, show warning*/
				if (strstr(input, data_commands[j++])) {
					fprintf(stderr, "WARNING: There is Label command on entry or extern, on line %d, in file '%s'", number_of_line, file_name);
				}
			}
			for (i = 0; i < NumberOfCommands; i++) {
				if (!strcmp(label, commands_table[i]->name)) {		/*check if there is command in the input*/
					add_input_error_message(label_cannot_be_named_by_code_command, number_of_line, file_name);
					label[0] = '\0';
					i = NumberOfCommands;
					return FALSE;
				}
			}
			return TRUE;
		}
	}
	label[0] = '\0';
	return TRUE;
}
