#include "engine/CTavernDef.h"
#include <cstdio>
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};
struct CTavernDef_Methods : CTavernDef { void base_clone(const CDefClassBase* src); };
static void* g_self; static const void* g_src;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src; g_self = ((CTavernDef_Methods*)this); g_src = src;
    return *this;
}
void __fastcall CTavernDef_Copy(CTavernDef* self, int, const CDefClassBase* src);
int main()
{
    CTavernDef_Methods dst; dst.BedCost = 0;
    CTavernDef_Methods srcObj; srcObj.BedCost = 0x12345678;
    CTavernDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.BedCost == 0x12345678 && g_self == &dst && g_src == &srcObj) printf("004e0f02_TEST PASS\n");
    else printf("FAIL %x\n", dst.BedCost);
    return 0;
}
