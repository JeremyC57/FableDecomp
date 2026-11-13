## 0x00854DD0

# DOSSIER 0x00854DD0 -- CCreatureAction_SpiritAttack::PerformAction (720 bytes) -- scout pass 2 (2026-11-12)

## 0. READ FIRST -- new facts this pass (supersede the older text below where they disagree)
- Identity re-verified: rtti-vtables.tsv `CCreatureAction_SpiritAttack 01275d74 slot 3 = 00854dd0`; xref.py: only reference is the
  vtable word .rdata 0x01275D80; Ego_r + FableWin leaf `?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ`
  (public virtual void __thiscall (void)). Manifest label is CORRECT.
- Retail shape re-verified (xdis 0x2e0): `sub esp,7Ch` + push ebx/ebp/esi/edi, NO `push ebp; mov ebp,esp` (default /Oy, no
  optimize("y",off)), NO SEH frame (no /EHsc state), plain `ret` x2 (tail duplicated at 0x00855085 / 0x00855098), 0x008550A0 is the
  next row (SpiritHitResponse::IsOverrideableByActionOfSamePriority). No size-opt idioms -> no optimize("s"). Flags: none.
- CORRECTION: CreateAttackEffect 0x008548B0 is now LANDED: rebuild/src/compiled/00/85/CCreatureActionSpiritAttack_CreateAttackEffect_008548b0.cpp
  (same TU, defined earlier; called out of line here -- just declare it). It uses the TYPED shared class header:
  `#define FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_TYPED` + engine/CCreatureAction_SpiritAttack.h (PTarget +0xA8,
  PParticleEmitter +0xB0, private CreateAttackEffect, size 0xB8 asserted) and wraps engine/_quarantine/CTCCoopSpirit.h
  with `#define CTCCoopSpirit CTCCoopSpiritFields` + a local `class CTCCoopSpirit : public CTCCoopSpiritFields` holding
  StatGetInterfaceType()=0xC7 plus method decls. The best differ source (differ/00854dd0.cpp) still uses the DONOR rename trick
  (`#define CCreatureAction_SpiritAttack DonorCCreatureAction_SpiritAttack`) -- switch to the _TYPED opt-in like the landed sibling.
  GAP: the typed header does NOT declare `PerformAction()`; the row must add `public: virtual void PerformAction();` (PDB: public
  virtual, UAEXXZ) to the typed class -- via a header overlay line under the _TYPED block, not a local class.
- fadd-order lead (0x00854ED2, retail `fld [esp+14h] (range) ; fadd [edi+34h] (radius)`): attempt 3 concluded "only an lvalue
  radius gives this order". COUNTER-EVIDENCE: landed 0x006A4FB0 CThingPhysical::GetFirstCollision
  (rebuild/src/compiled/00/6a/CThingPhysical_GetFirstCollision_006a4fb0.cpp l.51 `distance < radius + GetPhysicsTC()->GetRadius()`)
  uses the BY-VALUE header GetRadius() (FABLE_RETAIL_CTCPHYSICSBASE, no Radius-public probe) and retail/landed emit
  `mov edx,[ebp+60h] ; fld [esp+90h] (radius param) ; fadd [edx+34h]` (0x006A50D3..0x006A50DD) -- i.e. the left operand first.
  So the order depends on the LEFT operand's kind (a parameter/home-resident float there; a call-result temp here), not on the
  getter. Ideas: make `range` a variable with a real home before the add (e.g. the range passed into a __forceinline helper BY VALUE as
  a float parameter, see the helper lead below), or the comparison form. Ego_r/FableWin: `?GetRadius@CTCPhysicsBase@@QBEMXZ`
  (by value) -- the Radius-public probe has no second source, do not land it.
