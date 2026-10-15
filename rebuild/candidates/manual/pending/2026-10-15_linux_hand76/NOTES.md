# Hand chunk 76 (CSmallVector .. CSpellContainerList): 33 of 40 byte-exact

Authored by agent76 (wave 25, Linux Wine lane, DDK 13.10.4035 cl.exe), graded with `q.py` (`grades.txt`: 33 MATCH /
RELOCATION_MATCH), behaviour-tested with g++ (`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`): 33/33 PASS under both.
No pragmas or flags were needed for any landed row.

## What matched (33)
- CSmallVector (label wrong): 006fb960 is SGI `__unguarded_linear_insert` (/Gr `__fastcall`, `last` in ecx, the 12-byte
  pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32>> and the empty comparator on the stack, `ret 10h`) with the
  inline CSmallVector `operator=` (`&other != this` then the out-of-line Copy 0x006FB5C0) and the by-value parameter's
  `delete[] m_pData` at the end. SHAPE: the value key must be read into a named LOCAL before the loop -- retail keeps it in
  ebx across the Copy calls; comparing `value.first` directly makes the lane keep it in edx and reload the parameter slot.
- NCameraModes::CSnapToPoint / CTrack constructors (0056e330 / 0069df80, the second labelled CSnapToPoint): root
  NCameraModes::CBase(manager, def) out of line, the inline CCameraPointModeBase whose vtable store survives (its member
  constructor call follows), the CIntelligentPointer converting constructor inlined (base ctor, vtable, SetPItem), the
  derived vtable, then TrackThing = true (+0xc0) / ZTargeting = false (+0xc8). Retail offsets = PDB - 0x24.
- CSoundBankLH::vfunc_11 (00a0b8e0): `if (p) return p->Get(); return 0;` -- tail `jmp [eax+4]`, long result.
- Def constructors (size-optimised 0x0045xxxx def module): CSoundDef 00458413, CSpecialAbilitiesTurncoatSpellDef 00457a83
  (label CSoundDef), CSpecialAbilitiesDrainLifeDef 004578de (label BattleChargeDef). SHAPE: the PDB members shifted to the
  retail build -- the CParentDefClassBase root ends at +0x3c (PDB +0x40) and a retail vector is 12 bytes -- modelled as
  `CRetailVector<T>` member objects with inline constructors: `xor eax,eax` lands before the vtable store, the zero stores
  after it, exactly as retail, with the real PDB member names.
- 00f336f0 CVtable_012e5ff0 (no-RTTI, base 0x0042D9E0) and 00c5d0a0 CVtable_012b8fc4: zeroed block = one embedded member
  object with an inline constructor.
- CSoundEmitterLH: vfunc_23 (00aabc50) -- ONE shared `xor al,al` exit for the system null test and the exhausted walk means
  the loop is nested inside `if (m_pManager->m_pSystem) { ... } return false;` (an early `return false` gets its own exit);
  vfunc_9 (00aac400) -- stop every active sound whose handle matches through system slot 6 -> player slot 22, mark it -1,
  then prune (0x00AABCA0), all inside the system test. +0x04 is the owning CSoundManagerLH (its +0x40 is the sound system).
- CSoundInit-labelled bodies (all labels propagated): 0041c930 a POD record whose +0x10 word is assigned in the BODY (stored
  after the initialiser list); 00a248f0 C3DMeshInfo(void*, a, b) -- root-base words before the vtable, the bounding sphere
  zeroed as dwords by its member constructor and the radius overwritten with -1.0f in the body (dead store kept);
  00c8db30 `free(m_pBuffer); memset(this, 0, 32);`; 00c8df70 `memset(this, 0, 32); codec_setup = calloc(1, 0xE80);` --
  libvorbis `vorbis_info_init` (the exe links "Xiph.Org libVorbis I 20030909"): a 32-byte intrinsic memset is eight
  zero-register stores.
- CSoundManagerLH: vfunc_1 (00a3e430, `new CSoundBankLH(this)` + slot 1, the tail duplicated into the null arm), vfunc_49
  (00a3e480, nested `if` + `== 2` bool), vfunc_6 / vfunc_10 (00a3ec70 / 00a3ec90, STLport list size by the SGI `__distance`
  walk; `count < limit` as a `long` via `sbb/neg`, `limit - count`), vfunc_38 (00a3ed90, lookup then a six-argument slot-6
  call with a literal 4.0f; `return r >= 0;` gives the full-register `setge dl ; mov al,dl`), vfunc_40 (00a3eed0),
  vfunc_52 (00a41800, implicit 12-byte vector assignment: `add ecx,74h` + three dword moves).
