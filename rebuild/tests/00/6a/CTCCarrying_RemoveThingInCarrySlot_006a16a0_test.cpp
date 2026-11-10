// Behaviour test for 0x006A16A0 CTCCarrying::RemoveThingInCarrySlot(long slot, bool immediately).
#include <stdio.h>
#include <math.h>
#include <map>
#include <list>
class C3DVector { public: float X, Y, Z; };
class CThing { public: bool Dead, Dying, HasSolid, Carried, CarryingWeapon; int NotSolidTo; C3DVector Up, Forward; int Modes; };
class CTCCarrying { public:
    std::map<long, CThing*> CarrySlots; std::list<CThing*> ThingsInPrimarySlots; CThing* Parent; int RemovedModes; int Mode;
    CThing* RemoveThingInCarrySlot(long slot, bool)
    {
        ThingsInPrimarySlots.remove((CThing*)0);
        CThing* thing = 0;
        std::map<long, CThing*>::iterator it = CarrySlots.find(slot);
        if (it != CarrySlots.end()) {
            thing = it->second;
            if (thing && !thing->Dead) thing->Carried = false;
            it->second = 0;
            if (thing && !thing->Dead) {
                ThingsInPrimarySlots.remove(thing);
                if (!Parent->CarryingWeapon) RemovedModes |= 1;
                if (Mode == 34) {
                    Mode = 0; RemovedModes |= 2;
                    float dot = thing->Up.Z;
                    float angle = -((float)acos(dot) * 0.15915494f);
                    (void)angle;
                    thing->Up.X = 0.0f; thing->Up.Y = 0.0f; thing->Up.Z = 1.0f; thing->Forward.Z = 0.0f;
                    if (!thing->Dying) { thing->HasSolid = true; ++thing->NotSolidTo; }
                }
            }
        }
        return thing;
    }
};
int main()
{
    CThing parent = { false, false, false, false, false, 0, {0,0,1}, {0,1,0}, 0 };
    CThing box = { false, false, false, true, false, 0, {0.6f,0,0.8f}, {0,0.8f,0.6f}, 0 };
    CTCCarrying c; c.Parent = &parent; c.RemovedModes = 0; c.Mode = 34;
    c.CarrySlots[5] = &box; c.ThingsInPrimarySlots.push_back(0); c.ThingsInPrimarySlots.push_back(&box);
    int ok = c.RemoveThingInCarrySlot(9, false) == 0 && c.ThingsInPrimarySlots.size() == 1;
    ok = ok && c.RemoveThingInCarrySlot(5, false) == &box && c.CarrySlots[5] == 0 && !box.Carried && c.ThingsInPrimarySlots.empty()
            && c.RemovedModes == 3 && fabs(box.Up.Z - 1.0f) < 1e-6 && box.Forward.Z == 0.0f && box.NotSolidTo == 1;
    CThing dead = { true, false, false, true, false, 0, {0,0,1}, {0,1,0}, 0 };
    c.CarrySlots[6] = &dead; c.RemovedModes = 0;
    ok = ok && c.RemoveThingInCarrySlot(6, true) == &dead && dead.Carried && c.CarrySlots[6] == 0 && c.RemovedModes == 0;
    printf(ok ? "006a16a0_TEST PASS\n" : "006a16a0_TEST FAIL\n");
    return ok ? 0 : 1;
}