// 00944730 behaviour test: CAIStateGroup_WorkSawingWood::IsRunnable (final.cpp body over mock components)
#include <stdio.h>
#include <list>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CThing;
struct CTCUserActionPointBase { bool Free; bool IsActionPointFree() { return Free; } };
class CTCThingOwner { public: std::list<CThing*> ObjectList; };
class CThing { public:
    unsigned char m_flags91; int ThingClass; unsigned Properties; float X, Y;
    CTCUserActionPointBase* Point; CTCThingOwner* Owner; int Mode; CThing* Workplace;
    unsigned GetProperties() { return Properties; }
};
struct CAIStateGroup_WorkSawingWood {
    CThing* Creature; bool WorkHours; CThing* PSawHorse;
    bool IsInWorkHours() { return WorkHours; }
    bool IsRunnable();
};
bool CAIStateGroup_WorkSawingWood::IsRunnable()
{
    if (IsInWorkHours()) {
        long mode = Creature->Mode;
        if (mode != 0x22 && mode != 6) {
            CThing* workplace = Creature->Workplace;
            CTCThingOwner* owner;
            if (workplace && (owner = workplace->Owner) != 0) {
                CThing* best = 0;
                float bestDistance = 0.0f;
                std::list<CThing*> objects(owner->ObjectList);
                for (std::list<CThing*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                    CThing* horse = *it;
                    CTCUserActionPointBase* point;
                    if (horse && !(horse->m_flags91 & 1) && horse->ThingClass == 5
                        && (unsigned char)(horse->GetProperties() >> 4) & 1
                        && (point = horse->Point) != 0
                        && point->IsActionPointFree()) {
                        float dx = Creature->X - horse->X, dy = Creature->Y - horse->Y;
                        float distance = dx * dx + dy * dy;
                        if (best == 0 || distance < bestDistance) {
                            bestDistance = distance;
                            best = horse;
                        }
                    }
                }
                if (best) {
                    PSawHorse = best;
                    return true;
                }
            }
        }
    }
    return false;
}
static CThing mk(int cls, unsigned props, float x, CTCUserActionPointBase* p)
{
    CThing t = { 0, cls, props, x, 0.0f, p, 0, 0, 0 };
    return t;
}
int main()
{
    CTCUserActionPointBase freeP = { true }, busyP = { false };
    CThing far1 = mk(5, 0x10, 10.0f, &freeP), nearBusy = mk(5, 0x10, 1.0f, &busyP), near2 = mk(5, 0x10, 3.0f, &freeP),
           wrongClass = mk(4, 0x10, 0.5f, &freeP), noBit = mk(5, 0x00, 0.5f, &freeP), dead = mk(5, 0x10, 0.2f, &freeP);
    dead.m_flags91 = 1;
    CTCThingOwner owner;
    owner.ObjectList.push_back(&far1); owner.ObjectList.push_back(&nearBusy); owner.ObjectList.push_back(&near2);
    owner.ObjectList.push_back(&wrongClass); owner.ObjectList.push_back(&noBit); owner.ObjectList.push_back(&dead);
    owner.ObjectList.push_back(0);
    CThing work = mk(0, 0, 0, 0); work.Owner = &owner;
    CThing me = mk(0, 0, 0.0f, 0); me.Workplace = &work;
    CAIStateGroup_WorkSawingWood g = { &me, true, 0 };
    CHECK(g.IsRunnable() && g.PSawHorse == &near2);
    me.Mode = 6; g.PSawHorse = 0;
    CHECK(!g.IsRunnable() && g.PSawHorse == 0);
    me.Mode = 0; g.WorkHours = false;
    CHECK(!g.IsRunnable());
    g.WorkHours = true; near2.Point = &busyP; far1.Point = &busyP;
    CHECK(!g.IsRunnable() && g.PSawHorse == 0);
    me.Workplace = 0;
    CHECK(!g.IsRunnable());
    if (!g_fail) printf("00944730_TEST PASS\n");
    return g_fail;
}