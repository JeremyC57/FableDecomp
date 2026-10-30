// Behaviour test for 0x007A9B10 CTCAssassinRush::ResetCameraOnStick (see src/007a9b10.cpp): when the world's current game
// camera yields a camera manager, a new camera-on-a-stick module (parent thing + the two camera vectors) is handed to it,
// not preloaded; without a manager nothing is built.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Module { void* Thing; Vec Offset; Vec LookAt; };
struct Manager { Module* Last; bool Preload; int Calls; void SetMode(Module* m, bool p) { Last = m; Preload = p; ++Calls; } };
struct Camera { Manager* M; bool Get(Manager** out) { if (!M) return false; *out = M; return true; } };
struct Rush {
    void* ParentThing; Camera* Cam; Vec Local, Relative;
    void ResetCameraOnStick()
    {
        Manager* manager;
        if (Cam->Get(&manager)) {
            Module* m = new Module; m->Thing = ParentThing; m->Offset = Local; m->LookAt = Relative;
            manager->SetMode(m, false);
        }
    }
};
int main()
{
    int thing = 0; Manager mgr = { 0, true, 0 }; Camera cam = { &mgr };
    Rush r = { &thing, &cam, { 1, 2, 3 }, { 4, 5, 6 } };
    r.ResetCameraOnStick();
    if (mgr.Calls != 1 || mgr.Preload || mgr.Last->Thing != &thing || mgr.Last->Offset.Y != 2 || mgr.Last->LookAt.Z != 6) { std::printf("007a9b10_TEST FAIL\n"); return 1; }
    delete mgr.Last;
    Camera none = { 0 }; r.Cam = &none; r.ResetCameraOnStick();
    if (mgr.Calls != 1) { std::printf("007a9b10_TEST FAIL\n"); return 1; }
    std::printf("007a9b10_TEST PASS\n");
    return 0;
}