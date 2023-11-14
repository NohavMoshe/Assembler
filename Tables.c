#include "Header.h"

void make_table_of_commands(all_commands_table commands_table[]) {
	commands_table[0]->name = "mov";
	commands_table[0]->funct = 0;
	commands_table[0]->opcode = 0;

	commands_table[1]->name = "cmp";
	commands_table[1]->funct = 0;
	commands_table[1]->opcode = 1;

	commands_table[2]->name = "add";
	commands_table[2]->funct = 1;
	commands_table[2]->opcode = 2;

	commands_table[3]->name = "sub";
	commands_table[3]->funct = 2;
	commands_table[3]->opcode = 2;

	commands_table[4]->name = "lea";
	commands_table[4]->funct = 0;
	commands_table[4]->opcode = 4;

	commands_table[5]->name = "clr";
	commands_table[5]->funct = 1;
	commands_table[5]->opcode = 5;

	commands_table[6]->name = "not";
	commands_table[6]->funct = 2;
	commands_table[6]->opcode = 5;

	commands_table[7]->name = "inc";
	commands_table[7]->funct = 3;
	commands_table[7]->opcode = 5;

	commands_table[8]->name = "dec";
	commands_table[8]->funct = 4;
	commands_table[8]->opcode = 5;

	commands_table[9]->name = "jmp";
	commands_table[9]->funct = 1;
	commands_table[9]->opcode = 9;

	commands_table[10]->name = "bne";
	commands_table[10]->funct = 2;
	commands_table[10]->opcode = 9;

	commands_table[11]->name = "jsr";
	commands_table[11]->funct = 3;
	commands_table[11]->opcode = 9;

	commands_table[12]->name = "red";
	commands_table[12]->funct = 0;
	commands_table[12]->opcode = 12;

	commands_table[13]->name = "prn";
	commands_table[13]->funct = 0;
	commands_table[13]->opcode = 13;

	commands_table[14]->name = "rts";
	commands_table[14]->funct = 0;
	commands_table[14]->opcode = 14;

	commands_table[15]->name = "stop";
	commands_table[15]->funct = 0;
	commands_table[15]->opcode = 15;
}

void add_to_data_linked_list(DataPointer* hptr, int DataToInsert, int* IC,int* DC, char file_name[], char label[])
{
	int realNumberInOperand;
	DataPointer temp = (DataPointer)malloc(sizeof(WordForData));
	DataPointer p = *hptr;

	check_malloc_validation(temp, file_name);
	/*setting the IC parameter*/
	if (label[0] == '\0') {	/*in case we dont have label on the data command, making sure that we'll not skip the right address */
		temp->address = *IC + *DC;
	}
	else
		temp->address = *DC;
	*DC = *DC + 1;
	realNumberInOperand = DataToInsert;	/*the Data is a number*/
	temp->NumberInBits = realNumberInOperand;

	/*adding to the linked list of Data*/
	if (!(*hptr)) {					/*checking the case if the link list is empty, if it does - go in(this case accuring once, when the link list empty)*/
		temp->next = NULL;
		*hptr = temp;				/*make temp - the new header for the link list*/
		return;
	}
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = temp;
	p->next->next = NULL;
}

