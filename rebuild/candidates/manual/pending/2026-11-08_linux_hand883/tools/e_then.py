# then-arm (book icon branch) single-lever edits; occurrence 0 = then-arm, 1 = else-arm
SUB = '            frame->AddPositionChild(subtitle.Get());\n'
SST = '            subtitleText.SetStaticCast(subtitle);\n'
SV = '            subtitleText->SetValue(description, true, true);\n'
AC = '            entry->AddChild(subtitle);\n'
EDITS = {
 'T1': [(SUB, '            frame->AddPositionChild(subtitle.Data);\n', 0)],
 'T2': [(SST, '            subtitleText.ShareData(static_cast<CText*>(subtitle.Data), subtitle.Info);\n', 0)],
 'T3': [(SV, '            subtitleText.Get()->SetValue(description, true, true);\n', 0)],
 'T4': [(AC, '            entry.Get()->AddChild(subtitle);\n', 0)],
 'T5': [('if (icon->GetType() == NUISystem::UI_TYPE_SPRITE) {', 'if (icon.Get()->GetType() == NUISystem::UI_TYPE_SPRITE) {', None)],
 'T6': [('''                CSprite* sprite = static_cast<CSprite*>(icon.Get());
                sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(index).BankIndex);''',
         '''                static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(index).BankIndex);''', None)],
 'T10': [('if (CTCInventoryItem::GetEngineGraphic(index).BankIndex != 0) {', 'if (CTCInventoryItem::GetEngineGraphic(index).BankIndex) {', None)],
 'T12': [(SV, '            (*subtitleText).SetValue(description, true, true);\n', 0)],
 'T13': [('if (icon->GetType() == NUISystem::UI_TYPE_SPRITE) {', 'if ((*icon).GetType() == NUISystem::UI_TYPE_SPRITE) {', None)],
 'T15': [('''                sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(index).BankIndex);''',
          '''                CEngineGraphic graphic = CTCInventoryItem::GetEngineGraphic(index);
                sprite->SetGraphicIndex(graphic.BankIndex);''', None)],
 'T16': [('if (CTCInventoryItem::GetEngineGraphic(index).BankIndex != 0) {', 'CEngineGraphic graphic = CTCInventoryItem::GetEngineGraphic(index);\n        if (graphic.BankIndex != 0) {', None)],
 'T17': [('            CCountedPointer<CText> subtitleText;\n            subtitleText.SetStaticCast(subtitle);\n',
          '            CCountedPointer<CText> subtitleText;\n            subtitleText.SetStaticCast<CComponent>(subtitle);\n', 0)],
 'T18': [('if (icon->GetType() == NUISystem::UI_TYPE_SPRITE) {', 'if (NUISystem::UI_TYPE_SPRITE == icon->GetType()) {', None)],
 'T19': [('                CSprite* sprite = static_cast<CSprite*>(icon.Get());', '                CSprite* sprite = (CSprite*)icon.Get();', None)],
 'T20': [('                CSprite* sprite = static_cast<CSprite*>(icon.Get());', '                CSprite* sprite = static_cast<CSprite*>(icon.Data);', None)],
 'T21': [('            CCountedPointer<CComponent> icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));\n            frame->AddPositionChild(icon.Get());\n',
          '            CCountedPointer<CComponent> icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));\n            frame->AddPositionChild(icon.Data);\n', None)],
 'T22': [('            CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));\n', '            CCountedPointer<CComponent> subtitle = manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false);\n', 0)],
 'T23': [('            CCountedPointer<CComponent> icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));\n', '            CCountedPointer<CComponent> icon = manager->CreateComponent("PC_ICON_INDEPENDANT", false);\n', None)],
}
