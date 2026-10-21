// Behaviour test for CQuestManager::ActivateTestQuestCard (0x004B4C50): creates the card at the living hero, retargets
// its quest card to the given quest and, when the quest activates, makes the card active and resets the recall point;
// every early exit returns false and the def reference is always released.
#include <cstdio>
#include <cstring>
struct CCharString {
    char Text[64];
    CCharString(const char* t) { std::strncpy(Text, t, 63); Text[63] = 0; }
};
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CQuestCardDef { long RefCount; int deleted; void DeleteThis() { ++deleted; } };
struct CTCQuestCard { CQuestCardDef* PDef; CCharString ScriptQuestName; CTCQuestCard() : PDef(0), ScriptQuestName("") {} };
struct CThing {
    bool Dead; bool hasTC; CTCQuestCard* tc; bool inLimbo; bool persisted; C3DVector pos;
    CThing() : Dead(false), hasTC(true), tc(0), inLimbo(false), persisted(false), pos(1, 2, 3) {}
    bool HasTC(long type) const { return type == 0x6c && hasTC; }
    CTCQuestCard* GetTC(long) { return tc; }
    const C3DVector& GetPosition() const { return pos; }
    const char* GetDefName() const { return "OBJECT_QUEST_CARD_TEST"; }
};
static CThing* g_created = 0;
static int g_createdDef = 0;
CThing* CreateThingObject(long defIndex, const C3DVector&, long player, void*, unsigned long, const CCharString&)
{ g_createdDef = (int)defIndex + (int)player * 1000; return g_created; }
struct CPlayer { long PlayerNumber; CThing* hero; long GetPlayerNumber() const { return PlayerNumber; } CThing* GetHeroCreature() { return hero; } };
struct CWorld { C3DVector GuildSealRecallPos; float GuildSealRecallAngleXY; CWorld() : GuildSealRecallPos(5, 5, 5), GuildSealRecallAngleXY(0.5f) {} };
static long g_defIndex = 7;
static int g_refreshes = 0;
struct CQuestManager {
    CPlayer* player; CWorld* World; bool activateResult; int pushed; char removed[64];
    bool ActivateQuest(const CCharString& name, bool a, bool load) { return a && load && std::strcmp(name.Text, "Q_Test") == 0 && activateResult; }
    void RemoveAvailableQuestCardType(const CCharString& n) { std::strcpy(removed, n.Text); }
    bool ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName);
};
bool CQuestManager::ActivateTestQuestCard(const CCharString& questName, const CCharString& cardDefName)
{
    CThing* hero = player->GetHeroCreature();
    if (hero && !hero->Dead) {
        long defIndex = std::strcmp(cardDefName.Text, "OBJECT_QUEST_CARD_TEST") == 0 ? g_defIndex : 0;
        if (defIndex > 0) {
            CThing* card = CreateThingObject(defIndex, hero->GetPosition(), player->GetPlayerNumber(), 0, 0, "");
            if (card && card->HasTC(0x6c)) {
                CTCQuestCard* tc = card->GetTC(0x6c);
                if (tc) {
                    tc->ScriptQuestName = questName;
                    CQuestCardDef* def = tc->PDef;
                    if (def) {
                        ++def->RefCount;
                        if (ActivateQuest(tc->ScriptQuestName, true, true)) {
                            ++pushed;
                            card->inLimbo = true;
                            card->persisted = true;
                            World->GuildSealRecallPos = C3DVector(0.0f, 0.0f, 0.0f);
                            World->GuildSealRecallAngleXY = 0.0f;
                            RemoveAvailableQuestCardType(card->GetDefName());
                            ++g_refreshes;
                            if (--def->RefCount == 0)
                                def->DeleteThis();
                            return true;
                        }
                        if (--def->RefCount == 0)
                            def->DeleteThis();
                    }
                }
            }
        }
    }
    return false;
}
int main()
{
    CThing hero; CThing card; CTCQuestCard tc; CQuestCardDef def = { 1, 0 };
    tc.PDef = &def; card.tc = &tc; g_created = &card;
    CPlayer player = { 2, &hero }; CWorld world;
    CQuestManager qm; qm.player = &player; qm.World = &world; qm.activateResult = true; qm.pushed = 0; qm.removed[0] = 0;
    bool ok = qm.ActivateTestQuestCard("Q_Test", "OBJECT_QUEST_CARD_TEST");
    ok = ok && g_createdDef == 2007 && std::strcmp(tc.ScriptQuestName.Text, "Q_Test") == 0 && qm.pushed == 1
         && card.inLimbo && card.persisted && world.GuildSealRecallPos.X == 0.0f && world.GuildSealRecallAngleXY == 0.0f
         && std::strcmp(qm.removed, "OBJECT_QUEST_CARD_TEST") == 0 && g_refreshes == 1 && def.RefCount == 1 && def.deleted == 0;
    qm.activateResult = false;                                       // quest fails: false, reference released
    ok = ok && !qm.ActivateTestQuestCard("Q_Test", "OBJECT_QUEST_CARD_TEST") && def.RefCount == 1 && qm.pushed == 1;
    ok = ok && !qm.ActivateTestQuestCard("Q_Test", "OTHER");         // unknown def
    hero.Dead = true;
    ok = ok && !qm.ActivateTestQuestCard("Q_Test", "OBJECT_QUEST_CARD_TEST");
    hero.Dead = false; card.hasTC = false;
    ok = ok && !qm.ActivateTestQuestCard("Q_Test", "OBJECT_QUEST_CARD_TEST") && g_refreshes == 1;
    if (!ok) { std::printf("004b4c50_TEST FAIL\n"); return 1; }
    std::printf("004b4c50_TEST PASS\n");
    return 0;
}