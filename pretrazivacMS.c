#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024


//ovo je za windows, na macu radi i bez ovoga
char *strcasestr(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *)haystack;
        }

        haystack++;
    }

    return NULL;
}

bool contains_word(const char *line, const char *word, bool ignore_case) {
    if (ignore_case) {
        while (*line) {
            if (strncasecmp(line, word, strlen(word)) == 0) {
                if (isspace(*(line + strlen(word))) || *(line + strlen(word)) == '\0') {
                    return true;
                }
            }
            line++;
        }
    } else {
        while (*line) {
            if (strncmp(line, word, strlen(word)) == 0) {
                if (isspace(*(line + strlen(word))) || *(line + strlen(word)) == '\0') {
                    return true;
                }
            }
            line++;
        }
    }
    return false;
}

void print_red(const char *line, const char *word) {
    const char *pos = line;
    while ((pos = strstr(pos, word)) != NULL) {
        printf("%.*s", (int)(pos - line), line);
        printf("\033[0;31m%s\033[0m", word);
        line = pos + strlen(word);
        pos = line;
    }
    printf("%s", line);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s [-i] [-w] <word> <filename>\n", argv[0]);
        printf("-i: Ignore case\n");
        printf("-w: Print lines without the word\n");
        printf("--help: Display this help message\n");
        return 0;
    }

    bool ignore_case = false;
    bool print_without_word = false;
    char *word;
    char *filename;

    for (int i = 1; i < argc - 2; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = true;
        } else if (strcmp(argv[i], "-w") == 0) {
            print_without_word = true;
        }
    }

    word = argv[argc - 2];
    filename = argv[argc - 1];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Ne moze otvoriti %s\n", filename);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;

        if (ignore_case) {
            if (strcasestr(line, word) != NULL) {
                if (!print_without_word) {
                    print_red(line, word);
                    printf("Red %d: %s", line_number, line);
                }
            } else if (print_without_word) {
                if (strcasestr(line, word) == NULL) {
                    printf("Red %d: %s", line_number, line);
                }
            }
        } else {
            if (strstr(line, word) != NULL) {
                if (!print_without_word) {
                    print_red(line, word);
                    printf("Red %d: %s", line_number, line);
                }
            } else if (print_without_word) {
                printf("Red %d: %s", line_number, line);
            }
        }
    }

    fclose(file);

    return 0;
}