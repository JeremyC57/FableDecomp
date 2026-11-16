// CAttackPatternDef::Transfer(CPersistContext& context) -- persist the attack-pattern def: its one member, the vector<long> AttackPattern (+0x3C)
// Leaf: ?Transfer@CAttackPatternDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046C284, 16 bytes: `add ecx,3Ch ; push ecx ; mov ecx,[esp+8] ; call` the unnamed
//   CPersistContext::TransferVector<long> 0x00466A47 `; ret 4` (virtual slot 18, the shape of the landed 0x004EBF22).
// Port (round 4, NF40): was a pad struct with a stand-in Xfer_00466a47(void*) member on a local CPersistContext (and the
//   non-virtual QAE leaf); now engine/CPersistContext.h and the retail def chain engine/CParentDefClassBase.h. No shared
//   header exists for CAttackPatternDef, so the row keeps its own (pre-existing) model of the class, now with the Ego_r.pdb
//   member AttackPattern (vector<long>, Ego_r +0x40 = retail +0x3C after the 4-byte-shorter retail def base) and the PDB's
//   virtual override ?Transfer@CAttackPatternDef@@UAEXAAVCPersistContext@@@Z (Ego_r / FableWin).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CAttackPatternDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<long> AttackPattern;                       // +0x3C
};

void CAttackPatternDef::Transfer(CPersistContext& context)
{
    context.TransferVector(AttackPattern);
}
