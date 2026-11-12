// Behaviour test for CTCBulletTime::PlayEffect (0x0079CC80): models the def lookup, the effect creation, the alive check, the
// emitter lookup and the attach, on both arms (raw / intelligent-pointer result), with the same control flow as the row.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

struct Thing { bool dead; bool hasEmitter; int attachedTo; long attachType; long effectIndex; };
static Thing g_things[8];
static int g_thingCount = 0;
static bool g_hasSubDef = true;
static std::map<std::string, long> g_effects;
static int g_subDefCalls = 0;

static bool GetSubDef() { ++g_subDefCalls; return g_hasSubDef; }
static bool GetEffectFromString(const std::string& name, long& index)
{
    std::map<std::string, long>::const_iterator it = g_effects.find(name);
    if (it == g_effects.end()) return false;
    index = it->second;
    return true;
}
static Thing* CreateParticleEffect(long index, bool makeDead)
{
    Thing* t = &g_things[g_thingCount++];
    t->dead = makeDead; t->hasEmitter = true; t->attachedTo = -1; t->attachType = 0; t->effectIndex = index;
    return t;
}
static bool IS_THING_ALIVE(Thing* t) { if (t != 0) { if (!t->dead) return true; } return false; }

static void PlayEffect(Thing** effect, const std::string& name, int target, long attachType, bool makeDead)
{
    long effect_index = 0;
    if (effect == 0) {
        if (GetSubDef() && GetEffectFromString(name, effect_index)) {
            Thing* thing = CreateParticleEffect(effect_index, makeDead);
            if (IS_THING_ALIVE(thing)) {
                if (thing->hasEmitter) { thing->attachedTo = target; thing->attachType = attachType; }
            }
        }
    } else {
        if (GetSubDef() && GetEffectFromString(name, effect_index)) {
            *effect = CreateParticleEffect(effect_index, makeDead);
            if (IS_THING_ALIVE(*effect)) {
                if ((*effect)->hasEmitter) { (*effect)->attachedTo = target; (*effect)->attachType = attachType; }
            }
        }
    }
}

int main()
{
    g_effects["SPARK"] = 7;
    int fail = 0;
    PlayEffect(0, "SPARK", 3, 8, false);
    if (g_thingCount != 1 || g_things[0].attachedTo != 3 || g_things[0].attachType != 8 || g_things[0].effectIndex != 7) fail = 1;
    Thing* kept = 0;
    PlayEffect(&kept, "SPARK", 4, 2, false);
    if (kept != &g_things[1] || kept->attachedTo != 4 || kept->attachType != 2) fail = 2;
    Thing* dead = 0;
    PlayEffect(&dead, "SPARK", 5, 2, true);
    if (dead != &g_things[2] || dead->attachedTo != -1) fail = 3;
    Thing* none = 0;
    PlayEffect(&none, "MISSING", 5, 2, false);
    if (none != 0 || g_thingCount != 3) fail = 4;
    g_hasSubDef = false;
    PlayEffect(0, "SPARK", 5, 2, false);
    if (g_thingCount != 3 || g_subDefCalls != 5) fail = 5;
    if (fail) { std::printf("0079cc80_TEST FAIL\n"); return 1; }
    std::printf("0079cc80_TEST PASS\n");
    return 0;
}