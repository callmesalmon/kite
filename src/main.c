#include <stdio.h>
#include <string.h>

#define VERSION          "386Kite"
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
		"    %s run <filename>\n"
		"    %s help\n"
		"    %s about\n",

		name, name, name
	);
}

void about()
{
	printf("Name: Kite\n");
    printf("Version: %s\n", VERSION);
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
	else if (args_count == 0)
	{
        printf("%s: Unknown command!\n", program_name);
	}
    else
    {
        about();
    }
	return 0;
}

int main(int argc, char **argv)
{
	int o = cli(argc, argv);
	return o;
}
