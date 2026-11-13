# DOSSIER 0x00795300 (9268 bytes) -- scout pass, fresh (no earlier scout existed: $W/prior/ and agent4024 src/test/probe are empty)

Helpers left in $W/scout/: ann.txt (whole listing, callees + globals resolved), callees.tsv (every call, count, landed file),
twin_78fd60.txt (retail listing of the twin), rd.py (reads retail .rdata: strings/floats), callees.py / ann.py.

## 1. Name -- the manifest label is WRONG
- Real: **CTCUnholyPower::FrameUpdate**, `?FrameUpdate@CTCUnholyPower@@UAEXXZ` (public virtual, void, no args).
  - rtti-vtables.tsv: `CTCUnholyPower 0126bfdc slot 9 -> 00795300` (CTCDivineWrath's slot 9 is 0x0078FD60).
  - FableWin.exe.signatures.tsv / _fablewin_api.tsv: `?FrameUpdate@CTCUnholyPower@@UAEXXZ` (0268e510, 14791 debug bytes).
  - Ego_r.pdb order (egor_pdb_names.tsv): ...OnKill, OnRemove, OnCreate, ~dtor, **FrameUpdate** -- retail: OnKill 0x007952E0,
    OnRemove-forwarder 0x007952F0 (landed as "FinalCleanup_007952f0"), then 0x00795300.
  - Only reference: the vtable word at .rdata 0x0126C000 (`xref.py`); no direct callers.
- Class header: rebuild/include/engine/CTCUnholyPower.h already declares `virtual void FrameUpdate();`.
- **TWIN**: 0x0078FD60 CTCDivineWrath::FrameUpdate (also unlanded, same 9268 bytes, 2710 instructions) differs ONLY in
  11 constants: RemoveTC 0x86 vs 0x85; SpellExclusionChannel +0xb0 vs +0xa8 (x2); LoopingSound +0xc4 vs +0xbc (x2);
  AddDamageWithAttribute key 0xA vs 9 (x3); CEventHitBySpecialAbility ability 0x13 vs 0x12 (x3). One source text => two rows.

## 2. Retail shape (xdis / dis_all.txt)
- `push ebp; mov ebp,esp; and esp,0FFFFFFF8h; sub esp,230h; push ebx; push ebp; push esi; push edi` -- 8-aligned frame
  (an 8-byte-aligned local: some double/__int64 or a struct with one -- the CCreatureAction locals are the likely reason),
  ebp re-used as `this` (mov ebp,ecx), `this` also spilled to [esp+38h]. Epilogue `pop edi/esi/ebp/ebx; mov esp,ebp; pop ebp; ret`.
- Plain `ret` (thiscall, no args). **NO SEH prologue / no EH state** although it holds many CCharString / CIntelligentPointer /
  CHitParameters / action locals => same as every landed CTCUnholyPower row (no /EHsc; siblings carry no EH flag).
- No /GS cookie. Not size-optimised (loops aligned with `lea esp,[esp]` / `lea ebx,[ebx]` padding = /O2 speed).
- 4 early returns duplicate the epilogue (0x0079534D, 0x00795359, 0x007956FD; final 0x0079772C).
- Inline budget is visibly exhausted late in the body: the last block (0x00796EE7..) calls `std::bitset<274>::test`
  0x00410DE0 (x9), IS_THING_ALIVE 0x006961F0, CIntelligentPointer copy ctors 0x0078E5B0/0x006EF410 OUT OF LINE, while the
  earlier blocks expand the same things inline (`test [thing+20h+word],mask`; CBaseIP ctor+vft+SetPItem(GetPItem)).
  The map iterator ++ is out-of-line 0x00793A00 in 3 places but inline (successor walk) in 3 others; map erase is
  out-of-line 0x00797A60 once and fully inlined once (0x00796E3A..0x00796EBF). Expect a /d2inlT-style budget fight
  (sibling FinalCleanup 0x00795090 needed `// Flags: /d2inlT62`).

## 3. Callees (landed file under rebuild/src/compiled/<aa>/<bb>/; PDB name; flags)
| addr | n | landed name / file | note |
|---|---|---|---|
| 008EDDD0 | 1 | CWillButtonHelperTimer::FrameUpdate | Timer.FrameUpdate() |
| 004C9840 | 1 | CThing::RemoveTC (file CThing_RemoveTC_004c9840) | `GetParentThing().RemoveTC(0x86)` |
| 00793D20 | 1 | CTCUnholyPower::SetPTarget | private |
| 0099EBF0 / 0099EAE0 | 16/19 | CCharString(const char*,long=-1) / ~CCharString | |
| 008BD2E0 | 1 | CCreatureAction_BuildUpUnholyPowerSpell(CThingCreatureBase*, CCharString animName) | CCharString by value |
| 008BD330 | 1 | CCreatureAction_ReleaseUnholyPowerSpell(CThingCreatureBase*) | |
| 008BD390 | 1 | CCreatureAction_ReleaseAndLoopUnholyPowerSpell(CThingCreatureBase*) | |
| 0072D360 | 1 | CCreatureActionBase::SetPItemRemoved(CIntelligentPointer<CThing>) | by-value IP, copy ctor 0x004A9730 |
| 006644F0 | 3 | bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) | |
| 0062E900 | 3 | CCombatActionBase::~CCombatActionBase | the action temps' dtor |
| 009D49B0 | 4 | CDefStringTable::GetString(long) on GDefStringTable 0x013CA828 | |
| 00415DD0 | 1 | CDefString::GetString | out of line, once |
| 0040F020 | 21 | CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (inline GetTC chain) | |
| 004365B0 | 2 | another LowerBound instance (TC 0x49 only) | different COMDAT -- GetTC<T> for that T |
| 0057B1DB | 1 | CTCHeroStats::ModifyStaminaBy(long) | |
| 007940F0 | 4 | **landed as "CTCDivineWrath::PlaySoundW" -- WRONG**: it is CTCUnholyPower::PlaySoundW (`?PlaySoundW@CTCUnholyPower@@AAEKABVCCharString@@_N@Z`; sits between UnsheatheWeapon 0x00793EC0 and StopSound 0x00794180 like Ego_r's order; the file itself says "byte twin of 0x0078EBF0 in another TU"). NAME_FIXES candidate. |
| 00794180 | 1 | CTCUnholyPower::StopSound(unsigned long) | header has an INLINE StopSound body; retail calls it OUT of line here |
| 00662650 | 3 | CThingCreatureBase::FinishCurrentAction | |
| 00793EC0 | 2 | CTCUnholyPower::UnsheatheWeapon | |
| 00794C10 | 2 | CTCUnholyPower::FinishMove | |
| 004C73D0 | 22 | CThing::GetPos (file CThing_GetPosition_004c73d0) | returns const C3DVector& |
| 00BFEA70 | 7 | __ftol2 | |
| 00BFEA0E | 2 | malloc (list head node, 12 bytes) | inline STLport list<CThingCreatureBase*> ctor |
| 00686D90 | 3 | CTCBase::PeekThingSearchTools (NOT LANDED; `?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ`, __fastcall shape) | |
| 00792E50 | 2 | CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, list<CThingCreatureBase*>, CFilter_Bind2<CThingFilter_OverlappingCircle,CThingFilter_CanBeHitByEpicSpell>> const | `ret 0Ch` |
| 007930B0 | 1 | same template for CThingObject / list<CThingObject*> (non-const) | |
| 007926C0 | 1 | landed "_global::RBTree_AllocateNode" -- really the out-of-line list<CThingObject*> ctor (ecx=&list, arg=&allocator byte) | suspect name |
| 00792240 | 1 | std::list::~list (list<CThingObject*>) | |
| 006F09D0 | 2 | "StdList_Clear_006f09d0" (file CGuiControlTreePane_SortTreeRecursively_006f09d0) -- the folded list<T*>::~list used for the creature list | |
| 007E45D0 / 007E46D0 / 007E4680 | 4/4/5 | CTCWillResponse::IsThingNotBloodyWillableAtAll / IsThingEpicSpellable / IsThingEpicPowerExplodable (static __fastcall, const CThing*) | |
| 00793CC0 | 2 | CTCUnholyPower::Attackable(CThingCreatureBase*) | |
| 0085F120 | 2 | NCreatureMode::IsUnderground(const CThing&) __fastcall | |
| 007978C0 | 2 | map<CThingCreatureBase*,PerCreatureInfo>::find (landed under a CUnproven name) | PDB `?find@?$_Tree@...CTCUnholyPower...` |
| 00798330 | 8 | map::operator[] (landed "UnholyPowerCreatureMap::operator[]") | PDB `??A?$map@...` |
| 00793A00 | 3 | "CRetailTreeIterator::Increment" = map iterator operator++ | |
| 00797A60 | 1 | map::erase(iterator) | |
| 0042A34E | 1 | std::_Rb_global<bool>::_Rebalance_for_erase | inside the inlined erase |
| 005C05C0 / 004CC340 / 006961F0 | 3/1/1 | IS_THING_ALIVE<CThingCreatureBase*> / <CIntelligentPointer<CThing>> / <CIntelligentPointer<CThingCreatureBase>> | |
| 00668B60 / 00668B80 | 2/2 | CIntelligentPointer<CThingCreatureBase>(T*) / operator= | |
| 004A96F0 / 004A9730 / 004A9760 | 2/1/4 | CIntelligentPointer<CThing>(T*) / copy ctor / operator= | |
| 0078E5B0 / 006EF410 | 1/1 | CIntelligentPointer<CThingCreatureBase> copy ctor (two COMDAT copies) | |
| 00A01B10/B50/B90/C10 | 5/97/6/12 | CBaseIntelligentPointer ctor / GetPItem / SetPItem / dtor | |
| 004BFB40 | 3 | NGraphicAppearance::CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc&, bool, bool) | |
| 00669580 | 2 | CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>&) | |
| 00669D30 | 8 | CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) const | |
| 0072AB70 | 7 | CTCSpecialAbilities::CreateParticleEffect(long, const C3DVector&) static __fastcall -> CThing* | |
| 0072ABD0 | 2 | CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) static __fastcall | |
| 007F1B70 | 2 | CTCDRumble::Create(const C3DVector&, EQuakeStrength, EQuakeLength, bool) __fastcall | |
| 0044C6B0 / 00792350 | 1/1 | CGameDefinitionManager::Get() / CDefinitionManager::GetDef<T> (ability def) | |
| 006D8300 | 1 | CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory, long) | |
| 00789238 / 00789358 | 3/3 | CHitParameters(CHitLocation*, CThing* hitter, CThing* weapon, const C3DVector& pos, const C3DVector& dir, bool knockDown, bool blockable, bool flourish, bool blockCounter) / ~CHitParameters | |
| 00868CA0 | 3 | CHitParameters::AddDamageWithAttribute(long key, float value) | |
| 00811830 | 3 | NEntityEvents::CEventHitBySpecialAbility(const CHitParameters&, ESpecialAbility 0x13) (operator new 0x18) | |
| 008113C0 | 7 | CTCEntityEvents::AddEvent(CEventBase*) | |
| 00811190 | 2 | CEventKilledCreature(const CThing& killer, const CThing& killed) (new 0xC) | |
| 00669300 | 2 | CEventDied(CThing* killedBy, CThing* deadBody) (new 0x18) | |
| 0080F870 | 2 | CTCNymph::StopFlyingAround(bool) | on GetTC(0xF5) |
| 0073A8A0 / 0073AE10 | 1/1 | CTCCreatureModeManager::IsModeActive(0x5F) / AddMode(0x5F) | |
| 004C9D60 | 2 | CThing::AddTC(const CCharString&, bool, bool) | "CTCCreatureTintEffect", true, false |
| 0078DDB0 | 2 | CThing::SetAsGameStatePersisted(bool) | |
| 00686A70 | 4 | CTCBase::GetWorld | world+0x60 = CMessageEventManager* |
| 00687540 | 2 | CMessageEventManager::AddMessage(EMessageEventType, unsigned long lifespan, const C3DVector&, CThing* orig, CThing* target, float, float, long) | |
| 00687730 | 2 | CMessageEventManager::AddMessage(const CMessageEventDef&, const C3DVector&, CThing*, CThing*, long) | |
| 006869D0 / 007923C0 | 2/2 | CTCBase::GetDefinitionManager / CDefinitionManager::GetDef<CMessageEventDef> | |
| 006A4D00 | 6 | CThingPhysical::PeekPos | |
| 006C6990 | 4 | CTCHeroOpinionDeedLog::PostDeed (NOT LANDED; `?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@...`) | 9 args |
| 006D00A0 | 2 | CTCWife::Murdered() | |
| 0077A890 | 2 | CTCDExperienceOrb::Create(CThing&) static __fastcall (NOT LANDED; `?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z`) | |
| 004C9B80 | 2 | CThing::Kill(bool) | |
| 004C0560 | 5 | NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString&, ?, ?, C3DVector&, float) const -> bool (NOT LANDED) | 0,0 then &pos, 1.0f |
| 00666D40 | 2 | CThingCreatureBase::GetHeightZBelowAt(const C3DVector&, ?, bool) const -> float (NOT LANDED) | args (&pos, 0, 1) |
| 00BFEA1A / 00BFEA14 | 7/1 | operator new / free | |
| vcalls | | creature vtbl+0x104 (OnHit(const CHitParameters&)? slot 65), killer vtbl+0x134 (slot 77, bool -- IsHero-like), TC(0x3C) vtbl+0x34, CDefPointer release vtbl+4 | |

