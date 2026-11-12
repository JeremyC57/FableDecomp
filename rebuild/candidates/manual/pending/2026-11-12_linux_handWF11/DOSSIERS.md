## 0x008DA410

# DOSSIER 0x008DA410 -- NCreatureMode::CModeScreamerAggressive::OnActivate (511 bytes)

## 1. Retail shape (xdis 008da410 0x20f)
- __thiscall virtual, plain `ret` (no args). NO frame pointer: `sub esp,10h; push ebx; push ebp; push esi` (ebp = this, `mov ebp,ecx`), edi pushed later. /O2 /Oy default, no SEH frame (no /EHsc unwinding table: temporaries' dtors called inline). No size-opt idioms. => `// Flags: none`, no pragma.
- Locals (0x10): local0 = CDefPointer<const CSpecialEffectsDef> (released at the very end: `mov ecx,[esp+10h]; dec [ecx+4]; jne; call [eax+4]` = CDefPointer::Release inline, AFTER the register pops);
  local4 = long effect (zeroed in memory AND edi, reloaded `mov edi,[esp+14h]` after GetEffectFromString);
  local8..0xF = shared slot: zeroed at entry (`xor ebx,ebx; mov [esp+14h],ebx` before push edi), then CCharString temp, CIntelligentPointer temps (8 bytes), the GetTC key copies (0x4B, 0x30), and the uninitialised-read on the TCI_COMBAT false path (`mov eax,[esp+18h]`). Entry zero of local8 suggests `CTCCombat* pcombat = 0;` (or similar) declared early -- check it.
- bl = conditional-temporary flag for the CCharString("AGGRESSIVE") (`mov ebx,1` before the GetEffectFromString call, `test bl,1` dtor). Then ebx is REUSED to hold the CIntelligentPointer<CThing> vtable 0x01238C6C (`mov ebx,1238C6Ch` right after first GetPItem) stored into both temporaries.
- Flow: GetSubDef && GetEffectFromString (condition temp) -> IS_ALIVE(PEffect) {Kill(true); PEffect = CIntelligentPointer<CThing>(0);} -> if (effect > 0) {PEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false)); if IS_ALIVE(PEffect) { emitter=0 (`xor edi,edi`); PEffect.Get()->GetTC(TCI_PARTICLE_EMITTER(0x4B), &emitter) [expanded, `mov ecx,[eax+28h]; test ch,8`]; CCharString("HIT_LOCATION_CYLINDER_01") temp; emitter->AttachToThing(*ParentThing, 12, name, 0, 0.0f); } } -> ParentThing->GetTC(TCI_COMBAT(0x30), &combat) [`test dword [esi+24h],10000h`] ; combat->ReadyForMelee(+0x80) = true; CModeStandBase::OnActivate() (qualified, 0x008621A0, ecx=ebp) -> def release.
- Note `jle` on effect: `if (effect > 0)` signed long.
- GetSubDef called with ecx = ParentThing->PBaseDef raw word (`mov eax,[ebp+4]; mov ecx,[eax+70h]`) -- NO add-ref'd copy of the def (unlike SuperAttack 0x008D8330).

## 2. Identity
- Name: `?OnActivate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ` -- void NCreatureMode::CModeScreamerAggressive::OnActivate(), non-const virtual.
  Sources: (a) rtti-vtables.tsv NCreatureMode::CModeScreamerAggressive vtable 0x012681EC slot 5 = 0x008DA410 (xref.py: only ref is .rdata 0x01268200 = slot 5); slot 3 = 0x008DA610 FrameUpdate (landed), slot 6 = 0x008DA690 OnDeactivate (landed), slot 1 GetType 0x00741350, 4 GetAnimFilter 0x00741360, 0 dtor 0x00741370; CModeBase slot 5 = OnActivate (CTCCreatureModeManager.h line 20/27). (b) egor_pdb_names.tsv and _fablewin_api.tsv both list ?OnActivate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ. Manifest label correct but namespace missing: use NCreatureMode::.
- Mangled leaf: ?OnActivate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ
- Effect-name literals (from Fable.exe): 0x01285AD4 = "AGGRESSIVE", 0x01285AB8 = "HIT_LOCATION_CYLINDER_01". The old draft's "SCREAMER_AGGRESSIVE" / "EMITTER" are WRONG.

## 3. Callees (landed row / PDB name)
| addr | landed file | PDB name | draft spelling |
|---|---|---|---|
| 0x00669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (const, takes CDefPointer<const T>* ) | draft: non-const, CDefPointer<T>& -- WRONG signature |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*, long=-1) / ~CCharString | draft CNameString wrapper -- not a PDB class; use CCharString |
| 0x00669D30 | 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp | ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z (const CCharString&, long&) const | draft takes const CNameString& -- WRONG |
| 0x00A01B10/B50/B90/C10 | 00/a0/CBaseIntelligentPointer_*.cpp | ??0 / ?GetPItem@..QBEPAVCBaseObject@@XZ / ?SetPItem@..QAEXPAVCBaseObject@@@Z / ??1 | draft local BaseIntelligentPointer -- use engine/CIntelligentPointer.h |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z | ok (draft on ThingNative) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft `GetPosition` -- WRONG, GetPos |
| 0x006E0880 | not landed | ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z (static __fastcall, ecx=long, edx=const C3DVector&, push bool) | draft `CreateEffectThing` -- MADE UP/WRONG; SuperAttack 0x008D8330 row already spells CTCDParticleEmitter::Create |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE...ABW4ETCInterfaceType@@@Z | via CThing::GetTC expanded |
| 0x006E0BE0 | 00/6e/CTCDParticleEmitter_AttachToThing_006e0be0.cpp | ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z (CThing&, long, const CCharString&, long, float) | draft takes ThingNative* and CNameString -- fix |
| 0x008621A0 | 00/86/CModeStandBase_OnActivate_008621a0.cpp | ?OnActivate@CModeStandBase@@UAEXXZ | draft `StandActivate` -- WRONG, qualified CModeStandBase::OnActivate() |
| def release | inline CDefPointer::Release (slot 1 call [eax+4]) | | |

## 4. Members
- this: CModeScreamerAggressive (Ego_r, sizeof 0x84): Filter CAnimFilterOfGroup +0x68, PEffect CIntelligentPointer<CThing> +0x74, PTarget CIntelligentPointer<CThing> +0x7C. NO shared header NCreatureMode_CModeScreamerAggressive.h exists; model on engine/NCreatureMode_CModeScreamerSuperAttack.h (needs FABLE_RETAIL_CMODEBASE_VIRTUALS, FABLE_RETAIL_CMODESTANDBASE, FABLE_RETAIL_CINTELLIGENTPOINTER; includes CTCCreatureModeManager.h, CModeStandBase.h, CAnimFilterOfGroup.h, CIntelligentPointer.h) or engine/NCreatureMode_CModeDrained.h.
- ParentThing +0x04: CModeBase (engine/CTCCreatureModeManager.h, FABLE_RETAIL_CMODEBASE_PARENTTHING / GetParentThing()).
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents +0x20 (bit tests +0x28 bit 11 = TCI_PARTICLE_EMITTER 75, +0x24 bit 16 = TCI_COMBAT 48), Components +0x44 (CVectorMap), PBaseDef +0x70 (CDefPointer<const CThingBaseDef>; GetPBaseDef() returns a COPY -- retail does not copy, so read PBaseDef directly / PeekBaseDef), Dead bit0 of +0x91 (IsDead()). GetTC expanded: FABLE_RETAIL_CTHING_GETTC_EXPANDED (exactly the retail `lea esi,[x+44h]; ... mov esi,[esi+4]; cmp; je; cmp [eax],key; jle; mov eax,esi` shape).
- CTCCombat::ReadyForMelee bool +0x80 (engine/CTCCombat.h line 137, FABLE_RETAIL_CTCCOMBAT; static_assert line 173).
- CSpecialEffectsDef (engine/CSpecialEffectsDef.h): FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME, FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (same-TU noinline body -> effect long not escaping; retail reloads it into edi).
- CDefClassBase::GetSubDef: engine/CDefClassBase_GetSubDef.h, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (+ FABLE_RETAIL_DEF_CLASS_BASE). CDefPointer.h (Release inline).
- CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER, _ASSIGN (inline operator= = GetPItem then SetPItem), _COMPARE_NULL_INT (`= CIntelligentPointer<CThing>(0)`), ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (IS_THING_ALIVE(PEffect): GetPItem, test, GetPItem, test [eax+91h],1).
- CTCDParticleEmitter.h: FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL / the CTCBASE branch line ~106-118 declares AttachToThing and StatGetInterfaceType (TCI_PARTICLE_EMITTER). Create(long, const C3DVector&, bool) 0x006E0880 is NOT in the header -- declare (row-local or header addition).
- CModeStandBase.h: has FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE (virtual OnDeactivate) but NO OnActivate opt-in; CModeBase::OnActivate slot 5 is declared in CTCCreatureModeManager.h. Calling CModeStandBase::OnActivate() qualified needs a declaration of OnActivate on CModeStandBase (new opt-in e.g. FABLE_RETAIL_CMODESTANDBASE_ACTIVATE, mirroring _DEACTIVATE).
- Old draft LOCAL models that have shared headers: ThingNative (CThing.h), ComponentMap (CVectorMap.h), BaseIntelligentPointer/IntelligentThingPointer (CIntelligentPointer.h), ModeStandNative/ModeScreamer (CModeStandBase.h + new class header), CombatNative (CTCCombat.h), CSpecialEffectsDef/CDefBase/CDefPointer/CDefClassBase (CSpecialEffectsDef.h, CDefPointer.h, CDefClassBase_GetSubDef.h), CTCDParticleEmitter (CTCDParticleEmitter.h), C3DVector (C3DVector.h), plus the dead freight (WeaponMethods, CarryingMethods, AnimationGroupFilter, NativeStringValue, _quarantine/CThing.h donor).

