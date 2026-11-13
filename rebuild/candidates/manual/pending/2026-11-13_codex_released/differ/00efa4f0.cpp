// NScript::Cbandit_level_cutsceneScript::Main() -- the bandit-camp arrival cut scene: while the thread runs wait for the "bandits" region, bind welcome_guy, post the entities, then under a paused movie fly the camera through SPLINE_01..05 (preload on for the first four), wait out SPLINE_04, SPLINE_03 and SPLINE_01, reset the camera behind the hero and pause 3 s; then wait for the region to unload
// Grade: DIFFER(1365v1362) -- per-site MovieBase dtor inline split (33 lines): with the chunk 813/828 `for (;; NewScriptFrame()) { if (T) return; ... }` outer loop ours expands the implicit movie dtor (call 0x006E7AB0; GROBase vtable; call 0x0099A430) at loop1-return / post-loop1 / loop2-return AND post-loop2, retail only at the first three; loop3-return / post-loop3 / scope end are COMDAT 0x006E7B80 calls in both
// Next idea: /d2inlS12-20 and /d2inlT40-75 do not move it (T80+ is a cliff), so it is frequency, not budget; try cooling the post-SPLINE_03 terminate check (an extra folded condition before it) or heating the first two waits with do{}while(0) as chunk 834 did
// Leaf: ?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA4F0, 1362 bytes, thiscall, plain `ret`, no EH frame; ebx / edi saved inside the loop path.
// Identity (proven): slot 2 (Main) of the script vtable 0x012F69E8 (stored by the landed ctor 0x00EFA420 / Alloc 0x00EFAF30, which
//   RegisterScripts registers as "bandit_level_cutscene"); Ego_r.pdb name set ?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ. The
//   bound Alloc is the landed Cwelcome_guy::Alloc 0x00EFAA60 (binding-name literal "welcome_guy").
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseObject 0x0099A380 / ~ 0x0099A430 (the movie ctor expanded; its dtor
//   expanded at the first two exits around CCountedPointer<MovieBase>::~CCountedPointer 0x006E7AB0, called 0x006E7B80 elsewhere);
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 370 StartMovieSequence,
//   376 Pause, 379 PauseAllNonScriptedEntities (the inline CWideScreenMagicPauseEntities), 403 CameraSetCameraPreloadFlag,
//   412 CameraUseCameraPoint, 416 CameraDefault, 417 CameraResetToViewBehindHero, 418 IsCameraInScriptedMode.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/Cbandit_level_cutsceneScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"

namespace NScript {
class Cbandit_level_cutsceneScript::Cwelcome_guy : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(Cbandit_level_cutsceneScript&, CScriptDataBase*, const CScriptThing&);   // 0x00EFAA60
};
}

void NScript::Cbandit_level_cutsceneScript::Main()
{
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("bandits")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        AddEntityScriptBinding(new CEntityScriptBinding<Cbandit_level_cutsceneScript>("welcome_guy", this, &Cwelcome_guy::Alloc, 0));
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->CameraSetCameraPreloadFlag(true);
            Game->CameraUseCameraPoint("SPLINE_01", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_02", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_03", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_04", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_05", 0, 0, -1.0f, 0, -1);
            Game->CameraSetCameraPreloadFlag(false);
            Game->CameraUseCameraPoint("SPLINE_04", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CameraUseCameraPoint("SPLINE_03", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CameraUseCameraPoint("SPLINE_01", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CameraResetToViewBehindHero(0.0f);
            Game->CameraDefault();
            Game->Pause(3.0f);
        }
        while (Game->IsRegionLoaded("bandits") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
}
