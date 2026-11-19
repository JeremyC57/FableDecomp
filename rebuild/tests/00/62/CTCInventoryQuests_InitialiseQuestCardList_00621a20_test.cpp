// Behaviour test for CTCInventoryQuests::InitialiseQuestCardList (0x00621A20), a portable model. The category list is cleared first.
// On the inventory screen the cards are the quest manager's active cards, in list order; otherwise, when the screen is enabled and
// has a building, the building's hero-centre cards; otherwise none. The card count is stored. Each card with a quest-card component
// adds one category (its def's inventory category, associated with the card); cards without one add nothing. With at least one card,
// category 0 is selected.
#include <stdio.h>
#include <vector>
#include <list>
struct Card { bool has_quest_card; long inventory_category; };
struct Category { long def_index; const Card* association; bool selected; };
struct Screen {
    bool inventory_screen, enabled; const std::vector<const Card*>* building_cards;
    std::vector<Category> categories; long card_count;
    void Initialise(const std::list<const Card*>& active)
    {
        categories.clear();
        std::vector<const Card*> cards;
        if (inventory_screen) {
            for (std::list<const Card*>::const_iterator it = active.begin(); it != active.end(); ++it) cards.push_back(*it);
        } else if (enabled) {
            if (building_cards) cards = *building_cards;
        }
        card_count = (long)cards.size();
        for (long i = 0; i < card_count; ++i)
            if (cards[i]->has_quest_card) { Category c = { cards[i]->inventory_category, cards[i], false }; categories.push_back(c); }
        if (card_count > 0) categories[0].selected = true;
    }
};
int main()
{
    int bad = 0;
    Card a = { true, 7 }, b = { false, 0 }, c = { true, 3 };
    std::list<const Card*> active; active.push_back(&a); active.push_back(&b); active.push_back(&c);
    std::vector<const Card*> building; building.push_back(&c);
    Category stale = { 99, 0, true };
    Screen s; s.inventory_screen = true; s.enabled = true; s.building_cards = &building; s.categories.push_back(stale); s.card_count = 0;
    s.Initialise(active);
    if (s.card_count != 3 || s.categories.size() != 2 || s.categories[0].def_index != 7 || s.categories[0].association != &a
        || !s.categories[0].selected || s.categories[1].def_index != 3 || s.categories[1].selected) ++bad;
    s.inventory_screen = false;                                  // the building's cards
    s.Initialise(active);
    if (s.card_count != 1 || s.categories.size() != 1 || s.categories[0].association != &c || !s.categories[0].selected) ++bad;
    s.building_cards = 0;                                        // no building: nothing
    s.Initialise(active);
    if (s.card_count != 0 || !s.categories.empty()) ++bad;
    s.building_cards = &building; s.enabled = false;             // disabled: nothing
    s.Initialise(active);
    if (s.card_count != 0 || !s.categories.empty()) ++bad;
    printf(bad ? "00621a20_TEST FAIL\n" : "00621a20_TEST PASS\n");
    return bad ? 1 : 0;
}