## 5. Callee visibility
- GetSubDef<CSpecialEffectsDef> 0x00669580 and GetEffectFromString 0x00669D30: out of line in retail but the caller behaves as if the bodies were visible (effect reload into edi, def slot not escaping) -> use the noinline-body opt-ins above (same idiom in 00/8c/*008c4b80, 00/8d/CModeDragonFireBeam_StartBeamEffect_008db620, 00/8d/*SuperAttack_OnActivate_008d8330 which use stand-in noinline bodies; 00/84/... rows and 00/7c/CTCMultiStrike_OnKill_007cce00 use the header macros).
- LowerBound 0x0040F020 out of line (CVectorMap.h); GetTC/HasTC forceinline (GETTC_EXPANDED).
- CIntelligentPointer ctor/dtor/GetPItem/SetPItem out of line (0x00A01Bxx); the derived CIntelligentPointer<CThing> ctor/dtor/operator= inline (vtable 0x01238C6C stored by the caller).
- CModeStandBase::OnActivate 0x008621A0 is in a different TU (0x0086xxxx); same-TU neighbours: FrameUpdate 0x008DA610, OnDeactivate 0x008DA690 (defined after this one).

## 6. Siblings
- 00/8d/CModeScreamerAggressive_OnDeactivate_008da690.cpp (same class; old local-model style, FABLE_RETAIL_CINTELLIGENTPOINTER, no pragma).
- 00/8d/NCreatureModeCModeScreamerAggressive_FrameUpdate_008da610.cpp (same class; FABLE_RETAIL_CTHING_GETTC_EXPANDED + CThing.h + CVectorMap.h + C3DVector.h + CIntelligentPointer.h; `PTarget.Get()`, `ParentThing->GetTC(TCI_LOOK,&look)`; no Flags).
- 00/8d/NCreatureModeCModeScreamerSuperAttack_OnActivate_008d8330.cpp (closest twin: GetSubDef + GetEffectFromString("SUPER_ATTACK_BEAM") + CTCDParticleEmitter::Create + SetPItem; stand-in noinline bodies).
- 00/8d/NCreatureModeCModeDrained_OnDeactivate_008da7a0.cpp (cleanest modern idiom, Flags: none; macro set: FABLE_RETAIL_CCHARSTRING, _CCLASS, _BASE_OBJECT, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CTHING_GETTC_EXPANDED, _CMODEBASE_VIRTUALS, _CMODEBASE_PARENTTHING, _CMODESTANDBASE, _CMODESTANDBASE_DEACTIVATE, _CTCCREATUREMODEMANAGER, _CINTELLIGENTPOINTER_COMPARE_NULL_INT, _CINTELLIGENTPOINTER, _CINTELLIGENTPOINTER_ASSIGN, _IS_THING_ALIVE; pattern `X* p=0; if(thing->GetTC(&p)) ...; PDrainer=CIntelligentPointer<CThing>(0); CModeStandBase::OnDeactivate();`).
- 00/8c/CCombatActionStrikeResponseScorpionKing_Initialise_008c4b80.cpp (GetSubDef/GetEffectFromString/Create/AttachToThing in one row).

## 7. Old draft model errors
- Previous residue: DIFFER 514v511, register allocation (retail this=ebp, effect/new effect/emitter=edi, ebx = bool flag then the 0x01238C6C vtable constant). Draft gave this=edi, effect=ebp, immediates.
- Wrong names: GetPosition->GetPos, CreateEffectThing->CTCDParticleEmitter::Create(long, const C3DVector&, bool) (draft passes C3DVector* and the call is a free fastcall), StandActivate->CModeStandBase::OnActivate, CNameString->CCharString, wrong literals ("AGGRESSIVE", "HIT_LOCATION_CYLINDER_01").
- Wrong signatures: GetSubDef non-const with reference (PDB: const, CDefPointer<const T>*); def type CDefPointer<CSpecialEffectsDef> vs PDB CDefPointer<const CSpecialEffectsDef>.
- Draft never initialises local8 / combat (retail zeroes local8 at entry before anything); try `CTCCombat* pcombat = 0;` (or the emitter) declared at the top, as Drained's `X* p=0` idiom. Draft's combat GetInterface ignores the return; retail also ignores it (stores through combat unconditionally) -- keep `ParentThing->GetTC(&pcombat); pcombat->ReadyForMelee = true;`.
- Emitter: retail `xor edi,edi` = `CTCDParticleEmitter* pemitter = 0;` right before the 3rd GetPItem, GetTC called on that GetPItem result (`PEffect.Get()->GetTC(&pemitter)` or `PEffect->GetTC`), then AttachToThing with `*ParentThing` (CThing&).
- Local models instead of shared headers (ThingNative/_quarantine CThing donor etc.) -- the verifier warns/rejects; rewrite on shared headers (Drained 0x008DA7A0 style).

## 0x0079CC80

# DOSSIER 0x0079CC80 CTCBulletTime::PlayEffect (512 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 0079cc80 0x210)
- __thiscall, `ret 10h` (4 stack args), NO frame pointer (`sub esp,0Ch` then push ebx/ebp/esi/edi), NO SEH prologue -> plain /O2 TU
  (no optimize("y",off), no /EHsc frame, no size idioms). 0x0079CE80 (PlaySoundW) follows directly; the body ends at 0x0079CE7F.
- Frame: 3 locals. [esp+10h] = effects CDefPointer (zeroed via ebx), [esp+14h] scratch, [esp+18h] = effect index `long`
  (zeroed via ebx; its address is passed to GetEffectFromString). In the `effect != 0` arm the CIntelligentPointer<CThing> temp
  (8 bytes) sits on [esp+10h..17h] (the effects slot; Object is already held in ebp).
- Dead arg slot reuse: the `name` arg slot [esp+24h] (after 4 pushes) holds the GetTC key copy (0x4B), the UNINITIALISED emitter
  (`mov esi,[esp+24h]` on the not-present path) and the CCharString("") temporary (`lea ecx,[esp+2Ch]` after 2 pushes).
- `xor ebx,ebx` at the top: ebx = 0 used for both zero-initialisations, `cmp esi,ebx` (effect==0), `cmp eax,ebx` (thing), and
  `push ebx; push ebx` (helper_index 0 / offset 0.0f) in the effect==0 arm. In the effect!=0 arm ebx is reused for `target`
  and the zeros are `push 0; push 0`. This is the residue the old draft never closed (DIFFER 496v512, "zero-register lane gap").
- Control flow: GetSubDef call, THEN `cmp esi,ebx` (effect == 0) BEFORE `test al,al` (found); each arm re-tests `found` and has its
  own GetEffectFromString / GetPos / CreateParticleEffect / emitter / AttachToThing copy; both arms jump to one shared epilogue
  (0x0079CE65) that releases the effects def inline: `test ebp,ebp; je; dec [ebp+4]; jne; mov eax,[ebp]; mov ecx,ebp; call [eax+4]`.
  !found exits through 0x0079CE61 `mov ebp,[esp+10h]`.
- Arm effect==0: `thing = CreateParticleEffect(...)`; `cmp eax,ebx; je; test byte [eax+91h],1; jne` (raw-pointer IS_THING_ALIVE).
- Arm effect!=0: CIntelligentPointer<CThing> temp: ctor 0xA01B10, vptr store 0x01238C6C, SetPItem(thing) 0xA01B90, GetPItem
  0xA01B50, effect->SetPItem 0xA01B90 (inline operator=, item read first), ~ 0xA01C10 called directly; then GetPItem x3 (null test,
  Dead test, GetTC owner) = IS_THING_ALIVE(*effect) + (*effect)->GetTC(...).
- GetTC tail (both arms): `test ch,8` on [thing+28h] (UsingComponents bit 75), `lea esi,[eax+44h]`, key 0x4B stored after the test,
  LowerBound 0x40F020, `mov esi,[esi+4]` (end through the pointer local), `cmp eax,esi; je; cmp [eax],4Bh; jle; mov eax,esi;
  mov esi,[eax+4]` = exactly CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED's GetTC(id,T**) conditional form; return value unused.
- AttachToThing args: push offset(0), index(0), &CCharString(""), attachType ([esp+2Ch] = arg4), target; ecx = emitter.
  CCharString("") = ctor 0x99EBF0 with `push -1` (default length) and "" @ 0x0122D70E, then ~CCharString 0x99EAE0.

## 2. Identity (manifest label is CORRECT)
- Name: CTCBulletTime::PlayEffect, private.
  Mangled leaf: ?PlayEffect@CTCBulletTime@@AAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@PAVCThingCreatureBase@@J@Z
  Signature: `private: void CTCBulletTime::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name,
  CThingCreatureBase* target, long attachType)` (non-const, void return).
- Source 1: ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv (Ego_r 0x007A26B0) -- Ego_r order PlayCreatureOnHitEffect,
  FrameUpdate, FadeOutEffect, PlayEffect, PlaySoundW mirrors retail 0x79C750 PlayCreatureOnHitEffect, 0x79C880 FrameUpdate,
  0x79CC80 (this), 0x79CE80 PlaySoundW (FadeOutEffect has no retail body of its own).
- Source 2: FableWin.pdb ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv (0x026A7DD0, same mangled).
- Shape agrees: ret 10h = 4 dword args; arg1 pointer tested for null, arg2 passed as CCharString& to GetEffectFromString,
  arg3 -> GetPos + AttachToThing(CThing&), arg4 -> AttachToThing's long.
- Callers: xref.py finds NO call/jmp/abs references in retail (0). Not a vtable slot (private non-virtual). Nothing to read.

## 3. Callees (landed file / landed name / PDB name; draft disagreements flagged)
| addr | landed row | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x00669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z = `bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*) const` | WRONG: draft takes `CDefPointer<T>&` (reference, non-const method, non-const T) |
| 0x00669D30 | 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp | ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z | OK |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ (`const C3DVector& GetPos() const`) | WRONG: draft `GetPosition` |
| 0x0072AB70 | 00/72/CTCSpecialAbilities_CreateParticleEffect_0072ab70.cpp | ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (static __fastcall member) | WRONG: draft free function `::CreateParticleEffect` |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@...@@@@QAEPAU?$pair@...@std@@ABW4ETCInterfaceType@@@Z | WRONG: draft `CInterfaceMap::LowerBound(const long*)` (old spelling unified away 2026-11-04) |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) (fable_string.h:290) | OK (but local struct) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | OK |
| 0x006E0BE0 | 00/6e/CTCDParticleEmitter_AttachToThing_006e0be0.cpp | ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z | OK |
| 0x00A01B10/B50/B90/C10 | 00/a0/CBaseIntelligentPointer_*.cpp | ctor / GetPItem() const -> CBaseObject* / SetPItem(CBaseObject*) / virtual ~ | draft: GetPItem returns CThing*, SetPItem(CThing*), non-virtual dtor -- WRONG types |
| [vptr+4] on def | CDefPointer release (pointee Delete, slot 1) | engine/CDefPointer.h default Release | draft local `Destroy()` |

## 4. Members
- this+0x20: CTCBulletTime::PDef (`CDefPointer<const CSpecialAbilitiesBulletTimeDef>`, struct_layouts_egor.tsv CTCBulletTime +32).
  engine/CTCBulletTime.h (FABLE_RETAIL_CTCBULLETTIME + FABLE_RETAIL_CTCBULLETTIME_MEMBERS) names it `CDefPointeeBase* PDef_Object`
  (opaque) -- the author needs a cast to `const CSpecialAbilitiesBulletTimeDef*` (engine/CSpecialAbilitiesBulletTimeDef.h,
  : CParentDefClassBase : CDefClassBase, no this-adjust) to call GetSubDef, or a local overlay. No typed `PDef` opt-in exists.
  The header declares no PlayEffect (add it in the row or as a new opt-in).
- thing+0x20 UsingComponents (bitset<274>), +0x44 Components (CVectorMap), +0x91 Dead bit 0: engine/CThing.h (FABLE_RETAIL_CTHING,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED for GetTC(ETCInterfaceType, T**); `IsDead()` line 280; layout asserts line 404).
- effect (CIntelligentPointer<CThing>): engine/CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER (+ _ASSIGN for the
  inline `item=other.GetPItem(); SetPItem(item);` operator=, matches retail order). Ctor `CIntelligentPointer(T*)` inline SetPointer.
- IS_THING_ALIVE: engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (inline template, `thing!=0` then `!thing->IsDead()`).
- CTCDParticleEmitter: engine/CTCDParticleEmitter.h default branch (StatGetInterfaceType = TCI_PARTICLE_EMITTER 75, AttachToThing).
- CTCSpecialAbilities::CreateParticleEffect: engine/CTCSpecialAbilities.h FABLE_RETAIL_CTCSPECIALABILITIES_FULL.
- CSpecialEffectsDef: engine/CSpecialEffectsDef.h (FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME, _GETEFFECT_BODY; needs STLP map).
- GetSubDef<T>: engine/CDefClassBase.h FABLE_RETAIL_DEF_CLASS_BASE (declaration line 66); CDefPointer: engine/CDefPointer.h.
- Old draft models LOCALLY although shared headers exist: CCharString (fable_string.h), C3DVector (engine/C3DVector.h),
  CInterfaceMap (-> engine/CVectorMap.h), CDefBase/CDefVirtual/CDefPointer (engine/CDefPointer.h), CDefClassBase,
  CSpecialEffectsDef, CThing, CThingCreatureBase, CTCDParticleEmitter, CBaseIntelligentPointer/CIntelligentPointer,
  IS_THING_ALIVE (ThingAlive.h), CTCBulletTime.

## 5. Callee-body visibility
- GetSubDef<CSpecialEffectsDef> 0x00669580: a member TEMPLATE living in the def headers -> body visible in retail's TU but called
  out of line. Sibling CTCUnholyPower ctor 0x007941D0 (00/79/CTCUnholyPower_CTCUnholyPower_007941d0.cpp lines 86-95) writes the
  template body in-row (GetPSubDefInfo(T::StatGetTypeName()), PDefManager->GetPDefFromGlobalIndex, out->SetDynamicCast) and notes
  this is what keeps register allocation right.
- GetEffectFromString 0x00669D30: same-TU noinline body via FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY (chunk 782: makes VC7.1
  treat the effect-index local as uncaptured -- directly relevant to the ebx/zero-register residue).
- CVectorMap::LowerBound 0x0040F020: noinline body from engine/CVectorMap.h (as rows 0x0072AB70, 0x006E0BE0).
- IS_THING_ALIVE: retail expands it inline here (a CParticleEmitter instantiation is out of line at 0x0079DC20 in this TU, not used here).
- CThing::GetPos, CBaseIntelligentPointer members, CCharString ctor/dtor, AttachToThing, CreateParticleEffect: plain out-of-line calls.
- CDefPointer members: forced inline always (CDefPointer.h header note).

## 6. Sibling rows
- 00/79/CTCUnholyPower_CTCUnholyPower_007941d0.cpp: the SAME pattern (GetSubDef(&effects), GetEffectFromString, Create/CreateParticleEffect,
  IS_THING_ALIVE, GetTC(TCI_PARTICLE_EMITTER,&emitter), AttachToThing(parent,8,"",0,0)). Macros: FABLE_RETAIL_CCHARSTRING, _CCLASS,
  _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CTHING_GETTC_EXPANDED, _CINTELLIGENTPOINTER, _CINTELLIGENTPOINTER_ASSIGN, _CTHINGPHYSICAL,
  _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CDEFINITIONMANAGER, _STLP_MAP, _CSPECIALEFFECTSDEF_TYPENAME, _CSPECIALEFFECTSDEF_GETEFFECT_BODY;
  includes fable_string.h, engine/CThing.h, CDefPointer.h, stlport/fable_map.h, CDefClassBase.h, CSpecialEffectsDef.h.
- 00/79/CTCBulletTime_Pause_0079cf30.cpp: FABLE_RETAIL_BASE_OBJECT, _CTCBASE, _CINTELLIGENTPOINTER, _CINTELLIGENTPOINTER_ASSIGN,
  _CTCBULLETTIME, _CTCBULLETTIME_MEMBERS, _CTHING, _CTHING_GETTC_EXPANDED (shared CTCBulletTime header in use).
- 00/79/CTCBulletTime_PlayCreatureOnHitEffect_0079c750.cpp: same class, local models; inline operator= on CIntelligentPointer temp.
- 00/79/CTCBulletTime_FinalCleanup_0079d2d0.cpp, _PlaySoundW_0079ce80.cpp, ctor _0079d110.cpp: CThing.h + GETTC_EXPANDED + CVectorMap.h.
- 00/8c/global_AttachEffectToStaff_008c9a40.cpp: CDefPointer<const CSpecialEffectsDef> + GetSubDef(&) + AttachToThing, local models.
- None of these use #pragma optimize; all plain /O2.

## 7. Old draft model errors (vs disassembly)
- GetSubDef by reference on a non-const T; should be `PDef->GetSubDef(&effects)` with `CDefPointer<const CSpecialEffectsDef> effects;`
  (shared CDefPointer, ENullDef default ctor -> Object=0) and `effects->GetEffectFromString(name, effectType)`.
- Local GetSubDef / GetEffectFromString stand-ins with fake bodies calling invented `Find_*`/`Lookup_*` helpers: replace with the
  real bodies (template body as 0x007941D0; GETEFFECT_BODY opt-in) -- this is the lever for the ebx zero register (the draft's
  NOTES: "tried same-TU stand-ins, named zero, scoped blocks").
- Wrong callee spellings: GetPosition->GetPos, free CreateParticleEffect->CTCSpecialAbilities::CreateParticleEffect,
  CInterfaceMap->CVectorMap, GetPItem/SetPItem types (CBaseObject*), dtor virtual.
- GetInterface local forceinline -> CThing::GetTC(TCI_PARTICLE_EMITTER, &emitter) (GETTC_EXPANDED). Emitter must stay
  UNINITIALISED (retail reads the garbage slot on the not-present path; `= 0` as in 0x007941D0 would add a zero store).
- Structure (effect==0 tested first, both arms re-test found, duplicated tail, IS_THING_ALIVE(*effect) then (*effect)->GetTC) is right.
- Draft size note says 510 bytes; retail is 512 incl. padding-free end at 0x0079CE7F.

## 0x00AABD50

# DOSSIER 0x00AABD50 (515 bytes) -- CSoundEmitterLH::PlaySoundEvent

## 1. Retail shape (xdis 00aabd50 0x213)
- thiscall, `ret 14h` (5 dword args). NO frame pointer (`sub esp,7Ch ; push ebp ; mov ebp,ecx`): ebp = this, /Oy on. No SEH.
- Locals: LHA::CEventPlayOptions at [esp+24h] pre-push (0x5c bytes) + the 0x14-byte query temp at [esp+1Ch] post-push.
- Early-out BEFORE ebx/esi/edi are pushed: `if (!PManager->m_pSoundSystem || !PManager->SoundEnabled) return -1;` with its own
  epilogue `or eax,-1 ; pop ebp ; add esp,7Ch ; ret 14h` at the tail (0xAABF49).
- `this` is spilled to [esp+4] (mov [esp+4],ebp at 0xAABD5E) because ebp is later reused for `name`; the final
  `Playing = true` reloads this from that slot (0xAABF35). Playing=1 is set on BOTH success and handle<0 (jl only skips push_back).
- Order: options ctor 0xC0A790; Pitch = init.Pitch; Is3D = loop ? false : Positional; HeadRelative = attached; gain loop;
  pitch loop; Pos (HeadRelative -> whole C3DVector copy from PManager+0x74 via `add eax,74h`; else if Is3D -> field-wise from
  this+8/+0xC/+0x10); EmitterId = (init.OverrideID == -1) ? EmitterID : OverrideID; system->GetEventManager() (slot 6 +0x18);
  bank.+0x10 / name.GetPString() inline ("" = 0x0129AAF4); query ctor 0xC08720 (ret 8, so &options pushed FIRST stays as the
  play call's 2nd arg); eventManager slot 16 (+0x40)(query, options); query dtor 0xC08780; if (handle >= 0) push_back(new...).
- Gain-fader loop (mask init+0x14) = FindFirstSetBit(bits,16) INLINED (bound 0x80 = 16*8; `xor ecx,ecx` restart each pass; found ->
  `mov edi,ecx ; jmp`); SetFlag16 0xC0A8A0(bit, true). `xor esi,eax ; jne` loop.
- Pitch-fader loop (mask init+0x18 in ebx): ONE out-of-line call FindFirstSetBit 0xA3F860 (__fastcall ecx=esi = the EXHAUSTED gain
  variable (always 0), edx=0x10) before the loop, `1<<bit` hoisted into edi; body = SetFlag 0xC0A8C0(bit,true); `xor ebx,edi ; jne`.
  Draft spells this as a call before a do-while (works). Sibling PlaySample 0xAABF60 (same pattern) is parked as LTCG-impossible
  when written with the call inside the loop (PARKED_INDEX.tsv) -- keep the explicit hoisted form.
- Two `lea` alignment pads (0xAABDBA 6-byte, 0xAABE1D 3-byte) = loop-top alignment, normal /O2.

## 2. Name / signature (manifest label VSlot4 and old `vfunc_4`/`CEngineSubPrimitiveAnimatedMesh::Create` are WRONG)
- rtti-vtables.tsv: CSoundEmitterLH 0x0129E6EC slot 4 = 0x00AABD50; only xref is that vtable (0x0129E6FC).
- engine/CASoundEmitter.h slot 4: `virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop,
  const CSoundInit& init, bool attached) = 0;  // LH 0x00AABD50` (Anniversary 360 PDB CSoundEmitterUN + TLC forwarder).
- Second source: Ego_r/FableWin `?PlaySoundEvent@CThingSoundEmitter@@QAEKAAVCASoundBank@@ABVCCharString@@_NAAVCSoundInit@@2@Z`
  (0x0082A550, forwards to slot 4; egor_pdb_names.tsv). Return unsigned long (K).
- Mangled leaf: `?PlaySoundEvent@CSoundEmitterLH@@UAEKAAVCASoundBank@@ABVCCharString@@_NABVCSoundInit@@_N@Z`
  (pattern of landed `?PlayAtmos@CSoundEmitterLH@@UAEKABVCCharString@@@Z`, `?StopSample@CSoundEmitterLH@@UAEXAAVCASoundBank@@K@Z`).
- Arg mapping verified on the stack: arg1 bank (edi; [edi+10h]), arg2 name (ebp later), arg3 loop ([esp+8Ch]), arg4 init
  ([esp+90h]/[esp+9Ch]), arg5 attached ([esp+94h]).

## 3. Callees
| addr | landed row | landed name | note |
|---|---|---|---|
| 0x00C0A790 | 00/c0/LHACEventPlayOptions_CEventPlayOptions_00c0a790.cpp | LHA::CEventPlayOptions::CEventPlayOptions() (Ego_r, real) | draft: SSoundInstanceParams ctor -- WRONG |
| 0x00C0A8A0 | 00/c0/CBitMaskPair00c0a8c0_SetFlag16_00c0a8a0.cpp | CBitMaskPair_00c0a8c0::SetFlag16(ulong,bool) (descriptive; writes +0x2a WhichGainSendFaders / +0x4c Params.GainSendFaders of CEventPlayOptions) | draft SetFlagA |
| 0x00C0A8C0 | 00/c0/CBitMaskPair00c0a8c0_SetFlag_00c0a8c0.cpp | CBitMaskPair_00c0a8c0::SetFlag(ulong,bool) (+0x2c WhichPitchSendFaders / +0x4e Params.PitchSendFaders) | draft SetFlagB |
| 0x00A3F860 | 00/a3/global_FindFirstSetBit_00a3f860.cpp | `int __fastcall FindFirstSetBit(unsigned long mask, unsigned long numBytes)` (descriptive) | draft declares `long` return; landed is int |
| 0x00C08720 | 00/c0/CVtable012b1a54_CVtable012b1a54_00c08720.cpp | CVtable_012b1a54(void* first, void* second) ret 8; IsSoundEventValid 0x00A0BA40 models it locally as CSampleQueryLH(void* bank, const void* name) -- TWO spellings in tree | draft CSoundRequest -- wrong |
| 0x00C08780 | 00/c0/CBase_CBase_00c08780.cpp | ~T (generic) / CSampleQueryLH::~CSampleQueryLH in 0x00A0BA40 | |
| 0x00C0A590 (slot 6 +0x18) | -- | CSoundSystem::GetEventManager (descriptive, sibling rows) | draft CSoundDevice::GetSystem -- wrong |
| event mgr slot 16 (+0x40) vtable 0x012B2748 | -- | unnamed in sibling rows (v16); name descriptive e.g. PlayEvent(query, options) | draft CSoundSystem::Play |
| 0x009E1BC0 | 00/9e/global_GFGetTime_009e1bc0.cpp | `double __fastcall GFGetTime()` (FableWin PDB ?GFGetTime@@YINXZ) | OK |
| 0x00BFEA1A | operator new thunk (0x24 bytes) | | OK |
| 0x00AAB8B0 | 00/aa/CSoundEmitterLHCActiveSound_CActiveSound_00aab8b0.cpp | CActiveSound(void*,void*,const char* name,void*,bool,bool) ret 18h | real 3rd arg is `const CCharString&` (body calls CCharString COPY ctor 0x0099EC30 = 00/99/CCharString_CCharStringcopy_0099ec30.cpp; caller pushes &name). Retail args: (handle, &bank, name, (float)GFGetTime(), options.Is3D, options.HeadRelative). PlayAtmos's header model uses only ctor 0x00AAB910 -- add this overload. |
| 0x00AAC820 | 00/aa/CCountedPointerCSoundEmitterLHCActiveSound_CCountedPointer_00aac820.cpp | CCountedPointer<CSoundEmitterLH::CActiveSound>(CActiveSound*) out of line | via engine/CCountedPointer.h |
| 0x00AACCC0 | 00/aa/stdlist_pushback_00aaccc0.cpp | std::list<CCountedPointer<CActiveSound>>::push_back | opt-in FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE |
| 0x00AAC870 | 00/aa/Destval...COptimisedPrimitive_00aac870.cpp | ~CCountedPointer<CUnprovenPointee_00aac870> (one address, landed odd name) | the temp's dtor |

## 4. Members
- this (CSoundEmitterLH, no PDB; descriptive spelling used by EVERY landed self row, e.g. PlayAtmos 0x00AAC1D0 / StopSample 0x00AAC340):
  PManager +0x04 (CSoundManagerLH*), Position +0x08 (C3DVector), ActiveSounds +0x14 (std::list<CCountedPointer<CActiveSound>>),
  Positional +0x18 (bool), Playing +0x19 (bool), Active +0x1A, EmitterID +0x1C (long). Draft names m_pBank/m_position/m_sounds/
  m_active/m_started/m_scriptCounter are all wrong spellings. Class is modelled locally in each row (no engine/CSoundEmitterLH.h),
  deriving from engine/CASoundEmitter.h.
- PManager (CSoundManagerLH : CASoundManager, local in rows): +0x24 = CASoundManager::SoundEnabled (engine/CASoundManager.h opt-in
  FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS); +0x40 m_pSoundSystem (needs FABLE_RETAIL_CASOUNDMANAGER_SIZE40); +0x74 C3DVector
  `m_vector` (landed CSoundManagerLH ctor/dtor 0x00A3EF00/0x00A3EDE0 spelling; set by SetHeadRelativePosition slot 52 0x00A41800).
  Draft models these as a local `CSoundBankLH` -- wrong class.
- bank (CASoundBank&, engine/CASoundBank.h, 12 bytes) +0x10 = CSoundBankLH::m_pBank (engine bank handle; landed 0x00A0BA40 local
  model: CSoundBankLH +0x0C m_pSystem, +0x10 m_pBank). Needs a downcast/LH model; draft's CSoundSampleRef::Sample is wrong.
- init: engine/CSoundInit.h (Ego_r PDB): Pitch +0x04 (read -> options.Pitch; Volume +0 NOT read), GainFaderBits +0x14,
  PitchFaderBits +0x18, OverrideID +0x1C. Draft's SSoundPlayParams (Volume/MaskA/MaskB/Owner) = wrong names; draft's
  `instance.Volume = params.Volume` is really `options.Pitch = init.Pitch`.
- options: engine/LHA_CEventPlayOptions.h (Ego_r PDB, 0x5c): EmitterId +0, UserData +4, Gain +8, Pitch +0xC, Is3D +0x10,
  Pos +0x14 (LHA::CVector, NOT C3DVector), Pan +0x20, HeadRelative +0x24, WhichParams..WhichPitchSendFaders +0x26..+0x2C,
  Params (CResourceOptions) +0x30 (GainSendFaders +0x4C, PitchSendFaders +0x4E). Header has no setter methods -- SetFlag16/SetFlag
  at 0xC0A8A0/0xC0A8C0 must be declared (they are members of this class by offsets; landed spelling CBitMaskPair_00c0a8c0).
- name: fable_string.h CCharString, GetPString() inline (test/jne/"" form at 0xAABEA3).
- Local classes in draft that have shared headers: C3DVector, CCharString, CCountedPointer, CASoundEmitter base (missing),
  CSoundInit (as SSoundPlayParams), LHA::CEventPlayOptions (as SSoundInstanceParams), CASoundBank (as CSoundSampleRef),
  CASoundManager (as CSoundBankLH), std::list (as CActiveSoundList).

## 5. Visibility
- FindFirstSetBit body must be visible (it is inlined in the gain loop) yet also called out of line once (0xA3F860, 16-aligned,
  next to CSoundManagerLH code 0xA3E...): define it inline/static in the TU and keep the hoisted call outside the pitch loop.
- push_back out of line: FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE (PlayAtmos uses it). CCountedPointer ctor/dtor out of line
  (header default as PlayAtmos). CEventPlayOptions ctor out of line (header declares only).

## 6. Siblings (00/aa/)
- CSoundEmitterLH_PlayAtmos_00aac1d0.cpp: closest twin (new CActiveSound + counted temp + push_back), defines
  FABLE_RETAIL_CASOUNDMANAGER_SIZE40 + FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE; includes C3DVector, CASoundManager,
  CASoundEmitter, CASoundBank, CBaseClassNonCopyable, CCountedPointer, fable_string.h, stlport/fable_containers.h. No pragmas/flags.
- CSoundEmitterLH_StopSample_00aac340.cpp (bank param, CSoundEventManager slots 22-29), _StopAllSamples_00aac2e0 (+
  FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE), _StopAllCriteriaContainingString_00aac460 (+FABLE_RETAIL_CCHARSTRING), _SetPos_00aabbb0.
  None use #pragma optimize or // Flags:.
- 00/a0/CSoundBankLH_IsSoundEventValid_00a0ba40.cpp: the query temp + GetPString idiom.

## 7. Draft model errors
- Everything type/name-wise above (wrong classes, member names, vfunc_4, `long` return, sample-ref param, CActiveSound ctor signature
  with `float` before bools but CActiveSound 3rd arg must be `const CCharString&`).
- Logic matches retail (514v515, register tie: retail params ptr in ecx, bank in edi, event-mgr vtable in ebx). The draft's own
  next-steps: reorder options setup (HeadRelative before Pitch) or named const-ref local for init.

## 0x009810B0

# DOSSIER 0x009810B0 -- CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage (521 bytes)

## 1. Retail shape (xdis 009810b0 0x219)
- thiscall, `ret 14h` (5 dwords: CDefPointer 4 + C3DVector 12 + long 4). No frame pointer (esp-relative). No SEH prologue
  even though there are by-value class params and temporaries with dtors -> no EH unwinding emitted (/EHs-c-like for this TU,
  as the sibling AIAction rows). `sub esp,0E4h ; push ebx ; push esi ; push edi`. Speed-optimised (no `s` idioms).
- Arg offsets after prologue: message def ptr [esp+0F4h], position [esp+0F8h..100h], parameter [esp+104h].
- Locals: guard (CThingAICreature*) +0x14; filter (CThingFilter_AICreatureHasProperty) +0x18; compare (CThingCompare_Nearest,
  C3DVector) +0x20; guardPointer (CIntelligentPointer<CThingAICreature>, vt 0x0123E7E0) +0x0C; CAIFilter_NoOtherYellForHelp
  +0x18 (reuses filter slot: vt 0x01299350, World +0x1C); action block +0x2C shared by both action temporaries
  (ShowFear 0xA8 / SendMessage 0xC4: PDef +0xD4, PTarget +0xE4).
- Order of the search: `guard=0` stored FIRST (before PeekPos call, 0x9810C1), then PeekPos(+0x18 of PBrain) call, compare
  copied from the returned ref (3 dword loads), push &guard, push &compare, push &filter, THEN `mov [esp+24h],3` (filter=3
  store after the pushes), `mov ecx,esi ; call PeekThingSearchTools 0x8FD000 ; mov ecx,eax ; call GetPBestLeafThing 0x9823D0`.
- Fail (al==0): inline release of the by-value message param (`mov ecx,[esp+0F4h]; test; je; dec [ecx+4]; jne; mov edx,[ecx];
  call [edx+4]`, NO null store), return 3.
- Success: guardPointer = base ctor 0xA01B10, push guard, vptr 0x0123E7E0, SetPItem 0xA01B90 (inline CIntelligentPointer<CThingAICreature>(T*)).
  Then PeekWorld 0x8FCFA0 FIRST (result in eax), filter vt+World stored, then PBrain->Creature (+0x18) loaded, push &filter,
  push creature, ShowFear ctor 0x842270 on block +0x2C (ret 4), push action, push 2.0f (0x40000000), `sub esp,8` + in-place
  CIntelligentPointer<const CThingCreatureBase> (base ctor, vt 0x0125AAEC, GetPItem on guardPointer, SetPItem) -- converting
  ctor INLINE, not the out-of-line COMDAT 0x00982080. call Flee 0x9804A0, result to edi, ~CCreatureActionBase 0x693EF0 on block.
- result != 2: ~CBaseIntelligentPointer(guardPointer) 0xA01C10, param release, return edi (the flee result).
- result == 2: creature = PBrain->+0x18 (edi), push 0 (stopWhenReplaced, 2nd arg of PerformActionAndWait pushed first!),
  push parameter, push &position(param), GetPItem(guardPointer) -> push, push &message(param), push creature, SendMessage ctor
  0x982210 on block (ret 14h), push action, call PerformActionAndWait 0x97DB70 (ret 8), `cmp eax,2 ; setne bl` (bool in bl),
  then inline ~CCreatureAction_SendMessage: ~CBaseIntelligentPointer(+0xE4 PTarget) 0xA01C10, PDef(+0xD4) release in the
  load/dec/store form `mov esi,[eax+4]; mov ecx,eax; add eax,4; dec esi; mov edx,esi; test edx,edx; mov [eax],esi; jne;
  call [vt+4]` then `mov [esp+0D4h],0`, then ~CCreatureActionBase 0x693EF0; ~guardPointer 0xA01C10; `test bl,bl`;
  param release duplicated on both paths (return 3 if bl, else 2).
  So the source is effectively: `bool failed = AIAction_PerformActionAndWait(CCreatureAction_SendMessage(...), false) != 2;`
  (temporaries die at the full-expression end, before guardPointer) then `if (failed) return 3; return 2;`.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x0084C490) and FableWin.pdb (_fablewin_api.tsv 0x017B8AA9, naming_stragglers):
  `?AIAction_FindAGuardAndGiveMessage@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@V?$CDefPointer@$$CBVCMessageEventDef@@@@VC3DVector@@J@Z`
  = protected, non-virtual, non-const: `EAIActionReturn AIAction_FindAGuardAndGiveMessage(CDefPointer<const CMessageEventDef>, C3DVector, long)`.
  Manifest label is RIGHT. Mangled leaf: `?AIAction_FindAGuardAndGiveMessage@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@V?$CDefPointer@$$CBVCMessageEventDef@@@@VC3DVector@@J@Z`.
- Caller: CAIStateGroup_MinionReactToUnconsciousPerson::Update 0x009378F0 (calls at 0x937042/0x937AE0;
  rebuild/src/compiled/00/93/CAIStateGroupMinionReactToUnconsciousPerson_Update_009378f0.cpp declares it locally as
  `long ...(CDefPointer def, C3DVector position, long flags)` -- wrong spelling there).

## 3. Callees (landed row / landed name / PDB name)
| addr | landed file (rebuild/src/compiled/..) | PDB name | draft spelling problem |
|---|---|---|---|
| 006A4D00 | 00/6a/CThingPhysical_PeekPos_006a4d00.cpp | ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ | draft says CThingCreatureBase::GetPosition -- WRONG |
| 008FD000 | 00/8f/CAIStateGroupBase_PeekThingSearchTools_008fd000.cpp (padded-struct stand-in) | ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (returns const&, const) | draft returns non-const ptr, non-const |
| 009823D0 | 00/98/CThingSearchTools_GetPBestLeafThing_009823d0.cpp | ??$GetPBestLeafThing@VCThingFilter_AICreatureHasProperty@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVCThingFilter_AICreatureHasProperty@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z | draft: filter "CThingFilter_CreatureType", out CThingObject** -- WRONG (CThingFilter_AICreatureHasProperty(3), CThingAICreature**, const method) |
| 00A01B10/A01B50/A01B90/A01C10 | 00/a0/CBaseIntelligentPointer_* | CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor | use engine/CIntelligentPointer.h, not local |
| 008FCFA0 | 00/8f/CAIStateGroupBase_GetPThingWorld_008fcfa0.cpp (landed name GetPThingWorld is a made-up stand-in) | ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ (labels chunk 705 "Ego_r+FableWin"; engine/CAIStateGroupBase.h FABLE_RETAIL_CAISTATEGROUPBASE_PEEK) | draft GetPWorld -- WRONG; NAME_FIXES candidate for landed GetPThingWorld |
| 00842270 | 00/84/CCreatureActionShowFear_CCreatureActionShowFear_00842270.cpp | ??0CCreatureAction_ShowFear@@QAE@AAVCThingAICreature@@@Z (vt 0x0127344C, rtti) | draft CCreatureAction_AlertToPlayer -- WRONG (AlertToPlayer is 0x00842300) |
| 009804A0 | 00/98/CAIStateGroupBase_AIActionFleeToThingProximity_009804a0.cpp | ?AIAction_FleeToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@V?$CIntelligentPointer@$$CBVCThingCreatureBase@@@@MABVCCreatureActionBase@@ABVCAIFilterBase@1@@Z | params are const CCreatureActionBase& and const CAIStateGroupBase::CAIFilterBase& (nested class!), draft uses pointers + global CAIFilterBase |
| 00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ~CCreatureActionBase (virtual; slot0 deleting dtor 0x62ED20) | ok |
| 00982210 | 00/98/CCreatureActionSendMessage_CCreatureActionSendMessage_00982210.cpp (landed sig uses CDefPointer<CDefBase>, SMessageData -- wrong) | ??0CCreatureAction_SendMessage@@QAE@AAVCThingCreatureBase@@ABV?$CDefPointer@$$CBVCMessageEventDef@@@@PAV1@ABVC3DVector@@J@Z (Ego_r 0x0084D3A0) = (CThingCreatureBase&, const CDefPointer<const CMessageEventDef>&, CThingCreatureBase*, const C3DVector&, long) | draft passes creature by pointer and receiver as CThing* -- fix; 009822e0 row already spells it right |
| 0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z | draft passes pointer; PDB takes const& (header FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION declares it) |
| filter vt 01299350 | 00/98/CAIFilterNoOtherYellForHelp_Evaluate_009825f0.cpp | ??RCAIFilter_NoOtherYellForHelp@@UBE_NXZ (operator() const, virtual) ; Ego_r layout 8 bytes, `CWorld* World` +4 | draft names the virtual IsStillValid -- PDB says operator() |

## 4. Members
- this+0x04 = CAIStateGroupBase::PBrain (engine/CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]).
- PBrain+0x18 = CAIBrain::Creature (CThingAICreature*) -- engine/CAIBrain.h; inline GetCreature() via
  FABLE_RETAIL_CAIBRAIN_GETCREATURE + FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE (returns CThingAICreature&; FableWin ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ).
- Header opt-ins available: _PEEK (PeekWorld 0x8FCFA0, PeekThingSearchTools 0x8FD000, both const&), _PERFORMACTION
  (AIAction_PerformActionAndWait(const CCreatureActionBase&, bool)). NO declaration of AIAction_FleeToThingProximity or
  AIAction_FindAGuardAndGiveMessage in the header -> declare in-row (or add header opt-in at landing).
- CDefPointer: engine/CDefPointer.h (needs FABLE_RETAIL_DEF_CLASS_BASE; Object +0, forceinline dtor/Release; Release variants
  _RELEASE_COUNTREF (exact `mov ebx,[eax+4];mov ecx,eax;add eax,4;dec ebx;...;mov [eax],ebx` shape = the SendMessage PDef release
  here), _RELEASE_POINTEE, _RELEASE_SELFDELETING_POINTEE; default form documented to yield BOTH `dec [ecx+4]` and the load/dec/store shape).
- CIntelligentPointer: engine/CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER (CONVERT / COPY_OUTOFLINE would make
  the conversion an out-of-line call -- retail does it INLINE here, so do NOT define _CONVERT; need an inline converting ctor).
- CThingFilter_AICreatureHasProperty.h (Property unsigned long +0, inline ctor), CThingCompare_Nearest.h (Pos, inline ctor from const C3DVector&),
  C3DVector.h, CCreatureAction_ShowFear.h (sizeof 0xA8 asserted), CCreatureActionBase.h (header debt: flattens vptr -- landed rows model it locally at 0xA8 with virtual dtor).
- CCreatureAction_SendMessage: no header; 009822e0 row models PDef +0xA8 / Pos +0xAC / PTarget CIntelligentPointer<CThingCreatureBase> +0xB8 / ExtraData +0xC0, sizeof 0xC4.
- Draft models LOCALLY although shared headers exist: C3DVector, CDefPointer (and fake CDefCounted), CBaseIntelligentPointer/CIntelligentPointer,
  CAIStateGroupBase, CAIBrain, CThingCompare_Nearest, the filter (as made-up CThingFilter_CreatureType), CCreatureAction_ShowFear (as AlertToPlayer).

## 5. Callee visibility
- Inline in retail here: CIntelligentPointer<CThingAICreature>(T*) ctor, CIntelligentPointer<const CThingCreatureBase> converting ctor
  (despite COMDAT 0x00982080 existing elsewhere), filter/compare ctors, CAIFilter_NoOtherYellForHelp ctor, GetCreature (PBrain->Creature),
  ~CCreatureAction_SendMessage and ~CCreatureAction_ShowFear (implicit; out-of-line 009822e0 is an unreferenced COMDAT), CDefPointer dtor/Release.
- Out of line: PeekWorld, PeekThingSearchTools (protected accessors), GetPBestLeafThing (same file neighbourhood 0x9823D0 -- template COMDAT),
  ShowFear ctor, SendMessage ctor, ~CCreatureActionBase, CBaseIntelligentPointer members, Flee, PerformActionAndWait.
- No STLport helpers called.

## 6. Siblings
- 00/98/CAIStateGroupBase_AIActionFleeToThingProximity_009804a0.cpp (RELOCATION_MATCH, hand277): FABLE_RETAIL_CINTELLIGENTPOINTER,
  local CAIStateGroupBase/CAIBrain/CAIFilterBase model, CIntelligentPointerConstCreature local struct, no pragmas, no Flags line.
- 00/98/CAIStateGroupBase_AIActionMoveToPossiblyBlockedObjectAndFace_00980770.cpp, 00/98/CAIStateGroupBase_AIActionSpeakAISpeechToTarget_009812c0.cpp:
  only FABLE_RETAIL_CINTELLIGENTPOINTER, no pragmas.
- 00/98/CCreatureActionSendMessage_CCreatureActionSendMessage_009822e0.cpp: FABLE_RETAIL_DEF_CLASS_BASE + engine/CDefPointer.h (default Release),
  FABLE_RETAIL_CINTELLIGENTPOINTER, local CCreatureActionBase {virtual ~; _body[0xa4]}, the correct SendMessage class -- copy this class model.
- 00/93/CAIStateGroupMinionReactToUnconsciousPerson_Update_009378f0.cpp (caller).

## 7. Draft model errors
- Wrong callee names/types (table above): GetPosition->PeekPos, GetPWorld->PeekWorld, AlertToPlayer->ShowFear(CThingAICreature&),
  CThingFilter_CreatureType->CThingFilter_AICreatureHasProperty, CThingObject**->CThingAICreature**, guardPointer type is
  CIntelligentPointer<CThingAICreature> (vt 0x0123E7E0; draft calls it "Creature" i.e. CThingCreatureBase), Flee/PerformActionAndWait
  take const& not pointers, SendMessage creature by reference and receiver CThingCreatureBase*.
- Message param type should be CDefPointer<const CMessageEventDef> from engine/CDefPointer.h (draft's custom dtor forms caused the
  "re-read/materialise" residue). The member PDef in SendMessage must be the same CDefPointer class (its inline Release gives the load/dec/store + null store).
- Scheduling residue (guard=0 early, filter=3 late, PeekWorld before creature load) likely follows from correct types: guard
  declared and zeroed first, GetCreature().PeekPos() evaluated as the compare temp, filter temp `CThingFilter_AICreatureHasProperty(3)`.
- Draft returns on `result != DONE` right after Flee -- matches retail (returns flee result). Final: bool `failed` then 3/2 -- matches.

## 0x00A44F40

# DOSSIER 0x00A44F40 C3DSkeletalMorphTarget::CombineMorph (525 bytes, ret 8)

## 1. Retail shape (xdis 00a44f40 0x21d)
- __thiscall, `ret 8` (target ref + float weight). NO frame pointer: `sub esp,58h ; push ebp ; mov ebp,[esp+60h]` -- ebp is a
  general register holding `&target` (default /O2 /Oy; no optimize("y",off)). No SEH prologue even though a local with a dtor
  (CCharString in the miss arm) lives across the insert call -> TU built without EH state here (same as the draft, 516 bytes, no EH).
- Frame 0x58 (offsets from esp after `sub`): +0x0 byte offset into target.BoneMorphs (t*0x14, strength-reduced), +0x4 t,
  +0x8..+0x13 the (1,1,1) C3DVector temp, +0x1c/+0x20 found-arm x87 spills, +0x34/+0x38 miss-arm spills, +0x44..+0x57 the local
  C3DSkeletalBoneMorph (BoneName +0x44, CRC +0x48, Scaling +0x4c). Retail does not share temp slots between arms.
- Registers: esi = &this->BoneMorphs (`lea esi,[ecx+4]`, set once after the empty test), edi = cursor i, ebx = &target.BoneMorphs[t]
  (ebx = target.first + offset, recomputed each outer iteration from [esp+0x10]), ebp = &target (re-loaded from the arg slot
  [esp+6Ch] only on the inner-loop exit path 0xA44FCC, because ebp is borrowed as the &BoneMorphs[i].BoneNameCRC walking pointer).
  The old residue "esi/edi swapped (cursor vs vector)" means the draft got esi=i, edi=&BoneMorphs.
- size() = (last-first) signed /20 (`imul 66666667h ; sar 3 ; shr 1Fh ; add`) recomputed at EVERY use, unsigned compares (jae/jb):
  STLport `size_type(_M_finish-_M_start)`.
- Outer loop: `je` skip when target.size()==0 (rotated for-loop), t and offset spilled; bottom: `offset+=0x14 ; ++t ; t < target.size()`.
- Inner: `while (i < size() && BoneMorphs[i].BoneNameCRC < src.BoneNameCRC) ++i;` -- pre-test then a pointer-walk loop (ebp += 0x14, inc edi).
- Hit test: `i < size() && (eax=&first[i])->BoneNameCRC == src.CRC` (eax address reused for the update -> element reference CSE).
- Hit arm: x=(src.X-1.0f), y, z each `fsub [0x0122DED8]` (1.0f constant), z spilled, `fmul [weight]`, then `fld delta ; fadd [eax+8] ;
  fstp [eax+8]` per axis -> `BoneMorphs[i].Scaling += (src.Scaling - C3DVector(1,1,1)) * weight;`. Then jmp to loop bottom.
- Miss arm order (load-bearing): `call CCharString::CCharString() 0x0099E4B0` on [morph]; `mov [morph.CRC],0` (DEAD store KEPT in
  retail -- the draft lost it); `morph.CRC = src.CRC`; then per axis `mov [tmp.k],3F800000h ; mov reg,[tmp.k] ; mov [morph.Scaling.k],reg`
  (a constructed C3DVector(1,1,1) temp copied by dword moves, interleaved per component); then the delta math interleaved with the
  insert's pushes (`push &morph ; push &first[i]` (`mov ecx,[esi]` = begin()), `mov ecx,esi`); in-place `fld delta.k ; fadd morph.k ;
  fstp morph.k`; `fstp st(0)` x2; `call 0x00A46B50` (return ignored); `lea ecx,morph ; inc edi ; call ~CCharString 0x0099EAE0`.
  So ++i is between insert and the dtor (statement `++i;` inside the block, before the scope end).

## 2. Name / class / signature  (manifest label is CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv:34312) `?CombineMorph@C3DSkeletalMorphTarget@@QAEXABV1@M@Z` @ Ego_r 0x00BA0000, which sits in
  the same TU order as retail: CompareCRCs b9fc90/a44c90, GetBoneScaling b9fcd0/a44cd0, BoneMorph ctor b9fdf0/a44df0, Target ctor
  b9fe30/a44e60, CombineMorph ba0000/a44f40.
- FableWin.pdb (fablewin_pdb_names / FableWin.exe.signatures.tsv) same mangled name @ 0x0321CE20.
- BSim port audit ghidra_out/bsim_port_audit_05final.tsv: a44f40 -> ?CombineMorph@C3DSkeletalMorphTarget@@QAEXABV1@M@Z (sim 0.68, true).
- Mangled leaf: `?CombineMorph@C3DSkeletalMorphTarget@@QAEXABV1@M@Z` = `void C3DSkeletalMorphTarget::CombineMorph(const C3DSkeletalMorphTarget&, float)`,
  non-const, public, thiscall.
- Only caller: C3DSkeletalMorph::PeekCombinedMorph 0x00A45260 (call 0x00A452FA), landed at
  rebuild/src/compiled/00/a4/C3DSkeletalMorph_PeekCombinedMorph_00a45260.cpp (declares the same signature).
- Not virtual (no vtable entry).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x0099E4B0 | rebuild/src/compiled/00/99/CCharString_CCharString_0099e4b0.cpp | CCharString::CCharString() | ??0CCharString@@QAE@XZ | ok (shared fable_string.h declares it) |
| 0x0099EAE0 | rebuild/src/compiled/00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString() | ??1CCharString@@QAE@XZ | ok |
| 0x00A46B50 | rebuild/src/compiled/00/a4/stdvector_insert_00a46b50.cpp | std::vector<C3DSkeletalBoneMorph>::insert(iterator,const T&) | ?insert@?$vector@VC3DSkeletalBoneMorph@@... (Ego_r) | draft used a local CBoneMorphArray::insert; real owner is STLport vector (CArray<C3DSkeletalBoneMorph> derives from it). Catalog name "Vector_Insert_CParticleMorphEntry" is wrong/descriptive. |
- Data: 1.0f @ 0x0122DED8 (fsub operand; comes from C3DVector(1.0f,1.0f,1.0f) folded literal).

## 4. Members touched
- this+0x04 / target+0x04,+0x08: C3DSkeletalMorphTarget::BoneMorphs, PDB type `CArray<C3DSkeletalBoneMorph>` (struct_layouts_egor.tsv:
  C3DSkeletalMorphTarget Name +0 CCharString, BoneMorphs +4; retail object 0x10 since STLport vector is 12 bytes).
  Shared header: rebuild/include/engine/CArray.h (`template<class T> class CArray : public std::vector<T>`, stlport/fable_containers.h).
  Opt-ins: FABLE_RETAIL_CARRAY (operator[] as `*(begin()+i)`, else `_M_start[i]`), plus vector's own size()/operator[]
  (`*(begin()+n)`), FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE (declaration-only insert, used by CEnvironment_SetWeightedThemes_006b3e60.cpp)
  -- REQUIRED here, retail calls insert out of line.
- element+0x00 BoneName CCharString, +0x04 BoneNameCRC ulong, +0x08 Scaling C3DVector (struct_layouts_egor.tsv C3DSkeletalBoneMorph, 20 bytes).
  NO shared header for C3DSkeletalBoneMorph or C3DSkeletalMorphTarget (rebuild/include/engine has only C3DVector.h, CArray.h): local
  definitions are legitimate, but model them with the real names and include the shared ones: engine/C3DVector.h, fable_string.h
  (CCharString), engine/CArray.h.
- C3DVector.h: default ctor empty, (x,y,z) ctor, copy ctor user-defined `:X(o.X),Y(o.Y),Z(o.Z)` (default path), operator- / operator*
  / operator+= are __forceinline. Opt-ins: FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP, FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN,
  FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP (no-code paren tuples that move VC7.1's scheduling cut -- candidates for the interleave residue).
- Classes the old draft modelled LOCALLY that have shared headers: C3DVector (engine/C3DVector.h), CCharString (fable_string.h),
  CBoneMorphArray (= CArray<C3DSkeletalBoneMorph>, engine/CArray.h + stlport vector).

## 5. Callee visibility
- CCharString ctor/dtor: out of line (fable_string.h declarations), fine.
- vector::insert 0x00A46B50: out of line (only caller is this function) -> FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE; c2 then sees
  only a declaration (it may keep the address of `morph`, which could be why the CRC=0 store is not dead-eliminated in retail; the
  draft's local struct declared insert as an opaque member too, so test the real STLport vector path + this macro).
- Inline (no out-of-line retail copy expected): C3DSkeletalBoneMorph::C3DSkeletalBoneMorph() (FableWin ??0C3DSkeletalBoneMorph@@QAE@XZ
  @0x0321D1A0, right after CombineMorph 0x0321CE20 in FableWin -> defined in the same .cpp), GetNameCRC (FableWin 0x0321D310,
  ?GetNameCRC@C3DSkeletalBoneMorph@@QBEKXZ), GetScaling (?GetScaling@...@@QBEABVC3DVector@@XZ), SetScaling
  (?SetScaling@C3DSkeletalBoneMorph@@QAEXABVC3DVector@@@Z), GetBoneMorphs (?GetBoneMorphs@C3DSkeletalMorphTarget@@QBEABV?$CArray@...@@XZ).
  Retail's expanded default ctor body = `CCharString()` + `BoneNameCRC = 0` (Scaling left unconstructed: C3DVector() {}).
- Same TU, defined earlier (retail order): CompareCRCs 0x00A44C90, GetBoneScaling 0x00A44CD0, C3DSkeletalBoneMorph(const CCharString&,
  const C3DVector&) 0x00A44DF0, C3DSkeletalMorphTarget() 0x00A44E60; Initialise 0x00A45150 follows.

## 6. Sibling landed rows (rebuild/src/compiled/00/a4/)
- C3DSkeletalMorphTarget_GetBoneScaling_00a44cd0.cpp: no Flags/pragma; includes engine/C3DVector.h; local CCharString/C3DSkeletalBoneMorph
  (real member names) and CBoneMorphArray; `scaling = C3DVector(1.0f,1.0f,1.0f);` miss path copies a temp (three frame words) -- same idiom as here.
- C3DSkeletalBoneMorph_C3DSkeletalBoneMorph_00a44df0.cpp: no flags; engine/C3DVector.h; inline GetCRC/c_str/Length.
- C3DSkeletalMorph_PeekCombinedMorph_00a45260.cpp: engine/CCountedPointer.h; `size()` as `(unsigned long)(last-first)`, operator[] as `*(begin()+n)`.
- C3DSkeletalMorphTarget_Initialise_00a45150.cpp: fable_compiler.h + fable_string.h, CCharString from the shared header.
- stdvector_insert_00a46b50.cpp: `#pragma auto_inline(off/on)` around helpers, engine/C3DVector.h.
- Default lane flags (no // Flags: line) -- every C3DSkeletal* member row uses them; /Gr appears only on STL helper rows.

## 7. Old draft model errors (draft_original.cpp)
- Local C3DVector/CCharString/CBoneMorphArray instead of shared headers (verifier warns). CBoneMorphArray::insert should be
  std::vector<C3DSkeletalBoneMorph>::insert via CArray + FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE.
- Default ctor `C3DSkeletalBoneMorph() : BoneNameCRC(0) {}` -> its zero store was dead-eliminated by c2 in the draft but kept in retail;
  candidates: the escaping-address effect of the real vector insert declaration, the default ctor defined out-of-class/inline in the
  .cpp, or the CRC written through a separate statement order. NOTES' suggestion: "keep the CRC in a named member written before the insert".
- Register swap esi/edi: retail keeps &BoneMorphs in esi (used for begin()/size()/insert this) and i in edi; try a reference local
  (`CArray<C3DSkeletalBoneMorph>& morphs = BoneMorphs;`) or the real vector type (different use weights).
- Control flow, loop forms, signed/unsigned size, hit/miss arms and the (1,1,1)-then-+= miss arm already match the retail shape;
  residue is register allocation, the dead CRC store, and per-axis interleaving of int/float stores in the miss arm.
- Grade note: draft header comment says 528 bytes; true body is 525 (0x20D... ret at 0xA4514A, +3 int3 pad).

## 0x00D38D20

# DOSSIER 0x00D38D20 NScript::CQ_FireHeartScript::RunVillagerComments (526 bytes)

## 1. Retail shape (xdis 00d38d20 0x21e)
- __thiscall (this in ESI from ECX), plain `ret` (no args), no frame pointer (`sub esp,0x14; push ebx/ebp/esi/edi`), no SEH -> speed TU, no /Oy-, no optimize("s").
- Locals: [esp+0x10] count (spilled), [esp+0x14] CTimer.TimerIndex (also kept in EBP), [esp+0x18] vector<long> candidates (_M_start; 12 bytes at +0x18..+0x23).
- CTimer ctor/dtor/operator=/GetTime all EXPANDED (direct slot 87/88/89/90 calls through [0x0143E8F8]); no call to 0x00CD4450/0x00CD4470.
- vector<long>(n) called OUT OF LINE (0x00D38F30, `lea ecx,[esp+0x18]`, push NumberOfVillagers); its dtor is expanded at each exit (`if (p) free(p)` = _Vector_base dtor/allocator::deallocate).
- Three exit tails: 0x00D38EC6 (normal fall-through + some returns), 0x00D38EEE (other returns), 0x00D38F16 (the return inside the in-range `if`, which reloads TimerIndex from [esp+0x14] because EBP then holds &Villagers[i], and `jmp 0x00D38ED8` into the first tail's Deregister call).
  Returns -> EC6: inside timer-wait loop, after NewScriptFrame at top of do-loop, after the for loop, after the ShouldVillagerSpeak store. Returns -> EEE: after first NewScriptFrame, after timer-wait, top of for body, before rand(), and the final post-loop check (0x00D38EBB).
- Compare: GetSquaredDistanceBetweenThings result `fcomp [0x012C26F0]` (= 25.0f) ; `test ah,0x41 ; jp skip` => `if (d <= 25.0f)`.
- For loop: i in EDI, i*12 strength-reduced in EBX (`add ebx,0xc`), `lea ebp,[ebx+eax]` with eax=Villagers._M_start loaded BEFORE the GetHero vcall (right-to-left arg eval: Villagers[i] first, held in EBP); loop is `if (NumberOfVillagers>0) do {...} while (++i < NumberOfVillagers)` re-reading [esi+0x84] each iteration.
- Store: `ShouldVillagerSpeak._M_start[candidates[rand()%count]] = 1` (byte store => vector<bool> is a plain byte vector; `cdq; idiv ebx`).

## 2. Identity (proven, two sources)
- Leaf: `?RunVillagerComments@CQ_FireHeartScript@NScript@@QAEXXZ` -> `void NScript::CQ_FireHeartScript::RunVillagerComments()`, public non-virtual, no params.
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00D47020), ghidra_out/fablewin_pdb_names.tsv / _fablewin_api.tsv (FableWin 0x036FD710), naming_stragglers/FableWin.exe.signatures.tsv BSim match d38d20 (0.73); ghidra_out/labels_landing_2026-11-05y.tsv: spawned by name "RunVillagerComments" from Main 0x00D35090 (landed). xref: only data/abs ref from 0x00D357EF (inside Main, the CSpawnedFunc registration).
- Manifest label CORRECT. Declared in rebuild/include/engine/CQ_FireHeartScript.h:45 (`void RunVillagerComments(); // 0x00D38D20`).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00D38F30 | rebuild/src/compiled/00/d3/STLvectorlong_vector_00d38f30.cpp | std::vector<long>::vector(unsigned) | ??0?$vector@J... (STLport sized ctor) | ok (local fake vector) |
| 0x00CB7940 | rebuild/src/compiled/00/cb/CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating | Ego_r CScriptBase (global ns, NOT NScript) | draft puts CScriptBase in namespace NScript -- WRONG; use engine/CScriptBase.h |
| 0x00CBE583 | rebuild/src/compiled/00/cb/global_GetSquaredDistanceBetweenThings_00cbe583.cpp | GetSquaredDistanceBetweenThings | ?GetSquaredDistanceBetweenThings@@YIMABVCScriptThing@@0@Z | draft says GetDistanceBetweenThings -- WRONG (no fsqrt; 25.0f is squared => within 5 units; labels note chunk 654). Not in any shared header: declare `float FABLE_FASTCALL GetSquaredDistanceBetweenThings(const CScriptThing&, const CScriptThing&);` |
| 0x00BFEB16 | rebuild/src/compiled/00/bf/global_rand_00bfeb16.cpp | rand | CRT | ok (<stdlib.h>, as 00d368d0) |
| 0x00BFEA14 | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free | CRT | ok (via allocator::deallocate) |
| vslot 7 +0x1C on Game (+0x40) | CGameScriptInterfaceBase.h:30 | NewScriptFrame() const | | ok |
| vslot 70 +0x118 on Game | CGameScriptInterfaceBase.h:93 | `CScriptThing& GetHero()` | | ok |
| vslots 87/88/89/90 on pDSTGame 0x0143E8F8 | CGameScriptInterfaceBase.h:110-113 | RegisterTimer / DeregisterTimer(long) / SetTimer(long,long) / GetTimer(long) const | | draft global `g_pGameScriptInterface` -- use established `extern CGameScriptInterfaceBase* pDSTGame; // 0x0143E8F8` (272 rows) |
| CTimer | rebuild/include/engine/CTimer.h | CTimer(), ~CTimer(), operator=(long), GetTime() const, member TimerIndex | Ego_r ??4CTimer@@QAEAAV0@J@Z | draft's `SetTime`/`Index` are invented: use `timer = 25;` and `timer.GetTime()` |

## 4. Members (this = CQ_FireHeartScript, header rebuild/include/engine/CQ_FireHeartScript.h, PDB struct_layouts_egor.tsv)
- +0x40 Game (CLevelScriptBase, CGameScriptInterfaceBase*) ; +0x49 `bool MissionSucceeded` ; +0x84 `long NumberOfVillagers` ; +0xEC `std::vector<bool> ShouldVillagerSpeak` ; +0x110 `std::vector<CScriptThing> Villagers`. All named in the header with retail offsets.
- Header contract: define FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_BASE_OBJECT (+ FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CLEVELSCRIPTBASE as siblings do), include stlport/fable_containers.h and stlport/fable_map.h first. Opt-ins in it: only FABLE_RETAIL_CQ_FIREHEARTSCRIPT_TRIGGERPROPHETCOMMENT (not needed).
- Classes the draft models LOCALLY that have shared headers: CScriptThing (engine/CScriptThing.h), vector (stlport/fable_containers.h), CGameScriptInterface(Base) (engine/CGameScriptInterfaceBase.h), CTimer (engine/CTimer.h), CScriptBase (engine/CScriptBase.h), CQ_FireHeartScript (engine/CQ_FireHeartScript.h).

## 5. Visibility / opt-ins
- vector<long>(size_type) out of line: `#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE` (fable_containers.h:384, declaration only => call 0x00D38F30). Applies to every vector in the TU, but only candidates uses the n-ctor here.
- vector dtor inline (default `~vector(){_Destroy}` + `~_Vector_base` free) -- matches the expanded `if(p) free(p)`.
- CTimer: define after including CTimer.h, exactly as rebuild/src/compiled/00/d7/NScriptCQHangingTreeGoodScript_SpawnEnemies_00d736b0.cpp:56-59:
  `inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }` (or `: TimerIndex(pDSTGame->RegisterTimer())`, 104 rows), `inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }`, `FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) {...}`, `FABLE_FORCEINLINE long CTimer::GetTime() const {...}`.
- vector::operator[] = *(begin()+n) inline in fable_containers.h:464.
- CScriptBase::IsActiveThreadTerminating and GetSquaredDistanceBetweenThings are called (not inline).

## 6. Sibling landed rows (same class / TU 0x00D34xxx-0x00D3Bxxx)
- rebuild/src/compiled/00/d3/NScriptCQFireHeartScript_Main_00d35090.cpp (spawner): defines FABLE_RETAIL_CCHARSTRING, _NSCRIPT, _SUSPENDABLE_PROCESS, _BASE_OBJECT, _CLEVELSCRIPTBASE, _DEF_CLASS_BASE, _CDEFSTRINGTABLE; includes fable_containers.h, fable_string.h, CCountedPointer.h, CScriptThing.h, CGameScriptInterfaceBase.h, CQ_SunnyvaleMasterData.h, CEntityScriptBase.h, fable_map.h, CQ_FireHeartScript.h. No pragmas.
- rebuild/src/compiled/00/d3/NScriptCQFireHeartScript_StartRound_00d39960.cpp: same set + CTimer.h, TRIGGERPROPHETCOMMENT opt-in.
- rebuild/src/compiled/00/d3/NScriptCQFireHeartScriptCFHGuildMaster_Main_00d368d0.cpp: uses CTimer (operator=/GetTime FABLE_FORCEINLINE over pDSTGame), rand via <stdlib.h>, fable_script_distance.h, _STLP_MAP opt-in.
- rebuild/src/compiled/00/d3/NScriptCQFireHeartScript_PlayCutscene_00d39330.cpp: same header family, fable_compiler.h first.
- rebuild/src/compiled/00/d7/NScriptCQHangingTreeGoodScript_SpawnEnemies_00d736b0.cpp: CTimer inline-definitions idiom + vector.
- None of them uses a TU pragma; whole family is speed-optimised.

## 7. Old draft model errors
- Wrong callee name (GetDistanceBetweenThings -> GetSquaredDistanceBetweenThings) and wrong comment ("within 25" -> squared 25, i.e. within 5).
- All local models (fake vector, CGameScriptInterface with vNNN padding slots, CTimer with invented SetTime/Index, NScript::CScriptBase with _body pad, local CQ_FireHeartScript) must become shared headers.
- Control flow mostly right (outer while(!MissionSucceeded), rotated timer-wait while, do{...}while(count<=0), for over NumberOfVillagers, final IsActive check). Residue from hand295: DIFFER 529v526 = tail block order (retail: normal exit tail EC6 first, then return copy EEE, then the reload-timer copy F16) and `lea ebp,[ebx+eax]` vs `mov ebp,[esi+0x110]; add ebp,ebx`. The NOTES' untried ideas: top-tested `for(;;){ if (MissionSucceeded) break; ...}` and `Villagers.begin() + i` / real std::vector operator[] (the header's operator[] is `*(begin()+n)`, which may itself change the addressing). Note retail evaluates `Villagers[i]` before the GetHero vcall (fastcall right-to-left) - keep `GetSquaredDistanceBetweenThings(Game->GetHero(), Villagers[i])`.
- Retail's vector<bool> is a plain byte vector: header std::vector<bool> (no bit specialisation in fable_containers.h) gives the byte store.

## 0x00704860

# DOSSIER 0x00704860 -- CGameScriptThingManager::CreateScriptThing (531 B)

## 1. Retail shape (xdis 00704860 0x223)
- __thiscall, `ret 8` = hidden result pointer ([esp+34h] after prologue) + one arg `thing` ([esp+38h]). Returns CScriptThing by value (12 B: vptr 0x01238C8C, PImp.Data, PImp.Info).
- NO frame pointer (`sub esp,20h; push ebx,ebp,esi,edi`), no SEH, plain /O2 (no optimize("s") idioms). Siblings at 0x007046xx-0x00704Axx use no pragmas / no `// Flags:`.
- Frame (after pushes): +10h `this` spill, +14h..+1Fh local CScriptThing (both paths), **+20h..+27h never touched**, +28h..+2Fh CCountedPointer<CScriptThing> (not-found path; zeroed at entry together with the local CScriptThing).
- Loop: uid halves preloaded into ecx/edx (thing+18h/+1Ch), `lea ecx,[ecx]` alignment pad, node+8/+0Ch compared, `je` to found block at 0x7048E0. This is the inlined protected `FindScriptThing(const CThing&, CScriptThingInfo**)` (Ego_r 0x00724B40) or an equivalent iterator loop.
- Block order: loop exit -> not-found head (CScriptThing default ctor inlined: call CBaseClass ctor 0x0099A2D0, vptr, zero PImp, zero +28h/+2Ch; `new(0x48)`; null-check; CGameScriptThing ctor) -> `jmp 0x704988`; found block (0x7048E0..0x704983, own epilogue); then 0x704986 `xor eax,eax` (new failed) / 0x704988 rest of not-found (Reset 0x7045F0, list push_back inlined: malloc(0x18), placement-new null check `cmp ecx,esi`, copy {uid from thing+18h, Data, Info+addref}, link before sentinel: node->next=head, node->prev=head->prev, head->prev->next=node, head->prev=node).
- Found path: `[node+10h]->vtbl[0x11C/4=71]()` = **UpdateThingAttachment** (CScriptThing slot 71, 0x004AB0E0), then local CScriptThing default-constructed, by-value copy of node's PScriptThing built in arg area (`sub esp,8; mov eax,esp`, addref), `SetImplementation` 0x007046D0 on the local, copy local -> hidden result (inlined copy ctor 0x004ABE90: vptr, Data, Info, addref), then local destroyed (vptr store, release with re-read of Info `dec [eax]; mov eax,[esp+1Ch]; cmp [eax],0`, zero Data/Info, call ~CBaseClass 0x0099A2E0). VC7.1 has no NRVO -> `CScriptThing st; st.SetImplementation(...); return st;` is right.
- Not-found tail: after SetImplementation the counted pointer is released from a REGISTER (esi = Info loaded once after Reset, kept across malloc and SetImplementation; `dec [esi]; jne` no re-read, no zero stores), then local CScriptThing destroyed (re-read + zero + ~CBaseClass). Zero register: esi in not-found head, edi in found path, ebx in not-found tail.

## 2. Name / signature (two sources)
- `?CreateScriptThing@CGameScriptThingManager@@QAE?AVCScriptThing@@ABVCThing@@@Z` -- Ego_r.pdb (egor_pdb_names.tsv 0x00724BC0, bsim_port_audit_05final.tsv maps retail 0x704860 to it) + FableWin.pdb (_fablewin_api.tsv 0x01798100). Landed caller rebuild/src/compiled/00/4c/CThing_IsUnderScriptSupervision_004c7e90.cpp declares exactly `CScriptThing CreateScriptThing(const CThing&);`.
- Manifest label is RIGHT. Draft signature is WRONG: param must be `const CThing& thing` (draft `CThing*` mangles PAV). Public, non-const member, returns CScriptThing by value.
- Ego_r order: ShareData<CScriptThing> 724A50, SetImplementation 724AA0, GetNullScriptThing 724B20, FindScriptThing 724B40 (protected, IAE), HasScriptThing 724B80, CreateScriptThing 724BC0, FrameUpdate 724DF0, ctor 724E90, list::_Buynode 724F80. Retail: ShareData 7044D0, ..., Reset 7045F0, SetImplementation 7046D0, GetNull 704730, Has 704790, FrameUpdate 7047D0, Create 704860, ctor 704A80 => same TU (GameScriptThingManager.cpp); FindScriptThing has no retail body (inlined).
- Callers: 0x004C7E5F in 0x004C7E50 = `CThing::GetScriptThing() const` (?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ per the IsUnderScriptSupervision row header) -- LANDED UNDER A BAD NAME `Ctor_CPredicate_And_NScript` in rebuild/src/compiled/00/4c/CThing_GetScriptThing_004c7e50.cpp (NAME_FIXES candidate); 0x004C7EC3 in CThing::IsUnderScriptSupervision 0x004C7E90 (landed).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass | ??0CBaseClass | ok (via inline CScriptThing() 0x006E7B40) |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | ~CBaseClass | ??1CBaseClass@@UAE@XZ | ok |
| 0x00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new | ??2@YAPAXI@Z | ok |
| 0x008D0250 | 00/8d/CGameScriptThing_CGameScriptThing_008d0250.cpp | CGameScriptThing(CWorld*, CThing*) | ??0CGameScriptThing@@AAE@AAVCWorld@@ABVCThing@@@Z (PRIVATE, CWorld&, const CThing&) | WRONG: draft (CWorld*, CThing*) public; landed row also uses pointer spelling |
| vtbl+0x11C | 00/4a/CScriptThing_UpdateThingAttachment_004ab0e0.cpp | CScriptThing::UpdateThingAttachment (slot 71) | ?UpdateThingAttachment@CScriptThing@@UAEXXZ | WRONG: draft `Refresh` |
| 0x007045F0 | 00/70/CCountedPointer007044b0_Reset_007045f0.cpp | CCountedPointer_007044b0::Reset | almost certainly ?Reset@?$CCountedPointer@VCScriptThing@@@@QAEXPAVCScriptThing@@@Z (sits in the CCountedPointer<CScriptThing> block 7044B0..7046D0; deleter 0x7044B0 = DeleteData<CScriptThing>) | draft `CCountedPointer<CGameScriptThing>::Reset` -- type wrong: PDB PScriptThing is CCountedPointer<CScriptThing> |
| 0x007046D0 | 00/70/CMeshDataBank007046d0_SetAnimationMemoryPool_007046d0.cpp | CMeshDataBank_007046d0::SetAnimationMemoryPool (BAD propagated label) | ?SetImplementation@CScriptThing@@QAEXV?$CCountedPointer@VCScriptThing@@@@@Z (Ego_r 724AA0, FableWin; declared in engine/CScriptThing.h) | WRONG: draft `SetImp`; NAME_FIXES for landed row |
| 0x00BFEA0E | 00/bf/global_malloc_00bfea0e.cpp | malloc | malloc (STLport node allocator, 0x18 B node) | ok |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | ??3@YAXPAX@Z | ok |
Global 0x013B86A0 = `CMainGameComponent::PMainGameComponent` (protected static) read through inline `CMainGameComponent::Get()` -- spelling established in landed ctor 00/70/CGameScriptThingManager_CGameScriptThingManager_00704a80.cpp; draft's `g_pMainGameComponent` is the old stand-in.

## 4. Members
- this+0x00: `ScriptThings` list<CGameScriptThingManager::CScriptThingInfo> (PDB 12 B; retail STLport = sentinel node pointer only, 4 B); this+0x04 `NullThing` (unused here). struct_layouts_egor.tsv CGameScriptThingManager (24 B). NO engine/CGameScriptThingManager.h exists -- every landed sibling models it locally.
- CScriptThingInfo (nested, 16 B): +0 `ulong64 ThingUID`, +8 `CCountedPointer<CScriptThing> PScriptThing` (Data +8, Info +0xC) -> node +8/+0Ch/+10h/+14h. FableWin has default ctor ??0CScriptThingInfo@CGameScriptThingManager@@QAE@XZ.
- thing+0x18: CThing::UniqueID (ulong64) -- engine/CThing.h (line 64; the IsUnderScriptSupervision row uses engine/_quarantine/CThing.h via `#define CThing CThingLayout`).
- CMainGameComponent+0x24: `PWorld` boost::scoped_ptr<CWorld> -- engine/CMainGameComponent.h (`unsigned char PWorld[4]` by default; real scoped_ptr only under FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES; FABLE_RETAIL_CMAINGAMECOMPONENT(_WORLD) exist). CGameScriptThing ctor wants `CWorld&` -> `*PWorld`.
- CScriptThing (12 B): engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (vptr 0x01238C8C, CBaseClass base, PImp +4; SetImplementation, slot 71 UpdateThingAttachment declared; opt-ins FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (retail INLINES copy ctor 0x004ABE90 here -> use it), _IMPLICIT_ASSIGNMENT, _ORIGINAL_ACCESS; contract: includer defines CCountedPointer<T> first, and dtor `inline CScriptThing::~CScriptThing() {}` defined out of class (IsUnderScriptSupervision row does FABLE_FORCEINLINE). Default ctor 0x006E7B40 is declared only -> retail inlines it (CBaseClass call + vptr + zeros), so the row must supply an inline body or opt-in.
- CGameScriptThing (0x48): engine/CGameScriptThing.h under FABLE_RETAIL_CGAMESCRIPTTHING (+FABLE_RETAIL_NSCRIPT); private ctors (CWorld&) / (CWorld&, const CThing&) -> manager must be friend (header has no friend decl; check).
- CCountedPointer<T>: engine/CCountedPointer.h; FABLE_RETAIL_CCOUNTEDPOINTER (Reset body / ctor from T*), _RELEASEDATA, _RELEASEDATA_DECL_ONLY, _DTOR_DECL_ONLY, _VISIBLE_CALLED_CTOR/_DTOR, _RESET_CALLS_DTOR, _RESET_CAMERA_DECL (Reset declaration only -- retail CALLS Reset 0x7045F0 here, so it must not be inlined).
- Classes the draft models LOCALLY that have shared headers: CThing, CGameScriptThing, CScriptThing, CBaseClass, CMainGameComponent, CCountedPointer (as non-template), CCPPointerInfo (as SCountBlock). (CScriptPImp is a fake second counted-pointer type -- PImp is CCountedPointer<CScriptThing>.)

## 5. Callee visibility
- Same TU defined earlier: CCountedPointer<CScriptThing>::ShareData 0x7044D0, operator= 0x704580, Reset 0x7045F0, SetImplementation 0x7046D0 -- all CALLED out of line here (bodies visible to retail compiler; a visible-but-noinline body may change c2's alias assumptions, cf. the esi-kept Info in the not-found tail).
- Inlined: CScriptThing() 0x006E7B40, CScriptThing copy ctor 0x004ABE90, ~CScriptThing (inline), CCountedPointer copy ctor/dtor, FindScriptThing (protected, Ego_r out of line), list push_back/_M_create_node/_Construct (STLport, malloc allocator).
- Out-of-line: CBaseClass ctor/dtor, CGameScriptThing ctor (other TU), UpdateThingAttachment (virtual).
- STLport opt-ins in rebuild/include/stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST, _LIST_PUSHBACK_NOINLINE, _LIST_INSERT_OUTOFLINE, _LIST_CONSTRUCT_HELPER, _LIST_BASE_OUTOFLINE (retail inlines push_back here, so none of the NOINLINE/OUTOFLINE ones apply). _Construct is `new(p) T(v)` (null-check emitted = retail's `cmp ecx,esi`).

## 6. Siblings (all rebuild/src/compiled/00/70/, no pragmas/Flags)
- CGameScriptThingManager_GetNullScriptThing_00704730.cpp (local CScriptThing model, inline copy ctor)
- CGameScriptThingManager_HasScriptThing_00704790.cpp (uid loop shape; local models)
- CGameScriptThingManager_FrameUpdate_007047d0.cpp (STLport list model: _List_node {next, prev, data}, free 0x00BFEA14)
- CGameScriptThingManager_CGameScriptThingManager_00704a80.cpp (CMainGameComponent::Get(), named CCountedPointer local, by-value arg built in arg area, release after call)
- CMeshDataBank007046d0_SetAnimationMemoryPool_007046d0.cpp (= SetImplementation body; uses fable_compiler.h FABLE_FORCEINLINE)
- Engine-header idiom: 00/4c/CThing_IsUnderScriptSupervision_004c7e90.cpp (FABLE_RETAIL_NSCRIPT + CCountedPointer.h + CScriptThing.h).

## 7. Old draft model errors
1. Frame: the 8 unused bytes at +20h sit exactly where a 16-B `CScriptThingInfo` would start if the counted pointer at +28h is its `PScriptThing` (+8). Leading hypothesis: retail source builds a NAMED `CScriptThingInfo info;` (default ctor, PDB/FableWin), `info.ThingUID = thing.UniqueID` (store eliminated / uid reloaded from thing+18h), `info.PScriptThing.Reset(new CGameScriptThing(world, thing));`, `ScriptThings.push_back(info);`, `scriptThing.SetImplementation(info.PScriptThing);`. Draft used a bare CCountedPointer local + by-value uid PushBack -> no slot (538v531). Alt: two 4-B iterator/`CScriptThingInfo* found` locals of the inlined FindScriptThing keeping slots.
2. Declaration order: CScriptThing local constructed BEFORE `new` and destroyed LAST -> declare `CScriptThing scriptThing;` before the info/pointer; counted pointer destroyed first (from register).
3. Types: PScriptThing is CCountedPointer<CScriptThing> (not <CGameScriptThing>); slot 71 is UpdateThingAttachment; PImp is CCountedPointer<CScriptThing> (no separate CScriptPImp type); ctor takes CWorld& and const CThing&; param is const CThing&.
4. Found path is likely `CScriptThingInfo* info; if (FindScriptThing(thing, &info)) { info->PScriptThing->UpdateThingAttachment(); CScriptThing st; st.SetImplementation(info->PScriptThing); return st; }` -- draft's direct loop already matched it, keep the form.
5. Names: g_pMainGameComponent -> CMainGameComponent::Get(); SetImp -> SetImplementation; Refresh -> UpdateThingAttachment; PushBack -> STLport push_back.

## 0x00A5B370

# DOSSIER 0x00A5B370 CCachedFile::Read (534 bytes)

## 1. Retail shape (xdis 00a5b370 0x226)
- __thiscall virtual, `ret 0Ch` (3 args). NO frame pointer (no push ebp/mov ebp,esp: ebp is a general reg = destination). NO SEH/EH frame
  (no /EHsc: message locals never destroyed on throw paths, same as landed siblings). `push ecx` = one 4-byte frame word.
- /O2 not /O1: loop head 0xa5b4d0 is aligned with `jmp +3 ; lea ecx,[ecx]` padding (sibling CDiskFileWin32::Read notes "Opt: /O2").
- Shrink-wrapped pushes: ecx, ebp, esi at entry; `push ebx` only after the null-dst throw block (0xa5b3df); `push edi` only after size!=0 (0xa5b3f1).
- Register map: esi=this, ebp=destination (set at entry, `mov ebp,eax`), ebx=size (loaded 0xa5b3e0 `mov ebx,[esp+18h]`), edi=Position cached before GetLength.
- Flow: (a) dst==0 -> CCharString("",-1)+"Null dst parameter" (0x129A850), throw type 1. (b) `test ebx,ebx ; je exit` (size==0 return).
  (c) edi=Position; GetLength (slot 9 +0x24); `Position+size > len` (signed jle skips) -> "Cannot read past end of file" (0x129A830), type 3.
  (d) if(!BufferValid +0x25): BufferStartPosition(+8)=edi(cached Position); PDiskFile->SetPosition(edi) slot5; ebx=GetLength() slot9 FIRST, then GetPosition() slot7;
      remaining=len-pos -> stored in dead dst slot; bufsize=(long)(DataBuffer end-start) in frame word; Min(remaining,bufsize) via `cmp ; lea &remaining ; jl ; lea &bufsize` (reference Min, a<b?a:b);
      PDiskFile->Read(DataBuffer start (reloaded [esi+0Ch]), min, process) slot3; BufferValid=1 (after Read); `mov ebx,[esp+1Ch]` reloads size.
  (e) `mov eax,[size slot] ; test ; jle exit` (size reread from memory, ebx clobbered on the refill path) ; loop:
      top 0xa5b4d0: (unsigned)(Position-BufStart) < (end-start) `jae` -> refill test; inner 0xa5b4e4: Position-BufStart `js` out; size>0 `jle` -> straight to exit;
      copy `dl=[ (Pos-BufStart) + start ] ; [ebp]=dl` ; inc ebp ; Position reloaded/incremented/stored (`mov edi,[esi+4] ; inc edi ; mov [esi+4],edi`) ; dec ebx ;
      bottom recheck of the first clause only `jb 0xa5b4e4`.
      after inner: `test ebx ; jle exit`; refill #2: BufStart=Position(fresh load); SetPosition(Position); edi=PDiskFile (cached across both calls);
      GetPosition() slot7 FIRST (saved in dead size slot) then GetLength() slot9; remaining=len-pos (into size slot), bufsize into dst slot; Min; Read(start,min,process);
      NO BufferValid store; unconditional `jmp 0xa5b4d0` (no re-test of size at the top).
- Throw path temporaries: "" ctor and by-value copy in dead size slot, operator+ result (message) in dead process slot (first throw) / frame words, exception object in dead dst slot (first throw) ;
  type store `mov [..],1/3` comes AFTER copy+operator const char*+~copy (consistent with by-value ctor body).

## 2. Identity (manifest label is CORRECT)
- Leaf `?Read@CCachedFile@@UAEXPAXJPAVCASuspendableProcess@@@Z` = `virtual void CCachedFile::Read(void* destination, long size, CASuspendableProcess* process)` (non-const, void).
- Source 1: Ego_r.pdb egor_pdb_names.tsv 0x00b9ae20 (in donor order Close/Write/SetPosition/MovePosition/SetLength/GetLength/OpenFromFile/Open/Read).
- Source 2: FableWin.pdb _fablewin_api.tsv / FableWin.exe.signatures.tsv 0x03218850 (debug body 2239 bytes).
- Source 3: rtti-vtables.tsv: .?AVCCachedFile@@ vtable 0x012724D4 slot 3 = 0x00A5B370; engine/CAFile.h slot 3 is `virtual void Read(void*,long,CASuspendableProcess*)=0`.
- Callers: direct (statically bound) calls from CNavQuadTree::LoadFromFile 0x00A7AF60 (call sites 0xa7afe3 .. 0xa7b743, 17 sites; not landed); vtable ref 0x012724E0.
- Param names unknown from PDB (signature tsv has no names); landed CDiskFileWin32::Read uses destination/size/process.

## 3. Callees
| retail | landed row | landed name | PDB | note |
|---|---|---|---|---|
| 0x0099EBF0 | rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long) | ??0CCharString@@QAE@PBDJ@Z | draft uses default arg `length=-1`; siblings spell `CCharString("", -1)` explicitly |
| 0x0099F600 | not landed | operator+(const CCharString&, const char*) __fastcall (hidden result ecx, left edx) | | siblings declare `CCharString __fastcall operator+(const CCharString&, const char*)` |
| 0x0099EC30 | CCharString_CCharStringcopy_0099ec30.cpp | copy ctor | ??0CCharString@@QAE@ABV0@@Z | |
| 0x0099E4C0 | CCharString_operatorconstchar_0099e4c0.cpp | operator const char* | | |
| 0x0099EAE0 | CCharString_CCharString_0099eae0.cpp | ~CCharString | | |
| 0x00BFEB84 | (CRT) | _CxxThrowException, ThrowInfo 0x01369380 = CBBBFileException (4 bytes) | | |
| slot 3 +0x0C | CAFile::Read | | | |
| slot 5 +0x14 | CAFile::SetPosition(long) | | | |
| slot 7 +0x1C | CAFile::GetPosition() const | | | draft declares non-const `long GetPosition()` |
| slot 9 +0x24 | CAFile::GetLength() const | | | draft declares non-const; landed SetPosition names it `GetSize` (wrong; PDB GetLength) |
- CBBBFileException ctor, PDB (Ego_r 0x00b04820 + FableWin 0x02fedae0): `??0CBBBFileException@@QAE@VCCharString@@W4EFileExceptionType@0@@Z`
  = `CBBBFileException(CCharString message /*BY VALUE*/, EFileExceptionType type)`; also `(EFileExceptionType)` overload. Member `EFileExceptionType Type` +0 (struct_layouts_egor).
  The DRAFT's `CBBBFileException(long error, const CCharString&)` + `long Error` + `CExceptionBase` base is the WRONG spelling (copied from landed CDiskFileWin32::Read, which also has it wrong).
  Landed CCachedFile::Write 0x00A5AFB0 uses the PDB-faithful `(CCharString message, EFileExceptionType type) : Type(type) { (const char*)message; }`; MovePosition uses (CCharString, type) too.
  Enumerator names are not in any PDB dump found (fable_types.h has none) -> descriptive.

## 4. Members (no engine/CCachedFile.h exists; struct_layouts_egor CCachedFile 44 bytes; retail 0x28 because CArray = 12-byte STLport vector)
- +0x04 long Position; +0x08 long BufferStartPosition; +0x0C CArray<unsigned char> DataBuffer (engine/CArray.h = public std::vector<T>; start +0xC, finish +0x10);
  +0x18 CWideString Pathname (fable_string.h); +0x1C CCountedPointer<CAFile> PDiskFile (engine/CCountedPointer.h, Data +0, Info +4); +0x24 bool Opened; +0x25 bool BufferValid.
- Base: CAFile (engine/CAFile.h : CBaseClassNonCopyable, virtual dtor slot 0; slots 1 Open,2 Close,3 Read,4 Write,5 SetPosition,6 MovePosition,7 GetPosition const,8 SetLength,9 GetLength const ... match retail).
- Opt-ins: CArray.h FABLE_RETAIL_CARRAY (operator[] via begin(): `*(begin()+i)`), FABLE_RETAIL_CARRAY_GETASCARRAY_START; CAFile.h FABLE_RETAIL_CAFILE (pulls CArray), _ERRORCALLBACK etc. (irrelevant);
  CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER (operator->), _DTOR_DECL_ONLY etc.
- Locally modelled in draft though a shared header exists: CAFile (engine/CAFile.h), CArray<unsigned char> as `CArrayUChar` (engine/CArray.h), CCountedPointer (as raw CAFile* + void*; engine/CCountedPointer.h, as MovePosition 0x00A5B0A0 already uses), CWideString Pathname (fable_string.h).
  CCharString / CBBBFileException / CCachedFile have no engine header (all siblings model them locally).

## 5. Visibility
- All CCharString/operator+ callees are out-of-line library functions (other TU); nothing inline-visible. GetPosition (slot 7, 0x008263E0) is an inline body emitted out of line in the 0x826xxx COMDAT block but here called virtually through PDiskFile (a CAFile*), so irrelevant.
- STLport vector size()/begin()/operator[] are inline (size computed `[+10h]-[+0Ch]` each time).
- No direct calls to neighbouring CCachedFile rows.

## 6. Sibling landed rows (same TU, rebuild/src/compiled/00/a5/ and 00/82/)
- CCachedFile_Write_00a5afb0.cpp: PDB-faithful by-value CBBBFileException(CCharString, EFileExceptionType), `CCharString("", -1) + "..."`, local CAFile/CCachedFile.
- CCachedFile_MovePosition_00a5b0a0.cpp: `#include "engine/CCountedPointer.h"`, `CCountedPointer<CAFile> PDiskFile` + `PDiskFile->GetLength()`, CBBBFileException(CCharString text, type) : CExceptionBase(const char*).
- CCachedFile_SetPosition_00a5b020.cpp: CBBBFileException(type, const CCharString&) with inline ReportFileError(CCharString by value) (same bytes).
- CCachedFile_OpenFromFile_00a5b210.cpp, CCachedFile_Open_00a5b260.cpp, CCachedFile_CCachedFile_00826380.cpp (~CCachedFile, uses engine/CArray.h).
- None carry `// Flags:` or pragmas (Close has #pragma pack only); default /O2.
- CDiskFileWin32_Read_0099a960.cpp (same 2 messages, same codes 1/3): FABLE_FORCEINLINE on ReportFileError and the exception ctor (or the 3rd throw calls the ctor out of line) -- here there are only 2 throws but forceinline is the safe choice.

## 7. Old-draft model errors vs disassembly
- Residue: ebx/ebp swapped (retail dst=ebp, size=ebx); second refill must call GetPosition BEFORE GetLength while keeping PDiskFile cached in edi
  (e.g. `long pos = PDiskFile->GetPosition(); long remaining = PDiskFile->GetLength() - pos;` with a CCountedPointer operator->/ptr local) -- first refill is GetLength then GetPosition: the two refills are spelled differently (not one shared inline helper).
- Exception class spelled wrong (long Error / (long,const CCharString&) / CExceptionBase) vs PDB (CCharString by value, EFileExceptionType; Type member).
- CAFile virtuals declared non-const and with placeholder slots; use engine/CAFile.h (GetPosition/GetLength const).
- PDiskFile modelled as raw CAFile* (should be CCountedPointer<CAFile>); DataBuffer local CArrayUChar (should be engine/CArray.h CArray<unsigned char>, `DataBuffer.size()`, `&DataBuffer[0]`/begin()).
- Control flow matches otherwise (size==0 early return, BufferValid block, while(size>0){ inner 3-clause while; if(size>0) refill }); first-clause unsigned compare correct.

## 0x00E36750

# DOSSIER 0x00E36750 -- NScript::CV_ArcheryCompetitionScript::DoCountDown(long)

## 1. Retail shape (xdis 00e36750 0x228)
- thiscall, `ret 4` (two copies: 0xE3681A, 0xE36965). No frame pointer (`sub esp,0x10; push ebx/ebp/esi/edi`), no SEH/EH frame. Normal /O2-ish, no optimize("s") idioms beyond `lea esp,[esp]` loop-align pad at 0xE367B9.
- Locals: CTimer index in edi (register, never spilled); scripted-thing handle at [esp+0x10] (vptr +0, IntelligentPtr +4, PImp.Data +8, PImp.Info +C). ebp = 0 constant.
- Flow:
  1. RegisterTimer (pDSTGame slot 87, [eax+0x15C]) -> edi; SetTimer(edi, arg) slot 89 (+0x164)  == `CTimer timer; timer = arg;` (CTimer ctor + operator= expanded inline).
  2. handle ctor inline: call CBaseObject() 0x0099A380, vptr 0x0127094C, zero +8/+C.
  3. `while(!Game->StartScriptingEntity(Game->GetHero(), handle, 4 /*HIGHEST*/))` (Game=[esi+0x40]; GetHero slot 70 +0x118, StartScriptingEntity slot 8 +0x20, vtable cached in ebx) { NewScriptFrame (slot 7 +0x1C); if(IsActiveThreadTerminating()) -> 0xE3681D }.
     0xE3681D = the handle dtor EXPANDED INLINE (PImp release: dec Info->RefCount, DeleteFunc(Info->Data) fastcall, operator delete 0x00BFE9BC; zero +8/+C; vptr 0x0126008C; call ~CBaseObject 0x0099A430) then `jmp 0xE3694F` (DeregisterTimer + ret of the END copy).
  4. 0xE367F0: `if (IsActiveThreadTerminating())` -> 0xE367FB: call OUT-OF-LINE dtor 0x007E74D0; DeregisterTimer (slot 88 +0x160); ret 4.   <-- separate exit block A
  5. 0xE36860: `while (timer.GetTime() > 0)` (GetTimer slot 90 +0x168, `jle`) { NewScriptFrame; if (T) jne 0xE367FB (exit block A!); AddScreenTitleMessage(GFIntToCharString(timer.GetTime()+1), 0.6f /*0x3F19999A*/, false) (slot 113 +0x1C4); ~CCharString temp 0x0099EAE0; Pause(1.0f) (slot 376 +0x5E0) }.
     Loop is rotated: test at top (0xE36860) and duplicated test at bottom (0xE368D3 `jg 0xE36873`) = a plain `while`.
  6. 0xE368E6: `if (!T) { CCharString s("TEXT_QST_004_COUNTDOWN_40" @0x012E3450, -1) [0x0099EBF0]; Game->AddScreenTitleMessage(s, 1.0f, false); ~s; Game->Pause(0.3f /*0x3E99999A*/); if (handle.IsValid() [0x00CD23B9]) handle.Release() [0x00CD2770]; }`
  7. 0xE36946: out-of-line dtor 0x007E74D0 on handle; 0xE3694F: DeregisterTimer; ret 4 (exit block B).
- So: 4 exits. wait-loop exit = inline dtor + jump into block B's tail; post-wait T and in-loop T share block A (out-of-line dtor); post-loop T and fall-through share block B (out-of-line dtor). Old draft merged A into B (530 v 536).

## 2. Identity (manifest label is RIGHT)
- Mangled: `?DoCountDown@CV_ArcheryCompetitionScript@NScript@@QAEXJ@Z` -> public, non-virtual, `void DoCountDown(long)`.
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00E82A00), ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv (0x017F049A thunk, 0x03942650 body).
- Order evidence: Ego_r has DoTimesUp 0x00E827C0 then DoCountDown 0x00E82A00; retail DoTimesUp 0x00E36590 then this row 0x00E36750 (same order, same TU).
- Caller: xref -> 0x00E350D8 `mov ecx,[esi+0x14]; push 3; call` inside CAC_Owner::Main 0x00E33EA0 (ParentClass +0x14 is CV_ArcheryCompetitionScript&), i.e. `ParentClass.DoCountDown(3)`. Arg is a count of seconds; param name unknown in PDB lists.
- NOTE: CV_ArcheryCompetitionScript.h does NOT declare DoCountDown / DoTimesUp yet -- the row must add `void DoCountDown(long);` (header edit or a row-local opt-in macro; header today has FABLE_STATIC_ASSERT size 0xa4).

