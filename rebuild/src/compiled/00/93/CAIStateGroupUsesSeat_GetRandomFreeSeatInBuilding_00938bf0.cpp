// CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding(CThing* building) -- collect the free seats of the
// Leaf: ?GetRandomFreeSeatInBuilding@CAIStateGroup_UsesSeat@@IBEPAVCThingObject@@AAVCThingBuilding@@@Z
// NF72 (round 7, q10863): the PDB spelling (Ego_r.pdb 0x0095BD90 + FableWin.pdb 0x02B78310, engine/CAIStateGroup_UsesSeat.h): protected const,
//   CThingBuilding& in, CThingObject* out. CThingBuilding now comes from engine/CThingBuilding.h; the seat vector holds CThingObject* (FableWin
//   ??0CAddFreeSeatToList@@QAE@AAV?$vector@PAVCThingObject@@...); the world-seed accessor 0x008FCFD0 is CAIStateGroupBase's protected const
//   GetPWorldSeed (Ego_r.pdb ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ), declared on the row's CAIStateGroupBase model.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Grade: RELOCATION_MATCH -- landing retry: out-parameter GetInterface in the if (component lands in eax)
// building's interface-0x21 component into a vector and return one picked with the world's random seed (or null).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938BF0, length 220 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + entry-returning FindEntry, 0x21 = bit 1 of +0x24),
// std::for_each<list iterator, CAddFreeSeat> @ 0x00938E90 (__fastcall, functor returned by hidden pointer; the
// manifest's "_Find_if<...CMiniMapDisplay...>" label is propagated), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// GFROR13 @ 0x00497890 (__fastcall), operator delete @ 0x00BFEA14 (the vector's storage, two paths).
// Shape: the lookup key and the returned functor both live in the dead `building` argument slot; the inline
// random step `seed = ROR13(seed * 0x24a1 + 0x24df)` and `count ? r % count : 0`.
// No engine header for this class; the seat std::list is embedded at +0x8 of the building component (head +0xc).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CThingBuilding.h"
class CThingObject;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
long __fastcall GFROR13(unsigned long);   // 0x00497890

struct CThingVector {                           // std::vector<CThingObject*> (first, last, end of storage)
    CThingObject** m_first;
    CThingObject** m_last;
    CThingObject** m_end;
    CThingVector() : m_first(0), m_last(0), m_end(0) { }
    ~CThingVector() { if (m_first) free(m_first); }
    long size() const { return m_last - m_first; }
};

struct CSeatListNode { CSeatListNode* next; CSeatListNode* prev; };
struct CSeatListIterator {
    CSeatListNode* node;
    CSeatListIterator(CSeatListNode* n) : node(n) { }
};
struct CSeatList {                              // std::list of seats (VC7.1: allocator word, head node, size)
    unsigned char _alloc[4];
    CSeatListNode* head;
    unsigned long size;
    CSeatListIterator begin() const { return CSeatListIterator(head->next); }
    CSeatListIterator end() const { return CSeatListIterator(head); }
};

struct CAddFreeSeat {                           // functor: appends free seats to the vector
    CThingVector* m_pSeats;
    CAddFreeSeat(CThingVector* seats) : m_pSeats(seats) { }
};
CAddFreeSeat __fastcall ForEachSeat(CSeatListIterator first, CSeatListIterator last, CAddFreeSeat f);   // 0x00938E90

struct CTCBuildingSeats {                       // interface 0x21
    unsigned char _pad[0x8];
    CSeatList m_seats;                          // +0x08 (head node at +0x0c)
};

struct CAIStateGroupBaseR : CAIStateGroupBase {
    unsigned long GetRandom(unsigned long range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBaseR {
protected:
    CThingObject* GetRandomFreeSeatInBuilding(CThingBuilding& building) const;
};

CThingObject* CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding(CThingBuilding& building) const
{
    CTCBuildingSeats* comp;
    if (building.GetTC(TCI_THING_OWNER, &comp)) {
        CSeatListNode* head = comp->m_seats.head;
        CThingVector seats;
        ForEachSeat(CSeatListIterator(head->next), CSeatListIterator(head), CAddFreeSeat(&seats));
        if (seats.m_first != seats.m_last) {
            long count = seats.size();
            CThingObject* seat = seats.m_first[GetRandom(count)];
            return seat;
        }
    }
    return 0;
}