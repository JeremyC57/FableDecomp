// CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy) -- while the spy lives, each time EnableSpeechSpy is raised pick the spy's next line (sentry-seen, hero-proximity or, near the M_AT_EntitySpeech marker, the normal lines; the first five in order, then random ones without repeating the last) and play it as a conversation with the hero, restarting SpeechSpyTimer
// Leaf: ?WatchForSpySpeechMarkers@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0EF60, length 1824 bytes (ret 0xc: the CScriptThing argument by value).
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=
//   0x0099EFB0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, rand 0x00BFEB16, CScriptThing::IsAlive 0x004AB130 /
//   ~CScriptThing 0x004AA840, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer (the inline CTimer::SetTime); the game script interface
//   (+0x40) slots 7, 70, 77, 364, 365, 366. Data: pGlobalScriptDef 0x0143E90C (+0xA40 .. +0xA58 spy speech distances/delays).
// Strings: "M_AT_EntitySpeech" 0x012F8080.
// Layout (retail = Ego_r.pdb - 0x14, -4 per preceding vector): SpySeenBySentry +0x5a, SpyHeroProximity +0x5d, SpyContactMade
//   +0x5f, EnableSpeechSpy +0x69, SpeechSpyCount +0x78, SpeechSpyProximityCount +0x7c, SpeechSpySentryCount +0x80,
//   SpeechSpyTimer +0xa0, lastRandomSpySpeechIdx +0xa4, SpySpeechNormal +0x140, SpySpeechRandom +0x154,
//   SpySpeechProximityNormal +0x168, SpySpeechProximityRandom +0x17c, SpySpeechSentryNormal +0x190, SpySpeechSentryRandom +0x1a4.
// Shape: generated script code; the outer wait is `for (;;) { if (terminating) return; ... NewScriptFrame(); }` (VC
//   duplicates the head test after the NewScriptFrame). Exits before the marker exists expand the spy argument's destructor
//   inline; exits after it call ~CScriptThing out of line (in-body inline_depth(0), as the landed CMeleeThunder::Main 0x00D58080).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern "C" int __cdecl rand();                          // 0x00BFEB16
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define SLOTS_2_74 \
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); \
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); \
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); \
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); \
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); \
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); \
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); \
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); \
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); \
    virtual void t072(); virtual void t073(); virtual void t074();
struct CScriptThingImpl {                               // the game-side object behind a CScriptThing (PImp)
    virtual void t000(); virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad000[0xa40];
    float AmbushTradersSpySpeechDistance;               // +0xa40
    unsigned char _pad0a44[0xa4c - 0xa44];
    long  AmbushTradersSpyProximitySpeechDelay;         // +0xa4c
    long  AmbushTradersSpySpeechDelay;                  // +0xa50
    long  AmbushTradersSpyNoSpeechDelay;                // +0xa54
    long  AmbushTradersSpySentrySpeechDelay;            // +0xa58
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
struct CTimer {
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x5a - 0x48];
    bool                  SpySeenBySentry;              // +0x5a
    bool                  SpyFollowing;                 // +0x5b
    bool                  SpyFollowingHero;             // +0x5c
    bool                  SpyHeroProximity;             // +0x5d
    unsigned char         _pad5e;
    bool                  SpyContactMade;               // +0x5f
    unsigned char         _pad60[0x69 - 0x60];
    bool                  EnableSpeechSpy;              // +0x69
    unsigned char         _pad6a[0x78 - 0x6a];
    long                  SpeechSpyCount;               // +0x78
    long                  SpeechSpyProximityCount;      // +0x7c
    long                  SpeechSpySentryCount;         // +0x80
    unsigned char         _pad84[0xa0 - 0x84];
    CTimer                SpeechSpyTimer;               // +0xa0
    long                  lastRandomSpySpeechIdx;       // +0xa4
    unsigned char         _pada8[0x140 - 0xa8];
    CCharString           SpySpeechNormal[5];           // +0x140
    CCharString           SpySpeechRandom[5];           // +0x154
    CCharString           SpySpeechProximityNormal[5];  // +0x168
    CCharString           SpySpeechProximityRandom[5];  // +0x17c
    CCharString           SpySpeechSentryNormal[5];     // +0x190
    CCharString           SpySpeechSentryRandom[5];     // +0x1a4
    void WatchForSpySpeechMarkers(CScriptThing spy);
};
void CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy)
{
    CCharString speech;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!EnableSpeechSpy && spy.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!spy.IsAlive()) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            break;
        }
        {
        CScriptThing marker = Game->GetNearestWithScriptName(spy, "M_AT_EntitySpeech");
        if (SpySeenBySentry && !SpyContactMade && !SpyHeroProximity) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            SpeechSpyTimer.SetTime(pGlobalScriptDef->AmbushTradersSpySentrySpeechDelay);
            if (SpeechSpySentryCount < 5) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechSentryNormal[SpeechSpySentryCount];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    SpeechSpySentryCount++;
                }
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechSentryRandom[rand() % 5];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                }
            }
        } else if (SpyHeroProximity && !SpyContactMade) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            SpeechSpyTimer.SetTime(pGlobalScriptDef->AmbushTradersSpyProximitySpeechDelay);
            if (SpeechSpyProximityCount < 5) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechProximityNormal[SpeechSpyProximityCount];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    SpeechSpyProximityCount++;
                }
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                long index;
                for (;;) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    index = rand() % 5;
                    if (index != lastRandomSpySpeechIdx)
                        break;
                    Game->NewScriptFrame();
                }
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                lastRandomSpySpeechIdx = index;
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechProximityRandom[index];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (IsDistanceBetweenThingsUnder(spy, marker, pGlobalScriptDef->AmbushTradersSpySpeechDistance)) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                if (SpeechSpyCount < 5) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    speech = SpySpeechNormal[SpeechSpyCount];
#pragma inline_depth(0)
                    if (spy.IsAlive()) {
#pragma inline_depth()
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        long conversation = Game->AddNewConversation(spy, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                        SpeechSpyCount++;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long index;
                    for (;;) {
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        index = rand() % 5;
                        if (index != lastRandomSpySpeechIdx)
                            break;
                        Game->NewScriptFrame();
                    }
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    lastRandomSpySpeechIdx = index;
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    speech = SpySpeechRandom[index];
#pragma inline_depth(0)
                    if (spy.IsAlive()) {
#pragma inline_depth()
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        long conversation = Game->AddNewConversation(spy, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    }
                }
                SpeechSpyTimer.SetTime(pGlobalScriptDef->AmbushTradersSpySpeechDelay);
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                SpeechSpyTimer.SetTime(pGlobalScriptDef->AmbushTradersSpyNoSpeechDelay);
            }
        }
        EnableSpeechSpy = false;
#pragma inline_depth(0)
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}