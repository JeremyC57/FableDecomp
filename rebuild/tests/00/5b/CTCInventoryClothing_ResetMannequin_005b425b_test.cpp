// behaviour test for 005b425b: CTCInventoryClothing::ResetMannequin -- with a mannequin and both appearance-modifier
// components present, the mannequin's type-2 modifiers are cleared and replaced by the hero's, in the hero's order;
// nothing happens without a mannequin or when either thing lacks the component
#include <cstdio>
struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct CLongList {
    SLongListNode* _M_node; SLongListNode pool[8]; SLongListNode sentinel; int used;
    CLongList() : _M_node(&sentinel), used(0) { sentinel._M_next = sentinel._M_prev = &sentinel; }
    void push_back(long v) { SLongListNode* n = &pool[used++]; n->_M_data = v; n->_M_prev = sentinel._M_prev; n->_M_next = &sentinel; sentinel._M_prev->_M_next = n; sentinel._M_prev = n; }
};
struct CMods {
    long worn[8]; int count; int clears;
    void GetAppearanceModifierList(CLongList& out, long type) const { if (type == 2) for (int i = 0; i < count; ++i) out.push_back(worn[i]); }
    void RemoveAllAppearanceModifiers(long type) { if (type == 2) { count = 0; ++clears; } }
    void AddAppearanceModifierWithReplace(long def) { worn[count++] = def; }
};
class CThing { public:
    CMods* mods;      /* 0 = the thing does not use interface 0x5E */
    bool GetInterface(CMods** out, long wanted) { if (wanted != 0x5e || !mods) return false; *out = mods; return true; }
};
class CTCInventoryClothing { public:
    CThing* ParentThing; CThing* PMannequin;
    void ResetMannequin()
    {
        if (!PMannequin) return;
        CMods* heroModifiers;
        if (!ParentThing->GetInterface(&heroModifiers, 0x5e)) return;
        CMods* mannequinModifiers;
        if (!PMannequin->GetInterface(&mannequinModifiers, 0x5e)) return;
        CLongList worn;
        heroModifiers->GetAppearanceModifierList(worn, 2);
        mannequinModifiers->RemoveAllAppearanceModifiers(2);
        for (SLongListNode* node = worn._M_node->_M_next; node != worn._M_node; node = node->_M_next)
            mannequinModifiers->AddAppearanceModifierWithReplace(node->_M_data);
    }
};
int main()
{
    int bad = 0;
    CMods hero = { { 11, 12, 13 }, 3, 0 }, dummy = { { 99 }, 1, 0 };
    CThing heroThing = { &hero }, mannequin = { &dummy }, bare = { 0 };
    CTCInventoryClothing c = { &heroThing, 0 };
    c.ResetMannequin();                                   /* no mannequin */
    if (dummy.clears != 0 || dummy.count != 1) ++bad;
    c.PMannequin = &bare; c.ResetMannequin();             /* mannequin without the component */
    if (dummy.clears != 0) ++bad;
    c.PMannequin = &mannequin; c.ResetMannequin();
    if (dummy.clears != 1 || dummy.count != 3 || dummy.worn[0] != 11 || dummy.worn[1] != 12 || dummy.worn[2] != 13) ++bad;
    if (hero.count != 3 || hero.clears != 0) ++bad;
    heroThing.mods = 0; dummy.worn[0] = 5; c.ResetMannequin();   /* hero without the component */
    if (dummy.clears != 1 || dummy.worn[0] != 5) ++bad;
    if (bad) { std::printf("005b425b_TEST FAIL\n"); return 1; }
    std::printf("005b425b_TEST PASS\n");
    return 0;
}