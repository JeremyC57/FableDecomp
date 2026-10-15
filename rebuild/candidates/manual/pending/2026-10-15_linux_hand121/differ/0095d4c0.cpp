// CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::list<CThing*>& out) -- collect the things in
// Grade: DIFFER(130v130) -- the first empty one-byte filter is homed in the dead argument slot (`lea ecx,[esp+34h]`) where retail keeps both filters in the frame dword at +4 (`lea ecx,[esp+6]` / `lea edx,[esp+7]`); every other instruction (box build, ToIntBox, search call) is identical.
// the 60x60 area around the creature that pass the two bound filters.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D4C0, length 130 bytes, `ret 4`.
// Callees: CThing::GetPosition @ 0x004C73D0 on PBrain->PCreature, C2DBoxF::ToIntBox @ 0x00A55A30 (C2DBoxI returned
// through a hidden pointer, `ret 4`; manifest "CEditWorldMap::GetPenArea" is propagated), CAIStateGroupBase::
// PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI&, const filter&,
// list&) @ 0x0095DB90. Global: the float 30.0f @ 0x01295318 (the half-size, one memory constant for all four uses).
// Shape: the position's X and Y are loaded once each (named float locals) and the C2DBoxF built in the frame in
// member order (x - 30, y - 30, x + 30, y + 30: `fsub`, `fsubr`, `fxch ; fadd`, `fadd`); the filter is the chain
// of two EMPTY one-byte filters (`lea ecx,[esp+6]` / `lea edx,[esp+7]`, pointers at +8/+0xc: the landed message-
// filter Bind2 model) passed by const reference; the arguments go right to left (list, filter, then the box
// conversion whose hidden slot sits at +0x20). Layout local (the engine header flattens the base).
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
    virtual void InitStateGroup();              // slot 13 (+0x34)
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

struct C3DVector { float X, Y, Z; };
struct CThingPos {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct C2DBoxI { long X0, Y0, X1, Y1; };

struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
    C2DBoxI ToIntBox() const;                   // 0x00A55A30
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

struct CThingFilter_IsFirePoint {               // empty filters: user constructors, no stores
    CThingFilter_IsFirePoint() { }
};
struct CThingFilter_IsNotSelf {
    CThingFilter_IsNotSelf() { }
    // member form: the left operand is complete before the right one is built
    template<class B> CFilter_Bind2<CThingFilter_IsNotSelf, B> operator&&(const B& b) const
    {
        return CFilter_Bind2<CThingFilter_IsNotSelf, B>(*this, b);
    }
};

typedef CFilter_Bind2<CThingFilter_IsNotSelf, CThingFilter_IsFirePoint> CFirePointFilter;

struct CThingList;                              // std::list<CThing*>
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CFirePointFilter& filter, CThingList& out);   // 0x0095DB90
};

const float FIRE_POINT_SEARCH_HALF_SIZE = 30.0f;    // 0x01295318

struct CAIStateGroupBaseS : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBaseS {
    void GetFirePointList(CThingList& out);
};

void CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(CThingList& out)
{
    const C3DVector& pos = ((CThingPos*)PBrain->PCreature)->GetPosition();
    float x = pos.X;
    float y = pos.Y;
    C2DBoxF area(x - FIRE_POINT_SEARCH_HALF_SIZE, y - FIRE_POINT_SEARCH_HALF_SIZE,
                 x + FIRE_POINT_SEARCH_HALF_SIZE, y + FIRE_POINT_SEARCH_HALF_SIZE);
    PeekThingSearchTools()->GetAllThingsInRoughArea(area.ToIntBox(), CThingFilter_IsNotSelf() && CThingFilter_IsFirePoint(), out);
}
