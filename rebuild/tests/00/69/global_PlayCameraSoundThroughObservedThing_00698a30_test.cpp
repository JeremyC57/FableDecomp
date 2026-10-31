// behaviour test for 0x00698A30 (anonymous) PlayCameraSoundThroughObservedThing(const CThing*, const CCameraManagerDef&,
// const CCharString&): with a thing, a camera def that carries an entity-sound sub-def and a thing that has the sound
// player component (0x10), a named sound pair plays as an attached sample; otherwise the name is looked up as a criteria
// string and played as attached criteria; no thing, no sub-def, no component or no entry plays nothing.
#include <cstdio>
#include <cstring>
struct SoundPair { long first; long last; };
struct SoundMap {
    const char* pairName; SoundPair pair;
    const char* criteriaName; long criteria;
    const SoundPair* GetPSoundPair(const char* n) const { return pairName && std::strcmp(pairName, n) == 0 ? &pair : 0; }
    const long* GetPCriteriaString(const char* n) const { return criteriaName && std::strcmp(criteriaName, n) == 0 ? &criteria : 0; }
};
struct Player { int samples; long lastFirst; int criteria; long lastCriteria; };
struct Thing { bool hasPlayer; Player player; };
struct CameraDef { const SoundMap* soundDef; };
static void PlayCameraSoundThroughObservedThing(Thing* thing, const CameraDef& def, const char* sound)
{
    const SoundMap* soundDef = 0;
    if (thing && (soundDef = def.soundDef) != 0 && thing->hasPlayer) {
        Player& player = thing->player;
        const SoundPair* pair = soundDef->GetPSoundPair(sound);
        if (pair) { ++player.samples; player.lastFirst = pair->first; }
        else {
            const long* criteria = soundDef->GetPCriteriaString(sound);
            if (criteria) { ++player.criteria; player.lastCriteria = *criteria; }
        }
    }
}
int main()
{
    SoundMap map = { "swoosh", { 7, 9 }, "ambient", 42 };
    CameraDef def = { &map }, noDef = { 0 };
    Thing thing = { true, { 0, 0, 0, 0 } }, mute = { false, { 0, 0, 0, 0 } };
    PlayCameraSoundThroughObservedThing(&thing, def, "swoosh");
    if (thing.player.samples != 1 || thing.player.lastFirst != 7 || thing.player.criteria != 0) { std::printf("00698a30_TEST FAIL\n"); return 1; }
    PlayCameraSoundThroughObservedThing(&thing, def, "ambient");
    if (thing.player.samples != 1 || thing.player.criteria != 1 || thing.player.lastCriteria != 42) { std::printf("00698a30_TEST FAIL\n"); return 1; }
    PlayCameraSoundThroughObservedThing(&thing, def, "unknown");
    PlayCameraSoundThroughObservedThing(&thing, noDef, "swoosh");
    PlayCameraSoundThroughObservedThing(&mute, def, "swoosh");
    PlayCameraSoundThroughObservedThing(0, def, "swoosh");
    if (thing.player.samples != 1 || thing.player.criteria != 1 || mute.player.samples != 0) { std::printf("00698a30_TEST FAIL\n"); return 1; }
    std::printf("00698a30_TEST PASS\n");
    return 0;
}