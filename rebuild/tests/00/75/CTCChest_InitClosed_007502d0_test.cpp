// Behaviour test for 007502d0: src/007502d0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCChest::InitClosed()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCChest::InitClosed() -- set the chest up from its def: reset the thing's animator and start the appearance's first "CHEST_OPEN" animation (stopped, update mode 1, priority 100), reset the open/close helper to closed with the animation's length as the opening time (snapped open, animation at its end, when the chest is already open), and keep the thing frame-updating while the helper is mid-transition
// Leaf: ?InitClosed@CTCChest@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007502D0, 407 bytes, plain `ret`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCAnimationComplex 0x5A),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::Reset 0x0070EC30 (name unproven, as the landed
//   0x00835E30), CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (result ignored), CCharString(const char*, long)
//   0x0099EBF0 ("CHEST_OPEN" 0x01269568) / ~CCharString 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&, long,
//   const CAnimFilterBase&) 0x005DC340 (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / 0x0122FFCC), CTCBase::GetWorld
//   0x00686A70, CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::GetNonLoopingDuration 0x00A23CE0,
//   CAnimInit::Set 0x0070C050, CAnimateThing::SetMainAnim 0x0070D580, COpenCloseHelper::InitOpened 0x00A55C00,
//   CThing::SetForceFrameUpdate 0x004C7C60; the defs' slot 1.
// Layout: CTCChest (retail, Ego_r.pdb - 4): ChestOpener COpenCloseHelper +0x1c (Opened, Closed, Opening, Closing,
//   OpenCloseTransitionAmount +4, LastOpenCloseTransitionAmount +8, TotalTimeToOpen +0xc, TotalTimeToClose +0x10),
//   ChestOpen +0x30; CThing bitset +0x20, map +0x44, PBaseDef +0x70; CWorld PMainMeshBank +0x3c; CAppearanceDef
//   Animation +0x34; CAnimateThing main anim set CurrentPercentageComplete +0x1c / LastPercentageComplete +0x20 (Ego_r
//   minus the 4-byte-smaller retail vectors).
// Shape: the animation complex is `T* p = 0; if (HasTC) p = GetTC;`; the thing def is held add-referenced by a CDefPointer
//   copy for the whole function; GetSubDef has a same-TU out-only stand-in; the init is the CAnimInit Set-forwarding ctor
//   (frame zero stores after the pushes); the mesh bank is named before the PeekAnimation argument; the helper reset is an
//   inline Init in retail's store order (closed flag, both transition amounts last-first, the two times, then the
//   opening/closing flags); the transition test compares Last != Amount.
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
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
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
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
class CAnimSet { public:
    long          UpdateMode;                                  // +0x00
    unsigned char _pad04[0x1c - 0x04];                         // Anims, InternalAlphas
    float         CurrentPercentageComplete;                   // +0x1c
    float         LastPercentageComplete;                      // +0x20
};
class CAnimateThing { public:
    CAnimSet MainAnimSet;                                      // +0x00 (CurrentAnim.MainAnimSet)
    void Reset();                                              // 0x0070EC30
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                             // 0x0070B460
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                           // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CThingBaseDef* PBaseDef;                                   // +0x70
    void SetForceFrameUpdate(bool force);                      // 0x004C7C60
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};

class C3DAnimationInfo { public:
    float GetNonLoopingDuration() const;                       // 0x00A23CE0
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                              // +0x3c
};

struct COpenCloseHelper {
    bool  Opened;                                              // +0x00
    bool  Closed;                                              // +0x01
    bool  Opening;                                             // +0x02
    bool  Closing;                                             // +0x03
    float OpenCloseTransitionAmount;                           // +0x04
    float LastOpenCloseTransitionAmount;                       // +0x08
    float TotalTimeToOpen;                                     // +0x0c
    float TotalTimeToClose;                                    // +0x10
    void Init(float timeToOpen, float timeToClose)
    {
        Opened = false;
        Closed = true;
        LastOpenCloseTransitionAmount = 0.0f;
        OpenCloseTransitionAmount = 0.0f;
        TotalTimeToOpen = timeToOpen;
        TotalTimeToClose = timeToClose;
        Opening = false;
        Closing = false;
    }
    void InitOpened();                                         // 0x00A55C00
    bool IsTransitioning() const
    {
        return Opening || Closing || LastOpenCloseTransitionAmount != OpenCloseTransitionAmount;
    }
};

