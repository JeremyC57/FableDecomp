// CAIStateGroup_MinionSitAtInn::GetSeat(CThing* owner) -- ask the owner building's tavern component for a free seat
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// for the creature and keep it in PSeat.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FB60, length 140 bytes (+ int3 padding), `ret 4`. Vtable slot 18.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookup on the ARGUMENT, type 0xAB = CTCTavern, bit 11 of the
// bitset dword at +0x34: `test dh,8`), CTCTavern::GetFreeSeat(CThingAICreature*) @ 0x007BABE0 (`ret 4`, called
// on the possibly-null tavern pointer), then the converting CIntelligentPointer temporary (vtable 0x0125A858 =
// CIntelligentPointer<CThingObject>): CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem
// @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the tavern is a local INITIALISED to null (`xor ecx,ecx`) and set from the pointer-returning lookup only
// when the bitset says so (HasTC + FindTC, the component lands straight in ecx as `this` of GetFreeSeat); then
// `PSeat = seat` through the temporary in the 8-byte frame. PSeat is +0x14 (CAIStateGroup_UsesSeat's pointer;
// the engine header flattens the base, so the layout is local).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CBaseIntelligentPointerCtor {            // the full CBaseIntelligentPointer interface (converting temporaries)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment (the landed
// 0x00902045 shape): base constructor, the derived vtable store, SetPItem on the temporary, GetPItem from it,
// SetPItem on the member, destructor.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCTavern { public:                              // interface type 0xAB
    CThing* GetFreeSeat(CThing* creature);      // 0x007BABE0
};

struct CThingF : CThing {
    // the pointer-returning lookup
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};

struct CAIStateGroup_MinionSitAtInn : CAIStateGroupBase {
    CIntelligentPointerThing PSeat;             // +0x14 CIntelligentPointer<CThingObject>
    virtual void GetSeat(CThing* owner);
};

void CAIStateGroup_MinionSitAtInn::GetSeat(CThing* owner)
{
    CTCTavern* tavern = 0;
    if (owner->HasTC(0xab))
        tavern = ((CThingF*)owner)->FindTC<CTCTavern>(0xab);
    CThing* seat = tavern->GetFreeSeat(PBrain->PCreature);
    PSeat = seat;
}