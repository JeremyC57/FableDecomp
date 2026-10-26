// Behaviour test for 0078809b: src/0078809b.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCMeleeTrap::InitialiseAnimation(const CCharString& name, bool loop)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CTCBase;

class CCharString { public:
    void* PStringData;
};
class CAnimationEntry { public:
    long AnimIndex;                                            // +0x00
};
class CAnimFilterBase { public:                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                    // 0x00429A2F
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long RefCount;                                             // +0x04
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> class CDefPointer { public:
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
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
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

class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
};
class CAnimInit { public:                                             // 0x2c bytes
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
class CAnimateThing { public:
    void Reset();                                              // 0x0070EC30
    void SetMainAnim(const CAnimInit& init);                   // 0x0070D920
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:                                   // interface 0x5a
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

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static const char* g_name = 0;
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static CAnimationEntry g_entry = { 23 };
static long g_index = -1; static bool g_filterNoGroup = false; static const CAnimationSet* g_set = 0;
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ Log("GetAnimation"); g_name = (const char*)name.PStringData; g_index = index; g_filterNoGroup = filter.Accept(0); g_set = this; return &g_entry; }
void CDefPointeeBase::Destroy0() {}
static int g_destroyed = 0;
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CAppearanceDef* g_appearance = 0;
void* FindDef_CThingBaseDef_GetSubDef_0() { Log("GetSubDef"); if (g_appearance) ++g_appearance->RefCount; return g_appearance; }
static long g_set7[7];
void CAnimInit::Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
{ Log("Set"); g_set7[0] = type; g_set7[1] = (long)typeFlags; g_set7[2] = anim; g_set7[3] = updateMode; g_set7[4] = looping; g_set7[5] = (long)flags; g_set7[6] = priority; }
static long g_mainMode = -1, g_mainDelay = -1; static float g_frameCur = -1, g_frameLast = -1; static const CAnimInit* g_init1 = 0; static const CAnimInit* g_init3 = 0;
static CAnimateThing* g_reset = 0; static CAnimateThing* g_main1 = 0; static CAnimateThing* g_main3 = 0;
void CAnimateThing::Reset() { Log("Reset"); g_reset = this; }
void CAnimateThing::SetMainAnim(const CAnimInit& init) { Log("SetMainAnim1"); g_main1 = this; g_init1 = &init; }
void CAnimateThing::SetMainAnim(const CAnimInit& init, long mode, long delay)
{ Log("SetMainAnim3"); g_main3 = this; g_init3 = &init; g_mainMode = mode; g_mainDelay = delay; g_frameCur = init.Frame.Current; g_frameLast = init.Frame.Last; }
static CAnimateThing g_animator;
CAnimateThing& CTCAnimationComplex::GetAnimation() { Log("GetAnimationThing"); return g_animator; }
bool CComponentBitset::test(long bit) { return (m_words[bit >> 5] & (1UL << (bit & 31))) != 0; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static const CAnimationEntry* g_strike = 0; static CTCMeleeTrap* g_strikeTrap = 0;
void CTCMeleeTrap::InitialiseStrikeAnimation(const CAnimationEntry& entry) { Log("Strike"); g_strike = &entry; g_strikeTrap = this; }

static CTCAnimationComplex g_complex;
static void Reset() { g_log[0] = 0; g_step = 0; g_name = 0; g_index = -1; g_filterNoGroup = false; g_destroyed = 0; g_mainMode = g_mainDelay = -1; g_strike = 0; }

int main()
{
    CAppearanceDef* appearance = new CAppearanceDef; appearance->RefCount = 1; g_appearance = appearance;
    CThingBaseDef* thingDef = new CThingBaseDef; thingDef->RefCount = 1;
    CThing thing; memset(&thing, 0, sizeof thing);
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x5a; g_entries[1].component = (CTCBase*)&g_complex;
    thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 2;
    thing.UsingComponents.m_words[2] = 1UL << 26;
    thing.PBaseDef = thingDef;
    CTCMeleeTrap trap; memset(&trap, 0xcd, sizeof trap);
    trap.ParentThing = &thing;
    CCharString name; name.PStringData = (void*)"DEFAULT";
    // 1: looping start: reset, the named animation through a no-group filter, type-6 init passed to both setters, strike events
    {
        Reset();
        trap.InitialiseAnimation(name, true);
        CHECK(strcmp(g_log, "1:GetAnimationThing;2:Reset;3:GetSubDef;4:GetAnimation;5:Set;6:SetMainAnim1;7:SetMainAnim3;8:Strike;") == 0);
        CHECK(g_reset == &g_animator && g_main1 == &g_animator && g_main3 == &g_animator && g_init1 == g_init3);
        CHECK(strcmp(g_name, "DEFAULT") == 0 && g_index == 0 && g_filterNoGroup && g_set == &appearance->Animation);
        CHECK(g_set7[0] == 6 && g_set7[1] == 0 && g_set7[2] == 23 && g_set7[3] == 1 && g_set7[4] == 1 && g_set7[5] == 0 && g_set7[6] == 100);
        CHECK(g_mainMode == 1 && g_mainDelay == 0 && g_frameCur == 0.0f && g_frameLast == 0.0f);
        CHECK(g_strike == &g_entry && g_strikeTrap == &trap);
        CHECK(appearance->RefCount == 1 && thingDef->RefCount == 1 && g_destroyed == 0);
    }
    // 2: not looping
    {
        Reset();
        trap.InitialiseAnimation(name, false);
        CHECK(g_set7[4] == 0 && g_set7[0] == 6);
    }
    // 3: last references: the thing def is released after the lookup, the appearance on the way out
    {
        Reset();
        appearance->RefCount = 0; thingDef->RefCount = 0;
        trap.InitialiseAnimation(name, true);
        CHECK(g_destroyed == 2 && appearance->RefCount == 0 && thingDef->RefCount == 0);
        CHECK(strcmp(g_log, "1:GetAnimationThing;2:Reset;3:GetSubDef;4:GetAnimation;5:Set;6:SetMainAnim1;7:SetMainAnim3;8:Strike;") == 0);
    }
    if (fails == 0) printf("0078809b_TEST PASS\n");
    return fails;
}