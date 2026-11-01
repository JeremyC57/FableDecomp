// Behaviour test for CTCSpecialAbilities::SetupSummonSpell 0x0072B330 (model copy of the source logic).
#include <cstdio>
#include <cstring>
struct Spell { int resets; };
struct Entry { int first; void* second; };
struct Thing {
    unsigned int bits[9];
    Entry entries[4]; int count;
    int added; char lastAdded[32];
    Entry* LowerBound(int key) { int i = 0; while (i < count && entries[i].first < key) ++i; return &entries[i]; }
    template<class T> bool GetInterface(T** out, int type)
    {
        if (bits[type >> 5] & (1u << (type & 31))) {
            Entry* it = LowerBound(type);
            Entry* last = &entries[count];
            if (it == last || type < it->first) it = last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    void AddTC(const char* name) { ++added; std::strcpy(lastAdded, name); }
};
struct Abilities {
    Thing* Creature;
    bool SetupSummonSpell()
    {
        Spell* spell;
        if (!Creature->GetInterface(&spell, 0x91)) { Creature->AddTC("CTCSummonSpell"); return true; }
        ++spell->resets;
        return true;
    }
};
int main()
{
    Thing t; std::memset(&t, 0, sizeof(t));
    Spell s = { 0 };
    Abilities a; a.Creature = &t;
    if (!a.SetupSummonSpell() || t.added != 1 || std::strcmp(t.lastAdded, "CTCSummonSpell") != 0 || s.resets != 0) { std::printf("0072b330_TEST FAIL\n"); return 1; }
    t.bits[0x91 >> 5] |= 1u << (0x91 & 31);
    t.entries[0].first = 0x10; t.entries[1].first = 0x91; t.entries[1].second = &s; t.entries[2].first = 0xA7; t.count = 3;
    if (!a.SetupSummonSpell() || t.added != 1 || s.resets != 1) { std::printf("0072b330_TEST FAIL\n"); return 1; }
    if (!a.SetupSummonSpell() || s.resets != 2) { std::printf("0072b330_TEST FAIL\n"); return 1; }
    std::printf("0072b330_TEST PASS\n");
    return 0;
}