## 4. Members (retail offsets; CTCUnholyPower.h is already the retail layout, size 208 asserted -- no FABLE_RETAIL_* macro in it)
this: ParentThing +0x04 (CTCBase), PDef +0x0C (CDefPointer<const CSpecialAbilitiesUnholyPowerDef>, raw ptr read), PItemRemoved +0x10,
UnholyPowerActionInitialised +0x18 (bool), Timer +0x1C (CWillButtonHelperTimer, retail 52 bytes; Timer.CurrentState = +0x30),
SpellLevel +0x50, NumFramesBetweenDamageHit +0x54, DurationOfSpellFrames +0x58, NumFramesForEnvChange +0x5C,
CurFramesForEnvChange +0x60, Finished +0x64, NumFramesToWaitForFadeOut +0x68, PerformedReleaseAction +0x76,
LastFrameCreaturesHit +0x78 (STLport map, 12 bytes: header node ptr +0x78, size +0x7C), HitEffects +0x84 (vector<PerEffectInfo> begin/end +0x84/+0x88),
PerFrameStaminaModify +0x90, AccModifyStamina +0x94, StaminaModify +0x98, SpellExclusionChannel +0xB0, LoopingSound +0xC4.
PerCreatureInfo (map node value at node+0x14): ActualCreature +0x00, FramesToGetHit +0x08, Effect +0x0C, DefaultChannel +0x14.
PerEffectInfo (0x14): Position +0x00, Effect +0x0C.
Def (engine/CSpecialAbilitiesUnholyPowerDef.h, retail offsets in comments): ReleaseSound +0x54, ReleaseLoopSound +0x60,
FinishSound +0x6C, CancelSound +0x78, RandomFactorForNumSecsBetweenDamageHit +0x84, DamagePerHit +0x94, RadiusOfEffect +0xAC.
CThing (retail): UsingComponents bitset<274> +0x20 (TC-bit tests: +0x20 &0x10 =TC4 HeroStats; +0x24 &0x20000 =0x31 ModeManager;
+0x24 &0x10000000 =0x3C; +0x28 &4 =0x42 EntityEvents; +0x28 &0x200 =0x49; +0x28 &0x20000 =0x51 DeedLog; +0x2C &0x100 =0x68 HeroExperience;
+0x2C &0x200000 =0x75; +0x30 &0x800 =0x8B tint; +0x38 &0x800000 =0xD7 Wife; +0x3C &0x200000 =0xF5 Nymph), TC map +0x44, +0x60 (phys:
+0x34 radius float; +0x0C pos), +0x64 PTCGraphicAppearance (DefaultChannel copy from ga+0x40), +0x70 def ptr (CDefPointer, refcount +4;
def+0x148 bit0), +0x91 bit0 dead; creature: Health +0xB4, +0xF4 CIntelligentPointer<CThing> (last attacker), +0x15C bit5 / +0x15D bit6 flags.
TC 0x75 object: bool at +0x20 (false while hit, true on release). TC 0x49 object: long +0x18.
Engine opt-ins the class family uses (copy from OnKill 0x007952E0 / HasFrameUpdate 0x00798180, 25 defines): CTCUNHOLYPOWER_CANCELFORCUTSCENE,
CTCBASE, CTHING, CTHING_MOREACCESS, CTHING_STATEACCESS, CTHING_ADDANDCOPYTC, CTHING_GETTC_ORIGINAL_CHAIN, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
CTHINGCREATUREBASE, CTHINGCREATUREBASE_FINISHACTION, CINTELLIGENTPOINTER(+_COPY, _COPY_ORIGINAL_CHAIN, _GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT),
IS_THING_ALIVE, CCHARSTRING, DEF_CLASS_BASE, CTCSPECIALABILITIES_FULL, CWORLD, CWORLD_ENVIRONMENT, CENVIRONMENT, CENVIRONMENT_DEFAULTTHEME,
CTCSOUNDPLAYER; FinalCleanup adds CVECTORMAP_INLINE_LOWERBOUND, CTHING_STATTYPEFLAGS, CTHINGCREATUREBASE (STATTYPEFLAGS); FinishMove uses
CTHING_GETTC_EXPANDED. Also relevant: C3DVector.h GFFastOneOverSqrt variants (retail here: `(3.0f - r*r*value)*r*0.5f`, line 58 default form;
`mov edx,17Ch` before the table load), CTHINGCREATUREBASE_HEIGHTZBELOW (GetHeightZBelowAt).

