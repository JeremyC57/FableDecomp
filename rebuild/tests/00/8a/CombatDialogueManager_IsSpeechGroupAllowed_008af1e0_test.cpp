#include <cstdio>
// Behaviour test: IsSpeechGroupAllowed compares the world frame with the found group's NextAllowedFrame.
struct TSpeechGroupInfo { long NextAllowedFrame; };
struct SNode { unsigned long first; TSpeechGroupInfo second; };
struct CIter { SNode* _M_node; SNode* operator->() const { return _M_node; } };
class CMap { public:
    SNode* nodes; int count;
    CIter find(const unsigned long& key) const
    {
        CIter it; it._M_node = 0;
        int i = 0;
        for (i = 0; i < count; ++i) if (nodes[i].first == key) it._M_node = &nodes[i];
        return it;
    }
};
class CWorld { public: long frame; long GetFrameNumber() { return frame; } };
static CWorld g_world;
static CWorld* g_pWorld = &g_world;
class CombatDialogueManager { public:
    CMap SpeechGroupInfoMap;
    bool IsSpeechGroupAllowed(unsigned long group) const
    {
        const long frame = g_pWorld->GetFrameNumber();
        CIter it = SpeechGroupInfoMap.find(group);
        bool allowed = frame >= it->second.NextAllowedFrame;
        return allowed;
    }
};
int main()
{
    SNode n[2]; n[0].first = 4; n[0].second.NextAllowedFrame = 100; n[1].first = 9; n[1].second.NextAllowedFrame = 50;
    CombatDialogueManager m; m.SpeechGroupInfoMap.nodes = n; m.SpeechGroupInfoMap.count = 2;
    g_world.frame = 99;
    if (m.IsSpeechGroupAllowed(4) || !m.IsSpeechGroupAllowed(9)) { std::printf("008af1e0_TEST FAIL\n"); return 1; }
    g_world.frame = 100;
    if (!m.IsSpeechGroupAllowed(4)) { std::printf("008af1e0_TEST FAIL\n"); return 1; }
    std::printf("008af1e0_TEST PASS\n");
    return 0;
}