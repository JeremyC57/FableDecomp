// Behaviour test for 005f0c5f CTCInventoryAbilities::AssignAbility(long, unsigned long): the function body from src/005f0c5f.cpp
// verbatim over self-contained C++03 stand-ins (std::vector / std::map for the STLport containers, a recording NUISystem model).
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <string>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

enum EHeroAbility { HERO_ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 5, ABILITY_C = 7 };
namespace NUISystem {
class CDeletion { public:
    enum EMethod { UI_DELETE_NONE = 0, UI_DELETE_THIS_INSTANCE = 2 };
    EMethod Method;
    CDeletion() : Method(UI_DELETE_NONE) {}
};
class CComponent;
}
template <class T> class CCountedPointer { public:
    T* Data;
    explicit CCountedPointer(T* data) : Data(data) {}
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};
namespace NUISystem {
class CComponent { public:
    std::string Name;
    std::vector<CComponent*> Children;
    int Deleted;
    explicit CComponent(const char* name) : Name(name), Deleted(0) {}
    void AddChildDynamically(CCountedPointer<CComponent>& child) { Children.push_back(child.Get()); }
    CComponent* FindChild(const char* name) {
        for (unsigned long i = 0; i < Children.size(); ++i)
            if (Children[i]->Name == name) return Children[i];
        return 0;
    }
    void SetDeletion(const CDeletion& deletion) { Deleted = (int)deletion.Method; }
};
static std::vector<CComponent*> g_created;
class CManager { public:
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const char* name, bool) { CComponent* c = new CComponent(name); g_created.push_back(c); return c; }
};
}
class CTCInventoryAbilities { public:
    std::vector<EHeroAbility> AssignedAbilities;
    std::map<EHeroAbility, NUISystem::CComponent*> UnassignedIcons;
    std::map<EHeroAbility, NUISystem::CComponent*> AbilitiesMenuEntries;
    void AssignAbility(long ability, unsigned long slot);
};

// ---- source body (verbatim from src/005f0c5f.cpp) ----
void CTCInventoryAbilities::AssignAbility(long ability, unsigned long slot)
{
    {
        EHeroAbility current = AssignedAbilities[slot];
        bool assignedElsewhere = false;
        for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
            if (AssignedAbilities[i] == current && i != slot)
                assignedElsewhere = true;
        }
        if (!assignedElsewhere && ability != current && current != 0) {
            CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));
            AbilitiesMenuEntries[current]->AddChildDynamically(icon);
            UnassignedIcons[current] = icon.Get();
        }
    }
    AssignedAbilities[slot] = static_cast<EHeroAbility>(ability);
    std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(static_cast<EHeroAbility>(ability));
    if (found != UnassignedIcons.end()) {
        NUISystem::CComponent* icon = found->second->FindChild("UI_UNASSIGNED_SPELL_ICON");
        if (icon) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            icon->SetDeletion(deletion);
        }
        UnassignedIcons.erase(found);
    }
}

// ---- driver ----
int main()
{
    using NUISystem::CComponent;
    CComponent entryA("ENTRY_A"), entryB("ENTRY_B"), entryC("ENTRY_C");
    CTCInventoryAbilities inv;
    inv.AssignedAbilities.push_back(ABILITY_A);
    inv.AssignedAbilities.push_back(HERO_ABILITY_NULL);
    inv.AssignedAbilities.push_back(ABILITY_C);
    inv.AbilitiesMenuEntries[ABILITY_A] = &entryA;
    inv.AbilitiesMenuEntries[ABILITY_B] = &entryB;
    inv.AbilitiesMenuEntries[ABILITY_C] = &entryC;

    // 1. Replacing A (only in slot 0) with B: A's menu entry gets an unassigned icon, B had none.
    inv.AssignAbility(ABILITY_B, 0);
    CHECK(inv.AssignedAbilities[0] == ABILITY_B);
    CHECK(NUISystem::g_created.size() == 1);
    CHECK(entryA.Children.size() == 1 && entryA.Children[0]->Name == "UI_UNASSIGNED_SPELL_ICON");
    CHECK(inv.UnassignedIcons.size() == 1 && inv.UnassignedIcons[ABILITY_A] == entryA.Children[0]);

    // 2. Assigning A into the empty slot 1: no new icon (current is null); A's unassigned icon is found and deleted.
    // The model looks the icon up as a child of the map's component, so make the icon its own child.
    CComponent* iconA = entryA.Children[0];
    iconA->Children.push_back(iconA);
    inv.AssignAbility(ABILITY_A, 1);
    CHECK(inv.AssignedAbilities[1] == ABILITY_A);
    CHECK(NUISystem::g_created.size() == 1);
    CHECK(iconA->Deleted == NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE);
    CHECK(inv.UnassignedIcons.find(ABILITY_A) == inv.UnassignedIcons.end());

    // 3. Re-assigning the same ability: nothing is created.
    inv.AssignAbility(ABILITY_C, 2);
    CHECK(NUISystem::g_created.size() == 1);

    // 4. Replacing an ability still assigned in another slot: no unassigned icon.
    inv.AssignedAbilities[2] = ABILITY_B;   // B now in slots 0 and 2
    inv.AssignAbility(ABILITY_C, 2);
    CHECK(NUISystem::g_created.size() == 1);
    CHECK(inv.AssignedAbilities[2] == ABILITY_C);
    CHECK(entryB.Children.empty());

    // 5. Unassigned icon present but no matching child: entry is still erased, nothing deleted.
    CComponent holder("HOLDER");
    inv.UnassignedIcons[ABILITY_C] = &holder;
    inv.AssignAbility(ABILITY_C, 0);   // replaces B (still in no other slot? slot 0 B, slot2 C) -> B gets an icon
    CHECK(inv.UnassignedIcons.find(ABILITY_C) == inv.UnassignedIcons.end());
    CHECK(holder.Deleted == 0);
    CHECK(NUISystem::g_created.size() == 2 && entryB.Children.size() == 1);

    for (unsigned long i = 0; i < NUISystem::g_created.size(); ++i) delete NUISystem::g_created[i];
    if (fails) { std::printf("005f0c5f_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005f0c5f_TEST PASS\n");
    return 0;
}