#include <cstdio>
// Behaviour test for CAIStateGroup_LightHomeCandle::InitStateGroup (0x00946620): the override does nothing.
class CWorld {};
class CThingAICreature {};
class CAIStateGroup_LightHomeCandle {
public:
    int touched;
    CAIStateGroup_LightHomeCandle() : touched(0) {}
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature) {}
public:
    void Init(const CWorld& w, CThingAICreature& c) { InitStateGroup(w, c); }
};
int main()
{
    CWorld w; CThingAICreature c; CAIStateGroup_LightHomeCandle g;
    g.Init(w, c);
    if (g.touched != 0) { std::printf("AUTO_TINY_00946620_TEST FAIL\n"); return 1; }
    std::printf("AUTO_TINY_00946620_TEST PASS\n");
    return 0;
}