/*The command is exist in the table of commands*/
void add_to_code_linked_list(CodePointer* hptr, char input[], all_commands_table commands_table[], char* command, char operand1[], char operand2[], int* IC, int WordsToAllocate, char file_name[]) {
	int i, same = 0;
	int realNumberInOperand1, realNumberInOperand2;
	int operand1check = 0, operand2check = 0;
	char* all_registers[NumOfRegisters] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
	char operand2RealName[LabelLength] = "\0";
	CodePointer temp = (CodePointer)malloc(sizeof(CodeLinkedList));
	CodePointer p = *hptr;

	/*reset all the data in the node*/
	temp->word.DestinationAddressingMethod = 0; 
	temp->word.DestinationRegisterName = 0;
	temp->word.SourceAddressingMethod = 0;
	temp->word.SourceRegisterName = 0;
	temp->Elseword[0].operand[0] = '\0';
	temp->Elseword[0].ARE = 0;
	temp->Elseword[0].NumberInBits = 0;
	temp->Elseword[1].operand[0] = '\0';
	temp->Elseword[1].ARE = 0;
	temp->Elseword[1].NumberInBits = 0;

	check_malloc_validation(temp, file_name);

	/*filling the address in the addresss array*/
	for (i = 0; i < WordsToAllocate; i++)
	{
		temp->address[i] = *IC;
		*IC = *IC + 1;
	}
	strcpy(temp->input, input);

	for (i = 0; i < NumberOfCommands && same == 0; i++) {
		if (strcmp(command, commands_table[i]->name) == 0)
		{
			temp->word.opcode = commands_table[i]->opcode;
			temp->word.funct = commands_table[i]->funct;
			temp->word.ARE = 4; /*First Word is always Absulote*/
			same = 1;
		}
	}
	/*Checking the existence of the operands*/
	if (operand1 != NULL && operand2 != NULL) /*operand1 & operand2 are exist */
	{
		/*operand1 is register*/
		for (i = 0; i < NumOfRegisters; i++)
		{
			if (!strcmp(operand1, all_registers[i]))
			{
				operand1check = 1;
				temp->word.SourceAddressingMethod = 3; /*the sourse addressing method is 3*/
				temp->word.SourceRegisterName = i;
				/*Taking care of the rest of the words*/
				temp->Elseword[0].NumberInBits = 0; /*NULL for now*/
				temp->Elseword[0].ARE = 0; /*NULL for now*/
				temp->Elseword[1].NumberInBits = 0;
				temp->Elseword[1].ARE = 0;
				temp->Elseword[1].operand[0] = '\0';
			}
		}
		if (operand1[0] == '#' && operand1check == 0) /*Operand1 is Number*/
		{
			operand1check = 1;
			operand1[0] = '\n';
			temp->word.SourceAddressingMethod = 0;
			temp->word.SourceRegisterName = 0;

			/*Taking care of the rest of the words*/
			realNumberInOperand1 = atoi(operand1);
			temp->Elseword[0].NumberInBits = realNumberInOperand1; /*The translation of the number to bits*/
			temp->Elseword[0].ARE = 4; /*Number is Absolute*/
			strcpy(temp->Elseword[0].operand, operand1); /*the operand itself*/
			temp->Elseword[1].NumberInBits = 0; /*NULL for now*/
			temp->Elseword[1].ARE = 0; /*NULL for now*/
		}
		if (isalpha(operand1[0]) != 0 && operand1check == 0) /*operand1 is variable*/
		{
			operand1check = 1;
			temp->word.SourceAddressingMethod = 1;
			temp->word.SourceRegisterName = 0;
			/*taking care of the rest of the words*/
			temp->Elseword[0].NumberInBits = '?'; /*The address of the variable*/
			temp->Elseword[0].ARE = 2; /*variable is Relocatable*/
			strcpy(temp->Elseword[0].operand, operand1); /*variable*/
			temp->Elseword[1].NumberInBits = 0; /*NULL for now*/
			temp->Elseword[1].ARE = 0; /*NULL for now*/
		}
		/*operand2 is register*/
		for (i = 0; i < NumOfRegisters; i++)
		{
			if (!strcmp(operand2, all_registers[i]))
			{
				operand2check = 1;
				temp->word.DestinationAddressingMethod = 3; /*the sourse addressing method is 3*/
				temp->word.DestinationRegisterName = i;
			}
		}
		/*No extra words needed*/

		if (operand2[0] == '#' && operand2check == 0) /*Operand2 is Number*/
		{
			operand2check = 1;
			operand2[0] = '\n';
			temp->word.DestinationAddressingMethod = 0;
			temp->word.DestinationRegisterName = 0;
			/*taking care of the rest of the words*/
			realNumberInOperand2 = atoi(operand2);
			if (temp->Elseword[0].ARE == 0) { /*Checking if the space is already taken*/
				temp->Elseword[0].NumberInBits = realNumberInOperand2; /*the value of the number*/
				temp->Elseword[0].ARE = 4; /*variable is absolute*/
				strcpy(temp->Elseword[0].operand, operand2); /*the operand itself*/
			}
			else {
				temp->Elseword[1].NumberInBits = realNumberInOperand2; /*the value of the number*/
				temp->Elseword[1].ARE = 4; /*variable is absolute*/
				strcpy(temp->Elseword[1].operand, operand2); /*the operand itself*/
			}
		}
		if (isalpha(operand2[0]) != 0 && operand2check == 0)  /*operand2 is variable*/
		{
			operand2check = 1;
			temp->word.DestinationAddressingMethod = 1;
			temp->word.DestinationRegisterName = 0;
			/*taking care of the rest of the words*/
			if (temp->Elseword[0].ARE == 0) {
				temp->Elseword[0].NumberInBits = '?'; /*The address of the variable*/
				temp->Elseword[0].ARE = 2; /*variable is Relocatable*/
				strcpy(temp->Elseword[0].operand, operand2); /*variable*/
			}
			else {
				temp->Elseword[1].NumberInBits = '?'; /*The address of the variable*/
				temp->Elseword[1].ARE = 2; /*variable is Relocatable*/
				strcpy(temp->Elseword[1].operand, operand2); /*variable*/
			}
		}
	}
	if (operand1 == NULL && operand2 != NULL) /*operand1 isn't exist & operand2 is exist */
	{
		/*In this case there is no Source Method OR Sourse Register*/
		temp->word.SourceAddressingMethod = 0;
		temp->word.SourceRegisterName = 0;
		temp->Elseword[0].NumberInBits = 0; /*NULL for now*/
		temp->Elseword[0].ARE = 0;	/*NULL for now*/
		/*In this case there is no need to use the Elseword[1] place*/
		temp->Elseword[1].NumberInBits = 0;
		temp->Elseword[1].ARE = 0;
		temp->Elseword[1].operand[0] = '\0';

		/*operand2 is register*/
		for (i = 0; i < NumOfRegisters; i++)
		{
			if (!strcmp(operand2, all_registers[i]))
			{
				operand2check = 1;
				temp->word.DestinationAddressingMethod = 3; /*the sourse addressing method is 3*/
				temp->word.DestinationRegisterName = i;
				/*No extra words needed*/
			}
		}
		if (isalpha(operand2[0]) != 0 && operand2check == 0) /*operand2 is variable*/
		{
			operand2check = 1;
			temp->word.DestinationAddressingMethod = 1;
			temp->word.DestinationRegisterName = 0;
			/*taking care of the rest of the words*/
			temp->Elseword[0].NumberInBits = '?'; /*The address of the variable*/
			temp->Elseword[0].ARE = 2; /*variable is Relocatable*/
			strcpy(temp->Elseword[0].operand, operand2);
		}
		if (operand2[0] == '&' && operand2check == 0) /*operand2 is label*/
		{
			operand2check = 1;

			temp->word.DestinationAddressingMethod = 2;
			temp->word.DestinationRegisterName = 0;
			/*taking care of the rest of the words*/
			temp->Elseword[0].NumberInBits = '@'; /*The the distance*/
			temp->Elseword[0].ARE = 4; /*Lable is Relocatable*/
			strcpy(temp->Elseword[0].operand, strncat(operand2RealName, operand2 + 1, strlen(operand2)));
		}
		if (operand2[0] == '#' && operand2check == 0) /*Operand2 is Number*/
		{
			operand2check = 1;
			operand2[0] = '\n';
			temp->word.DestinationAddressingMethod = 0;
			temp->word.DestinationRegisterName = 0;
			/*taking care of the rest of the words*/
			realNumberInOperand2 = atoi(operand2);
			temp->Elseword[0].NumberInBits = realNumberInOperand2; /*the value of the number*/
			temp->Elseword[0].ARE = 4; /*variable is absolute*/
			strcpy(temp->Elseword[0].operand, operand2); /*the operand itself*/
		}
	}
	/*adding to the linked list of Codes*/
	if (!(*hptr)) {					/*checking the case if the link list is empty, if it does - go in(this case accuring once, when the link list empty)*/
		temp->next = NULL;
		*hptr = temp;				/*make temp - the new header for the link list*/
		return;
	}
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = temp;
	p->next->next = NULL;
}

