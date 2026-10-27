// behaviour test for 0x008603D0 CVillageTaskPickCrops::IsThingInvolvedWithTask (model; the intelligent pointer is a plain holder)
#include <cstdio>
class CThing { public: virtual ~CThing() { } };
class CThingObject : public CThing { };
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };
class CVillageTaskPickCrops {
public:
    bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<CThingObject> Crop;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
};
bool CVillageTaskPickCrops::IsThingInvolvedWithTask(const CThing& thing) const
{
    return Crop.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;
}
int main()
{
    CThingObject a; CThing b; CThing c;
    CVillageTaskPickCrops t; t.Crop.Item = &a; t.CreatureAssignedToTask.Item = &b;
    int ok = t.IsThingInvolvedWithTask(a) && t.IsThingInvolvedWithTask(b) && !t.IsThingInvolvedWithTask(c);
    t.Crop.Item = 0; t.CreatureAssignedToTask.Item = 0;
    ok = ok && !t.IsThingInvolvedWithTask(c);
    if (!ok) { std::printf("008603d0_TEST FAIL\n"); return 1; }
    std::printf("008603d0_TEST PASS\n");
    return 0;
}