- direction / hit-position lead (the other residue): landed 0x00798770 CTCForcePushed::FinishBeingForcePushed
  (rebuild/src/compiled/00/79/CTCForcePushed_FinishBeingForcePushed_00798770.cpp) builds the SAME 9-arg CHitParameters + AddDamageWithAttribute
  + OnHit(slot 65) inside a TU-local `FABLE_FORCEINLINE void HitOwner(CThing* owner, C3DVector position /*BY VALUE*/, const C3DVector&
  direction, float damage)`; its header comment: the by-value position gives retail's "store the three words between the CHitParameters
  argument pushes" -- exactly what this row's retail does at 0x00854FBD..0x00854FEF (fsub/fstp of hit_pos X,Y,Z interleaved with
  push 0/0/1/0) and what the best source only reaches with the memberwise hit_pos + extra `{}` block. Strong candidate spelling:
  `HitTarget(target, PCreature->GetPos() - direction, direction)` with position (and possibly direction) by value into a forceinline
  helper holding the CHitParameters/AddDamage/OnHit/AddExperience. A by-value direction parameter would also make the outer
  `direction` non-address-taken (attempt-3 probe A3-2: that is the condition for retail's X-first subtraction with only Z homed).
  The helper name is made up (none in Ego_r/FableWin for this class: PDB members are Clone, CreateAttackEffect, FinishAction,
  FinishAttackEffect, FrameUpdate, Initialise, PerformAction) -- mark descriptive in NOTES, as 00798770 does for HitOwner.
- C3DVector facts (FableWin/Ego_r): `??GC3DVector@@QBE...` operator- is const member returning const C3DVector (header line 142/144),
  `?Normalise@C3DVector@@QAEAAV1@XZ` returns C3DVector& (header l.360), `?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ` is out of line
  0x00A14480 (not this row: retail inlines the fast normalise). Copy ctor `??0C3DVector@@QAE@ABV0@@Z` exists in the PDB (user-declared),
  yet 00798770 needed the implicit aggregate copy -- FABLE_RETAIL_C3DVECTOR_IMPLICIT_COPY exists ONLY in this row's overlay $W/hdr/engine/C3DVector.h (attempt-2 probe), not in the shared header; 00798770 models C3DVector locally.
- Landed-callee name to flag: 0x00670000 is landed as CTCPhysicsBase_GetAirResistance_00670000.cpp but its body reads
  PCoopSpiritDef(+0x28)->+0x38; Ego_r `?GetMeleeTargetRange@CTCCoopSpirit@@QBEMXZ` (egor_pdb_names 0x00677730, FableWin too) --
  NAME_FIXES candidate (one address, one name).

# DOSSIER 0x00854DD0 -- CCreatureAction_SpiritAttack::PerformAction (720 bytes)

## 1. Retail shape (xdis 00854dd0 0x2e0)
- __thiscall, plain `ret` (two duplicated epilogues 0x855085 / 0x855098, tail-duplicated OnStrike call), `ret` ends at 0x0085509F; 0x008550A0 is the next row.
- NO frame pointer (`sub esp,7Ch; push ebx/ebp/esi/edi`) -> default /Oy; no SEH prologue even though temporaries with dtors exist
  (CIntelligentPointer temp, CHitParameters) -> no EH state => build without /EHsc unwinding (like siblings, `// Flags: none`).
- Registers: edi = this (spilled to [esp+10h] for the CreateAttackEffect call), ebp = &PCreature (this+8, every GetPItem re-called,
  never cached), esi = target, ebx = 8 constant (GetTC(8) bit, then type-flag test `test [esi+6Ch],bl`), later ebx = CTCCoopSpirit*.
- Frame: [esp+14h] TC out-pointer / CIntelligentPointer temp (vptr 0x01238C6C) / float temps; CHitParameters at [esp+2Ch] (0x60 bytes).
- Normalise: fld X-diff, Y-diff, Z-diff; `fst [esp+28h]; fmul [esp+28h]` (Z spilled) then Y*Y, X*X => sum Z*Z+Y*Y+X*X;
  GFFastOneOverSqrt inline (0x17C, table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C), factor = ((3-r*r*sq)*r)*0.5.

## 2. Identity (manifest label CORRECT)
- vtable CCreatureAction_SpiritAttack 0x01275D74 slot 3 = 0x00854DD0 (rebuild/manifest/rtti-vtables.tsv); slot 3 = PerformAction in
  every CCreatureAction (SpiritReturnToHero 0x00854720 slot 3, SpiritHitResponse 0x008550B0 slot 3, both landed as PerformAction).
- Ego_r.pdb (egor_pdb_names.tsv 0x00844350) and FableWin.pdb (_fablewin_api.tsv 0x01826DEC):
  `?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ` -> `public: virtual void __thiscall PerformAction(void)`.
- Only xref: the vtable (.rdata 0x01275D80). No direct callers.

## 3. Callees (landed file / landed name / PDB name; DRAFT MISMATCHES flagged)
| addr | landed row | name to use | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC<T> forceinline) | draft CInterfaceMap::LowerBound -- WRONG, local |
| vslot 33 (+0x84) on TCI_TARGETING(8) TC | -- | CTCTargetingBase::GetPTargetedThing() const (CTCTargetingBase.h l.34/83; landed 0x00802610 CTCTargetingPlayer::GetPTargetedThing; 007f9b90 / 0064c869 cite slot 33) | draft CTCTargeting::GetTarget -- WRONG |
| 0x00A01B10 / 0x00A01B90 / 0x00A01C10 | CBaseIntelligentPointer ctor / SetPItem / dtor | temp `CIntelligentPointer<CThing>(target)` then `PTarget = temp` | ok in behaviour |
| 0x00670000 | 00/67/CTCPhysicsBase_GetAirResistance_00670000.cpp (MISLABELLED) | CTCCoopSpirit::GetMeleeTargetRange() const (`?GetMeleeTargetRange@CTCCoopSpirit@@QBEMXZ`, Ego_r+FableWin); body `mov eax,[ecx+28h]; fld [eax+38h]` = PCoopSpiritDef(+0x28)->MeleeTargetRange(+0x38, struct_layouts_egor CCoopSpiritDef) | draft GetRange -- WRONG; NAME_FIXES candidate for the landed row |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp, Leaf ?GetPos@CThing@@QBEABVC3DVector@@XZ | CThing::GetPos() const, returns const C3DVector& | draft GetPosition returning pointer -- WRONG |
| 0x006A4DF0 | 00/6a/CThingPhysical_IsInRange_006a4df0.cpp | CThingPhysical::IsInRange(const C3DVector&, float) const (bool) | draft non-const pointer arg |
| 0x008548B0 | NOT landed (DIFFER in hand283) | CCreatureAction_SpiritAttack::CreateAttackEffect() (private, PDB AAEXXZ) | ok |
| 0x00789238 | 00/78/CHitParameters_CHitParameters_00789238.cpp | CHitParameters(NHitLocations::CHitLocation*, CThing*, CThing*, const C3DVector&, const C3DVector&, bool knockDown, bool blockable, bool flourish, bool blockCounter), ret 24h | draft `void* location`; arg values (0,creature,0,pos,dir,false,true,false,false) correct |
| 0x00868CA0 | 00/86/CHitParameters_AddDamageWithAttribute_00868ca0.cpp | CHitParameters::AddDamageWithAttribute(long/EDamageAttribute, float) | ok (attr 0, 1.0f) |
| 0x00789358 | 00/78/CHitParameters00789358_..._00789358.cpp | CHitParameters::~CHitParameters() (CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_CTOR declares it) | ok |
| vslot 65 (+0x104) on target | -- | CThingGameObject::OnHit(CHitParameters&) (see 00798770 FinishBeingForcePushed `owner->OnHit(hit)` slot 65) | draft CThing::Hit -- WRONG name |
| 0x0066FB10 | 00/66/CTCCoopSpirit_AddExperience_0066fb10.cpp | CTCCoopSpirit::AddExperience(long) (landed; PDB also has AddScore(long); body adds to +0x10 which Ego_r names `Score` -- ambiguous, keep landed) | ok |
| 0x006707E0 | 00/67/CTCCoopSpirit_OnStrike_006707e0.cpp | CTCCoopSpirit::OnStrike() (reads OnStrikeParticle +0x2c; PDB has both OnHit and OnStrike) | draft OnHit -- WRONG |

