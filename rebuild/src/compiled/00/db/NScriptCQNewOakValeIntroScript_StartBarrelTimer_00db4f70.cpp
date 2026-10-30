// NScript::CQ_NewOakValeIntroScript::StartBarrelTimer() -- Oakvale intro barrel-watch thread: wait for the watch timer to start, put up the HUD clock bar, then each frame colour it green while the hero stands within 2 units of the warehouse guard point (red otherwise) and feed it the remaining watch time, until the barrel man has spoken to the returning hero; then remove the bar
// Leaf: ?StartBarrelTimer@CQ_NewOakValeIntroScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB4F70, 562 bytes, thiscall, plain `ret`.
// Identity (proven): the landed NScript::CQ_NewOakValeIntroScript::Main 0x00DABAC0 spawns this address (site 0x00DAC255, the row's
//   only reference) as the member function of its CSpawnedFunc<CQ_NewOakValeIntroScript> (vtable 0x012D7A3C) right after the
//   "StartBarrelTimer" name literal; Ego_r.pdb ?StartBarrelTimer@CQ_NewOakValeIntroScript@NScript@@QAEXXZ (already bound so).
//   Catalog label "Script_Guard_Duty" is propagated.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   ~CScriptThing 0x004AA840, IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) 0x00CBE2FF
//   (__fastcall); g_pGameScriptInterface 0x0143E8F8 slot 90 GetTimer (the inline CTimer::GetTime); Game (+0x40) slots
//   7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 324 AddQuestInfoBar, 332 UpdateQuestInfoBar,
//   333 ChangeQuestInfoBarColour, 338 RemoveQuestInfoElement.
// Literals: "" 0x0122D70E, "HUD_CLOCK_ICON" 0x012CA7C8, "M_WHouse_GuardPoint" 0x012D9000; floats 45.0f, 0.0f, 1.0f, 2.0f,
//   -1.0f, -1.0f (immediates); colours green (0, 255, 0, 255) and red (255, 0, 0, 255) built byte by byte.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CScriptThing, CTimer, CLevelScriptBase, CGameScriptInterfaceBase);
//   members at Ego_r.pdb - 0x14, less 4 more per STLport vector before them (the eight 12-byte VillagerSpeech vectors):
//   GUIBarrelCounter +0x60, BarrelManSpokenToHeroOnReturn +0x73, WatchTimer +0x108.
// Shape: the two waits are the script idiom (NewScriptFrame, termination return) rotated by VC; the two colour branches
//   share one ChangeQuestInfoBarColour call tail; the guard point is a named local destroyed at every exit after it.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#pragma pack(push, 1)
class C3DVector {                                       // byte-aligned: Ego_r.pdb places it at odd offsets (+0x8a, +0x99)
public:
    float X, Y, Z;
};
#pragma pack(pop)
class CRGBColour {                                      // Ego_r.pdb union: B, G, R, A bytes (D3DCOLOR order)
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}   // namespace std

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_NewOakValeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool VictimShake;                                   // +0x48
    long StopTimeIndex;                                 // +0x4c
    bool AttackOver;                                    // +0x50
    bool DadFound;                                      // +0x51
    bool DadFinishedIntro;                              // +0x52
    bool DadOfferedRewards;                             // +0x53
    long GoodDeedsPerformed;                            // +0x54
    long BadDeedsPerformed;                             // +0x58
    long GUIGoodDeedCounter;                            // +0x5c
    long GUIBarrelCounter;                              // +0x60
    long GUIBullyHealthCounter;                         // +0x64
    long GuardsDealtWithBadDeeds;                       // +0x68
    bool BullySubdued;                                  // +0x6c
    bool BullyRanOff;                                   // +0x6d
    bool GivenHeroTeddy;                                // +0x6e
    bool HeroAttackedVictim;                            // +0x6f
    bool SpokeAboutFindingTeddy;                        // +0x70
    bool HeroDiscoveredInfidelity;                      // +0x71
    bool BarrelManLeftHeroInCharge;                     // +0x72
    bool BarrelManSpokenToHeroOnReturn;                 // +0x73
    bool BarrelBrokenInstantaneous;                     // +0x74
    bool BarrelBrokenPersistent;                        // +0x75
    C3DVector BarrelBrokenPos;                          // +0x76 (packed, Ego_r.pdb +0x8a)
    bool InstructionGiven_Barrels;                      // +0x82
    bool ReceiveKiss;                                   // +0x83
    bool ReceiveHug;                                    // +0x84
    C3DVector WarehouseMeetPoint;                       // +0x85 (packed, Ego_r.pdb +0x99)
    bool TeddyRuined;                                   // +0x91
    bool GuardsSpokenOnce;                              // +0x92
    bool TalkingToWoman;                                // +0x93
    bool GivenSweets;                                   // +0x94
    bool GivenTheresaChocs;                             // +0x95
    bool VictimComplainsAboutLosingTeddy;               // +0x96
    long lastVillagerSpeechIdx;                         // +0x98
    std::vector<CCharString> VillagerSpeechMaleGood;    // +0x9c
    std::vector<CCharString> VillagerSpeechMaleBad;     // +0xa8
    std::vector<CCharString> VillagerSpeechMaleBoth;    // +0xb4
    std::vector<CCharString> VillagerSpeechMaleNeither; // +0xc0
    std::vector<CCharString> VillagerSpeechFemaleGood;  // +0xcc
    std::vector<CCharString> VillagerSpeechFemaleBad;   // +0xd8
    std::vector<CCharString> VillagerSpeechFemaleBoth;  // +0xe4
    std::vector<CCharString> VillagerSpeechFemaleNeither;   // +0xf0
    bool WhichBadDeedsPerformed[5];                     // +0xfc
    CTimer TalkIntermittentTimer;                       // +0x104
    CTimer WatchTimer;                                  // +0x108
    void StartBarrelTimer();
};
FABLE_STATIC_ASSERT(offsetof(CQ_NewOakValeIntroScript, GUIBarrelCounter) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CQ_NewOakValeIntroScript, BarrelManSpokenToHeroOnReturn) == 0x73);
FABLE_STATIC_ASSERT(offsetof(CQ_NewOakValeIntroScript, lastVillagerSpeechIdx) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CQ_NewOakValeIntroScript, WatchTimer) == 0x108);

void CQ_NewOakValeIntroScript::StartBarrelTimer()
{
    while (WatchTimer.GetTime() <= 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GUIBarrelCounter = Game->AddQuestInfoBar(45.0f, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(0, 255, 0, 255), "HUD_CLOCK_ICON", "", 1.0f);
    CScriptThing guardPoint = Game->GetThingWithScriptName("M_WHouse_GuardPoint");
    while (!BarrelManSpokenToHeroOnReturn) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), guardPoint, 2.0f))
            Game->ChangeQuestInfoBarColour(GUIBarrelCounter, CRGBColour(0, 255, 0, 255), CRGBColour(0, 255, 0, 255));
        else
            Game->ChangeQuestInfoBarColour(GUIBarrelCounter, CRGBColour(255, 0, 0, 255), CRGBColour(255, 0, 0, 255));
        Game->UpdateQuestInfoBar(GUIBarrelCounter, (float)WatchTimer.GetTime(), -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(GUIBarrelCounter);
}
}   // namespace NScript