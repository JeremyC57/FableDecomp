// behaviour test for 0x008A9810 CGameScriptInterface::GetIsGossipForPlayer(CCharString) (model over std::map with a string key)
#include <cstdio>
#include <map>
#include <string>
#include <vector>
typedef std::string CCharString;
class CTCAIScratchpad { public:
    static std::map<CCharString, bool > IsGossipForPlayer;
    static bool* GetIsGossipForPlayer(CCharString name)
    {
        if (IsGossipForPlayer.find(name) == IsGossipForPlayer.end())
            return 0;
        return &IsGossipForPlayer[name];
    }
};
std::map<CCharString, bool > CTCAIScratchpad::IsGossipForPlayer;
static bool* GetIsGossipForPlayer(CCharString name) { return CTCAIScratchpad::GetIsGossipForPlayer(name); }
int main()
{
    CTCAIScratchpad::IsGossipForPlayer["Bowerstone"] = bool();
    int ok = GetIsGossipForPlayer("Oakvale") == 0 && GetIsGossipForPlayer("Bowerstone") == &CTCAIScratchpad::IsGossipForPlayer["Bowerstone"] &&
             CTCAIScratchpad::IsGossipForPlayer.size() == 1;
    if (!ok) { std::printf("008a9810_TEST FAIL\n"); return 1; }
    std::printf("008a9810_TEST PASS\n");
    return 0;
}