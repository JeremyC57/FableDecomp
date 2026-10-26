// Behaviour test for 0x007A44C0 CTCWillEnflame::CancelMove: clear the owner's enflame ability lock, then remove TC 0x95.
#include <cstdio>

class CTCBase { public:};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
class CThing { public:
    unsigned int UsingComponents[9];
    CInterfaceMap Components;
    long removed;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (UsingComponents[type >> 5] & (1U << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    void RemoveTC(long t) { removed = t; }
};
class CTCSpecialAbilities : public CTCBase { public: bool AbilityMutexLock; long CurrentAbility; };
class CTCWillEnflame { public:
    CThing* ParentThing;
    void CancelMove()
    {
        CThing* parent = ParentThing;
        CTCSpecialAbilities* abilities;
        if (parent->GetInterface(0xA7, &abilities)) {
            if (abilities->CurrentAbility != 3)
                return;
            abilities->AbilityMutexLock = false;
            abilities->CurrentAbility = 0;
        }
        parent->RemoveTC(0x95);
    }
};

int main()
{
    CTCSpecialAbilities sa; sa.AbilityMutexLock = true; sa.CurrentAbility = 3;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0xA7; entries[0].component = &sa; entries[1].type = 0; entries[1].component = 0;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.UsingComponents[i] = 0;
    thing.Components.first = entries; thing.Components.last = entries + 1; thing.removed = 0;
    CTCWillEnflame tc; tc.ParentThing = &thing;
    // no 0xA7 component: just removed
    tc.CancelMove();
    if (thing.removed != 0x95 || !sa.AbilityMutexLock || sa.CurrentAbility != 3) { std::printf("007a44c0_TEST FAIL\n"); return 1; }
    // component running enflame: cleared and removed
    thing.removed = 0; thing.UsingComponents[5] = 0x80;
    tc.CancelMove();
    if (thing.removed != 0x95 || sa.AbilityMutexLock || sa.CurrentAbility != 0) { std::printf("007a44c0_TEST FAIL\n"); return 1; }
    // component running another ability: early return, nothing removed
    thing.removed = 0; sa.AbilityMutexLock = true; sa.CurrentAbility = 5;
    tc.CancelMove();
    if (thing.removed != 0 || !sa.AbilityMutexLock || sa.CurrentAbility != 5) { std::printf("007a44c0_TEST FAIL\n"); return 1; }
    std::printf("007a44c0_TEST PASS\n");
    return 0;
}