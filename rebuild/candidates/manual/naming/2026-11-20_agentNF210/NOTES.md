# NF210 notes (round 10, 2026-11-20; single agent; entries in the brief's order; all four reached a terminal state)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- 4 entries: 0 done, 4 blocked (q4389 port: 2 rows left; q11760 + q11873 port / inline budget; q11772 decision), 0 open.
- Rows: 4 ported (0x006EA050, 0x006EDA80, 0x006ED510, 0x006EAC50; all RELOCATION_MATCH + PASS in recheck_final.log), 3 not-a-binder
  (0x006F0250, 0x006E97F0, 0x00410DE0), 5 blocked (0x006E93C0, 0x006EA770, 0x006D41F0, 0x006D4180, 0x0047DB73), 1 not touched (0x0096FA90,
  q12300 decision).
- Self renames (RENAMES s1): 0x006EA050, 0x006EAC50. Caller respellings (s2): 10 lines.
- Header opt-ins (hdr/README.md): engine/CCombatManager.h (ISDEFENDERDUEFORATTACK, GETCOMBATDEBUGINFO), engine/CThingCreatureBase.h
  (ISINMELEEASATTACKED), stlport/fable_map.h (RB_GLOBAL_DECREMENT_SIZE_BODY, TREE_ERASE_KEY_DECL_ONLY). Draft only, not in hdr/:
  engine/CThing.h FABLE_RETAIL_CTHING_HASTC_DECL_ONLY (work/hdr_drafts/; it fails, q11772).

