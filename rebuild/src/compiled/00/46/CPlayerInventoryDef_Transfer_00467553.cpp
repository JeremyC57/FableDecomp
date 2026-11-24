// Leaf: ?Transfer@CPlayerInventoryDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
// Name fix (agentNF54): the local CPersistContext stand-in replaced by the shared engine/CPersistContext.h and the opaque 12-byte
//   fields by the Ego_r.pdb members (Slot / MaxInSlot / InitialAmount vector<long>, IsUsable vector<bool>, InitialSelectedSlot long;
//   retail STLport vectors are 12 bytes, vector<bool> a plain byte vector as in the landed 0x00CDCF80): the three vector<long> calls
//   0x00466A47 are the name-dropping release ??$TransferVector@J@CPersistContext@@QAEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z (the
//   landed row's own leaf; was the stand-in ?Transfer_00466a47@CPersistContext@@QAEXPAX@Z), and 0x00467597 (whose callees are
//   GFSerialiseVectorBinaryIn / Out<bool> 0x004675F6 / 0x004675C5) the same template for bool (was ?Transfer_00467597, NAME_FIXES).
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"
class CPlayerInventoryDef { public:
    unsigned char _pad[0x38];
    std::vector<long> Slot;          // 0x3c
    std::vector<long> MaxInSlot;     // 0x48
    std::vector<long> InitialAmount; // 0x54
    std::vector<bool> IsUsable;      // 0x60
    long InitialSelectedSlot;        // 0x6c
    virtual void Transfer(CPersistContext& ctx);
};

void CPlayerInventoryDef::Transfer(CPersistContext& ctx)
{
    ctx.TransferVector(Slot);
    ctx.TransferVector(MaxInSlot);
    ctx.TransferVector(InitialAmount);
    ctx.TransferVector(IsUsable);
    ctx.Transfer(InitialSelectedSlot);
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CPlayerInventoryDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
