// Behaviour test for 004d5372 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class C3DVector { public: float X, Y, Z; };
class CTCOnHeroPush {
public:
    virtual ~CTCOnHeroPush() { }
    virtual void OnInitialActivate() { FrameSincePushed = 0; DistPushed = 0.0f; }
    void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; float DistPushed; C3DVector PushDir; int FrameSincePushed;
};
int main() {
    CTCOnHeroPush h; h.FrameSincePushed = 12; h.DistPushed = 3.5f; h.PushDir.X = 1.0f;
    CTCOnHeroPush* p = &h; p->OnInitialActivate();
    if (h.FrameSincePushed != 0 || h.DistPushed != 0.0f || h.PushDir.X != 1.0f) { std::printf("004d5372_TEST FAIL\n"); return 1; }
    { std::printf("004d5372_TEST PASS\n"); return 0; }
}