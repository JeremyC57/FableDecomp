# Hand chunk 624 (CTCCreatureOpinionOfHero .. std::__uninitialized_fill_n<CTargetPoint*>): 6 of 10 byte-exact

All 10 rows were parked same-length retries; 6 now match (1 MATCH, 5 RELOCATION_MATCH), 4 stay parked.

## What matched (6)
- **STLport `std::__uninitialized_fill_n` (4 rows: 0x00722BD0, 0x00792550, 0x007979B0, 0x00806C80)** -- placement
  copy-construct n records from x, return first + n. Real instantiations, proven from the only caller of each
  (the vector's `_M_insert_overflow`, bound in reloc_bindings) and the landed `__uninitialized_copy` sibling of the same
  vector: CTCSmokeGenerator::CGenerationPos (0x00723110 / copy 0x00722B70), CTCDivineWrath::PerEffectInfo (0x00793370 /
  0x007924F0), CTCUnholyPower::PerEffectInfo (0x007981B0 / 0x00797950), NPlayerTargeting::CTargetPoint (0x008092D0 /
  0x00806C00; 0x00806C80 is bound to exactly the symbol we emit). Element layouts from Ego_r.pdb; class keys from
  pdb_class_keys.tsv (CGenerationPos/CTargetPoint class, PerEffectInfo struct). SHAPE: the old residue (cur and x swapped
  between ebx and edi) disappears when the intelligent pointer's `__forceinline` copy constructor names its item:
  `CBaseObject* item = other.GetPItem(); SetPItem(item);` (the landed 0x007B8230 model). The unnamed
  `SetPItem(other.GetPItem())` is what all earlier attempts used.
- **CTCHeroPodium::GetCrowdPos 0x00771D50** (Ego_r.pdb `?GetCrowdPos@CTCHeroPodium@@QAE_NABVC3DVector@@AAV2@@Z`; the second
  parameter is a reference) -- false at 8 occupants, else crowd_pos = the best vacant position, slot marked, count bumped.
  SHAPE: the 16-byte frame is the inlined PDB helper `std::pair<C3DVector,long> GetBestVacantPositionForThisPos(const
  C3DVector&) const` returning `pair(C3DVector(0,0,0), 0)`. C3DVector's user copy constructor (FableWin/Ego_d
  `??0C3DVector@@QAE@ABV0@@Z`) keeps the whole pair in the frame. Without it VC7.1 scalarises the pair (`sub esp,0Ch`).
- **CGraphicDataBank::FrameEnd 0x009FD660** -- unless frozen: CResourceBank::FrameEnd, CTextureManager::FrameEnd, then the
  inlined UpdateAvailableMemory. SHAPE: `AvailableMemory = Max(ResourceList.GetFreeMemory(), 0L);` through a BY-VALUE
  `template <class T> T Max(T a, T b) { return a > b ? a : b; }` puts the clamp result in the value register
  (`and eax,edx`) and keeps the store before the ReduceSizeFrameDelay load. Shared headers: engine/CResourceList.h and engine/CTextureManager.h
  (the `#define Class ClassLayout` include pattern) plus engine/CCountedPointer.h; vland shows no warnings. Named-local ternary, if-assign, explicit mask
  and by-reference std::max all give `and edx,eax`. GetUnloadableMemory is const (PDB QBEKK).
  Name note: `Max` is descriptive. It is fully inlined, so there is no symbol to prove its name; the landed tree uses the
  same helper (0x00725A30, 0x007267C0).

