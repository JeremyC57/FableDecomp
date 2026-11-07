// NScript::CV_BordelloScript::WatchForHeroLeavingRegionWithBeer() -- script thread: while the Bordello region is loaded and the beer is set as the hero's preferred quick-access item, wait; once the hero leaves (or the beer flag is cleared), clear the flag and drop the beer tankard's quick-access preference
// Leaf: ?WatchForHeroLeavingRegionWithBeer@CV_BordelloScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E44980, 184 bytes (code to the second `ret` at 0x00E44A37; an early `ret` at
//   0x00E449F6 is the in-loop terminate exit), plain `ret`, no EH frame; int3 padding to IsHeroWearingBeard 0x00E44A40.
// Identity (proven, two sources): (1) the spawn name: CMagicman::Main 0x00E40E80 builds the thread name
//   "ParentClass." (0x012C9934) + "WatchForHeroLeavingRegionWithBeer" (0x012E48E0) and stores this address as the
//   CSpawnedFunc<CV_BordelloScript> member pointer (`mov [esi+0x34],0x00E44980` at 0x00E411E2 and 0x00E42B1D, object
//   = ParentClass at +0x38, vtable 0x012E38C4); (2) Ego_r.pdb / FableWin.pdb name sets list
//   ?WatchForHeroLeavingRegionWithBeer@CV_BordelloScript@NScript@@QAEXXZ (void (CV_BordelloScript::*)(), the
//   CSpawnedFunc<CV_BordelloScript> ctor's P823@AEXXZ type) directly before ?IsHeroWearingBeard@... in donor order, as
//   retail places 0x00E44980 directly before IsHeroWearingBeard 0x00E44A40.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through Game (+0x40): IsRegionLoaded (slot 12, +0x30), NewScriptFrame (slot 7, +0x1c),
//   SetPreferredQuickAccessItem (slot 312, +0x4e0).
// Strings: "Bordello" 0x012E39A4, "OBJECT_BEER_TANKARD" 0x012E4058.
// Layout: BeerSetToDPad +0x70 (Ego_r.pdb 0x84 - 0x14; the landed Init 0x00E399D0 clears it).
// Shape: the landed CV_ExposeMayorScript::CheckLampSlot 0x00E76CA0 loop: the loop test is a named bool whose region-name
//   temporary is built under a construction flag in ebx (zeroed at entry) and the bool lives in a frame byte.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_BordelloScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_BordelloScript::WatchForHeroLeavingRegionWithBeer()
{
    for (;;) {
        bool inRegion = Game->IsRegionLoaded("Bordello") && BeerSetToDPad;
        if (!inRegion)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    BeerSetToDPad = false;
    Game->SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, -1);
}