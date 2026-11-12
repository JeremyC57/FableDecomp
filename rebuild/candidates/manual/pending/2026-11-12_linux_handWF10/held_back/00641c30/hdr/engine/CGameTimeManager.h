#ifndef FABLE_ENGINE_CGAMETIMEMANAGER_H
#define FABLE_ENGINE_CGAMETIMEMANAGER_H
#ifdef FABLE_RETAIL_CGAMETIMEMANAGER_FULL
// Opt-in (Codex pass 20, row 0x0065BE6C CTCVillage::UpdateHornTasks): the complete Ego_r.pdb / FableWin.pdb class (members, accessors and layout as the
// pass 20 hand-back modelled it). Without the macro the earlier narrower model below is unchanged.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CWorld;class CMainGameComponent;class CPersistContext;class CCharString;class CGenericVar;
// Layout: complete original Ego37b9b/FW3bb41, native60 from smaller set.
class CGameTimeManager{
public:
 CGameTimeManager(const CGameTimeManager & p0);
 CGameTimeManager(CMainGameComponent & p0, CWorld & p1);
 ~CGameTimeManager();
 void Update(float p0);
 bool IsNightSounds() const;
 bool IsDaySounds() const;
 bool IsNightTime() const;
 bool IsEvening() const;
 bool IsDawn() const;
 bool IsDay() const;
 void AlterDaySpeed(float p0);
 unsigned long GetTimeAs24HourClock() const;
 float GetTimeFrom24HourClock(long p0) const;
 float GetTime() const;
 float GetVillageTime() const;
 long GetDayCount() const;
 float GetDaySpeed() const;
private:
 bool IsTimeBetween(float p0, float p1, float p2) const;
public:
 bool IsTimeBetween(long p0, long p1) const;
 bool IsVillageTimeBetween(long p0, long p1) const;
 bool IsDaybreak() const;
 bool IsNightfall() const;
 void SetTimeOfDay(float p0);
 void SetAsUsingVillageTime(bool p0);
 void FastForwardTimeTo(float p0, float p1);
 void SetTimeAsStopped(bool p0, long & p1);
 bool IsTimeStopped() const;
 void Load(CPersistContext & p0);
 void Load(const CCharString & p0);
 void Save(CPersistContext & p0);
private:
 static void FABLE_FASTCALL ConsoleSetTimeOfDay(CGameTimeManager & p0, const std::vector<CGenericVar,std::allocator<CGenericVar> > & p1);
 static void FABLE_FASTCALL ConsoleSetDaySpeed(CGameTimeManager & p0, const std::vector<CGenericVar,std::allocator<CGenericVar> > & p1);
 static void FABLE_FASTCALL ConsoleSetDayCount(CGameTimeManager & p0, const std::vector<CGenericVar,std::allocator<CGenericVar> > & p1);
 static void FABLE_FASTCALL ConsoleFastForwardTimeTo(CGameTimeManager & p0, const std::vector<CGenericVar,std::allocator<CGenericVar> > & p1);
 CWorld & World;
 const CMainGameComponent & GameComponent;
 float Time;
 float PrevTime;
 float TimeStep;
 long DayCount;
 bool UsingVillageTime;
 float VillageTime;
 bool VillageDay;
 bool FastForwardingToTime;
 float TimeFastForwardingTo;
 float SecondsLeftToFastForwardTime;
 std::set<long,std::less<long>,std::allocator<long> > StopTimeIndexes;
 long NextStopTimeIndex;
};
FABLE_STATIC_ASSERT(sizeof(CGameTimeManager)==60);
#else
// NEW (agentNF10, row 0x0072F170 CTCDummyVillager::SetupFromData): the world's game clock, reached through CWorld::PGameTimeManager
// (+0x1C, engine/CWorld.h GetGameTimeManager / PeekGameTimeManager). Layout: Ego_r.pdb CGameTimeManager (ghidra_out/struct_layouts_egor.tsv,
// 64 bytes): World +0x00, GameComponent +0x04, Time +0x08, PrevTime +0x0C, TimeStep +0x10, DayCount +0x14, UsingVillageTime +0x18,
// VillageTime +0x1C, VillageDay +0x20, FastForwardingToTime +0x21, TimeFastForwardingTo +0x24, SecondsLeftToFastForwardTime +0x28,
// StopTimeIndexes (set<long>) +0x2C, NextStopTimeIndex. Only the members up to DayCount are modelled: the retail size of the STLport
// set (12 bytes, not Ego_r's 16) is not yet proven by a retail access, so nothing past +0x2C is laid out here.
// GetDayCount: Ego_r.pdb / FableWin.pdb ?GetDayCount@CGameTimeManager@@QBEJXZ, inline in retail (0x0072F1E4 `mov ecx,[eax+1Ch] ;
// mov edx,[ecx+14h]` after CTCBase::GetWorld; the landed CTCShop::FrameUpdate 0x00775B40 and CTCVillage::AddThingToVillage 0x0065B854
// read DayCount +0x14 the same way). The landed rows that need more of the class keep their local models.
#ifdef FABLE_RETAIL_CGAMETIMEMANAGER
class CWorld;
class CMainGameComponent;
class CGameTimeManager {
public:
    long GetDayCount() const { return DayCount; }                   // ?GetDayCount@CGameTimeManager@@QBEJXZ (inline)
#ifdef FABLE_RETAIL_CGAMETIMEMANAGER_GETTIME
    // Opt-in (row 0x006414A0 CTCLight::DrawLight): FableWin.pdb ?GetTime@CGameTimeManager@@QBEMXZ, inline in retail
    // (0x006415BC `call PeekWorld ; mov eax,[eax+1Ch] ; mov ecx,[eax+8]`). No layout change.
    float GetTime() const { return Time; }
#endif
#ifdef FABLE_RETAIL_CGAMETIMEMANAGER_DAYSPEED
    // Retail opt-in (row 0x0065AA6F CTCVillage::IsTimeToGoToTavern): FableWin.pdb ?GetDaySpeed@CGameTimeManager@@QBEMXZ (0x01E6B1CF,
    // next to PeekGameTimeManager 0x01E6B1B6), inline in retail (`mov eax,[world+1Ch] ; fdiv dword ptr [eax+10h]`). Pairing with
    // TimeStep +0x10: it is the member the day-speed setter writes (ConsoleSetDaySpeed 0x006BB940, AlterDaySpeed expanded in
    // place: `fld 1.0f-pool ; fdiv [arg+18h] ; fstp [ecx+10h]`) and the only float const getter of the FableWin.pdb member set
    // besides GetTime / GetVillageTime; the body is inferred from those two facts (no out-of-line copy to read). No layout change.
    float GetDaySpeed() const { return TimeStep; }
#endif
private:
    CWorld*             World;                                      // +0x00
    CMainGameComponent* GameComponent;                              // +0x04
    float               Time;                                       // +0x08
    float               PrevTime;                                   // +0x0c
    float               TimeStep;                                   // +0x10
    long                DayCount;                                   // +0x14
};
#else
class CGameTimeManager;
#endif
#endif
#endif
