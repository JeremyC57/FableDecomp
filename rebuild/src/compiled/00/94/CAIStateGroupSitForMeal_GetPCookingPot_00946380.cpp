// CAIStateGroup_SitForMeal::GetPCookingPot() -- find the cooking pot among the things of the creature's home
// Leaf: ?GetPCookingPot@CAIStateGroup_SitForMeal@@ABEPAVCThing@@XZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946380, length 116 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the home at CThingAICreature+0x168, then the found
// element), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x21 = bit 1 of the presence word
// at +0x24), std::find_if<list iterator, CIsCookingPotForMeal> @ 0x009465E0 (/Gr algorithm TU: hidden result in ecx,
// both iterators and the 1-byte predicate on the stack, `ret 0Ch`).
//
// Shape: `home != 0 && home->GetInterface(&contents, 0x21)` (the bool lookup is the second `je` to the null
// tail), then find_if over the contents list with a VALUE-INITIALISED empty predicate (`CIsCookingPot()` with
// no user constructor gives the `mov byte ptr [esp+4],0` before the dword push); the key, the predicate and
// the returned iterator all share the single `push ecx` frame slot. A hit returns the element's GetPItem, a
// miss (end() re-read from the list head) or no home returns null.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// The building's contents component (interface 0x21) keeps a VC6-era Dinkumware std::list of
// CIntelligentPointer<CThing> at +0x08 ({allocator byte, head, size}: head at +0x0c, node {next, prev, value}).
struct SThingListNode {
    SThingListNode* next;
    SThingListNode* prev;
    CIntelligentPointer<CThing> value;              // +0x08
};

struct SThingListIterator {                     // 4-byte iterator class: returned through a hidden pointer
    SThingListNode* node;
    SThingListIterator() {}
    SThingListIterator(SThingListNode* n) : node(n) {}
};

struct CThingList {
    unsigned char allocator;                    // +0x00 (empty allocator, padded)
    SThingListNode* head;                       // +0x04
    long size;                                  // +0x08
    SThingListIterator begin() const { return SThingListIterator(head->next); }
    SThingListIterator end() const { return SThingListIterator(head); }
};

struct CTCBuildingContents {                    // interface type 0x21 component
    unsigned char _pad[0x8];
    CThingList Things;                          // +0x08
};

class CThingAICreature : public CThing { public:
    unsigned char _pad2[0x168 - sizeof(CThing)];
    CIntelligentPointer<CThing> PHome;              // +0x168 CIntelligentPointer<CThingBuilding>
    CThing* GetPHome() const { return PHome.Get(); }   // inline accessor: `lea ecx,[eax+168h]`
};

struct CIsCookingPotForMeal {                   // empty predicate with a user constructor (no zero store)
    CIsCookingPotForMeal() {}
};

SThingListIterator __fastcall FindIfCookingPotForMeal(SThingListIterator first, SThingListIterator last,
                                                      CIsCookingPotForMeal pred);   // 0x009465E0

struct CAIBrainSitForMeal {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrainSitForMeal* PBrain;                 // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                 // +0x14
    CIntelligentPointer<CThing> PPot;               // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
private:
    CThing* GetPCookingPot() const;
public:
};

CThing* CAIStateGroup_SitForMeal::GetPCookingPot() const
{
    CThing* home = PBrain->PCreature->GetPHome();
    CTCBuildingContents* contents;
    if (home != 0 && home->GetTC(TCI_THING_OWNER, &contents)) {
        SThingListIterator it = FindIfCookingPotForMeal(contents->Things.begin(), contents->Things.end(),
                                                        CIsCookingPotForMeal());
        if (it.node != contents->Things.end().node)
            return it.node->value.Get();
    }
    return 0;
}