// CWorld::SaveGameStateInternal(const CWideString& pathName, bool b) -- write the save game: a persisted HEADER section (world name, frame, the world's enable flags, save types, markers, region names, total time played), the game-time / ENTITIES / PLAYER / QUESTS / REGIONS / FACTIONS sections into a second stream, then the "FableSav" file (version, size slot, both streams zlib-compressed, padded to 0x4B000, CRC) written to disk
// Leaf: ?SaveGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049F4C0, 2825 bytes, `ret 8`, 8-aligned frame (`and esp,-8`: the double locals), no EH frame,
//   no /GS cookie (no array local; the TU is /GS, LoadGameStateInternal 0x004A21F0 has one, and the row also matches under /Gr /GS).
//   Ego_r.pdb ?SaveGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z (protected); called by SaveGameState 0x004A05C0 / SaveHeroStateInternal
//   0x004A00E0. Flags: /Gr as the TU's matched CWorld::Init 0x004A6E30 (NProgressDisplay::DisplayProgress and CCRC::Calc are __fastcall
//   anyway; the flag changes no byte here).
// Callees: StartAutoSaveProgress 0x0049E7C0 / StopAutoSaveProgress 0x0049E8C0 (Ego_r / FableWin IAE: protected), CDiskFileWin32(path,
//   FILE_ACCESS_CREATE, 2) 0x0099AD80 + inline ~CDiskFileWin32 (Close 0x0099A920), CMemoryDataOutputStream ctor 0x00996AF0 / dtor 0x00996B30 /
//   ReserveSize 0x00996BC0 / Clear 0x00996B60 / GetLength 0x00994B40 / PeekData 0x00995E70 / GetPosition 0x009945B0 / SetPosition 0x00994B10 /
//   Write 0x00996CC0, CDataOutputStream::WriteString 0x009959F0 / WriteULONG 0x00993EA0 / WriteZlibCompressed 0x009967E0 / WriteRepeatedByte
//   0x00994230, CPersistContext::Init(CDataOutputStream&, bool) 0x009BAA50 / StartSection 0x009BAB60 / EndSection 0x009BABF0, Transfer<T>
//   (bool 0x004045C0, long 0x00410BE0, unsigned long 0x004106F0, CCharString 0x004109A0, CWideString 0x00410AC0, ESaveType 0x004AB640,
//   C3DVector 0x004859AD, float 0x00410620, double 0x004AB710), NProgressDisplay::DisplayProgress 0x009E9F40, GFGetTime 0x009E1BC0,
//   CPlayerManager::GetMainPlayer 0x00449970 / SaveGameState 0x00449F90, CPlayer::PeekPControlledCreature 0x00487DD0, the const
//   CVectorMap::LowerBound 0x004365B0 (CThing::PeekTC expanded), CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion const 0x004FC180 /
//   SaveGameState 0x004FDE80, CRegion::GetDisplayName 0x006BBF20, CGameTimeManager::Save 0x006BBB30, CThingManager::SaveGameState 0x00524290,
//   CQuestManager::SaveGameState 0x004B64E0 (on the singleton 0x013B89FC), CFactionManager::SaveGameState 0x006B9560, CCRC::Calc 0x004014A0,
//   CAFile::IsRoomForFile 0x00999AB0, CDiskFileWin32::Write 0x0099AA90.
// Shape: the two-argument CPersistContext::Transfer(name, value) = Transfer(name, value, GetDefaultVal<T>()) with GetDefaultVal returning a
//   CPersistTraits<T> object's DefaultVal (CCharString / CWideString defaults: construct, copy, destroy; scalars a re-stored 0); the
//   write section and context are function-scope locals reused for every section; `StartTime` copied into a named double before
//   GFGetTime (retail spills it: `fld [esi+180h] ; fstp [esp+18h]` ahead of the call) and the new total stored to the local first
//   (`fst [esp+80h] ; fstp [esi+188h]`); the in-game menu pointer of PeekTC is left uninitialised (retail reads the dead frame word on
//   the miss path, 0x0049F9B1). The file-assembly tail is its own block: that puts the CRC word on the shared temporary slot [esp+18h]
//   (at function scope it gets its own word +0x88, in a CRC-only block +0x10, in a block from the size-slot rewrite +0x34).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_CWORLD_SAVEGAMESTATEINTERNAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_WRITESECTIONS
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_ZLIB
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE_ISROOMFORFILE_B
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_SAVEGAMESTATE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CTHINGMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE_B
#define FABLE_RETAIL_CFACTIONMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_PAUSEDTIME
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorld.h"
#include "engine/NProgressDisplay.h"
#include "engine/GFGetTime.h"
#include "engine/CCRC.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContextWriteSection.h"
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CGameTimeManager.h"
#include "engine/CThingManager.h"
#include "engine/CQuestManager.h"
#include "engine/CFactionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInGameMenu.h"

class CThingPlayerCreature : public CThing { };

template<class T> inline void CPersistContext::Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { CPersistTraits<T> traits; return traits.DefaultVal; }
template<> inline C3DVector FABLE_FASTCALL CPersistContext::GetDefaultVal<C3DVector>() { return C3DVector(0.0f, 0.0f, 0.0f); }

extern bool g_SavingQuestScreenState;                   // 0x013B89D0 (descriptive, as the landed SaveGameState 0x004A05C0)
extern bool g_SavingHeroState;                          // 0x013B89D1

