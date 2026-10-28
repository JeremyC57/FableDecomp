# Hand chunk 510 (CPlayer .. CFileInstaller::CBufferChunk vector): 16 of 20 byte-exact

All 16 are RELOCATION_MATCH. None are retail_asm. Three rows are parked and one is skipped. Every matched row has a test that prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). The vland dry run gave WINS 16, with no RELOC_IDENTITY and no DIFFER.

## Names

### Proven
- 00635e90 CPlayer::InitModeUseProjectileWeaponThirdPerson (Ego_r.pdb; mode-table store 0x0121B648)
- 00636d40 CPlayer::UninitModePCProjectileWeaponThirdPersonAiming (Ego_r.pdb; mode-table store 0x0121B868)
- 0067d450 std::vector<CBlurredAttitudesDef>::operator=
- 0067f280 std::__introsort_loop<CDegradableInfo*, CDegradableInfo, int, CDegradableDef::CIsHealth1GTHealth2> (bound)
- 006a3950 std::vector<NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser>::_M_insert_overflow
- 006d4850 std::vector<std::pair<CCharString,long> >::_M_fill_insert (Ego_r.pdb CPersistContext_TransferableVector<pair<CCharString,long> >::Resize is its caller)
- 00728df0 std::vector<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>::_M_fill_insert
- 007805d0 NFire::CFirePoint::SetAsSmoldering (Ego_r.pdb `?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ`, bound by the landed AddFireDamage 0x00780800) -- parked
- 008066a0 std::__find<NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint>
- 0085eaa0 std::vector<std::vector<NThingMap::CThingMapCell> >::_M_insert_overflow
- 0086cd70 std::vector<std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> >::_M_fill_insert
- 00889b20 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>::_M_insert_overflow
- 008aac50 std::__find_if<C3DVector*, std::unary_negate<CVectorFilter_FurtherFromPointThan> > (bound) -- parked
- 008ad5f0 std::vector<CScriptThing>::_M_insert_overflow
- 008f6a90 std::vector<CHeightMapCell>::operator= (only caller CHeightMap::Resize 0x008F61A0)
- 009b9320 std::__introsort_loop<pair<unsigned long, CDefinitionManager::CDefClassInfo>*, ..., CKeyPairCompareLess<..> >
- 009c73e0 std::vector<CFileInstaller::CBufferChunk>::_M_insert_overflow

### Probable
- 006fb240 std::__find_if<NAnimationEvents::CEvent*, CFilter_Bind2<NAnimationEvents::CIsEventIndexEqualTo, NAnimationEvents::CIsEventTimeEqualTo> >. Ego_r.pdb has `_Find_if` over exactly this predicate. The 8-byte element is the PDB CEvent (Index:30 compared with `and 3FFFFFFFh`, Time at +4). The 8-byte predicate holds two references, and the test is `fabs(dt) < 1e-4` (0x0129BA3C). Callers are 0x006F952A and 0x006FB62F.
- 00780430 CTCFlammable::GetClosestFirePointTo(const C3DVector&, bool). The name is in the PDB name set (private; Dinkumware returns an iterator, STLport a `CFirePoint*`). Address evidence: the CTCFlammable TU sits between SetAsBurning 0x780540 and the ctor 0x780820, it reads the vector at +0x10 with stride 0x2C (CFirePoint), it has two arguments (`ret 8`), and its caller is 0x0078086A. The bool is not read in the retail body. Parked.

### Descriptive
- (none)

## What matched (16)
- **CPlayer** (00635e90, 00636d40): mode init and uninit add or remove the input processes in retail order, then add or remove creature mode 0x19.
- **STLport vector `_M_insert_overflow`**:
  - 0085eaa0, 009c73e0, 00889b20, 008ad5f0, 006a3950 are STLport 4.6 verbatim.
  - `__false_type` needs a user ctor (009c73e0), or VC stores `mov byte [..],0`.
  - Out-of-line helpers whose tags home in dead slots need same-TU `#pragma auto_inline(off)` + `__declspec(noinline)` bodies (00889b20).
  - 006a3950's element is the 9-byte pack(1) CThingGraphicArgPasser.
- **STLport vector `_M_fill_insert`**:
  - 006d4850, 00728df0, 0086cd70 are verbatim.
  - In 006d4850, `__copy_backward` is out of line and `__copy_backward_ptrs` is inline.
- **STLport vector `operator=`**:
  - 0067d450 needs `__forceinline` on __copy, __copy_ptrs, __uninitialized_copy and _Construct.
  - 008f6a90 uses STLport's own `(const_pointer)` casts, so its helper is the PBV instantiation (see Label corrections).
  - 008f6a90 now includes `engine/CHeightMapCell.h`, which still matches.
- **STLport `__introsort_loop`** (0067f280 with explicit `__fastcall`, 009b9320 with `// Flags: /Gr`): `__median` and the comparator are inline; `__unguarded_partition` and `__partial_sort` are declared only.
- **STLport `__find` / `__find_if`** (008066a0, 006fb240): the landed 0x008060C0 body.
  - 008066a0 needs the non-const `CTargetPoint::operator==` (QAE per PDB).
  - In 006fb240 the predicate's key load is hoisted out of the unrolled loop and reloaded in the remainder. VC does this on its own; no source trick was needed.

