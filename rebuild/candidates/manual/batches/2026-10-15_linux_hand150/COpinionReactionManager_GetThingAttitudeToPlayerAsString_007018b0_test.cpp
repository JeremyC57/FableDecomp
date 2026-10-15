// behaviour test for 0x00701840 COpinionReactionManager::GetThingAttitudeToPlayer and 0x007018B0 ...AsString
#include <cstdio>
struct CTCOpinionSource { int id; };
struct CThing { bool has; CTCOpinionSource* tc;
    unsigned int HasTC(long) const { return has; } CTCOpinionSource* GetTC(long) { return tc; } };
struct CPlayer { CThing* creature; CThing* GetControlledCreature() { return creature; } };
struct CWorld { CPlayer* player; CPlayer* GetCurrentPlayer() { return player; } };
struct CCharString { const char* text; };
static CCharString g_OpinionAttitudeNames[4] = { { "NONE" }, { "LIKE" }, { "HATE" }, { "FEAR" } };
struct COpinionReactionManager {
    CWorld* World; bool AttitudeMask[79];
    const CTCOpinionSource* lastSource; const bool* lastMask;
    long GetAttitudeOrReactionType(const CThing*, bool attitudeOnly, bool* mask, CTCOpinionSource* source)
    { lastSource = source; lastMask = mask; return attitudeOnly ? (source ? source->id : 0) : -1; }
    long GetThingAttitudeToPlayer(const CThing* thing)
    {
        CThing* creature = World->GetCurrentPlayer()->GetControlledCreature();
        CTCOpinionSource* source = 0;
        if (creature && creature->HasTC(0x50)) source = creature->GetTC(0x50);
        return GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
    }
    const CCharString& GetThingAttitudeToPlayerAsString(const CThing* thing)
    {
        CThing* creature = World->GetCurrentPlayer()->GetControlledCreature();
        if (creature && creature->HasTC(0x50)) {
            CTCOpinionSource* source = creature->GetTC(0x50);
            if (source) return g_OpinionAttitudeNames[GetAttitudeOrReactionType(thing, true, AttitudeMask, source)];
        }
        return g_OpinionAttitudeNames[GetThingAttitudeToPlayer(thing)];
    }
};
int main()
{
    CTCOpinionSource src = { 2 };
    CThing hero = { true, &src }, bare = { false, 0 }, target = { false, 0 };
    CPlayer p = { &hero }; CWorld w = { &p };
    COpinionReactionManager m; m.World = &w;
    int bad = m.GetThingAttitudeToPlayer(&target) != 2 || m.lastSource != &src || m.lastMask != m.AttitudeMask;
    bad |= m.GetThingAttitudeToPlayerAsString(&target).text[0] != 'H';
    p.creature = &bare;
    bad |= m.GetThingAttitudeToPlayer(&target) != 0 || m.lastSource != 0;
    bad |= m.GetThingAttitudeToPlayerAsString(&target).text[0] != 'N';
    p.creature = 0;
    bad |= m.GetThingAttitudeToPlayer(&target) != 0 || m.lastSource != 0;
    if (bad) { std::printf("007018b0_TEST FAIL\n"); return 1; }
    std::printf("007018b0_TEST PASS\n");
    return 0;
}
