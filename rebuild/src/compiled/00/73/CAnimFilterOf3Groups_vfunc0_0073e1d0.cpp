// CAnimFilterOf3Groups::Accepts(const CAnimationEntry*) -- accept an animation in any of three named groups.
// Retail: 0x0073E1D0, 278 bytes; vtable 0x01267480 slot 0.
// Leaf: Accepts
// No shared CAnimFilterOf3Groups, CAnimationEntry, CCharString or CDefString header exists.
// Filter layout agrees with landed Clone 0x0073E2F0: strings +4/+8/+c, CRCs +10/+14/+18.
// Entry group ID +8 agrees with sibling filter 0x0073E940; string data text pointer +0.
// Callee: CDefStringTable::GetStringAsCharArray 0x009D49F0; table 0x013CA828.
#include <string.h>
extern const char g_EmptyString[]; // 0x0129AAF4
struct CCharStringData { const char* Text; };
struct CCharString {
    CCharStringData* PStringData;
    const char* c_str() const { if (!PStringData) return g_EmptyString; return PStringData->Text; }
};
struct CDefString { long TablePos; };
struct CAnimationEntry { long BankIndex; CDefString AnimName; CDefString GroupName; void* Components; };
struct CDefStringTable { const char* GetStringAsCharArray(long); }; // 0x009D49F0
extern CDefStringTable g_DefStringTable; // 0x013CA828
struct CAnimFilterOf3Groups {
    virtual long Accepts(const CAnimationEntry* anim);
    CCharString Group1, Group2, Group3;
    unsigned long Group1CRC, Group2CRC, Group3CRC;
};
long CAnimFilterOf3Groups::Accepts(const CAnimationEntry* anim)
{
    if (!strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group1.c_str()) ||
        !strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group2.c_str()) ||
        !strcmp(g_DefStringTable.GetStringAsCharArray(anim->GroupName.TablePos), Group3.c_str()))
        return 1;
    return 0;
}