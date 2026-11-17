# NF59 notes: land1117fu follow-up name fixes (q10497..q10551, excluding q10541 / q10542)

## Totals (counted from STATUS.tsv / APPLIED.tsv)

**Rows (96):**
- 59 ported;
- 33 not-a-binder;
- 4 blocked;
- 0 deferred.

**Entries (53):**
- 34 done;
- 6 done-already;
- 13 blocked;
- 0 rejected (REJECTED.tsv has the header only).

**Other hand-back files:**
- 13 header opt-ins in 8 overlay headers (hdr/README.md).
- 23 NAME_FIXES lines.
- RENAMES.tsv: 45 section-1 lines and 71 section-2 lines.

**Grades:** at most 6 on any row; every count is in STATUS.tsv, and work/grades.log agrees.

**Recheck:** recheck_final.log is two vland `--recheck-landed` runs, concatenated, because there are more than 30 entries.
- Run A: 30 rows.
- Run B: 29 rows plus the 3 EH funclets whose parent changed (0x01214E60, 0x01214E75, 0x01215C90).
- Result: 62/62 RELOCATION_MATCH/MATCH + PASS (WINS 30 + 32).

## Blocked rows and what must be ported first

**0x00467831 (q10530).** Needs an stlport/fable_containers.h opt-in that makes `__copy_trivial` / `fill_n` FORCEINLINE. The stlport overlays belong to NF58 this round. Three grades used.

**0x0053CB9B (q10531).** The queued spelling is the map copy constructor (FableWin.pdb 0x021A7386). fable_map.h has no `_Rb_tree(const _Rb_tree&)`. It only declares `map(const map&)` (MAP_COPY_OUTOFLINE). It needs a fable_map.h body opt-in for STLport's tree and map copy constructors. The binder 0x0053CCA5 is ported and already binds the queued spelling.

**0x006A0150 (q10502, authoring-scale).** Needs these on shared headers first:
- GetAnimationToUse: a private static, Ego_r 0x006C4160.
- GetAnimationFromName 0x006924B0: no shared declaration and no PDB name.
- CAnimationEntry(const CAnimationEntry&, long) 0x005DEC70 and its inline release.
- The CCountedPointer(T*) count block with the DeleteCreatureAction deleter.

**0x0064F6DC (q10525).** Needs a DrawOracleSymbols declaration opt-in on engine/NPlayerGui_CDrawQuestInfo.h. The row's local sprite / engine / gui / colour views must also move to their shared headers (they collide with that header's includes), plus:
- a declaration-only CEnginePrimitive2DSprite position constructor (0x00493AC2);
- the CPlayerGui singleton Alpha read.

The landed caller Draw 0x00654438 also models CDrawQuestInfo locally.

## Blocked entries with no row in this slice

- **q10500 / q10516 (authoring):** the call site or the function has no landed row.
- **q10524 / q10527 / q10528 / q10529 / q10532:** the remaining binders are outside rows.txt (NAME_FIXES.tsv lists each one).
- **q10543:** 0x004DBE58 and its binder 0x004E1AAF are ported (0x004E1AAF also self-renamed to ~vector<CTargetPoint>). The row left is 0x004E37BF ~CTCTargetingPlayer (outside rows.txt), which still reaches 0x004E1AAF through a made-up member destructor.
- **q10550:** 0x007AE0E0 and 0x007AEB70 are ported. The rows left (outside rows.txt) are:
  - 0x007AE190: the tree clear, which binds a masked stand-in;
  - 0x008AE380: a local tree over an allocator-less vector;
  - 0x01229D00 / 0x01229D60: the GossipVillages / GossipFactions atexit deleters, on a local CRetailTree.
- **Binder check:** for every address respelled after the context reset, callers.py mapped each call site to a landed row, and each landed binder was graded privately in pO (work/gext.sh, work/grades_ext.log). Its relocation was then read at the target.
  - Confirmed on the new spelling: 0x00467597 and 0x00CDCF80 (GFSerialise<bool>), 0x0077E580, 0x004DFBC6, 0x005BBF80, 0x0053CEF6 (both 0x0053CB9B and 0x0053CCA5), 0x00725C40, 0x006B9560 and 0x006B9610.

## Edits to existing local models (no new local model of a class with a shared header)

**Declaration edits of the self row's own signature in the row's pre-existing local model:**
- 0x0074E560 (CTCContainerRewardHero);
- 0x008ED090 (CTCContainer);
- 0x0095EE70 (CAIStateGroup_OpinionReactionRoot);
- 0x007FCEF0 (CThingSearchTools: const member template, two helpers const, scratch list mutable). The header FIRSTROUGH_BODY instance was tried first and was off by two frame slots.

**0x00724010 OnCreate (binder):**
- The made-up CThingPtrVector is replaced by the shared STLport vector.
- The existing local CTCPhysicsStandard model's declaration of GetPossibleInteractedWithList is respelled (protected const, `std::vector<CThing*>&`), together with UpdateOnSurface's parameter type.

