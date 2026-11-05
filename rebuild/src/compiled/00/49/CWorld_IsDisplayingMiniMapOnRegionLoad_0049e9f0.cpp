// CWorld::IsDisplayingMiniMapOnRegionLoad @ 0x0049e9f0
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pGlobalGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Loads a global pointer (ds:0x13b8790), reads a byte flag at +0xf6 on that
// global object; if the flag is set, returns false. Otherwise returns the byte
// field at this+0x12f. this = ecx (__fastcall).

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public: char pad[0xf6]; bool suppressMiniMap; };

class CWorld { public:
    char pad[0x12f];
    bool bDisplayMiniMapOnRegionLoad;  // +0x12f
    bool IsDisplayingMiniMapOnRegionLoad();
};

bool CWorld::IsDisplayingMiniMapOnRegionLoad()
{
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().suppressMiniMap)
        return false;
    return bDisplayMiniMapOnRegionLoad;
}