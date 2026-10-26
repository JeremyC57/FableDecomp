#include <cstdio>
// Behaviour test for NCreatureMode::CModeJackOfBladesScream::OnDeactivate (retail 0x008D78E0): a creature-type thing gets
// creature flags 1|4 set again, and the scream emitter is killed if it is still alive.
class CThing { public:
    unsigned long ThingTypeFlags;
    unsigned char Dead : 1;
    unsigned char CreatureFlags;
    int kills;
    void Kill(bool immediately) { kills += immediately ? 1 : 100; }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
struct CModeJackOfBladesScream {
    CThing* ParentThing;
    CBaseIntelligentPointer PEmitter;
    void OnDeactivate()
    {
        {
            CThing* thing = ParentThing;
            if (thing->ThingTypeFlags & 2)
                thing->CreatureFlags |= 5;
        }
        if (PEmitter.GetPItem() != 0 && !PEmitter.GetPItem()->Dead)
            PEmitter.GetPItem()->Kill(true);
    }
};

int main()
{
    CThing jack;
    jack.ThingTypeFlags = 2; jack.Dead = 0; jack.CreatureFlags = 0x10; jack.kills = 0;
    CThing emitter;
    emitter.ThingTypeFlags = 0; emitter.Dead = 0; emitter.CreatureFlags = 0; emitter.kills = 0;
    CModeJackOfBladesScream m;
    m.ParentThing = &jack; m.PEmitter.Item = &emitter;
    m.OnDeactivate();
    int pass = jack.CreatureFlags == 0x15 && emitter.kills == 1;
    emitter.Dead = 1;
    jack.ThingTypeFlags = 1; jack.CreatureFlags = 0;
    m.OnDeactivate();
    pass = pass && jack.CreatureFlags == 0 && emitter.kills == 1;
    m.PEmitter.Item = 0;
    m.OnDeactivate();
    pass = pass && emitter.kills == 1;
    if (!pass) { std::printf("008d78e0_TEST FAIL\n"); return 1; }
    std::printf("008d78e0_TEST PASS\n");
    return 0;
}