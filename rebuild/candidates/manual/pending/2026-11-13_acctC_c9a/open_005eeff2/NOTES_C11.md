# agentC11 NOTES (acctC lane; continues agentC10 -> prev_C10/, C9, C7)
Tools: tools/ = C10's (repointed) + tools/pk/wi.py, wig.py (from agentC10/tools/pk), NEW tools/hq.sh (grades the row's 4 same-TU
helpers in one line), tools/wq.sh (what-if + objcmp one-liner: `wq.sh <src> <tag> '<name>=<prio>,..'`), tools/mkv.py (writes
variant sources from a variants.py without grading). vb.py batches as before (~1.5 s a grade).

## Row 1 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList -- DIFFER(2649v2652), best differ/005f59c7.cpp (= probe/a/B1.cpp)
Changed vs C10: the same-TU helper texts now byte-match retail (hq.sh): GetMiscThingsDef 0x005EE3B3 RELOCATION_MATCH (string in
  its own scope ahead of the def, as the landed row; callee 0x009AD390 is GetGlobalIndexFromClassIndex(const CCharString&, long)
  const per Ego_r/FableWin PDB -- reloc_bindings' `GetDefGlobalIndex(const CCharString&, bool)` is not a PDB name, see
  NAME_FIXES.tsv), GetDef<T> (if (index <= 0) return false; def; if (!def.IsNull()) { out->SetDynamicCast(def); return true; }
  return false; + FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD) RELOCATION_MATCH for 0x005F81BE and 0x0042AF3C, GetSubDef via the
  header body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY + NEW FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME). 0x005DA240 itself
  is another TU's copy compiled with /Oy (esp frame, different release shape): it cannot match in this TU (162v182).
  The row is insensitive to all of these (as C10 found for the wrong texts).
What-if re-confirmed on this text: `const -1=40,_textBank=35` -> len 2652 D 13 (masked relocs only). tb=-100 + -1=40 also retail.
  inventory=60/80 (tb -100 or 35) does NOT help (DR 47): the -1 piece must be low.
K of the tail block b87 (kt.sh): candidates this, parent, -1, textBank, local78, local16/29, subList, const 0.
  - local78 = the PeekComponent() result as a value (inline GetTextBank's `this`, a named `game` ref, SMART_POINTER_ACCESSORS
    all keep it). Only a diagnostic raw cast `**(CDataBank* const*)((char*)&PeekComponent()+0x14)` removes it: K 7, tb 49, -1
    tail +84; grade unchanged (DR 38). Not a natural spelling (PTextBank is protected).
  - the "vector-free local" is the stealth vector's _M_start FIELD (memory-only candidate, spill -1), not the deallocate
    parameter: ~_Vector_base as `if (_M_start) free(_M_start)` / no outer if / this-> forms (test overlays tmp/hv1..3) keep it.
  - Model check (C10's -1 piece = tail share - ~25 from the latch/middle blocks it spans): -1 tail share must be <= ~60 =
    12 refs x K<=5. With natural code K >= 7 (this, parent, -1, subList, const 0, stealth _M_start + tb or local78).
Tried without effect (grades in ATTEMPTS.log): dead FABLE_RETAIL_ASSERT (DEAD_ARM) before each category call / after GetTC /
  after the sub list / &textBank (a1..a8); empty ifs on subList / parent / this / &textBank (e1..e7: folded before blocks); tail
  inside for(k<1) / if(true) / switch(0) / bool flag (s1..s6: folded, K unchanged; s1/s2/s6 move the categoryTitle scope only);
  helper bodies removed or moved after the row (h1..h5: h2..h4 change the row a little but worse; h1b no change); helper texts
  fixed (c1..c6: no change); SMART_POINTER_ACCESSORS / named game ref (t1, t2: no change).
Open: find a natural source with tail K <= 5 or textBank non-candidate AND the -1 piece <= 40. Not tried yet: TU flag families
  (optimize("g"/"t") variants), ConstructPCExpressionCategoryList declared after use with a different parameter spelling.

## Row 2 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* parent) -- DIFFER(5619v5619)
Name proven: Ego_r.pdb 0x0057DCEB + FableWin signatures 0x01EBF630 `?GetMagicListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z`;
  already declared in rebuild/include/engine/CTCInventoryAbilities.h:70 (`//005eeff2`, FABLE_RETAIL_CTCINVENTORYABILITIES_SCREEN) -- no
  new header opt-in needed; landed ConstructScreen 0x005F519F calls it. Extent 0x005EEFF2..0x005F05E4 (`ret 8`) confirmed by the grade.
Best: differ/005eeff2.cpp (= probe/m/P12.cpp). Every instruction is retail's (tools/sd.py: only the 2 masked vtable relocations);
  the residue is the frame packer: retail `received` [ebp+68h] and its loop counter [ebp+64h], ours swapped (+ one word [ebp+68h] v
  [ebp-0Ch]); D 49 DF 2 DR 2. Path (grades in ATTEMPTS.log, probe/m/v*.py): ConstructScreen's same-TU draft 4284 bytes -> fresh source
  from the acctB DOSSIER/OUTLINE (unrolled category blocks / finishes, std::find, chained caption = displayName =, L"" literals) 5675 ->
  same-TU helper bodies (GetCategory, GetAbilityPDef, GetMiscThingsDef; auto_inline(off)) 5621/DR 96 -> implicit `L""` in SetValue
  -> visible _List_base ctor/dtor bodies (allocator temp homed in the live parent slot [ebp+7Fh]) -> visible vector(size_type) body ->
  FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (as the matched sibling) -> named CSpellContainer* before SetEmptySlotDef (both places,
  one variable) -> GetAbilityLevel(*it) as the sibling's same-TU inline member (auto_inline(on)) for the level texts and the
  LevDescription index -> named CText* for the description -> received as `size <= total ? size : total` -> empty / savedEmpty
  declared inside the fill loop (stores hoisted after the zero-trip test) -> palette loop `n < reordered.size()` (no paletteSize
  local) -> fill and swap loops share one `n` (D 78 -> 49).
Frame tried without effect: declaration position of the shared n (t1..t4), received in the for-init (j1), sharing a second counter
  among received/reorder/palette loops (h1..h4, u1..u4: worse). Next: packer replay (lessons r3002/r3016: "most recently created slot
  that fits"; tools fmap.py / pk) to find which node order puts `received` after its loop counter; try received as a reused earlier
  local, or the received loop written as while / with the counter declared before the ternary.
Uses the matched sibling's idioms (prev_C10/prev_C9/src/005f1b10.cpp). No test yet (row not matched).
Also tried (row 2 frame): received loop reusing the shared n (o1, D 224 worse), received loop as while with its own index (o2, unchanged D 49).
