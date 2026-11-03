inline float CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
        CTCWeapon* weapon = 0;
        PActiveMeleeWeapon->GetTC(&weapon);
        damage = weapon->GetDamage();
    }
    return damage;
}
inline float CTCInventoryWeapons::GetActiveProjectileWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<const CWeaponDef> weaponDef;
        PActiveRangedWeapon->GetPBaseDef()->GetSubDef(&weaponDef);
        CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
        damage = shotDef->HitThingDamage;
    }
    return damage;
}
void CTCInventoryWeapons::RefreshWeaponDescriptions()
{
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long miscThingsIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(miscThingsIndex, &miscThingsDef);
    std::list<NUISystem::CComponent*> descriptions;
    if (WeaponsList) {
        NUISystem::CComponent* augmentEntry = WeaponsList->FindChild(g_CCharString_013b8bc0);
        if (augmentEntry) {
            NUISystem::CText* text = (NUISystem::CText*)augmentEntry->FindChildNonRecursive("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE")->FindChildNonRecursive("MENU_ENTRY_TEXT");
            CWideString label = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_AUGMENT") + CTCInventoryStats::GetXString();
            label += GFIntToWideString(NUISystem::CManager::GetInstance()->GetNumberAugmentations());
            text->SetValue(label, true, true);
        }
        WeaponsList->FindAllChildren(g_CCharString_013b8bac, descriptions);
        float meleeDamage = GetActiveMeleeWeaponDamage();
        float rangedDamage = GetActiveProjectileWeaponDamage();
        std::list<NUISystem::CComponent*>::iterator end = descriptions.end();
        for (std::list<NUISystem::CComponent*>::iterator it = descriptions.begin(); it != end; ++it) {
            NUISystem::CComponent* entry = *it;
            long defIndex = WeaponMenuItemMap[entry];
            if (defIndex > 0) {
                CDefPointer<const CThingBaseDef> thingDef;
                GetDefinitionManager().CDefinitionManager::GetDef(defIndex, &thingDef);
                CDefPointer<const CWeaponDef> weaponDef;
                thingDef->GetSubDef(&weaponDef);
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                entry->SetDeletion(deletion);
                WeaponMenuItemMap.erase(entry);
                CCountedPointer<NUISystem::CComponent> container;
                if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
                else
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF", false));
                container->SetName(g_CCharString_013b8bac);
                if (entry->GetParentViewport()) {
                    NUISystem::CViewport* viewport = (NUISystem::CViewport*)entry->GetParentViewport();
                    if (viewport)
                        viewport->AddFakeChild(container.Get());
                }
                ConstructWeaponDescriptionContainer(container.Get(), defIndex, CTCWeapon::IsMeleeWeaponClass(weaponDef->Class) ? meleeDamage : rangedDamage);
                if (entry->GetParent())
                    entry->GetParent()->AddChildDynamically(container);
                SetWeaponMappingToMenuEntry(container.Get(), defIndex);
            }
        }
    }
}
