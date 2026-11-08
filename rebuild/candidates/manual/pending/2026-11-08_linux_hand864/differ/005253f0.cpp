// CThingManager::LoadGameState(CPersistContext&) -- load the thing manager's part of a saved game: the game-state version and global UID count, the SAVED_ENTITIES section (hero state or entity persistence, binary or text) and the SAVED_NPC_NAMES section (UID, definition name, display name per NPC)
// Grade: DIFFER(2276v2263) -- 13 lines (was 21): with FABLE_RETAIL_STLP_MAP (visible map::operator[], key only read) the text UID's low dword is a register candidate kept in ebx and `this` reloads from [esp+24h] as retail (chunk 864 M1); left: at the 2nd in-loop Replace the GetAsCharArray join temp #59 (prio 32) is coloured after the `line.PStringData` load #20 (prio 40 = block 74 K=8 incl. the two uid-half candidates the map lever created) and takes eax (costs {eax:-1, edi:-1}, table order) where retail has the preset form `mov edi,NullChar ; je ; mov edi,[eax]`; at sites 1 and 3 #20 is 28 (K=5) so the join goes first and gets edi; the preset accessor (GETASCHARARRAY_SELECT, M3) gives retail's shape at every site but swaps text/search_text (edi/ebp) at the pre-loop site and in-loop sites 2-3: text 56/112/124 v the search preset 64/128/128 (site 1: 136 v 128 is retail's order), and one more `text` reference in the loop (M4, +64 per site) overshoots and flips the function-level this/context esi/edi pair
// Next idea: on M3: raise `text` by 4..16 at the three sites without a loop-weight reference (a depth-1 reference outside the while loop), or lower the search preset; the trace (candidates `text` v the accessor's `preset`) is the metric
// Leaf: ?LoadGameState@CThingManager@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005253F0, 2263 bytes; sole caller CWorld::LoadGameStateInternal 0x004A2AAF. Identity: Ego_r.pdb /
//   FableWin.pdb ?LoadGameState@CThingManager@@QAEXAAVCPersistContext@@@Z (chunk 843 proved it).
// Callees: ??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z 0x00410BE0, ??$Transfer@_K@CPersistContext@@QAEXPBDAA_KAB_K@Z 0x0044BB00,
//   CPersistContextReadSection() 0x00485CD2 (its dtor is expanded at the end: ~Text, the parser's comment list, two strings, ~CBaseClass
//   0x0099A2E0, ~CDataStream 0x00993B10), ReadSection 0x009BAE70, LoadHeroStateBinary 0x00520670 / LoadHeroStateScript 0x00520570,
//   NThingManager::CEntityRuntimePersistence::LoadFromGameStateBinary 0x00524EC0 / LoadFromGameStateScript 0x005248B0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline ReadULONG / ReadUHUGE), ReadPresizedString 0x00996390,
//   ReadNullTerminatedWString 0x00994960, SetNameForThing 0x00521A90, NPCNameMap operator[] 0x0052B8B0, CCharString::operator= 0x0099EFB0
//   / operator=(const char*) 0x0099EFE0 / copy 0x0099EC30 / += 0x0099F0A0 / ToWideString 0x0099E480, CStringParser::ReadLineAsString
//   0x009BA390, GFCharStringToInt 0x0099E7F0, GFCharStringToUHUGE 0x0099E870, strstr 0x00BFEB3A, CBasicString<char>::operator==(const
//   char*) 0x004115A0, ~CWideString 0x0099B510.
// Strings: "TNGGameStateVersion", "GlobalUIDCount", "SAVED_ENTITIES", "SAVED_NPC_NAMES", "\r\n", "", "UNATTACHED".
// Layout: engine/CThingManager.h (World +0x24, CurrentUniqueID +0x68, SerialisingGameState +0x9A, CurrentlyLoading +0x9B,
//   EntityRuntimePersistence +0xA0 (SaveFormatVersion at this +0xC4), NPCNameMap +0xCC typed under the opt-in); CWorld +0x102 the hero-state flag.
// Shape: `mov al,1` + two byte stores is `CurrentlyLoading = true; SerialisingGameState = CurrentlyLoading;`; the SAVED_NPC_NAMES ReadSection
//   test comes before the world flag (nested ifs); CCharString::Replace is the FableWin member, inline in retail (four expansions, the
//   FABLE_RETAIL_CCHARSTRING_REPLACE opt-in; strstr at the loop bottom advances `found` first). Binary path: the UID is read into one
//   local held in esi:edi for SetNameForThing and copied at once into a second local for the map key (`value` / `uid`), which shares
//   the GlobalUIDCount default temporary's slot; a single named UID left the key slot 8 bytes higher and the value in memory (md307).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING_REPLACE
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
#define FABLE_RETAIL_CTHINGMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContextReadSection.h"
long __fastcall GFCharStringToInt(const CCharString& text);                    // 0x0099E7F0
unsigned __int64 __fastcall GFCharStringToUHUGE(const CCharString& text);      // 0x0099E870

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
            unsigned long count = section.InputStream.ReadULONG();
            for (unsigned long i = 0; i < count; ++i) {
                unsigned __int64 value = section.InputStream.ReadUHUGE();
                unsigned __int64 uid = value;
                CCharString def_name = section.InputStream.ReadPresizedString();
                CWideString name = section.InputStream.ReadNullTerminatedWString();
                SetNameForThing(value, name);
                NPCNameMap[uid].first = def_name;
            }
        } else {
            CCharString line;
            line = section.Parser.ReadLineAsString();
            line.Replace("\r\n", "");
            unsigned long count = GFCharStringToInt(line);
            for (unsigned long i = 0; i < count; ++i) {
                line = section.Parser.ReadLineAsString();
                line.Replace("\r\n", "");
                unsigned __int64 uid = GFCharStringToUHUGE(line);
                line = section.Parser.ReadLineAsString();
                line.Replace("\r\n", "");
                if (line == "UNATTACHED")
                    line = "";
                CCharString def_name(line);
                line = section.Parser.ReadLineAsString();
                line.Replace("\r\n", "");
                CWideString name = line.ToWideString();
                SetNameForThing(uid, name);
                NPCNameMap[uid].first = def_name;
            }
        }
        }
    }
    SerialisingGameState = false;
    CurrentlyLoading = false;
}
