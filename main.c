#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_ARGS_LENGTH 2
#define MAX_ARGS_LENGTH 3
#define MIN_PASSWD_LENGTH 8
#define MAX_PASSWD_LENGTH 50
#define MIN_COMPLXT_LEVEL 1
#define MAX_COMPLXT_LEVEL 4
#define CHARS_SET_SIZE 95

char *generate_password(int passwd_length, int complxt_level) {
    // Dynamically allocates memory for the password array and adds one more
    // null byte ('\0') for the string termination.
    char *random_passwd = (char *)calloc(passwd_length + 1, sizeof(char));

    if (random_passwd == NULL) {
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

    // Sets the complexity level of the password.
    switch (complxt_level) {
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

    // Sets the seed for the rand() function based on the current time.
    srand(time(NULL));

    int random_index;

    // Generates a random character and adds it on the password array.
    for (int index = 0; index < passwd_length; index++) {
        random_index = rand() % (upper_index - lower_index) + lower_index;
        random_passwd[index] = characters_set[random_index];
    }

    return random_passwd;
}

int main(int argc, char *argv[]) {
    if (argc < MIN_ARGS_LENGTH) {
        printf("Usage: %s LENGTH COMPLEXITY\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc > MAX_ARGS_LENGTH) {
        printf("Too many arguments\n");
        return EXIT_FAILURE;
    }

    if (!argv[1]) {
        printf("LENGTH argument is required\n");
        return EXIT_FAILURE;
    }

    if (!argv[2]) {
        printf("COMPLEXITY argument is required\n");
        return EXIT_FAILURE;
    }

    // Converts the arguments to an integer variable.
    int passwd_length = atoi(argv[1]);
    int complxt_level = atoi(argv[2]);

    if (passwd_length < MIN_PASSWD_LENGTH) {
        printf("Password length must be at least 8 characters\n");
        return EXIT_FAILURE;
    }

    if (passwd_length > MAX_PASSWD_LENGTH) {
        printf("Password length must be at most 50 characters\n");
        return EXIT_FAILURE;
    }

    if (complxt_level < MIN_COMPLXT_LEVEL) {
        printf("Password complexity must be at least level 1\n");
        return EXIT_FAILURE;
    }

    if (complxt_level > MAX_COMPLXT_LEVEL) {
        printf("Password complexity must be at most level 4\n");
        return EXIT_FAILURE;
    }

    char *random_passwd = generate_password(passwd_length, complxt_level);

    printf("Password: ");

    // Prints each character of the password.
    for (int index = 0; index < passwd_length; index++) {
        printf("%c", random_passwd[index]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}
