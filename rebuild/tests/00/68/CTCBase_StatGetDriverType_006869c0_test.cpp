// Test for 0x006869C0 CTCBase::StatGetDriverType: a plain component is not a driver (type 0).
#include <stdio.h>
class CTCBase {
public:
    static unsigned long __fastcall StatGetDriverType();
};
unsigned long __fastcall CTCBase::StatGetDriverType()
{
    return 0;
}
class CTCPlain : public CTCBase { };
int main()
{
    unsigned long (__fastcall *fn)() = &CTCPlain::StatGetDriverType;
    if (CTCBase::StatGetDriverType() != 0 || fn() != 0) { puts("006869c0_TEST FAIL"); return 1; }
    puts("006869c0_TEST PASS");
    return 0;
}
