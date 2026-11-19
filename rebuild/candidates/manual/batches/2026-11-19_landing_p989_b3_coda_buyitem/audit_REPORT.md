# Audit p989 b3 (0x00989230) / e17 (0x009FDE80) / CCoda3DSource Calc* re-ports (2026-11-19, Account B)
Repo = working tree at HEAD 8f084678b (mid-merge nf99a by another agent; touched headers clean). Strict = LANDVERIFY_STRICT_HEADERS=1.

## Part 1: p989 b3 0x00989230 CShaderRenderManager::UpdateObjectSpaceTextureTransform -- PASS (fixed_b3 = comment fixes + stale header dropped)
- Oracle 636 B == Fable.exe. Retail column order 3b1b2b0b|3b1b2b0b|0b3b1b2b|3b1b2b0b; b3 built = same.
- fixed_b3/hdr = ONLY b3's CMatrix4x4AlignedSSE.h (b3's CMatrix4x4.h is stale: it drops today's nested EGOD AlignedPreMultiplyTransposed block).
  Strict vland: RELOCATION_MATCH PASS (fixed_b3/vland_strict.log).
- Header: additive only vs today (row-macro #error pin + nested `#ifdef ..._RHSREF_00989230` arm); no FABLE_STATIC_ASSERT in the file (nothing to pin).
  hdrcheck2 is vacuous on this header (0 lines: body sits behind FABLE_ENGINE_CMATRIX4X4_H), so tools/ppcmp.py was used: every includer preprocesses raw-identical.
  The macro is defined nowhere else in rebuild/.
- Includer regrade (strict, overlay on): 00a5cb90(/Zi) 00a9d480 00a9d580 00a9d750 00a9e1e0 00aa0fa0 00aa2110 00adefc0 00ae1320 00ae29d0 00bd2d90: all
  RELOCATION_MATCH PASS; extra found 00a5cee0: strict HEADER_POLICY identical with overlay off (local class, pre-existing), non-strict RM PASS on/off.
  ppcmp: all 12 raw-identical.
- Locals = Ego_d (this; c_transform 0x22 {range {transform, rows 0x22}}). AlignedSetMultiply Ego_d has no `r` (hence Inferred).
- Names: member names = Ego_r layout. In-TU SetVSConstants copy spells `start`; Ego_d params are index/data/count + local `hr`
  (start->index RM, byte-neutral) -- kept verbatim to the landed 0x00989CB0 copy; see review items.
- Purity: classify = genuine (row spells AlignedSetMultiplyTransposed; helper tag lists only AlignedSetMultiply) -- same for landed
  00adefc0 00ae1320 00ae29d0 00a9e1e0 00bd2d90. Adding `AlignedSetMultiplyTransposed, AlignedPreMultiplyTransposed` to the
  `// Retail-asm-helper:` line of CMatrix4x4AlignedSSE.h grades all six retail_asm (probes/purtag).
- Inferred line / unit table: reproduced (probe.py): RHSREF off = 635 B, order 2a3a1a0a|...; global `if (G&K) G|=M` at top / before flags /
  after flags = retail order; member UpdateFlags if before flags = retail; member if AFTER the flag pair = third order (647 B) -> "anywhere"
  overclaimed (fixed wording); Ego_d `long hr` in SetVSConstants (fn scope and in-if) = 0 units; named ref in the row = 0; RHSREF + one live
  if = a different order (mod-4 consistent). Wording of the SetVSConstants provenance also fixed (it is 0x00989CB0's copy, not 0x0098B930's text).
- Mutants: author 9/9 killed; audit 7/7 killed + 3 equivalent survivors (SIMD flag inverted: both arms equal; clamp `<=`; stub count check).
  Test is a model (project convention; links self-contained).

## Part 2: e17 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading -- PASS after rebase (fixed_9fde80)
- Oracle 1265 B == Fable.exe (`ret 8`).
- e17's hdr is stale: today's repo has engine/CBankFileAsyncEntry.h (full Ego_d class under FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER) and
  GFTanh.h. e17's text on today's repo = SRC_FAIL C2027 (undefined CBankFileAsyncEntry). Rebase: drop e17's CBankFileAsyncEntry.h (and GFTanh.h,
  another row's) and add `#define FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER` to the row; CTextureManager.h and CBankFileAsyncData.h = today's file + e17's
  block only (pin_asserts: 0 to pin; CBankFileAsyncData's assert is already _AT(71)); engine/C3DExtentsI.h new.
- Strict vland: RELOCATION_MATCH PASS (WARN RELOC_NAME RemoveFromResourceList -> 0x009FC440 'CTCActivationTrigger::OnKill', known catalog label).
- Locals = Ego_d exactly (header, async_entry, mipmap_bytesize, managed_texture, allocate_success, texture_manager, data_size long, total_size ulong,
  dimensions C3DExtentsI 0xbcdd, pixel_format, max_level long, texture2, data_offset, texture; {c_level {current_size}}).
- Names: every symbol the object references is in Ego_r / FableWin / Ego_d sets except the unreferenced COMDAT
  ??$GFPSizeof@PAVCGraphicFrameData@@@@YIKABQAV... (FableWin: ??$GFPSizeof@QAVCGraphicFrameData...: shared GFSizeof.h takes `const T&`; pre-existing,
  same in landed 0x009FE460 / 0x009FF220). C3DExtentsI: FableWin ctors (JJJ / XZ / ABV0) + Ego_r layout (base 8, Z +8) agree.
- Purity genuine. Test: paraphrase model; mutants 16/17 killed -- `mip_count_clamp` SURVIVED (SmallMip never above mips-1). Added case 21 -> 17/17.
- Headers: hdrcheck2 default=same (CBankFileAsyncData 90 lines, CTextureManager 33); C3DExtentsI NEW. ppcmp: 136/136 includers raw-identical.
  Strict regrade with overlay, 44 rows (all 20 CTextureManager.h includers + 24 CBankFileAsyncData.h incl. 00a24d70 00a26ad0 009fed30 009d5bf0
  00be06b0 00a24520 + seeded sample): 35 RM/MATCH PASS; 9 HEADER_POLICY (009c80f0 009fd660 00a69c20 00b2aee0 00b2af50 00b2b820 00b2c460 00b3cb30
  00be06b0) identical with overlay off; non-strict all 9 RM/MATCH PASS on and off.
- REVIEW: once engine/C3DExtentsI.h is in rebuild/include, verify_and_land's SHARED_HEADER_AVAILABLE fires (strict HEADER_POLICY) for the landed rows
  that define a local `class C3DExtentsI {`: 00a67a20 00a685e0 00a67860 00a67950 00a67530 00a699d0 00a677d0 00a68910 009fd9b0 (00a68370's
  `: public C2DExtentsI {` escapes the regex). Same class of issue as C3DCoordI in the h3c audit.

## Part 4: hand4d b12 0x00774D20 CTCShop::BuyItem -- FAIL as delivered (same-TU copy not verbatim); fixed_774d20 PASS
- Oracle 1149 B == Fable.exe. b12 text on today's tree: RELOCATION_MATCH (strict).
- FAIL (GOTCHAS 455): b12's same-TU CTCShop::GetFreeSaleItemPlaces is not the landed owner 0x00774B40's text (b12: `erase(begin(), end())`,
  no dead `C2DBoxI area`, `CFilter_Bind2<..>(..)`; owner: `clear()`, `area`, `GFFilter_Bind2(..)`) and has no Fidelity line.
  fixed_774d20: owner body verbatim + `// Fidelity: same-TU strong definition of 0x00774B40 (retail TU-mate)` + the owner's FABLE_RETAIL_C2DBOXI /
  _RADIUS_INITIALIZERS opt-ins and C2DCoordI.h / C2DBoxI.h includes: BuyItem RM, the copy RM at 0x00774B40 in this TU; dropping the copy = DIFFER
  1153v1149 (byte-required). Test's pasted row copy synced.
- Paren: without it DIFFER 1149v1149; p6 `1.0f - (ptc_stats->GetMoralityPercentage())` RM (measured). Under the owner's 2026-11-19 bracket rule
  (EXTRA_RULES "grouping brackets are an accepted lever") a bracket takes ONE `// Bracket:` line and NO `// Inferred:`; b12 labelled it
  FITTED + inline `// Inferred:`. fixed_774d20 relabels it `// Bracket:` (address 0x007750E5, the p6 alternative) and keeps the Ego_d
  894/898 disclosure as `// Unreproduced:` (not byte-required; likely debug-only).
- Locals/scopes = Ego_d exactly (this, def; fn scope tc_hero_experience, ptc_stats, pdef, phero, shop_def, pshop_def, stock_item_def,
  def_class_base; {pshop_item}; {places}; {profit, ptc_inventory_base, bought_price {experience}}; {value {8 morality locals}}). Line table:
  842 one `if` (120) and 885/886/888/890 shapes as the text; tail 894 / 898 not written (disclosed).
- Names: ?BuyItem@CTCShop@@QAEXJ@Z (Ego_r + FableWin); NAME_FIXES row correct (catalog label CTCShop_SellHighlightedItem is the WARN STANDIN_LEAF).
- fable_map.h opt-ins INSERT_UNIQUE_HINT_BODY / MAP_ORIGINAL_INSERT_BODY exist in today's header (28 / 130 landed users); without them still RM
  (README claim holds). No header change.
- Purity genuine. Mutants: author 16/16 + audit 10: `ceil`->`floor` SURVIVED (all cases integral); added case 10 (non-integral product) -> 26/26.
- Strict vland fixed_774d20: RELOCATION_MATCH PASS (RELOC_ALIAS puts/abort/_CIpow/ceil import thunks; STANDIN_LEAF = catalog label).

## Part 3: CCoda3DSource Calc* owner re-ports + b24 0x00C41FB0 Commit -- PASS (fixed_b24_final, 6 rows, one landable set)
- Base: h3c audit fixed_b24 (src/test/hdr). Its six hdr files vs today's tree: 5 additive (0 removed lines), SalMath.h new. nf99a touches none
  of the seven addresses.
- Owner re-ports (fixed_b24_final/src/00c418f0 00c41990 00c41a20 00c41a70 00c41bb0.cpp): body = b24's same-TU copy byte for byte (checked
  programmatically; each b24 copy has the exact `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)` line directly above), on
  b24's exact #define / #include prologue, PDB spelling (?CalcDirectionAndDistance@CCoda3DSource@@AAEMAAVCCodaVector@@ABV2@@Z,
  ?CalcDistanceGain@ / ?CalcDoppler@ / ?CalcConeGain@CCoda3DSource@@ABEMXZ, ?CalcEnvFxGain@CCoda3DSource@@ABEMM@Z). So b24 needed no text change.
  One text, three grades: each owner RM standalone at its own address, each copy RM at its own address in b24's TU (h3c audit), Commit RM.
