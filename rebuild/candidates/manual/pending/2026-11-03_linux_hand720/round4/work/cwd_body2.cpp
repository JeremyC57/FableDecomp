void CTCInventoryWeapons::ConstructWeaponDescription(CWideString& description, long defIndex, float damage, bool useDefaultItemData) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long miscThingsIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(miscThingsIndex, &miscThingsDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    NInventory::CPerItemData defaultItemData;
    const NInventory::CPerItemData* itemData;
    if (useDefaultItemData) {
        CDefPointer<const CObjectAugmentationsDef> augmentationsDef;
        if (def->GetSubDef(&augmentationsDef)) {
            defaultItemData.Augmentations.resize(augmentationsDef->NumberOfSlots, 0);
            for (unsigned long i = 0; i < augmentationsDef->InitialAugmentations.size() && i < defaultItemData.Augmentations.size(); ++i)
                defaultItemData.Augmentations[i] = augmentationsDef->InitialAugmentations[i];
        }
        itemData = &defaultItemData;
    } else {
        itemData = GetPerItemDataForItem(defIndex);
    }
    float weaponDamage;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        weaponDamage = weaponDef->Damage;
    } else {
        CDefPointer<const CThingShotDef> shotDef = GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot);
        weaponDamage = shotDef->HitThingDamage;
    }
    long baseDamage = GFFastFloatToLong(CTCObjectAugmentations::GetDamageMultiplierFromAugmentationSet(itemData->Augmentations) * weaponDamage * 10.0f);
    long currentDamage = GFFastFloatToLong(damage * 10.0f);
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    long damageDifference = baseDamage - currentDamage;
    description += textBank.GetTextBySymbol("TEXT_GUI_MENU_DAMAGE") + miscThingsDef->SpaceSeparator;
    description += GFIntToWideString(baseDamage) + miscThingsDef->SpaceSeparator;
    description += miscThingsDef->OpenBracket;
    if (damageDifference > 0)
        description += miscThingsDef->Positive;
    description += GFIntToWideString(damageDifference) + miscThingsDef->CloseBracket;
    description += NEWLINE;
    description += textBank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscThingsDef->SpaceSeparator;
    const std::vector<long>& augmentations = itemData->Augmentations;
    if (!augmentations.empty()) {
        long lastAugmentation = -1;
        long count = augmentations.size();
        long numAugmentations = 0;
        for (long i = 0; i < count; ++i) {
            if (augmentations[i] > 0) {
                ++numAugmentations;
                lastAugmentation = i;
            }
        }
        if (numAugmentations > 0) {
            for (long i = 0; i < count; ++i) {
                if (augmentations[i] > 0) {
                    description += CTCObjectAugmentations::GetAugmentationName(augmentations[i]);
                    if (i < lastAugmentation)
                        description += miscThingsDef->CommaSeparator;
                }
            }
        } else {
            description += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
        }
    } else {
        description += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    }
    description += NEWLINE;
    if (!useDefaultItemData) {
        CDefPointer<const CStockItemDef> stockItemDef;
        def->GetSubDef(&stockItemDef);
        long value = stockItemDef->DefaultPrice;
        const CTCVolumeContainmentTracker* tracker;
        if (ParentThing->PeekTC(&tracker)) {
            CThing* thingInside = tracker->PThingInside.Get();
            CTCShop* shop;
            if (IS_THING_ALIVE(thingInside) && thingInside->GetTC(&shop) && shop->IsValidStockItem(defIndex))
                value = shop->GetCurrentBuyPrice(defIndex, 1);
        }
        description += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_VALUE") + miscThingsDef->SpaceSeparator + GFIntToWideString(value);
        description += NEWLINE;
    }
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        description += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + miscThingsDef->SpaceSeparator;
        if (weaponDef->Class == WC_HEAVY)
            description += textBank.GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
        else if (weaponDef->Class == WC_LIGHT)
            description += textBank.GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
        description += NEWLINE;
    }
    description += textBank.GetTextBySymbol("TEXT_GUI_MENU_KILLS") + miscThingsDef->SpaceSeparator;
    description += GFIntToWideString(itemData->NumberOfKills);
    description += NEWLINE;
}
