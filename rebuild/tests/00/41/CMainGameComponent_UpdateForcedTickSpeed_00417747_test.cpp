#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_COUNTERS_PASS26
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
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_COUNTERS_PASS26
#include "engine/CThingSearchTools.h"
extern unsigned long GRenderFrame; // Original Ego_d GDATA and public; paired native GetFrame store013b7d70.
#pragma optimize("s", on)
#pragma optimize("y", off)



#include <cstdio>
#include <cstring>
#include <cmath>
static int failures=0,time_calls=0,sleeps=0,inputs=0,signals=0,packages=0;static double clock_value=1,step=.001;static unsigned long sleep_values[10];static bool active=true;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
unsigned long CThingSearchTools::NoLookups=99,CThingSearchTools::NoThingsScanned=0,CThingSearchTools::SearchID=0;unsigned long GRenderFrame=77;
static double FABLE_FASTCALL FixtureTime(){++time_calls;double v=clock_value;clock_value+=step;return v;}
static void FixtureSleep(unsigned long ms){CHECK(sleeps<10);sleep_values[sleeps++]=ms;}
struct FixtureSystem{bool IsApplicationActive(){return active;}};static FixtureSystem tick_system;static FixtureSystem* FABLE_FASTCALL FixtureSystemGetter(){return&tick_system;}
CInterpolationInfo::CInterpolationInfo(){std::memset(this,0,sizeof(*this));}
struct FixtureWorld{static long frame;int updates,renders;CInterpolationInfo infos[10];FixtureWorld():updates(0),renders(0){}void UpdateInputRelatedInterpolation(){++updates;}static long FABLE_FASTCALL GetFrame(){return frame;}int&GetThingManager(){static int m;return m;}void RenderUpdate(const CInterpolationInfo*p){CHECK(renders<10);infos[renders++]=*p;}};long FixtureWorld::frame=1;
struct FixtureThing{static void FABLE_FASTCALL SetCurrentThingManager(int&){}};
struct FixtureClient{bool present;int queries;FixtureClient():present(false),queries(0){}bool GetGameEventPackageSet(int*){++queries;return present;}};
struct FixtureRenderer{int renders;bool expected_update;FixtureRenderer():renders(0),expected_update(false){}void Render(const CInterpolationInfo*,bool update,bool yes){CHECK(update==expected_update&&yes);++renders;}};
struct FixtureRoughCounter{int frames;FixtureRoughCounter():frames(0){}void NewFrame(){++frames;}};
struct FixtureMain{float ForceUpdateTickSpeedMultipler,ForceUpdateTickSpeedDesiredFramerate;long ForceUpdateNoFailedUpdates,CurrentServerFrame;unsigned long NoRenderFramesSinceLastGameUpdate;bool WorldUpdateTurn;double LastFrameRenderDuration,LastRenderFrameStartTime;CInterpolationInfo LastInterpolationInfo;FixtureClient Client;FixtureWorld*PWorld;FixtureRenderer*PDisplayEngine;FixtureRoughCounter RoughFPSCounter;int EventPackageSet;FixtureMain(FixtureWorld&w,FixtureRenderer&r):ForceUpdateTickSpeedMultipler(1),ForceUpdateTickSpeedDesiredFramerate(60),ForceUpdateNoFailedUpdates(0),CurrentServerFrame(45),NoRenderFramesSinceLastGameUpdate(7),WorldUpdateTurn(false),LastFrameRenderDuration(-1),LastRenderFrameStartTime(-1),PWorld(&w),PDisplayEngine(&r){LastInterpolationInfo.Current.WFInterpolate=.8f;LastInterpolationInfo.PausedCurrent.WFInterpolate=.7f;}static long FABLE_FASTCALL GetConstantFPS(){return 30;}void GetInputs(){++inputs;}void SignalFrameUpdate(){++signals;}void UpdateFromEventPackageSet(const int*){++packages;}double GetPredictedTimeSinceLastRenderFrame()const{return .25;}double GetCurrentGameTime()const{return 123;}void UpdateForcedTickSpeed();};
#define CMainGameComponent FixtureMain
#define CWorld FixtureWorld
#define CThing FixtureThing
#define GFGetSystemManager FixtureSystemGetter
#define GFGetTime FixtureTime
#define Sleep FixtureSleep
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

#undef CMainGameComponent
#undef CWorld
#undef CThing
#undef GFGetSystemManager
#undef GFGetTime
#undef Sleep
int main(){for(int mode=0;mode<7;++mode){FixtureWorld world;FixtureRenderer render;FixtureMain main(world,render);active=mode!=0;FixtureWorld::frame=mode==2||mode==6?0:1;main.Client.present=mode==3;render.expected_update=mode==3;step=mode==4?1:.001;clock_value=1;time_calls=sleeps=inputs=signals=packages=0;CThingSearchTools::NoLookups=99;GRenderFrame=77;if(mode==5)main.ForceUpdateTickSpeedDesiredFramerate=10;if(mode==6)main.ForceUpdateTickSpeedMultipler=1000;main.UpdateForcedTickSpeed();
 if(!active){CHECK(time_calls==0&&sleeps==0&&main.Client.queries==0&&CThingSearchTools::NoLookups==99&&GRenderFrame==77);continue;}
 CHECK(main.Client.queries==1&&CThingSearchTools::NoLookups==0&&GRenderFrame==(unsigned long)FixtureWorld::frame);CHECK(inputs==(mode==3?1:0)&&signals==inputs&&packages==inputs&&world.updates==inputs);int n=FixtureWorld::frame?(mode==5?1:2):0;CHECK(world.renders==n&&render.renders==n&&main.RoughFPSCounter.frames==n&&sleeps==n+1&&time_calls==2+2*n);CHECK(main.NoRenderFramesSinceLastGameUpdate==(unsigned long)((mode==3?0:7)+n));CHECK(main.WorldUpdateTurn==(mode==3));for(int i=0;i<n;++i){CHECK(world.infos[i].WFServerCurrentTime==45&&world.infos[i].Current.GTPredictedTimeSinceLastRenderFrame==.25f);CHECK(world.infos[i].Current.WFInterpolate==(i? .5f:0));CHECK(world.infos[i].PausedCurrent.WFInterpolate==world.infos[i].Current.WFInterpolate);CHECK(world.infos[i].Last.WFInterpolate==(i?0:.8f));}for(int i=0;i<sleeps;++i)CHECK(sleep_values[i]<=1000);if(mode==6)CHECK(sleep_values[0]==1000);if(mode==5)CHECK(main.ForceUpdateNoFailedUpdates==1);if(mode==4){CHECK(main.ForceUpdateNoFailedUpdates==0);for(int i=0;i<sleeps;++i)CHECK(sleep_values[i]==0);}if(n)CHECK(main.LastRenderFrameStartTime==123&&std::fabs(main.LastFrameRenderDuration-step)<.000001);}
 std::puts(failures?"00417747_TEST FAIL":"00417747_TEST PASS");return failures?1:0;}