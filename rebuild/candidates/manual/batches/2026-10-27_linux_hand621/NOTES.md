# Hand chunk 621 (CParticleEmitter .. CAIStateGroup_BanditPlot): 1 of 10 byte-exact

Chunk 621 is a 10-row retry of rows already parked at the same length as retail (CHUNK_BRIEF.md). Every row started from its
best parked source (reproduced first, same grade as recorded) and got new levers.

## What matched (1)
- CPersistContext (1): 0x00756D00 `TransferMap<pair<long, long>, CCharString>(name, map, defaultValue)`: RELOCATION_MATCH 1116v1116,
  and vland (read-only, switch jump table included) prints RELOCATION_MATCH PASS. The text-load branch transfers the map value
  through the value traits object. The shape lesson: Ego_r gives `CPersistTraits<std::pair<long,long> >` an 8-byte `DefaultVal`
  at +0. The value is that DefaultVal: zeroed member by member (one `xor edi,edi`, two stores, and the zero register is reused by
  the later `cmp ecx,edi`), overwritten by the old entry or by the default, then `valueTraits.TransferIn(this, &parser,
  &valueTraits.DefaultVal)`, so retail's `this` == &value ([esp+0x44]). The old empty-traits model put `this` 4 bytes below
  (the parked 1116v1116 residue). Case 1 (text save) builds its traits object with no stores, as retail does: the real
  zeroing STLport `pair()` constructor is used and VC7.1 drops its two stores as dead.
  Respelled (coordinator follow-up): the source is now the real template member
  `template<class V, class K> void CPersistContext::TransferMap(const char*, std::map<K, V>&, const V&)`, explicitly
  instantiated for <std::pair<long,long>, CCharString> over a modelled STLport 4.6 std::map/_Rb_tree, with every callee in its
  PDB spelling. Still RELOCATION_MATCH 1116v1116 (19 normalised diff lines, all jump-table entries), vland RELOCATION_MATCH PASS.
  The one lever the respell needed: in the save loop, `iterator entry = it;` (a non-escaping copy) keeps the node in esi
  across the TransferOut calls. Using `it->` directly reloads the iterator from memory (1124v1116), and so do `value_type&`
  (1124) and key/value references (1120).
  Test PASS under g++ and VC7.1.

## Parked for Fable 5.1 (9)
- 00aeeba0 NParticleEngine::CParticleEmitter::StartFrame DIFFER(890v890): fld/fmul operand order of 8 of the 18 inverse cofactor products is mirrored (retail `fld E22 ; fmul E33`, ours `fld E33 ; fmul E22`), plus the fxch/mov schedule after Transform.Set and one `xor eax,eax` placement | tried: `const CMatrix3x4& transform = Transform` alias (identical); earlier sweeps: cofactor written twice, nested a11, GetInverseNoTranslate member form, mirrored operands | next: lane gap: x87 product operand canonicalisation (source operand order inert)
- 00a8a220 C3DMesh2::GetDummyObjectTransformAnimated DIFFER(965v965): term order now exact in all 24 rows; left: fld/fmul operand order inside about half the products (block 1 mixed; operator*= rows 2-4 all mirrored: retail `fld a.E2x ; fmul b`, ours `fld b ; fmul a`) | tried: sequenced retail-order sums transcribed from the x87 trace (268 -> 202 lines, kept), the same with every product's operands swapped (identical code) | next: find what sets VC's commutative-operand rank (the copy `CMatrix3x4 a = *this` may number every a-element before b; try a non-copy form of operator*= that still emits the rep movsd)
- 0088b0c0 GFLoadSkeletalMorphTargetFromFile DIFFER(971v971): `mov edi, vtable` held in a register (ctor store and an inline-dtor vptr store) with the file length in ebx; retail stores the vtable as an immediate and has no dtor vptr store | tried: compiler-generated dtor (970v971: never inlined, not even at the start-marker return), generated dtor emitted early by an explicit-call helper (identical) | next: lane gap: a generated (vptr-free) dtor inlined at only one of several exits
- 00a8a5f0 C3DMesh2::GetDummyObjectTransformAnimated (blended overload) DIFFER(981v981): as 00a8a220 (product operand order) | tried: sequenced retail-order sums (256 -> 204, kept) | next: as 00a8a220
- 0063d0a0 CWorldMap::FindGroundCollisionPointWithinCell DIFFER(1017v1017): `fld [esp+0Ch] ; push esi ; fmul` (retail) vs `push esi ; faddp ; ... fld` (ours) in the normalise prologue, and the end/hit/start pointer loads (retail ebp, esi, edi after the first normal product; ours esi, ebp, edi before the normal copy) | tried: block-scoped normal per plane test (90 lines), inline Normalise() (not inlined), __forceinline Normalise() (identical) | next: lane gap: prologue push interleave + hoisted parameter-load order
- 0063d4a0 CWorldMap::FindThemeCollisionPointWithinCell DIFFER(1017v1017): twin of 0063d0a0 (same two residues) | tried: the 0063d0a0 levers (bodies identical but for the GetThemeSizeZAt callee) | next: as 0063d0a0
- 00972f30 CAIStateGroup_JackOfBladesStage2::SetupArena DIFFER(1026v1026): /GS frame-slot layout. Retail: marker +0x10, track +0x14, tag byte +0x1B, intelligent-pointer temporary +0x1C, filter +0x2C, index +0x30, box +0x34. Ours sits 4-12 bytes lower (marker +0x0C, temporary +0x14, tag +0x1F) | tried: function-scope name buffer (identical); earlier: decl orders, name[32]/[40] | next: model `this` with the real class (the stand-in `CAIStateGroupJackOfBladesStage2Methods` mangles QAE, retail MAE) and the real CIntelligentPointer temporaries, then re-check the slot order
- 009482a0 CAIStateGroup_GoToPub::GetSeat DIFFER(1072v1072): the random index stays in edx (retail `div ebp ; mov eax,edx ; jmp` / `xor eax,eax`), and every return sets al (and the -1 store) after pop edi/esi/ebp, where retail sets it before | tried: signed index (identical), optimize("t") (identical), optimize("s") (730 bytes), /Oa (1100) | next: a GFRandom(seed, range) helper whose result is consumed by a counted walk helper (index as a returned value)
- 009289b0 CAIStateGroup_BanditPlot::Act DIFFER(1133v1131): `lea ebx,[eax+ebx+177h]` (endFrame coalesced with wait) vs retail `lea ebp,...`; result in ebp (retail ebx), and `cmp dword ptr [eax+0Ch],2` vs retail `mov ecx,[eax+0Ch] ; mov ebx,2 ; cmp ecx,ebx` (+2 bytes) | tried: endFrame declared first (identical), early return + `return Perform(OUTOF)` (worse), result as loop counter (identical) | next: keep `wait` live past endFrame's definition or compute endFrame in its own block

