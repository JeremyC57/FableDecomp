// Behaviour test for CCombatAbility::IsActive (0x00733090): links the row object; true exactly when ActiveTimer (+4) >= 0, RepeatTimer (+0)
// ignored, nothing written.
#include <stdio.h>
#include <string.h>
extern "C" bool __fastcall RowIsActive(const void* self);
#pragma comment(linker, "/alternatename:@RowIsActive@4=?IsActive@CCombatAbility@@QBE_NXZ")
int main()
{
    static const long actives[] = { -2147483647L - 1, -1000, -2, -1, 0, 1, 2, 1000, 2147483647L };
    static const long repeats[] = { -1, 0, 5 };
    bool ok = true;
    for (unsigned r = 0; r < 3; ++r)
        for (unsigned i = 0; i < sizeof actives / sizeof actives[0]; ++i) {
            long a[4] = { repeats[r], actives[i], 0x11111111, 0x22222222 }, b[4];
            memcpy(b, a, sizeof a);
            bool got = RowIsActive(a), want = actives[i] >= 0;
            if (got != want || memcmp(a, b, sizeof a)) { printf("repeat %ld active %ld: %d want %d\n", repeats[r], actives[i], got, want); ok = false; }
        }
    printf(ok ? "00733090_TEST PASS\n" : "00733090_TEST FAIL\n");
    return ok ? 0 : 1;
}