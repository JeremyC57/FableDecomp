// Behaviour test for CAIStateGroup_WorkHammerAnvil::IsRunnable (0x00944E80): same logic over mocks.
#include <cstdio>
#include <list>
class C3DVector { public: float x, y, z; };
class CThing { public:
    bool dead; unsigned char thingClass; unsigned int props; bool hasActionPoint; bool apFree; C3DVector pos;
};
static CThing MakeThing(float x, float y, float z)
{
    CThing t; t.dead = false; t.thingClass = 5; t.props = 0x40; t.hasActionPoint = true; t.apFree = true;
    t.pos.x = x; t.pos.y = y; t.pos.z = z; return t;
}
struct CState {
    bool inWorkHours; int frame; int NextFrameHammer; bool hasWorkplace; bool workplaceIsOwner;
    std::list<CThing*> owned; CThing* creature; CThing* PAnvil;
    bool IsRunnable()
    {
        if (inWorkHours && NextFrameHammer <= frame) {
            if (hasWorkplace && workplaceIsOwner) {
                CThing* best = 0;
                float bestDistance = 0.0f;
                std::list<CThing*> objects(owned);
                for (std::list<CThing*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                    CThing* anvil = *it;
                    if (anvil && !anvil->dead && anvil->thingClass == 5 && ((unsigned char)(anvil->props >> 6) & 1)
                        && anvil->hasActionPoint && anvil->apFree) {
                        float dx = creature->pos.x - anvil->pos.x, dy = creature->pos.y - anvil->pos.y, dz = creature->pos.z - anvil->pos.z;
                        float distance = dx * dx + dy * dy + dz * dz;
                        if (best == 0 || distance < bestDistance) { bestDistance = distance; best = anvil; }
                    }
                }
                if (best) { PAnvil = best; return true; }
            }
        }
        return false;
    }
};
int main()
{
    CThing me = MakeThing(0, 0, 0);
    CThing far1 = MakeThing(10, 0, 0), near1 = MakeThing(0, 2, 1), deadOne = MakeThing(0, 0, 0.5f), busy = MakeThing(0.1f, 0, 0);
    CThing noProp = MakeThing(0.2f, 0, 0), wrongClass = MakeThing(0.3f, 0, 0);
    deadOne.dead = true; busy.apFree = false; noProp.props = 0x80; wrongClass.thingClass = 3;
    CState s; s.inWorkHours = true; s.frame = 100; s.NextFrameHammer = 100; s.hasWorkplace = true; s.workplaceIsOwner = true;
    s.creature = &me; s.PAnvil = 0;
    s.owned.push_back(&far1); s.owned.push_back(0); s.owned.push_back(&deadOne); s.owned.push_back(&near1);
    s.owned.push_back(&busy); s.owned.push_back(&noProp); s.owned.push_back(&wrongClass);
    bool ok = s.IsRunnable() && s.PAnvil == &near1;
    s.PAnvil = 0; s.NextFrameHammer = 101; ok = ok && !s.IsRunnable() && s.PAnvil == 0;
    s.NextFrameHammer = 50; s.inWorkHours = false; ok = ok && !s.IsRunnable();
    s.inWorkHours = true; s.workplaceIsOwner = false; ok = ok && !s.IsRunnable();
    s.workplaceIsOwner = true; s.owned.clear(); s.owned.push_back(&busy); ok = ok && !s.IsRunnable() && s.PAnvil == 0;
    if (!ok) { std::printf("00944e80_TEST FAIL\n"); return 1; }
    std::printf("00944e80_TEST PASS\n");
    return 0;
}