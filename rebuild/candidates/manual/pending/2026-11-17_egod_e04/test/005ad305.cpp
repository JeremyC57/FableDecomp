// Behaviour test for CTCInventoryTrade::ApplyTattooCard (0x005AD305): the hero's current tattoo things are filtered to the ones whose
// tattoo sub-def is permanent; the card's tattoo is added to the hero morph (and the morph updated) when the hero has one; then, when
// the appearance morph exists, a card that removes all tattoos clears the texture morphs (null set), re-adds every permanent tattoo
// (only when the hero morph exists) and, if there were any, uploads the set of their texture layers (things without a tattoo sub-def
// are re-added but contribute no layer); any other card uploads just its own layer.
#include <cstdio>
#include <set>
#include <vector>
struct TattooDef { bool permanent; long layer; bool removes_all; };
struct Thing { const TattooDef* sub; const TattooDef* sub_again; };   // a tattoo thing (sub-def per lookup; 0: none)
struct Upload { bool null_set; std::set<long> layers; };
struct Morph {
    std::vector<const Thing*> worn; std::vector<long> added_cards; std::vector<const Thing*> added_things; int frame_updates;
    std::vector<Upload> uploads;
};
struct Hero { bool has_morph; Morph morph; };
static void ApplyTattooCard(Hero& hero, const TattooDef& card, long card_index)
{
    std::vector<const Thing*> tattoos, tattoos_backup;
    if (hero.has_morph) tattoos = hero.morph.worn;
    for (unsigned long count = 0; count < tattoos.size(); count++) {
        const TattooDef* d = tattoos[count]->sub;
        if (d && d->permanent) tattoos_backup.push_back(tattoos[count]);
    }
    Morph* ptc_hero_morph = 0;
    if (hero.has_morph) { ptc_hero_morph = &hero.morph; ptc_hero_morph->added_cards.push_back(card_index); ++ptc_hero_morph->frame_updates; }
    if (hero.has_morph) {
        Morph* ptc_morph = &hero.morph;
        std::set<long> Textures;
        if (card.removes_all) {
            Upload u0 = { true, std::set<long>() }; ptc_morph->uploads.push_back(u0);
            for (unsigned long count = 0; count < tattoos_backup.size(); count++) {
                if (ptc_hero_morph) { ptc_hero_morph->added_things.push_back(tattoos_backup[count]); ++ptc_hero_morph->frame_updates; }
                if (tattoos_backup[count]->sub_again) Textures.insert(tattoos_backup[count]->sub_again->layer);
            }
            if (!tattoos_backup.empty()) { Upload u = { false, Textures }; ptc_morph->uploads.push_back(u); }
        } else {
            Textures.insert(card.layer);
            Upload u = { false, Textures }; ptc_morph->uploads.push_back(u);
        }
    }
}
static int fail(int n) { std::printf("005ad305_TEST FAIL %d\n", n); return 1; }
int main()
{
    TattooDef perm7 = { true, 7, false }, perm9 = { true, 9, false }, temp5 = { false, 5, false };
    Thing a = { &perm7, &perm7 }, b = { &temp5, &temp5 }, c = { &perm9, &perm9 }, d = { 0, 0 }, f = { &perm9, 0 };
    TattooDef plain = { false, 12, false }, remover = { false, 0, true };
    Hero h; h.has_morph = true; h.morph.frame_updates = 0;
    h.morph.worn.push_back(&a); h.morph.worn.push_back(&b); h.morph.worn.push_back(&c); h.morph.worn.push_back(&d);
    ApplyTattooCard(h, plain, 42);                       // ordinary card: one upload of its own layer
    if (h.morph.added_cards.size() != 1 || h.morph.added_cards[0] != 42 || h.morph.frame_updates != 1) return fail(1);
    if (h.morph.uploads.size() != 1 || h.morph.uploads[0].null_set || h.morph.uploads[0].layers.size() != 1 || !h.morph.uploads[0].layers.count(12)) return fail(2);
    if (!h.morph.added_things.empty()) return fail(3);
    Hero r = h; r.morph.uploads.clear(); r.morph.added_cards.clear(); r.morph.frame_updates = 0;
    ApplyTattooCard(r, remover, 43);                     // remover: clear, re-add the two permanent ones (7, 9), upload {7, 9}
    if (r.morph.uploads.size() != 2 || !r.morph.uploads[0].null_set) return fail(4);
    if (r.morph.added_things.size() != 2 || r.morph.added_things[0] != &a || r.morph.added_things[1] != &c || r.morph.frame_updates != 3) return fail(5);
    if (r.morph.uploads[1].layers.size() != 2 || !r.morph.uploads[1].layers.count(7) || !r.morph.uploads[1].layers.count(9)) return fail(6);
    Hero e; e.has_morph = true; e.morph.frame_updates = 0; e.morph.worn.push_back(&b);
    ApplyTattooCard(e, remover, 44);                     // remover, nothing permanent: only the clearing upload
    if (e.morph.uploads.size() != 1 || !e.morph.uploads[0].null_set || !e.morph.added_things.empty()) return fail(7);
    Hero g; g.has_morph = true; g.morph.frame_updates = 0; g.morph.worn.push_back(&f); g.morph.worn.push_back(&a);
    ApplyTattooCard(g, remover, 46);                     // a permanent thing whose sub-def lookup fails the second time: re-added, no layer
    if (g.morph.added_things.size() != 2 || g.morph.uploads.size() != 2 || g.morph.uploads[1].layers.size() != 1 || !g.morph.uploads[1].layers.count(7)) return fail(9);
    Hero n; n.has_morph = false; n.morph.frame_updates = 0; n.morph.worn.push_back(&a);
    ApplyTattooCard(n, plain, 45);                       // no morph: nothing happens
    if (!n.morph.added_cards.empty() || !n.morph.uploads.empty() || n.morph.frame_updates) return fail(8);
    std::printf("005ad305_TEST PASS\n");
    return 0;
}
