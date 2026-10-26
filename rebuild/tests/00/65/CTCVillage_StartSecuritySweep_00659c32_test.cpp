#include <cstdio>
struct SSweepTaskNode { SSweepTaskNode* next; SSweepTaskNode* prev; void* Object; };
struct CSweepTaskList {
    SSweepTaskNode* _M_node;
    unsigned long size() const { unsigned long n = 0; for (SSweepTaskNode* p = _M_node->next; p != _M_node; p = p->next) ++n; return n; }
};
static long g_frame = 0;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
class CTCVillage { public:
    long FrameStartedSecuritySweep; CSweepTaskList SecuritySweepTasks; bool SecuritySweepUrgent; float InitialSecuritySweepRadius;
    int invalidated, calculated;
    const CWorld* GetWorld() const { return &g_world; }
    void InvalidateAndClearAllSecuritySweepTasks() { ++invalidated; SecuritySweepTasks._M_node->next = SecuritySweepTasks._M_node; }
    void CalculateSecuritySweepTasks() { ++calculated; }
    void StartSecuritySweep(bool urgent)
    {
        unsigned long queued = SecuritySweepTasks.size();
        SecuritySweepUrgent = urgent;
        if (queued != 0) InvalidateAndClearAllSecuritySweepTasks();
        else { FrameStartedSecuritySweep = GetWorld()->GetWorldFrame(); InitialSecuritySweepRadius = 20.0f; }
        CalculateSecuritySweepTasks();
    }
};
int main()
{
    SSweepTaskNode head, a; head.next = &head;
    CTCVillage v; v.SecuritySweepTasks._M_node = &head; v.invalidated = v.calculated = 0; v.FrameStartedSecuritySweep = -1; v.InitialSecuritySweepRadius = 0.0f; v.SecuritySweepUrgent = false;
    g_frame = 42; v.StartSecuritySweep(true);
    if (!v.SecuritySweepUrgent || v.FrameStartedSecuritySweep != 42 || v.InitialSecuritySweepRadius != 20.0f || v.invalidated != 0 || v.calculated != 1) { std::printf("00659c32_TEST FAIL\n"); return 1; }
    head.next = &a; a.next = &head; g_frame = 50; v.InitialSecuritySweepRadius = 3.0f;
    v.StartSecuritySweep(false);
    if (v.SecuritySweepUrgent || v.FrameStartedSecuritySweep != 42 || v.InitialSecuritySweepRadius != 3.0f || v.invalidated != 1 || v.calculated != 2) { std::printf("00659c32_TEST FAIL\n"); return 1; }
    std::printf("00659c32_TEST PASS\n");
    return 0;
}