**0x00745730:** access-only callee fix (round-1 rule). UpdateLineOfSight is declared private in the row's existing local CTCPerceiveThing model.

**0x008E57D0:** CThingFilter_CanCollideOrInteractWithThing stays a local class. It is the Ego_r.pdb name, and the class has no shared header. Only its constructor and a declared `operator()` are modelled.

## Descriptive names and non-retail helpers

- **CAnimateThing::IsFinished (0x00661720):** the landed catalog name. No PDB names this accessor. It is declared by the opt-in FABLE_RETAIL_CANIMATETHING_ISFINISHED.
- **CLockedRingBuffer::GetHeadIndex (0x00C49DD0):** the landed self row's descriptive spelling, now declared by FABLE_RETAIL_CLOCKEDRINGBUFFER_GETHEADINDEX.
- **Non-retail helper functions:** DeleteEngineTextureMorph (0x005BBBCE) and DestroyMapOfCCharStringVectorCCharString (0x007AEB70). They exist only to make VC7.1 emit the compiler-generated `??_G` / implicit `~map` COMDAT, like the landed ~map 0x006F24D0. They are not retail functions.
- **STLport instances are descriptive-by-library.** Retail is STLport 4.6 and both PDBs are Dinkumware builds. This covers `_M_clear` 0x004AC1D0, `__destroy_aux<CTargetPoint*>` 0x004DBE58, `~vector<CTargetPoint>` 0x004E1AAF, `__uninitialized_copy<map*,map*>` 0x0053CCA5, `_M_erase` 0x007AE0E0 and `~map` 0x007AEB70.

## Judgement calls to review

**0x007AEB70 (~map vs ~_Rb_tree).**
- The bytes are identical for STLport's ~_Rb_tree and the implicit ~map<CCharString, vector<CCharString> >, and xref.py finds no callers.
- I chose ~map because of the neighbours: the adjacent uncalled 0x007AEB30 is the landed map() constructor of the same map, and 0x007AEBE0 is its operator[]. That makes this a run of map members.
- FableWin.pdb has `??1?$map@VCCharString@@V?$vector@VCCharString@@...` (0x01E8D0F0), but it also has `??1?$_Tree@...`.
- The landed pair 0x006F2490 map() / 0x006F24D0 ~map (all uncalled) has the same layout. There the ~_Rb_tree 0x006F1BA0 follows the tree constructor 0x006F1B60 instead.
- If the coordinator prefers ~_Rb_tree, it is a one-line respell. The explicit instantiation would be `template std::_Rb_tree<...>::~_Rb_tree();`.

**vector<bool>::resize (0x00467686).**
- Ego_r.pdb spells it `?resize@?$vector@_NV?$allocator@_N@std@@@std@@QAEXI_N@Z`, with the bool by value.
- Retail TLC pushes the address of the bool() temporary, so it is const& (`QAEXIAB_N@Z`). The landed row 0x00467686 emits the const& form, and 0x004675F6 now binds that.
- I left it as it is, on the byte evidence.

**GetFOV (0x00745120).** The row no longer defines GetFOV itself. FABLE_RETAIL_CTCPERCEIVETHING_GETFOV_INLINE makes it a header inline. Inline-function semantics make it a COMDAT rather than a second strong definition next to the landed copy 0x007445D0; this was not link-tested.

**TransferBinaryIn<bool> body inside 0x004675F6.** The retail frame needs the callee's body visible in the TU but not inlined. It is written `inline __declspec(noinline)`, which makes it a COMDAT. I checked the IMAGE_SCN_LNK_COMDAT section flag in the object; it was not link-tested against the 0x00403EB0 row (NAME_FIXES).

**Test pass strings.** The rewritten tests for 0x004DBE58 and 0x004E1AAF print the catalogue PassPatterns, `DESTROY_RANGE_OK` and `AUTO_TINY_004e1aaf_TEST PASS`. Both pattern names describe the old stand-ins. Renaming them is a catalogue edit for the coordinator.

## Flags

No row's `// Flags:` line differs from the landed copy. A `/Os` line first added to 0x004E1AAF was removed: the landed `#pragma optimize("s", on)` alone matches.

## Rule slips (work/slips.txt, 7 lines)

**Two read-only Bash calls exited non-zero; neither changed a file:**
1. A mis-sliced test lookup loop exited 1.
2. An `ls` of absent `wd*` directories exited 2.

**Five assembly or argument slips; each was caught and fixed:**
1. An unquoted `<bool>` in an s1.py argument became a shell redirection. The call failed silently and was rerun quoted.
2. 0x004675C5's final row lost its `#pragma optimize("s", on)` line to a sed off-by-one. That wasted one grade.
3. st.py was called with an extra entry-id argument for 13 rows, so their note column got the entry id. All 13 notes were rewritten.
4. 0x008E57D0's final row was assembled by reading the same file the group redirected into, which truncated its first lines. It was rebuilt from work/orig.
5. The rewritten tests for 0x004DBE58 / 0x004E1AAF first printed their own pass strings. Run A failed on them; after the fix, run A was repeated in full.
