
/*declaring the functions that dealing with the different tables/linked lists*/

/*table of all the possible commands, with their opcode and funct*/
void make_table_of_commands(all_commands_table commands_table[]);

/*adding to the label link list, making the symbol table*/
void add_to_labels_list(LabelPointer* hptr, char* label, int* IC, cod_or_dat command_type, ext_or_ent ExtOrEnt, char file_name[], int number_of_line);

/*building the code node according to the structure and adding the node to the linked list*/
void add_to_code_linked_list(CodePointer* hptr, char input[], all_commands_table commands_table[], char* command, char operand1[], char operand2[], int* IC, int WordsToAllocate, char file_name[]);

/*building the data node according to the structure and adding the node to the linked list*/
void add_to_data_linked_list(DataPointer* hptr, int DataToInsert, int* IC, int* DC, char file_name[], char label[]);

/*adding an operand label into the link list, with number of line, in order to check later if all the opernads labels are defined*/
void add_to_operand_label_list(OperandLabelPointer* hptr_operand_label, char label[], int number_of_line, char file_name[]);

/*the function is checking whether operand label is defined on the file as a legal label, if not, make error message*/
void check_if_operand_label_defined(OperandLabelPointer* hptr_operand_label, LabelPointer* hptr_label, char file_name[]);

/*adding to the linked list that contains only the .data & .string commands*/
void linked_list_for_real_data(LabelPointer* hptr_label, char* label, int number_of_enteries, cod_or_dat command_type, ext_or_ent ExtOrEnt, char file_name[], int number_of_line);

/*free the different linked lists*/
void freeLabelList(LabelPointer* hptr);
void freeCodeList(CodePointer* hptr);
void freeDataList(DataPointer* hptr);
void freeOperandLabelList(OperandLabelPointer* hptr);
