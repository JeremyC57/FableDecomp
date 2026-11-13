# OUTLINE 0x00652FA4 CDrawQuickAccessIcons::Draw(const CEnginePrimitivePrepareInfo& info)

## Frame
`push ebp ; lea ebp,[esp-74h] ; sub esp,12Ch ; (push ebx ; push esi ; push edi interleaved)`; cookie [ebp+70h]; arg info [ebp+7Ch];
esi = this, edi = def (PeekGuiDef), ebx = 0xFF for most of the body (first &PrimitiveHandles, then hero, then 0xFF from 0x0065311A).
No EH states. Exit 0x006541BF: cookie check, pop edi/esi/ebx, `add ebp,74h ; leave ; ret 4`.

| slot | local |
|---|---|
| [ebp-0B8h..-65h] | CEnginePrimitive2DSprite sprite (0x54), rebuilt 5x, no dtor |
| [ebp-64h],[ebp-3Ch] | C2DVector(0,0) uv temps of the cross sprite; [ebp-3Ch] also GFAnchorToBottomLeft result in the flash/dpad blocks |
| [ebp-5Ch],[ebp-54h] | C2DCoordI temps (loop 2 seal / lamp effect) |
| [ebp-4Ch] | C2DVector pos+GuildSealLevelUpOffset (loop 2) |
| [ebp-44h] | C2DVector DPadRingsPos+button.Offset (loop 2) |
| [ebp-34h..-1Ch] | CActionInputControl control (7 dwords zeroed: 5 ints, 2 floats at -20h,-1Ch) |
| [ebp-18h],[ebp-10h] | C2DCoordI temps (loop 1 seal / lamp) ; [ebp-14h] loop2 `slot`; [ebp-0Ch] loop2 `graphic`; [ebp-8],[ebp-4] sprite pos temp |
| [ebp+0] | &PrimitiveHandles (CSE pointer, deref'd for [0],[1],[6],[7]) |
| [ebp+4..+0Bh] | C2DVector temps (tooltip VCoord, zero uv, -2 charge pos) |
| [ebp+0Ch] | loop2 float fade |
| [ebp+10h] | loop1 `const C2DVector& offset` (=&button.Offset); loop2 IV `1 - &effectUsed[0]` |
| [ebp+14h] | `&def.DPadRingsPos` (kept) |
| [ebp+18h] | CTCInventory* inventory (GetTC out; uninitialised if no TC) |
| [ebp+1Ch] | long dpadGraphic = 0 |
| [ebp+20h],[ebp+2Ch] | C2DVector(0,0) uv temps (ring sprites); [ebp+20h] also pos+GuildSealLevelUpOffset (loop1); [ebp+2Ch/30h] tooltip pos loop1 |
| [ebp+24h] | loop2 `const C2DVector& offset` |
| [ebp+28h] | CWideString name (loop1) / CDefPointer<CThingBaseDef> (loop2) |
| [ebp+30h] | CWideString temp (GFIntToWideString) ; flash alpha byte |
| [ebp+34h] | CCharString temp ; loop1 float fade ; loop2 byte offset i*0x50 ; font ptr |
| [ebp+38h] | GFAnchor result / C2DCoordI / CIntelligentPointer temp |
| [ebp+40h] | C2DVector(pos) copy for ring sprites |
| [ebp+44h] | key temp for LowerBound / InputInterface / &button / abilities / font |
| [ebp+48h] | CThingPlayerCreature* hero |
| [ebp+4Ch] | C2DVector pos |
| [ebp+54h] | long alpha / CRGBColour colour (loop1) / CDefPointer<CThingObjectDef> (loop2) |
| [ebp+58h] | guildSealIndex / CWideString name / loop1 graphic / flash graphic / CRGBColour (loop2 tooltip) |
| [ebp+5Ch] | long i (both loops) |
| [ebp+60h] | topGraphic ; CDefPointer<CThingObjectDef> (loop1) ; itemDef / CDefPointer<CAbilityDef> (loop2) ; effect index e |
| [ebp+64h..6Fh] | bool effectUsed[12] (12 single-byte zero stores at 0x006534E8) |

## Body (retail order)
```
0x00652FA4  C2DVector pos(0.0f, 0.0f);                          // [ebp+4C] (fldz x2 interleaved with the cookie)
0x00652FC6  PlayerGui->SetGuildSealLevelUpPos(pos);             // 0x0043A4D0
0x00652FD2  CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();  // 0x449970, 0x487DC0
0x00652FE7  if (!IS_THING_ALIVE(hero)) return;                  // 0x43DBD0, je 0x006541BF
0x00652FF4  Drawing = true;                                     // SetAsDrawing(true)
0x00652FF8  const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();// edi
0x00652FFD  PrimitiveHandles.clear(); SpecialPrimitiveHandles.clear(); QuickAccessSpellCrossHandles.clear(); // erase(begin,end) 0x440D70 x3
0x0065302B  PrimitiveHandles.resize(def.ContextSensitiveItemButtons.size() + 5);   // 0x443F90
0x00653048  SpecialPrimitiveHandles.resize(def.QuickAccessItemButtons.size());
0x00653063  QuickAccessSpellCrossHandles.resize(def.QuickAccessItemButtons.size());
0x0065307E  ChargeUpPrimitiveHandle.Clear();                    // inline: refcount==1 -> Data->slot3(this); release (slot1); two zero stores
0x006530B6  CTCInventory* inventory; hero->GetTC(TCI_INVENTORY, &inventory);  // bitset 0x410DE0(0x11), LowerBound 0x40F020
0x006530F0  inventory->UpdateContextSensitiveItemFades(info);   // 0x581E6F
0x006530FB  long dpadGraphic = 0;
0x006530FF  pos = GFAnchorToBottomLeft(def.DPadRingsPos);       // edx=&def+174h kept in [ebp+14]
0x00653110  if (!PlayerGui->NormalToSpecial && !PlayerGui->SpecialToNormal && PlayerGui->ShowSpecialAbilityButtons) {
0x00653133     if (def.DPadRingsFavouriteGraphic > 0) goto ring(def.DPadRingsFavouriteGraphic);   // else -> 0x0065322A
            } else if (def.DPadRingsGraphic > 0) {             // 0x00653198
               ring(g):  // both arms duplicated up to 0x006531FB, merged there
               CEnginePrimitive2DSprite s(0, C2DVector(pos.X,pos.Y)[+40], (EEngine2DLayer)0x46, g, 0, (blend)2, (interp)0, 0.0f, 0.0f,
                    ScaleAlpha((unsigned char)GLOBAL_255 /*0x01375744*/), 0xFF,0xFF,0xFF, 0, C2DVector(0,0)[+20], C2DVector(0,0)[+2C]); // 0x493AC2
0x0065320E     info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], s, 0xC0);
            }
0x0065322A  pos += def.DPadRingsTopOffset;                      // X then Y (fld pos; fadd off)
0x0065323E  long topGraphic;                                    // [ebp+60]
            if (same 3-flag test) topGraphic = def.DPadRingsTopFavouriteGraphic;          // 0x0065325F
            else { topGraphic = def.DPadRingsTopGraphic;                                   // 0x00653267
                   if (!World->GuildSealRecallPos.IsZero /*|v|^2 > eps^2*/) topGraphic = def.GuildSealRecallGraphic; } // 0x006532AE
0x006532B7  long alpha = GLOBAL_255;                            // dword read
0x006532BC  long sealIndex = World->DefinitionManager->GetDefGlobalIndexFromName(CCharString("OBJECT_GUILD_SEAL_1"));  // (str,-1); temp dtor 0x99EAE0
0x006532E8  if (sealIndex > 0 && inventory->GetNumberOfItemsOfTypeInInventory(sealIndex) <= 0) alpha /= 2;   // signed: cdq/sub/sar
0x00653308  if (topGraphic > 0) {
0x00653312     if (PlayerManager->GetMainPlayer().<bool +0x217>) { topGraphic = def.DPadRingsTopAggressiveGraphic; alpha = GLOBAL_255; }
0x00653334     sprite(pos copy[+40], 0x46, topGraphic, ..., ScaleAlpha((unsigned char)(PlayerGui->GetGuildSealAlpha() * alpha)) /*fimul, ftol*/ , ...)
0x006533A3     info.Engine->SetPersistentPrimitive(PrimitiveHandles[1], s, 0xC0);
0x006533C3     if (CUserProfileManager::Get().IsHUDTooltipEnabled()          // inline: UseNewProfileValues ? [+D9] : [+19]
0x006533E1         && inventory->GetNumberOfItemsOfTypeInInventory(sealIndex) > 0) {
0x006533F4        CActionInputControl control;
                  if (Get().GetAssignedInputForAction((EGameAction)0x4E, true, control) ||        // 0x408C90
                      Get().GetAssignedInputForAction((EGameAction)0x4E, false, control)) {
0x00653443           CWideString name;                                   // [ebp+58]
                     if (CGamePlayerInterface::GetActionName(control, name, false)) {   // 0x446720 fastcall
0x0065345C              pos = def.DPadRingsPos + def.DPadGuildSealTooltipOffset;  // X: ringsPos.X+off.X ; Y: off.Y+ringsPos.Y
0x0065347B              C2DVector vpos; GFACoordToVCoord(GFAnchorToBottomLeft(pos)[+38], vpos[+4]);
0x0065348B              PlayerGui->Draw2DText(vpos, name, *Component->font(+0x1614C), CRGBColour(0xFF,0xFF,0xFF, ScaleAlpha(0xFF))[+54],
                                              0.6f, (EEngine2DLayer)0x49, true);   // 0x438010; colour bytes 56,55,54 stored after the alpha byte 57
                     }
0x006534CA        }  // ~CWideString 0x99B510
               }
            }
0x006534D2  bool effectUsed[12] = {};   // after `i = 0`; 12 byte stores
            for (long i = 0; i < def.ContextSensitiveItemButtons.size(); ++i) {   // jbe/jb unsigned, size via idiv 50h
0x00653520     PlayerGui->GetButtonAlpha(i, false, !PlayerGui->ShowSpecialAbilityButtons, true);   // result popped
0x0065353C     float fade = 1.0f;                                                 // [ebp+34]
               if (dpadGraphic == 0) {
0x00653547        CGamePlayerInterface* input = PlayerGui->InputInterface;          // read BEFORE GetMainPlayer
                  if (input->IsQuickAccessEventInQueue(i, (EInputButtonEventType)2, PlayerManager->GetMainPlayer().GetJoystickDeviceNumber()))
0x00653571            dpadGraphic = def.ContextSensitiveItemButtons[i].ButtonGraphic;   // `*(begin()+i)` (lea*5, shl 4, [eax+ecx+3C])
               }
0x00653587     const CQuickAccessItemButtonGuiDef& button = def.ContextSensitiveItemButtons[i];   // [ebp+44] (add form)
0x0065359D     long graphic = inventory->GetContextSensitiveItemGraphicIndex(i);   // [ebp+58]
0x006535A2     const C2DVector& offset = button.Offset;                            // [ebp+10]
0x006535B4     pos = GFAnchorToBottomLeft(def.DPadRingsPos) + offset;             // tmp [ebp+38]
0x006535D2     long item = inventory->GetQuickAccessItem(i);
0x006535D7     CDefPointer<const CThingObjectDef> objDef;                          // [ebp+60] = 0
               if (item > 0 && World->DefinitionManager->GetDef(item, &objDef)) {  // 0x42B040
0x006535FB        if (objDef->Property & 0x20) {
0x0065360B           fade = PlayerGui->GetGuildSealAlpha();
0x00653616           PlayerGui->SetGuildSealLevelUpPos(pos + def.GuildSealLevelUpOffset);    // tmp [ebp+20]
0x00653639           if (!World->GuildSealRecallPos.IsZero) graphic = def.DPadGuildSealRecallGraphic;
0x00653680           if (ChargingButtonIndex != -1) {
                        DrawEffectAtPos(C2DCoordI((long)pos.X,(long)pos.Y)[-18], def.QuickAccessGuildSealEffect, i + 9);
0x006536B4              effectUsed[i + 9] = true; }
                  }
0x006536BC        CTCLight* light;
                  if ((objDef->Property & 1) && hero->PeekTC(TCI_LIGHT /*0x27*/, &light) && light->IsActive() /*+0x1C*/) {  // 0x4365B0
0x0065370C           DrawEffectAtPos(C2DCoordI(pos)[-10], def.QuickAccessLampEffect, i + 9);
                     effectUsed[i + 9] = true; }
               }
0x00653742     DrawIcon(info, pos, fade, i + 2, 0, graphic, false);                 // 0x645292
0x00653765     if (ChargingButtonIndex == i) DrawChargeGraphic(info, offset, i);   // 0x64D7A6
0x0065377B     if (graphic > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
0x006537A3        CActionInputControl control;
                  if (Get().GetAssignedInputForContextSensitiveItemAction((EGameAction)0x38, true, i, control) ||   // 0x408F50
                      Get().GetAssignedInputForContextSensitiveItemAction((EGameAction)0x38, false, i, control)) {
0x006537F8           CWideString name;                                                // [ebp+28]
                     if (CGamePlayerInterface::GetActionName(control, name, false)) {
0x00653815              font = Component font word +0x1614C;                           // [ebp+34]
                        C2DVector tp(GFAXToVX(pos.X), GFAYToVY(pos.Y - 12.0f));        // Y evaluated first ([ebp+44]->[+30]); X [+2C]
0x00653842              if (i == 3) { tp.X += GFAXToVX(32.0f); tp.Y += GFAXToVX(16.0f); }  // NB both via GFAXToVX 0x9E1DA0
0x0065387B              PlayerGui->Draw2DText(tp, name, *font, CRGBColour(0xFF,0xFF,0xFF,ScaleAlpha(0xFF))[+54], 0.6f, (EEngine2DLayer)0x43, true);
                     }
0x006538B4        }  // ~CWideString
               }
0x006538BC     // ~CDefPointer objDef (inline: dec [p+4]; vtbl slot 1)
            }
0x006538ED  for (long i = 0; i < def.QuickAccessItemButtons.size(); ++i) {    // IVs: [ebp+34] = i*0x50, [ebp+10] = 1-&effectUsed[0]
0x0065391C     PlayerGui->GetButtonAlpha(i, false, PlayerGui->ShowSpecialAbilityButtons, false);   // movzx byte; result popped
0x00653936     float fade = 1.0f;                                             // [ebp+0C]
               long graphic = inventory->GetQuickAccessItemGraphicIndex(i);  // 0x581FA4 -> [ebp-0C]
0x00653949     long slot = def.ContextSensitiveItemButtons.size() + i;       // [ebp-14]
0x00653969     const C2DVector& offset = def.QuickAccessItemButtons[i].Offset;  // begin + i*50h + 48h -> [ebp+24]
               pos = GFAnchorToBottomLeft(def.DPadRingsPos + offset);        // sum first into [ebp-44] (X: rings.X+off.X; Y: off.Y+rings.Y)
0x0065398E     long item = inventory->GetQuickAccessItem(slot);              // [ebp+60]
0x00653999     CDefPointer<const CThingObjectDef> objDef;                     // [ebp+54]
               if (item > 0) {
                 if (World->DefinitionManager->GetDef(item, &objDef)) {
                   [same seal block as loop 1, temps [ebp-4C] / [ebp-5C]; DrawEffectAtPos(..., def.QuickAccessGuildSealEffect, i); effectUsed[i]=true]
                   [same lamp block, temp [ebp-54]; DrawEffectAtPos(..., def.QuickAccessLampEffect, i); effectUsed[i]=true]
                 }
0x00653B01       CDefPointer<const CThingBaseDef> baseDef;                    // [ebp+28]
                 if (World->DefinitionManager->GetDef(item, &baseDef)) {     // 0x42AF3C
0x00653B1F         CDefPointer<const CAbilityDef> abilityDef;                // [ebp+60]
                   if (baseDef->GetSubDef(&abilityDef)) {                    // 0x5936C2 (ecx = baseDef object)
0x00653B37           CTCInventoryAbilities* abilities;                       // [ebp+44]; PeekTC(0x6F) THEN `test eax,eax ; je` (extra null test:
                     if (hero has TCI_HERO_ABILITIES && abilities != 0) {    //  a pointer-returning accessor / `&& abilities`)
0x00653B84             if (abilities->IsAbilityAvailable(abilityDef->Ability)) {        // 0x5EDF08
                         bool skipCross;                                     // shared `test al,al ; jne 0x653CD4` at 0x00653C1E
0x00653B9A               if (abilities->IsAbleToUseAbility(abilityDef->Ability, true)) {  // 0x5EE49A
0x00653BC9                 if (!hero->HasTC(TCI_SPECIAL_ABILITIES)) goto noCross;
                           CTCSpecialAbilities* sp = PeekTC(0xA7);  skipCross = sp->IsAbilityAvailable(abilityDef->Ability);  // 0x72C430
                         } else if (abilityDef->Ability == 4) skipCross = hero->HasTC(TCI_PHYSICAL_SHIELD_SPELL /*0x93*/);   // 0x00653BAE
                         else skipCross = false;
0x00653C26               if (!skipCross) {
                           CEnginePrimitive2DSprite c(0, pos + def.QuickAccessAbilityCrossOffset [-8], (EEngine2DLayer)0x43,
                               def.ABXYAbilityUncastableGraphic, 0, 2, 0, 0.0f, 0.0f, ScaleAlpha((unsigned char)(fade * 255.0f)),
                               0xFF,0xFF,0xFF, 0, C2DVector(0,0)[-3C], C2DVector(0,0)[-64]);   // uv temps zeroed FIRST
0x00653CA1                 info.Engine->SetPersistentPrimitive(QuickAccessSpellCrossHandles[i], c, 0xC0);   // [esi+70h]+i*8
0x00653CC8                 fade *= 1.0f/3;                       // 0x0125A4DC
                         }
0x00653CD4  noCross:     if (abilityDef->Ability == 4 && hero->HasTC(TCI_PHYSICAL_SHIELD_SPELL)) graphic = def.ABXYAbilityShieldOffGraphic;
                       } else graphic = 0;                                     // 0x00653CFC
                     }
0x00653D00         } // ~abilityDef
0x00653D15       } // ~baseDef
               }
0x00653D28     DrawIcon(info, pos, fade, i, 0, graphic, true);
0x00653D47     if (ChargingButtonIndex == slot) DrawChargeGraphic(info, offset, slot);
               else if (ChargingButtonIndex == -2) DrawChargeGraphic(info, C2DVector(0,0)[+4], -2);   // tail merged at 0x00653D6B
0x00653D75     if (graphic > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
0x00653D9D        font = Component font word;                                  // [ebp+44]
                  CRGBColour colour(0xFF,0xFF,0xFF, ScaleAlpha(0xFF));         // [ebp+58..5B]: 5A,59,58 then 5B
                  C2DVector tp(GFAXToVX(pos.X + K.X + 12.0f), GFAYToVY(pos.Y + K.Y + 32.0f));   // K = static C2DVector @0x01375404; Y first
0x00653E11        PlayerGui->Draw2DText(tp, GFIntToWideString((i + 1) % 10) /*temp [ebp+30], idiv 10*/, *font, colour, 0.6f, (EEngine2DLayer)0x43, true);
0x00653E35        // ~CWideString temp
               }
0x00653E3D     // ~objDef (falls here from the item<=0 path too: 0x006539A2 jle 0x00653D28 skips only the def blocks)
            }
0x00653E72  for (unsigned long e = 0; e < Effects.size(); ++e) {      // size via sar 3; guard `je` once; top test `e >= 12 -> exit`
              if (e >= 12) break;                                     // (or `e < size && e < 12`: check which rotates to this shape)
0x00653E8A    if (e != 9 && !effectUsed[e] && Effects[e].GetPItem()) {  // 0xA01B50
                Effects[e].GetPItem()->Kill();                        // 0xAEE8C0
                Effects[e] = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);  // 0x655318 / 0x65533C / 0xA01C10, temp [ebp+38]
              }
            }
0x00653EFF  if (ChargingButtonIndex != -1) {
              const C2DVector& a = GFAnchorToBottomLeft(def.DPadRingsPos + def.GuildSealMainLevelUpOffset);  // sum [ebp+38], result [ebp+4], ptr [ebp+30]
0x00653F29    DrawEffectAtPos(C2DCoordI((long)a.X, (long)a.Y)[+38], def.QuickAccessMainGuildSealEffect, 9);
            } else if (Effects[9].GetPItem()) {                        // 0x00653F59 ([esi+7Ch]+48h)
              Effects[9].GetPItem()->Kill(); Effects[9] = CIntelligentPointer<CParticleEmitter>(0);
            }
0x00653F9B  if (dpadGraphic == 0) dpadGraphic = def.QuickAccessDPadGraphic;
0x00653FAA  if (FlashButton != 0) {
              dpadGraphic = def.QuickAccessDPadGraphic;               // scheduled before the jne
0x00653FB4    if (!FlashFade.Active) FlashFade.Open(0.5f, 0.5f, CRGBColour(0,0,0,0xFF));   // by-value colour built in the arg slot
0x00653FF1    FlashFade.Update(*(float*)&info /* info +0 float */);  // check UpdateContextSensitiveItemFades 0x581E6F for the member name
0x00654002    if (FlashFade.IsOpen()) { FlashFade.Opening = false; FlashFade.Closing = true; }   // likely inline Close()
0x00654016    long g;                                                 // [ebp+58]; sub 0Dh / dec / dec / dec chain, no jump table
              switch (FlashButton) { case 13: g = def.QuickAccessItemButtons[0].ButtonGraphic; break;  // 0x00654055
                                     case 14: g = ...[1].ButtonGraphic; break;  case 15: ...[2]; case 16: ...[3];   // +8C/+DC/+12C
                                     default: goto dpad; }
0x0065405E    if (g > 0) {
0x00654069      unsigned char a = (unsigned char)(FlashFade.GetFadeInOutAlpha() * 255.0f);    // [ebp+30]
                // uv temps [+38],[+4] zeroed; anchor = GFAnchorToBottomLeft(def.DPadRingsPos) -> [ebp-3C]
                CEnginePrimitive2DSprite f(0, C2DVector(def.DPadMiddleOffset.X + anchor.X, anchor.Y + def.DPadMiddleOffset.Y)[-8],
                    (EEngine2DLayer)0x43, g, 0, 2, 0, 0.0f, 0.0f, ScaleAlpha(a), 0xFF,0xFF,0xFF, 0, uv[+4], uv[+38]);
0x006540FC      info.Engine->SetPersistentPrimitive(PrimitiveHandles[7], f, 0xC0);
              }
            }
0x0065411C  dpad: if (dpadGraphic > 0) {
              // same position expression (edi += 1C8h), layer 0x44, graphic dpadGraphic, ScaleAlpha(0xFF), zeros via `xor edi,edi`
0x0065419F    info.Engine->SetPersistentPrimitive(PrimitiveHandles[6], s, 0xC0);
            }
0x006541BF  return;   // cookie check
```
No cold blocks after the epilogue; no jump tables.
