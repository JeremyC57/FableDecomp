// CWorld::LoadQuestsFromFile(CAFile& file, bool clearRegisteredQuests) -- read the world's quest list script: optionally clear the
//   registered quests, load the file into a zero-terminated buffer and parse it statement by statement: AddQuest("name", TRUE|FALSE)
//   registers the quest (RegisteredQuests, InitialActiveQuests when TRUE, CQuestManager::RegisterQuest) and AddTestQuest("quest",
//   "hsp", colour, "display", "ini", "outi", "card") appends a CTestQuest to ActiveTestQuests; every statement ends at ';'
// Shape note: CBasicString<char>::operator==(const char*) and Compare are visible to the TU (existing opt-ins
//   FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE + FABLE_RETAIL_CBASICSTRING_COMPARE_BODY): the command test calls 0x004115A0 out of
//   line, and VC7.1, knowing the call writes no memory, keeps the token's PStringData in esi across it and threads the second
//   literal test on both the null and non-null paths (0x004A0E70..0x004A0EB2, 0x004A1127..).
// Leaf: ?LoadQuestsFromFile@CWorld@@QAEXAAVCAFile@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0D90, 2733 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb + FableWin.pdb name sets ?LoadQuestsFromFile@CWorld@@QAEXAAVCAFile@@_N@Z; callers 0x004A193C / 0x004A199C; the
//   "AddQuest" / "AddTestQuest" literals and the RegisteredQuests / InitialActiveQuests / ActiveTestQuests members it fills.
// Callees: CWorld::ClearRegisteredQuests 0x004A08D0, CAFile virtuals GetLength (+0x24) / Read (+0x0C), vector<char>(n) 0x00410F30,
//   CStringParser(const char*, ...) 0x00414E30 + NextItemExists 0x009BA4F0 / SkipWhitespace 0x009B9C60 / ReadAsStringUntilString
//   0x009BA330 / SkipPastNextItemIfIs 0x009B9E00 / ReadAsInteger 0x009BA540 / SkipPastString 0x009B9BA0, CBasicString<char>::
//   operator==(const char*) 0x004115A0, _stricmp 0x00BFEBA8, vector<CCharString>::_M_insert_overflow 0x00433530, CCharString copy
//   ctor 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, CQuestManager::RegisterQuest 0x004B2850 (singleton 0x013B89FC),
//   CTestQuest copy ctor 0x004A89D0, vector<CTestQuest>::_M_insert_overflow 0x004ADB50, ~list 0x004046B0, ~CBaseClass 0x0099A2E0,
//   free 0x00BFEA14. Literals "(" 0x0122E028, "\"" 0x0122DF24, "," 0x0122E024, ")" 0x0122E020, ";" 0x0123542C, "TRUE" 0x0122D6D4,
//   "AddQuest" 0x01238EA8, "AddTestQuest" 0x01238E98.
// Shape: CCharString::EqualsCaseInsensitive is the inline _stricmp body (retail expands it, 0x004A1028); CTestQuest's default
//   constructor and destructor are the implicit ones (expanded), its copy constructor out of line.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/CAFile.h"
#include "engine/CStringParser.h"
#include "engine/CArray.h"
#include "engine/CTestQuest.h"
#include "fable_string.h"
#include <string.h>

void CWorld::LoadQuestsFromFile(CAFile& file, bool clearRegisteredQuests)
{
    if (clearRegisteredQuests)
        ClearRegisteredQuests();
    if (file.GetLength() > 0) {
        std::vector<char> buffer(file.GetLength() + 1);
        file.Read(&buffer[0], file.GetLength() + 1, 0);
        buffer[file.GetLength()] = 0;
        CStringParser parser(&buffer[0]);
        while (parser.NextItemExists()) {
            parser.SkipWhitespace();
            CCharString command = parser.ReadAsStringUntilString("(");
            if (command == "AddQuest") {
                parser.SkipPastNextItemIfIs("(");
                parser.SkipPastNextItemIfIs("\"");
                CCharString questName = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                CCharString activeString = parser.ReadAsStringUntilString(")");
                parser.SkipPastNextItemIfIs(")");
                bool active = false;
                if (activeString.EqualsCaseInsensitive("TRUE"))
                    active = true;
                RegisteredQuests.push_back(questName);
                if (active)
                    InitialActiveQuests.push_back(questName);
                CQuestManager::Get().RegisterQuest(questName);
            } else if (command == "AddTestQuest") {
                parser.SkipPastNextItemIfIs("(");
                parser.SkipPastNextItemIfIs("\"");
                CCharString questName = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs("\"");
                CCharString hspName = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                long colourIndex = parser.ReadAsInteger();
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs("\"");
                CCharString displayName = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs("\"");
                CCharString iniFile = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs("\"");
                CCharString outiFile = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs("\"");
                CCharString questCardDefName = parser.ReadAsStringUntilString("\"");
                parser.SkipPastNextItemIfIs("\"");
                parser.SkipPastNextItemIfIs(")");
                CTestQuest testQuest;
                testQuest.QuestName = questName;
                testQuest.HSPName = hspName;
                testQuest.ColourIndex = colourIndex;
                testQuest.DisplayName = displayName;
                testQuest.IniFile = iniFile;
                testQuest.OutiFile = outiFile;
                testQuest.QuestCardDefName = questCardDefName;
                ActiveTestQuests.push_back(testQuest);
            }
            parser.SkipPastString(";");
        }
    }
}
