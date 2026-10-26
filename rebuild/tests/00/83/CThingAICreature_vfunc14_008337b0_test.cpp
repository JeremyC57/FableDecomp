// Behaviour test for CThingAICreature::vfunc_14 @ 0x008337B0: resuming -> summon-spell component resumed (if present); else sounds restarted.
#include <cstdio>
class CTCBase { public: int dummy; };
class CTCSummonSpell : public CTCBase { public: int resumed; CTCSummonSpell() : resumed(0) { } void ResumeForCutscene_0066aeb0() { ++resumed; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThingAICreature { public:
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    int sounds;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void PlayCreatureContinuousSounds() { ++sounds; }
    void vfunc_14(bool resuming)
    {
        if (resuming) {
            CTCSummonSpell* summon;
            if (GetInterface(&summon, 0x10))
                summon->ResumeForCutscene_0066aeb0();
        } else {
            PlayCreatureContinuousSounds();
        }
    }
};
int main()
{
    CTCSummonSpell spell;
    CInterfaceMap::SEntry entries[2] = { { 0x10, &spell }, { 0x20, 0 } };
    CThingAICreature c; for (int i = 0; i < 9; ++i) c.UsingComponents[i] = 0;
    c.Components.first = entries; c.Components.last = entries + 2; c.sounds = 0;
    c.vfunc_14(true);
    if (spell.resumed != 0 || c.sounds != 0) { std::printf("008337b0_TEST FAIL\n"); return 1; }
    c.UsingComponents[0] = 0x10000;
    c.vfunc_14(true);
    if (spell.resumed != 1 || c.sounds != 0) { std::printf("008337b0_TEST FAIL\n"); return 1; }
    c.vfunc_14(false);
    if (spell.resumed != 1 || c.sounds != 1) { std::printf("008337b0_TEST FAIL\n"); return 1; }
    std::printf("008337b0_TEST PASS\n");
    return 0;
}