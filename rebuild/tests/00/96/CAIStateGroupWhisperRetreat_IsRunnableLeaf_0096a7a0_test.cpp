// Behaviour test for 0096a7a0: src/0096a7a0.cpp copied verbatim; IsRunnableLeaf, on a fresh hit, stamps the frame and
// counts the hit; when the count reaches the retreat def's HitsBeforeRetreat it resets the count, points PArenaCentre at
// the thing scripted "WhisperArenaCentre" (null when none) and returns true; the def reference is always released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_WhisperRetreat::IsRunnableLeaf() -- on a fresh hit, stamp the hit frame and count it; once the count reaches the def's hits-before-retreat, reset it and point PArenaCentre at the thing scripted "WhisperArenaCentre"; returns whether Whisper retreats
// Leaf: ?IsRunnableLeaf@CAIStateGroup_WhisperRetreat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096A7A0, length 270 bytes (plain `ret`).
// Callees: FindHitEvent 0x0096A260, GetPWorld 0x008FCFC0 + the static frame getter 0x0049D870, CDefClassBase::GetSubDef
//          0x00956590 (on the creature's def +0x70), CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//          PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName,CThing> 0x0056D050,
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10; def slot 1.
// Layout: Ego_r.pdb CAIStateGroup_WhisperRetreat: PArenaCentre +0x1c, bIsRangeAttack +0x24, NumHits +0x28,
//          FrameOfLastHit +0x2c; the def's hit threshold at +0x3c (the manifest's GetSubDef<CAICreatureWillPowerIndicatorDef>
//          label is propagated: that def is 0x2c bytes, so the def type is modelled locally).
// Shape: GetSubDef and GetPFirstThing are same-TU noinline stand-in bodies that write only their out slot, so neither the
//          def slot nor `arenaCentre` is address-exposed: the frame packs to 0x10 (the HasScriptName filter temporary reuses
//          the def slot, the intelligent-pointer temporary the name's) and the zeros are stored as immediates.
struct CThing;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CThingFilter_HasScriptName {
public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThingSearchTools {
public:
    template<class F, class T> bool GetPFirstThing(const F& filter, T** ppThing) const;   // 0x0056D050 (HasScriptName, CThing)
};
void* Find_CThingSearchTools_GetPFirstThing(const void* tools, const void* filter);   // stand-in callee
#pragma auto_inline(off)
template<class F, class T> __declspec(noinline) bool CThingSearchTools::GetPFirstThing(const F& filter, T** ppThing) const
{
    void* thing = Find_CThingSearchTools_GetPFirstThing(this, &filter);
    if (!thing)
        return false;
    *ppThing = (T*)thing;
    return true;
}
#pragma auto_inline(on)
struct CWorld {
    static long __fastcall GetFrame();                 // 0x0049D870
};
struct CDefVirtual {
    virtual ~CDefVirtual();                            // slot 0
    virtual void Destroy();                            // slot 1
};
struct CWhisperRetreatDef : CDefVirtual {
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          HitsBeforeRetreat;                   // +0x3c
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
struct CDefClassBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00956590
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CThingCreature {
    unsigned char  _pad[0x70];
    CDefClassBase* m_pDef;                             // +0x70
    CWhisperRetreatDef* GetRetreatDef()                // counted reference handed over to the caller
    {
        CDefPointer<CWhisperRetreatDef> def;
        m_pDef->GetSubDef(def);
        return def.m_p;
    }
};
struct CAIBrain {
    unsigned char   _pad[0x18];
    CThingCreature* PCreature;                         // +0x18
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    long ScriptAIPriority;                             // +0x08
    void* Name_PStringData;                            // +0x0c
    unsigned char Terminated;                          // +0x10
    unsigned char Priority;                            // +0x11
    unsigned char Enabled;                             // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                               // 0x008FCFC0
    CThingSearchTools* PeekThingSearchTools();         // 0x008FD000
};
struct CAIStateGroup_WhisperRetreat : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    CIntelligentPointerThing PArenaCentre;             // +0x1c
    bool  bIsRangeAttack;                              // +0x24
    long  NumHits;                                     // +0x28
    long  FrameOfLastHit;                              // +0x2c
    bool FindHitEvent();                               // 0x0096A260
    bool IsRunnableLeaf();
};

bool CAIStateGroup_WhisperRetreat::IsRunnableLeaf()
{
    CThing* arenaCentre = 0;
    if (FindHitEvent()) {
        FrameOfLastHit = GetPWorld()->GetFrame();
        ++NumHits;
        CWhisperRetreatDef* retreat = PBrain->PCreature->GetRetreatDef();
        if (NumHits >= retreat->HitsBeforeRetreat) {
            NumHits = 0;
            {
                CCharString name("WhisperArenaCentre");
                PeekThingSearchTools()->GetPFirstThing(CThingFilter_HasScriptName(name), &arenaCentre);
            }
            PArenaCentre = arenaCentre;
            if (DecRefToZero(retreat->RefCount))
                retreat->Destroy();
            return true;
        }
        if (DecRefToZero(retreat->RefCount))
            retreat->Destroy();
    }
    return false;
}
// ---- harness ----
static int g_destroyed = 0, g_strCtor = 0, g_strCopy = 0, g_strDtor = 0, g_searches = 0, g_ipDtor = 0;
static bool g_hit = false; static long g_frame = 0; static void* g_def = 0; static const void* g_subOwner = 0;
static CThing* g_found = 0; static char g_searchName[64]; static const void* g_searchTools = 0;
static unsigned char g_tools[4]; static unsigned char g_world[4];
CCharString::CCharString(const char* text, long length) { ++g_strCtor; PStringData = (void*)text; CHECK(length == -1); }
CCharString::CCharString(const CCharString& other) { ++g_strCopy; PStringData = other.PStringData; }
CCharString::~CCharString() { ++g_strDtor; }
long __fastcall CWorld::GetFrame() { return g_frame; }
CDefVirtual::~CDefVirtual() { }
void CDefVirtual::Destroy() { ++g_destroyed; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_subOwner = owner; if (g_def) ++((CWhisperRetreatDef*)g_def)->RefCount; return g_def; }
void* Find_CThingSearchTools_GetPFirstThing(const void* tools, const void* filter)
{ ++g_searches; g_searchTools = tools; strcpy(g_searchName, (const char*)((const CThingFilter_HasScriptName*)filter)->ScriptName.PStringData);
  return g_found; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
CAIStateGroupBase::~CAIStateGroupBase() { }
CWorld* CAIStateGroupBase::GetPWorld() { return (CWorld*)g_world; }
CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools() { return (CThingSearchTools*)g_tools; }
bool CAIStateGroup_WhisperRetreat::FindHitEvent() { return g_hit; }
int main()
{
    static unsigned char defClassBuf[4];
    static unsigned char creatureBuf[sizeof(CThingCreature)]; CThingCreature& creature = *(CThingCreature*)creatureBuf;
    creature.m_pDef = (CDefClassBase*)defClassBuf;
    CAIBrain brain; brain.PCreature = &creature;
    static CWhisperRetreatDef def; def.RefCount = 1; def.HitsBeforeRetreat = 2;
    static unsigned char groupBuf[sizeof(CAIStateGroup_WhisperRetreat)];
    CAIStateGroup_WhisperRetreat& group = *(CAIStateGroup_WhisperRetreat*)groupBuf;
    group.PBrain = &brain; group.NumHits = 0; group.FrameOfLastHit = 0; group.PArenaCentre.m_pItem = 0;
    static unsigned char thingBuf[4]; CThing* centre = (CThing*)thingBuf;
    g_def = &def; g_found = centre;

    // no hit: nothing happens
    g_hit = false; g_frame = 5;
    CHECK(!group.IsRunnableLeaf() && group.NumHits == 0 && group.FrameOfLastHit == 0 && g_subOwner == 0);

    // first hit: counted, below the threshold
    g_hit = true; g_frame = 10;
    CHECK(!group.IsRunnableLeaf() && group.NumHits == 1 && group.FrameOfLastHit == 10 && g_subOwner == defClassBuf);
    CHECK(def.RefCount == 1 && g_searches == 0 && g_destroyed == 0);

    // second hit: threshold reached, arena centre found
    g_frame = 20;
    CHECK(group.IsRunnableLeaf() && group.NumHits == 0 && group.FrameOfLastHit == 20);
    CHECK(g_searches == 1 && g_searchTools == g_tools && strcmp(g_searchName, "WhisperArenaCentre") == 0);
    CHECK(group.PArenaCentre.m_pItem == centre && def.RefCount == 1 && g_destroyed == 0);
    CHECK(g_strCtor == 1 && g_strCtor + g_strCopy == g_strDtor && g_ipDtor == 1);

    // threshold reached with no scripted thing: the centre is cleared; last reference destroyed
    g_found = 0; group.NumHits = 1; def.RefCount = 0;
    CHECK(group.IsRunnableLeaf() && group.PArenaCentre.m_pItem == 0 && g_destroyed == 1);

    if (fails == 0) printf("0096a7a0_TEST PASS\n");
    return fails;
}