## Parked for Fable 5.1 (4)
- 006817f0 CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion DIFFER(84v84): `sub eax,2` (retail) vs `add eax,-2` (ours) as the switch bias | tried: long/unsigned/enum/inline-helper/explicit `x-2` switch, every accepting case enumerated, if-chain (87 bytes, compare chain), QFE-4035, /Os, /O1, /Ox, /Op, /Oa, /Ow, /G5, /G6, /Gy, /Ob1, optimize("g",off) | next: lane gap: switch-bias canonicalisation (only this TU's two switches, 0x0068180E and 0x0068186E, use `sub` in the whole exe); a /GL (LTCG) build is the one untested lead (the lane cannot grade LTCG objects)
- 00bba3e0 CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace DIFFER(87v87): `faddp` and `fsqrt` each issue one integer move later than retail (same instructions) | tried: named x/y locals, inline length helper, single expression, float local order, memberwise operator=, implicit copy, named sphere result local, `(float)sqrt(double)` | next: lane gap: x87/integer scheduling
- 004310a7 CPersistContext::Transfer<CWideString> DIFFER(91v91): traits default string at [ebp-8] and read temporary at [ebp-4] (retail the reverse; 6 `lea` operands) | tried: plain local (earlier), CPersistTraits<CWideString> traits object with the PDB TransferBinaryIn/Out inlined, by-value TransferBinaryIn overload, inline TransferBinary helper holding traits+switch, read in the body, explicit instantiation, reversed case order, QFE-4035 | next: lane gap: frame-slot order of two same-size class locals (try a permutation search over extra dummy scopes if a new lever appears)
- 00431143 CPersistContext::Transfer<CCharString> DIFFER(91v91): same slot swap as 004310a7 | tried: same list | next: lane gap: frame-slot order (twin of 004310a7)

## Skipped
- none

## Label corrections
- 0x00722BD0 "Vector_Fill_CountedPtr_Pairs" -> `std::__uninitialized_fill_n<CTCSmokeGenerator::CGenerationPos*, unsigned int, CTCSmokeGenerator::CGenerationPos>`.
- 0x00792550 "Vector_CopyConstruct_CBaseIntelligentPointer" -> `std::__uninitialized_fill_n<CTCDivineWrath::PerEffectInfo*, unsigned int, CTCDivineWrath::PerEffectInfo>`.
- 0x007979B0 "CopyArray_CBaseIntelligentPointer" -> `std::__uninitialized_fill_n<CTCUnholyPower::PerEffectInfo*, unsigned int, CTCUnholyPower::PerEffectInfo>`.
- 0x00806C80 "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" -> `std::__uninitialized_fill_n<NPlayerTargeting::CTargetPoint*, unsigned int, NPlayerTargeting::CTargetPoint>` (bound spelling).
- 0x004310A7 / 0x00431143 "TransferWideString" / "TransferString" -> the nameless `CPersistContext::Transfer<CWideString>(CWideString&)` /
  `Transfer<CCharString>(CCharString&)`. Spelling follows the binding of 0x00431143 and the landed Transfer<float>/<long>/<unsigned long>
  rows. The PDB overloads all take a name, so this TLC overload has no PDB symbol (name_review).
- 0x009FD660 has a second, wrong binding `?FrameEnd@CGraphicDataBank_009f8200@@QAEXXZ` (from 0x009F8200), and 0x009FC070 is landed non-const: see NAME_FIXES.tsv.

## Names and evidence
- Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem 0x00A01B10/0x00A01B50/0x00A01B90 and the vtable 0x01238C6C
  (RTTI `.?AV?$CIntelligentPointer@VCThing@@@@`) use the landed spellings. CResourceBank::FrameEnd 0x009FC430 and
  CTextureManager::FrameEnd 0x00A698B0 are bound. CResourceList::GetUnloadableMemory 0x009FC070 is QBEKK in both PDBs.
  CTCBase::PeekWorld 0x00686A80 uses the landed spelling. COpinionReactionManager::GetThingAttitudeToPlayer(const CThing&) const
  0x00701840 is from Ego_r.pdb. CPersistContext::CheckCRC 0x00404500 is IAEXPBD (PDB). CCharString/CWideString
  operator= are QAEABV0@ABV0@@Z (PDB).
- CDataInputStream's virtual order (~, SetPosition, GetPosition, Close, ReadCompressed, ReadZlibCompressed,
  ReadNullTerminatedString, ReadNullTerminatedWString) comes from the RTTI vtables 0x0129A6B0/0x0123675C plus the landed slot rows.

## Header debt
- engine/CTCCreatureOpinionOfHero.h carries the Ego_r (Dinkumware, 12-byte list) offsets. Retail HeroOpinionEnemy is +0x342,
  not +0x3E2, so 006817f0 uses a local model (differ only).
- engine/CDataInputStream.h flattens the vptr (no virtuals), so the persist rows keep a local stream model.
- No engine header exists for CTCHeroPodium or CPersistContext. The 00bba3e0 differ keeps its earlier local decal model.

## Bonus (not chunk rows; in extra/, review at landing)
- extra/007af940_from_chunk622.cpp: chunk 622's parked `__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*>`
  now grades RELOCATION_MATCH with the named-item lever. Its test is in chunk 622.
- extra/009fd3a0_retry1.cpp: the parked twin `CGraphicDataBank::UpdateAvailableMemory` grades RELOCATION_MATCH with the
  by-value Max clamp. It needs a test and must be made private (PDB AAEXXZ).
