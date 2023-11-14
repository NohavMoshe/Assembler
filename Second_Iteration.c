#include "Header.h"

void fixing_the_label_linked_list(LabelPointer* hptr_label, LabelPointer* hptr_label_for_data, int* IC) {
	LabelPointer labelspointer1 = *hptr_label;
	LabelPointer labelspointer2 = labelspointer1;
	LabelPointer labelfordata = *hptr_label_for_data;
	boolean hasChanged = FALSE;
	boolean is_first = FALSE;
	int previous_data_address = 0;
	int temp = 0;
	int IC_demo = *IC;

	/*Deleting the node in the 'hptr_label' that are not places in the right place*/
	while (labelfordata != NULL) {
		labelspointer2 = labelspointer1;
		hasChanged = FALSE;
		while (labelspointer2 != NULL && hasChanged == FALSE ) {
			if (strcmp(labelspointer2->label, labelfordata->label) == 0) {
				if (labelspointer2->ExtOrEnt == entry) {
					labelfordata->ExtOrEnt = entry;
				}
				labelspointer1 = labelspointer1->next;
				labelspointer2->next = NULL;
				labelspointer2 = labelspointer1;
				hasChanged = TRUE;
			}
			if (hasChanged == FALSE) {
				labelspointer2 = labelspointer2->next;
			}
		}
		labelfordata = labelfordata->next;
	}
	/*updating the 'hptr_label_for_data' with the real address*/
	labelfordata = *hptr_label_for_data;
	while (labelfordata) {
		if (!is_first) {
			previous_data_address = labelfordata->address; /* adding the next address after the last code command to the label address */
			labelfordata->address = IC_demo;
			is_first = TRUE;
		}
		else {
			temp = labelfordata->address;
			labelfordata->address = previous_data_address + IC_demo;
			IC_demo = IC_demo + previous_data_address;
			previous_data_address = temp;
		}
		labelfordata = labelfordata->next;
	}

	/*adding the end of 'hptr_label' to the start of the 'hptr_label_for_data'*/
	labelspointer2 = labelspointer1;
	while (labelspointer2->next != NULL) {
		labelspointer2 = labelspointer2->next;
	}
	labelspointer2->next = *hptr_label_for_data;
	*hptr_label = labelspointer1;
}



void second_running_on_code_linked_list(CodePointer* HeadCodeLinkedList, DataPointer* HeadDataLinkedList, LabelPointer* HeadLabelLinkedList, char file_name[], int* IC, int* DC) {
	CodePointer codepointer = *HeadCodeLinkedList;
	LabelPointer labelspointer= *HeadLabelLinkedList;
	int i;

	while (codepointer != NULL) /*running on the whole code linked list, node by node*/
	{
		for (i = 0; i < 2; i++) /*2 because there is at list 2 else words*/
		{
			if (codepointer->Elseword[i].NumberInBits == '?') /* '?' is where a address should be */
			{
				labelspointer = *HeadLabelLinkedList;
				while (labelspointer != NULL) /*running on the label linked list*/
				{
					if ((labelspointer->command_type == code || labelspointer->command_type == data) && strcmp(codepointer->Elseword[i].operand, labelspointer->label) == 0) /*trying to match between the data labels in the linked list*/
					{
						codepointer->Elseword[i].NumberInBits = labelspointer->address; /*inserting the address to the code linked list*/
					}
					if (labelspointer->ExtOrEnt == external && strcmp(codepointer->Elseword[i].operand, labelspointer->label) == 0) { /*checking whether the label is external, if it is- NumberInBits should be 0 and ARE =1*/
						codepointer->Elseword[i].ARE = 1;
						codepointer->Elseword[i].NumberInBits = 0;
					}
					labelspointer = labelspointer->next; /*preceding to the next label in the linked list*/
				}
			}
			if (codepointer->Elseword[i].NumberInBits == '@') /* '@' is where a distance should be*/
			{
				labelspointer = *HeadLabelLinkedList;
				while (labelspointer != NULL) /*running on the label linked list*/
				{
					if (labelspointer->command_type == code && strcmp(codepointer->Elseword[i].operand, labelspointer->label) == 0) /*trying to match between the code labels in the linked list*/
						codepointer->Elseword[i].NumberInBits = labelspointer->address - codepointer->address[0]; /*inserting the address to the code linked list*/
					labelspointer = labelspointer->next; /*preceding to the next label in the linked list*/
				}
			}
		}
		codepointer = codepointer->next; /*preceding to the next code command in the linked list*/
	}
	print_ps_ob_file(HeadCodeLinkedList, HeadDataLinkedList, file_name, IC, DC);
	print_ps_ent_ext_files(HeadCodeLinkedList, HeadLabelLinkedList, file_name);
}

