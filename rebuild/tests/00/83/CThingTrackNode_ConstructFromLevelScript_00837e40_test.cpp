// Behaviour test for CThingTrackNode::ConstructFromLevelScript (0x00837E40): a standalone model of the key walk -- after the
// base construction the script's "Key value;" items set the previous / next node UIDs (LinkedToUID1 / LinkedToUID2, remapped),
// the previous / next thing file IDs (LinkedToFileID1 / LinkedToFileID2), the IsStart / IsEnd flags from a case-insensitive
// TRUE / FALSE word, the track name (ScriptName), and EndThing stops the walk; unknown keys are skipped; the base failure
// returns false before any key is read.
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
static unsigned long long ParseU64(const std::string& v) { unsigned long long u = 0; for (size_t i = 0; i < v.size(); ++i) if (v[i] >= '0' && v[i] <= '9') u = u * 10 + (unsigned)(v[i] - '0'); return u; }
struct Node { unsigned long long NextUID, PrevUID; long PrevFileID, NextFileID; bool IsStart, IsEnd; std::string TrackName; bool Constructed; };
static bool EqualsCI(const std::string& a, const char* b) { if (a.size() != std::strlen(b)) return false; for (size_t i = 0; i < a.size(); ++i) { char x = a[i], y = b[i]; if (x >= 'a' && x <= 'z') x -= 32; if (y >= 'a' && y <= 'z') y -= 32; if (x != y) return false; } return true; }
static unsigned long long Remap(unsigned long long uid) { return uid + 1000; }   // stands in for RemapUIDForSerialisation
static bool Construct(Node& n, const char* script, bool baseOk)
{
    n.Constructed = baseOk;
    if (!baseOk) return false;
    n.NextUID = 0; n.PrevUID = 0; n.PrevFileID = 0; n.NextFileID = 0;
    std::istringstream in(script);
    std::string item;
    while (std::getline(in, item, ';')) {
        std::istringstream line(item);
        std::string key, value;
        if (!(line >> key)) continue;
        line >> value;
        if (key == "LinkedToUID1") n.PrevUID = Remap(ParseU64(value));
        else if (key == "LinkedToUID2") n.NextUID = Remap(ParseU64(value));
        else if (key == "LinkedToFileID1") n.PrevFileID = std::atol(value.c_str());
        else if (key == "LinkedToFileID2") n.NextFileID = std::atol(value.c_str());
        else if (key == "Start") { if (EqualsCI(value, "TRUE")) n.IsStart = true; else if (EqualsCI(value, "FALSE")) n.IsStart = false; }
        else if (key == "End") { if (EqualsCI(value, "TRUE")) n.IsEnd = true; else if (EqualsCI(value, "FALSE")) n.IsEnd = false; }
        else if (key == "ScriptName") n.TrackName = value;
        else if (key == "EndThing") break;
    }
    return true;
}
int main()
{
    int fail = 0;
    Node n = { 9, 9, 9, 9, false, true, "", false };
    if (!Construct(n, "LinkedToUID1 5; Start true; LinkedToFileID2 77; ScriptName Track_A; End FALSE; Unknown 1; EndThing; LinkedToUID2 6;", true)) fail = 1;
    if (n.PrevUID != 1005 || n.NextUID != 0 || n.NextFileID != 77 || n.PrevFileID != 0 || !n.IsStart || n.IsEnd || n.TrackName != "Track_A") fail = 2;
    Node m = { 9, 9, 9, 9, true, false, "x", false };
    if (Construct(m, "LinkedToUID2 3;", false) || m.NextUID != 9) fail = 3;   // base failure: nothing read
    Node k = { 0, 0, 0, 0, false, false, "", false };
    Construct(k, "LinkedToUID2 3; Start maybe; End True; LinkedToFileID1 -4;", true);
    if (k.NextUID != 1003 || k.IsStart || !k.IsEnd || k.PrevFileID != -4) fail = 4;
    if (fail) { std::printf("00837e40_TEST FAIL %d\n", fail); return 1; }
    std::printf("00837e40_TEST PASS\n");
    return 0;
}