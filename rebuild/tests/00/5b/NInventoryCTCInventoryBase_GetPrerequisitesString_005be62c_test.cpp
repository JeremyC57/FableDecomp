// Behaviour test for CTCInventoryBase::GetPrerequisitesString(CDefPointer<const CQuestCardDef>) @ 0x005BE62C
#include <cstdio>
#include <string>
#include <map>
#include <sstream>

static std::string IntStr(int v) { std::ostringstream o; o << v; return o.str(); }
static std::string Text(const std::string& sym) { if (sym == "TEXT_GUI_MENU_PREREQUISITES") return "Prerequisites"; if (sym == "TEXT_GUI_MENU_RENOWN") return "Renown"; return "?"; }

static std::string GetPrerequisitesString(const std::map<int, int>& prereqs, bool hasStats, int renown)
{
    std::string result = "";
    if (!prereqs.empty()) {
        result += Text("TEXT_GUI_MENU_PREREQUISITES") + ":";
        for (std::map<int, int>::const_iterator it = prereqs.begin(); it != prereqs.end(); ++it) {
            if (it->first == 0) {
                result += Text("TEXT_GUI_MENU_RENOWN") + ":" + IntStr(it->second);
                if (hasStats)
                    result += std::string("(") + IntStr(renown) + std::string(")");
            }
        }
    }
    return result;
}

int main()
{
    std::map<int, int> m;
    if (GetPrerequisitesString(m, true, 5) != "") { std::printf("005be62c_TEST FAIL\n"); return 1; }
    m[3] = 9;
    if (GetPrerequisitesString(m, true, 5) != "Prerequisites:") { std::printf("005be62c_TEST FAIL\n"); return 1; }
    m[0] = 120;
    if (GetPrerequisitesString(m, true, 5) != "Prerequisites:Renown:120(5)") { std::printf("005be62c_TEST FAIL\n"); return 1; }
    if (GetPrerequisitesString(m, false, 5) != "Prerequisites:Renown:120") { std::printf("005be62c_TEST FAIL\n"); return 1; }
    std::printf("005be62c_TEST PASS\n");
    return 0;
}