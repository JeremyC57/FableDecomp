// Behaviour test for CTCBed::FrameUpdate (0x00746B20): updates the bed animation only when the def says the bed animates.
#include <cstdio>
class CBedDef { public: bool Animating; };
class CTCBed { public:
    const CBedDef* PDef;
    int AnimUpdates;
    void UpdateAnimation() { ++AnimUpdates; }
    void FrameUpdate() { if (PDef->Animating) UpdateAnimation(); }
};
int main()
{
    CBedDef d; CTCBed b; b.PDef = &d; b.AnimUpdates = 0;
    d.Animating = false; b.FrameUpdate();
    d.Animating = true; b.FrameUpdate(); b.FrameUpdate();
    if (b.AnimUpdates != 2) { std::printf("00746b20_TEST FAIL\n"); return 1; }
    std::printf("00746b20_TEST PASS\n");
    return 0;
}