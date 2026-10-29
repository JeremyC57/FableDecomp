// CAIStateGroup_MinionFollowThing::OnActivate() -- start following: clear the creature's follow-stopped flag, join the followee's follower list (and, with an expression, its 0x7a list when valid), unblock movement, and make the creature unpushable by the hero (remembering the old state)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009212C0, length 384 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xAA bit 10 of +0x34, 0xBC = CTCFollowed bit 28 of
//          +0x34 on the followee, 0x7a bit 26 of +0x2c on the followee), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
//          (PFollowee +0x14), CTCFollowed::IsCreatureValidFollower @ 0x006D4E60 / AddFollower @ 0x006D50C0, the 0x7a
//          component's AddThing @ 0x006AEDC0, IsAbleToMove / RemoveMovementBlockingModes @ 0x0085F3E0 / 0x0085F600
//          (__fastcall), the creature's +0x60 sub-object virtual slots 83/84 (+0x14c IsPushableByHero, +0x150 Set).
// Shape: the model and lookup forms of the landed Cleanup 0x00921870 (same class, same TU); the add-to-front flag is a
//          frame bool passed by address and then by value; the valid-follower result is kept in bl.
// Layout: PDB names from engine/CAIStateGroup_MinionFollowThing.h (the header flattens the base and this class has a
//          virtual OnActivate, so the class is modelled locally like the landed Cleanup).
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned char _pad3[0x1c9 - 0x93];
    unsigned char m_flags1c9;                   // +0x1c9 bit 1 (creature state, descriptive)
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};


class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CTCFollowerState {                       // interface 0xAA
    unsigned char _pad[0x7b];
    bool m_followStopped;                       // +0x7b
};
class CTCFollowed { public:                            // interface 0xBC
    bool IsCreatureValidFollower(CThing* creature, bool* addToFront);   // 0x006D4E60
    void AddFollower(CThing* follower, bool addToFront);               // 0x006D50C0
};
struct CTCFollowerList {                        // interface 0x7a (descriptive)
    void AddThing(CThing* thing);               // 0x006AEDC0 (manifest "InsertIntelligentPointerInList" is propagated)
};
struct CThingPhysicsSub {                       // the creature's +0x60 sub-object
    V16(a) V16(b) V16(c) V16(d) V16(e) virtual void f0(); virtual void f1(); virtual void f2();
    virtual bool IsPushableByHero();            // slot 83 (+0x14c)
    virtual void SetPushableByHero(bool pushable);   // slot 84 (+0x150)
};

struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    CBaseIntelligentPointer PFollowee;          // +0x14 CIntelligentPointer<CThingCreatureBase>
    float FollowDistance;                       // +0x1c
    bool AvoidDynamicObstacles;                 // +0x20
    bool PreviousPushableByHero;                // +0x21
    bool ExpressionUsed;                        // +0x22
    bool FollowerAdded;                         // +0x23
    long CurrentMovementType;                   // +0x24
    void OnActivate();
};
bool __fastcall IsAbleToMove(CThing* thing);                  // 0x0085F3E0
void __fastcall RemoveMovementBlockingModes(CThing* thing);   // 0x0085F600

void CAIStateGroup_MinionFollowThing::OnActivate()
{
    CTCFollowerState* state;
    if (GetCreature()->GetInterface(&state, 0xaa))
        state->m_followStopped = false;
    if (((CThing*)PFollowee.GetPItem()) != 0) {
        CThing* followee = ((CThing*)PFollowee.GetPItem());
        if (followee->HasTC(0xbc)) {
            CTCFollowed* followed;
            followee->GetInterfaceV(&followed, 0xbc);
            {
                bool addToFront = false;
                bool valid = followed->IsCreatureValidFollower(GetCreature(), &addToFront);
                followed->AddFollower(GetCreature(), addToFront);
                if (ExpressionUsed && valid && !(GetCreature()->m_flags1c9 & 2)) {
                    CTCFollowerList* list;
                    if (((CThing*)PFollowee.GetPItem())->GetInterface(&list, 0x7a))
                        list->AddThing(GetCreature());
                }
            }
            FollowerAdded = true;
        }
    }
    if (!IsAbleToMove(GetCreature()))
        RemoveMovementBlockingModes(GetCreature());
    if (!AvoidDynamicObstacles) {
        PreviousPushableByHero = ((CThingPhysicsSub*)GetCreature()->m_pSub60)->IsPushableByHero();
        ((CThingPhysicsSub*)GetCreature()->m_pSub60)->SetPushableByHero(false);
    }
}