## 3. Callees (landed rows; draft spelling vs PDB)
| addr | landed row | PDB / landed name | draft spelling |
|---|---|---|---|
| 0x00CB7940 | rebuild/src/compiled/00/cb/CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating | ok |
| 0x0099A380 | 00/99/CBaseObject_CBaseObject_0099a380.cpp | CBaseObject::CBaseObject() | ok (local struct -> use header) |
| 0x0099A430 | 00/99/CBaseObject_CBaseObject_0099a430.cpp | CBaseObject::~CBaseObject | draft "CBaseObjectDtor" fake class |
| 0x007E74D0 | 00/7e/CScriptGameResourceObjectScriptedThingBase_CScriptGameResourceObjectScriptedThingBase_007e74d0.cpp | ??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ (implicit virtual dtor) | WRONG: draft "Destroy()" |
| 0x00CD23B9 | 00/cd/CScriptGameResourceObjectScriptedThingBase_IsValid_00cd23b9.cpp | ?IsValid@...@@QBE_NXZ (bool IsValid() const) | WRONG: draft "IsScripting()" non-const |
| 0x00CD2770 | 00/cd/CScriptGameResourceObjectScriptedThingBase_Release_00cd2770.cpp | ?Release@...@@QAEXXZ | ok |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | ok (use fable_string.h) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x0099F830 | 00/99/global_GFIntToCharString_0099f830.cpp | ?GFIntToCharString@@YI?AVCCharString@@J@Z (__fastcall) | ok |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete (from CCountedPointer release inline) | ok |
| 0x0143E8F8 | global | `extern CGameScriptInterfaceBase* pDSTGame;` (?pDSTGame@@3PAVCGameScriptInterfaceBase@@A, ESTABLISHED_SPELLINGS, 200+ landed rows) | WRONG: draft `GameCalls* g_pGameScriptInterface` |
| CTimer ctor/dtor 0x00CD4450/0x00CD4470 (not called: expanded) | 00/cd/CTimer_CTimer_00cd4450.cpp / _00cd4470.cpp | CTimer::CTimer / ~CTimer; set = `CTimer::operator=(long)` (??4CTimer@@QAEAAV0@J@Z), get = GetTime() const | WRONG: draft `SetTime`, member `Index` (real: TimerIndex) |
- Interface slots (rebuild/include/engine/CGameScriptInterfaceBase.h, names from FableWin.pdb): 7 NewScriptFrame() const, 8 StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority), 70 CScriptThing& GetHero(), 87 RegisterTimer, 88 DeregisterTimer(long), 89 SetTimer(long,long), 90 GetTimer(long) const, 113 AddScreenTitleMessage(const CCharString&, float, bool) const, 376 Pause(float) const. Draft's GameCalls slot numbers all agree; replace the local struct with the header.
- Priority 4 = SCRIPT_AI_PRIORITY_HIGHEST (enum as in landed 0x00ED5830 / 0x00E64FB0).

