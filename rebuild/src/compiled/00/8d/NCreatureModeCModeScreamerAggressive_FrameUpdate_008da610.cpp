// NCreatureMode::CModeScreamerAggressive::FrameUpdate() -- while its target is alive, a screamer with a look component looks at the target; then the base frame update
// Leaf: ?FrameUpdate@CModeScreamerAggressive
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA610, 121 bytes (+ int3 padding), `ret`; vtable slot 3.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type
//   0x43 = bit 3 of the bitset dword at +0x28 = CTCLook), CThing::GetPos @ 0x004C73D0,
//   CTCLook::LookAtPos(const C3DVector*, long priority) @ 0x00753E90, CModeBase::FrameUpdate @ 0x0073AC50 (qualified).
// Name: FableWin.pdb `?FrameUpdate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ` (slot 3).
// Layout: Ego_r.pdb CModeScreamerAggressive: CModeStandBase 0x68, Filter +0x68, PEffect +0x74, PTarget +0x7c.
// Shape: IS_ALIVE (GetPItem twice, dead bit +0x91) then the out-parameter GetInterface with the ternary entry pick (the
//   CModeBlock::FrameUpdate 0x008D8EA0 model); the component stays in esi across the GetPItem / GetPos calls.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

class CTCLook { public:                           // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
struct CModeScreamerAggressive : CModeStandBase {
    unsigned char           Filter[0x0c];  // +0x68 CAnimFilterOfGroup
    CIntelligentPointer<CThing> PEffect;       // +0x74
    CIntelligentPointer<CThing> PTarget;       // +0x7c
    void FrameUpdate();
};
}

void NCreatureMode::CModeScreamerAggressive::FrameUpdate()
{
    CTCLook* look;
    if (PTarget.Get() != 0 && !PTarget.Get()->IsDead() && ParentThing->GetTC(TCI_LOOK, &look))
        look->LookAtPos(&PTarget.Get()->GetPos(), 0x67);
    CModeBase::FrameUpdate();
}