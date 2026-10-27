// behaviour test for 0x00860310 CVillageTaskTorch::IsThingInvolvedWithTask (model; the intelligent pointer is a plain holder)
#include <cstdio>
class CThing { public: virtual ~CThing() { } };
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };
class CVillageTaskTorch {
public:
    bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<CThing> Torch;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
};
bool CVillageTaskTorch::IsThingInvolvedWithTask(const CThing& thing) const
{
    return Torch.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;
}
int main()
{
    CThing a; CThing b; CThing c;
    CVillageTaskTorch t; t.Torch.Item = &a; t.CreatureAssignedToTask.Item = &b;
    int ok = t.IsThingInvolvedWithTask(a) && t.IsThingInvolvedWithTask(b) && !t.IsThingInvolvedWithTask(c);
    t.Torch.Item = 0; t.CreatureAssignedToTask.Item = 0;
    ok = ok && !t.IsThingInvolvedWithTask(c);
    if (!ok) { std::printf("00860310_TEST FAIL\n"); return 1; }
    std::printf("00860310_TEST PASS\n");
    return 0;
}