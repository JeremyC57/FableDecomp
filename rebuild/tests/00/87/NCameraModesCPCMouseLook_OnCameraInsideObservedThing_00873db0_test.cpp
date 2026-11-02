// Behaviour test for 0x00873DB0 NCameraModes::CPCMouseLook::OnCameraInsideObservedThing: the current camera state is reset to the
// last one, every queued mouse input node is freed (the list ends empty), then the base FrameUpdate runs.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0, g_frameUpdates = 0;
struct Node { Node* next; Node* prev; float x, y; };
class CBase { public:
    virtual ~CBase() { }
    virtual void OnCameraInsideObservedThing() { }
    void FrameUpdate() { ++g_frameUpdates; }
};
struct CState { float AngleXY, AngleUpDown, DollyLength, DeltaAngleXY; long RecentreTimer; float ViewPosOffset[3]; };
class CPCMouseLook : public CBase { public:
    Node*  MouseInputs;                 // the list's sentinel
    CState StateCurrent;
    CState StateLast;
    virtual void OnCameraInsideObservedThing()
    {
        StateCurrent = StateLast;
        Node* cur = MouseInputs->next;
        while (cur != MouseInputs) { Node* tmp = cur; cur = cur->next; ++g_frees; std::free(tmp); }
        MouseInputs->next = MouseInputs;
        MouseInputs->prev = MouseInputs;
        FrameUpdate();
    }
};
int main()
{
    Node sentinel; sentinel.next = sentinel.prev = &sentinel;
    for (int i = 0; i < 3; ++i) {                       // push_back three inputs
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->x = (float)i; n->y = 0.0f; n->next = &sentinel; n->prev = sentinel.prev;
        sentinel.prev->next = n; sentinel.prev = n;
    }
    CPCMouseLook look;
    look.MouseInputs = &sentinel;
    look.StateCurrent.AngleXY = 1.0f; look.StateCurrent.RecentreTimer = 5;
    look.StateLast.AngleXY = 2.5f; look.StateLast.RecentreTimer = 9; look.StateLast.ViewPosOffset[2] = 3.0f;
    CBase* base = &look;
    base->OnCameraInsideObservedThing();
    int ok = g_frees == 3 && g_frameUpdates == 1 && sentinel.next == &sentinel && sentinel.prev == &sentinel
          && look.StateCurrent.AngleXY == 2.5f && look.StateCurrent.RecentreTimer == 9 && look.StateCurrent.ViewPosOffset[2] == 3.0f;
    base->OnCameraInsideObservedThing();                 // empty list: nothing freed
    if (g_frees != 3 || g_frameUpdates != 2) ok = 0;
    if (!ok) { std::printf("00873db0_TEST FAIL\n"); return 1; }
    std::printf("00873db0_TEST PASS\n");
    return 0;
}