## 4. Members
- this+0x08 PCreature: CCreatureActionBase, CIntelligentPointer<CThingCreatureBase> -- engine/CCreatureActionBase.h opt-in
  FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE (+ FABLE_RETAIL_CINTELLIGENTPOINTER first).
- this+0xA8 PTarget (CIntelligentPointer<CThing>), +0xB0 PParticleEmitter: engine/CCreatureAction_SpiritAttack.h (opaque bytes,
  offsets asserted); FrameUpdate 00854b90 redeclares `class CCreatureAction_SpiritAttack : public CCreatureActionBase { CIntelligentPointer<CThing> PTarget, PParticleEmitter; }`.
- thing+0x20 UsingComponents, +0x44 Components (CThing.h, GetTC<T> FABLE_FORCEINLINE; FABLE_RETAIL_CTHING), bit 8 -> TCI_TARGETING, bit 0xC7 -> CTCCoopSpirit.
- thing+0x60 PhysicsTC (CThing.h, GetPhysicsTC() const) -> CTCPhysicsBase::Radius +0x34 (GetRadius(), CTCPhysicsBase.h; FABLE_RETAIL_CTCPHYSICSBASE).
- thing+0x6C ThingTypeFlags, bit 8 = creature bit (CThingCreatureBase mask 15 vs CThingGameObject 7): `GetTypeFlags() & 8` (CThing.h l.423),
  same idiom as landed 008c0c00 / 008c48a0 / 008c92c0.
