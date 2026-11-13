# OUTLINE 0x00651B04 NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo& info)

## Frame facts
- `push ebp ; lea ebp,[esp-74h] ; sub esp,0E0h ; push edi` ... (after the alive test) `push ebx ; push esi`.
  Epilogue 0x00652F00 `pop esi ; pop ebx` / 0x00652F02 `pop edi ; add ebp,74h ; leave ; ret 4` (early exit enters at pop edi).
- `#pragma optimize("s", on)` only (biased frame; CDrawPerceivers::Draw 0x0064499C proof). No EH, no /GS. Flags: none.
- Frame: locals ebp-6Ch .. ebp+73h (0xE0 bytes); [ebp+74h] saved ebp, [ebp+7Ch] = &info (arg).
- Register plan: edi = 0 constant (after spilling this to [ebp+70h]); ebx = 255 from 0x00651C3B on (0x6F before); esi = &def
  (PeekGuiDef result) until 0x00652E09 where it becomes &def.ABXYMiddleOffset, then this.

## Frame map (slot -> local; slots are shared between scopes)
| slot | local | type | notes |
|---|---|---|---|
| ebp-6Ch | sprite | CEnginePrimitive2DSprite | built by 0x00493AC2 at every sprite; no dtor call |
| ebp-4Ch | ev | CInputEvent | ctor 0x00A04410, dtor 0x00A03B30 (both loops) |
| ebp-18h, ebp-10h | anchored (loop / tail) / GetVStringExtents result | C2DVector / C2DExtentsF | hidden results of GFAnchorToBottomRight, GetVStringExtents |
| ebp-8 .. ebp+13h | control | CActionInputControl | 5 dwords zeroed (ebp-8,-4,0,+4,+8) + floats +0Ch,+10h = 0.0f |
| ebp+14h | text extents / drawPos (loop 1) / uv / overlay pos | C2DExtentsF / C2DVector | |
| ebp+1Ch | &def.ABXYRingsPos | const C2DVector* | the anchor source, set once 0x00651C22, reused by every later GFAnchor of the rings |
| ebp+20h | anchored (white/black) / special-button pos / uv | C2DVector | |
| ebp+28h | ringsGraphic | long | = 0 at 0x00651C1F; joystick override; tail default ABXYButtonMiddleGraphic |
| ebp+2Ch | abilities | CTCInventoryAbilities* | PeekTC 0x6F, = 0 first |
| ebp+30h | hero / ability (loop1) / j (loop2) / graphic (loop3) | CTCHero* / long | |
| ebp+34h | spritePos | C2DVector | by-value copy of pos for the sprite |
| ebp+38h | &SpecialPrimitiveHandles / moveIndex / temps | | |
| ebp+3Ch | weapons / i (loop1) / byte offset (loop3) | CTCInventoryWeapons* / unsigned long | |
| ebp+40h | pos | C2DVector | main running position |
| ebp+4Bh | normal | bool | "draw the normal ABXY set" |
| ebp+4Ch | uvA / buttonPos (loop1) / text pos | C2DVector | |
| ebp+54h | creature | const CThingPlayerCreature* | |
| ebp+58h | meleeWeapon / font / name (loop1) / abilityDef (loop3) | CThing* / const CFontBank* / CWideString / CDefPointer | |
| ebp+5Ch | quickMenu / carrying / alpha (loop1) / offset (loop2) / drawAlpha (loop3) / alphaByte (tail) | | |
| ebp+60h | uvB / white tooltip pos | C2DVector | |
| ebp+64h | PeekTC key temp / item / button ref | | |
| ebp+68h | weaponTC / rangedWeapon / colour (68..6B) / specialAlpha (loop3) | CTCWeapon* / CThing* / CRGBColour / float | |
| ebp+6Ch | graphic / name (white+black) / byte offset (loop1) / k (loop3) | long / CWideString / unsigned long | |
| ebp+70h | this | CDrawAbilityButtons* | |