## 5. Visibility
- Out-of-line in retail although probably visible: StopSound (inline in the header -> call it so it is emitted? retail CALLS 0x00794180),
  CDefString::GetString 0x00415DD0, IS_THING_ALIVE instances, CIntelligentPointer ctors/op=, map find/operator[]/++ /erase,
  std::list ctor (CThingObject) / ~list, bitset::test (late), LowerBound 0x0040F020 (always called; inline chain around it).
- Inline in retail: CThing::GetTC<T> (bitset word test + LowerBound + end/key compare), creature list ctor (malloc(12) head,
  head->next=head->prev=head), map iterator successor walk (3x), map erase (1x), CBaseIP copy (ctor + vft + SetPItem(GetPItem)),
  CDefPointer release (`dec [p+4]; jne; call [vft+4]`), vector element reads, GFFastOneOverSqrt/Normalise, the stamina loop.
- Same TU, defined earlier: Attackable 0x00793CC0, SetPTarget 0x00793D20, ResetCreatureHitState 0x00793E00, UnsheatheWeapon 0x00793EC0,
  PlaySoundW 0x007940F0, StopSound 0x00794180, FinishMove 0x00794C10, CancelMove, CancelForCutscene, FinalCleanup 0x00795090.

## 6. Siblings to copy
- CTCUnholyPower_OnKill_007952e0.cpp / _HasFrameUpdate_00798180.cpp (full 25-define TU set, includes CThingCreatureBase.h, CTCUnholyPower.h,
  CTCSpecialAbilities.h, CWorld.h, CEnvironment.h, ThingAlive.h).
- CTCUnholyPower_FinalCleanup_00795090.cpp (`// Flags: /d2inlT62`, map iteration + ResetCreatureHitState + FadeOrKillEffect + SetLightingChannel + TC 0x75 idioms).
- CTCUnholyPower_CancelForCutscene_00794e60.cpp (`// Flags: none`), _FinishMove_00794c10.cpp (CTHING_GETTC_EXPANDED; inline StopSound note).
- CTCDivineWrath_* 0x0078E7C0.. (the twin TU) -- same idioms.
