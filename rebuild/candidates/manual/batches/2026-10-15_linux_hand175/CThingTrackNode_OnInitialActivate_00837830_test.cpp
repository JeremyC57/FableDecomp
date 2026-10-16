#include <cstdio>
struct CThing;
static CThing* g_things[4];
struct CThingUIDMapOwner {
    CThing* GetThingWithUID(long long uid) { return (uid >= 1 && uid <= 3) ? g_things[uid] : 0; }
};
static CThingUIDMapOwner g_owner;
static CThingUIDMapOwner* g_pThingUIDMapOwner = &g_owner;
static int g_baseCalls = 0;
struct CThing { int dummy; void OnInitialActivate() { ++g_baseCalls; } };
struct CThingTrackNode : CThing {
    CThingTrackNode* PNextNode;
    CThingTrackNode* PPreviousNode;
    long long NextNodeUID, PreviousNodeUID;
    bool IsStart, IsEnd;
    void OnInitialActivate();
};
void CThingTrackNode::OnInitialActivate()
{
    CThing::OnInitialActivate();
    if (NextNodeUID != 0)
        PNextNode = (CThingTrackNode*)g_pThingUIDMapOwner->GetThingWithUID(NextNodeUID);
    else {
        PNextNode = 0;
        IsStart = false;
    }
    if (PreviousNodeUID != 0)
        PPreviousNode = (CThingTrackNode*)g_pThingUIDMapOwner->GetThingWithUID(PreviousNodeUID);
    else {
        PPreviousNode = 0;
        IsEnd = false;
    }
    if (PNextNode == 0 && PPreviousNode == 0) {
        IsStart = true;
        IsEnd = true;
    }
}
static int Fail() { std::printf("00837830_TEST FAIL\n"); return 1; }
int main()
{
    CThingTrackNode a, b, c;
    g_things[1] = &a; g_things[2] = &b; g_things[3] = &c;
    CThingTrackNode n; n.PNextNode = &n; n.PPreviousNode = &n;
    n.NextNodeUID = 2; n.PreviousNodeUID = 0; n.IsStart = true; n.IsEnd = true;
    n.OnInitialActivate();
    if (n.PNextNode != &b || n.PPreviousNode != 0 || !n.IsStart || n.IsEnd || g_baseCalls != 1) return Fail();
    n.NextNodeUID = 0; n.PreviousNodeUID = 3; n.IsStart = true; n.IsEnd = false;
    n.OnInitialActivate();
    if (n.PNextNode != 0 || n.PPreviousNode != &c || n.IsStart || n.IsEnd) return Fail();
    n.NextNodeUID = 0; n.PreviousNodeUID = 0;
    n.OnInitialActivate();
    if (n.PNextNode != 0 || n.PPreviousNode != 0 || !n.IsStart || !n.IsEnd) return Fail();
    n.NextNodeUID = 9; n.PreviousNodeUID = 0x100000000LL; n.IsStart = false; n.IsEnd = false;   // unknown UIDs resolve to null
    n.OnInitialActivate();
    if (n.PNextNode != 0 || n.PPreviousNode != 0 || !n.IsStart || !n.IsEnd) return Fail();
    std::printf("00837830_TEST PASS\n");
    return 0;
}
