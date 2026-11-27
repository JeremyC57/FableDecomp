#include <stdio.h>
static int UnityWhenZeroOrWide(float macro, unsigned long column)
{
    if (macro == 0.0f)
        return 1;
    if (column >= 100)
        return 1;
    if (column < 20)
        column = 20;
    column -= 20;
    return column == 0 ? 2 : 3;
}
int main(void)
{
    if (UnityWhenZeroOrWide(0.0f, 5) != 1)
        return 1;
    if (UnityWhenZeroOrWide(1.0f, 100) != 1)
        return 1;
    if (UnityWhenZeroOrWide(1.0f, 10) != 2)
        return 1;
    if (UnityWhenZeroOrWide(1.0f, 25) != 3)
        return 1;
    printf("00c6be60_TEST PASS\n");
    return 0;
}