#include "Header.h"

void check_malloc_validation(void* object, char file_name[]) {
	if (!object) {
		add_program_error_message(cannot_allocate_memory, file_name);
		exit(0);
	}
}

void add_program_error_message(program_error_message message, char file_name[]) {
	is_error_msg = TRUE;
	switch (message) {
	case 0:
		fprintf(stderr, "\nCannot allocate memory, in file '%s'", file_name);
		break;
	case 1:
		fprintf(stderr, "\nCannot open file '%s'", file_name);
		break;
	case 2:
		fprintf(stderr, "\nCannot create file '%s'", file_name);
		break;
	case 3:
		fprintf(stderr, "\nFile '%s' is empty", file_name);
		break;
	}
}

void add_input_error_message(input_error_message message, int number_of_line, char file_name[]) {
	is_error_msg = TRUE;
	switch (message) {
	case 0:
		fprintf(stderr, "\nThe command is not exists, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 1:
		fprintf(stderr, "\nLocation of the commend is illegal, on line %d, in file '%s'", number_of_line,file_name);
		break;
	case 2:
		fprintf(stderr, "\nNo comma between operands, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 3:
		fprintf(stderr, "\nOperand is missing, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 4:
		fprintf(stderr, "\nExtraneous syntax, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 5:
		fprintf(stderr, "\nThe command is written illegally, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 6:
		fprintf(stderr, "\nDestination operand cannot be number, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 7:
		fprintf(stderr, "\nLabel cannot be named by register, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 8:
		fprintf(stderr, "\nOperand is illegal, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 9:
		fprintf(stderr, "\nData command is illegal, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 10:
		fprintf(stderr, "\nThe label is written illegally, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 11:
		fprintf(stderr, "\nThe command has no operands, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 12:
		fprintf(stderr, "\nLabel is already exists, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 13:
		fprintf(stderr, "\nOperand label is not defined, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 14:
		fprintf(stderr, "\nMissing apostropheson, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 15:
		fprintf(stderr, "\nThere is space before the note, on line %d, in file '%s'", number_of_line, file_name);
		break;
	case 16:
		fprintf(stderr, "\nLabel cannot be named by code command, on line % d, in file '%s'", number_of_line, file_name);
		break;
	case 17:
		fprintf(stderr, "\nData command has no opernads, on line % d, in file '%s'", number_of_line, file_name);
		break;
	}
}

boolean check_if_row_is_empty(char input[], int number_of_line, char file_name[]) {
	int i = 0;
	while (isspace(input[i]) || input[i] == '\0' || input[i] == ';') {				/*check if there is empty row*/
		if (input[i] == '\0' || input[i] == ';') {
			if (input[i] == ';' && i != 0) {
				add_input_error_message(there_is_space_before_note, number_of_line, file_name);
			}
			return TRUE;
		}
		i++;
	}
	return FALSE;
}

boolean check_if_file_is_empty(FILE* file_ptr, char file_name[]) {
	int size = 0;
	if (file_ptr) {
		fseek(file_ptr, 0, SEEK_END);
		size = ftell(file_ptr);
		if (0 == size) {
			add_program_error_message(file_is_empty, file_name);
			return TRUE;
		}
	}
	fseek(file_ptr, 0, SEEK_SET);
	return FALSE;
}

void deleting_empty_labels(LabelPointer* hptr_label) {
	LabelPointer temp = *hptr_label;
	LabelPointer prev;
	LabelPointer curr;

	boolean is_deleted = FALSE;

	if (temp->label[0] == '\0') {		/*in case there is empty label in the head*/
		*hptr_label = temp->next;
		free(temp);
	}
	prev = *hptr_label;
	curr = prev->next;
	while (prev && curr) {
		is_deleted = FALSE;
		if (curr->label[0] == '\0') {
			temp = curr;
			prev->next = curr->next;
			curr = curr->next;
			free(temp);
			is_deleted = TRUE;
		}
		if (!is_deleted) {
			curr = curr->next;
			prev = prev->next;
		}
	}
}
