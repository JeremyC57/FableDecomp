// Behaviour test for CTCTargetingAI::BulletTimeFrameUpdate (0x004DDAA4): it runs the component's (virtual) FrameUpdate once.
#include <stdio.h>
class CTCTargetingAI { public:
    int frameUpdates;
    CTCTargetingAI() : frameUpdates(0) { }
    virtual ~CTCTargetingAI() { }
    virtual void FrameUpdate() { ++frameUpdates; }
    virtual void BulletTimeFrameUpdate();
};
void CTCTargetingAI::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
struct Derived : CTCTargetingAI {
    int derivedUpdates;
    Derived() : derivedUpdates(0) { }
    virtual void FrameUpdate() { ++derivedUpdates; }
};
int main(void)
{
    CTCTargetingAI a; a.BulletTimeFrameUpdate(); a.BulletTimeFrameUpdate();
    Derived d; d.BulletTimeFrameUpdate();
    if (a.frameUpdates != 2 || d.derivedUpdates != 1 || d.frameUpdates != 0) { printf("004ddaa4_TEST FAIL\n"); return 1; }
    printf("004ddaa4_TEST PASS\n");
    return 0;
}