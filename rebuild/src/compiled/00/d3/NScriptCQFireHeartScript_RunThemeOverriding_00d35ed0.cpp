// CQ_FireHeartScript::RunThemeOverriding() -- every time the hero reaches the Lookout Point (while the demon door's region is not loaded) override the music and switch to the haunted theme, and restore both once he leaves
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D35ED0, length 519 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface at +0x40: NewScriptFrame (slot 7), IsLevelLoaded (slot 11), TransitionToTheme
//   (slot 656), ResetToDefaultTheme (slot 657), OverrideMusic (slot 696), StopOverrideMusic (slot 697).
// Layout: CScriptBase is 0x40 bytes in retail (Ego_r.pdb - 0x14), the interface pointer sits at +0x40.
// Shape: the level name is a `const CCharString&` temporary, destroyed before the loop test (the `!` result is kept
//   in bl through neg/sbb/inc; the positive test is written `!= false`, which VC keeps as test/setne -- a plain
//   bool test would copy al); the trailing terminating test is a bare call.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_FireHeartScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void RunThemeOverriding();
};
} // namespace NScript
using NScript::CQ_FireHeartScript;

void NScript::CQ_FireHeartScript::RunThemeOverriding()
{
    while (!Game->IsLevelLoaded("DemonDoor_LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("LookoutPoint")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->OverrideMusic((EMusicSetType)0x11, false, false);
        Game->TransitionToTheme("ENVIRONMENT_HAUNTED", 0.5f);
        while (Game->IsLevelLoaded("LookoutPoint") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->StopOverrideMusic(false);
        Game->ResetToDefaultTheme(0.5f);
    }
    if (IsActiveThreadTerminating())
        return;
}