## 4. Members
- this+0x40 = `Game` of CLevelScriptBase<CQ_SunnyvaleMasterData> (rebuild/include/engine/CLevelScriptBase.h; retail CScriptBase 0x40 so Game +0x40, MasterScriptData +0x44). Original type `CGameScriptInterfaceBase& Game` under FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS (else pointer). Also define FABLE_RETAIL_CLEVELSCRIPTBASE (implicit ~CLevelScriptBase).
- CV_ArcheryCompetitionScript.h: `class NScript::CV_ArcheryCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData>`, size 0xa4; no other member touched by this function (Score +0x48 etc. unused).
- CScriptBase.h: PActiveThread +0x2C (read by IsActiveThreadTerminating, not here). Requires FABLE_RETAIL_NSCRIPT.
- CTimer.h: TimerIndex +0 (FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS makes it private). Row must define inline ctor/dtor/operator=/GetTime after the include (wave 8 rule): `inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()){}` etc. (copy from 0x00CEF3B0 / 0x00E64FB0).
- CScriptGameResourceObjectScriptedThingBase.h (needs FABLE_RETAIL_BASE_OBJECT; CCountedPointer.h first): PImp +0x08 (CCountedPointer<...>, Data +8 / Info +C), vtable 0x0127094C; base CScriptGameResourceObjectBase vtable 0x0126008C (inline `virtual ~...(){}`), CBaseObject.h. Opt-ins: _OUTOFLINE_DTOR (declares the dtor out of line -> calls 0x007E74D0 everywhere), _OUTOFLINE_CTOR, _OUTOFLINE_ASSIGN, _FORCEINLINE_ASSIGN, FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS.
- Locally modelled in old draft though a shared header exists: CCharString (fable_string.h + FABLE_RETAIL_CCHARSTRING), GameCalls (CGameScriptInterfaceBase.h), CCPPointerInfo + PImp words (engine/CCountedPointer.h), CBaseObject / CBaseObjectDtor (engine/CBaseObject.h), CScriptGameResourceObjectBaseDtor (engine/CScriptGameResourceObjectBase.h), CScriptGameResourceObjectScriptedThingBase, CTimer (engine/CTimer.h), CScriptBase, CV_ArcheryCompetitionScript (engine/CV_ArcheryCompetitionScript.h).

