// NScript::CV_ArcheryCompetitionScript::DoCountDown(long) -- take scripting control of the hero, then count the archery round in on screen (the remaining seconds each second, then "GO")
// Grade: DIFFER(541v536) -- block placement only (62 masked lines, every instruction otherwise present): retail keeps the post-wait IsActiveThreadTerminating exit and the hero-wait loop's return block directly after the post-wait test and tail-shares the "release hero; DeregisterTimer; ret 4" epilogue; VC7.1 here sinks both return blocks to the end and duplicates one inline destructor. 19 probes (loop forms for/while/for(;;), named locals, bool loop variable, GetTime out of line, /Ob1 /Og- /Os /GL, QFE 4035) did not move the blocks; the closest alternative p14 keeps the exit in place but inlines the destructors at more sites (561v536, 47 lines). Next: the real CV_ArcheryCompetitionScript source shape (Aeon Lua port of quest 004 / FSE) for how the countdown loop and early returns were written, and a CScriptGameResourceObjectScriptedThingBase dtor declared out of line.
// Leaf: ?DoCountDown@CV_ArcheryCompetitionScript@NScript@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E36750, 536 bytes, thiscall `ret 4`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);       // 0x0099F830
#include "engine/CBaseClass.h"
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ArcheryCompetitionScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void DoCountDown(long time);
};
} // namespace NScript

void NScript::CV_ArcheryCompetitionScript::DoCountDown(long time)
{
    CTimer timer;
    timer = time;
    CScriptGameResourceObjectScriptedThingBase hero;
    while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (timer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->AddScreenTitleMessage(GFIntToCharString(timer.GetTime() + 1), 0.6f, false);
        Game->Pause(1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->AddScreenTitleMessage("TEXT_QST_004_COUNTDOWN_40", 1.0f, false);
    Game->Pause(0.3f);
    if (hero.IsValid())
        hero.Release();
}
