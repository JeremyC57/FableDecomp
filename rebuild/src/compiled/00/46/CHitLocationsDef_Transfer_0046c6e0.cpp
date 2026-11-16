// CHitLocationsDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CHitLocationsDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046C6E0, 16 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CSubDefClassBase): TransferVector<long> 0x00466A47.
// Port (round 4, NF40): was a stand-in (leaf ?Transfer@CHitLocationsDef@@QAEXPAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CSubDefClassBase.h. No shared header exists for CHitLocationsDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (HitLocations +0x28; the retail offsets are the Ego_r ones) and the virtual override
//   ?Transfer@CHitLocationsDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CHitLocationsDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<long>          HitLocations;                  // +0x28
};

void CHitLocationsDef::Transfer(CPersistContext& context)
{
    context.TransferVector(HitLocations);
}
