// CThingManager::LoadGameState -- Restore saved entities and NPC names for a world or hero.
// Inline-budget: native 00525667 expands the UID reader. The four text sections expand CCharString::Replace.
// Leaf: ?LoadGameState@CThingManager@@QAEXAAVCPersistContext@@@Z
// Retail: 0x005253F0, 2263 bytes, thiscall.
#define FABLE_RETAIL_CDATAINPUTSTREAM_UID_EXPAND_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING_REPLACE_EGOD_PASS24
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READPRESIZED
#define FABLE_RETAIL_CDATAINPUTSTREAM_READUHUGE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_READLINE
#define FABLE_RETAIL_CTHINGMANAGER_NPCNAMES
#define FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
#define FABLE_RETAIL_CTHINGMANAGER_LOADHERO_SCRIPT_PASS24
#define FABLE_RETAIL_CTHINGMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContextReadSection.h"
long FABLE_FASTCALL GFCharStringToInt(const CCharString& text);                    // 0x0099E7F0
unsigned __int64 FABLE_FASTCALL GFCharStringToUHUGE(const CCharString& text);      // 0x0099E870

inline CPersistContextReadSection::~CPersistContextReadSection() {}

void CThingManager::LoadGameState(CPersistContext& context)
{
    CurrentlyLoading = true;
    SerialisingGameState = CurrentlyLoading;
    if (!World.IsSerialisingHeroState()) {
        long version = 2;
        context.Transfer("TNGGameStateVersion", version, 0L);
        EntityRuntimePersistence.SaveFormatVersion = version;
        context.Transfer("GlobalUIDCount", CurrentUniqueID, (unsigned __int64)0);
    }
    CPersistContextReadSection section;
    if (context.ReadSection(section, "SAVED_ENTITIES")) {
        if (World.IsSerialisingHeroState()) {
            if (context.IsBinary())
                LoadHeroStateBinary(section.Context);
            else
                LoadHeroStateScript(section.Parser);
        } else {
            if (context.IsBinary())
                EntityRuntimePersistence.LoadFromGameStateBinary(section.Context);
            else
                EntityRuntimePersistence.LoadFromGameStateScript(section.Parser);
        }
    }
    if (context.ReadSection(section, "SAVED_NPC_NAMES")) {
        if (!World.IsSerialisingHeroState()) {
        if (context.IsBinary()) {
            unsigned long map_population = section.InputStream.ReadULONG();
            for (int i = 0; i < map_population; ++i) {
                unsigned __int64 uid = section.InputStream.ReadUHUGE();
                CCharString passport = section.InputStream.ReadPresizedString();
                CWideString stored_name = section.InputStream.ReadNullTerminatedWString();
                SetNameForThing(uid, stored_name);
                NPCNameMap[uid].first = passport;
            }
        } else {
            CCharString str;
            str = section.Parser.ReadLineAsString();
            str.Replace("\r\n", "");
            unsigned long map_population = GFCharStringToInt(str);
            for (int i = 0; i < map_population; ++i) {
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                unsigned __int64 uid = GFCharStringToUHUGE(str);
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                if (str == "UNATTACHED")
                    str = "";
                CCharString passport(str);
                str = section.Parser.ReadLineAsString();
                str.Replace("\r\n", "");
                CWideString stored_name = str.ToWideString();
                SetNameForThing(uid, stored_name);
                NPCNameMap[uid].first = passport;
            }
        }
        }
    }
    SerialisingGameState = false;
    CurrentlyLoading = false;
}
