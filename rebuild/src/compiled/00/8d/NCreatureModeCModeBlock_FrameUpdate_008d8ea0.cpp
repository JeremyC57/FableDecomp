// NCreatureMode::CModeBlock::FrameUpdate() -- while blocking, a creature with a look component that is a registered combatant looks at the live creature it is attacking; then the base frame update
// Leaf: ?FrameUpdate@CModeBlock
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8EA0, 129 bytes (+ int3 padding), `ret`; vtable 0x01267C3C slot 3.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x43 = bit 3 of the bitset dword at +0x28 =
//   CTCLook), CThingCreatureBase::IsRegisteredCombatant @ 0x00662410, CModeBase::GetWorld @ 0x0073A680 (combat manager
//   at +0x4c), CCombatManager::GetCombatantInfo @ 0x006E9360, CThing::GetPos @ 0x004C73D0,
//   CTCLook::LookAtPos(const C3DVector*, long priority) @ 0x00753E90, CModeBase::FrameUpdate @ 0x0073AC50 (qualified).
// Name: FableWin.pdb `?FrameUpdate@CModeBlock@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: CModeBase (retail 0x1c) ParentThing +0x04; CModeBlock PCreature +0x1c (Ego_r.pdb). CCombatantInfo (Ego_r.pdb):
//   Parent +0, AttackerInfo +4 whose first member is Attacked.
// Shape: the out-parameter GetInterface with the ternary entry pick (the landed CModeBlock::OnActivate 0x008D8CE0 model);
//   the component stays in esi across the combat calls; IsRegisteredCombatant and GetCombatantInfo both take PCreature;
//   the combat manager is a named local so GetWorld is called before PCreature is pushed; Attacked is re-read through
//   the info pointer at each use (a named `attacked` local moves it from ecx to eax and adds a `mov ecx,eax`).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

class CThingCreatureBase : public CThing { public:
    bool IsRegisteredCombatant() const;    // 0x00662410
};

class CTCLook { public:                           // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
};

class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;          // +0x00
};
struct CCombatantInfo {
    CThingCreatureBase*     Parent;        // +0x00
    CAttackingCombatantInfo AttackerInfo;  // +0x04
};
class CCombatManager { public:
    CCombatantInfo* GetCombatantInfo(const CThingCreatureBase* thing);   // 0x006E9360
};
class CWorld { public:
    unsigned char   _pad00[0x4c];
    CCombatManager* CombatManager;         // +0x4c
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];       // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CWorld* GetWorld() const;              // 0x0073A680
    void FrameUpdate();                    // 0x0073AC50
};

namespace NCreatureMode {
struct CModeBlock : CModeBase {            // vtable 0x01267C3C
    CThingCreatureBase* PCreature;         // +0x1c
    void FrameUpdate();
};
}

void NCreatureMode::CModeBlock::FrameUpdate()
{
    CTCLook* look;
    if (ParentThing->GetTC(TCI_LOOK, &look)) {
        if (PCreature->IsRegisteredCombatant()) {
            CCombatManager* combatManager = GetWorld()->CombatManager;
            CCombatantInfo* info = combatManager->GetCombatantInfo(PCreature);
            if (info->AttackerInfo.Attacked != 0 && !info->AttackerInfo.Attacked->IsDead())
                look->LookAtPos(&info->AttackerInfo.Attacked->GetPos(), 0x21);
        }
    }
    CModeBase::FrameUpdate();
}