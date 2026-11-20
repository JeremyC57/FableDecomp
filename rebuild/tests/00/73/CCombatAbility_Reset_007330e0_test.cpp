// Behaviour test for CCombatAbility::Reset (0x007330E0): links the row object; RepeatTimer (+0) and ActiveTimer (+4) become -1, nothing else.
#include <stdio.h>
#include <string.h>
extern "C" void __fastcall RowReset(void* self);
#pragma comment(linker, "/alternatename:@RowReset@4=?Reset@CCombatAbility@@QAEXXZ")
int main()
{
    static const long init[][2] = { { 5, 7 }, { 0, 0 }, { -1, -1 }, { -5, 123 } };
    bool ok = true;
    for (unsigned i = 0; i < 4; ++i) {
        long a[4] = { init[i][0], init[i][1], 0x55555555, 0x66666666 };
        RowReset(a);
        if (a[0] != -1 || a[1] != -1 || a[2] != 0x55555555 || a[3] != 0x66666666) { printf("case %u: %ld %ld\n", i, a[0], a[1]); ok = false; }
    }
    printf(ok ? "007330e0_TEST PASS\n" : "007330e0_TEST FAIL\n");
    return ok ? 0 : 1;
}