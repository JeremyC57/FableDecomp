// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool) const -- still runnable while the base
// Grade: DIFFER(ours v retail) -- 192v192: register order only -- after the second GetPItem retail loads the creature's collision record into edx first (mov edx,[edi+4] ; mov ecx,[edx+0x18] ; mov edx,[ecx+0x60] ; mov eax,[eax+0x60]), ours the enemy's first (mov edx,[eax+0x60] ; then the creature through eax/ecx) | tried: plain vector temporary (frame copy, double fabs compare), memberwise copy ctor + (float)fabs (this file; all else matches), inline GetRadius accessor (spills the creature radius)
// says so and the enemy is within 2 units vertically and inside (radius sum * 1.2 + 2) in the XY plane.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DC10, length 192 bytes (+ int3 padding). `ret 4`.
// Callees: CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(bool) const @ 0x0093D800, CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50 (PEnemy +0x14, twice), CThing::GetPosition @ 0x004C73D0 (creature first, then enemy);
// the 2.0f @ 0x0122DCDC and 1.2f @ 0x0125A690 constants.
// Shape: the inline C3DVector operator- result is a named local (x/y to the frame, z kept for fabs); the reach is
// computed before the squared XY length and squared on the FPU (`fld st(1) ; fmul st(2)`); `len2 < reach * reach`
// gives `fcompp ; test ah,41h ; jne`. Each thing's collision record (+0x60) carries its radius at +0x34.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290 / 0x006AF2B0 twins, out of line, `ret 4`
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

struct CCollisionRecord {
    unsigned char _pad[0x34];
    float m_radius;                             // +0x34
};

struct CThingCombatant {
    unsigned char _pad[0x60];
    CCollisionRecord* m_pCollision;             // +0x60
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c
    virtual bool IsStillRunnable(bool checkEnemy) const;   // 0x0093D800
};

struct CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose : CAIStateGroup_MeleeCombatNearbyBase {
    virtual bool IsStillRunnable(bool checkEnemy) const;
};

bool CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool checkEnemy) const
{
    if (CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(checkEnemy)) {
        CThingCombatant* creature = (CThingCombatant*)PBrain->PCreature;
        CThingCombatant* enemy = (CThingCombatant*)PEnemy.GetPItem();
        C3DVector delta = enemy->GetPosition() - creature->GetPosition();
        if ((float)fabs(delta.z) < 2.0f) {
            float reach = (((CThingCombatant*)PBrain->PCreature)->m_pCollision->m_radius
                           + ((CThingCombatant*)PEnemy.GetPItem())->m_pCollision->m_radius) * 1.2f + 2.0f;
            if (delta.x * delta.x + delta.y * delta.y < reach * reach)
                return true;
        }
    }
    return false;
}
