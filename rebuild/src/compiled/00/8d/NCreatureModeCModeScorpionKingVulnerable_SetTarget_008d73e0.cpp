// NCreatureMode::CModeScorpionKingVulnerable::SetTarget(CThingCreatureBase& target) -- store the target in PTarget through a temporary CIntelligentPointer<CThingCreatureBase> (converting constructor, then the pointer assignment)
// Leaf: ?SetTarget@CModeScorpionKingVulnerable@NCreatureMode@@QAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D73E0, 71 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb (0x00818760) and FableWin.pdb (0x0284D510) both spell
//   ?SetTarget@CModeScorpionKingVulnerable@NCreatureMode@@QAEXAAVCThingCreatureBase@@@Z; Ego_r.pdb CModeScorpionKingVulnerable
//   has PTarget (CIntelligentPointer<CThingCreatureBase>) at +0x7D (125) -- the only Ego_r class with that pointer type at
//   that offset -- and the landed ctor 0x008D8120 builds PTarget at retail +0x7D. The temporary's vtable 0x0123E7D8 is RTTI
//   .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@. The sole caller is the landed
//   CCreatureAction_ScorpionPrepareTailPlunge::FinishAction 0x008C4990, which news a CModeScorpionKingVulnerable (vtable
//   0x0128582C) and calls this with its target. The catalog label "CVillageTaskCarryCrate::AssignCreatureToTask" is
//   propagated.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90 (twice), CBaseIntelligentPointer::GetPItem() const 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: the 0x007F5810 CTCTargetingBase setter body: `PTarget = &target;` through the PDB's CIntelligentPointer(T*) and
//   operator=(const CIntelligentPointer&) inline; the reference parameter compiles exactly like the pointer one.
// Layout: engine/CModeStandBase.h (flattened vptr, `struct` key: header debt) as the base; the retail members from the landed ctor
//   0x008D8120: RecoveryCount +0x68, Filter +0x6C, PCreature +0x78, bRecovering +0x7C, PTarget +0x7D (byte-aligned
//   intelligent pointer), Obstructions +0x88.
#include "rebuild_abi.h"

class CThing;
class CThingCreatureBase;
class CBaseObject;
class CBaseObjectPointer;
#pragma pack(push, 1)                                       // the pointer classes are byte-aligned (PTarget-style +0x0d / +0x7d members)
class CBaseIntelligentPointer {                             // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* Get() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(pop)


#include "engine/CModeStandBase.h"                        // CModeStandBase (flattened vptr, 0x68 bytes)

namespace NCreatureMode {
class CModeScorpionKingVulnerable : public CModeStandBase { // vtable 0x0128582C, 0xB8 bytes in retail
public:
    CModeScorpionKingVulnerable(CThing& thing);             // 0x008D8120
    void SetTarget(CThingCreatureBase& target);
    long                                    RecoveryCount;  // +0x68
    unsigned char                           Filter[0xc];    // +0x6c CAnimFilterOfGroup
    CThingCreatureBase*                     PCreature;      // +0x78
    bool                                    bRecovering;    // +0x7c
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0x7d
};
} // namespace NCreatureMode

void NCreatureMode::CModeScorpionKingVulnerable::SetTarget(CThingCreatureBase& target)
{
    PTarget = &target;
}