## Coordinator decisions / items
1. q11772 (0x0047DB73): the declaration-only HasTC opt-in the brief asked for does stop the pick-any HasTC copy, but the row then calls HasTC
   out of line instead of expanding it (DIFFER 65v69, and HasTC would bind at 0x00410DE0: a new split). Retail expands HasTC there (`lea ecx,
   [esi+20h] ; call test`). Declaration-only cannot serve a row that expands the inline. Needs a ruling on the held draft's 8-byte HasTC copy
   (v 0x004118C0's 58) or a different header control (one that keeps HasTC expandable at sites but gives the emitted COMDAT the test-expanded body).
2. q11760 / q11873 (0x006D41F0): /d2inlT61..64 all DIFFER(269v295) on both operator== spellings (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO,
   NF209's draft, and FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE), the same as T60; T65+ is 307 (NF209). No per-row inline-budget flag
   reproduces retail's per-site mix. The per-row preference is exhausted and recorded, so the next step is a shared-header inline control
   (EXTRA_RULES 2026-11-08 preference 3: e.g. a noinline / forceinline control on CBasicString<char>::operator==(const char*) at the two later
   sites), which the coordinator rule reserves until now.
3. 0x006EAC50 in-place respell: CCombatDebugInfo and CCombatSequenceBase moved out of namespace CombatDebug (same class tokens as HEAD; neither
   has a shared header: CCombatDebugInfo has none anywhere, CCombatSequenceBase only engine/_quarantine/). The row's object defines
   ??0CCombatDebugInfo@@QAE@XZ, ??0CCombatDebugInfo@@QAE@ABV0@@Z and ??1CCombatDebugInfo@@QAE@XZ. The last is landed 0x006F2FD0's Leaf, but
   that owner's copy is a COMDAT and this copy equals it (q.py RELOCATION_MATCH at 0x006F2FD0; cmpbody 44/44 True/True), so it is the
   pick-any case, not a one-definition clash. The copy constructor ??0CCombatDebugInfo@@QAE@ABV0@@Z grades RELOCATION_MATCH at 0x006F3000, which IS a landed row (stand-in
   CRecord_006f3000, no Leaf line; its object defines only ??0CRecord_006f3000@@QAE@ABU0@@Z, so the symbols never compete at link). By hand
   against that owner object the relocation targets are the same retail addresses (0x0099EC30 ??0CCharString; 0x006F0810 twice), except that
   mine reach 0x006F0810 through the STLport vector copy-ctor COMDAT my own object defines (itself RELOCATION_MATCH at 0x006F0810), where the
   owner calls an external stand-in. Under a strict reading of the nf205 amendment ("a symbol defined inside the row's own object is always a
   different target") that is a different target; it is a function COMDAT for the same retail address, not a literal. Coordinator to judge. Read
   strictly, the in-place rule's condition (2) says "no symbol naming the respelled class beyond its own leaf"; I handed the row back
   on the pick-any reading. Hold it if the coordinator reads the rule strictly.
4. 0x006EDA80: the nf184 SIZE_BODY pattern applied as the brief suggested. It is a body opt-in (scaffolding), not declaration-only; the three
   EXTRA_RULES size-pragma conditions are checked in hdr/README.md. Queue 0x006F0030 to define the same macro: its own HEAD object emits a 64-byte
   _M_decrement copy that differs from the owner (NAME_FIXES, debt).

## Splits (none created, none grown)
- 0x006EA050: self stand-in ?CheckSummonerChargeable@CCombatManager_Retail@@.. -> ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z.
  The old self leaf is bound by no landed row (reloc_bindings + source grep). 0x0096FA90 keeps its own, different caller stand-in
  ?CheckSummonerChargeable@CCombatManager@@QAE_NPAVCThing@@@Z (q12300, not touched): spellings at 0x006EA050 stay 2 -> 2. That is the same
  shape NF207 landed for 0x006E9310. 0x006E97F0 narrows 2 -> 1 (its only binder now binds its Leaf).
- 0x006EAC50: 0 referencers (refs.py exe scan, reloc_bindings, source grep). Its callee moves: 0x006E99B0 and 0x006F0810 move onto spellings
  already bound (GetDefenderZone self Leaf; the STLport vector copy-ctor spelling, witness 0x00765B70). Their CombatDebug stand-ins had
  0x006EAC50 as their only witness, so they leave. 0x006F0810's self row keeps its own stand-in Leaf (existing split). New binding
  ?NullChar@CCharString@@1DB at 0x0129AAF4 is already bound there.
- 0x006F0250 / 0x006F4F30 / 0x006F50B0 / 0x006F3200: every moved caller now binds the self row's own Leaf (GOTCHAS nf171b: gate.py may print
  CREATED / GREW for these; they narrow self-vs-callers splits). 0x006F3200's stand-in ?erase@CAttackerMap@@QAEKAB_K@Z and 0x006F50B0's
  ?erase@CCombatantMap@@QAEXAB_K@Z had 0x006ED510 as their only binder; 0x006F4F30's ?insert@CRetailRbMap_UID_CombatantInfo@@.. had
  0x006EDA80 as its last.

## Inferred / fitted constructs
- 0x006EA050 (NF207 draft, unchanged): `// Inferred:` the id local is a signed __int64 (retail keeps the id in ebx:ebp and re-stores it before
  each find; an unsigned local is passed by address: DIFFER 92v106).
- No other Inferred, Bracket or Include-order lines added. No // Flags: lines added (0x006EAC50 keeps its HEAD `// Flags: /O2 /Oy /W3`).

## Local models
- No class token added to any row. 0x006EDA80 / 0x006ED510 keep their HEAD local models in place (vland SHARED_HEADER_AVAILABLE warnings are
  HEAD's) and drop the stand-in map classes (CRetailRbMap_UID_CombatantInfo, SCombatantValue, CCombatantMap, SCombatantNode, CAttackerMap).
- 0x006EA050 and 0x006EAC50 are ported whole onto the shared headers. 0x006EAC50 also drops the macro-renamed include of
  engine/CThingCreatureBase.h (Round-2 lesson) and its locally modelled CCharString / C3DVector / CThing / CAttackingCombatantInfo /
  CCombatantInfo / CCombatManager.
- Tests: all four unchanged. They are standalone host models that do not link the row; that is NF199 / NF207 precedent, and none uses a
  renamed symbol. Each still PASSes.

## Descriptive names
- None. Every new spelling is a PDB name (IsDefenderDueForAttack, GetCombatDebugInfo, IsInMeleeAsAttacked, GetDefenderZone), a self row's Leaf,
  or the repo STLport spelling (descriptive-by-library: _M_find, erase(const K&), insert_unique, vector<C3DVector> copy ctor).

## EH funclets
- No landed row or funclet includes any changed file (grep of the four file names under rebuild/src and rebuild/tests; 0x006F2300 only names
  0x006ED510's file in a comment). The 0x006EA050 self rename moves its file name at landing (CheckSummonerChargeable -> IsDefenderDueForAttack).
  Nothing includes it.

## Found in passing (not applied)
- 0x00662430 is bound as ?IsCombatant@CThingCreatureBase@@QAE_NXZ by 0x006ED510 and as ?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ by
  others (reloc_bindings). The PDB spelling is IsInMeleeAsAttacked (Ego_r 0x0053D2C0 / FableWin 0x01DA3370, next to IsInMeleeAsAttacker as
  0x00662410 is next to 0x00662430). I did not respell 0x006ED510's stand-in; that is outside the entry (NAME_FIXES).

## Locks
- All rows touched are in rows.txt (locked NF210). No new lock was claimed. Locks still held on rows not changed: 0x006E93C0 0x006EA770 0x006D41F0
  0x006D4180 0x0047DB73 0x00410DE0 0x006F0250 0x006E97F0 0x0096FA90.

## Rule slips
- One ad-hoc /Od preflight (`wine cl.exe` by hand in pS/) ran under `timeout 90` and was terminated without output. No stray cl.exe was left
  (ps). I graded only through q.py / vland after that.
- 0x006EA770 got 2 new forms, not 3. I stopped early for the slice budget, and both forms hit the same register-choice residue.
