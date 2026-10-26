// Behaviour test for 0077d230: src/0077d230.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCCreatureHive::Open()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureHive::Open() -- play the hive's "OPEN_UP" appearance animation as the main animation (type 6, mode 1, not looping, percent 100) and start animating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D230, 243 bytes (+ int3 padding), plain `ret`.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>), CCharString(const
//   char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&, long, const
//   CAnimFilterBase&) @ 0x005DC340, CAnimInit::Set @ 0x0070C050, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   @ 0x0040F020 (inlined lookup of CTCAnimationComplex, interface 0x5a), CTCAnimationComplex::GetAnimation @ 0x0070B460,
//   CAnimateThing::SetMainAnim(const CAnimInit&, long, long) @ 0x0070D580, the def's virtual Destroy (slot 1).
// Literal: "OPEN_UP" @ 0x0126AF84. Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: Ego_r.pdb CTCCreatureHive (0x10): Animating +0x0c; retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the landed CTCAnimationComplex::OnCreate 0x0070B4D0 animation block (out CDefPointer zeroed in the call's frame
//   word, the def read after the name is built and kept in ebx for the release at the end) with the base def read
//   straight from the thing; the animation component comes from the landed CTCCupboard 0x00749B40 GetTCIfPresent
//   accessor (unset when absent: the absent arm reads the key's frame word); Animating is set after SetMainAnim.
//   The CDefPointer releases its def in an inline destructor (so the out word stays live to the end and the
//   inlined key reuses the dead name word, as in retail), and the animation reference is taken into its own
//   local before SetMainAnim's arguments are pushed (retail calls GetAnimation first).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                             // +0x00
};
class CAnimFilterBase { public:                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CAnimInit;
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef();       // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CThingBaseDef* PBaseDef;                    // +0x70
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                            // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CTCCreatureHive { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Animating;                          // +0x0c
    void Open();
};

void CTCCreatureHive::Open()
{
    CDefPointer<CAppearanceDef> out;
    ParentThing->PBaseDef->GetSubDef(&out);
    long animIndex;
    {
        CCharString name("OPEN_UP");
        CAnimFilterNoGroup filter;
        animIndex = out.Object->Animation.GetAnimation(name, 0, filter)->AnimIndex;
    }
    CAnimInit init(6, 0, animIndex, 1, false, 0, 100);
    CAnimateThing& anim = ((CTCAnimationComplex*)ParentThing->GetTCIfPresent(0x5a))->GetAnimation();
    anim.SetMainAnim(init, 1, 0);
    Animating = true;
}

// ---- stubs and driver ----
static const char* g_name; static int g_nameDtors = 0;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; g_name = text; }
CCharString::~CCharString() { ++g_nameDtors; }
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
static int g_noGroupTag;
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return (CAnimFilterBase*)&g_noGroupTag; }
static CAnimationEntry g_entry = { 77 };
static const CAnimationSet* g_set; static long g_index = -5; static bool g_filterIsNoGroup = false;
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ g_set = this; g_index = index; g_filterIsNoGroup = filter.Clone() == (CAnimFilterBase*)&g_noGroupTag && strcmp((const char*)name.PStringData, "OPEN_UP") == 0; return &g_entry; }
static long g_set_args[7]; static int g_setCalls = 0;
void CAnimInit::Set(long a, long b, long c, long d, bool e, unsigned long f, long g)
{ ++g_setCalls; g_set_args[0]=a; g_set_args[1]=b; g_set_args[2]=c; g_set_args[3]=d; g_set_args[4]=e; g_set_args[5]=(long)f; g_set_args[6]=g; }
static CAnimateThing g_animThing; static const CAnimateThing* g_mainThis; static const CAnimInit* g_mainInit; static long g_mainMode = -1, g_mainDelay = -1; static int g_mainCalls = 0;
void CAnimateThing::SetMainAnim(const CAnimInit& init, long mode, long delay) { ++g_mainCalls; g_mainThis = this; g_mainInit = &init; g_mainMode = mode; g_mainDelay = delay; }
struct CTestDef : CAppearanceDef { };
static int g_destroys = 0;
void CDefPointeeBase::Destroy0() { }
void CDefPointeeBase::Destroy() { ++g_destroys; }
static CTestDef g_def;
void* FindDef_CThingBaseDef_GetSubDef() { return &g_def; }
static CInterfaceMap::SEntry g_entries[2];
static const long* g_lbKey;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbKey = type; CHECK(*type == 0x5a); return &g_entries[0]; }
static CTCAnimationComplex g_tcAnim;
CAnimateThing& CTCAnimationComplex::GetAnimation() { CHECK(this == &g_tcAnim); return g_animThing; }
void CTCCreatureHive::v00() { }
int main()
{
    static CThing thing; static CThingBaseDef baseDef;
    thing.PBaseDef = &baseDef;
    thing.m_presentBits[0x5a >> 5] = 1UL << (0x5a & 31);
    g_entries[0].type = 0x5a; g_entries[0].component = (CTCBase*)&g_tcAnim;
    thing.m_interfaces.first = &g_entries[0]; thing.m_interfaces.last = &g_entries[1];
    static CTCCreatureHive hive; hive.ParentThing = &thing; hive.Animating = false;
    g_def.RefCount = 2;
    hive.Open();
    CHECK(g_name != 0 && strcmp(g_name, "OPEN_UP") == 0 && g_nameDtors == 1);
    CHECK(g_set == &g_def.Animation && g_index == 0 && g_filterIsNoGroup);
    CHECK(g_setCalls == 1 && g_set_args[0] == 6 && g_set_args[1] == 0 && g_set_args[2] == 77 && g_set_args[3] == 1 && g_set_args[4] == 0 && g_set_args[5] == 0 && g_set_args[6] == 100);
    CHECK(g_mainCalls == 1 && g_mainThis == &g_animThing && g_mainMode == 1 && g_mainDelay == 0 && g_mainInit != 0);
    CHECK(hive.Animating == true);
    CHECK(g_def.RefCount == 1 && g_destroys == 0);
    hive.Animating = false;
    hive.Open();
    CHECK(g_def.RefCount == 0 && g_destroys == 1 && hive.Animating && g_mainCalls == 2 && g_nameDtors == 2);
    if (fails == 0) printf("0077d230_TEST PASS\n");
    return fails;
}
