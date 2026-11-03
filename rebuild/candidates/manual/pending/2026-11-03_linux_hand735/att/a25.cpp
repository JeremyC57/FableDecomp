// CTCInGameMenu::AddRegionAndTimeInfo(const CWideString& file_name) -- for one save slot, read the region name, the region's
// minimap graphic name and the world frame (play time) from the save's HEADER section and append them to FileRegionNames /
// FileRegionMiniMapNames / FilePlayTimes; the world frame comes from the ".hs" companion file when that verifies, else from
// the save itself
// Leaf: ?AddRegionAndTimeInfo@CTCInGameMenu@@AAEXABVCWideString@@@Z
// Flags: /GS
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
            long size = file_stream.ReadSLONG();
            long data_size = file_stream.ReadSLONG();
            data.resize(size);
            file_stream.ReadZlibCompressed(data.GetAsCArray(), size);
            memory_stream.OpenMemorySource(data.GetAsCArray(), size);
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
            long size = file_stream.ReadSLONG();
            long data_size = file_stream.ReadSLONG();
            data.resize(size);
            file_stream.ReadZlibCompressed(data.GetAsCArray(), size);
            memory_stream.OpenMemorySource(data.GetAsCArray(), size);
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
            bool flag;
            long value;
            unsigned long disabled_groups;
            float angle;
            C3DVector pos1;
            C3DVector pos2;
            section.Context.Transfer("WorldName", world_name);
            long* frame = got_world_frame ? &value : &world_frame;
            section.Context.Transfer("WorldFrame", *frame);
            section.Context.Transfer("TeleportingEnabled", flag);
            section.Context.Transfer("SavingEnabled", flag);
            section.Context.Transfer("ExperienceSpendingEnabled", flag);
            section.Context.Transfer("CreatureGenerationEnabled", flag);
            section.Context.Transfer("CreatureGenerationDisabledGroups", disabled_groups);
            section.Context.Transfer("HeroSleepingEnabled", flag);
            section.Context.Transfer("MapTableShowQuestCardsOnUsed", flag);
            section.Context.Transfer("MiniMapEnabled", flag);
            section.Context.Transfer("MiniMapActiveBeforeDisabled", flag);
            section.Context.Transfer("GuildMasterMessagesEnabled", flag);
            section.Context.Transfer("SummonerDeathExplosionAffectsHero", flag);
            section.Context.Transfer("MostRecentSaveType", value);
            section.Context.Transfer("MostRecentSaveTypeBeforeManualSave", value);
            section.Context.Transfer("MostRecentManualSaveName", most_recent_manual_save_name);
            section.Context.Transfer("SaveGameMarkerPos", pos1);
            section.Context.Transfer("SaveGameMarkerAngleXY", angle);
            section.Context.Transfer("GuildSealRecallPos", pos2);
            section.Context.Transfer("GuildSealRecallAngleXY", angle);
            section.Context.Transfer("CurrentRegionName", region_name);
            section.Context.Transfer("CurrentRegionMinimapGraphicName", region_minimap_name);
        }
        file.Close();
    }
    FileRegionNames.push_back(region_name);
    FileRegionMiniMapNames.push_back(region_minimap_name);
    FilePlayTimes.push_back(world_frame);
}
