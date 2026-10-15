// CTCInventoryMap::RemapUIDsForLoading(std::vector<unsigned __int64>&) -- rewrites every saved teleporter UID through the thing manager's serialisation remap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FAE90, 79 bytes (+1 int3). Single caller 0x006068C4.
// Callees: CThing::IsLevelLess @ 0x004C7930 (reads [thing+0x70]+0x41 bit 3), the thing-manager accessor @ 0x00686AF0
//          (`mov eax,[0x013B8A1C] ; ret`, a member on `this` that ignores it), CThingManager::RemapUIDForSerialisation
//          @ 0x0051E210 (`ret 14h`: bool, bool, ulong, unsigned __int64 -> unsigned __int64 in edx:eax).
//
// Shape: the vector is a reference (`mov ebx,[esp+8]`, first/last re-read through ebx: `mov eax,[ebx+4]` at the loop
// tail), an 8-byte element (`add esi,8`), the result stored back as edx:eax. Arguments evaluate right to left: the
// UID (hi then lo), the owner's level short (`movsx`), the IsLevelLess call on the owner, the constant `false`, then
// the manager accessor as the object expression. The owner is CTCBase::ParentThing at +4 (engine/CTCBase.h); the
// container is the retail 12-byte vector (first/last/end), modelled locally. CThing has no engine header.
#include "engine/CTCBase.h"
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};

struct CThing {
    unsigned char _pad_0x00[0x8e];
    short m_level;                                    // +0x8E
    bool IsLevelLess();                               // 0x004C7930
};

struct CUIDVector {                                   // retail 12-byte vector<unsigned __int64>
    unsigned __int64* m_first;
    unsigned __int64* m_last;
    unsigned __int64* m_end;
};

struct CTCInventoryMap : CTCBase {
    CThingManager* GetThingManager();                 // 0x00686AF0
    void RemapUIDsForLoading(CUIDVector& uids);
};

void CTCInventoryMap::RemapUIDsForLoading(CUIDVector& uids)
{
    for (unsigned __int64* it = uids.m_first; it != uids.m_last; ++it)
        *it = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, *it);
}