class CTCChest {
public:
    void*            __vftable;                                // +0x00
    CThing*          ParentThing;                              // +0x04
    unsigned char    _pad08[0x1c - 0x08];
    COpenCloseHelper ChestOpener;                              // +0x1c
    bool             ChestOpen;                                // +0x30
    CWorld* GetWorld();                                        // 0x00686A70 (CTCBase)
    void InitClosed();
};

void CTCChest::InitClosed()
{
    CTCAnimationComplex* complex = 0;
    if (ParentThing->HasTC(0x5a))
        complex = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animator = complex->GetAnimation();
    animator.Reset();
    CDefPointer<CThingBaseDef> thingDef(ParentThing->PBaseDef);
    CDefPointer<CAppearanceDef> appearance;
    thingDef->GetSubDef(&appearance);
    const CAnimationEntry* entry = appearance->Animation.GetAnimation("CHEST_OPEN", 0, CAnimFilterNoGroup());
    CMeshDataBank* meshBank = GetWorld()->PMainMeshBank;
    float duration = meshBank->PeekAnimation(entry->AnimIndex)->GetNonLoopingDuration();
    CAnimInit init(0, 0, entry->AnimIndex, 1, false, 0, 100);
    animator.SetMainAnim(init, 1, 0);
    ChestOpener.Init(duration, 0.0f);
    if (ChestOpen) {
        ChestOpener.InitOpened();
        animator.MainAnimSet.CurrentPercentageComplete = duration;
        animator.MainAnimSet.LastPercentageComplete = duration;
    }
    if (ChestOpener.IsTransitioning())
        ParentThing->SetForceFrameUpdate(true);
}

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static int g_strings = 0;
static char g_name[64];
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strings; CHECK(length == -1); }
CCharString::~CCharString() { --g_strings; }
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return false; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static CAnimationEntry g_entry = { 17 };
static long g_index = -1; static bool g_filterNoGroup = false; static const CAnimationSet* g_set = 0;
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ Log("GetAnimation"); strcpy(g_name, (const char*)name.PStringData); g_index = index; g_filterNoGroup = filter.Accept(0); g_set = this; return &g_entry; }
void CDefPointeeBase::Destroy0() {}
static int g_destroyed = 0;
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CAppearanceDef* g_appearance = 0;
void* FindDef_CThingBaseDef_GetSubDef_0() { Log("GetSubDef"); if (g_appearance) ++g_appearance->RefCount; return g_appearance; }
static long g_set7[7];
void CAnimInit::Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
{ Log("Set"); g_set7[0] = type; g_set7[1] = (long)typeFlags; g_set7[2] = anim; g_set7[3] = updateMode; g_set7[4] = looping; g_set7[5] = (long)flags; g_set7[6] = priority; }
static long g_mainMode = -1, g_mainDelay = -1; static float g_frameCur = -1, g_frameLast = -1;
void CAnimateThing::Reset() { Log("Reset"); }
void CAnimateThing::SetMainAnim(const CAnimInit& init, long mode, long delay)
{ Log("SetMainAnim"); g_mainMode = mode; g_mainDelay = delay; g_frameCur = init.Frame.Current; g_frameLast = init.Frame.Last; }
static CAnimateThing g_animator;
CAnimateThing& CTCAnimationComplex::GetAnimation() { Log("GetAnimationThing"); return g_animator; }
static int g_forced = 0;
void CThing::SetForceFrameUpdate(bool force) { Log("ForceUpdate"); if (force) ++g_forced; }
float C3DAnimationInfo::GetNonLoopingDuration() const { return 2.5f; }
static C3DAnimationInfo g_info; static unsigned long g_peeked = 0;
const C3DAnimationInfo* CMeshDataBank::PeekAnimation(unsigned long index) const { Log("Peek"); g_peeked = index; return &g_info; }
void COpenCloseHelper::InitOpened() { Log("InitOpened"); Opened = true; Closed = false; OpenCloseTransitionAmount = 1.0f; }
static CMeshDataBank g_bank;
static CWorld g_world;
CWorld* CTCChest::GetWorld() { Log("GetWorld"); return &g_world; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }

