// Behaviour test for 0x005B406D CTCInventoryClothing::SetThingAsWearingSelectedClothes
#include <cstdio>
static int g_removed = 0, g_added[8], g_nadd = 0;
struct CMods {
    void RemoveAllAppearanceModifiers(long t) { g_removed = (int)t; }
    void AddAppearanceModifierWithReplace(long d) { g_added[g_nadd++] = (int)d; }
};
struct SNode { SNode* next; long first; long second; };
static SNode* _M_increment(SNode* n) { return n->next; }
class CThing { public: CMods* mods; bool GetInterface(CMods** out, long) { if (!mods) return false; *out = mods; return true; } };
class CMap { public: SNode header; SNode* begin() { return header.next; } SNode* end() { return &header; } };
struct CClothing {
    CMap CurrentClothingSelected;
    void SetThingAsWearingSelectedClothes(CThing* thing)
    {
        CMods* modifiers;
        if (thing->GetInterface(&modifiers, 0x5E)) {
            modifiers->RemoveAllAppearanceModifiers(2);
            for (SNode* it = CurrentClothingSelected.begin(); it != CurrentClothingSelected.end(); it = _M_increment(it))
                if (it->second > 0)
                    modifiers->AddAppearanceModifierWithReplace(it->second);
        }
    }
};
int main()
{
    CClothing c; SNode a, b, d;
    c.CurrentClothingSelected.header.next = &a; a.next = &b; b.next = &d; d.next = &c.CurrentClothingSelected.header;
    a.second = 5; b.second = 0; d.second = 9;
    CMods m; CThing t; t.mods = &m;
    c.SetThingAsWearingSelectedClothes(&t);
    bool ok = g_removed == 2 && g_nadd == 2 && g_added[0] == 5 && g_added[1] == 9;
    CThing none; none.mods = 0; c.SetThingAsWearingSelectedClothes(&none);
    if (g_nadd != 2) ok = false;
    if (!ok) { std::printf("005b406d_TEST FAIL\n"); return 1; }
    std::printf("005b406d_TEST PASS\n");
    return 0;
}