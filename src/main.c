#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERSION_MAJOR    1
#define VERSION_MINOR    0
#define VERSION_PATCH    0
#define KITE_STR_OBJ_LIM 1024

#define bool  char
#define true  1
#define false 0

#define dmain_printf(...)
#define dmain_puts(...)

#include "intr.h"

void help(const char *name)
{
	printf(
		"Usage:\n"
		"  %s run <filename>\n"
		"  %s help\n"
		"  %s about\n",

		name, name, name
	);
}

void about()
{
	printf("(name): Kite\n(version): %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

int cli(int argc, char **argv)
{
	const char *program_name = argv[0];

	if(argc < 2) return help(program_name), 1;

	char *command = argv[1];
	char **args = argv + 2;
	int args_count = argc - 2;

	if(strcmp(command, "run") == 0)
	{
		if(args_count != 1) return help(program_name), 1;
		free_context(run_file(args[0], true));
	}
	else if(strcmp(command, "help") == 0)
	{
		help(program_name);
	}
	else if(strcmp(command, "about") == 0)
	{
		if(args_count != 0) return help(program_name), 1;
		about();
	}
	else if (args_count != 0)
	{
		free_context(run_file(argv[1], true));
	}
	return 0;
}

int main(int argc, char **argv)
{
	int o = cli(argc, argv);
	return o;
}