## 5. Callee-body visibility
- Handle dtor: retail MIXES -- inline at the wait-loop exit, out-of-line 0x007E74D0 at the other 3 exits. Proven landed mechanism (rebuild/src/compiled/00/ec/NScriptCVSickChildScriptCManInLove_Main_00ec8300.cpp lines ~75-80): keep the implicit (inline) dtor and wrap the specific `return` that must CALL the dtor in `#pragma inline_depth(0)` ... `#pragma inline_depth()`. Here: wrap the post-wait `return`, the in-loop `return`, and (probably) the function end / post-loop path; leave the wait-loop return plain. Alternative used by NOTES: OUTOFLINE_DTOR opt-in + hand-inlined first site (old draft did this, still differed by tail merge).
- CTimer ctor/dtor/operator=/GetTime: all expanded (no calls to 0x00CD4450/0x00CD4470) -> row-local inline definitions after the CTimer.h include. The DeregisterTimer at the wait-loop exit is reached by a jump into block B's tail, so the timer dtor is shared; it's the handle dtor that differs per exit.
- IsValid 0x00CD23B9 is an UNALIGNED COMDAT (inline member emitted out of line) but retail CALLS it here -> declare, do not define (as the header does). Release 0x00CD2770 called.
- CCountedPointer release body (inside the inline dtor) must be visible: engine/CCountedPointer.h (pattern in 0x00ED5830 defines the template dtor under #pragma auto_inline(off) only for the movie variant; here the release is expanded inline, so the default header inline body is needed).
- No STLport containers in this function.

## 6. Siblings (copy idioms)
- 0x00ED5830 NScriptCVSwordInTheStoneScript_SwordPullCutscene (rebuild/src/compiled/00/ed/...): CV_ level-script member function, RELOCATION_MATCH; defines FABLE_RETAIL_CCHARSTRING, _NSCRIPT, _BASE_OBJECT, _CLEVELSCRIPTBASE, _DEF_CLASS_BASE, _CDEFSTRINGTABLE, _STLP_MAP, ScriptedThingBase _OUTOFLINE_ASSIGN + _OUTOFLINE_DTOR; local `enum EScriptAIPriority {LOWEST..HIGHEST}`; includes fable_containers.h, fable_string.h, CCountedPointer.h, CScriptThing.h, CScriptGameResourceObjectScriptedThingBase.h, CGameScriptInterfaceBase.h, CQ_SunnyvaleMasterData.h.
- 0x00CEF3B0 NScriptCGameflowScript_CoreQuestReminder (00/ce/): CTimer local idiom `CTimer timer; timer = x; while(timer.GetTime()){Game->NewScriptFrame(); if(IsActiveThreadTerminating()) return;}` with row-local inline CTimer bodies; uses `#pragma inline_depth(0)` around a `return` (flagged "Revisit: inline-depth").
- 0x00E64FB0 NScriptCVChickenKickingScriptCChickenMaster_Main (00/e6/): CV_ TU, CTimer inline bodies (FABLE_FORCEINLINE for operator=/GetTime in a 15 KB body), wait idiom `while(!Game->StartScriptingEntity(Me,seh,SCRIPT_AI_PRIORITY_HIGHEST)){NSF; if(T) return;} if(T) return;` -- exactly this row's steps 3-4.
- 0x00EC8300 NScriptCVSickChildScriptCManInLove_Main (00/ec/): per-return `#pragma inline_depth(0)` to force the out-of-line handle dtor; `if (seh_me.IsValid()) seh_me.Release();` spelling; ORIGINAL_MEMBERS (Game is a reference: `Game.X()`).
- 0x00EF3F00 CLeader::Main (00/ef/): older "inline-once destructor band" (hand-destroy) idiom the draft copied; superseded by the inline_depth idiom.
- Same-file neighbours (DoTimesUp 0x00E36590, WatchForAttack 0x00E36390, CAC_Owner::Main 0x00E33EA0) are NOT landed.
- Pending siblings with the same residue: 0x00E6B900 CV_ChapelOfEvilScript::TeleportAcolytes and 0x00E5E350 HenchmanCommentary (hand309 NOTES).

## 7. Old-draft model errors
- Control flow is right (4 exits, rotated while loops, arg order). Residue is purely which exit blocks VC7.1 tail-merges.
- Hand-written Destroy()/DestroyInline() replace the implicit dtor: should be the real implicit dtor with inline_depth(0) around the returns that call 0x007E74D0 (ManInLove idiom). In retail, the post-wait return and the in-loop return share one block (A) separate from the end block (B); try inline_depth(0) on those two returns only, and let the natural scope-end dtor be out of line via inline_depth(0) around the closing part (or OUTOFLINE_DTOR + inline_depth on the wait-loop return to... no: the wait-loop one must be INLINE).
- Wrong names: IsScripting -> IsValid() const; Destroy -> ~CScriptGameResourceObjectScriptedThingBase; g_pGameScriptInterface/GameCalls -> pDSTGame/CGameScriptInterfaceBase; CTimer::SetTime/Index -> operator=(long)/TimerIndex.
- Step 6 body likely written in source as `if (T) return;` followed by the title/Pause/IsValid-Release block (all-return idiom), which then falls through to the end exit; try that spelling (it gives VC a 4th distinct return the draft lacked -- the draft used `if(!T){...}`).
- All local structs should become shared headers (list in section 4); DoCountDown must be added to CV_ArcheryCompetitionScript.h.

## 0x00773EE0

# DOSSIER 0x00773EE0 CTCShop::GetSaleItemsToPlace (540 bytes incl. 2-byte pad; body 0x773EE0..0x7740FB)

## 1. Retail shape (xdis 00773ee0 0x22c)
- __thiscall, `ret 4` (hidden result pointer at [esp+0x30] after prologue; returned in eax via `mov eax,esi`).
- NO frame pointer: `sub esp,1Ch ; push ebx ; push ebp ; push esi ; push edi` -> /O2 TU with /Oy (no `optimize("y",off)`), not size-optimised
  (fild/fcomp/fnstsw, `lea esp,[esp]` loop-alignment pad at 0x773FBC = speed). No SEH frame (no fs:0) despite dtors.
- `this` lives in EBP for the whole function (`mov ebp,ecx` 0x773EEB; `mov ecx,ebp` 0x773FE8 for GetDefinitionManager; `cmp eax,[ebp+34h]` loop end).
- Frame (final esp): +0x13 bool byte (no shop def), +0x14 shopDef (CDefPointer<const CShopDef>), +0x18 itemDef temp (by-value return of
  GetPDefFromGlobalIndex), +0x1C stockDef (CDefPointer<const CStockItemDef>), +0x20/+0x24/+0x28 the local vector<long> (_M_start/_M_finish/_M_end).
  Vector + shopDef zeroed up front; stockDef slot ([esp+0x1C]) ALSO zeroed in prologue (0x773EE6, before the pushes) and again per-iteration
  at 0x773FFF after the GetSubDef arg push.
- Def test: two by-value copies of ParentThing->PBaseDef (+0x70): esi copy (`inc [esi+4]`), edi copy (`inc [edi+4]`), ebx = conditional
  temporary flags (1, then 3); bool stored as byte at [esp+0x13]; releases edi copy (flag 2) then esi copy (flag 1) via `dec [x+4]; jne; call [vtbl+4]`.
  Exactly the landed 0x00774100 named-bool form.
- Early return (bool true): copy-ctor 0x514160(&items) into hidden ptr, release shopDef, NO free (empty vector known), own epilogue at 0x773FA2.
  shopDef==0 jumps to the LATE epilogue 0x7740F0. Retail has TWO epilogues (0x773FA2 and 0x7740F0). Old draft merged them (NOTES: "early return merges into the late one").
- Loop over Stock (map header at [this+0x34]): `it=header->_M_left ([hdr+8])`, `it != header`.
  - `ecx = it->first ([it+10h]); lea ebx,[it+10h]` (ebx = &key, used as push_back arg); `if (key <= 0) skip` (jle, signed long).
  - `fld [it+14h] ; fcomp 0.0f@0x0122DEDC ; test ah,41h ; jne skip` => condition `Quantity > 0.0f`.
  - `push key ; push &itemDef ; ecx=ebp ; call 0x6869D0` (GetDefinitionManager, returns ref in eax) `; ecx=eax ; call 0x9AD9E0` (GetPDefFromGlobalIndex const).
  - `push &stockDef ; ecx=itemDef.Object ; stockDef=0 ; call 0x59350C` (GetSubDef<CStockItemDef>). edi = stockDef.Object loaded AFTER the call regardless.
  - `if (al && (float)stockDef->DefaultPrice(+0x28, fild) <= shopDef->MaxPriceToDisplay(+0xA4) [jp-skip form] && stockDef->CanBeDisplayedInShop(+0x3C))`
    `push ebx ; lea ecx,items ; call 0x485FF3` (vector<long>::push_back OUT OF LINE).
  - stockDef release in place (`test edi; dec [edi+4]; jne; call [vtbl+4]`, no null store), THEN itemDef release in the count-reference form
    `mov edi,[eax+4]; mov ecx,eax; add eax,4; dec edi; mov edx,edi; test edx,edx; mov [eax],edi; jne; call [edx+4]` + `mov [esp+18h],0`
    (same bytes as the GetPDefFromGlobalIndex temporary release inside landed 0x005130A0, which uses the DEFAULT engine/CDefPointer.h Release).
  - Tree increment inline (right/left-most or parent climb, STLport _Rb_tree_base_iterator::_M_increment).
- After loop: `edx=items._M_finish ; ecx=items._M_start ; call 0x777080` (random_shuffle<long*>, __fastcall), copy-ctor 0x514160 into
  hidden ptr, release shopDef, `if (_M_start) free(_M_start)` (0xBFEA14, cdecl, add esp,4), epilogue, ret 4.

## 2. Identity (label CORRECT)
- Name: CTCShop::GetSaleItemsToPlace, public, const. Leaf `?GetSaleItemsToPlace@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ`
  -> `std::vector<long> CTCShop::GetSaleItemsToPlace() const`.
- Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x00781160, immediately before GetWantedItemsList 0x007813A0 -- retail order 0x773EE0 then 0x774100
  matches); ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv (FableWin 0x017C57B3/0x0261ADB0, same leaf).
