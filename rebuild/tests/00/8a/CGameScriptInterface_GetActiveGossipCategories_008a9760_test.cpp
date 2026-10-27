// behaviour test for 0x008A9760 CGameScriptInterface::GetActiveGossipCategories(CCharString) (model over std::map with a string key)
#include <cstdio>
#include <map>
#include <string>
#include <vector>
typedef std::string CCharString;
class CTCAIScratchpad { public:
    static std::map<CCharString, bool > ActiveGossipCategories;
    static bool* GetActiveGossipCategories(CCharString name)
    {
        if (ActiveGossipCategories.find(name) == ActiveGossipCategories.end())
            return 0;
        return &ActiveGossipCategories[name];
    }
};
std::map<CCharString, bool > CTCAIScratchpad::ActiveGossipCategories;
static bool* GetActiveGossipCategories(CCharString name) { return CTCAIScratchpad::GetActiveGossipCategories(name); }
int main()
{
    CTCAIScratchpad::ActiveGossipCategories["Bowerstone"] = bool();
    int ok = GetActiveGossipCategories("Oakvale") == 0 && GetActiveGossipCategories("Bowerstone") == &CTCAIScratchpad::ActiveGossipCategories["Bowerstone"] &&
             CTCAIScratchpad::ActiveGossipCategories.size() == 1;
    if (!ok) { std::printf("008a9760_TEST FAIL\n"); return 1; }
    std::printf("008a9760_TEST PASS\n");
    return 0;
}