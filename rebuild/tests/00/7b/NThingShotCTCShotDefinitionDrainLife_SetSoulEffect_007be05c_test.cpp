// Behaviour test for NThingShot::CTCShotDefinitionDrainLife::SetSoulEffect (0x007BE05C): a model of the row -- a zero effect type
// does nothing; otherwise the effect thing is created at the shot position, and only a live one is kept in PSoulEffect and has its
// emitter attached to the shot's "weapon_pos_a" helper.
#include <stdio.h>
#include <string.h>
struct Thing { bool dead; float pos; const char* attachedHelper; long attachType; };
static Thing g_effects[4]; static int g_made = 0; static bool g_nextDead = false;
static Thing* Create(long, float pos) { Thing* t = &g_effects[g_made++]; t->dead = g_nextDead; t->pos = pos; t->attachedHelper = 0; return t; }
static bool IsAlive(Thing* const& t) { return t && !t->dead; }
struct Shot { Thing* parent; Thing* PSoulEffect;
    void SetSoulEffect(long type) {
        if (type) {
            Thing* effect = Create(type, parent->pos);
            if (IsAlive(effect)) { PSoulEffect = effect; effect->attachedHelper = "weapon_pos_a"; effect->attachType = 0x3c; }
        }
    }
};
int main()
{
    Thing parent = { false, 5.0f, 0, 0 }; Shot s = { &parent, 0 };
    s.SetSoulEffect(0);
    int ok = g_made == 0 && s.PSoulEffect == 0;
    g_nextDead = true; s.SetSoulEffect(3);
    ok = ok && g_made == 1 && s.PSoulEffect == 0;
    g_nextDead = false; s.SetSoulEffect(3);
    ok = ok && s.PSoulEffect == &g_effects[1] && s.PSoulEffect->pos == 5.0f && strcmp(s.PSoulEffect->attachedHelper, "weapon_pos_a") == 0 && s.PSoulEffect->attachType == 0x3c;
    printf(ok ? "007be05c_TEST PASS\n" : "007be05c_TEST FAIL\n");
    return ok ? 0 : 1;
}