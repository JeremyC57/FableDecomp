// CAIStateGroup_HoverHornetQueen::KillBossHelpers() -- gets the creature's boss component (interface type 0xAD) and calls its KillHelpers
// Leaf: KillBossHelpers
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x0095AA00, 81 bytes. Only caller: CAIStateGroup_HoverHornetQueen::OnReplace
//         (0x0095AB30, vtable 0x0128D484 slot 10) -- a direct `call` with `this` first thing.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface),
//          CTCBoss::KillHelpers 0x007AF490 (manifest label, also called at 0x0097561B).
// Label: the manifest's "CTCTavernTable::FreeServingPoint" is propagated over the byte-identical
// GetInterface shape (as the landed CTCTavernTable::FreePoint 0x007B9090); here `this`+4 is the
// state group's PBrain and brain+0x18 its Creature (engine/CAIBrain.h), so it is a hornet-queen
// state-group helper; the method name is descriptive.
// Shape: the landed out-parameter GetInterface template (0x007B9090): type 0xAD tests bit 13 of the
// presence word at +0x34 (`test ch,20h`), the key lives in the `push ecx` slot, the map pointer is a
// named local (esi); on a miss the never-written component local is read from that same slot
// (`mov eax,[esp]`) -- the callee is called on it anyway, so both arms end in the call.
// `this` is modelled through engine/CAIStateGroupBase.h (PBrain +0x04): the flat
// engine/CAIStateGroup_HoverHornetQueen.h hides PBrain in `_pad_0x04`.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { public:
};

class CTCBoss { public:
    void KillHelpers();                         // 0x007AF490
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    void KillBossHelpers();
};

void CAIStateGroup_HoverHornetQueen::KillBossHelpers()
{
    CTCBoss* boss;
    PBrain->Creature->GetTC(TCI_BOSS, &boss);
    boss->KillHelpers();
}