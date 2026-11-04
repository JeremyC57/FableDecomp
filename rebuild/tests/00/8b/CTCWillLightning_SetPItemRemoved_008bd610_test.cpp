// Behaviour test for CTCWillLightning::SetPItemRemoved (0x008BD610): the by-value intelligent pointer is assigned to the member.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Ptr { int* item; };
struct Spell { Ptr PItemRemoved; void SetPItemRemoved(Ptr item) { PItemRemoved = item; } };
int main()
{
    int thing = 5;
    Spell s; s.PItemRemoved.item = 0;
    Ptr p = { &thing };
    s.SetPItemRemoved(p);
    bool ok = s.PItemRemoved.item == &thing;
    printf(ok ? "008bd610_TEST PASS\n" : "008bd610_TEST FAIL\n");
    return ok ? 0 : 1;
}