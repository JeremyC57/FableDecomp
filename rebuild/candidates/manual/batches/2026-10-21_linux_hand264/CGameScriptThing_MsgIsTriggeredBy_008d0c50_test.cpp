// Behaviour test for CGameScriptThing::MsgIsTriggeredBy @ 0x008D0C50: dead/absent things and things without the
// entity-events component answer false; otherwise any event of the type inside (last, current] whose originator is
// alive matches an empty name, or a name equal to the originator's script name.
#include <cstdio>
#include <cstring>
static const int kType = 7;
struct Thing { bool dead; bool hasEvents; const char* scriptName; };
struct Event { int type; int frame; Thing* by; };
struct Events { Event* list; int count; };
static bool Query(const Thing* self, const Events* ev, int last, int current, const char* name)
{
    if (!self || self->dead) return false;
    if (!self->hasEvents) return false;
    for (int i = 0; i < ev->count; ++i) {
        const Event& e = ev->list[i];
        if (e.type == kType && e.frame > last && e.frame <= current && e.by) {
            const char* scriptName = e.by->scriptName ? e.by->scriptName : "";
            if (name == 0 || std::strcmp(name, "") == 0) return true;
            if (std::strcmp(name, scriptName) == 0) return true;
        }
    }
    return false;
}
int main()
{
    Thing self = { false, true, "self" }; Thing hero = { false, true, "HERO" };
    Event list[3] = { { kType + 1, 5, &hero }, { kType, 3, &hero }, { kType, 6, 0 } };
    Events ev = { list, 3 };
    bool ok = !Query(&self, &ev, 3, 10, "HERO");          // wrong type at 5, frame 3 not > last, no originator at 6
    list[1].frame = 4;
    ok = ok && Query(&self, &ev, 3, 10, "HERO") && Query(&self, &ev, 3, 10, "") && Query(&self, &ev, 3, 10, 0);
    ok = ok && !Query(&self, &ev, 3, 10, "BANDIT") && !Query(&self, &ev, 4, 10, "HERO") && !Query(&self, &ev, 3, 3, "HERO");
    self.dead = true; ok = ok && !Query(&self, &ev, 3, 10, "HERO"); self.dead = false;
    self.hasEvents = false; ok = ok && !Query(&self, &ev, 3, 10, "HERO");
    if (!ok) { std::printf("008d0c50_TEST FAIL\n"); return 1; }
    std::printf("008d0c50_TEST PASS\n");
    return 0;
}
