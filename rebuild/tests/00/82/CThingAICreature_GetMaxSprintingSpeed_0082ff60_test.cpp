// Behaviour test for 0x0082FF60 CThingAICreature::GetMaxSprintingSpeed(const C3DVector&) const: forwards to the
// virtual GetMaxRunningSpeed with the same direction.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CreatureBase {
public:
    virtual ~CreatureBase() { }
    virtual float GetMaxRunningSpeed(const C3DVector& d) const { return d.X * 2.0f; }
    virtual float GetMaxSprintingSpeed(const C3DVector& d) const { return d.X * 10.0f; }
};
class AICreature : public CreatureBase {
public:
    virtual float GetMaxSprintingSpeed(const C3DVector& d) const { float s = GetMaxRunningSpeed(d); return s; }
};
class FastAICreature : public AICreature {
public:
    mutable const C3DVector* seen;
    virtual float GetMaxRunningSpeed(const C3DVector& d) const { seen = &d; return 7.5f; }
};

int main()
{
    C3DVector d; d.X = 3.0f; d.Y = 0.0f; d.Z = 0.0f;
    AICreature a;
    const CreatureBase& ra = a;
    if (ra.GetMaxSprintingSpeed(d) != 6.0f) { std::printf("0082ff60_TEST FAIL\n"); return 1; }
    FastAICreature f; f.seen = 0;
    const CreatureBase& rf = f;
    if (rf.GetMaxSprintingSpeed(d) != 7.5f || f.seen != &d) { std::printf("0082ff60_TEST FAIL\n"); return 1; }
    std::printf("0082ff60_TEST PASS\n");
    return 0;
}