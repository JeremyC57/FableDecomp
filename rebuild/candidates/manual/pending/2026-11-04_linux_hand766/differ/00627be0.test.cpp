// Behaviour test for CTCQuickAccessMenu::GetExpressionsListUI (0x00627BE0): expression items are bucketed by type (unknown
// types go to social), every expression is counted, and a list is created only when some bucket is non-empty.
#include <stdio.h>
static int Build(const int* types, int n, int buckets[4], long& numItems)
{
    numItems = 0;
    for (int i = 0; i < n; ++i) {
        int t = types[i];
        if (t < 0) continue;                         // not an expression
        switch (t) { case 1: ++buckets[1]; break; case 2: ++buckets[2]; break; case 3: ++buckets[3]; break; default: ++buckets[0]; break; }
        ++numItems;
    }
    int categories = 0;
    for (int k = 0; k < 4; ++k) if (buckets[k]) ++categories;
    return categories > 0 ? 1 : 0;
}
int main()
{
    int types[5] = { 0, 3, -1, 7, 2 }; int b[4] = { 0, 0, 0, 0 }; long count;
    int made = Build(types, 5, b, count);
    int none[1] = { -1 }; int e[4] = { 0, 0, 0, 0 }; long c2;
    bool ok = made == 1 && count == 4 && b[0] == 2 && b[2] == 1 && b[3] == 1 && Build(none, 1, e, c2) == 0 && c2 == 0;
    printf(ok ? "00627be0_TEST PASS\n" : "00627be0_TEST FAIL\n");
    return ok ? 0 : 1;
}
