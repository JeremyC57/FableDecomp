// behaviour test for 0x00860520 CVillageTaskCarryCrate::IsThingInvolvedWithTask (model; the intelligent pointer is a plain holder)
#include <cstdio>
class CThing { public: virtual ~CThing() { } };
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };
class CVillageTaskCarryCrate {
public:
    bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
    CIntelligentPointer<CThing> PCrateStack;
};
bool CVillageTaskCarryCrate::IsThingInvolvedWithTask(const CThing& thing) const
{
    return CreatureAssignedToTask.GetPItem() == &thing || PCrateStack.GetPItem() == &thing;
}
int main()
{
    CThing a; CThing b; CThing c;
    CVillageTaskCarryCrate t; t.CreatureAssignedToTask.Item = &a; t.PCrateStack.Item = &b;
    int ok = t.IsThingInvolvedWithTask(a) && t.IsThingInvolvedWithTask(b) && !t.IsThingInvolvedWithTask(c);
    t.CreatureAssignedToTask.Item = 0; t.PCrateStack.Item = 0;
    ok = ok && !t.IsThingInvolvedWithTask(c);
    if (!ok) { std::printf("00860520_TEST FAIL\n"); return 1; }
    std::printf("00860520_TEST PASS\n");
    return 0;
}