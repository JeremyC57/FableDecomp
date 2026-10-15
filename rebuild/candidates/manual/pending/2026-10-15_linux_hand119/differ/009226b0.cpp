// Grade: DIFFER(156v156) -- scheduling/regalloc only: retail stores the Bind2 first pointer (&type) before the IsForThing constructor call and the 0x14 type after the creature push, with the creature in eax both times; the lane stores the type first, the first pointer after the call, and uses ecx/edx for the creature (creature locals and a member operator&& tried)
// CAIStateGroup_Drunk::IsStillValid() const -- still valid while no type-0x14 message is pending for the creature and its drunkenness component (interface 0xC3) still says drunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009226B0, length 156 bytes (+ int3 padding).
// Callees: the thing filter constructor @ 0x006AF290 (thiscall, CThing*; copies +0x18/+0x1c and returns `this`),
// CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (message manager at +0x60), CMessageEventManager::FindMostRecentMessage
// <CFilter_Bind2<..>> @ 0x008D46D0 (the bound filter by const reference), the interface-map lower bound @ 0x004365B0
// (a second CVectorMap::LowerBound instantiation; interface 0xC3 = bitset word 6 bit 3 -> `test byte ptr [eax+38h],8`).
// Shape: the landed message-filter expression chain (0x00939D70): `IsOfType(0x14) && IsForThing(creature)` -> a
// CFilter_Bind2 of two pointers at frame +4 over the temporaries at +0 and +0xc (the type store scheduled after the
// creature push); the manager is reached after the filter (object expression evaluated last, through an inline
// accessor); the result is kept in edi. The drunkenness lookup's bool is NOT tested (the MazeTeleport 0x009543B0
// form: the miss path reads the dead key slot `[esp+8]`). `msg == 0 && drunk->flag` returns a full-register 0/1
// (`mov eax,1` / `xor eax,eax`) through a bool.
// Model: local (engine/CAIStateGroup_Drunk.h flattens the base; FrameToNextTalkToSelf +0x14 from it).
struct CTCBase;
struct CThing;
struct CMessageEvent;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThingAICreature {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
};

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsForThing {
    long m_handle[2];
    CMessageEventFilter_IsForThing(CThingAICreature* thing);   // 0x006AF290
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsForThing> CDrunkMessageFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CDrunkMessageFilter& filter);   // 0x008D46D0
};

struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CTCDrunk {                               // interface 0xC3
    unsigned char _pad[0xc];
    unsigned char m_bDrunk;                     // +0x0c
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
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
    CWorld* GetPWorld() const;                  // 0x008FCFA0
};

struct CAIStateGroup_Drunk : CAIStateGroupBase {
    long FrameToNextTalkToSelf;                 // +0x14
    virtual bool IsStillValid() const;
};

bool CAIStateGroup_Drunk::IsStillValid() const
{
    CMessageEvent* message = GetPWorld()->GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsForThing(PBrain->Creature));
    CTCDrunk* drunk;
    PBrain->Creature->GetInterface(&drunk, 0xC3);
    return message == 0 && drunk->m_bDrunk;
}
