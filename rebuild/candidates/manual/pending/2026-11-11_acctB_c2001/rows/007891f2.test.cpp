// Behaviour test for 0x007891F2 CTCTrapBase::BulletTimeFrameUpdate(): the bullet-time update runs the (virtual) FrameUpdate,
// so a derived trap's FrameUpdate override is what executes.
#include <cstdio>

class CTCTrapBaseModel {
public:
    CTCTrapBaseModel() : BaseFrames(0) {}
    virtual ~CTCTrapBaseModel() {}
    virtual bool HasFrameUpdate() const { return true; }
    virtual void FrameUpdate() { ++BaseFrames; }
    virtual void BulletTimeFrameUpdate() { FrameUpdate(); }          // 0x007891F2
    int BaseFrames;
};

class CTrapModel : public CTCTrapBaseModel {
public:
    CTrapModel() : DerivedFrames(0) {}
    virtual void FrameUpdate() { ++DerivedFrames; }
    int DerivedFrames;
};

int main()
{
    CTCTrapBaseModel base;
    base.BulletTimeFrameUpdate();
    base.BulletTimeFrameUpdate();
    if (base.BaseFrames != 2) { std::printf("007891f2_TEST FAIL\n"); return 1; }

    CTrapModel trap;
    CTCTrapBaseModel* p = &trap;
    p->BulletTimeFrameUpdate();
    if (trap.DerivedFrames != 1 || trap.BaseFrames != 0) { std::printf("007891f2_TEST FAIL\n"); return 1; }

    std::printf("007891f2_TEST PASS\n");
    return 0;
}
