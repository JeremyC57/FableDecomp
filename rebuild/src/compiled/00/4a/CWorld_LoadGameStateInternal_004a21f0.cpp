// CWorld::LoadGameStateInternal(const CWideString& pathName, bool b) -- load a save game: verify the file's signature, read it either as a binary "FableSav" file (two zlib-compressed streams into one buffer, persisted through a memory stream) or as text, restore the HEADER section (world flags, save types, markers, total time played) and load the world's level, then the game time and the ENTITIES / PLAYER / QUESTS / REGIONS / FACTIONS sections, restart the play clock, clear the in-game menu's paused time, activate the world and delete a stale region-leaving autosave
// Leaf: ?LoadGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z
// Flags: /Gr /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A21F0, 2931 bytes, `ret 8`, /GS cookie (the `char header[8]` array), no EH frame. Ego_r.pdb
//   ?LoadGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z (protected); callers LoadGameState 0x004A3200, LoadHeroStateInternal 0x004A2D70.
//   Flags: /GS for the cookie (as the TU's landed LoadGameState 0x004A3200), /Gr as the TU's CWorld::Init 0x004A6E30 (no byte depends on it here).
// Callees: CUserProfileManager::VerifySignature 0x00409730 / Get 0x0040D2A0 / GetAutoSaveRegionLeavingPathName 0x004080B0,
//   NProgressDisplay::StartProgress 0x009E9EF0, PrepareForLoad 0x004A0220, GFGetTime 0x009E1BC0, CDiskFileWin32(path, FILE_ACCESS_READ, 2)
//   0x0099AD80 / Read 0x0099A960 / SetPosition 0x0099ABF0 / Close 0x0099A920 + the inline destructor, CQuestManager::DeactivateAllQuests
//   0x004B4450 / LoadGameState 0x004B6380, CMemoryDataInputStream (CDataInputStream ctor 0x00993B20, OpenMemorySource 0x00994570, ~CDataStream
//   0x00993B10), CStringParser (CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, list dtor 0x004046B0, Init(const char*, ...) 0x009B9530),
//   CFileDataInputStream(CAFile&, 0x4000) 0x00994700 / dtor 0x00994780 (inline ReadULONG with ReadWithSrcChunkOverflow 0x00993CA0,
//   ReadZlibCompressed 0x00996230), vector<unsigned char>::_M_fill_insert 0x00485F3A (CArray resize), malloc / free,
//   CCharString::LoadFromFile 0x0099F2E0 / operator= 0x0099EFB0 / ToWideString 0x0099E480, CPersistContext::Init(CMemoryDataInputStream&, bool)
//   0x009BAA30 / Init(CStringParser&, ...) 0x009BA9D0 / ReadSection 0x009BAE70, CPersistContextReadSection ctor 0x00485CD2 / dtor 0x00485CF1,
//   Transfer<CCharString>(name, value) 0x00411010, Transfer<CWideString>(name, value) 0x00411060, Transfer<C3DVector>(name, value) 0x00485B44,
//   Transfer<T>(name, value, default) for bool / long / unsigned long / ESaveType / float / double, NResourceDirectoryNames::GetLevelDir
//   0x0041A060, operator+(CWideString, CWideString) 0x0099BE70 / (CWideString, L".wld") 0x0099BF30, Load 0x004A1840, CGameTimeManager::Load
//   0x006BBAC0, CThingManager / CPlayerManager / CWorldMap / CFactionManager::LoadGameState 0x005253F0 / 0x00449E60 / 0x005079C0 / 0x006B9610,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CVectorMap::LowerBound 0x0040F020 (GetTC),
//   ActivateWorld 0x0049F180, CAFile::PathExists 0x00999230, CDriveManager::DeleteFileW 0x009E4BC0, __security_check_cookie.
// Shape: the bool defaults are explicit (true for the enable flags, false for the guild-master / summoner flags), the save types default to
//   2; the string, wide-string and vector members go through the two-argument Transfer, which this caller calls out of line. Both
//   loaders feed one CPersistContext; the sections are read through one CPersistContextReadSection and its own context. The tail is an
//   early `if (SerialisingHeroState) return true;` before the clock / menu / autosave work: the extra `return true` copy is cross-jumped
//   into the final one (no code), but its destructor references raise the file's and the parser's frame-packer weights, which is what
//   gives retail's frame (0x170 bytes: the autosave string in its own word +0x5C, file +0x64, parser +0x80, the level-path temporaries
//   +0xA8 / +0xAC after the parser); written as an `if (!SerialisingHeroState) { ... }` block the frame is 0x16C and 107 words differ.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_CWORLD_LOADGAMESTATEINTERNAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_DEFAULTCTOR
#define FABLE_RETAIL_CSTRINGPARSER_INIT_STRING_INLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDRIVEMANAGER
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADGAMESTATE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE_CONTEXT
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE_CONTEXT
#define FABLE_RETAIL_CFACTIONMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_PAUSEDTIME
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVEPATHS
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES_LEVELDIR
#define FABLE_RETAIL_NPROGRESSDISPLAY_STARTPROGRESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorld.h"
#include "engine/NProgressDisplay.h"
#include "engine/GFGetTime.h"
#include "engine/CArray.h"
#include "engine/CPersistContext.h"
#include "engine/CDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContextReadSection.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CDriveManager.h"
#include "engine/CWorldMap.h"
#include "engine/CGameTimeManager.h"
#include "engine/CThingManager.h"
#include "engine/CQuestManager.h"
#include "engine/CFactionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInGameMenu.h"
#include "fable_user_profile.h"
#include "fable_resource_directories.h"
#include "engine/CThingPlayerCreature.h"