- Already declared in rebuild/include/engine/CTCShop.h under FABLE_RETAIL_CTCSHOP (line 53, no address comment).
- Callers (xref): 0x00774C7C (inside CTCShop::StockDisplay 0x00774C60, NOT landed) and 0x009282A2. Landed std_randomshuffle_00777080.cpp names this row as caller.

## 3. Callees (landed file / landed name / PDB name)
- 0x005130A0 rebuild/src/compiled/00/51/CDefClassBase_GetSubDefCShopDef_005130a0.cpp, `??$GetSubDef@VCShopDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCShopDef@@@@@Z`
  (member template, const, out param CDefPointer<const CShopDef>*). Draft: non-const non-template `GetSubDef(CDefPointer<CShopDef>*)` -> WRONG spelling.
- 0x006869D0 .../00/68/CTCBase_GetDefinitionManager_006869d0.cpp, `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` (protected const,
  returns const CGameDefinitionManager&). Declared in engine/CTCBase.h line 148. Draft: `CTCShop::GetDefinitionManager()` returning CDefinitionManager* -> WRONG class/return.
- 0x009AD9E0 .../00/9a/CDefinitionManager_GetPDefFromGlobalIndex_009ad9e0.cpp; proven name (naming wave1 DECISIONS.tsv, Ego_r+FableWin)
  `?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z` = PUBLIC CONST overload returning
  CDefPointer<const CDefClassBase> (0x009ADA10 is the protected non-const IAE twin). engine/CDefinitionManager.h line 82 declares it.
  Draft: non-const returning a local CDefClassPointer -> wrong overload/type.
