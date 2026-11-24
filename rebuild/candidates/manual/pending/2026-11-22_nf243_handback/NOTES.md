# NF243 notes (2026-11-22, round 14b): 0x004046B0, the DTOR_OUTOFLINE census, L108-L110

## 1. Entry 1: 0x004046B0 (list<CStringParserCommentBoundaries>): claim proven, family HELD (self row unchanged)
**Retail layout** (same as 0x0042ABAF.. nf241): 0x00404690 32 B `_List_base(const allocator&)` (already bound so by 0x00463871),
0x004046B0 62 B clear + free = `~_List_base`, 0x004046F0 32 B `list(const allocator&)` (base ctor inlined), 0x00404710 `e9 9b ff ff ff`
= `jmp 0x004046B0` = `~list` (the only jmp to 0x004046B0 in the exe; 90 E8 calls; no absolute reference to any of the four). A
shared-header TU compiles all four copies EQ to those addresses (pk.py, 0x004C88E0 draft). PDBs are Dinkumware: descriptive-by-library.
**Census** (compiled, every referencer at HEAD: work/hc/, work/e1_census_keep.tsv; 65 rows locked NF243, none held by anyone else):
18 bind `??1?$_List_base<CSPCB>` (CStringParser.h users), 46 bind `??1?$list<CSPCB>`: 43 row-local `namespace std { allocator; list
{ ~list(); } }` stand-ins (UND) and 3 shared-header rows with DTOR_OUTOFLINE (0x004C9120, 0x004CAA00, 0x00814260).
**Recipe (proved on drafts, work/t_*.cpp; tools/xl.py, xl2.py):** replace the row-local allocator / list block with
`#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE` + stlport/fable_containers.h (dtor-only rows): 0x005DC550, 0x009EC890 RELOCATION_MATCH,
only the 0x004046B0 relocation renamed; the new pick-any `~list` copy (5-byte jmp) is EQ to 0x00404710. Rows that build the parser inline
(an inline list ctor: 0x004C88E0, 0x004C8A30, 0x00686E10, 0x009ADE90, 0x009E5AD0, 0x00A38E50) need
`FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE` and a COMPLETE CStringParserCommentBoundaries: 0x004C88E0 then RELOCATION_MATCH with
every list copy EQ (ctor 0x00404690, dtor 0x004046B0, list ctor 0x004046F0, ~list 0x00404710); 0x009AF1E0 likewise with its HEAD-local
class. The complete type exists only inside engine/CStringParser.h's FABLE_RETAIL_CSTRINGPARSER block, so those six rows and the self row
need an add-only partial mode there (`FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY`: the line-182 `#else` -> `#elif
!defined(..._ONLY)` plus the one class at the end; not written: the family is held). The 3 DTOR_OUTOFLINE rows take NF241's swap.
**Blockers (why held):**
- 0x00501D30 CWorldMap::GetLevelFilesFromWorldFile, blocked after 3 forms: its local std::vector<char> / vector<CWideString> model sits
  beside the list (form 1, list-only port: std::vector redefinition); form 2 (vector + list shared, the inline parser ctor's hand-made
  sentinel replaced by the shared list ctor): DIFFER 1052 v 1086, the shared vector<char>(size_type) is called out of line and
  resize(n) has a visible body; form 3 (+ FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL): the same DIFFER. Needs a vector port first.
- 0x00756D00 / 0x007AE3D0: local _Rb_tree / map models beside the list; with stlport/fable_map.h: no map::erase and no CCharString `<`
  on that path (form 1). Need a map port first.
So the self row cannot move without leaving 3 binders on `~list` (see decision 1).

