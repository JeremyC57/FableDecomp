// CFactionDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CFactionDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004717A1, 33 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CParentDefClassBase): TransferVector<CCharString> 0x004331F9 x2.
// Port (round 4, NF40): was a pad-struct stand-in (no Leaf line) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CParentDefClassBase.h. No shared header exists for CFactionDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (EnemyFactions +0x3C, AlliedFactions +0x48; the retail offsets are the Ego_r ones minus 4 after the 0x39-byte retail CParentDefClassBase) and the virtual override
//   ?Transfer@CFactionDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"

class CFactionDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<CCharString>   EnemyFactions;                 // +0x3C
    std::vector<CCharString>   AlliedFactions;                // +0x48
};

void CFactionDef::Transfer(CPersistContext& context)
{
    context.TransferVector(EnemyFactions);
    context.TransferVector(AlliedFactions);
}
