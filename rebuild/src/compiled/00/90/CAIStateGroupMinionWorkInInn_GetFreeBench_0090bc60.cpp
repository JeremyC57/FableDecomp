// CAIStateGroup_MinionWorkInInn::GetFreeBench() -- a random free bench among the objects owned by the creature's workplace (CTCThingOwner, interface 0x21), or null
// Leaf: ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090BC60, 211 bytes (224 with padding), __fastcall, returns CThing*.
// Identity: Ego_r.pdb ?GetFreeBench@CAIStateGroup_MinionWorkInInn@@AAEPAVCThing@@XZ, between IsRunnable (0x0090BBF0) and
//   CleanBench (0x0090BD40, its only caller) as in the PDB. The catalog label "CleanBench_PickRandomMarker" is an autoname.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (the inline CThingAICreature::GetWorkBuilding, +0x170),
//   CVectorMap::LowerBound const 0x004365B0 (same-TU noinline body, engine/CVectorMap.h), std::for_each<list<CIntelligentPointer
//   <CThing> >::const_iterator, CAddFreeBenchToList> 0x0090C100 (/Gr: the functor result through the hidden pointer in ecx),
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFRandom(range, seed) 0x004CDAE0 (__fastcall), free 0x00BFEA14.
// Shape (chunk 892): the owner component comes out of CThing::PeekTC(TCI_THING_OWNER, &owner) (FableWin.pdb's GetTC(id, T**)
//   template shape) inside the if; with the component stored through the out pointer the vector's zero stores no longer order
//   the list-head load, so retail's `mov eax,[eax+0Ch] ; lea edx,[esp+0Ch] ; push edx ; <3 zero stores> ; mov ecx,[eax]`
//   (0x0090BCB2..0x0090BCD2) follows. The bench vector is block-scoped, so its destructor (free of _M_start) is duplicated
//   into both returns of the block.
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCThingOwner.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

unsigned long FABLE_FASTCALL GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

class CAddFreeBenchToList {                             // FableWin.pdb ??0CAddFreeBenchToList@@QAE@AAV?$vector@PAVCThing@@..@std@@@Z
public:
    CAddFreeBenchToList(std::vector<CThing*>& list) : BenchList(&list) { }
    void operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x0090C060 (landed)
private:
    std::vector<CThing*>* BenchList;                    // +0x00
};

class CAIStateGroup_MinionWorkInInn : public CAIStateGroupBase {   // no retail-branch shared header
private:
    CThing* GetFreeBench();
};

CThing* CAIStateGroup_MinionWorkInInn::GetFreeBench()
{
    CThing* workplace = GetCreature().GetWorkBuilding();
    const CTCThingOwner* owner;
    if (workplace && workplace->PeekTC(TCI_THING_OWNER, &owner)) {
        std::vector<CThing*> benches;
        std::for_each(owner->GetObjectList().begin(), owner->GetObjectList().end(), CAddFreeBenchToList(benches));
        if (!benches.empty())
            return benches[GFRandom(benches.size(), GetPWorldSeed())];
    }
    return 0;
}