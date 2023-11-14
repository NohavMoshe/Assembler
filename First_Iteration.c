#include "Header.h"

boolean is_error_msg;

void handle_file(char file_name[], all_commands_table commands_table[]) {
	FILE* file_ptr;
	int i = 0, j = 0, IC = 100, number_of_line = 0, DC = 0;
	int* IC_ptr = &IC;
	int* DC_ptr = &DC;
	char input[LineLength], label[LabelLength], code_command[MaxLengthOfCommands], data_command[MaxLengthOfCommands];
	LabelPointer hptr_label = NULL;										/*header of the label_list*/
	LabelPointer hptr_label_for_data = NULL;							/*header of the label_list only for data commands*/
	CodePointer hptr_code = NULL;										/*header of the code command linked list*/
	DataPointer hptr_data = NULL;										/*header of the data command linked list*/
	OperandLabelPointer hptr_operand_label = NULL;						/*header of the operand label linked list*/

	if (!(file_ptr = fopen(file_name, "r"))) {
		add_program_error_message(cannot_open_file, file_name);
		exit(1);
	}
	if (check_if_file_is_empty(file_ptr, file_name)) 
		return;
	while (!feof(file_ptr)) {
		fgets(input, LineLength, file_ptr);	/*get input sentence from the file*/
		i = strlen(input) - 1;
		if (input[i] == '\n')		/*get rid of the '\n' if exists*/
			input[i] = '\0';
		code_command[0] = '\0';
		data_command[0] = '\0';
		label[0] = '\0';
		i = 0;														/*for 'input' to start next time from i = 0*/
		number_of_line++;
		if (!check_if_row_is_empty(input, number_of_line, file_name)) {		/*go inside only if row has input or has ';'*/
			is_error_msg = FALSE;
			make_table_of_commands(commands_table);					/*table of commands with opcode and funct*/
			if (get_label(commands_table, input, label, file_name, number_of_line)) {	/*if there is error on the label defenition, don't enter the if*/
				if (!get_data_command(&hptr_operand_label, &hptr_data, &hptr_label, &hptr_label_for_data, label, data_command, input, commands_table, file_name, IC_ptr, DC_ptr, number_of_line)) {	/*check if there is data command, if not enter the if*/
					if (get_code_command(input, code_command, label, commands_table, file_name, number_of_line)) {	/*get the command, if the command not exist, print error*/
						for (j = 0; j < NumberOfCommands; j++) {
							if (!strcmp(code_command, commands_table[j]->name)) {
								if (j < 5)							/*check if code command is bellonging to first group*/
									first_command_group(&hptr_operand_label, &hptr_code, &hptr_label, label, commands_table, input, code_command, file_name, IC_ptr, number_of_line);
								else if (j > 4 && j < 14) 			/*check if code command is bellonging to second group*/
									second_command_group(&hptr_operand_label, &hptr_code, &hptr_label, label, commands_table, input, code_command, file_name, IC_ptr, number_of_line);
								else if (j >= 14 && j <= 15)	
									third_command_group(&hptr_operand_label, &hptr_code, &hptr_label, label, commands_table, input, code_command, file_name, IC_ptr, number_of_line);
								j = NumberOfCommands;		/*for ending the loop*/
							}
						}
					}
				}
			}
		}
	}		
	fixing_the_label_linked_list(&hptr_label, &hptr_label_for_data, IC_ptr);
	if(hptr_operand_label != NULL)		/*if we have operand label, check whether it defined*/
		check_if_operand_label_defined(&hptr_operand_label, &hptr_label, file_name);	
	if (!is_error_msg) {				/*if 'is_error_msg' still false, it tell us that the assmbly code is legel, so procede to second iteration*/
		second_running_on_code_linked_list(&hptr_code,&hptr_data, &hptr_label, file_name, IC_ptr, DC_ptr);
	}
	deleting_empty_labels(&hptr_label);
	freeOperandLabelList(&hptr_operand_label);
	freeLabelList(&hptr_label);
	freeCodeList(&hptr_code);
	freeDataList(&hptr_data);
	fclose(file_ptr);
}