## 2. Entry 2: the DTOR_OUTOFLINE census (19 rows; HEAD compile of each, tools/dtk.py: work/e2_dtk.tsv)
Rule used: a list destructor relocation is right as `~list` only if its retail target is a 5-byte `jmp` thunk; a body is `~_List_base`
when a 5-byte jmp thunk to it exists (STLport `~list() {}`), as nf241 ruled.
- **~list bound at a ~_List_base body (wrong), thunk present:** 0x004C9120, 0x004CAA00, 0x00814260 (-> 0x004046B0, entry 1 family);
  0x005EB590 (-> 0x004AB4F0: standalone thunk 0x004AC080; 0x00515AA3 / 0x00518D43 are `add ecx,4 ; jmp` member tail jumps); 0x0063E6F0 (-> 0x0063F360, thunk 0x0063F470): FIXED with
  its self row; 0x00682810 (-> 0x00685BC0, thunk 0x006860E0); 0x00A82BE0, 0x00A83570 (-> 0x005127A0: standalone thunks 0x00514280 / 0x00A81C40; four `add ecx,8 ; jmp` member tail jumps).
  Every standalone thunk cited (0x00404710, 0x0063F470, 0x006860E0, 0x004AC080, 0x00514280, 0x00A81C40) was dumped: `ret` / `ret 4` before,
  `e9 rel32`, CC padding after.
- **~list bound at a body with NO thunk anywhere:** 0x004E0080, 0x004E0122, 0x004E1437 (-> 0x004B8940, 25 B, list<CIntelligentPointer
  <CThing>>): only STLport structure says ~_List_base (one source); left as is (NAME_FIXES, undecided).
- **No list ctor / dtor relocation at all** (each HEAD compile RELOCATION_MATCH; xr lists 2 / 119 / 2 / 3 / 3 / 5 / 1 / 0 relocations) (DTOR_OUTOFLINE only shapes the TU / its pick-any copies): 0x00449980, 0x004804DE,
  0x004E0064, 0x00A83680, 0x00A84470, 0x00A84A60, 0x0121E010, 0x01229260. Nothing to respell.
- 0x00682810's ctor relocation 0x006860C0 (`??0?$list<const CThingAICreature*>`): 32 B, just before the ~list thunk 0x006860E0, the
  0x004046F0 position: consistent with list's own ctor (left).
**Fixed: the 0x0063F360 family** (self + its only referencer, census: refs = 0x0063E6F0 only): self row off the
`SortTreeRecursively_00436900(TreeList*)` stand-in onto `template std::_List_base<const CThingGameObject*>::~_List_base();`
(RELOCATION_MATCH; besides the leaf it defines only shared-header inlines: clear, deallocate, alloc_proxy / allocator dtors);
0x0063E6F0 DTOR_OUTOFLINE -> BASE_DTOR_VISIBLE_NOINLINE (relocation diff: only 0x0063F360 renamed). 2 -> 1 spelling at 0x0063F360.
pk.py HEAD v NEW: the new ~_List_base copy EQ 0x0063F360, ~list copy EQ 0x0063F470; the one NEQ (GetAllThingsInRoughArea<..> 586 v
596 against 0x0063FAE0) is NEQ at HEAD too. Link: 0 duplicates. (72 compiled rows still define the same
SortTreeRecursively_00436900 stand-in at other list-destructor addresses: HEAD debt, NOT touched.)
**Left (families, context):** 0x00685BC0 (0x00682810 + 0x00986510, which models a global-namespace `list` that also spells
GetWivesInMarriageState 0x0057AE53 and the list ctor 0x006860C0: port that row first), 0x004AB4F0 (7 referencers), 0x005127A0 (13),
0x004046B0 (entry 1).

