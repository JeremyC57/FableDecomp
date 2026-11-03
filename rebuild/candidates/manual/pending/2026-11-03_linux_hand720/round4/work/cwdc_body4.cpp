void CTCInventoryWeapons::ConstructWeaponDescriptionContainer(NUISystem::CComponent* container, long defIndex, float damage) const
{
    RefreshActiveWeaponData();
    CCountedPointer<NUISystem::CComponent> textComponent;
    CWideString text;
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef;
    long miscThingsIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(miscThingsIndex, &miscThingsDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    const NInventory::CPerItemData* itemData = GetPerItemDataForItem(defIndex);
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
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_DAMAGE") + miscThingsDef->SpaceSeparator;
    text += GFIntToWideString(baseDamage) + miscThingsDef->SpaceSeparator;
    text += miscThingsDef->OpenBracket;
    if (damageDifference > 0)
        text += miscThingsDef->Positive;
    text += GFIntToWideString(damageDifference) + miscThingsDef->CloseBracket;
    text += NEWLINE;
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscThingsDef->SpaceSeparator;
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
                    text += CTCObjectAugmentations::GetAugmentationName(augmentations[i]);
                    if (i < lastAugmentation)
                        text += miscThingsDef->CommaSeparator;
                }
            }
        } else {
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
        }
    } else {
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    }
    text += NEWLINE;
    {
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
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_VALUE") + miscThingsDef->SpaceSeparator + GFIntToWideString(value);
    }
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    NUISystem::CText* textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        text += textBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + miscThingsDef->SpaceSeparator;
        if (weaponDef->Class == WC_HEAVY)
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
        else if (weaponDef->Class == WC_LIGHT)
            text += textBank.GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
    }
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
    if (weaponDef->Class == WC_HEAVY) {
        const CTCHeroExperience* experience;
        if (ParentThing->PeekTC(&experience) && ParentThing->HasTC(TCI_INVENTORY_EXPERIENCE)) {
            CDefPointer<const CHeroCombatDef> combatDef;
            combatDef = experience->GetPHeroCombatDef();
            if (!combatDef.IsNull()) {
                const std::map<EHeroTrainableStatType, long>& thresholds = combatDef->HeavyWeapons.StatThresholds;
                for (std::map<EHeroTrainableStatType, long>::const_iterator it = thresholds.begin(); it != thresholds.end(); ++it) {
                    if (experience->GetTrainableStatLevel(it->first) < it->second) {
                        NUISystem::CUIState& blueState = textItem->GetStates()[0];
                        blueState.Colour.B *= 0.0f;
                        NUISystem::CUIState& greenState = textItem->GetStates()[0];
                        greenState.Colour.G *= 0.0f;
                    }
                }
            }
        }
    }
    text += textBank.GetTextBySymbol("TEXT_GUI_MENU_KILLS") + miscThingsDef->SpaceSeparator;
    text += GFIntToWideString(itemData->NumberOfKills);
    text += NEWLINE;
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    else
        textComponent.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    textItem = static_cast<NUISystem::CText*>(textComponent.Get());
    container->AddChild(textComponent);
    textItem->SetValue(text, true, true);
    text = g_EmptyWideString;
}
