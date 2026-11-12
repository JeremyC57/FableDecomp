// Behaviour test for CTCInventoryAbilities::UnAssignAbility (0x005F11A9): the same logic run through a small model
// (vectors for the quick slots and their saves, maps for the menu entries and the unassigned icons).
#include <cstdio>
#include <vector>
#include <map>

struct Component {
    int children;        // "unassigned" icons added to this menu entry
    bool hasIconChild;   // FindChild("UI_UNASSIGNED_SPELL_ICON") succeeds
    int deletions;       // SetDeletion(UI_DELETE_THIS_INSTANCE) calls on the icon child
    Component() : children(0), hasIconChild(false), deletions(0) {}
};

struct Model {
    std::vector<long> AssignedAbilities;
    std::vector<long> AssignedAbilitiesSave;
    std::map<long, Component*> UnassignedIcons;
    std::map<long, Component*> AbilitiesMenuEntries;
    int created;
    Model() : created(0) {}

    void UnAssignAbility(unsigned long slot)
    {
        long current = AssignedAbilities[slot];
        long saved = AssignedAbilitiesSave[slot];
        long removed = AssignedAbilities[slot];
        long ability = removed;
        if (current != saved) {
            AssignedAbilities[slot] = saved;
        } else {
            AssignedAbilities[slot] = 0;
            AssignedAbilitiesSave[slot] = 0;
        }
        bool stillAssigned = false;
        for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
            if (AssignedAbilities[i] == ability) {
                stillAssigned = true;
                break;
            }
        }
        if (!stillAssigned && ability != 0) {
            Component* icon = new Component;
            ++created;
            AbilitiesMenuEntries[removed]->children++;
            UnassignedIcons[removed] = icon;
        }
        if (AssignedAbilities[slot] != 0) {
            std::map<long, Component*>::iterator found = UnassignedIcons.find(AssignedAbilities[slot]);
            if (found != UnassignedIcons.end()) {
                Component* entry = found->second;
                if (entry->hasIconChild)
                    entry->deletions++;
                UnassignedIcons.erase(found);
            }
        }
    }
};

int main()
{
    bool ok = true;
    Component menu5, menu7, icon7, icon9;
    icon7.hasIconChild = true;

    // 1. slot holds 5, its save holds 7: the slot reverts to 7, 5 is assigned nowhere any more so its menu entry gets a
    //    fresh icon, and the unassigned icon of 7 (now back in the slot) is deleted and forgotten.
    {
        Model m;
        m.AssignedAbilities.push_back(5); m.AssignedAbilities.push_back(3);
        m.AssignedAbilitiesSave.push_back(7); m.AssignedAbilitiesSave.push_back(3);
        m.AbilitiesMenuEntries[5] = &menu5;
        m.UnassignedIcons[7] = &icon7;
        m.UnAssignAbility(0);
        if (m.AssignedAbilities[0] != 7 || m.AssignedAbilitiesSave[0] != 7) ok = false;
        if (m.created != 1 || menu5.children != 1) ok = false;
        if (m.UnassignedIcons.find(5) == m.UnassignedIcons.end()) ok = false;
        if (m.UnassignedIcons.find(7) != m.UnassignedIcons.end() || icon7.deletions != 1) ok = false;
    }
    // 2. slot and save agree (5 and 5): both are cleared, 5 gets its icon, and the empty slot skips the tail.
    {
        Model m;
        menu5.children = 0;
        m.AssignedAbilities.push_back(5);
        m.AssignedAbilitiesSave.push_back(5);
        m.AbilitiesMenuEntries[5] = &menu5;
        m.UnassignedIcons[9] = &icon9;
        m.UnAssignAbility(0);
        if (m.AssignedAbilities[0] != 0 || m.AssignedAbilitiesSave[0] != 0) ok = false;
        if (m.created != 1 || menu5.children != 1) ok = false;
        if (m.UnassignedIcons.size() != 2) ok = false;
    }
    // 3. the removed ability is still in another slot: no icon is made.
    {
        Model m;
        menu5.children = 0;
        m.AssignedAbilities.push_back(5); m.AssignedAbilities.push_back(5);
        m.AssignedAbilitiesSave.push_back(7); m.AssignedAbilitiesSave.push_back(5);
        m.AbilitiesMenuEntries[5] = &menu5;
        m.UnAssignAbility(0);
        if (m.AssignedAbilities[0] != 7 || m.created != 0 || menu5.children != 0) ok = false;
        if (!m.UnassignedIcons.empty()) ok = false;
    }
    // 4. an empty slot with a saved ability: nothing was removed, so no icon, but the restored ability's icon is erased
    //    (no child to delete this time).
    {
        Model m;
        m.AssignedAbilities.push_back(0);
        m.AssignedAbilitiesSave.push_back(9);
        m.UnassignedIcons[9] = &icon9;
        m.UnAssignAbility(0);
        if (m.AssignedAbilities[0] != 9 || m.created != 0) ok = false;
        if (!m.UnassignedIcons.empty() || icon9.deletions != 0) ok = false;
    }
    if (!ok) { std::printf("005f11a9_TEST FAIL\n"); return 1; }
    std::printf("005f11a9_TEST PASS\n");
    return 0;
}