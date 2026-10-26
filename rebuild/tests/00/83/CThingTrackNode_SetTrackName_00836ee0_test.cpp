#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[16];
    CCharString(const char* t) { std::strcpy(Text, t); }
    CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
    bool operator!=(const CCharString& o) const { return std::strcmp(Text, o.Text) != 0; }
};
static int g_sets = 0;
class CThingTrackNode { public:
    CThingTrackNode* PNextNode;
    CThingTrackNode* PPreviousNode;
    CCharString TrackName;
    CThingTrackNode() : PNextNode(0), PPreviousNode(0), TrackName("old") {}
    void SetTrackName(const CCharString& name);
};
void CThingTrackNode::SetTrackName(const CCharString& name)
{
    ++g_sets;
    TrackName = name;
    if (PPreviousNode && name != PPreviousNode->TrackName)
        PPreviousNode->SetTrackName(name);
    if (PNextNode && name != PNextNode->TrackName)
        PNextNode->SetTrackName(name);
}
int main()
{
    CThingTrackNode n[5];
    for (int i = 0; i < 5; ++i) { n[i].PPreviousNode = i ? &n[i - 1] : 0; n[i].PNextNode = i < 4 ? &n[i + 1] : 0; }
    n[2].SetTrackName(CCharString("RIVER"));
    for (int i = 0; i < 5; ++i) if (std::strcmp(n[i].TrackName.Text, "RIVER")) { std::printf("00836ee0_TEST FAIL\n"); return 1; }
    if (g_sets != 5) { std::printf("00836ee0_TEST FAIL\n"); return 1; }
    n[4].PNextNode = &n[0]; n[0].PPreviousNode = &n[4];          // a closed loop terminates on equal names
    g_sets = 0;
    n[1].SetTrackName(CCharString("LAKE"));
    for (int i = 0; i < 5; ++i) if (std::strcmp(n[i].TrackName.Text, "LAKE")) { std::printf("00836ee0_TEST FAIL\n"); return 1; }
    std::printf("00836ee0_TEST PASS\n");
    return 0;
}