// Behaviour test for 008968c0 CGameScriptInterface::GiveHeroQuestCardDirectly: decision flow over a model hero / quest manager.
#include <cstdio>
#include <cstring>
struct Card { bool hasTC; bool vignette; char quest[32]; bool killed; };
struct World {
    bool heroExists, heroDead, alreadyActive, activateOk, created;
    long index; Card card; int messages;
};
static void Give(World& w, const char* def, const char* quest, bool silent)
{
    if (!w.heroExists || w.heroDead) return;
    if (w.alreadyActive) return;
    long index = w.index;
    if (index <= 0) return;
    Card* card = w.created ? &w.card : 0;
    if (!card) return;
    if (card->hasTC) {
        std::strcpy(card->quest, quest);
        if (w.activateOk) {
            if (!silent && !card->vignette) ++w.messages;
            return;
        }
    }
    card->killed = true;
}
static World Base() { World w; std::memset(&w, 0, sizeof(w)); w.heroExists = true; w.activateOk = true; w.created = true; w.index = 5; w.card.hasTC = true; return w; }
int main()
{
    World a = Base(); Give(a, "D", "Q", false);
    World b = Base(); Give(b, "D", "Q", true);
    World c = Base(); c.activateOk = false; Give(c, "D", "Q", false);
    World d = Base(); d.card.hasTC = false; Give(d, "D", "Q", false);
    World e = Base(); e.index = 0; Give(e, "D", "Q", false);
    World f = Base(); f.card.vignette = true; Give(f, "D", "Q", false);
    if (a.messages != 1 || std::strcmp(a.card.quest, "Q") || a.card.killed || b.messages != 0 || !c.card.killed || c.messages
        || !d.card.killed || e.card.killed || f.messages) { std::printf("008968c0_TEST FAIL\n"); return 1; }
    std::printf("008968c0_TEST PASS\n");
    return 0;
}
