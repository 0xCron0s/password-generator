#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_ARGS_LENGTH 2
#define MAX_ARGS_LENGTH 2
#define MIN_PASSWD_LENGTH 1

char *generate_password(int passwd_length) {
    // Dynamically allocates memory for the password array and
    // adds 1 null byte ('\0') for the string termination.
    char *password = (char *)calloc(passwd_length + 1, sizeof(char));

    if (password == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Sets the seed for the rand() function based on the current time.
    srand(time(NULL));

    // Generates a random ASCII character and adds it on the password array.
    for (int index = 0; index < passwd_length; index++) {
        // 33 = minimal visible ASCII character code
        // 126 = maximum visible ASCII character code (127 doesn't count)
        password[index] = (char)(rand() % (127 - 33) + 33);
    }

    return password;
}

int main(int argc, char *argv[]) {
    if (argc < MIN_ARGS_LENGTH) {
        printf("Usage: %s [passwd_length]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc > MAX_ARGS_LENGTH) {
        printf("Too many arguments\n");
        return EXIT_FAILURE;
    }

    // Converts the second argument to an integer variable.
    int passwd_length = atoi(argv[1]);

    if (passwd_length < MIN_PASSWD_LENGTH) {
        printf("The value must be a positive number\n");
        return EXIT_FAILURE;
    }

    char *password = generate_password(passwd_length);

    printf("Password: ");

    // Prints each character of the password.
    for (int index = 0; index < passwd_length; index++) {
        printf("%c", password[index]);
    }

    printf("\n");

    return EXIT_SUCCESS;
}