- Locals = CODAApiDbg CodeView per proc (DirAndDist: vDirection, cvPosition, fDistance; DistGain: fGain, fDistance; Doppler: f, fDoppler;
  ConeGain: fGain, fOuter, fMagnitudeMinus, vPlus, vMinus, fMagnitudePlus, fAngle, fInner = the CodeView record order; EnvFxGain: fRolloff,
  fGain, fDistance). SDK release COMDATs byte-identical (relocations masked) for 4 of 5 (re-measured); CalcConeGain 125 diffs (native fork).
  Header claims checked against retail disassembly (constants, `_CIpow` 0x00BFEB40, Normalize 0x00C402E0; ConeGain has no call: fpatan).
- Tests now LINK THE ROW OBJECT (mirror classes with the retail offsets measured by VC7.1, `friend int main()`, test-defined CODA and
  CCodaVector::Normalize). Mutants by mut.py on the ROW SOURCE: 00c418f0 5/6 (survivor equivalent: Magnitude of the fresh copy),
  00c41990 7/7, 00c41a20 5/5 (case 6 added), 00c41a70 8/9 (survivor equivalent: the two atan2 arms compute the same angle; cases 10/11 added),
  00c41bb0 6/6. Commit's test unchanged from fixed_b24.
- Strict vland --recheck-landed on the combined 6-row payload: all RELOCATION_MATCH PASS (fixed_b24_final/vland_strict.log; WARN RELOC_NAME
  on Commit = the stand-in catalog labels, cleared by the rename; RELOC_ALIAS _CIpow import thunk).
