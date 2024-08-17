#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <argp.h>

#define CHARS_SET_SIZE 95

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

    return EXIT_SUCCESS;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

char *generate_password(int length, int complexity) {
    char *password = (char *)calloc(length + 1, sizeof(char));

    if (!password) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char characters_set[CHARS_SET_SIZE] = {
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    };

    int lower_index;
    int upper_index;

    switch (complexity) {
        case 1:
            lower_index = 10;
            upper_index = 36;
            break;
        case 2:
            lower_index = 10;
            upper_index = 56;
            break;
        case 3:
            lower_index = 0;
            upper_index = 56;
            break;
        case 4:
            lower_index = 0;
            upper_index = 94;
            break;
    }

    srand(time(NULL));

    int random_index;

    for (int index = 0; index < length; index++) {
        random_index = rand() % (upper_index - lower_index) + lower_index;
        password[index] = characters_set[random_index];
    }

    return password;
}

int main(int argc, char **argv) {
    struct arguments arguments;

    arguments.length = 8;
    arguments.complexity = 1;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    char *password = generate_password(arguments.length, arguments.complexity);

    for (int index = 0; index < arguments.length; index++) {
        printf("%c", password[index]);
    }

    printf("\n");

    free(password);

    exit(EXIT_SUCCESS);
}
