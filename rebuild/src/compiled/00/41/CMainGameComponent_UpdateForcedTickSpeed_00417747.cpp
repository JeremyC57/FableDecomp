#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_COUNTERS_PASS26
// Leaf: ?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ
// Flags: /O1 /Oy-
// Retail-asm-helper: GFFPUSetDoublePrecision, GFFPUSetRoundingNearest (engine/GFFPUSetDoublePrecision.h, engine/GFFPUSetRoundingNearest.h)
// Retail-asm: 0x00417798 `fnstcw word ptr [ebp-4]` + 0x004177AB `fldcw word ptr [ebp-4]` (GFFPUSetDoublePrecision), 0x004177AE `fnstcw word ptr [ebp-4]` + 0x004177B5 `fldcw word ptr [ebp-4]` (GFFPUSetRoundingNearest)
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
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_COUNTERS_PASS26
#include "engine/CThingSearchTools.h"
extern unsigned long GRenderFrame; // Original Ego_d GDATA and public; paired native GetFrame store013b7d70.
#pragma optimize("s", on)
#pragma optimize("y", off)

void CMainGameComponent::UpdateForcedTickSpeed()
{
    const float slow_game_tick_by_factor=ForceUpdateTickSpeedMultipler;
    const float final_simulated_frame_rate=ForceUpdateTickSpeedDesiredFramerate;
    const float simulate_world_tick_hertz=GetConstantFPS()/slow_game_tick_by_factor;
    const float simulate_frame_frate=final_simulated_frame_rate/slow_game_tick_by_factor;
    const float no_renders_this_tick=simulate_frame_frate/simulate_world_tick_hertz;
    const float total_time_ms_this_tick=1.0f/simulate_world_tick_hertz;
    const long max_no_acceptable_failed_updates=40;
    if (GFGetSystemManager()->IsApplicationActive()) {
        double start_world_tick_time = GFGetTime();
        CThingSearchTools::NoLookups = 0;
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
        GRenderFrame = CWorld::GetFrame();
        double delta_world_tick_time = GFGetTime() - start_world_tick_time;
        if (CWorld::GetFrame()) {
            CThing::SetCurrentThingManager(PWorld->GetThingManager());
            float current_interpolate = 0.0f;
            double total_time_remaining_for_rendering=total_time_ms_this_tick-delta_world_tick_time;
            double total_time_allocated_per_render=total_time_remaining_for_rendering/no_renders_this_tick;
            long actual_no_renders_this_turn = (long)no_renders_this_tick;
            if (actual_no_renders_this_turn < 1)
                actual_no_renders_this_turn = 1;
            for (long c1 = 0; c1 < actual_no_renders_this_turn; ++c1) {
                double start_render_time = GFGetTime();
                CInterpolationInfo interpolation_info;
                RoughFPSCounter.NewFrame();
                interpolation_info.Last = LastInterpolationInfo.Current;
                interpolation_info.PausedLast = LastInterpolationInfo.PausedCurrent;
                interpolation_info.WFServerCurrentTime = CurrentServerFrame;
                interpolation_info.Current.GTPredictedTimeSinceLastRenderFrame = GetPredictedTimeSinceLastRenderFrame();
                interpolation_info.Current.WFInterpolate = current_interpolate;
                interpolation_info.PausedCurrent = interpolation_info.Current;
                LastInterpolationInfo = interpolation_info;
                PWorld->RenderUpdate(&interpolation_info);
                LastRenderFrameStartTime = GetCurrentGameTime();
                PDisplayEngine->Render(&interpolation_info, WorldUpdateTurn, true);
                current_interpolate += 1.0f / no_renders_this_tick;
                ++NoRenderFramesSinceLastGameUpdate;
                double delta_render_time=GFGetTime()-start_render_time;
                LastFrameRenderDuration=delta_render_time;
                delta_world_tick_time+=delta_render_time;
                double sleep_in_render_loop_seconds = total_time_allocated_per_render - delta_render_time;
                long sleep_in_render_loop_ms = (long)(sleep_in_render_loop_seconds * 1000.0);
                if (sleep_in_render_loop_ms < 0)
                    sleep_in_render_loop_ms = 0;
                else if (sleep_in_render_loop_ms > 1000)
                    sleep_in_render_loop_ms = 1000;
                Sleep(sleep_in_render_loop_ms);
                delta_world_tick_time += sleep_in_render_loop_seconds;
            }
            if (delta_world_tick_time > total_time_ms_this_tick)
                ++ForceUpdateNoFailedUpdates;
        }
        double sleep_for_seconds=total_time_ms_this_tick-delta_world_tick_time;
        long sleep_for_ms=(long)(sleep_for_seconds*1000.0);
        if (sleep_for_ms < 0)
            sleep_for_ms = 0;
        else if (sleep_for_ms > 1000)
            sleep_for_ms = 1000;
        Sleep(sleep_for_ms);
    }
}