bool CWorld::SaveGameStateInternal(const CWideString& pathName, bool b)
{
    if (b && !g_SavingHeroState)
        StartAutoSaveProgress();

    CDiskFileWin32 file(pathName, FILE_ACCESS_CREATE, 2);
    CMemoryDataOutputStream save_stream;
    save_stream.ReserveSize(0x4b000);
    CCharString error;
    CMemoryDataOutputStream header_stream;
    CMemoryDataOutputStream body_stream;
    CPersistContextWriteSection section;
    CPersistContext context;
    context.Init(header_stream, true);
    header_stream.ReserveSize(0x400);
    body_stream.ReserveSize(0x8000);

    NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    context.StartSection(section, "HEADER");
    context.Transfer("WorldName", CurrentWorldName);
    context.Transfer("WorldFrame", Frame);
    if (!SerialisingHeroState) {
        context.Transfer("TeleportingEnabled", TeleportingEnabled);
        context.Transfer("SavingEnabled", SavingEnabled);
        context.Transfer("ExperienceSpendingEnabled", ExperienceSpendingEnabled);
        context.Transfer("CreatureGenerationEnabled", CreatureGenerationEnabled);
        context.Transfer("CreatureGenerationDisabledGroups", CreatureGenerationDisabledGroups);
        context.Transfer("HeroSleepingEnabled", HeroSleepingEnabled);
        context.Transfer("MapTableShowQuestCardsOnUsed", MapTableShowQuestCardsOnUsed);
        context.Transfer("MiniMapEnabled", MiniMapEnabled);
        context.Transfer("MiniMapActiveBeforeDisabled", MiniMapActiveBeforeDisabled);
        context.Transfer("GuildMasterMessagesEnabled", GuildMasterMessagesEnabled);
        context.Transfer("SummonerDeathExplosionAffectsHero", SummonerDeathExplosionAffectsHero);
        context.Transfer("MostRecentSaveType", MostRecentSaveType);
        context.Transfer("MostRecentSaveTypeBeforeManualSave", MostRecentSaveTypeBeforeManualSave);
        context.Transfer("MostRecentManualSaveName", MostRecentManualSaveName);
        context.Transfer("SaveGameMarkerPos", SaveGameMarkerPos);
        context.Transfer("SaveGameMarkerAngleXY", SaveGameMarkerAngleXY);
        context.Transfer("GuildSealRecallPos", GuildSealRecallPos);
        context.Transfer("GuildSealRecallAngleXY", GuildSealRecallAngleXY);

        const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion());
        CCharString current_region_name = region.GetDisplayName();
        context.Transfer("CurrentRegionName", current_region_name);
        CCharString current_region_minimap_graphic_name = region.GetMiniMapGraphic();
        context.Transfer("CurrentRegionMinimapGraphicName", current_region_minimap_graphic_name);

        double start_time = StartTime;
        double total_time_played = GFGetTime() - start_time + TimePlayed;
        TimePlayed = total_time_played;
        const CTCInGameMenu* in_game_menu;
        PlayerManager->GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_IN_GAME_MENU, &in_game_menu);
        total_time_played -= in_game_menu->GetAccumulativePausedTime();
        if (total_time_played < 0.0)
            total_time_played = 0.0;
        context.Transfer("TotalTimePlayed", total_time_played);
    }
    context.EndSection(section);
    NProgressDisplay::DisplayProgress("", -1.0f, false, false);

    context.Init(body_stream, true);
    if (!SerialisingHeroState)
        PGameTimeManager->Save(context);
    NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    context.StartSection(section, "ENTITIES");
    GetThingManager().SaveGameState(context);
    context.EndSection(section);
    NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    if (!SerialisingHeroState) {
        context.StartSection(section, "PLAYER");
        PlayerManager->SaveGameState(context);
        context.EndSection(section);
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        context.StartSection(section, "QUESTS");
        CQuestManager::Get().SaveGameState(context);
        context.EndSection(section);
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        context.StartSection(section, "REGIONS");
        PWorldMap->SaveGameState(context);
        context.EndSection(section);
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        context.StartSection(section, "FACTIONS");
        PFactionManager->SaveGameState(context);
        context.EndSection(section);
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    }

    {
        unsigned long header_length = header_stream.GetLength();
        unsigned long body_length = body_stream.GetLength();
        save_stream.WriteString("FableSav");
        save_stream.WriteULONG(0x2165);
        unsigned long size_position = save_stream.GetPosition();
        save_stream.WriteULONG(0);
        save_stream.WriteULONG(header_length);
        save_stream.WriteULONG(body_length);
        save_stream.WriteZlibCompressed(header_stream.PeekData(), header_length, 0x1000);
        save_stream.WriteZlibCompressed(body_stream.PeekData(), body_length, 0x1000);
        unsigned long data_size = save_stream.GetPosition();
        body_stream.Clear();
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        save_stream.SetPosition(size_position);
        save_stream.WriteULONG(data_size);
        save_stream.SetPosition(data_size);
        if (data_size < 0x4b000)
            save_stream.WriteRepeatedByte(0, 0x4b000 - data_size);
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        unsigned long crc = CCRC::Calc(0, save_stream.PeekData(), data_size);
        save_stream.SetPosition(data_size);
        save_stream.Write(&crc, 4);
        if (CAFile::IsRoomForFile(save_stream.GetLength(), pathName)) {
            NProgressDisplay::DisplayProgress("", -1.0f, false, false);
            file.Write(save_stream.PeekData(), save_stream.GetLength(), 0);
            NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        }
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
        file.Close();
    }

    if (b && !g_SavingQuestScreenState)
        StopAutoSaveProgress();
    return true;
}