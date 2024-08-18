#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <argp.h>

static char doc[] = "Simple password generator.";
static char args_doc[] = "LENGTH COMPLEXITY";

static struct argp_option options[] = {
    {"avoid-repeats", 'a', 0, 0, "Avoid repeated characters"},
    {0}
};

struct arguments {
    int length;
    int complexity;
    int avoid_repeats;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    int length;
    int complexity;

    switch (key) {
        case 'a':
            arguments->avoid_repeats = true;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2) {
                argp_usage(state);
            }

            if (state->arg_num == 0) {
                length = atoi(arg);
                if (length < 8 || length > 50) {
                    argp_error(state, "Password length must be between 8 and 50.");
                }
                arguments->length = length;
            }

            if (state->arg_num == 1) {
                complexity = atoi(arg);
                if (complexity < 1 || complexity > 4) {
                    argp_error(state, "Complexity level must be between 1 and 4.");
                }
                arguments->complexity = complexity;
            }

            break;
        case ARGP_KEY_END:
            if (state->arg_num < 2) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

char *generate_password(int length, int complexity, bool avoid_repeats) {
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

    if (avoid_repeats) {
        int index = 0;
        char random_char;

        while (index < length) {
            random_char = characters[rand() % characters_total];

            if (strchr(password, random_char)) {
                continue;
            }

            password[index] = random_char;
            index++;
        }
    } else {
        for (int index = 0; index < length; index++) {
            password[index] = characters[rand() % characters_total];
        }
    }

    return password;
}

void main(int argc, char **argv) {
    struct arguments arguments;

    arguments.avoid_repeats = false;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    char *password = generate_password(arguments.length, arguments.complexity, arguments.avoid_repeats);

    puts(password);

    free(password);

    exit(EXIT_SUCCESS);
}
