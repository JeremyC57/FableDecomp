// Behaviour test for CWorld::LoadQuestsFromFile (0x004A0D90): a standalone model of the statement parser -- AddQuest("name", TRUE)
// registers the quest and marks it initially active (TRUE matched case-insensitively), AddTestQuest(...) records a test quest
// with its colour index, other statements are skipped to the next ';'.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <strings.h>

struct TestQuest { std::string quest, hsp, display, ini, outi, card; long colour; };

static std::string Field(const std::string& s, size_t& pos)       // the next "quoted" field after pos
{
    size_t a = s.find('"', pos), b = s.find('"', a + 1); pos = b + 1; return s.substr(a + 1, b - a - 1);
}

static void Load(const std::string& text, std::vector<std::string>& registered, std::vector<std::string>& active,
                 std::vector<TestQuest>& tests)
{
    size_t pos = 0;
    while (pos < text.size()) {
        size_t end = text.find(';', pos); if (end == std::string::npos) end = text.size();
        std::string stmt = text.substr(pos, end - pos); pos = end + 1;
        size_t open = stmt.find('('); if (open == std::string::npos) continue;
        std::string cmd = stmt.substr(0, open); cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
        size_t p = open;
        if (cmd == "AddQuest") {
            std::string name = Field(stmt, p);
            size_t c = stmt.find(',', p) + 1; while (stmt[c] == ' ') ++c;
            std::string flag = stmt.substr(c, stmt.find(')', c) - c);
            registered.push_back(name);
            if (strcasecmp(flag.c_str(), "TRUE") == 0) active.push_back(name);
        } else if (cmd == "AddTestQuest") {
            TestQuest t; t.quest = Field(stmt, p); t.hsp = Field(stmt, p);
            size_t c = stmt.find(',', p) + 1; t.colour = std::strtol(stmt.c_str() + c, 0, 10); p = stmt.find(',', c);
            t.display = Field(stmt, p); t.ini = Field(stmt, p); t.outi = Field(stmt, p); t.card = Field(stmt, p);
            tests.push_back(t);
        }
    }
}

int main()
{
    std::vector<std::string> registered, active; std::vector<TestQuest> tests;
    Load("AddQuest(\"Q1\", TRUE);\nAddQuest(\"Q2\", false);\nOther(1);\n"
         "AddTestQuest(\"T1\", \"H1\", 3, \"D1\", \"I1\", \"O1\", \"C1\");", registered, active, tests);
    if (registered.size() != 2 || registered[1] != "Q2" || active.size() != 1 || active[0] != "Q1" || tests.size() != 1 ||
        tests[0].colour != 3 || tests[0].hsp != "H1" || tests[0].card != "C1") { std::printf("004a0d90_TEST FAIL\n"); return 1; }
    std::printf("004a0d90_TEST PASS\n");
    return 0;
}
