// Behaviour test of CTCInventoryWeapons::RefreshWeaponDescriptions (005cacb9): with a weapons list, the augmentation entry's text
// becomes "Augment" + "x" + <number of augmentations>; every WEAPON_DESCRIPTION component mapped to a positive item index is
// deleted, unmapped, and replaced by a new description container (PC or console template) named WEAPON_DESCRIPTION, registered
// as a fake child of the old one's viewport, filled for that item against the active melee weapon's damage (melee items) or the
// active ranged weapon's shot damage (ranged items; 0 when no live active weapon), added to the old one's parent and mapped to the
// item; unmapped or non-positive entries are left alone.
#include <cstdio>
#include <string>
#include <vector>
#include <map>
struct CComponent {
    std::string Name, Template, Text; bool Deleted; CComponent* Parent; CComponent* Viewport; std::vector<CComponent*> Children, FakeChildren;
    long Item; float Damage;
    CComponent() : Deleted(false), Parent(0), Viewport(0), Item(0), Damage(-1.0f) { }
};
struct CWeapon { bool Alive; float Damage; };
struct CInventoryWeapons {
    CComponent* WeaponsList;
    std::map<CComponent*, long> WeaponMenuItemMap;
    std::vector<CComponent*> Descriptions;             // FindAllChildren(WEAPON_DESCRIPTION_STRING)
    CComponent* AugmentationEntryText;                 // FindChild(..)->FindChildNonRecursive(..)->FindChildNonRecursive(..)
    unsigned long NumberAugmentations;
    CWeapon Melee, Ranged;
    std::map<long, bool> MeleeItems;
    std::vector<CComponent*> Created;
    void Refresh(bool live)
    {
        if (!WeaponsList) return;
        if (AugmentationEntryText) {
            char b[32]; std::sprintf(b, "%lu", NumberAugmentations);
            AugmentationEntryText->Text = std::string("Augment") + "x" + b;
        }
        float meleeDamage = 0.0f, rangedDamage = 0.0f;
        if (Melee.Alive) meleeDamage = Melee.Damage;
        if (Ranged.Alive) rangedDamage = Ranged.Damage;
        for (std::vector<CComponent*>::iterator it = Descriptions.begin(); it != Descriptions.end(); ++it) {
            CComponent* description = *it;
            long item = WeaponMenuItemMap[description];
            if (item > 0) {
                description->Deleted = true;
                WeaponMenuItemMap.erase(description);
                CComponent* container = new CComponent;
                Created.push_back(container);
                container->Template = live ? "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE" : "UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF";
                container->Name = "WEAPON_DESCRIPTION";
                if (description->Viewport) description->Viewport->FakeChildren.push_back(container);
                container->Item = item;
                container->Damage = MeleeItems[item] ? meleeDamage : rangedDamage;
                if (description->Parent) { description->Parent->Children.push_back(container); container->Parent = description->Parent; }
                WeaponMenuItemMap[container] = item;
            }
        }
    }
};
int main()
{
    int bad = 0;
    CComponent list, view, augText, d1, d2, d3;
    d1.Parent = &list; d1.Viewport = &view; d2.Parent = &list; d3.Parent = &list;
    CInventoryWeapons w;
    w.WeaponsList = &list; w.AugmentationEntryText = &augText; w.NumberAugmentations = 3;
    w.Melee.Alive = true; w.Melee.Damage = 4.5f; w.Ranged.Alive = false; w.Ranged.Damage = 9.0f;
    w.Descriptions.push_back(&d1); w.Descriptions.push_back(&d2); w.Descriptions.push_back(&d3);
    w.WeaponMenuItemMap[&d1] = 11; w.WeaponMenuItemMap[&d2] = 22; w.WeaponMenuItemMap[&d3] = 0;
    w.MeleeItems[11] = true; w.MeleeItems[22] = false;
    w.Refresh(true);
    bad |= augText.Text != "Augmentx3";
    bad |= !d1.Deleted || !d2.Deleted || d3.Deleted || w.Created.size() != 2;
    bad |= w.WeaponMenuItemMap.count(&d1) != 0 || w.WeaponMenuItemMap.count(&d2) != 0;
    bad |= w.Created[0]->Damage != 4.5f || w.Created[1]->Damage != 0.0f || w.Created[0]->Item != 11 || w.Created[1]->Item != 22;
    bad |= view.FakeChildren.size() != 1 || view.FakeChildren[0] != w.Created[0] || list.Children.size() != 2;
    bad |= w.WeaponMenuItemMap[w.Created[1]] != 22 || w.Created[0]->Template != "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE";
    bad |= w.Created[0]->Name != "WEAPON_DESCRIPTION";
    CInventoryWeapons none; none.WeaponsList = 0; none.Refresh(false);           // no list: nothing happens
    bad |= !none.Created.empty();
    for (unsigned i = 0; i < w.Created.size(); ++i) delete w.Created[i];
    if (bad) { std::printf("005cacb9_TEST FAIL\n"); return 1; }
    std::printf("005cacb9_TEST PASS\n");
    return 0;
}