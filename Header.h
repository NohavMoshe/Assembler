#include "Data_Structure.h"
#include "Error_Handle.h"
#include "Tables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MaxLengthOfCommands 8
#define NumOfDataCommands 5
#define NumOfRegisters 8

extern boolean is_error_msg;		/*the flag will tell us whether we got error message. if we got error message, don't procede to second iteration*/

/*----------------- First_Iteration ---------------------------------------*/

/*the function gets a sentence from the input and uses functions to decide what the sentence contains*/
void handle_file(char file_name[], all_commands_table commands_table[]);	

/*----------------- Get_Input_Commands ---------------------------------------*/

/*the function checking if the is data command in the input. and then checking whether the data command legal, if yes return true*/
boolean get_data_command(OperandLabelPointer* hptr_operand_label, DataPointer* hptr_data, LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, char* label, char* command, char input[], all_commands_table commands_table[], char file_name[], int* IC, int* DC, int number_of_line);

/*the function checking if there is label in the input, if yes - add the label to the symbol table and return true*/
boolean get_label(all_commands_table commands_table[], char input[], char* label, char file_name[], int number_of_line);

/*the function checking if there is code command in the input, if yes - add it to code link list*/
boolean get_code_command(char input[], char* command, char* label, all_commands_table commands_table[], char file_name[], int number_of_line);

/*the function checking wether the data command is exists and legal, if yes, inserts the relavent parts into data command link list*/
void check_if_data_command_lligal(OperandLabelPointer* hptr_operand_label, DataPointer* hptr_data, LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, char* label, char input[], char command[], char file_name[], int* IC, int* DC, int number_of_line);

/*----------------- Operands_Handle---------------------------------------*/

/*the function is operates when there is only 1 operand in the input*/
void second_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char* command, char file_name[], int* IC, int number_of_line);

/*the function is operates when there is 2 operand in the input*/
void first_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char command[], char file_name[], int* IC, int number_of_line);

/*the function is operates when there is 'stop' or 'rts' commands in the input. also it makes sure that after command 'stop' it cannot be other code commands*/
void third_command_group(OperandLabelPointer* hptr_operand_label, CodePointer* hptr_code, LabelPointer* hptr_label, char* label, all_commands_table commands_table[], char input[], char command[], char file_name[], int* IC, int number_of_line);

/*----------------- Second_Iteration ---------------------------------------*/

/*running on the code table and fill it with the relavent data where its nessesery*/
void second_running_on_code_linked_list(CodePointer* HeadCodeLinkedList, DataPointer* HeadDataLinkedList, LabelPointer* HeadLabelLinkedList, char file_name[], int* IC, int* DC);

/*printing the object file*/
void print_ps_ob_file(CodePointer* HeadCodeLinkedList, DataPointer* HeadDataLinkedList, char file_name[], int* IC, int* DC);

/*printing the extern and the entry files*/
void print_ps_ent_ext_files(CodePointer* HeadCodeLinkedList, LabelPointer* HeadLabelLinkedList, char file_name[]);

/*fixing the linked list to the proper way it should looks like*/
void fixing_the_label_linked_list(LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, int* IC);

/*----------------- Assistance_Functions---------------------------------------*/

/*checking if row is empty and also if there is illegal note*/
boolean check_if_row_is_empty(char input[], int number_of_line, char file_name[]);	

/*checking whether the file is empty, if it does - sends an error*/
boolean check_if_file_is_empty(FILE* file_ptr, char file_name[]);	

/*deleting all the labels that are empty on the symbol table. this function will operates after the second iteration*/
void deleting_empty_labels(LabelPointer* hptr_label);


