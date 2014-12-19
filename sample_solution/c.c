#include <stdio.h>

int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d\n", a+b);
    int i, j;
    for(i = 1; i < 10000; i++) {
        for (j = 1; j < 10000; j ++) a++;
    }
    printf("%d\n", a+b);
    return 0;
}
