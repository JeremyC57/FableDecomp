// behaviour test for 0x00860350 CVillageTaskHorn::IsThingInvolvedWithTask (model; the intelligent pointer is a plain holder)
#include <cstdio>
class CThing { public: virtual ~CThing() { } };
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };
class CVillageTaskHorn {
public:
    bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<CThing> PHorn;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
};
bool CVillageTaskHorn::IsThingInvolvedWithTask(const CThing& thing) const
{
    return PHorn.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;
}
int main()
{
    CThing a; CThing b; CThing c;
    CVillageTaskHorn t; t.PHorn.Item = &a; t.CreatureAssignedToTask.Item = &b;
    int ok = t.IsThingInvolvedWithTask(a) && t.IsThingInvolvedWithTask(b) && !t.IsThingInvolvedWithTask(c);
    t.PHorn.Item = 0; t.CreatureAssignedToTask.Item = 0;
    ok = ok && !t.IsThingInvolvedWithTask(c);
    if (!ok) { std::printf("00860350_TEST FAIL\n"); return 1; }
    std::printf("00860350_TEST PASS\n");
    return 0;
}