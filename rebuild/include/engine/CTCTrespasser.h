#ifndef FABLE_ENGINE_CTCTRESPASSER_H
#define FABLE_ENGINE_CTCTRESPASSER_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCTRESPASSER.
// Class: Ego_r.pdb CTCTrespasser : CTCBase; interface TCI_TRESPASSER (190: PeekTC key 0xBE at 0x004A4955). Virtual: Ego_r.pdb
// ?LeavingRegion@CTCTrespasser@@UAEXXZ, the class's first own virtual = slot 28 (+0x70) after CTCBase's 28 (`call [eax+70h]`, 0x004A4970).
// Members are not modelled; no vtable is emitted (nothing virtual is defined here).
#ifdef FABLE_RETAIL_CTCTRESPASSER
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCTRESPASSER_FRAMEUPDATE
#include "engine/CIntelligentPointer.h"
#include "engine/EOpinionDeedType.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingAICreature;
#endif
class CTCTrespasser : public CTCBase {
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
public: void ForceTrespass();
#endif

public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_TRESPASSER; }
    virtual void LeavingRegion();                                        // slot 28 (+0x70)
#ifdef FABLE_RETAIL_CTCTRESPASSER_FRAMEUPDATE
    virtual void FrameUpdate();
private:
    EOpinionDeedType GetDeedType(long index) const;
    bool SeeIfAnyOccupantsKnowAboutDeed(long index, EOpinionDeedType deed_type);
    long InsideBuildingNotTrespassingDeedHandle;
    long DeedHandles[3];
    long CurrentDeedHandleIndex;
    long OccupantsKnowOfDeedHandleIndex;
    unsigned long FrameSomeoneWasInformedOfLastDeed;
    unsigned long FramePlayerLeft;
    bool AttachedOpinionSource;
    CIntelligentPointer<CThing> TrespassingThing;
    std::list<CIntelligentPointer<CThingAICreature> > OccupantsOfCurrentTrespassingThing;
#endif
};
#endif
#endif // FABLE_ENGINE_CTCTRESPASSER_H
