
typedef enum {
	not_exists, location_illegal, no_comma_between_operands, operand_is_missing, extraneous_syntax,
	the_command_is_written_illegally, destination_operand_cannot_be_number, label_cannot_be_named_by_register,
	operand_is_illegal, data_command_illegal, the_label_is_written_illegally,
	the_command_has_no_operands, label_is_already_exists, operand_label_not_defined, missing_apostrophes,
	there_is_space_before_note, label_cannot_be_named_by_code_command, 
	data_command_has_no_opernads
} input_error_message;

typedef enum { cannot_allocate_memory, cannot_open_file , cannot_create_file , file_is_empty} program_error_message;

void add_input_error_message(input_error_message message, int number_of_line, char file_name[]);	/*sends error if we have a program problem*/
void add_program_error_message(program_error_message message, char file_name[]);		/*sends error if we have a problem with the input */
void check_malloc_validation(void* object, char file_name[]);							/*making sure the malloc went through properly*/

