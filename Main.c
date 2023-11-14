#include "Header.h"

int main(int argc, char *argv[]) {
	int i;
	all_commands_table commands_table[NumberOfCommands];
	for (i = 1; i < argc; i++) {
		handle_file(argv[i], commands_table);
	}
	puts("");
	return 0;
}
