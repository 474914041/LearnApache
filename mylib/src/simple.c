#include "simple.h"
#include "string.h"
void sayhello(char *p, int len) {
    if (p == NULL || len <= 0) {
        return;
    }

    strncpy(p, "Words from libso", len);
    return;
}