// NScript::Cbandit_level_cutsceneScript::Main() -- the bandit-camp arrival cut scene: while the thread runs wait for the "bandits" region, bind welcome_guy, post the entities, then under a paused movie fly the camera through SPLINE_01..05 (preload on while the five are set up), wait out SPLINE_04, SPLINE_03 and SPLINE_01, reset the camera behind the hero and pause 3 s; then wait for the region to unload
// Inline-budget: /d2inlT36 (owner rule 2026-11-08, preference 2; plain C++ otherwise; the landed 0x00D6C830 / 0x00D8A240 precedent) -- the
//   implicit ~CScriptGameResourceObjectMovieBase (speech_movie) return sites. Retail expands it (call 0x006E7AB0 ; GROBase vtable store ;
//   call 0x0099A430) at the SPLINE_04 wait return, the post-SPLINE_04 check and the SPLINE_03 wait return (0x00EFA9D0 / 0x00EFA9DD) and
//   calls the COMDAT 0x006E7B80 at the post-SPLINE_03 check, the SPLINE_01 wait return, the post-SPLINE_01 check and the scope end.
//   c2 trace (tools/trace.sh): the seven dtor sites sit at f 0.625 / 0.3125 / 0.156 / 0.078 / 0.039 / 0.0195 / 0.0195. At the default T74
//   the nested ~CCountedPointer<MovieBase> (c 50, f 1.0, score 50) expands inside, the dtor costs 60 and only the first site expands
//   (score 65): DIFFER(1389v1362). Below T51 the nested ~CCountedPointer stays a call (retail's 0x006E7AB0 in the expanded copies), the
//   dtor costs 19 and the sites score 20 / 24 / 33 / 51 / 85 / 155 / 155, so the first three expand. Measured: T35 DIFFER(1346v1362)
//   (the CEntityScriptBinding ctor, score 35, called), T36 / T38 / T40 / T43 / T46 / T48 / T50 RELOCATION_MATCH, T51 / T52
//   DIFFER(1300v1362), /d2inlS10 DIFFER(1365v1362), S15 / S20 DIFFER(1318v1362); smallest N = 36. No header inline opt-in and no
//   declaration-only ~CCountedPointer specialization: each was byte-neutral under the flag and is dropped (README ablation).
// Leaf: ?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ
// Flags: /d2inlT36
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA4F0, 1362 bytes, thiscall, plain `ret`, no EH frame; ebx / edi saved inside the loop path.
// Identity (proven): slot 2 (Main) of the script vtable 0x012F69E8 (stored by the landed ctor 0x00EFA420 / Alloc 0x00EFAF30, which
//   RegisterScripts registers as "bandit_level_cutscene"); Ego_r.pdb name set ?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ. The
//   bound Alloc is the landed Cwelcome_guy::Alloc 0x00EFAA60 (binding-name literal "welcome_guy").
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseObject 0x0099A380 / ~ 0x0099A430 (the movie ctor expanded; its dtor
//   expanded at the first three exits around CCountedPointer<MovieBase>::~CCountedPointer 0x006E7AB0, called 0x006E7B80 elsewhere);
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 370 StartMovieSequence,
//   376 Pause, 379 PauseAllNonScriptedEntities (the inline CWideScreenMagicPauseEntities), 403 CameraSetCameraPreloadFlag,
//   412 CameraUseCameraPoint, 416 CameraDefault, 417 CameraResetToViewBehindHero, 418 IsCameraInScriptedMode.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"

namespace NScript {
class Cjoss_test_scriptsData;
// The class as the landed ctor 0x00EFA420 / Alloc 0x00EFAF30 / Cwelcome_guy::Alloc 0x00EFAA60 model it (no engine header exists):
//   no members of its own (Alloc `push 48h` = the CLevelScriptBase size); script vtable 0x012F69E8.
class Cbandit_level_cutsceneScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    virtual void RegisterMain();                                         // 0x00EFA450
    virtual void Main();                                                 // 0x00EFA4F0
    class Cwelcome_guy : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(Cbandit_level_cutsceneScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00EFAA60
    };
};
}

// Locals = Ego_d.pdb NScript::Cbandit_level_cutsceneScript::Main (debug length 2264): _Fail_Val (TRetVoid); loop-body block: use_preload;
//   inner block: speech_movie, PauseMyEntitiesPlease.
// Inferred: use_preload -- Ego_d's loop-body local, its `= true` (line 214, 4 B) and `if (use_preload)` (line 219, 12 B) calibrate
//   exactly against VC7.1 /Od (egod_lines/); retail folds both, byte-neutral (README). The rest of Ego_d's block is a later revision
//   (other line sizes differ), so the guarded statements are retail's, not Ego_d's.
// Inferred: loop form -- `for (;; Game->NewScriptFrame())` as the chunk 813/828 Mains. `while (true)` with the frame call last gives
//   DIFFER(1318v1362) at T40, and no T splits it: its three post-wait checks share f 0.1429 (scores 35 / 111), so post-SPLINE_04 and
//   post-SPLINE_03 always flip together.
void NScript::Cbandit_level_cutsceneScript::Main()
{
    TRetVoid _Fail_Val;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        while (!Game->IsRegionLoaded("bandits")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        AddEntityScriptBinding(new CEntityScriptBinding<Cbandit_level_cutsceneScript>("welcome_guy", this, &Cwelcome_guy::Alloc, 0));
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        bool use_preload = true;
        {
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (use_preload) {
            Game->CameraSetCameraPreloadFlag(true);
            Game->CameraUseCameraPoint("SPLINE_01", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_02", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_03", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_04", 0, 0, -1.0f, 0, -1);
            Game->CameraUseCameraPoint("SPLINE_05", 0, 0, -1.0f, 0, -1);
            Game->CameraSetCameraPreloadFlag(false);
            }
            Game->CameraUseCameraPoint("SPLINE_04", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->CameraUseCameraPoint("SPLINE_03", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->CameraUseCameraPoint("SPLINE_01", 0, 0, -1.0f, 0, -1);
            while (Game->IsCameraInScriptedMode()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->CameraResetToViewBehindHero(0.0f);
            Game->CameraDefault();
            Game->Pause(3.0f);
        }
        while (Game->IsRegionLoaded("bandits") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
}