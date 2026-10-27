// behaviour test for 0x00860390 CVillageTaskPatrol::IsThingInvolvedWithTask (model; the intelligent pointer is a plain holder)
#include <cstdio>
class CThing { public: virtual ~CThing() { } };
class CThingTrackNode : public CThing { };
template <class T> class CIntelligentPointer { public: T* Item; CIntelligentPointer() : Item(0) { } T* GetPItem() const { return Item; } };
class CVillageTaskPatrol {
public:
    bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<const CThingTrackNode> PatrolStart;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
};
bool CVillageTaskPatrol::IsThingInvolvedWithTask(const CThing& thing) const
{
    return PatrolStart.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;
}
int main()
{
    CThingTrackNode a; CThing b; CThing c;
    CVillageTaskPatrol t; t.PatrolStart.Item = &a; t.CreatureAssignedToTask.Item = &b;
    int ok = t.IsThingInvolvedWithTask(a) && t.IsThingInvolvedWithTask(b) && !t.IsThingInvolvedWithTask(c);
    t.PatrolStart.Item = 0; t.CreatureAssignedToTask.Item = 0;
    ok = ok && !t.IsThingInvolvedWithTask(c);
    if (!ok) { std::printf("00860390_TEST FAIL\n"); return 1; }
    std::printf("00860390_TEST PASS\n");
    return 0;
}