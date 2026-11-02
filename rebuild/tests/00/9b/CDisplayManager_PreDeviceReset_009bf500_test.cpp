// Behaviour test for 0x009BF500 CDisplayManager::PreDeviceReset: the render manager is uninitialised only when present, the
// vsync timer always, every registered callback once in list order, and the system manager last.
#include <cstdio>
static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
static void Reset() { g_n = 0; g_log[0] = 0; }

class CRenderManager { public: void Uninit() { Log('R'); } };
class CVsyncCallbackTimer { public: void OnPreDeviceReset() { Log('V'); } };
class CSystemManager { public: void OnPreDeviceReset() { Log('S'); } };
class CDeviceResetCallback { public: char Tag; virtual void OnPreDeviceReset() { Log(Tag); } virtual bool OnPostDeviceReset() { return true; } };
struct Node { Node* Next; Node* Prev; CDeviceResetCallback* Data; };
class CDisplayManager { public:
    CSystemManager* SystemManager;
    CRenderManager* PRenderManager;
    Node* DeviceResetCallbacks;              // sentinel
    CVsyncCallbackTimer* VsyncCallbackTimer;
    void PreDeviceReset()
    {
        if (PRenderManager)
            PRenderManager->Uninit();
        VsyncCallbackTimer->OnPreDeviceReset();
        for (Node* n = DeviceResetCallbacks->Next; n != DeviceResetCallbacks; n = n->Next)
            n->Data->OnPreDeviceReset();
        SystemManager->OnPreDeviceReset();
    }
};

int main()
{
    CRenderManager rm; CVsyncCallbackTimer vt; CSystemManager sm;
    CDeviceResetCallback a, b; a.Tag = 'a'; b.Tag = 'b';
    Node head, n1, n2;
    head.Next = &n1; n1.Next = &n2; n2.Next = &head; head.Prev = &n2; n2.Prev = &n1; n1.Prev = &head;
    n1.Data = &a; n2.Data = &b;
    CDisplayManager dm; dm.SystemManager = &sm; dm.PRenderManager = &rm; dm.DeviceResetCallbacks = &head; dm.VsyncCallbackTimer = &vt;
    dm.PreDeviceReset();
    int ok = g_log[0] == 'R' && g_log[1] == 'V' && g_log[2] == 'a' && g_log[3] == 'b' && g_log[4] == 'S' && g_log[5] == 0;
    Reset(); dm.PRenderManager = 0; head.Next = &head; head.Prev = &head;
    dm.PreDeviceReset();
    ok = ok && g_log[0] == 'V' && g_log[1] == 'S' && g_log[2] == 0;
    if (!ok) { std::printf("009bf500_TEST FAIL\n"); return 1; }
    std::printf("009bf500_TEST PASS\n");
    return 0;
}