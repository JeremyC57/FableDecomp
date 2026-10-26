#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingDeadCreature::SetupDeadAnimation() -- start the corpse's "STANDARD_DEAD" animation: a random one passing the creature's animation filter (else of any group) from its appearance def, played as the main animation and immediately finished, then a zero-time frame update, a frame-updated mark and a graphic appearance update
// Leaf: ?SetupDeadAnimation@CThingDeadCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835E30, length 392 bytes, plain `ret`.
//   Name from Ego_r.pdb `?SetupDeadAnimation@CThingDeadCreature@@IAEXXZ`.
// Callees: LowerBound @ 0x0040F020 (component 0x5A CTCAnimationComplex, read uninitialised when absent, as the landed
//   CTCBed GetAnimationComplex model), CTCAnimationComplex::GetAnimation @ 0x0070B460, CDefClassBase::GetSubDef
//   <CAppearanceDef> @ 0x0042B0A2, the animation set's has-animation test @ 0x005DC2E0 and random pick @ 0x005DCC00,
//   CThing::GetRandomSeed @ 0x004C7AE0, the animate thing's reset @ 0x0070EC30 (name unproven),
//   CAnimationEntry::GetAnimateThingFlags @ 0x005DC390, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim
//   @ 0x0070D580, CAnimateThing::SetAsFinished @ 0x0070BE80, CAnimateThing::UpdateFrameLogic(const CAnimFrameTime&)
//   @ 0x0070E8B0, CThing::SetAsFrameUpdated @ 0x004C7C40, CThing::UpdateGraphicAppearance(long) @ 0x004C7590, the def's
//   virtual slot 1 on the last release. The name is the static CCharString @ 0x013BAF40 "STANDARD_DEAD".
// Identity: CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout (retail): CThing bitset +0x20 (0x5A = bit 26 of +0x28), map +0x44, PBaseDef +0x70; CThingDeadCreature
//   PAnimFilter +0xd0 (Ego_r.pdb +0xe8, CThingPhysical 0x18 lower; CCountedPointer object word); CAppearanceDef
//   Animation +0x34; CAnimInit 0x2c bytes (the landed CTCBed model).
// Shape: the filter is reached through the counted pointer's inline operator* (a raw pointer member rotates eax/edx);
//   the zero frame time is three floats (immediate stores after the push); the entry's index is read before the reset.
class CTCBase;
class CCharString { public: void* PStringData; };
extern CCharString g_CCharString_013baf40;              // "STANDARD_DEAD"

class CAnimationEntry { public:
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
class CAnimFilterBase { public:                                // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {           // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet { public:
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;     // 0x005DC2E0
    CAnimationEntry* GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter,
                                        long exclude) const;                             // 0x005DCC00
};

class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef();                          // slot 1 (+4)
    long m_refCount;                                    // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer() { if (m_pObject) m_pObject->ReleaseDef(); }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                            // +0x34
};
class CDefClassBase { public:
    CAppearanceDef* FindAppearanceDef();                // stand-in callee of the stand-in body below
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);   // 0x0042B0A2
};
// Same-TU stand-in for GetSubDef<CAppearanceDef> (the landed 0x00835BD0 idiom): it only writes the out slot, so VC7.1
// knows the def pointer does not escape and reloads it from the slot after a clobber.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    ++def->m_refCount;
    out.m_pObject = def;
    return true;
}
#pragma auto_inline(on)

struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                                      // 0x2c bytes
    long          Mode;                                 // +0x00
    unsigned char AnimType;                             // +0x04
    bool          Flag05;                               // +0x05
    long          AnimIndex;                            // +0x08
    float         Scales[3];                            // +0x0c
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;                                // +0x20
    long          Arg24;                                // +0x24
    long          Percent;                              // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimFrameTime {
    float Frame;
    float SubFrame;
    float Extra;
    CAnimFrameTime() : Frame(0.0f), SubFrame(0.0f), Extra(0.0f) { }
};
class CAnimateThing { public:
    void SetAsFinished();                               // 0x0070BE80
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameTime& time);  // 0x0070E8B0
    void Reset();                                       // 0x0070EC30
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                            // +0x70
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    unsigned long* GetRandomSeed();                     // 0x004C7AE0
    void SetAsFrameUpdated();                           // 0x004C7C40
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    __forceinline CTCAnimationComplex* GetAnimationComplex()
    {
        CTCAnimationComplex* complex;
        if (HasTC(0x5a))
            complex = GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
};

template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    T& operator*() const { return *m_pObject; }
};
class CThingDeadCreature : public CThing { public:
    unsigned char    _pad74[0xd0 - 0x74];
    CCountedPointer<CAnimFilterBase> PAnimFilter;       // +0xd0
    void SetupDeadAnimation();
};

