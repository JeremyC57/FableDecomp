// CTCInGameMenu::AddRegionAndTimeInfo(const CWideString& file_name) -- for one save slot, read the region name, the region's minimap graphic name and the world frame (play time) from the save's HEADER section and append them to FileRegionNames / FileRegionMiniMapNames / FilePlayTimes; the world frame comes from the ".hs" header companion when that one verifies, else from the save itself
// Leaf: ?AddRegionAndTimeInfo@CTCInGameMenu@@AAEXABVCWideString@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047F58B, 2296 bytes, thiscall, `ret 4`. Identity: Ego_r.pdb + FableWin.pdb
//   ?AddRegionAndTimeInfo@CTCInGameMenu@@AAEXABVCWideString@@@Z (private); the catalog said NUISystem::CFrontEndManager, but the
//   row sits between CTCInGameMenu::VideoOptionsNotAccepted 0x0047F30F and GetFileNameList 0x0047FE83 (its three callers
//   0x0047FF80 / 0x004802A3 / 0x004803C2), calls CTCInGameMenu::VerifySignatureCached 0x0047ED0A and appends to CTCInGameMenu's
//   vectors at +0x78 / +0x84 / +0x90. Its byte twin with the CFrontEndManager members (+0x74 / +0x80 / +0x8C) and
//   VerifySignatureCached 0x005957D9 is 0x00597228.
// Callees: CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 / ~ 0x0099EAE0, operator+(const CWideString&, const wchar_t*)
//   0x0099BF30 (L".hs", the header companion), CTCInGameMenu::VerifySignatureCached 0x0047ED0A, CAFile::PathExists 0x00999230, CDiskFileWin32(const CWideString&,
//   FileAccessType, unsigned long) 0x0099AD80 / Read 0x0099A960 / SetPosition 0x0099ABF0 / Close 0x0099A920 / ~ 0x00401630,
//   CMemoryDataInputStream() 0x00485400 / OpenMemorySource 0x00994570 / ~CDataStream 0x00993B10, CStringParser() 0x00485C90 /
//   Init(const CCharString&, ...) 0x00403C20 / ~ 0x004047A0, CFileDataInputStream(CAFile&, unsigned long) 0x00994700 / ~ 0x00994780,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline ReadSLONG slow path) / ReadZlibCompressed 0x00996230,
//   vector<unsigned char>::resize(size_type) 0x004861DF, free 0x00BFEA14 (the STLport byte vector), CCharString::LoadFromFile
//   0x0099F2E0, CPersistContext::Init(CMemoryDataInputStream&, bool) 0x009BAA30 / Init(CStringParser&, ...) 0x009BA9D0 /
//   ReadSection 0x009BAE70, CPersistContextReadSection() 0x00485CD2 / ~ 0x00485CF1, the CPersistContext::Transfer instances
//   2-argument CCharString 0x00411010 / CWideString 0x00411060 / C3DVector 0x00485B44 and 3-argument long 0x00410BE0 / bool
//   0x004045C0 / unsigned long 0x004106F0 / float 0x00410620, CWideString() 0x0099AED0 / ~ 0x0099B510,
//   vector<CCharString>::push_back 0x0044BFF0, vector<long>::push_back 0x00485FF3, __security_check_cookie 0x00BFE9F9.
// Shape (size TU: `optimize("s", on)` alone gives the biased `lea ebp,[esp-74h]` frame; /GS for the cookie): two copies of
//   the CWorld save-file opener (cf. CWorld::LoadGameStateInternal 0x004A21F0): "FableSav" binary saves skip two longs, read the
//   zlib header size (the data size stays unread) and decompress only the header block into a CArray<unsigned char>; text
//   saves go through CStringParser; then the "HEADER" section. The inline 2-argument Transfer<T> + GetDefaultVal<T>() is visible
//   (as in CUserProfileManager::LoadGameState 0x0040BCA0): VC7.1 expands it for the scalar types (the default temporary is stored
//   just before each call) and keeps the CCharString / CWideString / C3DVector instances out of line. The WorldFrame transfer of
//   the save itself is an if/else of two calls that VC7.1 tail-merges into one call over a selected address (a `?:` operand is
//   evaluated before the default temporary instead). The inline CDataInputStream::Read chain is __forceinline in the header (no
//   out-of-line copy in Ego_r.pdb or retail) and written with the `||` range test (the `bool valid` form changes the register
//   allocation around the movsd of the header-size read).
// Layout: engine/CTCInGameMenu.h retail branch (FileRegionNames +0x78, FileRegionMiniMapNames +0x84, FilePlayTimes +0x90).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContextReadSection.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include <string.h>

