// CMainGameComponent::UpdateForcedTickSpeed() -- forced tick-speed mode: run one world update, then render the frames the desired frame rate asks for inside one tick, sleeping out the rest of each frame's share, and count the ticks that overran
// Grade: DIFFER(600v596) -- 0.941 instruction share; everything after the first 10 instructions matches. Residue: retail computes the two
//   reciprocals as `fld [ebx+38h] ; fld1 ; fdivrp st(1)` and `fild ConstantFPS ; fmul st(1) ; fld1 ; fdivrp st(1)` (0x00417750..0x00417761,
//   the only `fld1 ; fdivrp` pair in Fable.exe), ours `fdivr qword ptr [1.0]` twice (float 1.0f: `fld1 ; fdiv [ebx+38h]`), and retail
//   stores frame_duration (`fst [ebp-0Ch]`) after the tick product, ours before it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00417747, 596 bytes, thiscall, frame pointer (`leave ; ret`), no /GS; size TU (/O1 /Oy- as Update).
// Identity: Ego_r.pdb (0x00416A85) / FableWin.pdb (0x018A5B2F) ?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ; called by Run 0x004189C2.
// Retail-asm: 0x00417798 `fnstcw word ptr [ebp-4]` ... 0x004177AB `fldcw word ptr [ebp-4]` (GFFPUSetDoublePrecision, engine/GFFPUSetDoublePrecision.h)
//   and 0x004177AE `fnstcw word ptr [ebp-4]` + 0x004177B1 `and byte ptr [ebp-3],0F3h` + 0x004177B5 `fldcw` (engine/GFFPUSetRoundingNearest.h).
// Callees: GFGetSystemManager 0x009A4EC0 / CSystemManager::IsApplicationActive 0x009A57B0, GFGetTime 0x009E1BC0, CNetworkClient::GetGameEventPackageSet
//   0x004AEBA0, CWorld::UpdateInputRelatedInterpolation 0x0049D9E0 / GetFrame 0x0049D870 / GetThingManager (slot 3) / RenderUpdate 0x0049E080,
//   GetInputs (slot 6), SignalFrameUpdate 0x00416047, UpdateFromEventPackageSet 0x0041726D, CThing::SetCurrentThingManager 0x004C74F0,
//   CInterpolationInfo() 0x00415A60, CRoughFrameCounter::NewFrame 0x009F8BA0, GetPredictedTimeSinceLastRenderFrame 0x00416258,
//   GetCurrentGameTime 0x00416231, CDisplayEngine::DoRender 0x00435F70 (protected), _ftol2 0x00BFEA70, Sleep (IAT 0x0143FE1C).
// Shape: the reciprocals take double 1.0 (fld [m] first, as retail; 1.0f folds the member into `fdiv [m]`); the tick product is grouped
//   (a * desired) * frame_duration as retail multiplies; the frame loop is a counted for (retail `dec [ebp-8] ; jne`, the frame count in
//   the no_frames slot); the two Sleep clamps are if / else-if on a named long.
// Tried (ATTEMPTS.log): 1.0f, 1.0, 1.0L constants, base /O2 + pragmas, three product spellings.
// Chunk 1023 (0 grades; the row had 3 left): VC7.1 /O1 micro probes (probe/recip/r*.cpp, no grade) for the retail
//   `fld [m] ; fld1 ; fdivrp st(1)` reciprocal: 1.0f, 1.0, 1.0L, float(1), (float)1, (double)1, a static const double ONE, a long
//   double local, a named float local divided in place (t = m; t = 1/t), t = 1.0f; t /= m, inline Div / RecipD / RecipF helpers, a
//   template T(1)/x, pow(m, -1) / pow(double, -1.0) / powf, and /G5 /G6 /G7 /Op /O2 variants. Every one gives `fld1 ; fdiv [m]` or
//   `fdivr [1.0]`. Only a loop-carried x87-held float (the shape of std::_Pow_int's `y` after the loop) produced `fld [m] ; fld1 ;
//   fdivrp`, which no plain spelling of this function has. Next lever: a shared retail-asm reciprocal helper if a second retail site
//   with `fld1 ; fdivrp st(1)` is found (Fable.exe has none other per the residue line), else owner ruling.
// Leaf: ?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ
// Flags: /O1 /Oy-
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_UPDATE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_FORCEDTICK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GAMETIME
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CNETWORKCLIENT_UPDATE
#define FABLE_RETAIL_CNETWORKCLIENT_GAMEEVENTPACKAGESET
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CDISPLAYENGINE_DORENDER
#define FABLE_RETAIL_CTHING_SETCURRENTTHINGMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_APPACTIVE
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "engine/CThing.h"
#include "engine/CSystemManager.h"
#include "engine/GFFPUSetDoublePrecision.h"
#include "engine/GFFPUSetRoundingNearest.h"
#include "engine/GFGetTime.h"
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // IAT 0x0143FE1C
extern long g_frameCounter_013b89a8;                 // 0x013B89A8 (landed spelling, CMainGameComponent::UpdateRegular 0x004162B5)
extern unsigned long g_RenderFrame_013b7d70;         // 0x013B7D70 (landed spelling)
#pragma optimize("s", on)
#pragma optimize("y", off)

