#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <argp.h>

char doc[] = "Simple password generator.";
char args_doc[] = "LENGTH COMPLEXITY";

struct argp_option options[] = {
    {"avoid-repeats", 'a', 0, 0, "Avoid repeated characters"},
    {"output", 'o', "FILE", 0, "Write output data to FILE"},
    {"quantity", 'q', "NUMBER", 0, "Define how many passwords will be generated"},
    {0}
};

struct arguments {
    int length;
    int complexity;
    bool avoid_repeats;
    char *output_file;
    int quantity;
};

error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    int length;
    int complexity;
    int quantity;

    switch (key) {
        case 'a':
            arguments->avoid_repeats = true;
            break;
        case 'o':
            arguments->output_file = arg;
            break;
        case 'q':
            quantity = atoi(arg);
            if (quantity < 1) {
                argp_error(state, "Quantity value must be a positive number.");
            }
            arguments->quantity = quantity;
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

struct argp argp = {options, parse_opt, args_doc, doc};

char *generate_password(int length, int complexity, bool avoid_repeats) {
    char *password = (char *)malloc((length + 1) * sizeof(char));

    if (!password) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char *charset;

    switch (complexity) {
        case 1:
            charset = "abcdefghijklmnopqrstuvwxyz";
            break;
        case 2:
            charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            break;
        case 3:
            charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            break;
        case 4:
            charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
            break;
    }

    int charset_size = strlen(charset);

    if (avoid_repeats) {
        int index = 0;
        char random_char;

        while (index < length) {
            random_char = charset[rand() % charset_size];

            if (strchr(password, random_char)) {
                continue;
            }

            password[index] = random_char;
            index++;
        }
    } else {
        for (int index = 0; index < length; index++) {
            password[index] = charset[rand() % charset_size];
        }
    }

    password[length] = '\0';

    return password;
}

void main(int argc, char **argv) {
    struct arguments arguments;

    arguments.avoid_repeats = false;
    arguments.output_file = NULL;
    arguments.quantity = 1;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    srand(time(NULL));

    char *password;

    if (arguments.output_file) {
        FILE *file = fopen(arguments.output_file, "w");

        if (!file) {
            perror("File opening error");
            exit(EXIT_FAILURE);
        }

        for (int counter = 0; counter < arguments.quantity; counter++) {
            password = generate_password(arguments.length, arguments.complexity, arguments.avoid_repeats);
            fwrite(password, sizeof(char), arguments.length, file);
            fwrite("\n", sizeof(char), 1, file);
            free(password);
        }

        fclose(file);
    } else {
        for (int counter = 0; counter < arguments.quantity; counter++) {
            password = generate_password(arguments.length, arguments.complexity, arguments.avoid_repeats);
            puts(password);
            free(password);
        }
    }

    exit(EXIT_SUCCESS);
}
