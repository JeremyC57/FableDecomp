// Behaviour test for CTCInventory::CloseInventory @ 0x00589D30 (shut the inventory and restore game state)
#include <cstdio>
#include <vector>

struct Comp { bool Active; int Deactivations; Comp() : Active(true), Deactivations(0) {} };
struct Inv {
    bool Open; bool Paused; bool Rendering; int Handles; std::vector<int*> Screens;
    int Modal; Comp* Modes[5]; int InputA; int InputB; long CloseFrame;
    float Music; float Sound; int Style; int RefCount; int Destroyed;
};
static void Close(Inv& v, long frame, float music, float sound)
{
    v.Open = false;
    v.Handles = 0;
    for (unsigned i = 0; i < v.Screens.size(); ++i) delete v.Screens[i];
    v.Screens.clear();
    v.Modal = 0;
    for (int i = 0; i < 5; ++i)
        if (v.Modes[i] && v.Modes[i]->Active) { v.Modes[i]->Active = false; ++v.Modes[i]->Deactivations; }
    v.InputA = 0; v.InputB = 0;
    v.Rendering = true;
    v.CloseFrame = frame;
    v.Music = music; v.Sound = sound;
    ++v.RefCount;                                    // temporary style def pointer
    v.Style = 7;
    if (--v.RefCount == 0) ++v.Destroyed;
    v.Paused = false;
}
int main()
{
    Inv v; v.Open = true; v.Paused = true; v.Rendering = false; v.Handles = 3;
    v.Screens.push_back(new int(1)); v.Screens.push_back(new int(2));
    v.Modal = 1; Comp a, b; b.Active = false;
    v.Modes[0] = &a; v.Modes[1] = 0; v.Modes[2] = &b; v.Modes[3] = 0; v.Modes[4] = 0;
    v.InputA = 4; v.InputB = 5; v.CloseFrame = 0; v.Music = 0; v.Sound = 0; v.Style = 0; v.RefCount = 1; v.Destroyed = 0;
    Close(v, 1234, 0.5f, 0.75f);
    if (v.Open || v.Paused || !v.Rendering || !v.Screens.empty() || a.Active || a.Deactivations != 1 || b.Deactivations != 0 ||
        v.CloseFrame != 1234 || v.Music != 0.5f || v.Style != 7 || v.RefCount != 1 || v.Destroyed != 0) { std::printf("00589d30_TEST FAIL\n"); return 1; }
    std::printf("00589d30_TEST PASS\n");
    return 0;
}