// behaviour test for 0x008A9F50 CGameScriptInterface::GetGossip(CCharString) (model over std::map with a string key)
#include <cstdio>
#include <map>
#include <string>
#include <vector>
typedef std::string CCharString;
class CTCAIScratchpad { public:
    static std::map<CCharString, std::vector<CCharString> > Gossip;
    static const std::vector<CCharString>* GetGossip(CCharString name)
    {
        if (Gossip.find(name) == Gossip.end())
            return 0;
        return &Gossip[name];
    }
};
std::map<CCharString, std::vector<CCharString> > CTCAIScratchpad::Gossip;
static const std::vector<CCharString>* GetGossip(CCharString name) { return CTCAIScratchpad::GetGossip(name); }
int main()
{
    CTCAIScratchpad::Gossip["Bowerstone"] = std::vector<CCharString>();
    int ok = GetGossip("Oakvale") == 0 && GetGossip("Bowerstone") == &CTCAIScratchpad::Gossip["Bowerstone"] &&
             CTCAIScratchpad::Gossip.size() == 1;
    if (!ok) { std::printf("008a9f50_TEST FAIL\n"); return 1; }
    std::printf("008a9f50_TEST PASS\n");
    return 0;
}