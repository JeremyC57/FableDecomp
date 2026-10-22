// CTCMeleeTrap::InitialiseAnimation(const CCharString& name, bool loop) -- reset the thing's animator and start the appearance's first animation of the given name (type 6, update mode 1, looping as asked, priority 100), then pick up its strike events
// Leaf: ?InitialiseAnimation@CTCMeleeTrap@@AAEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078809B, 253 bytes, `ret 8`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 5Ah ; pop edi`, `leave`), as the landed trap siblings (0x00788555).
// Callees: std::bitset::test 0x00410DE0 (out of line), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020
//   (inlined unchecked lookup of CTCAnimationComplex 0x5a), CTCAnimationComplex::GetAnimation 0x0070B460,
//   CAnimateThing::Reset 0x0070EC30 (name as the landed 0x007502D0), CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2
//   (result ignored), CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&) 0x005DC340
//   (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / 0x0122FFCC), CAnimInit::Set 0x0070C050, the one-argument
//   CAnimateThing::SetMainAnim(const CAnimInit&) 0x0070D920 (name as the landed 0x00732850) and SetMainAnim(const
//   CAnimInit&, long, long) 0x0070D580, CTCMeleeTrap::InitialiseStrikeAnimation 0x00787F29 (landed); the defs' slot 1.
// Layout: CTCBase ParentThing +0x04; CThing bitset +0x20, map +0x44, PBaseDef +0x70; CAppearanceDef Animation +0x34;
//   CAnimInit 0x2c bytes (Frame +0x18).
// Shape: the animator is the UpdateAnimation (0x00788555) PeekTC unchecked lookup feeding a reference; the thing def is
//   `CThingBaseDef* thingDef = ParentThing->GetPBaseDef();` (the by-value CDefPointer converted and released at the `;`,
//   the inc/dec folded) with the release through DecRefToZero(long&) (a plain `--RefCount == 0` loads and compares in a
//   register); CDefPointer members __forceinline (size TU); GetSubDef has a same-TU out-only stand-in; the init is the
//   Set-forwarding CAnimInit ctor (type 6, update mode 1, priority 100).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;

struct CCharString {
    void* PStringData;
};
struct CAnimationEntry {
    long AnimIndex;                                            // +0x00
};
struct CAnimFilterBase {                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                    // 0x00429A2F
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long RefCount;                                             // +0x04
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer() : Object(0) { }
    __forceinline CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer()
    {
        if (Object)
            if (DecRefToZero(Object->RefCount))
                Object->Destroy();
    }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real template instantiation is out of line in retail): it writes the out slot only, so VC7.1 knows
// the def pointer's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CAnimFrameInfo {
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
};
struct CAnimInit {                                             // 0x2c bytes
    long          UpdateMode;                                  // +0x00
    unsigned char Type;                                        // +0x04
    bool          Looping;                                     // +0x05
    long          Anim;                                        // +0x08
    float         Speed;                                       // +0x0c
    float         Alpha;                                       // +0x10
    float         LastAlpha;                                   // +0x14
    CAnimFrameInfo Frame;                                      // +0x18
    unsigned long Flags;                                       // +0x20
    unsigned long TypeFlags;                                   // +0x24
    long          Priority;                                    // +0x28
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
              long priority)
    {
        Set(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
             long priority);                                   // 0x0070C050
};
struct CAnimateThing {
    void Reset();                                              // 0x0070EC30
    void SetMainAnim(const CAnimInit& init);                   // 0x0070D920
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
struct CTCAnimationComplex {                                   // interface 0x5a
    CAnimateThing& GetAnimation();                             // 0x0070B460
};

struct CInterfaceMap {                                         // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
struct CComponentBitset {                                      // std::bitset<274>
    unsigned long m_words[9];
    bool test(long bit);                                       // 0x00410DE0 (std::bitset::test)
};
class CThing {
public:
    unsigned char    _pad00[0x20];
    CComponentBitset UsingComponents;                          // +0x20
    CInterfaceMap    m_interfaces;                             // +0x44
    unsigned char    _pad4c[0x70 - 0x4c];
    CThingBaseDef*   PBaseDef;                                 // +0x70
    CDefPointer<CThingBaseDef> GetPBaseDef() const { return CDefPointer<CThingBaseDef>(PBaseDef); }
    template <class T> __forceinline T* PeekTC(long type)
    {
        T* tc;
        if (UsingComponents.test(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            tc = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
        return tc;
    }
};

struct CTCMeleeTrap {
    void*         __vftable;                                   // +0x00
    CThing*       ParentThing;                                 // +0x04 (CTCBase)
    unsigned char _pad08[0x62 - 0x08];
    void InitialiseStrikeAnimation(const CAnimationEntry& entry);   // 0x00787F29
    void InitialiseAnimation(const CCharString& name, bool loop);
};

void CTCMeleeTrap::InitialiseAnimation(const CCharString& name, bool loop)
{
    CAnimateThing& animator = ParentThing->PeekTC<CTCAnimationComplex>(0x5a)->GetAnimation();
    animator.Reset();
    CThingBaseDef* thingDef = ParentThing->GetPBaseDef();
    CDefPointer<CAppearanceDef> appearance;
    thingDef->GetSubDef(&appearance);
    const CAnimationEntry* entry = appearance->Animation.GetAnimation(name, 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry->AnimIndex, 1, loop, 0, 100);
    animator.SetMainAnim(init);
    animator.SetMainAnim(init, 1, 0);
    InitialiseStrikeAnimation(*entry);
}