- 0x0059350C .../00/59/CDefClassBase_GetSubDefCStockItemDef_0059350c.cpp, `??$GetSubDef@VCStockItemDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCStockItemDef@@@@@Z`.
  Draft uses non-const non-template on a local `CStockItemDef` -> wrong spelling.
- 0x00485FF3 .../00/48/stdvector_pushback_00485ff3.cpp, `?push_back@?$vector@JV?$allocator@J@std@@@std@@QAEXABJ@Z` (frame-pointer size-opt COMDAT).
- 0x00777080 .../00/77/std_randomshuffle_00777080.cpp (Flags /Gr, __fastcall). Landed leaf is a placeholder `random_shuffle<CUnprovenElement_00777080**>`;
  this row proves the element is `long` (std::random_shuffle<long*>, STLport 2-arg). Possible NAME_FIXES item. Draft calls it `RandomShuffle` (made up).
- 0x00514160 .../00/51/stdvector_vector_00514160.cpp, `??0?$vector@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z` (copy ctor; by-value return, no NRVO).
- 0x00440990 (only via push_back) .../00/44/stdvector_Minsertoverflow_00440990.cpp `_M_insert_overflow`. Not called directly here.
- 0x00BFEA14 free (.../00/bf/global_free_00bfea14.cpp). Vtable slot 1 (+4) = CDefPointeeBase Delete on every release.
- Data: 0.0f literal @ 0x0122DEDC (compiler constant; other landed rows cite it).

