// CTCInventoryClothing::ConstructScreenPC(NUISystem::CComponent* parent) -- builds the PC clothing screen: dresses the mannequin, sorts the hero's clothing into six suit-part lists, then the PC_SUB_LIST with one menu entry per non-empty part, the title, the per-modifier description entries and the PC_DESCRIPTION_VIEWPORT
// Leaf: ?ConstructScreenPC@CTCInventoryClothing@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// STATUS: PARKED, NOT COMPILED. This file is a flow skeleton only (agentSCR, 2026-11-10); see ../NOTES.md "0x005B8743".
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B8743..0x005B9641 (3,838 bytes), thiscall, ret 4, `push ebp; lea ebp,[esp-74h]; sub esp,124h`
//   (frame 0x124, ebp biased -0x74). Ego_r: non-virtual (QAE), donor 0x005641A2; its only caller is 0x0041FBDD.
//
// Flow (from $S/w22/agent900/ann_005b8743.txt):
//   InitialiseMannequin() 0x005B37F7; this->+0x15c = true; SetUpSelectedClothes() 0x005B4795;
//   CManager::GetInstance()->[slot 0x58](this) (SetInventoryClothing, Ego_r 0x0043ADB8); ->[slot 0x68](ParentThing) (SetPlayerThing);
//   frame = parent->FindChild("PC_UI_FRAME");
//   ItemVector items; GetAllItems(items) 0x005BF482;
//   six vector<pair<long,long>> part lists (head, hands, torso, suits, legs, feet; zero-initialised at ebp-0x50/-0x88/-0x74/-0x5c/-0x44/-0x68)
//   for (i = 0; i < items.size(); ++i) {
//       CDefPointer<CDefClassBase> def; PDefManager->GetDef(items[i].DefIndex, &def) 0x0042AF3C;
//       CDefPointer<CHeroSuitDef> suit;
//       if (def->GetSubDef(&suit)) 0x005935C4 {                       // a hero suit: walk its part set (set<EClothingSuitPart> -- agentNF)
//           for (it = suit->Parts.begin(); it != end; it = _Helper(it) 0x004292C0) {
//               if (GetNumberOfItemsOfTypeInInventory(it->DefIndex) 0x005BDF08 > 0 &&
//                   items[i].DefIndex != CountOf("HERO_SUIT_NAKED") && it->DefIndex != CountOf("OBJECT_HERO_NO_HAT") && ... NO_SHIRT,
//                   NO_GLOVES, NO_TROUSERS, NO_BOOTS)      // short-circuit chain; the six CCharString temporaries live behind a
//                   { suitParts.PushBack(items[i]) 0x005947B6; break; }   // flag word [ebp+6c] bits 1..0x20 (conditional dtors)
//           }
//       } else {
//           CDefPointer<const CAppearanceModifierDef> mod;
//           if (def->GetSubDef(&mod)) 0x0057FF92                         // switch (mod->+0x4c part 1..5) -> PushBack into list 1..5
//       }
//   }
//   textBank = g_13b86a0->+0x14;
//   vector<vector<pair<long,long> > > lists; vector<long> partIds; vector<CWideString> names; CChildVector descs;
//   per part (HEAD, HANDS, TORSO, SUITS, LEGS, FEET), if its list is non-empty:
//       lists.insert(list) 0x005BC237; names.push_back(textBank->GetTextBySymbol("TEXT_GUI_MENU_<PART>")) 0x004128A0;
//       descs.push_back(CreateMenuDescPC("TEXT_GUI_MENUGUIDE_CLOTHING_<PART>")) 0x005B3613 / 0x00535AD0;
//       if (frame) frame->[0x1f0](descs.back());  partIds.push_back(<part>) 0x005BBA94;
//   list = CreateComponent("PC_SUB_LIST"); title = CreateComponent("PC_TITLE") ("CLOTHING TITLE" name, slot 0x14c);
//   CCountedPointer<CText> text; ShareData 0x00429460; CWideString s; ConstructTitleString(s) 0x005B6881; text->[0x240]/[0x244];
//   frame->[0x1f0](title); list->[0x270]; ...
//   CLongList modifiers; CTCHeroAttachableAppearanceModifiers::GetModifiers(modifiers, ..) 0x00706800; map 0x005BB3D1;
//   per modifier: GetDef(CDefPointerObjectDef) 0x0042B040, GetSubDef<CAppearanceModifierDef> 0x0057FF92, map[] 0x005BBC4E;
//   viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT"); frame->[0x1f0](viewport); _Rb_tree clear 0x004DB806;
//   per part entry: name + GetXString() + GFIntToWideString(count);
//       CreateMenuEntryComponent(name, .., "PC_MENU_ENTRY_BRANCH_TEMPLATE", .., "PC_BUTTON_BASTARD", .., g_profileTag) 0x00428494;
//       FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE"); two __ftol2 positions; new CAction (0x10) actions with CActionParamString
//       and CMember::Set 0x0042B646; ShareData<CMenuEntry> 0x0048524C; subtitle "PC_SUBTITLE" with ConvertToCharString;
//       sub-list via 0x005B6A66 (probably ConstructClothingListPC, Ego_r donor 0x00562A81) and Reset 0x00429C6F;
//   cleanup: list dtors 0x005BB090 / 0x005BC080, vector<CCountedPointer<CComponent>> dtor 0x00535900, CWideStringVector dtor,
//   eight vector frees; return list.
//
// Not compiled: no CTCInventoryClothing retail header with the NHeroInformationScreens::CBase members (+0x130..) or the
// ConstructScreenPC / ConstructClothingListPC declarations exists yet, the six helpers above have no Ego_r-proven spellings,
// and the suit-part set / damage map instantiations belong to agentNF (codex20 entries 5..10).
