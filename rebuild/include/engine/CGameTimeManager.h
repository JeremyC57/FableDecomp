#ifndef FABLE_ENGINE_CGAMETIMEMANAGER_H
#define FABLE_ENGINE_CGAMETIMEMANAGER_H
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
