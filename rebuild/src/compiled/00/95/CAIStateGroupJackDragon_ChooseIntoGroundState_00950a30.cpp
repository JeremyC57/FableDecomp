// CAIStateGroup_JackDragon::ChooseIntoGroundState() -- from boss phase 2 on, dive into the ground (state 5) with
// the dragon def's percentage chance, else no state change (0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00950A30, length 239 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef @ 0x008E3300 on the creature's def (+0x70), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface, 0xad = bit 13 of +0x34), CTCBoss::GetBossPhase @ 0x007AED10, the brain world accessor
// @ 0x008FCFC0, CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890, the def's virtual slot 1 (+4).
// Shape: a zeroed CDefPointer local filled by GetSubDef first; the boss lookup used unconditionally; the inlined
// world LCG `% 100` converted unsigned -> float and compared with the def's +0x48 chance; the def pointer is
// kept in edi and released on both returns -- VC drops the destructor's null test on the path that has already
// dereferenced it.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
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
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
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

// ---- CDefClassBase::GetSubDef<CJackDragonDef> (0x008E3300, landed body) compiled in this TU ----
#include "fable_string.h"

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void Release()                         // CDefPointer<T>: destructor and assignment
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()                // CDefPointer<CDefBase>: the manager's temporary
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointerBase {                   // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    ~CDefPointerBase()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CSubDef : CDefBase {                 // T = CJackDragonDef
    unsigned char _pad08[0x40];
    float ChanceIntoGround;                // +0x48 percent
    unsigned char _pad4c[0x18];
    float HoverRange;                      // +0x64
};

struct CDefPointer {                       // CDefPointer<T>
    CSubDef* m_p;
    CDefPointer() { }
    CDefPointer(const CDefPointerBase& other) : m_p((CSubDef*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

class CDefinitionManager { public:
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

class CDefClassBase { public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* m_pManager;        // +0x1C
    bool GetSubDef(CDefPointer& out);          // 0x008E3300 GetSubDef<CJackDragonDef> (same-TU template body)
};

#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CJackDragonDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CThingAICreatureDef {
    unsigned char _pad[0x70];
    CDefClassBase* PDef;                        // +0x70 CDefPointer<const CDefClassBase>::Object
};

struct CWorldFrame;
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PLandedMarker;      // +0x1c CIntelligentPointer<CThing>
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    CBaseIntelligentPointer PStrafeMarker1;     // +0x34
    CBaseIntelligentPointer PStrafeMarker2;     // +0x3c
    CBaseIntelligentPointer PInvisibleBodgeCreature;  // +0x44
    long FrameOfLastSlam;                       // +0x4c
    long FrameWhenLastInRange;                  // +0x50
    long FrameWhenLanded;                       // +0x54
    long FramesInSnapRange;                     // +0x58
    long CurrentState;                          // +0x5c EDragonState
    long GoLeft;                                // +0x60
    long ChooseIntoGroundState();
};

long __fastcall GFROR13(unsigned long);      // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

class CTCBoss { public:                                // interface 0xad
    long GetBossPhase();                        // 0x007AED10
};

struct CWorldSeed {
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

long CAIStateGroup_JackDragon::ChooseIntoGroundState()
{
    CDefPointer def;
    def.m_p = 0;
    ((CThingAICreatureDef*)PBrain->PCreature)->PDef->GetSubDef(def);
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    if (boss->GetBossPhase() >= 2) {
        if ((float)WorldRandomRange(*((CWorldSeed*)&GetWorld())->GetPRandomSeed(), 100) < def.m_p->ChanceIntoGround)
            return 5;
    }
    return 0;
}