// Behaviour test for CTCFineDialogue::Show (007e27f0): guards, stored fields, text assembly and the pay / lack-funds choice
#include <cstdio>
#include <cstring>

struct Dialogue { long fine, money; bool chance, waiting; const void* village; int registered; char text[256]; };

static void Show(Dialogue& d, bool allowed, long fine, long money, const void* village, const char* report)
{
    if (!allowed || fine <= 0 || !village)
        return;
    d.fine = fine;
    d.money = money;
    d.village = village;
    char buf[256];
    strcpy(d.text, "PAY_FOR\n");
    strcat(d.text, report);
    strcat(d.text, "\n");
    if (d.fine <= d.money) {
        sprintf(buf, "HAND_OVER %ld", fine);
        strcat(d.text, buf);
        d.waiting = true;
        ++d.registered;
    } else {
        sprintf(buf, "LACK_FUNDS %ld", fine);
        strcat(d.text, buf);
        d.waiting = false;
        d.chance = false;
    }
}

int main()
{
    int fails = 0;
    int village = 1;
    Dialogue d;
    memset(&d, 0, sizeof(d));
    d.chance = true;
    Show(d, false, 10, 100, &village, "theft");
    Show(d, true, 0, 100, &village, "theft");
    Show(d, true, 10, 100, 0, "theft");
    if (d.fine != 0 || d.text[0]) ++fails;
    Show(d, true, 10, 100, &village, "theft");
    if (d.fine != 10 || d.money != 100 || !d.waiting || d.registered != 1 || strcmp(d.text, "PAY_FOR\ntheft\nHAND_OVER 10")) ++fails;
    Show(d, true, 500, 100, &village, "murder");
    if (d.waiting || d.chance || d.registered != 1 || strcmp(d.text, "PAY_FOR\nmurder\nLACK_FUNDS 500")) ++fails;
    Show(d, true, 100, 100, &village, "x");
    if (!d.waiting || d.registered != 2) ++fails;
    if (fails) {
        printf("007e27f0_TEST FAIL\n");
        return 1;
    }
    printf("007e27f0_TEST PASS\n");
    return 0;
}