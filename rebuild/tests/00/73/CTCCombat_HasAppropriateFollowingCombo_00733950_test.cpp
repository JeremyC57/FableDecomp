// behaviour test for 0x00733950 CTCCombat::HasAppropriateFollowingCombo
#include <cstdio>
struct Combo { long ComboStage; long ComboID; };
struct Entry { Combo* combo; void GetComponent(Combo** out) const { *out = combo; } };
struct SetEntry { unsigned int first; Entry second; };
struct GetMax { long MaxComboStage; long ComboID; GetMax(long id) : MaxComboStage(-777), ComboID(id) {} };
static GetMax for_each_(const SetEntry* f, const SetEntry* l, GetMax g)
{
    for (; f != l; ++f) { Combo* c; f->second.GetComponent(&c); if (c->ComboID == g.ComboID && c->ComboStage > g.MaxComboStage) g.MaxComboStage = c->ComboStage; }
    return g;
}
struct Pred {
    Combo* CombatAnimCombo; long CurrentChainPos; long MaxComboStage;
    Pred(Combo* c, long p, long m) : CombatAnimCombo(c), CurrentChainPos(p), MaxComboStage(m) {}
    bool operator()(const Entry& e) const
    {
        Combo* c; e.GetComponent(&c);
        if (c->ComboID == -999 || c->ComboID != CombatAnimCombo->ComboID) return false;
        if (c->ComboStage == CombatAnimCombo->ComboStage + 1) return true;
        if (CurrentChainPos <= MaxComboStage && c->ComboStage == -888) return true;
        return false;
    }
};
struct AnimSet {
    SetEntry* first; SetEntry* last;
    bool IsAppropriateEntryExisting(const Pred& p) const { for (SetEntry* it = first; it != last; ++it) if (p(it->second)) return true; return false; }
};
struct Combat {
    long ComboChainPos, ComboChainID; AnimSet* set;
    bool HasAppropriateFollowingCombo(const Entry* entry)
    {
        Combo* combo; entry->GetComponent(&combo);
        if (combo->ComboID != -999) {
            AnimSet* s = set; long maxStage;
            if (ComboChainID == -999) maxStage = 0;
            else { maxStage = for_each_(set->first, set->last, GetMax(ComboChainID)).MaxComboStage; if (maxStage == -777) maxStage = 0; }
            return s->IsAppropriateEntryExisting(Pred(combo, ComboChainPos, maxStage));
        }
        return false;
    }
};
int main()
{
    Combo c1 = { 1, 5 }, c2 = { 2, 5 }, fin = { -888, 5 }, none = { 0, -999 };
    SetEntry e[2]; e[0].second.combo = &c1; e[1].second.combo = &c2;
    AnimSet s; s.first = e; s.last = e + 2;
    Combat cb; cb.ComboChainPos = 1; cb.ComboChainID = 5; cb.set = &s;
    Entry q; q.combo = &c1;
    bool ok = cb.HasAppropriateFollowingCombo(&q);            // stage 2 follows stage 1
    q.combo = &c2;
    ok = ok && !cb.HasAppropriateFollowingCombo(&q);          // nothing follows stage 2
    e[1].second.combo = &fin;                                  // finisher allowed while pos <= max stage (1)
    q.combo = &c1;
    ok = ok && cb.HasAppropriateFollowingCombo(&q);
    q.combo = &none;
    ok = ok && !cb.HasAppropriateFollowingCombo(&q);
    if (!ok) { std::printf("00733950_TEST FAIL\n"); return 1; }
    std::printf("00733950_TEST PASS\n");
    return 0;
}