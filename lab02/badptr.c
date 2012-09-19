#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *remove_whitespace(char *s) {
    char buffer[strlen(s) + 1];
    int i = 0, j = 0;
    for ( ; i < strlen(s); i += 1) {
        if (!isspace(s[i])) {
            buffer[j] = s[i];
            j += 1;
        }
    }
    buffer[j] = '\0';
    printf("%s\n", buffer);
    return buffer;
}

int main(int argc, char **argv) {
    char *s = strdup("  the \tinternet\t\nis a series of tubes  ");
    char *newstr = remove_whitespace(s);
    printf("%s\n", newstr);
    return 0;
}
