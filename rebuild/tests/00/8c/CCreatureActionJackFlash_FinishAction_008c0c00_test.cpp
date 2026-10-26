// Behaviour test for 0x008C0C00 CCreatureAction_JackFlash::FinishAction: a flagged target gets a flash mode pushed on the
// creature (aimed at the target, reference-counted correctly), a player creature has its animation suspended.
#include <cstdio>
class CThing;
static int g_modes = 0;
struct CMode { CThing* owner; CThing* target; int refs; CMode(CThing* o) : owner(o), target(0), refs(0) { ++g_modes; } ~CMode() { --g_modes; } void SetTarget(CThing* t) { target = t; } };
struct CModePtr {
    CMode* object;
    CModePtr(CMode* m) : object(m) { if (object) ++object->refs; }
    CModePtr(const CModePtr& o) : object(o.object) { if (object) ++object->refs; }
    ~CModePtr() { if (object && --object->refs == 0) delete object; }
    CMode* operator->() const { return object; }
};
struct CModes { CModePtr* held; int count; void AddMode(CModePtr m) { delete held; held = new CModePtr(m); ++count; } };
class CThing { public:
    unsigned char type; unsigned char flags; CModes modes; bool suspended;
    void SetSuspendAnimation(bool s, long) { suspended = s; }
    bool GetModes(CModes** out) { *out = &modes; return true; }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CAction {
    CPtr PCreature; CPtr PTarget; int baseFinish;
    void FinishAction()
    {
        if (PTarget.GetPItem()->flags & 8) {
            CThing* target = PTarget.GetPItem();
            CModes* modes; PCreature.GetPItem()->GetModes(&modes);
            CModePtr mode(new CMode(PCreature.GetPItem()));
            mode->SetTarget(target);
            modes->AddMode(mode);
            if (PCreature.GetPItem()->type == 1) { CThing* creature = PCreature.GetPItem(); creature->SetSuspendAnimation(true, 0); }
        }
        ++baseFinish;
    }
};
int main()
{
    CThing jack; jack.type = 1; jack.suspended = false; jack.modes.held = 0; jack.modes.count = 0;
    CThing hero; hero.flags = 8;
    CAction a; a.PCreature.item = &jack; a.PTarget.item = &hero; a.baseFinish = 0;
    a.FinishAction();
    int ok = jack.modes.count == 1 && jack.modes.held->object->target == &hero && jack.modes.held->object->owner == &jack
          && jack.modes.held->object->refs == 1 && jack.suspended && a.baseFinish == 1 && g_modes == 1;
    hero.flags = 0; a.FinishAction(); ok = ok && jack.modes.count == 1 && a.baseFinish == 2;
    delete jack.modes.held; ok = ok && g_modes == 0;
    if (!ok) { std::printf("008c0c00_TEST FAIL\n"); return 1; }
    std::printf("008c0c00_TEST PASS\n");
    return 0;
}