void CThingDeadCreature::SetupDeadAnimation()
{
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    const CAnimationSet& animations = def.m_pObject->Animation;
    const CAnimationEntry* entry;
    if (animations.HasAnimation(g_CCharString_013baf40, *PAnimFilter))
        entry = animations.GetRandomAnimation(g_CCharString_013baf40, GetRandomSeed(), *PAnimFilter, -1);
    else if (animations.HasAnimation(g_CCharString_013baf40, CAnimFilterNoGroup()))
        entry = animations.GetRandomAnimation(g_CCharString_013baf40, GetRandomSeed(), CAnimFilterNoGroup(), -1);
    else
        entry = 0;
    if (entry) {
        long animIndex = entry->AnimIndex;
        animate.Reset();
        CAnimInit init(2, 0, animIndex, 1, true, entry->GetAnimateThingFlags(), 100);
        animate.SetMainAnim(init, 5, 0);
        animate.SetAsFinished();
    }
    animate.UpdateFrameLogic(CAnimFrameTime());
    SetAsFrameUpdated();
    UpdateGraphicAppearance(2);
}


CCharString g_CCharString_013baf40;
static unsigned long g_flags = 0x44;
unsigned long CAnimationEntry::GetAnimateThingFlags() const { return g_flags; }
bool CAnimFilterBase::Accept(const CAnimationEntry* entry) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry* entry) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static bool g_modeHas = false, g_anyHas = false; static CAnimationEntry g_entries[2] = { {7}, {9} };
static unsigned long g_seed = 1; static const unsigned long* g_seedSeen = 0; static long g_exclude = 0;
bool CAnimationSet::HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const
{ return &name == &g_CCharString_013baf40 && (filter.Accept(0) ? g_anyHas : g_modeHas); }
CAnimationEntry* CAnimationSet::GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter, long exclude) const
{ g_seedSeen = seed; g_exclude = exclude; return &g_entries[filter.Accept(0) ? 1 : 0]; }
static int g_destroyed = 0;
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() { ++g_destroyed; }
static CAppearanceDef g_app;
CAppearanceDef* CDefClassBase::FindAppearanceDef() { return &g_app; }
static double g_initBuf[sizeof(CAnimInit) / 8 + 1]; static CAnimInit& g_lastInit = *(CAnimInit*)g_initBuf; static int g_sets = 0, g_finished = 0, g_resets = 0, g_frameUpdates = 0, g_marked = 0; static long g_reason = 0;
static long g_mode = 0, g_delay = 0; static float g_time = -1.0f;
void CAnimInit::Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
{ AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent; }
void CAnimateThing::SetAsFinished() { ++g_finished; }
void CAnimateThing::SetMainAnim(const CAnimInit& init, long mode, long delay) { ++g_sets; std::memcpy(g_initBuf, &init, sizeof(CAnimInit)); g_mode = mode; g_delay = delay; }
void CAnimateThing::UpdateFrameLogic(const CAnimFrameTime& time) { ++g_frameUpdates; g_time = time.Frame + time.SubFrame + time.Extra; }
void CAnimateThing::Reset() { ++g_resets; }
static CAnimateThing g_animate;
CAnimateThing& CTCAnimationComplex::GetAnimation() { return g_animate; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CThing::UpdateGraphicAppearance(long reason) { g_reason = reason; }
unsigned long* CThing::GetRandomSeed() { return &g_seed; }
void CThing::SetAsFrameUpdated() { ++g_marked; }
#define CHECK(c) if (!(c)) { std::printf("00835e30_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static CThingDeadCreature d; static CDefClassBase base; static CTCAnimationComplex complex; static CInterfaceMap::SEntry e[2];
    static CAnimFilterBase filter;
    e[0].type = 0x5a; e[0].component = (CTCBase*)&complex; e[1].type = 0x7fffffff; e[1].component = 0;
    d.m_interfaces.first = e; d.m_interfaces.last = e + 1; d.m_presentBits[2] = 0x4000000;
    d.PBaseDef = &base; d.PAnimFilter.m_pObject = &filter; g_app.m_refCount = 0;
    g_modeHas = true;
    d.SetupDeadAnimation();
    CHECK(g_resets == 1 && g_sets == 1 && g_lastInit.AnimIndex == 7 && g_lastInit.AnimType == 2 && g_lastInit.Mode == 1);
    CHECK(g_lastInit.Flag05 && g_lastInit.Flags == 0x44 && g_lastInit.Percent == 100 && g_lastInit.Arg24 == 0);
    CHECK(g_lastInit.Blend.m_first == 0.0f && g_mode == 5 && g_delay == 0 && g_finished == 1 && g_seedSeen == &g_seed && g_exclude == -1);
    CHECK(g_frameUpdates == 1 && g_time == 0.0f && g_marked == 1 && g_reason == 2 && g_app.m_refCount == 0 && g_destroyed == 1);
    g_modeHas = false; g_anyHas = true; g_app.m_refCount = 0;
    d.SetupDeadAnimation();
    CHECK(g_sets == 2 && g_lastInit.AnimIndex == 9 && g_frameUpdates == 2 && g_destroyed == 2);
    g_anyHas = false; g_app.m_refCount = 1;
    d.SetupDeadAnimation();                                            // no dead animation: only the frame update
    CHECK(g_sets == 2 && g_resets == 2 && g_frameUpdates == 3 && g_marked == 3 && g_app.m_refCount == 1 && g_destroyed == 2);
    std::printf("00835e30_TEST PASS\n");
    return 0;
}