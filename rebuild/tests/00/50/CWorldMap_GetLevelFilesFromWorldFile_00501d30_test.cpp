// Behaviour test for CWorldMap::GetLevelFilesFromWorldFile 0x00501D30: the world-file key loop over a token stream.
// Non-string items are skipped; "NewMap n" sets the current map; "LevelName" needs a '"' symbol (else throws) and builds
// levels-dir + text up to the closing quote; "EndMap" stores the name at levelFiles[map] (growing the list) when map != 0;
// "LoadedOnPlayerProximity" consumes its value only.
#include <cstdio>
#include <cstring>

enum EParsedItemType { PARSED_ITEM_NULL = 0, PARSED_ITEM_SYMBOL = 1, PARSED_ITEM_STRING = 2, PARSED_ITEM_INTEGER = 3 };
struct Tok { EParsedItemType type; const char* text; long value; };

struct Parser {
    const Tok* toks; int n; int pos;
    bool NextItemExists() { return pos < n; }
    EParsedItemType PeekNextItemType() { return toks[pos].type; }
    void ReadNextItem() { ++pos; }
    const char* ReadAsString() { return toks[pos++].text; }
    const char* ReadAsSymbol() { return toks[pos++].text; }
    long ReadAsInteger() { return toks[pos++].value; }
};
struct GenericException { };

static const char* kLevelsDir = "Data/Levels/";
struct Files { char name[8][64]; int size; };

static void GetLevelFiles(Parser& parser, Files& levelFiles)
{
    char levelName[64]; levelName[0] = 0;
    long map = 0;
    while (parser.NextItemExists()) {
        if (parser.PeekNextItemType() != PARSED_ITEM_STRING) {
            parser.ReadNextItem();
        } else {
            const char* token = parser.ReadAsString();
            if (std::strcmp(token, "NewMap") == 0) {
                map = parser.ReadAsInteger();
            } else if (std::strcmp(token, "EndMap") == 0) {
                if (map != 0) {
                    while (levelFiles.size < map + 1) { levelFiles.name[levelFiles.size][0] = 0; ++levelFiles.size; }
                    std::strcpy(levelFiles.name[map], levelName);
                }
            } else if (std::strcmp(token, "LoadedOnPlayerProximity") == 0) {
                parser.ReadAsString();
            } else if (std::strcmp(token, "LevelName") == 0) {
                if (std::strcmp(parser.ReadAsSymbol(), "\"") != 0)
                    throw GenericException();
                std::strcpy(levelName, kLevelsDir);
                std::strcat(levelName, parser.ReadAsString());
            }
        }
    }
}

int main()
{
    Tok t[] = {
        { PARSED_ITEM_INTEGER, "", 7 },
        { PARSED_ITEM_STRING, "NewMap", 0 }, { PARSED_ITEM_INTEGER, "", 2 },
        { PARSED_ITEM_STRING, "LevelName", 0 }, { PARSED_ITEM_SYMBOL, "\"", 0 }, { PARSED_ITEM_STRING, "Oakvale.lev", 0 },
        { PARSED_ITEM_STRING, "LoadedOnPlayerProximity", 0 }, { PARSED_ITEM_STRING, "TRUE", 0 },
        { PARSED_ITEM_STRING, "EndMap", 0 },
        { PARSED_ITEM_STRING, "NewMap", 0 }, { PARSED_ITEM_INTEGER, "", 0 },
        { PARSED_ITEM_STRING, "EndMap", 0 },
    };
    Parser p = { t, (int)(sizeof(t) / sizeof(t[0])), 0 };
    Files f; f.size = 0;
    GetLevelFiles(p, f);
    if (f.size != 3 || std::strcmp(f.name[2], "Data/Levels/Oakvale.lev") != 0 || f.name[1][0] != 0) {
        std::printf("00501d30_TEST FAIL\n"); return 1;
    }
    Tok bad[] = { { PARSED_ITEM_STRING, "LevelName", 0 }, { PARSED_ITEM_SYMBOL, "=", 0 }, { PARSED_ITEM_STRING, "x", 0 } };
    Parser q = { bad, 3, 0 };
    bool threw = false;
    try { GetLevelFiles(q, f); } catch (GenericException&) { threw = true; }
    if (!threw) { std::printf("00501d30_TEST FAIL\n"); return 1; }
    std::printf("00501d30_TEST PASS\n");
    return 0;
}