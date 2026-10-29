// CAIStateGroup_MinionFollowThing::Cleanup() -- undo the follow set-up: flag the creature's 0xAA component, remove
// the creature as a follower of the followee (when it was added and the creature is dead or flagged), restore the
// pushable-by-hero state, clear the navigation's ignore-thing, and reset the expression flag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921870, length 286 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface x3: 0xAA = bit 10 of +0x34, 0xBC = CTCFollowed
// bit 28 of +0x34 on the followee, 7 = CTCCreatureNavigation sign bit of byte +0x20), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (x2), CTCFollowed::RemoveFollower(CThing*) @ 0x006D5180, the creature's +0x60 sub-object virtual slot
// 84 (+0x150, a long taking the bool), the navigation's SetThingToIgnore(const CThing*) @ 0x006CBE30 (manifest
// "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated).
// Shape: the 0xAA flag uses the bool-returning out-parameter lookup inside the `if`; the followee and navigation
// lookups are `if (HasTC(n))` + the void out-parameter / entry forms (the bool form there flips this to esi).
// Members are the PDB names (rebuild/include/engine/CAIStateGroup_MinionFollowThing.h; the header flattens the base,
// so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
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
    unsigned long HasTC(long interfaceType) const
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCFollowerState {                       // interface 0xAA
    unsigned char _pad[0x7b];
    bool m_followStopped;                       // +0x7b
};
class CTCFollowed { public:                            // interface 0xBC
    void RemoveFollower(CThing* follower);      // 0x006D5180
};
class CTCCreatureNavigation { public:                  // interface 7
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
};
struct CThingPhysicsSub {                       // the creature's +0x60 sub-object
    V16(a) V16(b) V16(c) V16(d) V16(e) V4(f)
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
    virtual void Cleanup();
};

void CAIStateGroup_MinionFollowThing::Cleanup()
{
    CTCFollowerState* state;
    if (PBrain->PCreature->GetInterface(&state, 0xaa))
        state->m_followStopped = true;

    CThing* creature = PBrain->PCreature;
    if (((creature->m_flags91 & 1) || (creature->m_flags92 & 4)) && FollowerAdded) {
        if (PFollowee.GetPItem() != 0) {
            CThing* followee = PFollowee.GetPItem();
            if (followee->HasTC(0xbc)) {
                CTCFollowed* followed;
                followee->GetInterfaceV(&followed, 0xbc);
                followed->RemoveFollower(PBrain->PCreature);
            }
        }
        FollowerAdded = false;
    }
    if (!AvoidDynamicObstacles)
        ((CThingPhysicsSub*)PBrain->PCreature->m_pSub60)->SetPushableByHero(PreviousPushableByHero);
    CThing* self = PBrain->PCreature;
    if (self->HasTC(7))
        ((CTCCreatureNavigation*)self->FindEntry(7)->second)->SetThingToIgnore(0);
    ExpressionUsed = false;
}