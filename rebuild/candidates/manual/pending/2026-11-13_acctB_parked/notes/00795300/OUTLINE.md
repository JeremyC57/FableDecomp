# OUTLINE CTCUnholyPower::FrameUpdate (retail 0x00795300..0x00797733); twin CTCDivineWrath::FrameUpdate 0x0078FD60 (same text, 11 constants)

## Frame
- `push ebp; mov ebp,esp; and esp,-8; sub esp,230h; push ebx,ebp,esi,edi`; ebp=this (also [esp+38h]); ebx=owner (CThingCreatureBase*)GetParentThing() kept in [esp+6Ch].
- No EH states, no /GS. `ret` (0 bytes). 4 duplicated epilogues.
- Frame slots (esp after the 4 pushes; reused heavily -- treat as a guide):
  13h bool tmp (GetEffectFromString result / explode flag / dummy CThingFilter_CanBeHitByEpicSpell + allocator byte);
  14h..1Bh CIntelligentPointer<CThingCreatureBase> `cr` (map loops) / the CFilter_Bind2 {&circle,&canBeHit} in searches;
  1Ch CCharString sound tmp / long effect id / float r / CTCGraphicAppearance*; 20h creature* key / map iterator `it` / objects list;
  24h ETCInterfaceType key for GetTC / HeroStats* fallback; 28h list iter / CTCWife*; 2Ch effect id / killer / float reach;
  30h key / list<CThingCreatureBase*> head ptr (search 2) / murder CDefPointer; 34h radius float; 38h this; 3Ch/40h/44h/48h dir C3DVector + CCharString tmps;
  4Ch damage (final block) / list iterator; 50h..5Ch C3DVector pos / circle copy; 60h CDefPointer<const CSpecialEffectsDef> effects (whole release phase);
  64h damage float; 6Ch owner; 70h map iterator (final block) / find result; 74h..80h C2DBoxI / C3DVector pos;
  84h..8Ch owner pos; 90h..9Ch CThingFilter_OverlappingCircle {x,y,z,radius}; A0h/B4h PerEffectInfo tmp (0x14);
  C8h CHitParameters hit (fields +0x1E, +0x22, +0x23 written after ctor); 128h action temp (CCreatureAction_*).