/*The symbols table*/
void add_to_labels_list(LabelPointer* hptr_label, char* label, int* IC, cod_or_dat command_type, ext_or_ent ExtOrEnt, char file_name[], int number_of_line) {
	LabelPointer temp = (LabelPointer)malloc(sizeof(labels_list));
	LabelPointer label_ptr = *hptr_label;
	boolean flag = FALSE;
	check_malloc_validation(temp, file_name);
	temp->address = 0;
	strcpy(temp->label, label);
	if(ExtOrEnt != external && ExtOrEnt != entry)	/*get the IC only whether the label is on data command or code command*/
		temp->address = *IC;
	temp->command_type = command_type;
	temp->ExtOrEnt = ExtOrEnt;
	if (!(*hptr_label)) {					/*checking the case if the link list is empty, if it does - go in(this case accuring once, when the link list empty)*/
		temp->next = NULL;
		*hptr_label = temp;				/*make temp the new header for the link list*/
		return;
	}
	while (label_ptr->next != NULL || !strcmp(label_ptr->label, label)) {
		if (!strcmp(label_ptr->label, label)) {			/*if the label is already exists, and we get on function ext or ent, insert it into the label list*/
			if (label_ptr->ExtOrEnt == -1) {		
				label_ptr->ExtOrEnt = ExtOrEnt;
				flag = TRUE;
			}
			if (label_ptr->command_type == -1) {
				label_ptr->command_type = command_type;
				label_ptr->address = *IC;
				flag = TRUE;
			}
			if (!flag) 
				add_input_error_message(label_is_already_exists, number_of_line, file_name);
			return;
		}
		label_ptr = label_ptr->next;
	}
	label_ptr->next = temp;
	label_ptr->next->next = NULL;
}

