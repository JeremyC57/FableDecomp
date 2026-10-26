#include "engine/CEnemyDef.h"
#include <cstdio>
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};
struct CEnemyDef_Methods : CEnemyDef { void base_clone(const CDefClassBase* src); };
static void* g_self; static const void* g_src;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src; g_self = ((CEnemyDef_Methods*)this); g_src = src;
    return *this;
}
void __fastcall CEnemyDef_Copy(CEnemyDef* self, int, const CDefClassBase* src);
int main()
{
    CEnemyDef_Methods dst; dst.Faction = 0;
    CEnemyDef_Methods srcObj; srcObj.Faction = 0x12345678;
    CEnemyDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.Faction == 0x12345678 && g_self == &dst && g_src == &srcObj) printf("004e064e_TEST PASS\n");
    else printf("FAIL %x\n", dst.Faction);
    return 0;
}