## Parked for Fable 5.1 (3)
- 00780430 CTCFlammable::GetClosestFirePointTo DIFFER(273v271): retail keeps `closest` in ebp and spills the appearance pointer to the frame; this build keeps the appearance in ebp and spills `closest` | tried: one shared helper (frame 0x10), a block-scoped first helper (VC shares the slot), `closest = begin; it = closest + 1` (271v271, frame shared, `lea esi,[ebp+2Ch]`), a function-scope first helper (frame right, registers swapped) | next: the `it = closest + 1` iterator form together with the function-scope first helper
- 007805d0 NFire::CFirePoint::SetAsSmoldering DIFFER(300v299): the "SINGE" conditional temporary differs. Retail keeps its construction flag in a frame word (zeroed at entry), the `&&` result in bl (`mov bl,1 / xor bl,bl`), and builds the string before both argument pushes. This build keeps the flag in ebx and the result in a frame byte | tried: `if (A && B)`, `bool found = A && B; if (found)`, the wrapped `if (!p || dead) {...}` form, an inline bool helper (goes out of line, 224 bytes) | next: GOTCHAS wave 25 chunks 201-202 (the condition written straight into a loop gives a frame-word flag plus a bl condition)
- 008aac50 std::__find_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> > DIFFER(411v411): the first inlined predicate copy squares dy before dx (`fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3)`); retail does dx first (`fld st(1) ; fmul st(2)` x2). The other six copies match | tried: C2DVector delta + radius local, dropping the radius local | next: lane gap: x87 operand order in one of several identical inline copies

## Skipped
- 0074b77c "CTCCreatureGenerator_SpawnInArea": not a function entry. The real entry is 0x0074B770 (`mov al,[esp+4] ; sub esp,0Ch ; push ebx ; xor ebx,ebx ; cmp al,bl`), which Ghidra cut into a 12-byte head "InitialiseCreatureGenerator" plus this 383-byte tail. The tail starts on a flags-dependent `jne` and ends `pop ebx ; add esp,0Ch ; ret 4` for registers the head pushed. Fix the boundary: one 395-byte function at 0x0074B770, CTCCreatureGenerator::InitialiseCreatureGenerator(bool) per the catalog label (its callee 0x0074D940 is the landed GetAllThings<CThingMarker,...>).

## Label corrections
- 0x0085DE40 is vector<NThingMap::CThingMapCell>::vector(const vector&). It is landed and bound as `CUnprovenEnum4_0085de40` / `CElement12_0085de40`.
- 0x005BC0D3 is ~vector<CEngineTextureMorph>. The catalog has `_global::StdList_Dtor_005bc0d3`, and it is also bound under CActiveFile::OnReadFinished and member-dtor stand-ins.
- 0x0067C490 is CBlurredAttitudesDef::operator=; it is landed as CCardDef::operator=.
- The landed push_back 0x006A3C70 calls its element CScreenFilterSThingByPass, but it is CScreenEffectPriorityStack::CThingGraphicArgPasser (9 bytes; CScreenFilterSThingByPass is 16 bytes in Ego_r.pdb).
- 0x008F67C0 is `__uninitialized_copy<const CHeightMapCell*, CHeightMapCell*>` (PBV): STLport operator= passes const_pointer. It is landed with the PAV spelling, which belongs to its byte twin 0x008F6800 (bound by the landed _M_fill_insert 0x008F6C90). Rename the landed 0x008F67C0 to `??$__uninitialized_copy@PBVCHeightMapCell@@PAV1@@std@@YIPAVCHeightMapCell@@PBV1@0PAV1@ABU__false_type@0@@Z`.
- 0x00487DC0 is labelled `CPlayer::GetHeroCreature`. These sources call it CPlayer::GetPControlledCreature (Ego_r.pdb order, next to PeekPControlledCreature 0x00487DD0).
- Catalog labels for this chunk's rows are propagated or behavioural and are wrong:
  - 0x008F6A90: CTCSmokeGenerator::CGenerationPos (20 bytes; the cell is 12).
  - 0x0067D450: CConversation.
  - 0x006D4850: `_Insert_range_fill_8byte`.
  - 0x00728DF0: `vector_InsertMultiple`.
  - 0x0086CD70: `Insert_range_C3DClothQuad`.
  - 0x008066A0: `find<CBaseIntelligentPointer*>`.
  - 0x009C73E0: `vector_reserve_pair8byte`.
  - 0x0085EAA0: `Reallocate_element_init`.
  - 0x00889B20: `_Allocate_impl`.
  - 0x009B9320: `introsort_partial_24byte_quicksort`.
- 0x00780430 is CTCFlammable::GetClosestFirePointTo and 0x007805D0 is NFire::CFirePoint::SetAsSmoldering. Both were gap-scan placeholders.
- 0x0074B77C should be merged into 0x0074B770 (see Skipped).

## Header debt
- 009b9320: CDefinitionManager has to stay a local model. engine/CDefinitionManager.h has no nested CDefClassInfo, and it gives the PDB 0x10 CVectorMap, where retail has a 20-byte CDefClassInfo (CArray 16 -> 12). vland warns SHARED_HEADER_AVAILABLE.
- 00780430 / 007805d0: CTCFlammable, NFire::CFirePoint and CThing are local models, as in the landed CTCFlammable TU rows (no CTCFlammable header).

## Callee names used (one name per address)
- 0x00669580 `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z`. This is the Ego_r.pdb spelling; it is not yet bound, and the bindings hold several stand-ins. Used only by parked 007805d0.
- 0x00669D30 `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` (PDB, bound).
- 0x006E0880 `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` (PDB, bound).
- 0x006E0BE0 `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` (PDB, bound).
- 0x004C0560 `?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z` (PDB, bound).
- The other callees follow the landed or bound spellings recorded in each source's `// Callees:` line.