void linked_list_for_real_data(LabelPointer* hptr_label, char* label, int number_of_enteries, cod_or_dat command_type, ext_or_ent ExtOrEnt, char file_name[], int number_of_line)
{
	LabelPointer temp = (LabelPointer)malloc(sizeof(labels_list));
	LabelPointer label_ptr = *hptr_label;
	check_malloc_validation(temp, file_name);
	strcpy(temp->label, label);
	temp->address = number_of_enteries;
	temp->command_type = command_type;
	temp->ExtOrEnt = ExtOrEnt;
	if (!(*hptr_label)) {					/*checking the case if the link list is empty, if it does - go in(this case accuring once, when the link list empty)*/
		temp->next = NULL;
		*hptr_label = temp;				/*make temp the new header for the link list*/
		return;
	}
	while (label_ptr->next != NULL) {
		label_ptr = label_ptr->next;
	}
	label_ptr->next = temp;
	label_ptr->next->next = NULL;
}

void add_to_operand_label_list(OperandLabelPointer* hptr_operand_label, char label[], int number_of_line, char file_name[]) {
	OperandLabelPointer temp = (OperandLabelPointer)malloc(sizeof(operand_label));
	OperandLabelPointer p_operand_label = *hptr_operand_label;

	check_malloc_validation(temp, file_name);
	strcpy(temp->label, label);
	temp->number_of_line = number_of_line;
	if (!(*hptr_operand_label)) { 					/*checking the case if the link list is empty, if it does - go in(this case accuring once, when the link list empty)*/
		temp->next = NULL;
		*hptr_operand_label = temp;				/*make temp the new header for the link list*/
		return;
	}
	while (p_operand_label->next) {
		if (!strcmp(p_operand_label->label, label)) {
			return;								/*dont insert the label if it already exists*/
		}
		p_operand_label = p_operand_label->next;
	}
	p_operand_label->next = temp;
	p_operand_label->next->next = NULL;
}

void check_if_operand_label_defined(OperandLabelPointer* hptr_operand_label, LabelPointer* hptr_label, char file_name[]) {	/*check whether the operand label appearing on label list*/
	OperandLabelPointer operand_label_ptr = *hptr_operand_label;
	LabelPointer label_ptr = *hptr_label;
	boolean is_exists;

	while (operand_label_ptr) {		/*for each operand label, check whether it appears on the symbol list*/
		is_exists = FALSE;
		label_ptr = *hptr_label;
		while (label_ptr) {
			if (!strcmp(operand_label_ptr->label, label_ptr->label)) 
				is_exists = TRUE;	/*if we got here it means that the operand label is appearing on label list*/				
			label_ptr = label_ptr->next;
		}
		if (!is_exists) {
			add_input_error_message(operand_label_not_defined, operand_label_ptr->number_of_line, file_name);
			return;
		}
		operand_label_ptr = operand_label_ptr->next;
	}
}

void freeOperandLabelList(OperandLabelPointer* hptr) {
	OperandLabelPointer p;
	while (*hptr) {
		p = *hptr;
		*hptr = p->next;
		free(p);
	}
}

void freeLabelList(LabelPointer* hptr) {		/*update the list itself - therefore we are sending &h and the parameter is ptr* */
	LabelPointer p;
	while (*hptr) {
		p = *hptr;
		*hptr = p->next;
		free(p);
	}
}

void freeCodeList(CodePointer* hptr) {
	CodePointer p;
	while (*hptr) {
		p = *hptr;
		*hptr = p->next;
		free(p);
	}
}

void freeDataList(DataPointer* hptr) {
	DataPointer p;
	while (*hptr) {
		p = *hptr;
		*hptr = p->next;
		free(p);
	}
}