## Sprite idiom (8 times) -- copy DrawButton 0x00645099
```
C2DVector spritePos(pos);                 // [ebp+34h]/[ebp+38h] copy (mov from pos, or x87 store)
CEnginePrimitive2DSprite sprite(0 /*u64*/, spritePos, LAYER, graphic, 0, (EEnginePrimitiveBlendMode)2, (EEngineInterpolationSet)0,
                                0.0f, 0.0f, ScaleAlpha(A), 255, 255, 255, 0, C2DVector(0,0) /*uvB [60h]*/, C2DVector(0,0) /*uvA [4Ch]*/);
info.Engine->SetPersistentPrimitive(&HANDLE, sprite, 0xC0);
```
Push order (last pushed = arg1): &uvA[4Ch], &uvB[60h], 0, 255, 255, 255, ScaleAlpha(A), 0.0f, 0.0f, 0, 2, 0, graphic, LAYER, &spritePos, 0, 0.
Check the uchar order (ScaleAlpha first of the four) against DrawButton's source. uvs: `fldz` stores [4Ch],[50h] then [60h],[64h].

## Block-by-block (retail order)
```
0x00651B04  this -> edi, [ebp+70h]
            const CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().PeekPControlledCreature();   // 449970, 487DD0
0x00651B24  if (!IS_THING_ALIVE(creature)) return;                       // 43DBF0 (lea ecx,[ebp+54h]) -> 0x00652F02
0x00651B39  SetAsDrawing(true);                                          // mov byte [edi+18h],1
            const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();          // 438570 -> esi
0x00651B42  PrimitiveHandles.erase(PrimitiveHandles.begin(), PrimitiveHandles.end());          // 440D70 (ptr kept [ebp+64h])
0x00651B54  SpecialPrimitiveHandles.erase(SpecialPrimitiveHandles.begin(), SpecialPrimitiveHandles.end());  // ptr kept [ebp+38h]
0x00651B64  InteractBorderPrimitiveHandle.Clear();   // inline: Data&&count==1 -> Data->vslot3(&h) ; release (dec, vslot1) ; Data=0 ; Data=0
0x00651B9C  PrimitiveHandles.resize(def.AbilityButtons.size() + 48);          // 443F90
0x00651BBA  SpecialPrimitiveHandles.resize(def.SpecialAbilityButtons.size() + 3);
0x00651BD8  CTCInventoryAbilities* abilities = 0;  creature->PeekTC(TCI_HERO_ABILITIES /*0x6F*/, &abilities);   // [ebp+2Ch]
            // `push 6Fh ; pop ebx` ; test 410DE0 ; LowerBound 4365B0 ; select ; [eax+4]
0x00651C16  long ringsGraphic = 0;                                       // [ebp+28h]
            C2DVector pos = GFAnchorToBottomRight(def.ABXYRingsPos);      // 643D0C, edx=&def+214h kept [ebp+1Ch], result [ebp+40h]
0x00651C2A  bool normal = false;                                         // [ebp+4Bh]
            if (!PlayerGui->NormalToSpecial && !PlayerGui->SpecialToNormal && PlayerGui->ShowSpecialAbilityButtons) {
0x00651C54      CTCQuickAccessMenu* qam = 0; creature->PeekTC(TCI_QUICK_ACCESS_MENU /*0xF6*/, &qam);   // [ebp+5Ch]
0x00651C9E      if (!qam->IsActive()) goto FAVOURITE;                   // 625930 -> 0x00651D10
            }
0x00651CAA  normal = true;
            if (def.ABXYRingsGraphic > 0)          sprite(ABXYRingsGraphic, pos, A=g_byte_01375744, layer 0x46) -> PrimitiveHandles[0]
            goto TOP;                                                    // (tail cross-jumped at 0x00651D70)
FAVOURITE:
0x00651D10  if (def.ABXYRingsFavouriteGraphic > 0) sprite(ABXYRingsFavouriteGraphic, pos, A=g_byte, 0x46) -> PrimitiveHandles[0]
TOP:
0x00651D9C  pos += def.ABXYRingsTopOffset;   // x kept in st0 (stored only into spritePos [34h] or dropped by `fstp st(0)` at 0x00651EA4)
            if (normal) { if (def.ABXYRingsTopGraphic > 0)          sprite(.., pos, g_byte, 0x46) -> PrimitiveHandles[1] }   // 0x00651DB6
            else        { if (def.ABXYRingsTopFavouriteGraphic > 0) sprite(.., pos, g_byte, 0x46) -> PrimitiveHandles[1] }   // 0x00651E15
0x00651EA6  CTCInventoryWeapons* weapons = 0; creature->PeekTC(TCI_INVENTORY_WEAPONS /*0x13*/, &weapons);   // [ebp+3Ch]
0x00651EEA  CTCCarrying* carrying = 0;        creature->PeekTC(TCI_CARRYING /*0x46*/, &carrying);           // [ebp+5Ch]
0x00651F2E  pos = GFAnchorToBottomRight(def.WhiteButtonPos);             // result [ebp+20h], copied into pos
            if (def.WhiteButtonGraphic > 0) sprite(WhiteButtonGraphic, pos, 255, 0x46) -> PrimitiveHandles[6]
0x00651FCB  pos += def.WhiteButtonIconOffset;
            CTCHero* hero = 0; creature->PeekTC(TCI_HERO /*0x29*/, &hero);   // [ebp+30h]  (push 29h interleaved with the add)
0x00652026  if (hero->WeaponsUsable) {
0x00652033    CThing* melee = weapons->PActiveMeleeWeapon.GetPItem();   // A01B50 on +0x134 -> [ebp+58h]
0x00652041    if (IS_THING_ALIVE(melee)) {                              // 40DCE0
0x00652054      CTCInventoryItem* item; if (melee->PeekTC(TCI_INVENTORY_ITEM /*0x1A*/, &item)) {   // NO zero init; [ebp+64h]
                  long graphic;                                          // [ebp+6Ch]
0x00652096        if (carrying->GetThingInPrimarySlot() == melee) {      // 6A1440
0x006520AA          CTCWeapon* w; if (!melee->PeekTC(TCI_WEAPON /*0x48*/, &w)) goto BLACK;   // [ebp+68h]
0x006520EE          if (w->GetWeaponType() == 2 && melee->GetDefName() == "OBJECT_HERO_STICK") graphic = def.AbilityButtonSheatheStickGraphic;   // 5DA7D0, 4C7CC0, 5B3440
0x0065211F          else if (w->GetWeaponType() == 2) graphic = def.AbilityButtonSheatheMaceGraphic;
0x00652134          else if (w->GetWeaponType() == 1) graphic = def.AbilityButtonSheatheAxeGraphic;
0x00652149          else graphic = def.AbilityButtonSheatheSwordGraphic;
                  } else
0x00652151          graphic = item->GetEngineGraphic().BankIndex;        // 5D92D0 ; mov eax,[eax]
0x0065215B        if (graphic > 0) {
                    sprite(graphic, pos, 255, layer 0x44) -> PrimitiveHandles[7]
0x006521E0          if (CUserProfileManager::Get().IsHUDTooltipEnabled()) {          // 40D2A0 ; inline ?: on +0C / +D9 / +19
0x006521FE            CActionInputControl control;                                    // [ebp-8]
                      if (CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0D, true, &control) ||
                          CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0D, false, &control)) {   // 408C90 x2
0x0065224A              CWideString name;                                              // [ebp+6Ch] 99AED0
                        if (GetActionName(control, name, false)) {                     // 446720
0x00652266                const CFontBank& font = Component->PeekCutSceneMainFont();   // +0x1614C -> [ebp+58h]
                          C2DVector tip = def.WhiteButtonPos + def.WhiteButtonTooltipOffset;   // [ebp+60h]; x = Pos.x+Tip.x, y = Tip.y+Pos.y (fld order)
                          C2DVector v; GFACoordToVCoord(GFAnchorToBottomRight(tip) /*[20h]*/, v /*[4Ch]*/);   // 643D0C, 9E21D0
0x006522A7                v.X -= font.GetVStringExtents(name).Width * 0.6f;   // 437210 -> [14h]; `fld [eax]; fmul 0.6; fsubr [4Ch]`
0x006522C2                CRGBColour col(255,255,255, ScaleAlpha(255));         // [68h..6Bh]: 6B = alpha, 6A/69/68 = bl
                          PlayerGui->Draw2DText(v, name, font, col, 0.6f, (EEngine2DLayer)0x43, true);   // 438010
                        }
0x00652302              // ~CWideString 99B510
      }}}}}
BLACK:
0x0065230A  pos = GFAnchorToBottomRight(def.BlackButtonPos);             // [20h] -> pos
            if (def.BlackButtonGraphic > 0) sprite(BlackButtonGraphic, pos, 255, 0x46) -> PrimitiveHandles[8]
0x006523A7  pos += def.BlackButtonIconOffset;
            if (hero->WeaponsUsable) {                                   // 0x006523BB
0x006523CB    CThing* ranged = weapons->PActiveRangedWeapon.GetPItem();  // +0x141 -> [ebp+68h]
              if (IS_THING_ALIVE(ranged)) { CTCInventoryItem* item; if (ranged->PeekTC(TCI_INVENTORY_ITEM, &item)) {   // [64h]
0x0065242E      long graphic = (carrying->GetThingInPrimarySlot() == ranged) ? def.AbilityButtonSheatheBowGraphic
                                                                             : item->GetEngineGraphic().BankIndex;   // [ebp+5Ch]
0x00652450      if (graphic > 0) {
                  sprite(graphic, pos, 255, 0x44) -> PrimitiveHandles[9]
0x006524D5        tooltip block as above with action 0x0E, BlackButtonPos + BlackButtonTooltipOffset (sum stored in [ebp+40h] = pos's slot),
                  font -> [ebp+5Ch], name [ebp+6Ch], v [4Ch], layer 0x43; ~CWideString 0x006525F7
            }}}}
0x006525FF  UpdateWeaponAbilities();                                     // 64B1AC
0x00652607  if (normal) {
              for (unsigned long i = 0 /*[3Ch]*/; i < def.AbilityButtons.size(); ++i) {    // entry test jbe 0x006528DA; byte offset [6Ch] += 0x5C
0x00652631      float alpha = PlayerGui->GetButtonAlpha(i, true, !PlayerGui->ShowSpecialAbilityButtons, false);   // 439AB0 -> [5Ch]
0x00652651      if (ringsGraphic == 0) {
                  CInputEvent ev;                                                    // [ebp-4Ch]
                  if (PInputManager->IsJoystickEventInQueue((EInputEventType)0x14, def.AbilityButtons[i].ControllerButton, &ev,
                                                            PlayerManager->GetMainPlayer().GetJoystickDeviceNumber()))   // 449970, 487510, 9F4C60
                    ringsGraphic = def.AbilityButtons[i].ButtonGraphic;           // +0x48
                }                                                                    // ~CInputEvent A03B30 at 0x006526A9
0x006526B1      long ability = PlayerGui->GetAbilityButton(i);                       // 438990 -> [30h]
                if (ability == def.ABXYRunGraphic) {                                  // +0x7C0
0x006526CA        CTCHeroSpecialMovement* sm = ?; if (creature->PeekTC(TCI_HERO_SPECIAL_MOVEMENT /*0xDF*/, &sm) && sm
                      && !NCreatureMode::IsAvailableForHeroRun(*creature))            // `cmp [eax+4],edi ; je` then 85F560
                    alpha *= 0.5f;
                }
0x00652728      const CAbilityButtonGuiDef& b = def.AbilityButtons[i];   // `lea eax,[eax+ecx+54h]` = &b.Offset -> [64h]
                C2DVector buttonPos = GFAnchorToBottomRight(def.ABXYRingsPos) /*[20h]*/ + b.Offset;   // [4Ch]; plus a copy [14h] (drawPos)
                C2DVector drawPos(buttonPos);   // x87: fst [4Ch]..; fld st(1); fstp [14h]; fst [18h]
0x0065275C      if (b.ControllerType == 0 && ability > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
0x0065279C        tooltip block: action 6; name [58h]; tip = buttonPos + def.InteractButtonTooltipOffset [34h];
                  GFACoordToVCoord(tip, v=[40h]); font [64h]; extents [-10h]; layer 0x49; ~CWideString 0x0065288D
                }
0x00652895      DrawButton(info, drawPos, alpha, i + 2, ability, false, (EEngine2DLayer)0x44);   // 645099
              }
0x006528DA    for (unsigned long j = 0 /*[30h]*/; j < def.AbilityButtons.size(); ++j) {   // offset [5Ch]
0x006528FA      const CAbilityButtonGuiDef& b = def.AbilityButtons[j];                       // [64h]
                if (b.ControllerType == 0) {
0x00652911        C2DVector p = GFAnchorToBottomRight(def.ABXYRingsPos) /*[-10h]*/ + b.Offset;   // kept in x87
                  sprite(def.InteractButtonBorderGraphic, p + def.InteractButtonBorderOffset /*[40h]*/, 255, layer 0x45)
                     -> InteractBorderPrimitiveHandle (this+0x34)     // uvs here are [4Ch] and [14h]
              }}
            }
0x006529CD  if ((PlayerGui->NormalToSpecial || PlayerGui->SpecialToNormal || PlayerGui->ShowSpecialAbilityButtons) && !normal) {
0x006529FC    float specialAlpha = 0.0f;                                                    // [68h]
              for (unsigned long k = 0 /*[6Ch]*/; k < def.SpecialAbilityButtons.size(); ++k) {   // offset [3Ch] += 0x5C
0x00652A21      specialAlpha = PlayerGui->GetButtonAlpha(k, true, PlayerGui->ShowSpecialAbilityButtons /*byte via [44h]*/, false);
0x00652A41      if (ringsGraphic == 0) { same CInputEvent block on def.SpecialAbilityButtons[k] }
0x00652AA1      long moveIndex = GetAssignableSpecialMoveIndexForButtonDef(def.SpecialAbilityButtons[k]);   // vslot3 -> [38h]
                long graphic = 0;                                                            // [30h]
                if (abilities) graphic = abilities->GetSpecialAbilityGraphicSetToButton(moveIndex);   // 5EE31D
0x00652ACF      float drawAlpha = specialAlpha;                                              // [5Ch]
                CDefPointer<const CHeroAbilityDef> abilityDef;   // [58h] = 0   (see DOSSIER open point)
                if (abilities) {
0x00652AE1        abilityDef = abilities->GetSpecialAbilityPDefSetToButton(moveIndex);   // 5EE365 hidden [64h]; addref; temp release
0x00652B16        if (abilityDef) {
                    if (abilities->IsAbilityAvailable(abilityDef->HeroAbility)) {        // 5EDF08
0x00652B35            bool ok;
                      if (abilities->IsAbleToUseAbility(abilityDef->HeroAbility, true)) {   // 5EE49A
0x00652B64              CTCSpecialAbilities* sa; ok = creature->PeekTC(TCI_SPECIAL_ABILITIES /*0xA7*/, &sa)   // no TC -> skip overlay
                                                       ? sa->IsAbilityAvailable(abilityDef->HeroAbility) : true;   // 72C430
                      } else
                        ok = abilityDef->HeroAbility == 4 && creature->HasTC(TCI_PHYSICAL_SHIELD_SPELL /*0x93*/);   // 0x00652B49
0x00652BBD            if (!ok) {
                        drawAlpha = specialAlpha * (1.0f/3.0f);                              // 0x0125A4DC
                        long idx = SpecialPrimitiveHandles.size() - k;                       // (end-begin)>>3 - k -> [50h]
                        C2DVector p = GFAnchorToBottomRight(def.ABXYRingsPos) /*[-10h]*/ + def.SpecialAbilityButtons[k].Offset;   // [14h]
                        DrawButton(info, p, drawAlpha, idx, def.ABXYAbilityUncastableGraphic, true, (EEngine2DLayer)0x43);
                      }
0x00652C2E            if (abilityDef->HeroAbility == 4 && creature->HasTC(TCI_PHYSICAL_SHIELD_SPELL)) graphic = def.ABXYAbilityShieldOffGraphic;
                    } else graphic = 0;                                                      // 0x00652C56
                }}
0x00652C59      C2DVector p = def.SpecialAbilityButtons[k].Offset + GFAnchorToBottomRight(def.ABXYRingsPos) /*[-18h]*/;   // [20h]; NOTE operand order offset first
                DrawButton(info, p, drawAlpha, k, graphic, true, (EEngine2DLayer)0x44);
0x00652CA7      if (abilityDef) DrawEffects(k, def.SpecialAbilityButtons[k].Position /*+0x4C*/, *abilityDef);   // 6451DD
                // ~CDefPointer inline: if (p && --p->count == 0) p->vslot1()
              }
0x00652CF9    if (abilities) {
                if (PlayerGui->ShowSpecialAbilityButtonsAbilities) {                       // +0x251
0x00652D14        if (abilities->GetNumAssignedReceivedAbilities() > 0) {                  // 5EE146
                    if (abilities->GetNumAssignedAbilityRows() > 1) {                      // 5EE183
0x00652D2E            C2DVector p = def.AbilityButtons[0].Offset + GFAnchorToBottomRight(def.ABXYRingsPos) /*[-18h]*/;   // [20h]
                      DrawButton(info, p, specialAlpha, 0, def.ABXYAbilitySwitchGraphic, true, (EEngine2DLayer)0x44);
                    }
0x00652D79          DrawAbilityList(info, specialAlpha);                                   // vslot4
                  }
0x00652D8D      } else if (abilities->GetNumAssignedReceivedExpressions() > 0)             // 5EE24B
                  DrawExpressionList(info, specialAlpha);                                  // vslot7
              }
            } else
0x00652DAA    ClearEffects(Effects);                                                       // 6445BA, ecx = this+0x48
0x00652DB5  unsigned char alpha = PlayerGui->GetAlpha();          // (unsigned char)(255.0f*Alpha) via __ftol2 -> [5Ch]
            if (PlayerGui->ShowSpecialAbilityButtons && alpha < 255) alpha = 255;
0x00652DE6  if (ringsGraphic == 0) ringsGraphic = def.ABXYButtonMiddleGraphic;             // shared `jle` on the result
            if (ringsGraphic > 0) {
0x00652DFC    // uvs [20h], [14h] zeroed first; esi = &def.ABXYMiddleOffset
              C2DVector p = def.ABXYMiddleOffset + GFAnchorToBottomRight(def.ABXYRingsPos) /*[-18h]*/;   // x87, copy to [4Ch]
              sprite(ringsGraphic, p, ScaleAlpha(alpha), layer 0x43) -> BackgroundPrimitiveHandle (this+0x2C)   // esi = this from [70h]
            }
0x00652E8D  if (!World->IsPaused() && (PlayerGui->NormalToSpecial || PlayerGui->SpecialToNormal)) {
0x00652EB1    PlayerGui->SetAbilityButtonScale(PlayerGui->GetAbilityButtonScale() + info.Interpolate.<first float> * 5.0f);   // 4375E0
0x00652ECF    if (PlayerGui->GetAbilityButtonScale() >= 1.0f) {                            // 4375E0 ; fcomp 1.0 ; test ah,1 ; jne skip
                PlayerGui->SetAbilityButtonScale(1.0f);                                    // fld1
                PlayerGui->NormalToSpecial = false; PlayerGui->SpecialToNormal = false;
            }}
0x00652F00  return
```
No cold blocks after the epilogue; no jump tables.
