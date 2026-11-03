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
            long size;
            do {
            file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            size = file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            } while (0);
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
            section.Context.Transfer("WorldFrame", world_frame, 0L);
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
            long size;
            do {
            file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            size = file_stream.ReadSLONG();
            file_stream.ReadSLONG();
            } while (0);
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
            section.Context.Transfer("WorldFrame", got_world_frame ? value : world_frame, 0L);
            section.Context.Transfer("TeleportingEnabled", flag, false);
            section.Context.Transfer("SavingEnabled", flag, false);
            section.Context.Transfer("ExperienceSpendingEnabled", flag, false);
            section.Context.Transfer("CreatureGenerationEnabled", flag, false);
            section.Context.Transfer("CreatureGenerationDisabledGroups", disabled_groups, 0UL);
            section.Context.Transfer("HeroSleepingEnabled", flag, false);
            section.Context.Transfer("MapTableShowQuestCardsOnUsed", flag, false);
            section.Context.Transfer("MiniMapEnabled", flag, false);
            section.Context.Transfer("MiniMapActiveBeforeDisabled", flag, false);
            section.Context.Transfer("GuildMasterMessagesEnabled", flag, false);
            section.Context.Transfer("SummonerDeathExplosionAffectsHero", flag, false);
            section.Context.Transfer("MostRecentSaveType", value, 0L);
            section.Context.Transfer("MostRecentSaveTypeBeforeManualSave", value, 0L);
            section.Context.Transfer("MostRecentManualSaveName", most_recent_manual_save_name);
            section.Context.Transfer("SaveGameMarkerPos", pos1);
            section.Context.Transfer("SaveGameMarkerAngleXY", angle, 0.0f);
            section.Context.Transfer("GuildSealRecallPos", pos2);
            section.Context.Transfer("GuildSealRecallAngleXY", angle, 0.0f);
            section.Context.Transfer("CurrentRegionName", region_name);
            section.Context.Transfer("CurrentRegionMinimapGraphicName", region_minimap_name);
        }
        file.Close();
    }
    FileRegionNames.push_back(region_name);
    FileRegionMiniMapNames.push_back(region_minimap_name);
    FilePlayTimes.push_back(world_frame);
}
