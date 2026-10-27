// behaviour test for 0x008AA410 CGameScriptInterface::GetGossipFactions(CCharString) (model over std::map with a string key)
#include <cstdio>
#include <map>
#include <string>
#include <vector>
typedef std::string CCharString;
class CTCAIScratchpad { public:
    static std::map<CCharString, std::vector<CCharString> > GossipFactions;
    static const std::vector<CCharString>* GetGossipFactions(CCharString name)
    {
        if (GossipFactions.find(name) == GossipFactions.end())
            return 0;
        return &GossipFactions[name];
    }
};
std::map<CCharString, std::vector<CCharString> > CTCAIScratchpad::GossipFactions;
static const std::vector<CCharString>* GetGossipFactions(CCharString name) { return CTCAIScratchpad::GetGossipFactions(name); }
int main()
{
    CTCAIScratchpad::GossipFactions["Bowerstone"] = std::vector<CCharString>();
    int ok = GetGossipFactions("Oakvale") == 0 && GetGossipFactions("Bowerstone") == &CTCAIScratchpad::GossipFactions["Bowerstone"] &&
             CTCAIScratchpad::GossipFactions.size() == 1;
    if (!ok) { std::printf("008aa410_TEST FAIL\n"); return 1; }
    std::printf("008aa410_TEST PASS\n");
    return 0;
}