/*.ob File printing*/
void print_ps_ob_file(CodePointer* HeadCodeLinkedList, DataPointer* HeadDataLinkedList, char file_name[], int* IC, int* DC) {
	CodePointer codepointer = *HeadCodeLinkedList;
	DataPointer datapointer = *HeadDataLinkedList;
	FILE* ps_ob;
	int i = 0;
	int final_number_in_word, final_number_in_Elseword;
	char* fileName_ps_ob = file_name;
	char real_file_name[LineLength];

	/*creating the proper name of the file depands on the name*/
	while (*fileName_ps_ob != '.') {
		real_file_name[i]= *(fileName_ps_ob);
		i++;
		fileName_ps_ob = fileName_ps_ob + 1;
	}
	real_file_name[i] = '\0';
	strcat(real_file_name, ".ob");

	/*opening the file for writing*/
	ps_ob = fopen(real_file_name, "w");
	if (!ps_ob) {
		add_program_error_message(cannot_create_file, real_file_name);
		exit(1);
	}
	fprintf(ps_ob, "%d\t%d\n", *IC - 100, *DC); /*printing the IC AND DC*/

	while (codepointer != NULL) {
		/*summing the final number in the word*/
		final_number_in_word = codepointer->word.ARE +
			(codepointer->word.funct) * pow(2, 3) +
			(codepointer->word.DestinationRegisterName) * pow(2, 8) +
			(codepointer->word.DestinationAddressingMethod) * pow(2, 11) +
			(codepointer->word.SourceRegisterName) * pow(2, 13) +
			(codepointer->word.SourceAddressingMethod) * pow(2, 16) +
			(codepointer->word.opcode) * pow(2, 18);
		/*printing the first word*/

		fprintf(ps_ob, "%d\t%x\n", codepointer->address[0], final_number_in_word); /*printing the first word*/
		/*dealing with the Elsewords if exsist*/
		for (i = 0; i < 2; i++)
		{
			if (codepointer->Elseword[i].operand[0] != '\0') /*means that there is Elseword in this case*/
			{
				/*summing the final number in the Elseword*/
				final_number_in_Elseword = codepointer->Elseword[i].ARE + (codepointer->Elseword[i].NumberInBits) * pow(2, 3);
				/*printing the Elseword*/
				fprintf(ps_ob, "%d\t%x\n", codepointer->address[i + 1], final_number_in_Elseword); /*printing the first word*/
			}
		}
		codepointer = codepointer->next;
	}
	/*printing the data in the data linked list*/
	while (datapointer != NULL) {
		final_number_in_word = datapointer->NumberInBits;
		if (datapointer->address < *IC) { /*checking whether the data wasn't defined with label by its side*/
			datapointer->address = datapointer->address + *IC;
		}
		fprintf(ps_ob, "%d\t%x\n", datapointer->address, final_number_in_word); /*printing the word*/
		datapointer = datapointer->next;
	}
}

/*.ent & .ext Files printing*/
void print_ps_ent_ext_files(CodePointer* HeadCodeLinkedList, LabelPointer* HeadLabelLinkedList, char file_name[]) {
	CodePointer codePointer;
	LabelPointer labelPointer = *HeadLabelLinkedList;
	FILE* ps_ent;
	FILE* ps_ext;
	char* fileName_ent_ext = file_name;
	char real_file_name_ent[LineLength];
	char real_file_name_ext[LineLength];
	int i = 0;

	while (*fileName_ent_ext != '.') {
		real_file_name_ent[i] = *(fileName_ent_ext);
		real_file_name_ext[i] = *(fileName_ent_ext);
		i++;
		fileName_ent_ext = fileName_ent_ext + 1;
	}
	real_file_name_ent[i] = '\0';
	strcat(real_file_name_ent, ".ent");
	real_file_name_ext[i] = '\0';
	strcat(real_file_name_ext, ".ext");
	/*Opening this file for writing*/
	ps_ent = fopen(real_file_name_ent, "w");
	if (!ps_ent) {
		add_program_error_message(cannot_create_file, real_file_name_ent);
		exit(1);
	}
	/*Opening this file for writing*/
	ps_ext = fopen(real_file_name_ext, "w");
	if (!ps_ext) {
		add_program_error_message(cannot_create_file, real_file_name_ext);
		exit(1);
	}
	/*Printing in the 2 files*/
	while (labelPointer != NULL) {
		if (labelPointer->ExtOrEnt == entry) /*if the Label is entry*/
			fprintf(ps_ent, "%s\t%d\n", labelPointer->label, labelPointer->address);
		if (labelPointer->ExtOrEnt == external) { /*if the Label is external*/
			codePointer = *HeadCodeLinkedList;
			/*Dealing with all the encounters with the external label and printing the label itself and the relevant address*/
			while (codePointer != NULL) {
				if (strcmp(labelPointer->label, codePointer->Elseword[0].operand) == 0) {
					fprintf(ps_ext, "%s\t%d\n", labelPointer->label, *(codePointer->address) + 1);
				}
				if (strcmp(labelPointer->label, codePointer->Elseword[1].operand) == 0) {
					fprintf(ps_ext, "%s\t%d\n", labelPointer->label, *(codePointer->address) + 2);
				}
				codePointer = codePointer->next;
			}
		}
		labelPointer = labelPointer->next; /*preceding to the next label for check*/
	}
}