- Header regrade: tools/ppcmp.py over every landed transitive includer of CCoda3DSource/CCodaContext/CCodaListener/CCodaObject/CCodaVector.h:
  7678/7678 preprocess raw-identical with the overlay (plus the h3c audit's strict regrade of the 20 opt-in-aware CODA rows).
- Callers of stand-in spellings: none in landed code (comments only: 0x00C41E40, 0x00C402E0). reloc_bindings: ?g_damageResistanceScale@@3MA
  (0x0143BBF8, caller 00c41990) and ?g_combatLightingScale@@3MA (0x0143BBF0, caller 00c41a20) -- both CODA members; repair_remove drops them.
  No owner row is used as a same-TU copy elsewhere in landed code.
- Class-level NAME_FIXES for Account A (out of scope, not re-ported): the landed CLibpkObject1D8_* setters (00c412d0 00c41440 00c41470
  00c414a0 00c414d0 00c41500 00c41570 00c415c0 00c415e0 00c417f0 00c41840 00c447a0 00c44800) and CCombatant::SetCombatParameters 0x00C41E40 sit on
  the CCoda3DSource layout (dirty word +0x1D8 = m_ffCommit, +0x17C.. = m_fMaxDistance..); 0x00C40F50 AmplitudeToMillibels = sal_gainToLevel and
  0x00C3FF70 RoundHalfDownToLong = sal_ftoi stay stand-ins by scope (b24 NAME_FIXES).
- Purity (review item): no row-side `// Retail-asm:` line can fix it -- b24 already carries a valid one (0x00C423ED.. `fistp dword ptr`).
  classify() needs a helper NAME spelled in row code and declared by `// Retail-asm-helper:` in a header that itself contains `__asm`.
  Measured: (a) SalMath.h tagged without asm -> naked_asm (worse); (b) COMMENT-ONLY: libpk/FtoL.h line
  `// Retail-asm-helper: FtoL, LIBPK_FTOL, sal_ftoi, sal_round` -> b24 retail_asm, 35/35 FtoL.h includers unchanged (all retail_asm before and after);
  (c) SDK-faithful alternative: SalMath.h owns a macro-wrapped `__asm fld / fistp` (SAL_FISTP, FABLE_PORTABLE twin) under sal_ftoi / sal_round with
  its own tags -> retail_asm, Commit RM and sal_gainToLevel RM at 0x00C40F50 -- but needs a new dual-path test (ASM_RULES 3.2). Recommend (b).
  Same mechanism as b3: CMatrix4x4AlignedSSE.h `// Retail-asm-helper: ..., AlignedSetMultiply, AlignedSetMultiplyTransposed,
  AlignedPreMultiplyTransposed` flips exactly 00a9e1e0 00adefc0 00ae1320 00ae29d0 00bd2d90 (and b3) genuine -> retail_asm, 0 other of 194 includers.

### Landing commands (coordinator; not run)
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/repair_remove.py 00c418f0 00c41990 00c41a20 00c41a70 00c41bb0
    cp $AW/fixed_b24_final/hdr/engine/*.h rebuild/include/engine/
    python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/fixmove.py $AW/fixed_b24_final/payload.json
    python3 tools/decomp_pipeline/linux_lane/lane_tools/naming/keypayload.py $AW/fixed_b24_final/payload.json   # then re-check the 5 copies are still verbatim
    LANDVERIFY_STRICT_HEADERS=1 python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py $AW/fixed_b24_final/payload.json $AW/fixed_b24_final/oracle.tsv --land
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py   # must print 0
    + catalog / manifest rename of the 5 stand-ins + 0x00C41FB0 (b24 NAME_FIXES.tsv; batch_rerow.py)

## Final re-check (after the nf99a merge completed in the working tree)
All overlays still additive vs rebuild/include (0 removed lines; C3DExtentsI.h / SalMath.h new); strict vland re-run: fixed_b3 RM PASS,
fixed_9fde80 RM PASS, fixed_b24_final 6/6 RM PASS (--recheck-landed), fixed_774d20 RM PASS (*/vland_strict_final.log).