## Skipped
None (no row spans two functions).

## Label corrections
- 0x0063D4A0 catalog/manifest `CWorldMap::FindGroundCollisionPointWithinCell` is **CWorldMap::FindThemeCollisionPointWithinCell**.
  Evidence: Ego_r donor order Ground 0x006E9B50 -> Theme 0x006E9F10 (FableWin 0x024A8A10 -> 0x024A8ED0); the GetThemeSizeZAt
  0x0063A390 callee; its only caller is FindLastUnblockedThemePositionAndReportCollision 0x0063DB30 (NAME_FIXES.tsv).
- 0x00756D00 catalog `TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>` is
  **CPersistContext::TransferMap<std::pair<long,long>, CCharString>(const char*, map<CCharString, pair<long,long> >&, const pair<long,long>&)**
  (Ego_r `??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@QAEXPBDAAV?$map@...@std@@ABU?$pair@JJ@2@@Z`, donor 0x00768180;
  the landed 0x00757160 overload's only callee; it parses CCharString keys and two-word values).
- Manifest (catalog already right): 0x0063DB30 is labelled FindLastUnblockedGroundPositionAndReportCollision, but it is the Theme
  twin (Ego_r 0x006EA4D0, and it calls the Theme cell test). 0x00757160 is labelled `Transfer<C2DVector>`, but it is the landed
  TransferMap overload.
- 0x00756350 (the map call in 0x00756D00) is `_Rb_tree::_M_find`, not lower_bound (see Names and evidence).

## Names and evidence (00756d00)
- The function: Leaf `??$TransferMap@U?$pair@JJ@std@@VCCharString@@@CPersistContext@@QAEXPBDAAV?$map@VCCharString@@U?$pair@JJ@std@@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@ABU?$pair@JJ@2@@Z`
  (Ego_r donor 0x00768180; landed no-default overload 0x00757160 calls only this row).
- Callee spellings in the object, all from Ego_r/FableWin PDB names: PersistError (QBE), SetAsFoundItem, CleanOutLine (IAE),
  CheckCRC (IAE), SkipPastWholeString, ReadAsSymbol, the CStringParser copy constructor, `std::list<CStringParserCommentBoundaries>::~list`,
  CPersistTraits<CCharString>::TransferIn/TransferOut (reference forms), CPersistTraits<std::pair<long,long> >::TransferIn/TransferOut
  (FableWin 0x025BD2E0 / Ego_r 0x00767690, defined in the TU), GFSerialiseMapBinaryOut/In (Ego_r 0x007677E0 / 0x00768000, exact),
  and the STLport tree members `_Rb_tree::insert_unique` (equal to the existing binding from 0x00756790), `erase(const key_type&)`,
  `clear`, `_Rb_tree_iterator::operator++` (equal to the existing binding from 0x00756790), and `_Rb_tree::_M_find`.
- 0x00756350 is `_M_find`, not `find`: the retail call pushes one argument and compares the link returned in eax with the header.
  The STLport 4.6 `find` is inline (`iterator(_M_find(k))`); an out-of-line `find` would return the iterator through a hidden
  pointer (two pushes). The catalog label is `std::_Rb_tree::_M_find`. Neither PDB lists this instantiation's tree members.
- The long helpers keep the existing landed __fastcall spellings: `TransferIn(CPersistContext*, CStringParser*, long*)` 0x009E2590
  and `TransferOutLong(CPersistContext*, CCharString*, long*)` 0x009E2560. The PDB has them as CPersistTraits<long>::TransferIn /
  TransferOut (FableWin 0x018986E0 / 0x01898790). Retail calls them __fastcall with no traits `this`, which no C++ member
  spelling reproduces. Left as review.
- vland now warns RELOC_NAME on only 2 callees, both wrong catalog labels: 0x009B9C00 'CStringParser::SeekToName' and 0x009BAB40
  'CTCCreditsUI::Deactivate'. NAME_FIXES.tsv has 9 rows for the landed callers' wrong spellings of these callees
  (Error / EntryTransferred / CleanText / SkipToItem / CCommentList / pointer-form traits / landed own symbols).
- Header debt: CPersistContext has no engine header (HAND_OWNED, the landed 0x00757160 / 0x00463334 models). CStringParser is the
  retail 0x28 layout; engine/CStringParser.h is the Ego_r 0x30 layout.
  0x00757160 / 0x00463334 models). CStringParser is the retail 0x28 layout; engine/CStringParser.h is Ego_r 0x30.