## Pseudo-C++ in retail order
```
void CTCUnholyPower::FrameUpdate()
{
  if (!UnholyPowerActionInitialised) return;                                  // 795312 -> 79772C
  Timer.FrameUpdate();                                                        // 795324 0x008EDDD0
  CThingCreatureBase* owner = (CThingCreatureBase*)&GetParentThing();         // ebx, [esp+6Ch]
  long state = Timer.CurrentState;                                            // esi (+0x30)
  if (Finished) {                                                             // 79532C
    if (NumFramesToWaitForFadeOut <= 0) owner->RemoveTC(0x86 /*own TCI*/);   // 795341 (twin 0x85); return
    else --NumFramesToWaitForFadeOut;                                         // 795355; return
    return;
  }
  if (CurFramesForEnvChange < NumFramesForEnvChange) {                        // 795361 (else -> 795705)
    // ---------- BUILD-UP ----------
    if (++CurFramesForEnvChange >= NumFramesForEnvChange) SetPTarget();       // 79536F/795379
    owner->SetCurrentAction(CCreatureAction_BuildUpUnholyPowerSpell(owner, CCharString("ST_BUILD_UNHOLY_POWER")));  // 79537E..7953A9 (arg CCharString built in the arg slot: push ecx; mov ecx,esp)
    if (state != 0) {                                                         // 7953AE
      CCharString s = GDefStringTable.GetString(PDef->CancelSound[SpellLevel]);   // 7953B2..7953C9 -> jmp 795473
      goto play;
    }
    CTCHeroStats* stats = owner->GetTC<CTCHeroStats>(/*4*/);                  // 7953D3 inline: [ebx+20h]&10h, LowerBound 0x0040F020; else stays uninit ([esp+24h])
    long modify = StaminaModify;                                              // 795407 fld Per / fadd Acc / fst Acc
    AccModifyStamina += PerFrameStaminaModify;
    while (AccModifyStamina > 1.0f) { AccModifyStamina -= 1.0f; ++modify; }   // 79542C..795446 (rotated loop, fstp at end)
    stats->ModifyStaminaBy(-modify);                                          // 79544C
    if (stats->Stamina(+0x58) <= 0) {                                         // 795456 jg 79549F
      CCharString s = PDef->CancelSound[SpellLevel].GetString();              // 79545D CDefString::GetString 0x00415DD0 into same slot [esp+1Ch]
   play:                                                                      // 795473 (tail-merged with the state!=0 block -- write both blocks out in full)
      PlaySoundW(s, false); /* ~s */ owner->FinishCurrentAction(); UnsheatheWeapon(); FinishMove();
    }
    // 79549F: creature search
    float radius = PDef->RadiusOfEffect[SpellLevel];                          // [esp+1Ch]
    const C3DVector& p = owner->GetPos();  C3DVector pos = p;                 // copy to [esp+50h]
    long x = ftol(pos.X), y = ftol(pos.Y), r = ftol(radius + 20.0f);
    C2DBoxI box(x - r, y - r, x + r, y + r);                                  // [esp+74h..80h]
    std::list<CThingCreatureBase*> things;                                    // 795516 inline: malloc(12), head->next=head->prev=head ([esp+34h])
    CThingFilter_OverlappingCircle circle(pos, radius);                       // [esp+90h..9Ch] {X,Y,Z,radius}
    CThingFilter_CanBeHitByEpicSpell canBeHit;                                // byte [esp+13h] = 0
    CFilter_Bind2<...> filter(circle, canBeHit);                              // [esp+14h]/[esp+18h]
    PeekThingSearchTools().GetAllThingsInRoughArea(box, filter, things);      // 79557E/795585
    for (it = things.begin(); it != things.end(); ++it) {                     // 79558A..7956EE
      CThingCreatureBase* c = *it;                                            // [esp+20h]
      if (c == owner) continue;
      if (CTCWillResponse::IsThingNotBloodyWillableAtAll(c)) continue;
      if (!CTCWillResponse::IsThingEpicSpellable(c)) continue;
      if (!Attackable(c)) continue;
      if (NCreatureMode::IsUnderground(*c)) continue;
      if (LastFrameCreaturesHit.find(c) != LastFrameCreaturesHit.end()) continue;   // 7955FD 0x007978C0
      if (!IS_THING_ALIVE(c)) continue;                                       // 795612 0x005C05C0
      if (!(c->Health > 0.0f)) continue;                                      // 79561F (+0xB4)
      LastFrameCreaturesHit[c].ActualCreature = CIntelligentPointer<CThingCreatureBase>(c);   // 795636 ctor 0x668B60, op[] 0x798330, op= 0x668B80, dtor
      LastFrameCreaturesHit[c].FramesToGetHit = 0;                            // 795661
      if (CTCGraphicAppearance* ga = c->PGraphicAppearance /*+0x64*/) {      // 795674 (ga kept in [esp+1Ch])
        LastFrameCreaturesHit[c].DefaultChannel = ga->(+0x40 channel desc);   // ga+0x40 read BEFORE op[]
        ga->SetLightingChannel(SpellExclusionChannel, true, true);            // 7956A8 (twin +0xA8)
      }
      if (c has TC 0x75 /*[c+2Ch]&200000h*/) c->GetTC(0x75)->(+0x20 bool) = false;   // 7956AD..7956DE
    }
    /* ~things */ return;                                                     // 7956F4 0x006F09D0
  }
  // ---------- RELEASE PHASE (795705) ----------
  CDefPointer<const CSpecialEffectsDef> effects;                              // [esp+60h] = 0
  PDef->GetSubDef(effects);                                                   // 795713 0x00669580 (ecx = PDef raw)
  if (CurFramesForEnvChange == NumFramesForEnvChange) {                       // 795718 (else 795855)
    ++CurFramesForEnvChange;
    if (effects) {                                                            // 795729
      long e = 0;  bool ok = effects->GetEffectFromString(CCharString("RELEASE_EFFECT"), e);       // ok stored [esp+13h] before ~CCharString
      if (ok) CTCSpecialAbilities::CreateParticleEffect(e, owner->GetPos());
      ok = effects->GetEffectFromString(CCharString("RELEASE_COLUMN_EFFECT"), e);
      if (ok) CTCSpecialAbilities::CreateParticleEffect(e, owner->GetPos());
    }
    CTCDRumble::Create(owner->GetPos(), (EQuakeStrength)3, (EQuakeLength)0, false);   // 7957C8
    PlaySoundW(GDefStringTable.GetString(PDef->ReleaseSound[SpellLevel]), false);      // 7957DD (edi = 0 used as `false`)
    StopSound(LoopingSound);                                                  // 79580F call 0x00794180 (twin +0xBC)
    LoopingSound = PlaySoundW(GDefStringTable.GetString(PDef->ReleaseLoopSound[SpellLevel]), false);  // 79581D; store before ~CCharString
  }
  if (DurationOfSpellFrames > 0) {                                            // 795855 (else 796EE7)
    --DurationOfSpellFrames;
    float damage = PDef->DamagePerHit[SpellLevel];                            // [esp+64h]
    float radius = PDef->RadiusOfEffect[SpellLevel];                          // [esp+34h]
    C3DVector pos = owner->GetPos();  box as above ([esp+90h..9Ch]);          // 795889..7958FC
    if (!PerformedReleaseAction) {                                            // 7958E9 (push ebx = owner arg shared by both ctors)
      PerformedReleaseAction = owner->SetCurrentAction(CCreatureAction_ReleaseUnholyPowerSpell(owner));   // 79590A..795928
      if (PerformedReleaseAction) {                                           // 795930
        if (owner has TC 0x68 /*[ebx+2Ch]&100h*/) {                           // 795938
          CTCHeroExperience* xp = owner->GetTC(0x68);                          // edi
          CDefPointer<const CHeroAbilityDef?> ab;                             // [esp+28h]=0  (type = what 0x00792350 instantiates)
          CGameDefinitionManager::Get().GetDef(CCharString("HERO_ABILITY_SKILL_UNHOLY_POWER_SPELL"), ab);
          xp->AddTrainableStatExperience(ab->(+0x40), ab->(+0x7C));           // 7959AD; ~ab inline release
        }
        std::list<CThingObject*> objs;                                        // 7959BE out-of-line ctor 0x007926C0 ([esp+20h], allocator byte [esp+13h])
        CThingFilter_OverlappingCircle circle(pos, radius); filter; PeekThingSearchTools().GetAllThingsInRoughArea(box, filter, objs);  // 0x007930B0
        for (it = objs.begin(); it != objs.end(); ++it) {                     // 795A2A..795C8F, it in [esp+4Ch]
          CThingObject* o = *it;
          if (!o || (o->dead +0x91 & 1)) continue;
          if (IsThingNotBloodyWillableAtAll(o) || !IsThingEpicSpellable(o)) continue;
          C3DVector dir = o->GetPos() - owner->GetPos(); dir.Normalise();     // 795A76..795B27 (owner pos first -> edi), GFFastOneOverSqrt inline
          CHitParameters hit(0, owner, 0, o->GetPos() - dir, dir, true, false, false, false);  // 795B2B..795B6E (pos [esp+50h], dir [esp+40h])
          hit.AddDamageWithAttribute(0xA /*twin 9*/, damage * 100.0f);       // 795B73
          hit.(+0x23) = false; hit.(+0x1E) = true;                             // 795B93/795B9B
          bool explode = false;                                                // [esp+13h]
          if ((o->ThingTypeFlags +0x6C & 8) && !(o->def(+0x70)->(+0x148) & 1) && IsThingEpicPowerExplodable(o)) explode = true;
          bool saved = true;                                                   // bl
          if (explode) { saved = (o->+0x15C >> 5) & 1; o->+0x15C &= ~0x20; }  // inline SetAsGameStatePersisted(false)
          o->vfunc_0x104(hit);                                                 // 795BF2
          if (explode) o->+0x15C bit5 = saved;                                 // 795C00 (xor/and/xor bitfield write)
          if (o has TC 0x42) o->GetTC(0x42)->AddEvent(new CEventHitBySpecialAbility(hit, 0x13 /*twin 0x12*/));   // 795C18..795C6C
        } /* ~hit 795C71 */
        /* ~objs 795C95 0x00792240 */
      }
    } else {                                                                  // 795CA0
      CCreatureAction_ReleaseAndLoopUnholyPowerSpell a(owner);
      a.SetPItemRemoved(PItemRemoved);                                         // by-value copy 0x004A9730
      owner->SetCurrentAction(a);                                              // result discarded; ~a
    }
    CTCDRumble::Create(owner->GetPos(), (EQuakeStrength)0, (EQuakeLength)0, false);   // 795CE1
    // creature search 2 (795CF7): same list/filter/search as build-up (list head in ebx/[esp+30h])
    for (lit = things.begin(); lit != things.end(); ++lit) {                  // 795D6E..79617F, [esp+2Ch]
      c = *lit; same 5 filters as build-up (owner, NotBloodyWillable, EpicSpellable, Attackable, IsUnderground);
      MapIt f = LastFrameCreaturesHit.find(c);                                // 795DDD, f in [esp+70h]
      if (f == end) {                                                         // 795DEC
        if (!IS_THING_ALIVE(c) || !(c->Health > 0)) continue;
        LastFrameCreaturesHit[c].ActualCreature = CIntelligentPointer<CThingCreatureBase>(c);
        LastFrameCreaturesHit[c].FramesToGetHit = 0;
        { CCharString name("VICTIM_EFFECT"); long e = 0;                      // 795E4D ([esp+28h], e [esp+1Ch]; ebx=0 reused)
          if (effects && effects->GetEffectFromString(name, e)) {             // 795E66
            C3DVector ep = c->GetPos();                                       // [esp+74h]
            if (CTCGraphicAppearance* ga = c->+0x64) ga->GetHelperPoint(CCharString("TARGET_CURSOR_POINT"), 0, 0, ep, 1.0f);
            ep.Z = c->GetHeightZBelowAt(ep, 0, true);                          // 795EE9
            LastFrameCreaturesHit[c].Effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(e, ep));   // 795EFA..795F31
            PerEffectInfo info; info.Position = ep; info.Effect = LastFrameCreaturesHit[c].Effect;   // 795F36..795F7A
            HitEffects.push_back(info);                                       // 795F8D 0x00798410; ~info.Effect
          }
          if (ga = c->+0x64) { LastFrameCreaturesHit[c].DefaultChannel = ga->+0x40; ga->SetLightingChannel(SpellExclusionChannel, true, true); }  // 795F9E
          if (c has TC 0x75) c->GetTC(0x75)->(+0x20) = false;
        } // ~name 796006
      } else {                                                                // 796018
        if (!IS_THING_ALIVE(c)) continue;
        if (IS_THING_ALIVE(f->second.Effect)) continue;                       // 796030 0x004CC340
        { CCharString name("VICTIM_EFFECT"); long e = 0;                      // [esp+3Ch]
          if (effects && effects->GetEffectFromString(name, e)) {
            C3DVector ep = c->GetPos();  helper point (as above); ep.Z = c->GetHeightZBelowAt(ep, 0, true);
            f->second.Effect = CIntelligentPointer<CThing>(CreateParticleEffect(e, ep));    // esi = &f->second.Effect
            PerEffectInfo info; info.Position = ep; info.Effect = f->second.Effect; HitEffects.push_back(info);
          }
        }
      }
    }
    // map sweep 1 (796185): drop creatures that left the radius
    for (MapIt it = LastFrameCreaturesHit.begin(); it != LastFrameCreaturesHit.end(); ) {    // [esp+20h]
      CIntelligentPointer<CThingCreatureBase> cr(it->second.ActualCreature);  // INLINE copy 796197
      if (!cr.GetPItem() || (cr->+0x91 & 1)) { ++it /*inline walk*/; continue; }    // 7961D9 -> 7963DC
      float reach = radius + cr->(+0x60)->(+0x34);                            // [esp+2Ch]
      C3DVector d = cr->GetPos() - owner->GetPos();
      if (d.X*d.X + d.Y*d.Y + d.Z*d.Z <= reach*reach + 5.0f &&                // 79627E (fcompp, test ah,5 / jnp)
          !IsThingNotBloodyWillableAtAll(cr) && IsThingEpicSpellable(cr)) { ++it /*call 0x00793A00*/; continue; }
      ResetCreatureHitState(cr);                                              // 7962D5 (inline by-value copy)
      CTCSpecialAbilities::FadeOrKillEffect(it->second.Effect);
      if (cr.GetPItem() && !(cr->+0x91 & 1)) {                               // 796307
        if (ga = cr->+0x64) ga->SetLightingChannel(it->second.DefaultChannel, true, true);
        if (cr has TC 0x75) cr->GetTC(0x75)->(+0x20) = true;
        cr->+0x15D &= ~0x40;
      }
      LastFrameCreaturesHit.erase(it++);                                      // 796391 inline successor, call 0x00797A60(old)
    } // ~cr 7963DC
    // effect refresh (7963EE)
    for (PerEffectInfo* e = HitEffects.begin(); e != HitEffects.end(); ++e) {  // end in ebx/[esp+24h], edi=&e->Effect
      if (e->Effect.GetPItem() && !(e->Effect->+0x91 & 1)) continue;          // inline IS_THING_ALIVE
      CCharString name("VICTIM_EFFECT"); long id = 0;                         // [esp+3Ch], id [esp+2Ch]
      if (effects && effects->GetEffectFromString(name, id))
        e->Effect = CIntelligentPointer<CThing>(CreateParticleEffect(id, e->Position));   // inline ctor + SetPItem(GetPItem)
    }
    // map sweep 2 = damage tick (7964C1)
    for (MapIt it = begin; it != end; ) {
      CIntelligentPointer<CThingCreatureBase> cr(it->second.ActualCreature);  // inline
      if (cr alive) {                                                         // 7964F6
        if (cr has TC 0xF5 /*[+3Ch]&200000h*/) cr->GetTC(0xF5)->StopFlyingAround(false);   // CTCNymph
        if (--it->second.FramesToGetHit <= 0) {                               // 796563
          it->second.FramesToGetHit = ftol(ConstantFPS * PDef->RandomFactorForNumSecsBetweenDamageHit) + NumFramesBetweenDamageHit;  // fild ConstantFPS
          dir = cr->GetPos() - owner->GetPos(); normalise;                     // owner pos first (esi)
          CHitParameters hit(0, owner, 0, cr->GetPos() - dir, dir, false, false, false, false);   // 7966A6
          hit.AddDamageWithAttribute(0xA, damage);                             // raw damage (no *100)
          hit.(+0x23)=0; hit.(+0x22)=0; hit.(+0x1E)=1;
          if (cr has TC 0x31) { mm = cr->GetTC(0x31); if (!mm->IsModeActive(0x5F)) { cr->FinishCurrentAction(); mm->AddMode(0x5F); } }
          bool explode = !(cr->def+0x148 & 1) && IsThingEpicPowerExplodable(cr);   // [esp+13h]
          bool saved = true; if (explode) { saved = (cr->+0x15C>>5)&1; cr->+0x15C &= ~0x20; }
          cr->vfunc_0x104(hit);
          if (!(cr has TC 0x8B /*[+30h]&800h*/)) cr->AddTC(CCharString("CTCCreatureTintEffect"), true, false);
          cr->+0x15D |= 0x40;
          if (explode) cr->+0x15C bit5 = saved;
          if (cr has TC 0x42) cr->GetTC(0x42)->AddEvent(new CEventHitBySpecialAbility(hit, 0x13));
        } // ~hit 79688F
      }
      if (cr alive && !(cr->Health <= 0)) { ++it /*call 0x00793A00*/; ~cr; continue; }   // 79689B..7968DF (jnp)
      if (cr.GetPItem()) {                                                    // 7968E4
        if (IsThingEpicPowerExplodable(cr)) {                                 // 796900 -> KILL block K1 (79690D)
          K(cr, /*use cr's own effects def first*/ true);
        } else {                                                              // 796E02
          ResetCreatureHitState(cr); FadeOrKillEffect(it->second.Effect);
        }
      }
      LastFrameCreaturesHit.erase(it++);                                      // 796E3A..796EBF FULLY INLINE (successor, _Rebalance_for_erase, ~Effect, ~ActualCreature, free, --size)
      // ~cr 796EC7
    }
    /* ~things 796ED9 */ goto end;
  }
  // ---------- SPELL OVER (796EE7) ----------
  float damage = PDef->DamagePerHit[SpellLevel];                              // [esp+4Ch]
  for (MapIt it = begin; it != end; ++it /*call 0x00793A00 at 7976C1*/) {      // [esp+70h], end in [esp+34h]
    CIntelligentPointer<CThingCreatureBase> cr(it->second.ActualCreature);    // OUT-OF-LINE copy 0x0078E5B0
    if (IS_THING_ALIVE(cr) /*0x006961F0*/) {
      if (cr->UsingComponents.test(0xF5) /*0x00410DE0*/) cr->GetTC(0xF5)->StopFlyingAround(false);
      ResetCreatureHitState(cr);                                              // copy 0x006EF410
      dir/normalise; CHitParameters hit(0, owner, 0, cr->GetPos()-dir, dir, true, false, false, false);
      hit.AddDamageWithAttribute(0xA, damage); hit.(+0x23)=0; hit.(+0x1E)=1;
      bool explode = false (bl); if (!(cr->def+0x148&1) && IsThingEpicPowerExplodable(cr)) explode = true;
      bool persisted = true ([esp+2Ch]); if (explode) { persisted = (cr->+0x15C>>5)&1; cr->SetAsGameStatePersisted(false); }   // CALL 0x0078DDB0 here
      cr->vfunc_0x104(hit);
      if (!cr->UsingComponents.test(0x8B)) cr->AddTC(CCharString("CTCCreatureTintEffect"), true, false);
      if (explode) cr->SetAsGameStatePersisted(persisted);
      if (test(0x42)) GetTC(0x42)->AddEvent(new CEventHitBySpecialAbility(hit, 0x13));
      if (cr->Health <= 0 && IsThingEpicPowerExplodable(cr)) K(cr, false);   // 79721E (jp), K2 at 797256 with bitset::test calls
      // ~hit 79769C
    }
    // ~cr 7976B4
  }
  owner->FinishCurrentAction(); UnsheatheWeapon(); FinishMove();              // 7976D2
  PlaySoundW(GDefStringTable.GetString(PDef->FinishSound[SpellLevel]), false);
end:                                                                          // 79771A
  /* ~effects (inline CDefPointer release) */
}

K(cr) = kill-and-explode block (K1 79690D..796E00, K2 797256..79768B):
  CThing* hero = ParentThing (esi);  CMessageEventManager* mem = GetWorld()->(+0x60);
  mem->AddMessage((EMessageEventType)9, ConstantFPS, cr->(+0x60)->(+0x0C pos), hero, cr, 0, 0, 0);
  if (hero has TC 0x42) GetTC(0x42)->AddEvent(new CEventKilledCreature(*hero, *cr));   // new 0xC
  CTCDExperienceOrb::Create(*cr);
  if (cr has TC 0x3C) cr->GetTC(0x3C)->vfunc_0x34();
  cr->Kill(false);
  CDefPointer<const CMessageEventDef> md;  ([esp+34h] K1 / ebp K2)
  CThing* killer = cr->(+0xF4 CIntelligentPointer<CThing>).GetPItem();
  if (killer && killer->vfunc_0x134())
    if (GetDefinitionManager().GetDef(CCharString("STANDARD_MESSAGE_EVENT_MURDER"), md))
      GetWorld()->(+0x60)->AddMessage(*md, cr->PeekPos(), killer, cr, 0);
  CTCWife* wife = 0; if (cr has TC 0xD7) wife = cr->GetTC(0xD7);
  if (killer && killer has TC 0x51) { deeds = killer->GetTC(0x51);
     if (cr has TC 0x49) { x = cr->GetTC(0x49) /*LowerBound 0x004365B0*/;
        if (long v = x->(+0x18)) { deeds->PostDeed(9, cr->PeekPos(), 0, 2, 1, 0, v, 0, 0); deeds->PostDeed(0x3A, cr->PeekPos(), 0, 7, 0, 0, x->+0x18, 1, 0); } } }
  if (wife) wife->Murdered();
  if (cr has TC 0x42) cr->GetTC(0x42)->AddEvent(new CEventDied(killer, cr));   // new 0x18
  K1 only: CDefPointer<const CSpecialEffectsDef> cfx (copy of cr->def(+0x70), refcount inc/dec around) -> if (cr->def->GetSubDef(cfx)) use cfx else use `effects`:
     { CCharString n("VICTIM_EXPLODE_EFFECT"); long e=0; ok = d->GetEffectFromString(n, e); }  if (ok) { p = cr->GetPos(); if (ga) ga->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, p, 1.0f); CreateParticleEffect(e, p); }
     (both arms tail-merge at 796DD4); ~cfx; ~md.
  K2: only the `effects` arm; ~md.
```
Notes: GetHelperPoint takes MAIN_FOCAL_POSITION_NAME (global CCharString 0x013BAC48) in K, a temp CCharString("TARGET_CURSOR_POINT") in the victim effect.
Floats: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 20.0f 0x0122F160, 100.0f 0x01230A08, 5.0f 0x0126C04C (TU-local const).
No jump tables.
