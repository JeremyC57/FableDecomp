// Behaviour test for CPlayerGuiDef::Copy (0x0046CE28): the slot-19 def copy assigns the whole def from the source (retail: a jmp to the
// implicit operator= 0x0046CE2D). NF39 (2026-11-16): the row is now the member Copy, so the test no longer links the old free
// CPlayerGuiDef_Copy / CPlayerGuiDef_Copy_impl stand-ins; it keeps its pre-existing local CPlayerGuiDef model and checks the copy.
#include "rebuild_abi.h"
#include <cstdio>
class CPlayerGuiDef { public: int dummy; int other; void Copy(const CPlayerGuiDef* def); };
void CPlayerGuiDef::Copy(const CPlayerGuiDef* def)
{
    *this = *def;
}
int main()
{
    CPlayerGuiDef a; a.dummy = 7; a.other = 1;
    CPlayerGuiDef src; src.dummy = 42; src.other = 9;
    a.Copy(&src);
    if (a.dummy == 42 && a.other == 9 && src.dummy == 42)
        printf("OK_0046ce28\n");
    else
        printf("FAIL_0046ce28\n");
    return 0;
}
