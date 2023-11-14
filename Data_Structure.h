
#define NumberOfCommands 16
#define LineLength 81	/*another 1 is for '\n' */
#define LabelLength 31

typedef enum { FALSE, TRUE } boolean;
typedef enum { code, data } cod_or_dat;
typedef enum { external, entry } ext_or_ent;

typedef struct {	/*table of all the commands*/
	char* name;
	int funct;
	int opcode;
} all_commands_table[NumberOfCommands];		

typedef struct labels_list* LabelPointer;	
typedef struct labels_list {					/*the structure of the symbol table*/
	char label[LabelLength];
	int address;
	cod_or_dat command_type;
	ext_or_ent ExtOrEnt;
	LabelPointer next;
}labels_list;

typedef struct operand_label* OperandLabelPointer;
typedef struct operand_label {					/*link list of all the label operands exists in the code*/
	char label[LabelLength];
	int number_of_line;		/*in order to make error message with the correct line in the code*/
	OperandLabelPointer next;
}operand_label;

typedef struct WordForData* DataPointer;
typedef struct WordForData {				/*the structure of a 'data command' word*/
	int address;
	unsigned int NumberInBits : 24;
	DataPointer next;
} WordForData;

typedef struct FirstWords {					/*the structure of the first word of code command*/
	unsigned int ARE : 3;
	unsigned int funct : 5;
	unsigned int DestinationRegisterName : 3;
	unsigned int DestinationAddressingMethod : 2;
	unsigned int SourceRegisterName : 3;
	unsigned int SourceAddressingMethod : 2;
	unsigned int opcode : 6;
} FirstWord;

typedef struct NextWordForCode {			/*the structure of the remaining words of code command*/
	unsigned int ARE : 3;
	unsigned int NumberInBits : 21;
	char operand[LineLength];
} NextWordForCode;

typedef struct CodeLinkedList* CodePointer;
typedef struct CodeLinkedList {				/*the structure of the code commad table*/
	int address[3];
	char input[LineLength];
	FirstWord word;
	NextWordForCode Elseword[2];
	CodePointer next;
} CodeLinkedList;

