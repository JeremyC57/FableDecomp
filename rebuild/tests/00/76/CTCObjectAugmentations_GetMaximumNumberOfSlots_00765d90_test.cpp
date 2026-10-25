// Test for 0x00765D90 CTCObjectAugmentations::GetMaximumNumberOfSlots: links the source object; the result is the
// pointed-to def's MaximumNumberOfSlots (+0x2C), not NumberOfSlots (+0x28).
#include <stdio.h>
#include <string.h>
class CTCObjectAugmentations {
public:
    virtual void v00();
    long GetMaximumNumberOfSlots() const;
};
void CTCObjectAugmentations::v00() { }
static unsigned char g_obj[0x20];
static long g_def[16];
int main()
{
    CTCObjectAugmentations proto; memcpy(g_obj, &proto, 4);
    g_def[0x28 / 4] = 3; g_def[0x2c / 4] = 7;
    void* def = g_def; memcpy(g_obj + 0x0c, &def, 4);
    const CTCObjectAugmentations* a = (const CTCObjectAugmentations*)g_obj;
    bool ok = a->GetMaximumNumberOfSlots() == 7;
    g_def[0x2c / 4] = -2;
    ok = ok && a->GetMaximumNumberOfSlots() == -2;
    if (!ok) { puts("00765d90_TEST FAIL"); return 1; }
    puts("00765d90_TEST PASS");
    return 0;
}