template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { CPersistTraits<T> traits; return traits.DefaultVal; }
template<> inline C3DVector FABLE_FASTCALL CPersistContext::GetDefaultVal<C3DVector>() { return C3DVector(0.0f, 0.0f, 0.0f); }

bool CWorld::LoadGameStateInternal(const CWideString& pathName, bool b)
{
    if (!CUserProfileManager::VerifySignature(pathName))
        return false;
    if (b) {
        NProgressDisplay::StartProgress(true, false, false, false, false, false);
        PrepareForLoad();
    }
    GFGetTime();
    double total_time_played = 0.0;
    AutoSaveLocked = true;
    CDiskFileWin32 file(pathName, FILE_ACCESS_READ, 2);
    if (!SerialisingHeroState)
        CQuestManager::Get().DeactivateAllQuests();

    CArray<unsigned char> data;
    CMemoryDataInputStream memory_stream;
    CCharString text;
    CStringParser parser;
    CPersistContext context;
    char header[8];
    file.Read(header, 8, 0);
    if (memcmp(header, "FableSav", 8) == 0) {
        CFileDataInputStream file_stream(file, 0x4000);
        unsigned long version = file_stream.ReadULONG();
        unsigned long file_size = file_stream.ReadULONG();
        unsigned long header_size = file_stream.ReadULONG();
        unsigned long body_size = file_stream.ReadULONG();
        __int64 total_size = header_size + body_size;
        if (total_size < 0 || total_size > 0x7fffffff)
            return false;
        data.resize((unsigned long)total_size);
        file_stream.ReadZlibCompressed(data.GetAsCArray(), header_size);
        file_stream.ReadZlibCompressed(data.GetAsCArray() + header_size, body_size);
        memory_stream.OpenMemorySource(data.GetAsCArray(), (unsigned long)total_size);
        context.Init(memory_stream, true);
    } else {
        file.SetPosition(0);
        text.LoadFromFile(file, 0, -1);
        parser.Init(text, 0, CCharString(""), 0);
        context.Init(parser, 0, true, 0, 0, 0);
    }

    CPersistContextReadSection section;
    if (context.ReadSection(section, "HEADER")) {
        CCharString world_name;
        section.Context.Transfer("WorldName", world_name);
        section.Context.Transfer("WorldFrame", Frame, 0L);
        if (!SerialisingHeroState) {
            section.Context.Transfer("TeleportingEnabled", TeleportingEnabled, true);
            section.Context.Transfer("SavingEnabled", SavingEnabled, true);
            section.Context.Transfer("ExperienceSpendingEnabled", ExperienceSpendingEnabled, true);
            section.Context.Transfer("CreatureGenerationEnabled", CreatureGenerationEnabled, true);
            section.Context.Transfer("CreatureGenerationDisabledGroups", CreatureGenerationDisabledGroups, 0UL);
            section.Context.Transfer("HeroSleepingEnabled", HeroSleepingEnabled, true);
            section.Context.Transfer("MapTableShowQuestCardsOnUsed", MapTableShowQuestCardsOnUsed, true);
            section.Context.Transfer("MiniMapEnabled", MiniMapEnabled, true);
            section.Context.Transfer("MiniMapActiveBeforeDisabled", MiniMapActiveBeforeDisabled, true);
            section.Context.Transfer("GuildMasterMessagesEnabled", GuildMasterMessagesEnabled, false);
            section.Context.Transfer("SummonerDeathExplosionAffectsHero", SummonerDeathExplosionAffectsHero, false);
            section.Context.Transfer("MostRecentSaveType", MostRecentSaveType, (ESaveType)2);
            section.Context.Transfer("MostRecentSaveTypeBeforeManualSave", MostRecentSaveTypeBeforeManualSave, (ESaveType)2);
            section.Context.Transfer("MostRecentManualSaveName", MostRecentManualSaveName);
            section.Context.Transfer("SaveGameMarkerPos", SaveGameMarkerPos);
            section.Context.Transfer("SaveGameMarkerAngleXY", SaveGameMarkerAngleXY, 0.0f);
            section.Context.Transfer("GuildSealRecallPos", GuildSealRecallPos);
            section.Context.Transfer("GuildSealRecallAngleXY", GuildSealRecallAngleXY, 0.0f);
            CCharString current_region_name;
            CCharString current_region_minimap_graphic_name;
            section.Context.Transfer("CurrentRegionName", current_region_name);
            section.Context.Transfer("CurrentRegionMinimapGraphicName", current_region_minimap_graphic_name);
            section.Context.Transfer("TotalTimePlayed", total_time_played, 0.0);
            Load(NResourceDirectoryNames::GetLevelDir() + world_name.ToWideString() + L".wld");
        }
    }
    if (!SerialisingHeroState)
        PGameTimeManager->Load(context);
    if (context.ReadSection(section, "ENTITIES"))
        GetThingManager().LoadGameState(section.Context);
    if (!SerialisingHeroState) {
        if (context.ReadSection(section, "PLAYER"))
            PlayerManager->LoadGameState(section.Context);
        if (context.ReadSection(section, "QUESTS"))
            CQuestManager::Get().LoadGameState(section.Context);
        if (context.ReadSection(section, "REGIONS"))
            PWorldMap->LoadGameState(section.Context);
        if (context.ReadSection(section, "FACTIONS"))
            PFactionManager->LoadGameState(section.Context);
    }
    file.Close();
    AutoSaveLocked = false;
    if (SerialisingHeroState)
        return true;
    StartTime = GFGetTime() - total_time_played;
    TimePlayed = total_time_played;
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCInGameMenu* in_game_menu = 0;
        creature->GetTC(TCI_IN_GAME_MENU, &in_game_menu);
        if (in_game_menu)
            in_game_menu->SetAccumulativePausedTime(0.0);
    }
    ActivateWorld(true);
    GFGetTime();
    CWideString auto_save = CUserProfileManager::Get().GetAutoSaveRegionLeavingPathName();
    if (CAFile::PathExists(auto_save))
        CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get().DeleteFileW(auto_save);
    return true;
}