# NF95 notes (round 8, naming queue part 3 of 3) -- PAUSED at a long-slice stop

Stopped after q11245 (context budget). Entries q11253 onward are `open` in APPLIED.tsv; their rows are `open: entry not reached` in STATUS.tsv.

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- Entries (75): 10 done (q11225, q11227, q11228, q11231, q11233, q11234, q11235, q11240, q11241, q11245), 2 blocked (q11236, q11237: port),
  2 deferred (q11229, q11414: held by acctB), 61 open, 0 rejected, 0 done-already.
- Rows: 18 ported, 3 not-a-binder (0x005BDD8C, 0x005BDE50, 0x004CC150), 2 blocked (0x005BDEC1, 0x005C150B), 90 rows of rows.txt open.
- recheck_final.log: one vland --recheck-landed run over all 18 ported rows: 2 MATCH + 16 RELOCATION_MATCH, every test PASS.
- Rows claimed outside rows.txt (nflocks owner NF95): 0x005BDC08, 0x00581A6D, 0x005B34D5, 0x005C2E2A, 0x005BF91A, 0x005BDEC1, 0x005C150B.

## Descriptive names
- No new made-up names. 0x0042B687 (list<long>::push_back) is descriptive-by-library (STLport instantiation's mangled name).

## Self renames (RENAMES s1) -- the file slug moves for these
0x0042B687 (leaf only), 0x005BD33C (access only), 0x005BDCDC, 0x005D9270 (was CTCWeapon::IsAnalogueLoading), 0x005BDD04, 0x005B34D5,
0x005C2E2A (catalog module was CTCInventoryClothing), 0x005BD4CE (was _global::CIVCountedPointer_Assign_005bd4ce), 0x005BF8CB.
No EH funclet row includes any changed row's file (grep of rebuild/src/compiled for each file name: 0 includers).
No row changed a `// Flags:` line; every row grades with its catalog CompilerFlags (empty for all 18).
Tests: none changed; all 18 PASS in recheck_final.log. The ones I read are self-contained; several still carry the old stand-in names
inside the test (0x005D9270 / 0x005BD4CE `CTCWeapon_IsAnalogueLoading`, 0x008959B0 `g_lastFishWeight_013bae80`).

## Per entry
- q11225: self row now an explicit instantiation over stlport/fable_containers.h (0x0042B6B8 recipe). Binder 0x0064A3A1 moved onto
  engine/NPlayerGui_CDrawQuestInfo.h; it also now binds GetScriptSpriteBySymbol as const (QBE), as both PDBs and its self row 0x0064A37B.
  The sibling 0x0064A3C7 (OracleSymbolPushFront) still uses the CRetailLongList::push_front stand-in (no entry; not changed).
- q11227: access-only. 0x005BDB94 / 0x005BDC08 keep their pre-existing local models: a header port fails because retail inlines
  CCategory::SetAsSelected(false) and the header's Selected is private (work/005bdb94_headerport_srcfail.cpp).
- q11228: 0x005BDCDC on the header; 0x005BDD8C already right (reloc_bindings stale). The callee self row 0x005BD0F7 is still
  `CCategory_Retail::HighlightPreviousItem` and its callee 0x005BCFE2 `CCategory_Retail::GetPreviousOccupiedSlot` (NAME_FIXES; ports
  would follow the landed twin 0x005BD054 HighlightNextItem recipe).
- q11231 / q11235: both self rows were the made-up free `CTCWeapon_IsAnalogueLoading`; re-authored on the headers, MATCH. Both need
  FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (the byte load before the decrement).
- q11233: data symbol `?MostRecentFishWeight@CTCFish@@2MA` (Ego_d PUB32, egod_carved_names.tsv); 0x008959B0 now reads CTCFish::GetWeight()
  through the shared CTCFish (TCI_FISH) and its local `enum ETCInterfaceType { }` empty became the shared include.
- q11234: porting 0x005BDD04 renamed it, so its three callers 0x00581A6D (CTCInventory.h PASS22), 0x005B34D5 and 0x005C2E2A (new
  ProcessButtonAReleased opt-ins) were ported too.
- q11236 (blocked: port): only 0x005BDEC1 CCategory::SelectItemOfType is wrong; it needs std::binder2nd / const_mem_fun1_ref_t in
  stlport/fable_algorithm.h and the find_if self row 0x005C13E3 on the shared spelling first. Slot 33 is OnItemSelected(long, bool&).
- q11237 (blocked: port): 0x005C150B CIsSameItemType::operator() gets retail's ecx reuse from a same-TU noinline copy of
  IsSameItemTypeAsDefIndex (0x005BC450) under the CItem_Methods stand-in; on the header that body must be a header noinline inline,
  which needs the nf86/nf88 one-definition scaffolding on 0x005BC450 first.
- q11240: also found 0x005BF91A by probe (not in WORKLIST); 0x005BF8CB self-renamed onto the header.
- q11241: 0x005BF414 respelled in its pre-existing local model; a header port binds everything right but differs 103v110 (register
  choice; work/005bf414_headerport_differ103v110.cpp). Its own stand-in name goes to NAME_FIXES.
- q11245: 0x004CC150's own symbol was already right; 0x00686DF0 now calls GlobalUpdateTCs.remove(this) on the new CTCBase.h opt-in.
  0x013BA8AC still has two other landed spellings (0x0121E010, 0x00686DC0): NAME_FIXES.

## For the resume (q11253 next)
- q11253 0x006924B0 GetRandomAnimFromName (protected static __fastcall): 7 binders (0x0062DAD0, 0x0062D890, 0x00718650, 0x007E7FD0,
  0x007ED450, 0x00842A20, 0x0069F0F0) are CCreatureAction_* constructors with local CCreatureActionBase models calling free stand-ins.
  Their own ctor signatures differ from the PDB (pointer owner, `long` for the CCharString&), while some of their callers already bind
  the PDB ctor spelling (0x007E80C0, 0x00922F50, 0x0095E0B0, 0x008CD820). The landed 0x008421E0 shows the call form. Plan: add the
  protected static to each row's local base model (or move to engine/CCreatureActionBase.h ANIMCTOR), fix the base ctor 0x00693B30 to
  `(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long)`, and decide per row whether the own ctor
  goes to the PDB spelling (then respell its other callers).

## Caveats
- q11236 and q11237 are `blocked: port` with 0 grades: the blocks rest on prerequisite analysis only (missing shared STLport binder2nd /
  const_mem_fun1_ref_t and the 0x005C13E3 self row; the 0x005BC450 one-definition scaffolding). The resume may retry them.
- Rows respelled inside their pre-existing local models (not ported): 0x005BD33C (vland WARN SHARED_HEADER_AVAILABLE CInventoryDef),
  0x005BDB94, 0x005BDC08, 0x005BFB96, 0x005BF414. No new local model was added anywhere.
- RENAMES section 2 old symbols: taken from a before-compile (g.sh relocation dump of the landed row) for 0x0042B687 (insert), 0x005BF91A,
  0x005BDD8C (probe), 0x004CC150 (probe); the others were read from the landed source declarations and reloc_bindings.tsv (seen stale
  twice), not from a before-compile: 0x0064A3A1, 0x005BDB94, 0x005BDC08, 0x005BDCDC, 0x008959B0, 0x005BDD04, 0x00581A6D, 0x005B34D5,
  0x005C2E2A, 0x005BF8CB, 0x005BFB96, 0x005BF414, 0x00686DF0.
- Checked: none of the 7 rows claimed outside rows.txt appears in COORDINATION_CLAIMS.tsv or UNMATCHED_ROWS.tsv; none of the 9 new own
  leaves is bound or defined at another address (reloc_bindings.tsv, `// Leaf:` lines).

## Flags for the coordinator
- NF93 and NF94 both add FABLE_RETAIL_CCATEGORY_REMOVEITEM to CCategory.h (same line); merge once.
- CTCInventoryWeapons.h: pin_asserts could not pin the line-150 assert (trailing comment); I pinned it by hand to
  FABLE_STATIC_ASSERT_AT(150, ...).

## Rule slips
- Two Bash calls exited non-zero: a `cat` of a lock owner file that did not exist, and an xdis / xref call without `S` exported.
- q11236 / q11237 blocked with 0 grades (see Caveats).
- STATUS grades_used was reconciled with grades.log at hand-back (three rows were off by one).