## 4. Members
- CTCBase::ParentThing +0x04 (engine/CTCBase.h; inline GetParentThing exists).
- CThing::PBaseDef +0x70, `CDefPointer<const CThingBaseDef>` (engine/CThing.h line 74, asserted 0x70); inline `GetPBaseDef() const` returns BY VALUE
  (line 259, unconditional) -> the two `inc [x+4]` copies. Draft calls it PDef / GetPDef on CDefClassBase* -> wrong name/type.
- CTCShop::Stock +0x34 `std::map<long,CShopItem>` (private, engine/CTCShop.h line 106; PDB +0x3C; header comment "native Stock+34"); include stlport/fable_map.h.
- map node: key +0x10, CShopItem +0x14; CShopItem::Quantity +0x00 (float) (engine/CShopItem.h, PDB CShopItem).
- CShopDef::MaxPriceToDisplay retail +0xA4 (PDB +0xB4, float), engine/CShopDef.h line 41 (class CShopDef : CSubDefClassBase). Draft comment calls it "item-level limit" -- it is a price.
- CStockItemDef::DefaultPrice +0x28 (long), CanBeDisplayedInShop +0x3C (bool) -- PDB 40/60, engine/CStockItemDef.h (FABLE_RETAIL_CSTOCKITEMDEF or
  FABLE_RETAIL_CLOTHING_DEFS layouts; asserted DefaultPrice==0x28).
- Opt-ins: FABLE_RETAIL_CTCSHOP (declares GetSaleItemsToPlace), FABLE_RETAIL_CTHING, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_DEF_CLASS_BASE (required by CDefPointer.h),
  FABLE_RETAIL_CDEFINITIONMANAGER, FABLE_RETAIL_CGAMEDEFINITIONMANAGER, FABLE_RETAIL_CRESOURCELIST, FABLE_RETAIL_CSTOCKITEMDEF.
- Locally modelled in the draft though shared headers exist: CDefPointeeBase, CDefPointer, CShopDef, CStockItemDef (as CStockItemDef), CDefClassBase,
  CDefinitionManager, CThing, CTCShop, CShopItem, the map (SStockNode/CStockMap), vector<long> (CLongVector). All have engine/ or stlport/ headers.

## 5. Callee visibility (what retail's compiler saw)
- push_back 0x485FF3 out of line (odd address = COMDAT): stlport/fable_containers.h FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (visible body, noinline;
  "c2 knows push_back keeps no address of the value") or FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (decl only). The retail `lea ebx,[it+10h]`
  passing the node key address directly suggests either; try NOINLINE first (sibling 0x774100 needed visible bodies to keep vector state in regs).
- copy ctor 0x514160: FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE (visible body + noinline, fable_containers.h 395/599) -- exactly the "empty-vector
  knowledge on early return / no free" effect the landed 0x774100 got with a local same-TU body.
- random_shuffle 0x777080: FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG (fable_containers.h 1173; __fastcall decl; used by landed 0x008F10B0 ShuffleCards).
- GetSubDef<CShopDef>/<CStockItemDef> both out of line: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (engine/CDefClassBase_GetSubDef.h, visible
  body kept out of line -- tells c2 the callee only writes through the out pointer; retail reloads edi from [esp+1Ch] after the call regardless).
  The landed 0x774100 instead defines GetSubDef<CShopDef> locally as noinline -- same idea.
- GetDefinitionManager 0x6869D0 and GetPDefFromGlobalIndex 0x9AD9E0 are separate TUs (far addresses); declarations only.
- Same TU neighbours (CTCShop, earlier): SetAlarm 0x773C90, AddShopper 0x773DC0, RemoveStockItemsFromShop 0x773E10, then this, then GetWantedItemsList 0x774100.

## 6. Sibling landed rows
- rebuild/src/compiled/00/77/CTCShop_GetWantedItemsList_00774100.cpp -- the twin (same prologue, same named-bool def test, same loop, same copy/free epilogue).
  Uses LOCAL models (no shared headers), `#pragma auto_inline(off)` same-TU bodies for _M_insert_overflow / copy ctor / GetSubDef<CShopDef>, no Flags.
  Inline push_back there; here push_back is out of line (difference!).
- 00/77/CTCShop_GetCurrentSellPrice_007743b0.cpp -- shared-header idiom: defines FABLE_RETAIL_BASE_OBJECT, _CTCBASE, _DEF_CLASS_BASE,
  _CDEFPOINTER_RELEASE_SELFDELETING_POINTEE, _CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, _CTHING, _CCHARSTRING, _CDEFSTRINGTABLE, _STLP_LIST,
  _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY, _CGAMEDEFINITIONMANAGER, _CRESOURCELIST, _CRESOURCE,
  _CTCSHOP_SELLPRICE; includes engine/CThing.h, CThingBaseDef.h, CShopDef.h, CTCShop.h, CGameDefinitionManager.h. Uses `GetDefinitionManager()` and `ParentThing->`.
- 00/77/CTCShop_SetAlarm_00773c90.cpp (FABLE_RETAIL_CTHING, _CCHARSTRING, CTCSoundPlayer opt-ins); 00/77/CTCShop_RemoveStockItemsFromShop_00773e10.cpp (fable_compiler.h, auto_inline toggles).
- 00/8f/CTCTavernGameCardBase_ShuffleCards_008f10b0.cpp -- model for FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG + DEF_CLASS_BASE + CDEFCLASSBASE_GETSUBDEF_NOINLINE.
- 00/51/CDefClassBase_GetSubDefCShopDef_005130a0.cpp -- default CDefPointer release yields the same temp-release bytes as this row's itemDef release.

## 7. Old draft model errors
- Residue (hand305 NOTES/STATUS): DIFFER 495 v 540: compiler put a zero register in ebp and spilled `this`; early return merged into the late
  epilogue. Retail: `this` in ebp, eax as the zero, two distinct epilogues. 45 bytes short = mostly the missing duplicate epilogue/early copy path.
- Draft inlines nothing wrong on push_back (calls out-of-line 0x485FF3 correctly) but gives it no visible body (decl only) -- c2 may assume
  &it->first escapes; try PUSHBACK_NOINLINE.
- itemDef modelled as a local CDefClassPointer with Release()/DecRef/GetRefCount; really CDefPointer<const CDefClassBase> from the const
  overload. The `mov edi,[eax+4] ... mov [eax],edi` shape comes from the shared default Release on a temporary (see 0x5130A0); also
  FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF exists (CDefPointer.h 110) if the default does not reproduce it.
- Thing def: draft `CDefClassBase* PDef` + `GetPDef()` returning CDefPointer<CDefClassBase>; real is CDefPointer<const CThingBaseDef> PBaseDef + GetPBaseDef().
  The test then needs `->GetSubDef(&shopDef)` on a const CThingBaseDef (CDefClassBase const member template) -- consistent with const GetSubDef.
- Wrong types/names: CStockItemDef local, GetDefinitionManager on CTCShop returning CDefinitionManager*, RandomShuffle, CLongVector, the
  "level" comments (it is DefaultPrice vs MaxPriceToDisplay price cap). Loop condition order (key>0 && Quantity>0.0f) and the
  `DefaultPrice <= MaxPriceToDisplay && CanBeDisplayedInShop` test match retail.
- stockDef: retail zeroes its slot in the prologue too -> consider declaring stockDef at function scope? No: it is re-zeroed per iteration after the
  arg push (`CDefPointer<const CStockItemDef> stockDef;` inside the loop declared AFTER the itemDef temporary, matching release order stockDef then itemDef).
  The prologue zero at [esp+1Ch] may be a shared slot; verify against the compile.