- thing+0x91 bit0 Dead: `IsDead()` (CThing.h l.280).
- CHitParameters (CHitParameters.h, 0x60): Blocked +0x26 / HitNegated +0x27 -> IsBlocked() / IsHitNegated() (draft "Deflected" wrong).
  The 9-arg ctor and AddDamageWithAttribute are NOT declared in the shared header (no opt-in); landed 00798770 declares them locally.
- CTCCoopSpirit: engine/_quarantine/CTCCoopSpirit.h (fields only: PCoopSpiritDef +0x28, Score +0x10); no method opt-ins -> declare GetMeleeTargetRange/AddExperience/OnStrike + StatGetInterfaceType 0xC7 (FrameUpdate 00854b90 idiom).
- Draft models LOCALLY although shared headers exist: C3DVector, CThing, CTCPhysics(Base), CHitParameters, CBaseIntelligentPointer/CIntelligentPointer,
  CCreatureAction_SpiritAttack, CInterfaceMap (=CVectorMap), CTCTargeting (=CTCTargetingBase).

## 5. Visibility
- GetTC<T>, IsDead, GetTypeFlags, GetPhysicsTC, GetRadius, C3DVector ops, Normalise, GFFastOneOverSqrt: inline (headers).
- Out of line (not visible): GetPItem/SetPItem/ctor/dtor (0xA01Bxx), LowerBound 0x0040F020, GetPos, IsInRange, GetMeleeTargetRange (own TU 0x0067xxxx),
  CHitParameters ctor/dtor/AddDamage, AddExperience, OnStrike.
- Same TU, earlier: CreateAttackEffect 0x008548B0 (address < row; called, not inlined -- fine as an extern member decl).
- C3DVector opt-ins: FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX gives `(Z*Z+Y*Y+X*X)` with the default Normalise
  (= retail's `fst; fmul` Z spill pattern, cited at 0x00A587B2 in the header). Newton variants: GFFASTONEOVERSQRT_NEWTON_{GROUPED,SQUARE_PAREN,STEP_PAREN,PAREN}
  (no-code paren tuples that move the scheduling cut -- the residue here IS an x87 schedule/register-role issue, try these).
  operator-: default or FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP.

## 6. Siblings
- 00/85/CCreatureActionSpiritReturnToHero_PerformAction_00854720.cpp (`// Flags: none`, FABLE_RETAIL_BASE_OBJECT, _CCREATUREACTIONBASE,
  _CCREATUREACTIONBASE_PCREATURE, _CTHING, _CINTELLIGENTPOINTER, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTCPHYSICSBASE;
  `PCreature->GetPos()`, `PCreature->GetPhysicsTC()`).
- 00/85/CCreatureActionSpiritAttack_FrameUpdate_00854b90.cpp (same class; GetTC with `if(i==map.end()||type<i->first) i=map.end()`,
  CTCCoopSpirit wrapper over _quarantine header with StatGetInterfaceType 0xC7).
- 00/79/CTCForcePushed_FinishBeingForcePushed_00798770.cpp (same CHitParameters 9-arg ctor + AddDamageWithAttribute + OnHit slot 65).
- 00/85/CCreatureActionSpiritAttack_FinishAction_008547d0.cpp, _Initialise_008545f0.cpp, _Clone_00855230.cpp.

## 7. Old draft model errors
- Names: GetPosition->GetPos (reference), GetRange->GetMeleeTargetRange, OnHit->OnStrike, Hit->OnHit, GetTarget->GetPTargetedThing,
  Deflected->HitNegated, m_flags6c->ThingTypeFlags/GetTypeFlags()&8, m_flags91->Dead/IsDead().
- Local stand-ins everywhere (verifier warns / strict rejects); FILL10 vtable padding.
- Normalise sum order (Z,Y,X) written by hand; use header ZYX opt-in. The recorded residue (702v720) is the x87 schedule (retail keeps
  X/Y on the stack, only Z spilled) and edi(this)/ebp(&PCreature) swap -- try PCreature->GetPos() through the header's CIntelligentPointer
  `operator->` (Get() -> GetPItem) rather than a GetCreature() helper, and the Newton paren variants.
- Control flow is right: GetTC(8) unconditionally used (uninitialised out-ptr if absent), `PTarget = CIntelligentPointer<CThing>(target)`,
  `if(target && !target->IsDead() && (target->GetTypeFlags()&8))`, IsInRange(PCreature->GetPos(), range + target->GetPhysicsTC()->GetRadius()),
  `if(!hit.IsBlocked() && !hit.IsHitNegated()) spirit->AddExperience(1)`, final fresh GetTC(0xC7)->OnStrike().

