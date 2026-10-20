// Behaviour test for CQuestManager::ResetPositionOfAllQuestCards (0x004AFCA0): with a living hero every living,
// in-limbo card thing that has a physics component (on both lists) is reset to the hero's position; a dead or
// missing hero does nothing.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    int Resets; const C3DVector* Last;
    CTCPhysicsBase() : Resets(0), Last(0) {}
    virtual void ResetPosition(const C3DVector& position) { ++Resets; Last = &position; }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC;
    unsigned char   Dead : 1;
    unsigned char   CreatedByOwnerThingCreation : 1;
    unsigned char   IncludeInThingScans : 1;
    unsigned char   Activated : 1;
    unsigned char   GloballyLoaded : 1;
    unsigned char   InLimbo : 1;
    unsigned char   KillLocked : 1;
    unsigned char   CreatedBySerialisation : 1;
    C3DVector       Position;
    CThing(CTCPhysicsBase* p, int dead, int limbo) : PhysicsTC(p), Dead(dead), CreatedByOwnerThingCreation(0),
        IncludeInThingScans(0), Activated(0), GloballyLoaded(0), InLimbo(limbo), KillLocked(0), CreatedBySerialisation(0) {}
    const C3DVector& GetPosition() const { return Position; }
};
static CThing* g_hero;
struct CPlayer { CThing* GetHeroThing() const { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() { return &g_player; } };
struct CMainGameComponent { CPlayerManager* PlayerManager; };
static CPlayerManager g_pm;
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CBaseIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CCardNode { CCardNode* _M_next; CCardNode* _M_prev; CBaseIntelligentPointer _M_data; };
struct CQuestManager {
    CCardNode* ActiveQuestCards;
    CCardNode* CompletedQuestCards;
    void ResetPositionOfAllQuestCards();
};
void CQuestManager::ResetPositionOfAllQuestCards()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero == 0 || hero->Dead)
        return;
    CCardNode* node;
    for (node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead && card->InLimbo) {
            CTCPhysicsBase* physics = card->PhysicsTC;
            if (physics)
                physics->ResetPosition(hero->GetPosition());
        }
    }
    for (node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead && card->InLimbo) {
            CTCPhysicsBase* physics = card->PhysicsTC;
            if (physics)
                physics->ResetPosition(hero->GetPosition());
        }
    }
}
static void Ring(CCardNode* head, CCardNode* n, CThing** t, int count)
{
    CCardNode* prev = head;
    for (int i = 0; i < count; ++i) { n[i]._M_data.Item = t[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = head; head->_M_prev = prev;
}
int main()
{
    CTCPhysicsBase pa, pb, pc, pd, pe;
    CThing a(&pa, 0, 1), b(&pb, 0, 0), c(&pc, 1, 1), d(0, 0, 1), e(&pe, 0, 1);
    CThing* act[5] = { &a, &b, 0, &c, &d };
    CThing* done[1] = { &e };
    CCardNode ah, an[5], ch, cn[1];
    Ring(&ah, an, act, 5); Ring(&ch, cn, done, 1);
    CQuestManager qm; qm.ActiveQuestCards = &ah; qm.CompletedQuestCards = &ch;
    CThing hero(0, 0, 0); hero.Position.X = 1.0f;
    int bad = 0;
    g_hero = 0; qm.ResetPositionOfAllQuestCards();
    if (pa.Resets || pe.Resets) bad |= 1;
    hero.Dead = 1; g_hero = &hero; qm.ResetPositionOfAllQuestCards();
    if (pa.Resets || pe.Resets) bad |= 2;
    hero.Dead = 0; qm.ResetPositionOfAllQuestCards();
    if (pa.Resets != 1 || pe.Resets != 1 || pb.Resets || pc.Resets || pa.Last != &hero.Position || pe.Last != &hero.Position) bad |= 4;
    if (bad) { std::printf("004afca0_TEST FAIL\n"); return 1; }
    std::printf("004afca0_TEST PASS\n");
    return 0;
}