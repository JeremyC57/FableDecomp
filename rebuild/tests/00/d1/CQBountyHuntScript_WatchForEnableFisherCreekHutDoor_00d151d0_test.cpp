// Behaviour test for WatchForEnableFisherCreekHutDoor @ 0x00D151D0: wait for region-loaded messages until "Fisherman"
// is loaded, then fetch the FishermanHutDoor thing and open it only when it is alive.
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Regions; size_t Next; bool DoorAlive; std::vector<std::string> Log;
    bool MsgOnRegionLoaded(std::string* r) { if (Next >= Regions.size()) return false; *r = Regions[Next++]; return true; }
    void NewScriptFrame() { Log.push_back("frame"); }
    void OpenDoor(const std::string& d) { Log.push_back("open " + d); }
};
static void Watch(Game& g)
{
    std::string region;
    for (;;) {
        if (g.MsgOnRegionLoaded(&region) && region == "Fisherman") break;
        g.NewScriptFrame();
    }
    std::string door = "FishermanHutDoor";
    if (g.DoorAlive) g.OpenDoor(door);
}
int main()
{
    Game g; g.Next = 0; g.DoorAlive = true;
    g.Regions.push_back("Bowerstone"); g.Regions.push_back("Fisherman");
    Watch(g);
    Game h; h.Next = 0; h.DoorAlive = false; h.Regions.push_back("Fisherman");
    Watch(h);
    bool ok = g.Log.size() == 2 && g.Log[0] == "frame" && g.Log[1] == "open FishermanHutDoor" && h.Log.empty();
    if (!ok) { std::printf("00d151d0_TEST FAIL\n"); return 1; }
    std::printf("00d151d0_TEST PASS\n");
    return 0;
}