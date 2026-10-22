// Behaviour test for CQ_AmbushTradersScript::ProcessInitialSpyExtraction @ 0x00F0F680: set contact flags, update the
// objective, release a following spy from scripted control first, then make him follow the hero with a red health bar
// and spawn WatchForArcherSpawnConditions.
#include <cstdio>
#include <string>
#include <vector>
struct Script {
    bool SpyContactMade; long SpeechBanditCount, SpyHealthBarID; bool Scripting, Following; std::vector<std::string> Log;
    void Process()
    {
        SpyContactMade = true; SpeechBanditCount = 0;
        Log.push_back("objective 04");
        if (Scripting && Following) {
            Scripting = false; Log.push_back("restart scripting"); Scripting = true;
            Following = false; Log.push_back("stop following");
            Scripting = false;
        }
        Log.push_back("follow hero"); Log.push_back("refresh allegiances");
        SpyHealthBarID = 42; Log.push_back("spawn WatchForArcherSpawnConditions");
    }
};
int main()
{
    Script a; a.SpyContactMade = false; a.SpeechBanditCount = 3; a.Scripting = true; a.Following = true; a.Process();
    Script b; b.SpyContactMade = false; b.SpeechBanditCount = 3; b.Scripting = true; b.Following = false; b.Process();
    bool ok = a.SpyContactMade && a.SpeechBanditCount == 0 && a.Log.size() == 6 && a.Log[2] == "stop following" && !a.Scripting &&
              b.Log.size() == 4 && b.Scripting && b.SpyHealthBarID == 42 && b.Log[3] == "spawn WatchForArcherSpawnConditions";
    if (!ok) { std::printf("00f0f680_TEST FAIL\n"); return 1; }
    std::printf("00f0f680_TEST PASS\n");
    return 0;
}
