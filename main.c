#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <argp.h>

static char doc[] = "Simple password generator.";
static char args_doc[] = "";

static struct argp_option options[] = {
    {"length", 'l', "NUMBER", 0, "Set password length"},
    {"complexity", 'c', "NUMBER", 0, "Set complexity level"},
    {0}
};

struct arguments {
    int length;
    int complexity;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    int length;
    int complexity;

    switch (key) {
        case 'l':
            length = atoi(arg);
            if (length < 8 || length > 50) {
                argp_error(state, "Password length must be between 8 and 50.");
            }
            arguments->length = length;
            break;
        case 'c':
            complexity = atoi(arg);
            if (complexity < 1 || complexity > 4) {
                argp_error(state, "Complexity level must be between 1 and 4.");
            }
            arguments->complexity = complexity;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

char *generate_password(int length, int complexity) {
    char *password = (char *)calloc(length + 1, sizeof(char));

    if (!password) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char *characters;

    switch (complexity) {
        case 1:
            characters = "abcdefghijklmnopqrstuvwxyz";
            break;
        case 2:
            characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            break;
        case 3:
            characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            break;
        case 4:
            characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
            break;
    }

    int characters_total = strlen(characters);

    srand(time(NULL));

    for (int index = 0; index < length; index++) {
        password[index] = characters[rand() % characters_total];
    }

    return password;
}

void main(int argc, char **argv) {
    struct arguments arguments;

    arguments.length = 8;
    arguments.complexity = 1;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    char *password = generate_password(arguments.length, arguments.complexity);

    puts(password);

    free(password);

    exit(EXIT_SUCCESS);
}
