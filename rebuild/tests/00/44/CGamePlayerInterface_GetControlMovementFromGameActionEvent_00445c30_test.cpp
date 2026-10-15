// Behaviour test for 0x00445C30 CGamePlayerInterface::GetControlMovementFromGameActionEvent.
#include <cstdio>
struct C2DVector { float X, Y; };
struct CInputEvent { int code; };
struct CInputTypeBase {
    int accepts; C2DVector move; int calls;
    bool RecognisesEvent(const CInputEvent& e) { ++calls; return e.code == accepts; }
    C2DVector GetMovement(const CInputEvent&) { return move; }
};
struct SControlEntry { int first; CInputTypeBase* second; };
struct CControlsMap {
    SControlEntry e[4]; int n;
    SControlEntry* LowerBound(const int* k) { int i = 0; while (i < n && e[i].first < *k) ++i; return e + i; }
    SControlEntry* UpperBound(const int* k) { int i = 0; while (i < n && e[i].first <= *k) ++i; return e + i; }
};
struct CGamePlayerInterface {
    CControlsMap Controls;
    bool Get(int action, const CInputEvent& event, C2DVector& movement)
    {
        SControlEntry* end = Controls.UpperBound(&action);
        for (SControlEntry* it = Controls.LowerBound(&action); it != end; ++it) {
            CInputTypeBase* inputType = it->second;
            if (inputType->RecognisesEvent(event)) { movement = inputType->GetMovement(event); return true; }
        }
        return false;
    }
};
static int Fail() { std::printf("00445c30_TEST FAIL\n"); return 1; }
int main()
{
    CInputTypeBase a = { 1, { 1.0f, 0.0f }, 0 }, b = { 2, { 0.0f, 2.0f }, 0 }, c = { 2, { 9.0f, 9.0f }, 0 };
    CGamePlayerInterface g; g.Controls.n = 3;
    g.Controls.e[0].first = 4; g.Controls.e[0].second = &c;
    g.Controls.e[1].first = 5; g.Controls.e[1].second = &a;
    g.Controls.e[2].first = 5; g.Controls.e[2].second = &b;
    C2DVector out = { -1.0f, -1.0f };
    CInputEvent ev = { 2 };
    if (!g.Get(5, ev, out) || out.X != 0.0f || out.Y != 2.0f || a.calls != 1 || c.calls != 0) return Fail();
    CInputEvent none = { 7 };
    out.X = -1.0f;
    if (g.Get(5, none, out) || out.X != -1.0f) return Fail();
    if (g.Get(6, ev, out)) return Fail();
    std::printf("00445c30_TEST PASS\n");
    return 0;
}