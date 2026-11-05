// CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed(long deedIndex, EOpinionDeedType type) -- walk the building's occupants; the first live one whose opinion-of-hero component knows of the deed records the deed index and the frame and returns true
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model (landed 0x00584157 idiom): the opinion component loads straight into ecx before the KnowsOfDeed arguments
// Leaf: SeeIfAnyOccupantsKnowAboutDeed
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x007BB150, 143 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined
//          GetInterface for interface 79 = bit 15 of the bitset word at CThing+0x28, tested as `test ch,ch ; jns`);
//          CTCCreatureOpinionOfHero::KnowsOfDeed(long handle, EOpinionDeedType, bool) 0x00681AE0; CWorld::GetWorldFrame()
//          0x0049D870 (static).
// Layout (retail CTCTrespasser, as the landed ctor 0x007BAE00): DeedHandles[3] +0x10, OccupantsKnowOfDeedHandleIndex +0x20,
//         FrameSomeoneWasInformedOfLastDeed +0x24, OccupantsOfCurrentTrespassingThing (list, bare 4-byte head-node pointer)
//         +0x38; list node {next, prev, CIntelligentPointer<CThingAICreature> at +8}.
// Shape: the list end is kept in the `push ecx` slot and re-read after the call; the lookup key lives in the dead
//        deedIndex argument slot (the index itself is kept in ebp).
typedef long EOpinionDeedType;
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCreatureOpinionOfHero { public:
    bool KnowsOfDeed(long deedHandle, EOpinionDeedType type, bool includeReported);   // 0x00681AE0
};
class CWorld { public:
    static long __fastcall GetFrame();       // 0x0049D870
};

struct SOccupantNode {                          // STLport list node
    SOccupantNode* _M_next;
    SOccupantNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;            // CIntelligentPointer<CThingAICreature>
};

#pragma pack(push, 1)
class CTCTrespasser { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad_0x08[0x4];
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];                       // +0x10
    long          CurrentDeedHandleIndex;               // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;       // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;    // +0x24
    unsigned long FramePlayerLeft;                      // +0x28
    bool          AttachedOpinionSource;                // +0x2c
    CIntelligentPointer<CThing> TrespassingThing;           // +0x2d
    unsigned char _pad_0x35[0x3];
    SOccupantNode* OccupantsOfCurrentTrespassingThing;  // +0x38 list head node
    bool SeeIfAnyOccupantsKnowAboutDeed(long deedIndex, EOpinionDeedType type);
};
#pragma pack(pop)

bool CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed(long deedIndex, EOpinionDeedType type)
{
    SOccupantNode* end = OccupantsOfCurrentTrespassingThing;
    for (SOccupantNode* node = end->_M_next; node != end; node = node->_M_next) {
        CThing* occupant = node->_M_data.Get();
        if (occupant && occupant->HasTC(TCI_CREATURE_OPINION_OF_HERO)) {
            CTCCreatureOpinionOfHero* opinion = ((CTCCreatureOpinionOfHero*)&occupant->GetTC(TCI_CREATURE_OPINION_OF_HERO));
            if (opinion->KnowsOfDeed(DeedHandles[deedIndex], type, true)) {
                OccupantsKnowOfDeedHandleIndex = deedIndex;
                FrameSomeoneWasInformedOfLastDeed = CWorld::GetFrame();
                return true;
            }
        }
    }
    return false;
}