static CTCAnimationComplex g_complex;
static void Reset() { g_log[0] = 0; g_step = 0; g_name[0] = 0; g_index = -1; g_filterNoGroup = false; g_forced = 0; g_destroyed = 0; g_peeked = 0; g_mainMode = g_mainDelay = -1; memset(&g_animator, 0, sizeof g_animator); }

int main()
{
    g_world.PMainMeshBank = &g_bank;
    CAppearanceDef* appearance = new CAppearanceDef; appearance->RefCount = 1; g_appearance = appearance;
    CThingBaseDef* thingDef = new CThingBaseDef; thingDef->RefCount = 1;
    CThing thing; memset(&thing, 0, sizeof thing);
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x5a; g_entries[1].component = (CTCBase*)&g_complex;
    thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 2;
    thing.m_presentBits[2] = 1UL << 26;
    thing.PBaseDef = thingDef;
    CTCChest chest; memset(&chest, 0xcd, sizeof chest);
    chest.ParentThing = &thing;
    // 1: closed chest: animation started stopped, helper closed with the duration as opening time, no forced update
    {
        Reset(); chest.ChestOpen = false;
        chest.InitClosed();
        CHECK(strcmp(g_log, "1:GetAnimationThing;2:Reset;3:GetSubDef;4:GetAnimation;5:GetWorld;6:Peek;7:Set;8:SetMainAnim;") == 0);
        CHECK(strcmp(g_name, "CHEST_OPEN") == 0 && g_index == 0 && g_filterNoGroup && g_set == &appearance->Animation);
        CHECK(g_peeked == 17);
        CHECK(g_set7[0] == 0 && g_set7[1] == 0 && g_set7[2] == 17 && g_set7[3] == 1 && g_set7[4] == 0 && g_set7[5] == 0 && g_set7[6] == 100);
        CHECK(g_mainMode == 1 && g_mainDelay == 0 && g_frameCur == 0.0f && g_frameLast == 0.0f);
        CHECK(!chest.ChestOpener.Opened && chest.ChestOpener.Closed && !chest.ChestOpener.Opening && !chest.ChestOpener.Closing);
        CHECK(chest.ChestOpener.OpenCloseTransitionAmount == 0.0f && chest.ChestOpener.LastOpenCloseTransitionAmount == 0.0f);
        CHECK(chest.ChestOpener.TotalTimeToOpen == 2.5f && chest.ChestOpener.TotalTimeToClose == 0.0f);
        CHECK(g_forced == 0 && g_animator.MainAnimSet.CurrentPercentageComplete == 0.0f);
        CHECK(appearance->RefCount == 1 && thingDef->RefCount == 1 && g_destroyed == 0 && g_strings == 0);
    }
    // 2: open chest: helper snapped open (transitioning in this stub), animation set to its end, forced frame update
    {
        Reset(); chest.ChestOpen = true;
        chest.InitClosed();
        CHECK(strcmp(g_log, "1:GetAnimationThing;2:Reset;3:GetSubDef;4:GetAnimation;5:GetWorld;6:Peek;7:Set;8:SetMainAnim;9:InitOpened;10:ForceUpdate;") == 0);
        CHECK(chest.ChestOpener.Opened && !chest.ChestOpener.Closed);
        CHECK(g_animator.MainAnimSet.CurrentPercentageComplete == 2.5f && g_animator.MainAnimSet.LastPercentageComplete == 2.5f);
        CHECK(g_forced == 1);
        CHECK(appearance->RefCount == 1 && thingDef->RefCount == 1);
    }
    // 3: last references: both defs are destroyed on the way out
    {
        Reset(); chest.ChestOpen = false;
        appearance->RefCount = 0; thingDef->RefCount = 0;
        chest.InitClosed();
        CHECK(g_destroyed == 2 && appearance->RefCount == 0 && thingDef->RefCount == 0);
    }
    if (fails == 0) printf("007502d0_TEST PASS\n");
    return fails;
}