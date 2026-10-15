# Hand chunk 62 (COpinionSourceDef .. CParentDefClassBase): 37 of 40 byte-exact

Every src/ row graded MATCH or RELOCATION_MATCH (grades.txt) and every test prints PASS under g++
(tall.sh) and under the lane's VC7.1 cl.exe (tall.py). Three rows are parked (register colouring /
entry-block scheduling only); none skipped. Most catalog labels in this chunk were propagated and are
corrected below; the sources are named by the real class wherever two independent sources agreed.

## What matched (37)

- **CParentDefClassBase::operator= family (15 rows, all re-attributed)** -- each row is the
  operator= of a concrete def class: slot 19 (`Copy`) of that class's vtable is a 5-byte
  `jmp <row>` sitting right before the body, and the class's Ego_r.pdb layout lands exactly on the
  copied offsets once the retail containers are 4 bytes smaller (PDB +0x40 -> retail +0x3c; vectors,
  maps and the base's CVectorMap are 12/16 bytes in retail): CSoundThemeDef 0045c54e,
  CCreatureGenerationFamilyDef 0045c614, CCameraManagerDef 0045d505, CHeroPostcardGeneratorDef 0045d5d3,
  COpinionReactionMaskDef 0045dd05, COpinionDeedMaskDef 0045e187, COpinionPersonalityDef 0045e660,
  CSpecialAbilities{DrainLife 004600cb, HealLife 00460131, GhostSword 00460185, TurncoatSpell 004601ea,
  FireballSpell 00460256, SummonSpell 00460457, ElectrocutedEffect 0046051d, CreatureTint 00460a19}Def.
  Shape: `#pragma optimize("s", on)`; base `CParentDefClassBase::operator=` (0x00431ED0) first, then the
  members in declaration order (containers/strings through their out-of-line operator=, floats as dword
  moves). Layout lessons: the retail CParentDefClassBase is a PACKED 0x39-byte polymorphic base (the
  header's 0x3d is the PDB's 16-byte vector map), so a derived `bool` lands at +0x39 and a dword at
  +0x3c; CRGBColour must be 4-byte aligned (a 4-`unsigned char` struct slides to +0x39); the math
  vectors C2DVector/C3DVector are byte-aligned (CHeroPostcardGeneratorDef has them at +0x39/+0x41/+0x49
  but its float at +0x58), a C3DVector member copies as `movsd` x3; a `float[4]` member copies through
  an FPU element loop (`for (i < 4) a[i] = o.a[i]`); COpinionPersonalityDef's operator= is hand-written:
  no base call, the owned trait table (5 x 36 bytes) copied in place LAST (`*p = *o.p`, `rep movsd` 0x2d).
- **CParentDefClassBase / CDefClassBase constructors** -- 00430370: CResource ctor out of line, the
  CDefPointeeBase/CDefClassBase ctors inline (dead vtable stores), `CDefString() : TablePos(-1)` and a
  16-byte `CVectorMap() : 3 x 0, Dirty(false)` member ctor (the vector map declared OUTSIDE the pack(1)
  region so it stays 16 bytes). 0042d9e0 (label CPSCBase) is CDefClassBase::CDefClassBase (vtable
  0x01230BCC): `: PDefManager(0) { Setup = Template = DefaultValsApplied = 0; }` folds to
  `and byte ptr [esi+24h],0F8h` before the vtable store.
- **NParticleEngine::CPSCBase / CPSCAttractor ctors** -- 00af2620 same shape as CDefClassBase.
  00b18020: bit-field initialisation must be in the MEM-INITIALISER list (a body assignment sinks the
  `mov al ; and ; or ; mov` read-modify-write to the end, after the array zeroes); 1.0f floats and the
  CArray's inline constructor then follow the vtable store.
- **NParticleEngine::CPSCManager ctor (00af2970)** -- inline vector-map constructor (ebx zero reused
  after the call), `RegisterPSCs()`, then an inline `if (Dirty) { sort(first, last, Compare); Dirty =
  false; }`: the empty comparator passed by value copies its byte (`xor eax,eax ; mov al,[esi+0Ch] ;
  push eax`) to the /Gr `__fastcall` SGI sort.
- **CPSCOrbit / CPSCSpline WriteBinary (00b163e0, 00b1aff0)** -- vtable slot 16; WriteULong +
  WriteData(Orbits, 0x90) through stream slot 4; two WriteBool, WriteFloat, then the base write.
- **CPSCSplineBase Initialise / GetPointAtPos (00b215a0, 00b21940)** -- retail CCardinalSpline is 0x14
  bytes, the counted pointers sit at +0x24/+0x2c. Initialise needed the position-param data pointer as
  a NAMED LOCAL (it stays in ecx across the member store and becomes `this` of Update; the counted
  pointer read twice reloads after the store); the bit-field return is `mov al ; shr al,1 ; and al,1`.
- **CPSCSingleSprite::ClearAllPrimitives (00b07570)** -- slot 29; /O2 index loop over PrimIDList
  (+0x12c) calling engine slot 30 (group handle, id) only when the handle's second word is live.
- **CPackedUIntArray (0066c000, 0066c150, 00a62a60)** -- `delete[] m_pData; m_pData = 0; m_count = 0;`
  (the two byte-identical twins, label propagated); Sizeof = `0x10 + ((bits * count + 31) >> 5) * 4`.
- **COptimisedFace::RemapIndex (00ac6f50)** -- three independent `if (v[k] == from) v[k] = to;`.
- **CPersistTraits<T>::TransferBinaryIn (0046bf1c)** -- the landed 0x004793F8 reader with the
  destination as the SECOND argument (`ret 8`, `this` unused; the caller passes ecx = a local traits
  object) for a 0x54-byte record.
- **NTCScriptedControl::CActionWait::CActionWait(long) (00903610)** -- inline CActionBase ctor zeroes,
  `UpdateCounter(0)` before the vtable store, the argument after it (plain init list).
- **CVolumeTexture forwarders (009fcd40, 00a69550)** -- `GetVolumeLevel(level).Helper(arg)`: VC pushes
  the member call's argument BEFORE evaluating the object expression, so the argument rides on the
  stack across the `ret 8` hidden-return call; the CVolume temporary's inline virtual destructor gives
  the vtable store + `ReleaseVolume(true)` + out-of-line `~CBase`.
- **Deleting destructor 00a6f9c0** -- non-polymorphic {CCharString, C3DMeshVolume} record deleted by a
  `delete` helper (leaf `??_G`); same counted-pointer release as the landed 0x00AF69D0 plus the key's
  ~CCharString.
- **InitCompStringData (00cb27f0)** -- DXUT ImeUi: two intrinsic memsets (0x200, 0x100) + two zero
  dword stores; external linkage so the TU emits it.
- **CPW_NecklaceDiggingSpot::Main (00ec09e0)** -- the landed CDoorToMothersTortureChamber::Main shape
  with the condition not negated; CGSI slot 578 SetDiggingSpotAsHidden(spot, false).
- **CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs (00ec3230)** -- `CCharString s; s = "LIT";
  ++index; return s;` (copy into the hidden result, local destroyed; no NRVO, no EH frame).

## Parked for Fable 5.1 (3)

- 004afc60 CQuestManager::IsCreatureGenerationEnabled DIFFER(65v63): retail `mov eax,[esi+44h] ; mov ebx,[esi+40h] ; sub eax,ebx` vs ours `mov ebx,[esi+40h] ; mov eax,[esi+44h] ; mov ecx,ebx ; sub eax,ecx` (entry block only; loop and exits identical) | tried: indexed `[i]` (61 bytes, element re-derived per iteration), running element pointer incremented in the for-header or in the body (65), optimize("s") on both (58/59) | next: lane gap: register copy/load order of a CSE'd first-iteration load (try /Oa or a `begin()` iterator object on the VC7.1 RTM box)
- 00acd900 COptimisedPrimitive::DetectFoldOver DIFFER(83v83): loop body `mov eax,[esi+4] ; lea ecx,[eax+edi*8] ; push ecx` + count in edx (retail) vs `mov ecx,[esi+4] ; lea edx,[ecx+edi*8] ; push edx` + count in eax (ours) | tried: CArray operator[], `m_pData + i`, signed `long` Size() | next: lane gap: register colouring
- 00b219a0 CPSCSplineBase::GetPointAtIndex DIFFER(86v86): inlined C3DVector copy `mov eax,[edi+10h] ; lea edx,[eax+ecx*4] ; mov ecx,[edx] ; mov eax,ebx ; mov [eax],ecx` (retail) vs `mov edx,[edi+10h] ; lea eax,[edx+ecx*4] ; mov edx,[eax] ; mov ecx,ebx ; mov [ecx],edx` (ours), plus the final params load in edx instead of eax | tried: POD assignment from the element, CArray operator[], by-value GetControlPoint accessor (all identical), user C3DVector::operator= (84 bytes, drops the `mov`) | next: lane gap: register colouring

## Skipped

None (no row spans two functions; every row's trailing bytes are int3 padding only).

## Label corrections

- 0045c54e, 0045c614, 0045d505, 0045d5d3, 0045dd05, 0045e187, 0045e660, 004600cb, 00460131, 00460185,
  004601ea, 00460256, 00460457, 0046051d, 00460a19 "CParentDefClassBase::operator=" -> the operator= of
  CSoundThemeDef, CCreatureGenerationFamilyDef, CCameraManagerDef, CHeroPostcardGeneratorDef,
  COpinionReactionMaskDef, COpinionDeedMaskDef, COpinionPersonalityDef, CSpecialAbilitiesDrainLifeDef,
  CSpecialAbilitiesHealLifeDef, CSpecialAbilitiesGhostSwordDef, CSpecialAbilitiesTurncoatSpellDef,
  CSpecialAbilitiesFireballSpellDef, CSpecialAbilitiesSummonSpellDef, CSpecialAbilitiesElectrocutedEffectDef,
  CSpecialAbilitiesCreatureTintDef (each: vtable slot 19 Copy = `jmp <row>` at row-5, plus the PDB layout).
- 0042d9e0 "CPSCBase::CPSCBase" -> CDefClassBase::CDefClassBase (stores vtable 0x01230BCC).
- 00903610 "CPSCSpline::CPSCSpline" -> NTCScriptedControl::CActionWait::CActionWait(long) (vtable 0x0128D760).
- 00ec3230 "COpinionSourceDef::COpinionSourceDef" -> CV_RandomPopulationSimScript::ChooseRandomGuardFromDefs
  (returns "CREATURE_BS_GUARD"; leaf inferred from the neighbouring ChooseRandomVillager/BanditFromDefs).
- 0046bf1c "COpinionTransientOffset::TransferBinaryIn" -> CPersistTraits<T>::TransferBinaryIn of a
  0x54-byte record (COpinionTransientOffset is 0x18 and its reader is the landed 0x004793F8).
- 004afc60 "COptimisedPrimitive::DetectFoldOver" -> CQuestManager::IsCreatureGenerationEnabled (this =
  quest-manager global 0x013B89FC; vector<CCharString> CreatureGenerationDisabledQuests at +0x40).
- 004af610 "CQuest_Array_FindByName" -> CQuestManager::IsQuestActive(const CCharString&) (walks ActiveQuests).
- 00cb27f0 "CPSCOrbit::DatabaseCopy" -> DXUT ImeUi InitCompStringData (caller sets `*trapped = TRUE` after it).
- 00a6f9c0 "CPSCEmitterGeneric::`vector_deleting_destructor'" -> scalar deleting destructor of a
  non-polymorphic {CCharString, C3DMeshVolume} record (pair<CCharString, C3DMeshVolume> is the only PDB fit; no references).
- 009fcd40, 00a69550 "CPaintMapDialog::InitNumberBox" -> CVolumeTexture forwarders around
  0x00A67EA0 (= CVolumeTexture::GetVolumeLevel, IDirect3DVolumeTexture9 slot 18; label
  "CPlayer::UninitModeQuestCompletionUI" also wrong); real leaf names unrecoverable (no references).
- 0066c000, 0066c150 "~CPackedUIntArray": a two-word {buffer, count} destructor; the real
  CPackedUIntArray destructor (0x00A629D0) also clears +8/+0xc -- kept under the label, flagged.
- 00b21850 "CEngineScreenEffectColourFilter::Render" -> CSplinePosParamData::TransformFromPos (between
  Update 0x00B20BA0 and TransformFromIndex 0x00B218B0).
- 00432b38 "CTCCarryable_assign" -> multimap<CCharString,long>::operator=; 0045c64d, 0045dd2c, 0045e1ae,
  0045e694, 0045ef2d, 00432ee9 -> the vector/map operator= of the members listed in the sources
  (vector<ECreatureGeneratorGenerateType>, map<EOpinionReactionType,bool>, map<EOpinionDeedType,bool>,
  map<EOpinionAttitudeType,bool>, vector<float>, vector<CCharString>).