- CSoundReceiverLH vfunc_1 / vfunc_3 (00aad060 / 00aad090): position (+4), up (+0x1c) then forward (+0x10) assigned
  bitwise through `lea eax,[ecx+N]`, then Commit (0x00AACFB0) on `this`.
- 00c5ed90 (label BattleChargeDef): the CWound_00c5eca0 constructor matching the landed destructor 0x00C5ECA0 model -- the
  derived vtable first, five inline Win32 holder members (two -1 handles, two null handles, the critical section through
  `InitializeCriticalSection`), then four body words in source order.
- 009a9ad0 (label DrainLifeDef::operator=): the copy CONSTRUCTOR of a pair { CCharString; { 4 CWideString, long } } --
  re-based `lea edi,[ebx+4]` / `add esi,4` pointers = a nested record with its own inline memberwise copy constructor.
- 00af8b40 (label GhostSwordDef::operator=): CDynamicObjectHeap<116,529>(bool) -- FreeList +0x14, StartBlock.Blocks[529]
  +0x18, StartBlock.NextEntry +0xEFCC, AllowHeapToGrow +0xEFD0; the 529-entry push-front loop. SHAPE: the flag store comes
  first through al = the flag is assigned FIRST in the body (as an initialiser it is loaded into cl and stored after
  FreeList).
- 00b39f30 (label SummonSpellDef): CEnginePrimitiveAttachmentManager -- two base constructors (0x0099A2F0, CEngineComponent
  0x00B59710 at +4), both vtables, then the 4-byte STLport `AttachedPrimitives` list (`m_node = 0`, node malloc'd, linked
  to itself, published last).
- 00e99890 CSpectator::Init: FacingThunder = true, then game slots 516 / 517 on &Thing (518-slot interface generated).
- 00544b70 NUISystem::CSpellContainerList(long): CChangingStateComponent base, three vtables, SpellSelectionChild /
  SpellContainers / InputDelay cleared (PDB members mapped to retail by the base end +0x15c), GetDataFromDef in the body.
- AddChild-labelled forwarders (labels wrong): 00698b20 CGameCameraManager::PlayCameraSound (PObservedThing +0x100,
  PDef +0x134 -> `__fastcall` 0x00698A30), 006c27a0 a three-assignment setter (vector<28-byte> at +0x10, tree at +0,
  word at +0x1c), 009a8150 `m_entries[key] = value` -- the VALUE is pushed first and left on the stack across
  `operator[]`, so the parameter order is (value, key).

## Parked for Fable 5.1 (6)
- 004570d3 CSpecialAbilitiesBattleChargeDef::CSpecialAbilitiesBattleChargeDef DIFFER(61v61): `lea eax,[esi+90h]` placed before the +0x58 zero store (retail: before +0x60) | tried: SZeroed member object (65 bytes), body assignments, optimize("s") + PDB CRetailVector members (61v61), allocator-tag vector ctor | next: lane gap: lea scheduling (24i)
- 00457da9 CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef DIFFER(58v58): `lea ecx,[esi+64h]` hoisted above the three SummonTintColour zero stores (retail: before the last) | tried: optimize("s") (58v58), no pragma (61, -1 as a 7-byte mov) | next: lane gap: lea scheduling (24i)
- 00458c19 CMeleeCombatAbilityDef::CMeleeCombatAbilityDef DIFFER(45v45): `lea ecx,[esi+68h]` hoisted above the ValidWeaponClasses zero stores | tried: optimize("s") init-list vector, body-assignment vector ctor | next: lane gap: lea scheduling (24i)
- 004e00bc CQuestCardDef::CQuestCardDef DIFFER(45v45): `lea ecx,[esi+5Ch]` hoisted above the RewardObjects zero stores | tried: optimize("s") init-list vector | next: lane gap: lea scheduling (24i)
- 0045cfad CSoundMap::operator= DIFFER(49v49): last Dirty byte copied through cl with `pop edi ; mov eax,esi ; pop esi` between load and store (retail: through al before the pops) | tried: explicit memberwise outer + inner operator=, implicit outer operator=, void inner operator=, optimize("s") (out-lines the inner operator=, 33 bytes) | next: lane gap: register/scheduling (try `__forceinline` inner operator= under optimize("s"))
- 005bc6d6 CUIBackgroundOwner_005bc6d6::RenderBackground DIFFER(20v23): float argument re-pushed `mov eax,[esp+4] ; push eax` (retail `fld [esp+4] ; push ecx ; fstp [esp]`) | tried: plain float forwarder, /Op, 4-byte float wrapper with a user copy ctor, optimize("g",off) | next: optimize("s") (the unaligned 0x005BC6D6 start suggests a /Os TU), else lane gap: float argument copy

