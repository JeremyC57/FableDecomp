// behaviour test for 0x008AA1B0 CGameScriptInterface::GetGossipVillages(CCharString) (model over std::map with a string key)
#include <cstdio>
#include <map>
#include <string>
#include <vector>
typedef std::string CCharString;
class CTCAIScratchpad { public:
    static std::map<CCharString, std::vector<CCharString> > GossipVillages;
    static const std::vector<CCharString>* GetGossipVillages(CCharString name)
    {
        if (GossipVillages.find(name) == GossipVillages.end())
            return 0;
        return &GossipVillages[name];
    }
};
std::map<CCharString, std::vector<CCharString> > CTCAIScratchpad::GossipVillages;
static const std::vector<CCharString>* GetGossipVillages(CCharString name) { return CTCAIScratchpad::GetGossipVillages(name); }
int main()
{
    CTCAIScratchpad::GossipVillages["Bowerstone"] = std::vector<CCharString>();
    int ok = GetGossipVillages("Oakvale") == 0 && GetGossipVillages("Bowerstone") == &CTCAIScratchpad::GossipVillages["Bowerstone"] &&
             CTCAIScratchpad::GossipVillages.size() == 1;
    if (!ok) { std::printf("008aa1b0_TEST FAIL\n"); return 1; }
    std::printf("008aa1b0_TEST PASS\n");
    return 0;
}