## Review items for the coordinator
1. b3: land only CMatrix4x4AlignedSSE.h (b3's CMatrix4x4.h is stale). The in-TU SetVSConstants copy spells `start` (0x00989CB0's text);
   Ego_d: index / data / count + local hr; the owner 0x0098B930 is a view-struct stand-in -> re-port 0x0098B930 in Ego_d spelling and make
   both in-TU copies (0x00989CB0, 0x00989230) verbatim (start->index measured byte-neutral in 0x00989230).
2. Purity (b3 and b24, one mechanism): comment-only helper-tag lines (Part 3 bullet) -- AlignedSSE flips 5 landed rows + b3 to retail_asm;
   FtoL.h flips b24 only.
3. e17: new engine/C3DExtentsI.h triggers strict SHARED_HEADER_AVAILABLE for 9 landed local-model rows (list in Part 2).
4. b12: GOTCHAS-455 copy fixed; bracket relabelled per the 2026-11-19 bracket rule (coordinator asked for `// Inferred:`; the newer owner rule says
   `// Bracket:` and no Inferred -- flagging the conflict rather than silently picking).
5. GFSizeof.h `const T&` vs FableWin ??$GFPSizeof@QAV... instantiation spelling (unreferenced COMDAT; pre-existing).

## Corrections / additions after the final advisor review
- Purity per row: 00c418f0 00c41990 00c41a20 00c41a70 00c41bb0 genuine (plain C++, no asm reached); 00c41fb0 genuine as-is, retail_asm with the
  FtoL.h tag line; 0x00989230 genuine as-is, retail_asm with the AlignedSSE tag line; 0x009FDE80 and 0x00774D20 genuine.
