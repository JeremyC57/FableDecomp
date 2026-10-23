// Behaviour test for CTCInventoryBase::GetPrerequisitesString(CTCQuestCard*) @ 0x005BE3B1
#include <cstdio>
#include <string>
#include <map>
#include <sstream>

static std::string IntStr(int v) { std::ostringstream o; o << v; return o.str(); }
static std::string Text(const std::string& sym) { if (sym == "TEXT_GUI_MENU_RENOWN") return "Renown"; if (sym == "TEXT_GUI_MENU_CURRENT_LEVEL") return "Current "; return "?"; }

static std::string GetPrerequisitesString(const std::map<int, int>& prereqs, bool hasStats, int renown)
{
    std::string result = "";
    if (!prereqs.empty()) {
        std::map<int, int>::const_iterator it = prereqs.begin();
        while (it != prereqs.end()) {
            if (it->first == 0) {
                result += Text("TEXT_GUI_MENU_RENOWN") + std::string(":") + IntStr(it->second) + std::string(" ");
                if (hasStats)
                    result += std::string("(") + Text("TEXT_GUI_MENU_CURRENT_LEVEL") + IntStr(renown) + std::string(")");
            }
            ++it;
        }
    }
    return result;
}

int main()
{
    std::map<int, int> m;
    if (GetPrerequisitesString(m, true, 5) != "") { std::printf("005be3b1_TEST FAIL\n"); return 1; }
    m[2] = 9;
    if (GetPrerequisitesString(m, true, 5) != "") { std::printf("005be3b1_TEST FAIL\n"); return 1; }
    m[0] = 40;
    if (GetPrerequisitesString(m, true, 7) != "Renown:40 (Current 7)") { std::printf("005be3b1_TEST FAIL\n"); return 1; }
    if (GetPrerequisitesString(m, false, 7) != "Renown:40 ") { std::printf("005be3b1_TEST FAIL\n"); return 1; }
    std::printf("005be3b1_TEST PASS\n");
    return 0;
}
