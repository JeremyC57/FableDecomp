// Grade: DIFFER(-v5195) -- no compiling body yet: the whole function is decoded below as an annotated outline (every branch, callee, TC id, member offset); budget went to 0x00A1E5C0 first
// Residue: no source graded. Probe quantity: callee profile + full control-flow decode (this file). Inline-budget marker: the first ~20 HasTC tests are expanded (`test [this+0x20+4*(id/32)],1<<(id%32)`) and the last six are calls to std::bitset<274>::test 0x00410DE0 (ids 0x68 on the hitter, 0x42, 0x84, 0x77, 0x10, 0x7F) -- VC7.1's per-function inline budget running out, so the outline order must be kept exactly.
// Next: turn the outline into C++ on the landed CThingCreatureBase::OnHit 0x00667D30 model (FABLE_RETAIL_CTHING_GETTC_EXPANDED GetTC(id) with Components.Find = LowerBound 0x0040F020 non-const / 0x004365B0 const, noinline IS_THING_ALIVE<CThing*> 0x0040DCE0, CDefPointer refcount copies); no EH frame (no /EHsc) although CCharString temporaries are destroyed.
// CThingGameObject::OnHit(CHitParameters& hit) -- apply a hit to a game object: debug draw, hit location / material, perception, door/electrocute/fire reactions, damage, kill credit, hero stats, events, rumble, collision sound, hit effect
// Leaf: ?OnHit@CThingGameObject@@UAEXAAVCHitParameters@@@Z  (Ego_r.pdb + FableWin.pdb; vtable slot of CThingGameObject; CThingCreatureBase::OnHit 0x00667D30 calls it first)
// Retail: 0x00839940..0x0083AD8B (5195 bytes), thiscall `ret 4`, `sub esp,0A0h`, ebx/ebp/esi/edi saved, ebp = this, no EH frame.
//
// Offsets used (engine/CHitParameters.h): Localised +0, Directional +1, Position +2, Direction +0xE, PlayHitResponse +0x20,
// PlayHitResponseOverrideSet +0x21, Blocked +0x26, HitNegated +0x27, CauseRecoil +0x28, DamageMap +0x30, PHitLocation +0x48,
// PHitMaterialDef +0x4C, PThingHitting +0x50, PWeapon +0x58.  CThing: UsingComponents bitset<274> +0x20 (HasTC(id) =
// dword +0x20+4*(id/32) bit id%32), Components (CVectorMap) +0x44, thing-type byte +0x10, class flags +0x6C (bit 3 = creature,
// bit 2 = has material), +0x70 PBaseDef (CDefPointer), +0x91 bit 0 = dead (inline IS_ALIVE test), +0xB4 health (float),
// +0xBC flags (bit 0 = player-controlled?, bit 4 = invulnerable -> negate hit).
//
// OUTLINE (C++-like; `GetTC(t,id)` = the expanded HasTC-free Find, `HAS(t,id)` = the bitset test, L/C = non-const / const map):
//
// if (g_DebugFlags(0x013B8A1C)->ShowHits(+0x78) && hit.Localised) {
//     draw CEnginePrimitiveLine(..., hit.Position, Position+(0,0,?0x122EE58), colour(0xFF,0,0,0xFF), 0.05f(0x3D4CCCCD), 0,0,1,0,0,0) via
//         g_Engine(0x013B86A0)->+0x28->+0x2C vfunc +0x5C(name 0x013BAF44, line)            // 0x004C3090
//     if (hit.Directional) same with Position+Direction, colour(0,0xFF,0,0xFF), name 0x013BAF4C
// }
// RemoveTC(0x100);                                                                          // 0x004C9840
// CThing* weapon = hit.PWeapon.GetPItem();            [esp+2C]                              // 0x00A01B50 (+0x58)
// CThing* hitter = hit.PThingHitting.GetPItem();      [esp+1C], ebx                         // 0x00A01B50 (+0x50)
// CHitLocation* location = hit.PHitLocation;          [esp+20]
// if (!location) {
//     if (!HAS(this,0x7F)) return;                                                           // `jns` on dword +0x2C
//     location = GetTC(this,0x7F,L)->GetDefaultHitLocation();                                // 0x00814BE0
//     if (!location) return;
//     hit.SetHitLocation(location);                                                          // 0x00868590
// }
// hit.SetHitLocation(location);                                                              // 0x00868590 (second call, unconditional)
// CDefPointer<const CMaterialDef> material;  this->vfunc+0xE8(&material);   [esp+38]  (GetMaterialDef, slot 58)
// if ((Flags6C & 8) && location->Def(+8)->MaterialIndex(+0x64) > 0)
//     PeekDefinitionManager()->GetDef<CMaterialDef>(index, &material);                       // 0x004C7990, 0x0066C340
// hit.SetPHitMaterialDef(material);   (by value: refcount ++ in the argument slot)           // 0x00868640
// if (hitter && !hitter->dead && HAS(this,0x34)) GetTC(this,0x34,L)->OnHit(hitter);          // CTCPerceiveThing 0x00744CB0
// if (hit.IsPhysical()) {                                                                    // 0x00868860 (HasStateZeroOrOne label)
//     if (hitter && !hitter->dead) {
//         if (hitter->Flags6C & 8) {
//             hitter->MakeNoise(CCharString(0x01268A5C,-1));                                 // 0x0099EBF0, 0x00662110, ~ 0x0099EAE0
//             if (HAS(this,0xEE)) { GetTC(this,0xEE,L)->OnHit(hitter); hit.HitNegated = true; goto release_material_return; }  // 0x007E2600
//         }
//         if (!hitter->dead) {
//             bool doorTest;
//             if (HAS(this,0x26)) doorTest = true;
//             else if (HAS(this,0x0D)) {                       // const map 0x004365B0
//                 CCharString key(0x0123A16C,-1); CCharString name;  GetTC(this,0xD,C)->vfunc+0x58(&name);
//                 doorTest = name == key;   (pointer-equal / null / length / CBasicString<char>::Compare 0x00411570 expanded)
//             } else doorTest = false;
//             if (doorTest && HAS(hitter,0x68) && !GetTC(hitter,0x68,C)->IsAbleToBreakDownDoors())   // 0x006D6370
//                 hit.HitNegated = true;
//         }
//     }
//     if (weapon && !weapon->dead) {
//         if (hitter && !hitter->dead && HAS(weapon,0x48)) {
//             long type = GetTC(weapon,0x48,L)->GetExplosionType();                          // 0x005DA830
//             if (type > 0) {
//                 CThing* explosion = CTCDExplosion::Create(type, hitter->GetPos());          // __fastcall 0x0080C460, GetPos 0x004C73D0
//                 if (IS_THING_ALIVE(explosion)) { CTCDExplosion* d; explosion->GetTCDriver(&d); d->SetSpecificThingToHit(this); }  // 0x007512C0, 0x0080CCF0
//             }
//         }
//         if (!weapon->dead && HAS(this,0x6D) && HAS(weapon,0x6D)) {        // CTCFlammable, both GetTC L
//             if (mine->IsOnFire() && !theirs->IsOnFire()) theirs->SetOnFire();             // 0x00780400 / 0x00780410
//             else if (!mine->IsOnFire() && theirs->IsOnFire()) mine->SetOnFire();
//         }
//     }
// }
// if (HAS(this,0xA8) || HAS(this,0xA9)) { hit.SetPlayHitResponse(false); goto release_material_return; }   // `test ah,3` on +0x34
// if (HAS(this,0x84)) GetTC(this,0x84,L)->IncreaseBerserkDamageToMeMultiplier();             // 0x0078BEC0
// CTCCombat* combat = 0;                              [esp+3C]
// if (HAS(this,0x30)) { combat = GetTC(this,0x30,L); combat->SetStruck(location, hit); }    // 0x00734C60
// float damage = 0;                                   [esp+28]
// if (hit.Blocked) { if (HAS(this,0x68)) GetTC(this,0x68,L)->AddSuccessfulBlockParry(); goto hero_stats; }   // 0x006D7550
// if (FlagsBC & 0x10) {
//     hit.HitNegated = true;
//     if ((Flags6C & 8) && location->Def->+0x68 > 0) { CDefPointer<const CMaterialDef> m; PeekDefinitionManager()->GetDef<CMaterialDef>(idx,&m); hit.SetPHitMaterialDef(m); }
// }
// if (combat && combat->IsBlocking()) combat->UnsetBlocking();                              // 0x007331E0 (byte +0x63), 0x007338A0
// if (HAS(this,0x45)) GetTC(this,0x45,L)->SetEmoteIcon(8, 0, 0, 1.0f, 0xFF);                 // 0x0071AAB0
// if (!location->CanBeHitByMeleeStrike()) hit.SetPlayHitResponse(false);                    // 0x00815270
// CDefPointer<...> locationDef = location->Def(+8);   [esp+10] (refcount ++)
// hit.SetAsModifiedByArmour(this, locationDef);                                              // 0x00869020
// if (hit.HitNegated) { hit.SetPlayHitResponse(false); hit.CauseRecoil = true; }
// damage = hit.GetDamageToThing(*this);                                                      // 0x00868690 (fstp)
// if (!hit.HitNegated) {
//     if (hit.IsPhysical() && ((Flags6C & 8) || (FlagsBC & 1)) && IS_THING_ALIVE(weapon) && weapon != hitter
//         && IS_THING_ALIVE(hitter) && HAS(hitter,0x87) && !GetTC(hitter,0x87,L)->+0x14)
//         GetTC(hitter,0x87,L)->StartMultiHit(hit, this);                                    // 0x007CCB00
//     bool playResponse = true;                       [esp+14]
//     if (ThingType(+0x10) == 2 && IS_THING_ALIVE(hitter) && HAS(hitter,0xB4)) { if (GetTC(hitter,0xB4,L)->IsAttackingToFineRatherThanKill()) playResponse = false; }  // 0x007B2CF0
//     else if ((Flags6C & 8) && IS_THING_ALIVE(hitter) && HAS(hitter,0x29) && !GetTC(hitter,0x29,C)->+0xC) playResponse = false;
//     if (HAS(this,0x26) && HAS(this,0x33)) {
//         CThing* owner = GetTC(this,0x33,L)->+0xD.GetPItem();                              // 0x00A01B50
//         if (owner && HAS(owner,0x23)) { CTCVillage* v = GetTC(owner,0x23,L)->GetPVillage(); if (v && v->+0xB3) playResponse = false; }  // 0x007EE540
//     }
//     bool deadBefore = Health(+0xB4) < DELTA_NOTIONAL_ZERO;
//     if (!HAS(this,0x110) || --GetTC(this,0x110,L)->+0xC < 0) this->vfunc+0xEC(damage, playResponse);   // ApplyHitDamage
//     if (!deadBefore && Health < DELTA_NOTIONAL_ZERO) {
//         CThing* killer = weapon;
//         if (IS_THING_ALIVE(killer) && HAS(killer,0x77)) killer = GetTC(killer,0x77,C)->GetPWeapon();   // 0x007BEFF0
//         if (IS_THING_ALIVE(killer) && HAS(killer,0x48)) GetTC(killer,0x48,L)->IncrementNumThingsKilled(this);  // 0x005DA680
//     }
//     if ((Flags6C & 8) && IsInMeleeAsAttacker()) GetCombatManager()->DeregisterCombatant(this);          // 0x00662410, 0x00661ED0, 0x006EA430
//     if ((Flags6C & 8) || HAS(this,0x3D)) {
//         float change = -damage;
//         if ((FlagsBC & 1) && change > DELTA_NOTIONAL_ZERO) {
//             if (GetPlayerManager()->IsThingMainPlayer(*this)) g_PlayerGui(0x013B8790)->AddDamageChange(change, true);   // 0x004C7A00, 0x004497C0, 0x00437940
//             else if (IS_THING_ALIVE(hitter) && GetPlayerManager()->IsThingMainPlayer(*hitter)) g_PlayerGui->AddDamageChange(change, false);
//         }
//     }
//     if (damage < 0.0f) {
//         if (HAS(this,0x68)) GetTC(this,0x68,L)->SetAsHit();                                  // 0x006D80E0
//         if (hit.Localised) {
//             if (HAS(this,5)) GetTC(this,5,L)->AddPossibleWoundGivenStrikePoint(hit.Position);   // 0x00757F90
//             if (hit.Directional && HAS(this,0x67)) GetTC(this,0x67,L)->vfunc+0x78(hit.Position, hit.Direction);
//         }
//     }
// } else if (hit.IsPhysical() && IS_THING_ALIVE(hitter) && !(Flags6C & 8) && HAS(hitter,0x51))
//     GetTC(hitter,0x51,L)->PostDeed(0x49, GetPos(), this, 0, 2, 0, 0, 0, 0, 0);             // 0x006C6990
// if (hitter && !hitter->dead && combat) combat->OnHit(hitter);                              // 0x007332D0
// location->ProcessHitLocationCode(this, hit);                                               // 0x00815E80
// locationDef released (dec + vfunc+4 when 0)
// hero_stats:
// if (!hit.HitNegated && !hit.Blocked && hitter && !hitter->dead && HAS(hitter,4)) {
//     CTCHeroStats* stats = GetTC(hitter,4,L);         [esp+30]
//     if (IsGivingHeroStatChangesOnBeingHit()) {                                              // 0x00838B60
//         CDefPointer<const CCreatureDef> creatureDef; if (CDefPointer copy of PBaseDef(+0x70))->GetSubDef(&creatureDef)) {   // 0x00485815
//             bool direct = !(IS_THING_ALIVE(weapon) && weapon != hitter);
//             CDefPointer<const CHeroExperienceDef> heroDef; if (hitter->PBaseDef copy->GetSubDef(&heroDef)) {                  // 0x006DAA90
//                 for (damage map it over hit.DamageMap; ++it = 0x006DA040) {
//                     outer = heroDef->+0x9C.find(it->first);                                // 0x0083B120
//                     if (outer != end) for (inner over outer->second; ++ = 0x006DA0B0) {
//                         NHeroStatModifierEvent::CHitThing event(creatureDef->+0x40); event.Direct = direct; event.Stat = inner->first; event.Amount = inner->second;   // 0x0083AE30
//                         bool ghost = HAS(this,0xA0) && GetTC(this,0xA0,L)->+0x10;
//                         if (!ghost && !IsThingGhostSword(this)) stats->InformOfEvent(&event);   // 0x007C53F0 fastcall, 0x0057A56C
//                     }   // ~CHitThing inline: vptr 0x0127232C
//                 }
//             }
//         }
//     }
// }
// if (!hit.HitNegated && !hit.Blocked && IS_THING_ALIVE(hitter) && hitter->UsingComponents.test(0x68)    <- CALL 0x00410DE0 from here on
//     && IsAddToComboMultiplierOnHit()) GetTC(hitter,0x68,L)->AddSuccessfulStrikeWithHitParams(hit);   // 0x00838B20, 0x006D8D00
// if (UsingComponents.test(0x42)) { CTCEntityEvents* events = GetTC(this,0x42,L);
//     if (hitter && !hitter->dead) { events->AddEvent(new CEventHitBy(hit));                 // operator new(0x10) 0x00BFEA1A, 0x0083AF30, 0x008113C0
//         if (hitter->UsingComponents.test(0x84)) events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 8)); } }  // new(0x18), 0x00811830
// if (hit.CauseRecoil && hitter && !hitter->dead && (hitter->Flags6C & 8)
//     && !(IS_THING_ALIVE(weapon) && weapon->UsingComponents.test(0x77))) hitter->PerformBlockedReaction(0, 1);   // 0x00661EF0
// if (hit.IsPhysical()) { const CMaterialDef* m = hit.PHitMaterialDef (+0x4C, refcount ++);
//     if (m && m->Rumble(+0x44) != 0 && IS_THING_ALIVE(hitter) && hitter->ThingType == 2
//         && hitter->PeekPlayer().+0x28 == PeekPlayerManager()->GetMainPlayer()->+0x28) {   // 0x004C7A10, 0x004C79F0, 0x00449960
//         long dev = hitter->GetJoystickDeviceNumber();                                         // 0x004C7CA0
//         g_Input(0x013B8388)->+0x2C[dev*8]->vfunc+0x44(3, CRecord(m->+0x44, 0.5f, 0, m->+0x48, 0, 1));   // 0x006A92B0
//     } (refcount --) }
// if (UsingComponents.test(0x10)) { CTCSoundPlayer* sound = GetTC(this,0x10,L);
//     if (hit.Localised) { CDefPointer<const CMaterialDef> other;    [esp+20]
//         if (IS_THING_ALIVE(weapon) && (weapon->Flags6C & 4)) other = weapon->vfunc+0xE8(...);
//         if (!other && IS_THING_ALIVE(hitter) && (hitter->Flags6C & 4)) { other = hitter->vfunc+0xE8(...);
//             if (hit.IsLightningStrike()) PeekDefinitionManager()->GetDef(CCharString(0x01272FFC), &other);           // 0x008689B0, 0x0083B000
//             else if (hit.IsDivineWrathStrike()) ...(CCharString(0x01272FE4));  else if (hit.IsUnholyPowerStrike()) ...(0x01272FCC);  // 0x008689E0, 0x00868A10
//             else if ((hitter->Flags6C & 8) && hitter->UsingComponents.test(0x7F))
//                 PeekDefinitionManager()->GetDef<CMaterialDef>(GetTC(hitter,0x7F,L)->GetDefaultHitLocation()->Def->+0x64, &other); }
//         if (CDefPointer<const CMaterialDef> mine = hit.GetPHitMaterialDef())                                          // 0x0083AF60 (out of line)
//             sound->PlayCollisionSound(hit.Position, mine, other); } }                                                // 0x0066A9E0
// NThingHelpers::CreateEffectOnHit(this, hit, PeekWorldMap(), damage);       // __fastcall 0x00839420, 0x004C79E0
// release_material_return: material released; return.