void CMainGameComponent::UpdateForcedTickSpeed()
{
    float tick_speed = 1.0 / ForceUpdateTickSpeedMultipler;
    float frame_duration = 1.0 / (GetConstantFPS() * tick_speed);
    float no_frames = (tick_speed * ForceUpdateTickSpeedDesiredFramerate) * frame_duration;
    if (GFGetSystemManager()->IsApplicationActive()) {
        double start_time = GFGetTime();
        g_frameCounter_013b89a8 = 0;
        GFFPUSetDoublePrecision();
        GFFPUSetRoundingNearest();
        if (Client.GetGameEventPackageSet(&EventPackageSet)) {
            PWorld->UpdateInputRelatedInterpolation();
            GetInputs();
            SignalFrameUpdate();
            UpdateFromEventPackageSet(&EventPackageSet);
            NoRenderFramesSinceLastGameUpdate = 0;
            WorldUpdateTurn = true;
        }
        g_RenderFrame_013b7d70 = CWorld::GetFrame();
        double time_taken = GFGetTime() - start_time;
        if (CWorld::GetFrame()) {
            CThing::SetCurrentThingManager(PWorld->GetThingManager());
            float interpolate = 0.0f;
            double time_per_frame = (frame_duration - time_taken) / no_frames;
            long frames = (long)no_frames;
            if (frames < 1)
                frames = 1;
            for (long i = 0; i < frames; ++i) {
                start_time = GFGetTime();
                CInterpolationInfo info;
                RoughFPSCounter.NewFrame();
                info.Last = LastInterpolationInfo.Current;
                info.PausedLast = LastInterpolationInfo.PausedCurrent;
                info.WFServerCurrentTime = CurrentServerFrame;
                info.Current.GTPredictedTimeSinceLastRenderFrame = GetPredictedTimeSinceLastRenderFrame();
                info.Current.WFInterpolate = interpolate;
                info.PausedCurrent = info.Current;
                LastInterpolationInfo = info;
                PWorld->RenderUpdate(&info);
                LastRenderFrameStartTime = GetCurrentGameTime();
                PDisplayEngine->DoRender(&info, WorldUpdateTurn, true);
                interpolate += 1.0f / no_frames;
                ++NoRenderFramesSinceLastGameUpdate;
                LastFrameRenderDuration = GFGetTime() - start_time;
                time_taken += LastFrameRenderDuration;
                double sleep_time = time_per_frame - LastFrameRenderDuration;
                long sleep_ms = (long)(sleep_time * 1000.0);
                if (sleep_ms < 0)
                    sleep_ms = 0;
                else if (sleep_ms > 1000)
                    sleep_ms = 1000;
                Sleep(sleep_ms);
                time_taken += sleep_time;
            }
            if (time_taken > frame_duration)
                ++ForceUpdateNoFailedUpdates;
        }
        long sleep_ms = (long)((frame_duration - time_taken) * 1000.0);
        if (sleep_ms < 0)
            sleep_ms = 0;
        else if (sleep_ms > 1000)
            sleep_ms = 1000;
        Sleep(sleep_ms);
    }
}