## 3. Entry 3: L108-L110 (done, one unit)
Both PDBs (Ego_r.4k / FableWin.4k S_PUB32): ?SetBankUpdateStatus@CBankFile@@IAEXABVCCharString@@@Z (protected),
?SetBankUpdateProgress@CBankFile@@AAEXKK@Z (private), ?WriteBankLog@CBankFile@@IAEXABVCCharString@@@Z (protected); the bodies are the
header's `if (ProgressCallback) ProgressCallback->X(..)` over CProgressCallback slots 1 / 0 / 2 (`jmp [eax+4]` ret 4, `jmp [eax]` ret 8,
`jmp [eax+8]` ret 4). New add-only opt-in FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL (hdr/README.md; equivalence 517 rows, 0 differ).
Self rows off the CScriptedThingOwner_Retail stand-ins: MATCH, each object defines only its leaf; new tests on the shared header
(self-contained: they open access with `#define private/protected public` as the landed 0x009CEC60 test does, which changes the
mangling, so each carries the row body). Census: 0x009CBD30's one caller (0x009CEC60, call 0x009CF27B) already binds the PDB spelling;
0x009CBD50 / 0x009CBD70 have no caller. Link (0x009CEC60 HEAD + the three): 3 LNK4006, 0x009CEC60's inline pick-any copies of the three
wrappers v the strong self rows; pk.py: all three copies EQ (20 / 19 / 20 bytes), so the duplicates are byte-identical.

## 4. Entry 4: not started (context): QUEUE_LINES (36 lines besides L108-L110), 0x0049D800, 0x01296F14 + L101-L103.

## 5. Decisions needed from the coordinator
1. **New LNK4006 (entry 3):** linking 0x009CEC60 (HEAD) with the three new self rows gives 3 NEW LNK4006: 0x009CEC60's inline pick-any
   copies of SetBankUpdateStatus / SetBankUpdateProgress / WriteBankLog v the strong self rows. pk.py: all three copies EQ to the rows'
   retail bodies (20 / 19 / 20 bytes). Accept (byte-identical duplicates), or should 0x009CEC60 also take the DECL opt-in (it would then
   call, not define, them: a separate recheck)?
2. **0x004046B0:** the claim is proven, but only 4 drafts were graded (0x005DC550, 0x009EC890, 0x004C88E0, 0x009AF1E0; the other 39
   are ungraded; 0x004C88E0's draft used a temporary row-local complete CStringParserCommentBoundaries, which is not landable: it needs
   the CStringParser.h partial mode; pk.py was not run on 0x009EC890's draft, whose local vector also moved onto the shared one). Options:
   (a) a later pass lands the self row + 43 binders with that partial mode and leaves 0x00501D30 / 0x00756D00 / 0x007AE3D0 on `~list`
   until their vector / map ports (the split already exists at HEAD: 46 + 18 -> 3 + 61); (b) hold the whole family until those ports land.
3. Catalog additions (NAME_FIXES): 0x00404710 and 0x0063F470 (~list thunks); catalog renames of 0x00404690 / 0x004046F0 off their
   stand-ins. 0x004B8940: rule on the no-thunk case (one source) or leave.
4. Locks: the 65 entry-1 rows stay locked NF243 (rows.txt) so no one moves part of the family; release them if you rule otherwise.
   0x00685BC0 / 0x00682810 / 0x00986510 were claimed and released untouched.

selfcheck.py was not run: it is absent from $S/w22/nftools (absrefs, apply_rows, callers, mkpay, xref, xrow only).

## Flags lines / funclets / tests / descriptive names / file renames
No row needed new flags. No funclet row includes a changed source (grep of the five file names: none). Tests: 0x0063F360 / 0x0063E6F0
keep their HEAD tests (PASS); 0x009CBD30 / 50 / 70 new tests (PASS). Descriptive names: the _List_base spelling (STLport 4.6,
descriptive-by-library). Source-file renames for the coordinator: 00/63/CGuiControlTreePane_SortTreeRecursively_0063f360.cpp (+ test) ->
stdListBaseCThingGameObject; 00/9c/CScriptGameResourceObjectScriptedThingBase_func009cbd{30,50,70}_*.cpp (+ tests) -> CBankFile_*;
build-entry Module of the three ('CScriptGameResourceObjectScriptedThingBase' in build_candidates.ps1) -> CBankFile.
