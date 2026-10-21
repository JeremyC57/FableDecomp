// CAIStateGroup_MinionSleep::IsStillValid() const -- sleeping stays valid while the creature's village (via its villager component) has no type-4 message originated by the creature, the creature is (in sleep mode 0xd) still within reach of the bed, its mode manager is in neither mode 6 nor 1, and the bed is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C5A0, length 404 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x23 = CTCVillager, bit 3 of +0x24; 0x31 =
// CTCCreatureModeManager, bit 17 of +0x24), its const twin @ 0x004365B0 (interface 0x42, bit 2 of +0x28, reached through
// a const thing), GFGetPVillageInterface @ 0x007EE540 (__fastcall; manifest "IS_THING_ALIVE<..>" is propagated),
// CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF290, the brain's thing-world accessor @ 0x008FCFA0 (message
// manager at +0x60), CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x008D46D0,
// CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (mode's virtual type getter, slot 1), CThing::GetPosition @ 0x004C73D0,
// the 0x42 component's IsModeActive copy @ 0x00811340, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, inlined IS_THING_ALIVE).
// Shape: every miss path reads the never-written out slot, which is also the lookup key slot.
// Layout: engine/CAIStateGroup_MinionSleep.h flattens the base (pads only); Ego_r.pdb puts CAIStateGroup_GoToBed (0x60 bytes,
// PBed +0x14, BedPos +0x3c) under it, so the class is modelled locally with those PDB members.
#include <math.h>
struct CTCBase;
struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0 (byte twin, the const instantiation)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> COriginFilter;

struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const COriginFilter& filter);   // 0x008D46D0
};

struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CTCVillager;
struct CTCVillage;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CMode {
    virtual void v00();
    virtual long GetModeType();                 // slot 1
};
struct CTCCreatureModeManager {                 // interface 0x31
    CMode* GetCurrentMode();                    // 0x0073A6E0
};
struct CTCModeManager42 {                       // interface 0x42
    bool IsModeActive(unsigned long type) const;    // 0x00811340
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPThingWorld() const;             // 0x008FCFA0
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;               // +0x14
    unsigned char _pad_0x1c[0x20];
    C3DVector BedPos;                           // +0x3c
    unsigned char _pad_0x48[0x18];
};

struct CAIStateGroup_MinionSleep : CAIStateGroup_GoToBed {
    bool WasCarryingWeapon;                     // +0x60
    virtual bool IsStillValid() const;
};

#pragma auto_inline(off)
long TmplSink(const void* self, long key);    // stand-in: the binary search itself (outside the graded bytes)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    return (SEntry*)TmplSink(this, *type);
}
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    return (SEntry*)TmplSink(this, *type + 0);
}
// Same-TU stand-in for the FindMostRecentMessage instantiation (a template member compiled in the caller's TU): it only
// reads the bound filter through its const reference, so VC7.1 stores the Bind2 first pointer before the originator ctor.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const COriginFilter& filter)
{
    return (CMessageEvent*)TmplSink(this, *(const long*)&filter);
}
#pragma auto_inline(on)

bool CAIStateGroup_MinionSleep::IsStillValid() const
{
    CTCVillager* villager;
    PBrain->PCreature->GetInterface(&villager, 0x23);
    if (GFGetPVillageInterface(villager) != 0) {
        CThing* creature = PBrain->PCreature;
        if (GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
                CMessageEventFilter_IsOfType(4) && CMessageEventFilter_IsOriginatedByThing(creature)) != 0)
            return false;
    }
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetModeType() == 0xd) {
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        float dx = BedPos.X - pos.X;
        float dy = BedPos.Y - pos.Y;
        if ((float)sqrt(dy * dy + dx * dx) > 0.1f)
            return false;
    }
    const CThing* creature = PBrain->PCreature;
    CTCModeManager42* manager;
    creature->GetInterface(&manager, 0x42);
    if (manager->IsModeActive(6) || manager->IsModeActive(1))
        return false;
    if (IS_THING_ALIVE(PBed))
        return true;
    return false;
}
