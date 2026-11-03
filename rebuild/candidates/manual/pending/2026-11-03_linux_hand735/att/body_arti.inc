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
