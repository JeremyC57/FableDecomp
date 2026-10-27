// Behaviour test for 0x00918050 CIsFreeBench::operator()(const CIntelligentPointer<CThing>&) const: an object (type 5)
// with kind bit 17 (bench) and a user-action-point interface (0x2D) whose point is free.
#include <cstdio>

class ActionPoint { public: bool free; bool IsActionPointFree() { return free; } };
class Thing {
public:
    unsigned char type; unsigned long kindFlags; bool hasActionPoint; ActionPoint* actionPoint;
    unsigned long GetStateFlags() const { return kindFlags; }
};
class ThingPointer { public: Thing* item; Thing* GetPItem() const { return item; } };
class IsFreeBench {
public:
    bool operator()(const ThingPointer& p) const
    {
        if (p.GetPItem() && p.GetPItem()->type == 5 && ((unsigned char)(p.GetPItem()->GetStateFlags() >> 17) & 1)) {
            if (p.GetPItem()->hasActionPoint)
                return p.GetPItem()->actionPoint->IsActionPointFree();
        }
        return false;
    }
};

int main()
{
    IsFreeBench pred;
    ActionPoint ap; ap.free = true;
    Thing bench; bench.type = 5; bench.kindFlags = 1UL << 17; bench.hasActionPoint = true; bench.actionPoint = &ap;
    ThingPointer p; p.item = &bench;
    if (!pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    ap.free = false;
    if (pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    ap.free = true; bench.kindFlags = 1UL << 16;
    if (pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    bench.kindFlags = 1UL << 17; bench.type = 4;
    if (pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    bench.type = 5; bench.hasActionPoint = false;
    if (pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    p.item = 0;
    if (pred(p)) { std::printf("00918050_TEST FAIL\n"); return 1; }
    std::printf("00918050_TEST PASS\n");
    return 0;
}