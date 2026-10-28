// Behaviour test for 00747b80: src/00747b80.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCStealth::SetAsInDanger(const CThing* thing, long status)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCStealth::SetAsInDanger(const CThing* thing, long status) -- record (or refresh) the danger status of a thing; for the main player's creature entering danger start the danger music and stop the rain; restart the in-danger cool-off timer
// Leaf: SetAsInDanger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747B80, 238 bytes (`ret 8`).
// Callees: SGI __find_if over the danger list @ 0x007485F0 (__fastcall: first, last, the by-value thing predicate and the
// iterator tag by reference; compiled in this TU, see the body below), CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (the pair temporary's CIntelligentPointer<const CThing>, vtable
// 0x01237F1C), vector<pair<CIntelligentPointer<const CThing>, CDangerStatus> >::push_back @ 0x00749620, CThing::GetPlayer const
// @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CTCBase::GetWorld @ 0x00686A70 (twice), CMusicManager::EnableDangerMusic(bool)
// @ 0x006BDE50, CEnvironment::SetRainEnabled(bool) @ 0x006B32B0, __ftol2 @ 0x00BFEA70; g_framesPerSecond @ 0x01375550.
// Layout: as the landed ClearDangerStatus 0x00747090 / SetAsRunningAwayFromCombat 0x007470E0 (PDef +0x1c, InDangerCoolOffTimer
// +0x34, DangerThings +0x38/+0x3c of 16-byte {CIntelligentPointer, Status, Timer}); CStealthDef DangerFeedbackHangtimeSeconds +0x40.
// Shape: a miss pushes a pair temporary (pointer built in place, then the status and a zero timer), a hit refreshes the
// entry in place; the cool-off timer is `(long)(PDef ? seconds * fps : 1.0f)` (its __ftol2 tail duplicated per arm).
//        The SGI __find_if instantiation is defined in this TU (noinline, its real unrolled body): with its body visible VC7.1
//        homes the empty iterator-tag temporary in the still-live `status` argument slot as retail; an extern declaration
//        puts it in the `thing` slot instead.
class CThing;
class CBaseObject;
extern long g_framesPerSecond;                          // 0x01375550
class CPlayer { public:
    bool IsMainPlayer() const;                          // 0x00487520
};
class CMusicManager { public:
    void EnableDangerMusic(bool enable);                // 0x006BDE50
};
class CEnvironment { public:
    void SetRainEnabled(bool enabled);                  // 0x006B32B0
};
class CWorld { public:
    unsigned char  _pad00[0x18];
    CEnvironment*  PEnvironment;                        // +0x18 scoped_ptr<CEnvironment>
    unsigned char  _pad1c[0x6c - 0x1c];
    CMusicManager* PMusicManager;                       // +0x6c CCountedPointer<CMusicManager> (object word)
};
class CThing {
public:
    unsigned char _pad[0x10];
    unsigned char m_thingType;                          // +0x10 (2 = creature)
    CPlayer* GetPlayer() const;                         // 0x004C7A30
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01237F1C (const CThing)
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CDangerStatus {                                  // NStealth::CDangerStatus (PDB, 8 bytes)
    long DangerStatus;                                  // +0x00 EDangerStatus
    long Timer;                                         // +0x04
    CDangerStatus(long status) : DangerStatus(status), Timer(0) { }
};
struct CDangerThing {                                   // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>, 16 bytes
    CIntelligentPointer<const CThing> first;            // +0x00
    CDangerStatus second;                               // +0x08
    CDangerThing(const CThing* thing, CDangerStatus status) : first(thing), second(status) { }
};
struct CDangerThingIs {                                 // the find predicate: the entry's thing is the given one
    const CThing* Thing;
    CDangerThingIs(const CThing* thing) : Thing(thing) { }
    bool operator()(const CDangerThing& entry) const;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CDangerThingVector {                             // STLport vector (12 bytes)
    CDangerThing* _M_start;
    CDangerThing* _M_finish;
    CDangerThing* _M_end_of_storage;
    CDangerThing* begin() { return _M_start; }
    CDangerThing* end() { return _M_finish; }
    void push_back(const CDangerThing& value);          // 0x00749620
};
inline bool CDangerThingIs::operator()(const CDangerThing& entry) const { return entry.first.GetPItem() == Thing; }
#pragma auto_inline(off)
__declspec(noinline) CDangerThing* __fastcall __find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred,
                                                        const random_access_iterator_tag&)   // 0x007485F0
{
    long tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
inline CDangerThing* find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
class CStealthDef { public:
    unsigned char _pad[0x40];
    float DangerFeedbackHangtimeSeconds;                // +0x40
};
class CTCStealth { public:
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    bool          Viewable;                             // +0x0c
    bool          Smellable;                            // +0x0d
    bool          Hearable;                             // +0x0e
    bool          Sneaking;                             // +0x0f
    bool          SneakingToggleSet;                    // +0x10
    float         DistanceCanBeHeardFrom;               // +0x14
    float         MaxSneakingSpeed;                     // +0x18
    CStealthDef*  PDef;                                 // +0x1c CDefPointer<CStealthDef const>
    long          FrameStartedWalking;                  // +0x20
    long          FrameStartedSneakingAndMoving;        // +0x24
    long          SneakingAndMovingDeedHandle;          // +0x28
    long          ActionPermittedToSneakID;             // +0x2c
    long          RunningAwayFromCombatTimer;           // +0x30
    long          InDangerCoolOffTimer;                 // +0x34
    CDangerThingVector DangerThings;                    // +0x38
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void SetAsInDanger(const CThing* thing, long status);
};

void CTCStealth::SetAsInDanger(const CThing* thing, long status)
{
    CDangerThing* entry = find_if(DangerThings.begin(), DangerThings.end(), CDangerThingIs(thing));
    if (entry == DangerThings.end())
        DangerThings.push_back(CDangerThing(thing, CDangerStatus(status)));
    else {
        entry->second = CDangerStatus(status);
    }
    if (ParentThing->GetPlayer()->IsMainPlayer() && ParentThing->m_thingType == 2 && InDangerCoolOffTimer < 0) {
        CWorld* world = GetWorld();
        world->PMusicManager->EnableDangerMusic(true);
        CWorld* world2 = GetWorld();
        world2->PEnvironment->SetRainEnabled(false);
    }
    InDangerCoolOffTimer = (long)(PDef ? PDef->DangerFeedbackHangtimeSeconds * g_framesPerSecond : 1.0f);
}

// ---- stubs and driver ----
long g_framesPerSecond = 30;
static bool g_mainPlayer = true; static int g_music = 0, g_musicArg = -1, g_rain = 0, g_rainArg = -1, g_pushes = 0, g_ptrDtors = 0, g_getPlayer = 0;
static const CThing* g_pushedThing = 0; static long g_pushedStatus = -1, g_pushedTimer = -1;
static CPlayer g_player;
bool CPlayer::IsMainPlayer() const { return g_mainPlayer; }
CPlayer* CThing::GetPlayer() const { ++g_getPlayer; return &g_player; }
void CMusicManager::EnableDangerMusic(bool enable) { ++g_music; g_musicArg = enable; }
void CEnvironment::SetRainEnabled(bool enabled) { ++g_rain; g_rainArg = enabled; }
void CBaseIntelligentPointer::v00() {}
template <> void CIntelligentPointer<const CThing>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ptrDtors; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CDangerThingVector::push_back(const CDangerThing& value)
{ ++g_pushes; g_pushedThing = value.first.GetPItem(); g_pushedStatus = value.second.DangerStatus; g_pushedTimer = value.second.Timer; }
static CMusicManager g_musicManager; static CEnvironment g_environment; static CWorld g_world;
CWorld* CTCStealth::GetWorld() { return &g_world; }

static unsigned char g_entryBytes[6 * sizeof(CDangerThing)];
static CThing g_things[8];
static CThing g_parent;
static CStealthDef g_def;
static void Reset(CTCStealth& s, int entries)
{
    g_music = g_rain = g_pushes = g_ptrDtors = g_getPlayer = 0; g_musicArg = g_rainArg = -1; g_pushedThing = 0; g_pushedStatus = g_pushedTimer = -1;
    CDangerThing* e = (CDangerThing*)g_entryBytes;
    for (int i = 0; i < 6; ++i) { e[i].first.m_pItem = &g_things[i]; e[i].second.DangerStatus = 1; e[i].second.Timer = 77; }
    s.DangerThings._M_start = e; s.DangerThings._M_finish = e + entries; s.DangerThings._M_end_of_storage = e + 6;
    s.ParentThing = &g_parent; g_parent.m_thingType = 2; g_mainPlayer = true; s.InDangerCoolOffTimer = -1; s.PDef = &g_def;
}
int main()
{
    static CTCStealth stealth;
    g_world.PEnvironment = &g_environment; g_world.PMusicManager = &g_musicManager; g_def.DangerFeedbackHangtimeSeconds = 2.5f;
    CDangerThing* e = (CDangerThing*)g_entryBytes;

    // new thing: pushed with its status and a zero timer; main player's creature newly in danger -> music on, rain off
    Reset(stealth, 5);
    stealth.SetAsInDanger(&g_things[7], 3);
    CHECK(g_pushes == 1 && g_pushedThing == &g_things[7] && g_pushedStatus == 3 && g_pushedTimer == 0 && g_ptrDtors == 1);
    CHECK(g_music == 1 && g_musicArg == 1 && g_rain == 1 && g_rainArg == 0 && stealth.InDangerCoolOffTimer == 75);

    // known thing (fifth entry, past the unrolled group of four): status refreshed in place, timer reset
    Reset(stealth, 6);
    stealth.SetAsInDanger(&g_things[4], 2);
    CHECK(g_pushes == 0 && e[4].second.DangerStatus == 2 && e[4].second.Timer == 0 && e[3].second.DangerStatus == 1 && e[3].second.Timer == 77);
    CHECK(g_music == 1 && stealth.InDangerCoolOffTimer == 75);
    Reset(stealth, 3);
    stealth.SetAsInDanger(&g_things[0], 4);
    CHECK(g_pushes == 0 && e[0].second.DangerStatus == 4 && e[0].second.Timer == 0);

    // empty list
    Reset(stealth, 0);
    stealth.SetAsInDanger(&g_things[1], 1);
    CHECK(g_pushes == 1 && g_pushedThing == &g_things[1]);

    // already cooling off, not a creature, not the main player: no music or rain change
    Reset(stealth, 2); stealth.InDangerCoolOffTimer = 0;
    stealth.SetAsInDanger(&g_things[1], 1);
    CHECK(g_music == 0 && g_rain == 0 && stealth.InDangerCoolOffTimer == 75);
    Reset(stealth, 2); g_parent.m_thingType = 3;
    stealth.SetAsInDanger(&g_things[1], 1);
    CHECK(g_music == 0 && g_rain == 0);
    Reset(stealth, 2); g_mainPlayer = false;
    stealth.SetAsInDanger(&g_things[1], 1);
    CHECK(g_music == 0 && g_rain == 0 && g_getPlayer == 1);

    // no def: the cool-off is one frame
    Reset(stealth, 2); stealth.PDef = 0;
    stealth.SetAsInDanger(&g_things[1], 1);
    CHECK(stealth.InDangerCoolOffTimer == 1);
    if (fails == 0) printf("00747b80_TEST PASS\n");
    return fails;
}
