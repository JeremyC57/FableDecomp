// CAIStateGroup_GoToPub::IsStaffed() -- true when some living thing in the creature's village building list is
// linked (+0x170) to the pub this group targets (+0x14).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947FB0, length 168 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of +0x24),
// GFGetPVillageInterface @ 0x007EE540, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (list entry x3, the
// thing's +0x170 pointer, this->PPub at +0x14).
// Shape: the village's VillageBuildings std::list (PDB: +0x28, _Myhead at +0x2c) walked with the end node re-read
// every pass; inlined IS_THING_ALIVE, then the linked thing (an inline accessor gives `lea ecx,[eax+0x170]`) is
// compared with the pub pointer read each pass (`mov al,1` hit, shared `xor al,al` miss). Sibling of the
// parked CAIStateGroup_EatAtTavern::IsStaffed (0x0096D2D0), which compares with a virtual GetBuilding instead.
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
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x0040F020
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
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
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
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
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
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
public:
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

struct CThingLinked;
struct CBuildingPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingBuilding>
    CThingLinked* GetPBuilding() const { return (CThingLinked*)GetPItem(); }
};

struct CListNode {
    CListNode* next;                            // +0x00
    CListNode* prev;                            // +0x04
    CBuildingPointer value;                     // +0x08 CIntelligentPointer<CThingBuilding>
};

struct CThingLinked {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer m_linkedThing;      // +0x170
    CThing* GetPLinkedThing() { return m_linkedThing.GetPItem(); }
};

class CTCVillage { public:
    unsigned char _pad[0x28];
    void* m_allocator;                          // +0x28 VillageBuildings (std::list, PDB)
    CListNode* m_head;                          // +0x2c _Myhead
    unsigned long m_size;                       // +0x30 _Mysize
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14 CIntelligentPointer<CThing>
    bool IsStaffed();
};

bool CAIStateGroup_GoToPub::IsStaffed()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (village) {
            for (CListNode* node = village->m_head->next; node != village->m_head; node = node->next) {
                if (IS_THING_ALIVE(node->value)
                    && node->value.GetPBuilding()->GetPLinkedThing() == PPub.GetPItem())
                    return true;
            }
        }
    }
    return false;
}