## Skipped
- 00aacf90 CSoundReceiverLH::vfunc_2: the 32-byte row holds two functions (`mov eax,[ecx] ; jmp [eax+4]` at 0x00AACF90 and
  `lea eax,[ecx+4] ; ret` at 0x00AACFA0 after int3 padding).

## Label corrections
- 00457a83 CSoundDef::CSoundDef -> CSpecialAbilitiesTurncoatSpellDef ctor (vtable 0x0123453C; slot 20 is 0x00457AC4).
- 004578de CSpecialAbilitiesBattleChargeDef -> CSpecialAbilitiesDrainLifeDef ctor (vtable 0x01234404).
- 00f336f0 CSpecialAbilitiesBattleChargeDef -> CVtable_012e5ff0 ctor (no-RTTI vtable, base 0x0042D9E0).
- 00c5ed90 CSpecialAbilitiesBattleChargeDef -> the constructor of the class whose destructor is landed at 0x00C5ECA0 (CWound_00c5eca0).
- 00458c19 CSpecialAbilitiesSummonSpellDef -> CMeleeCombatAbilityDef ctor (vtable 0x01234D5C); 004e00bc -> CQuestCardDef ctor
  (vtable 0x01241E44); 00b39f30 -> CEnginePrimitiveAttachmentManager ctor (vtables 0x012A1AB0 / 0x012A1A78).
- 0069df80 CSnapToPoint -> NCameraModes::CTrack ctor (vtable 0x0125C50C).
- CSoundInit (all five rows propagated): 0041c930 a POD record ctor; 00a248f0 C3DMeshInfo ctor (vtable 0x0129CD3C);
  00c5d0a0 CVtable_012b8fc4 ctor; 00c8db30 a free+memset Clear(); 00c8df70 libvorbis `vorbis_info_init` (neither is a ctor).
- 006fb960 CSmallVector::Erase -> SGI `__unguarded_linear_insert` over pair<unsigned long, CSmallVector<CEvent,32>>.
- 009a9ad0 CSpecialAbilitiesDrainLifeDef::operator= -> copy ctor of pair<CCharString, {4 CWideString, long}>.
- 00af8b40 CSpecialAbilitiesGhostSwordDef::operator= -> CDynamicObjectHeap<116,529>::CDynamicObjectHeap(bool) (vtable 0x0129F650).
- 0045cfad CSoundMap::CSoundMap -> CSoundMap::operator=.
- CSpellContainerList::AddChild (four propagated rows): 005bc6d6 a float forwarder to CUserProfileManager::RenderUIBackground;
  00698b20 CGameCameraManager::PlayCameraSound; 006c27a0 a vector/tree/word setter; 009a8150 a map<CCharString,CCharString> setter.
- Callees: 0x0045324F / 0x004DF7D3 (CDisplayManager::CopyBackBufferToTexture) are the CHeroAbilityStatPackage and the
  CQuestCardDef Prerequisites-map constructors; 0x0084E420 (CActionParam_Constructor) is NCameraModes::CBase(manager, def);
  0x00AACFB0 (CShaderRenderManager::UpdateFogColour) is a CSoundReceiverLH commit; 0x00AABCA0
  (CModeHoverBase::RemoveInactiveHoverAnims) is the CSoundEmitterLH inactive-sound prune; 0x0045CFDE / 0x0045D183
  (CActiveFile_AssignUndefinedVector12/8) are vector<pair<ulong,CSoundPair>> / vector<pair<ulong,CDefString>> operator=;
  0x006C2D40 / 0x006B9E00 (CCombatWheel_CopyAssign / LTextTreeWalkThrough_CopyAssign) are a 28-byte-element vector
  operator= and a tree operator=.
- The landed CSoundEmitterLH constructor 0x00AAC580 types +0x04 as a CGameScriptThing and calls 0x00A3E420 on it; the
  emitter's vfuncs read +0x04 -> +0x40 as the CSoundManagerLH sound system (cf. CSoundManagerLH vfunc_49), and
  0x00A3E420 sits inside the CSoundManagerLH code range: +0x04 is the owning sound manager.