- Commit 0x00C41FB0 mutants: 8/8 killed (h3c audit logs/mut_b24.log; fixed_b24_final src + test are cmp-identical to fixed_b24, so it carries over).
- Mutant targets: Part 3 mutates the ROW SOURCE linked to the test; Parts 1, 2 and 4 mutate the test's model / pasted row copy (those tests do not
  call the row object: project convention for these rows).
- Part 4 line table: the statement mapping is the AUTHOR's; I re-checked only 842 = one 4-term `if` (120 B) and that the tail has 13 records
  881..898. Not re-derived record by record.
- SetVSConstants in-TU copies in landed code: 0x00989CB0 (start / data / count) and 0x00B99670 UploadShaderConstants mentions it in comments only;
  b3 is the second copy. GetFreeSaleItemPlaces same-TU copies: landed 0x00774C60 StockDisplay carries a DIFFERENT text (no `if (!allow_blocked)` block,
  `allow_blocked ||` folded into the condition, `ParentThing` vs `&GetParentThing()`, `n++`) and no Fidelity line -> pre-existing GOTCHAS-455 item.
- Parts 1 / 2 includer regrades re-run after the nf99a merge finished: identical (b3: 11 RM PASS + 00a5cee0 HEADER_POLICY; e17: 35 RM/MATCH + same 9 HEADER_POLICY).
- Disclosure: I ran read-only `git status` / `git log` twice (to see the in-flight merge); no git writes, no repo writes.

## Names for Account A (one place)
- b24 / Part 3 (fixed_b24_final/NAME_FIXES.tsv): 0x00C41FB0 ?Commit@CCoda3DSource@@QAEXABVCCodaObjectList@@0I@Z (was CCombatant::ComputeDerivedCombatStats);
  0x00C418F0 ?CalcDirectionAndDistance@CCoda3DSource@@AAEMAAVCCodaVector@@ABV2@@Z (CLibpkObject1D8::NormaliseCopy); 0x00C41990 ?CalcDistanceGain@
  (CCombatant::ComputeDamageResistance); 0x00C41A20 ?CalcDoppler@ (ComputeLightingAdjustment); 0x00C41A70 ?CalcConeGain@ (ComputeEffectiveRange);
  0x00C41BB0 ?CalcEnvFxGain@CCoda3DSource@@ABEMM@Z (ComputeSpellPower); 0x00C40F50 ?sal_gainToLevel@@YAHM@Z (AmplitudeToMillibels); 0x00C3FF70
  ?sal_ftoi@@YAHM@Z (RoundHalfDownToLong). Globals ?g_damageResistanceScale@@3MA 0x0143BBF8 / ?g_combatLightingScale@@3MA 0x0143BBF0 are CODA
  m_fRolloffFactor / m_fDopplerSpeed. Class-level: CLibpkObject1D8_* setters and CCombatant::SetCombatParameters 0x00C41E40 are CCoda3DSource-layout rows.
- e17 (fixed_9fde80/NAME_FIXES.tsv, LABELS.tsv): 0x009D58D0 CBankFileAsync::GetAsyncEntry (not CreateEntry); 0x01396C64 ?TextureLoadingPriority@@3JA (long);
  0x009FC440 catalog label 'CTCActivationTrigger::OnKill' -> ?RemoveFromResourceList@CResource@@QAEXXZ (RELOC_NAME); AllocateVolumeTexture 0x00A69C20,
  WaitUntilFinished 0x009D5B60 confirmed.
- b12 (fixed_774d20/NAME_FIXES.tsv): 0x00774D20 ?BuyItem@CTCShop@@QAEXJ@Z (catalog _global::CTCShop_SellHighlightedItem).
- b3: 0x0098B930 SetVSConstants landed as a view-struct stand-in (CShaderRenderManagerSetVSConstantsView); Ego_d params index / data / count + hr.