// The inline two-argument Transfer and GetDefaultVal<T>() (declared in engine/CPersistContext.h; FableWin.pdb
// ??$Transfer@J@CPersistContext@@QAEXPBDAAJ@Z / ??$GetDefaultVal@J@CPersistContext@@KIJXZ and the other instances).
template<class T> inline void CPersistContext::Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { T value = T(); return value; }

void CTCInGameMenu::AddRegionAndTimeInfo(const CWideString& file_name)
{
    CCharString region_name("");
    CCharString region_minimap_name("");
    long world_frame = 0;
    CWideString header_file_name = file_name + L".hs";
    bool got_world_frame = false;
    if (VerifySignatureCached(header_file_name) && CAFile::PathExists(header_file_name)) {
        CDiskFileWin32 file(header_file_name, FILE_ACCESS_READ, 2);
        char header[8];
        file.Read(header, 8, 0);
        int text_file = memcmp(header, "FableSav", 8);
        CArray<unsigned char> data;
        CMemoryDataInputStream memory_stream;
        CCharString text;
        CStringParser parser;
        CPersistContext context;
        if (text_file == 0) {
            CFileDataInputStream file_stream(file, 0x4000);
            file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            long header_size = file_stream.ReadSLONG();
            long data_size = file_stream.ReadSLONG();
            data.resize(header_size);
            file_stream.ReadZlibCompressed(data.GetAsCArray(), header_size);
            memory_stream.OpenMemorySource(data.GetAsCArray(), header_size);
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
            section.Context.Transfer("WorldFrame", world_frame);
        }
        got_world_frame = true;
        file.Close();
    }
    if (VerifySignatureCached(file_name) && CAFile::PathExists(file_name)) {
        CDiskFileWin32 file(file_name, FILE_ACCESS_READ, 2);
        char header[8];
        file.Read(header, 8, 0);
        int text_file = memcmp(header, "FableSav", 8);
        CArray<unsigned char> data;
        CMemoryDataInputStream memory_stream;
        CCharString text;
        CStringParser parser;
        CPersistContext context;
        if (text_file == 0) {
            CFileDataInputStream file_stream(file, 0x4000);
            file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            long header_size = file_stream.ReadSLONG();
            long data_size = file_stream.ReadSLONG();
            data.resize(header_size);
            file_stream.ReadZlibCompressed(data.GetAsCArray(), header_size);
            memory_stream.OpenMemorySource(data.GetAsCArray(), header_size);
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
            CWideString most_recent_manual_save_name;
            bool unused_bool;
            long unused_long;
            unsigned long creature_generation_disabled_groups;
            float unused_angle;
            C3DVector save_game_marker_pos;
            C3DVector guild_seal_recall_pos;
            section.Context.Transfer("WorldName", world_name);
            if (!got_world_frame)
                section.Context.Transfer("WorldFrame", world_frame);
            else
                section.Context.Transfer("WorldFrame", unused_long);
            section.Context.Transfer("TeleportingEnabled", unused_bool);
            section.Context.Transfer("SavingEnabled", unused_bool);
            section.Context.Transfer("ExperienceSpendingEnabled", unused_bool);
            section.Context.Transfer("CreatureGenerationEnabled", unused_bool);
            section.Context.Transfer("CreatureGenerationDisabledGroups", creature_generation_disabled_groups);
            section.Context.Transfer("HeroSleepingEnabled", unused_bool);
            section.Context.Transfer("MapTableShowQuestCardsOnUsed", unused_bool);
            section.Context.Transfer("MiniMapEnabled", unused_bool);
            section.Context.Transfer("MiniMapActiveBeforeDisabled", unused_bool);
            section.Context.Transfer("GuildMasterMessagesEnabled", unused_bool);
            section.Context.Transfer("SummonerDeathExplosionAffectsHero", unused_bool);
            section.Context.Transfer("MostRecentSaveType", unused_long);
            section.Context.Transfer("MostRecentSaveTypeBeforeManualSave", unused_long);
            section.Context.Transfer("MostRecentManualSaveName", most_recent_manual_save_name);
            section.Context.Transfer("SaveGameMarkerPos", save_game_marker_pos);
            section.Context.Transfer("SaveGameMarkerAngleXY", unused_angle);
            section.Context.Transfer("GuildSealRecallPos", guild_seal_recall_pos);
            section.Context.Transfer("GuildSealRecallAngleXY", unused_angle);
            section.Context.Transfer("CurrentRegionName", region_name);
            section.Context.Transfer("CurrentRegionMinimapGraphicName", region_minimap_name);
        }
        file.Close();
    }
    FileRegionNames.push_back(region_name);
    FileRegionMiniMapNames.push_back(region_minimap_name);
    FilePlayTimes.push_back(world_frame);
}
