# NF185 notes (round 10, STL / counted pointer / CCharString template instances)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 49 entries: 2 done (q4388 q4457), 9 done-already (q4384 q4387 q4459
q4478 q4496 q4796 q4804 q4932 q6507), 5 blocked/partial (q4385 q4386 q4389; q4451 / q6492 census-only, very large family), 33 open
(handed back for context; each APPLIED line carries the HEAD census; q4492 / q4522 have census only). Rows: 14 ported (all
RELOCATION_MATCH + PASS in recheck_final.log); 8 graded and blocked (0x006D0630 3 forms, 0x005D66BA 1 form [pick-any]; 0x006E9270 /
0x006E92C0 matched drafts, test rewrite not attempted; 0x006E93C0 0x006E9700 0x006E9E40 0x006EDC50 transformer DIFFER after 1 form);
about 120 census rows not attempted (grade '-', note 'not attempted'); 1 deferred (0x006D41F0, NF181); the rest not-a-binder.
Header opt-ins: 2 (hdr/README.md).

## Census
- work/probe.tsv: every landed REFS row of every entry compiled once at HEAD (probe.py, per (row,target)); work/cmp.tsv compares the
  bound symbol with the entry's claim (work/correct.tsv: the claim, hand-fixed where the queue line's columns were shifted:
  q6206 q6207 q6208 q6492 q6507 q11672 q11773 q12137 q12040). 1734 (row,target) pairs, all RELOCATION_MATCH at HEAD.
- Rows graded on "the largest function" by the probe (no matching Leaf; work/largestfn.txt) still bind the right symbol.
- q4457 NONE rows (0x00BBDDD0 0x00CBFB7D 0x00D40CF0 0x00DA0EF0 0x00E9BBC0): the exe scan hit is a coincidental absolute dword
  0x0099E900 inside the body (no call); not binders.
- 0x0099EFB0 (q4459 / q4932 / q6507): all 728 REFS rows bind ??4CCharString@@QAEABV0@ABV0@@Z. The WORKLIST lines (009e6fa0 SymbolString,
  004d0f80 std::CCharString, 008db030 / 0049b180) are stale reloc_bindings witness lines: those rows now bind the PDB spelling.

## Entries done out of strict queue order
- Census-only terminal states (done-already) were recorded for q4387 q4459 q4496 q4796 q4804 q4932 q6507 right after the census;
  no row was changed for them.

## Pick-any blocks (decision needed from the coordinator)
- The rule (EXTRA_RULES 2026-11-20) holds a port whose object newly defines a COMDAT copy of a function that has its own landed row
  unless the copy is byte-identical. VC7.1 here emits every used inline template member as a COMDAT, so a binder port onto
  stlport/fable_map.h in a TU of a different optimisation regime (/O2 or frame-pointer /Os) than the /Os owner emits a
  non-identical copy of the owner. Question: does the rule hold a port whose ONLY new owned COMDAT is a shared-header inline that
  differs solely by TU regime (the same copy retail's own TU would have produced and the linker discarded)?
  - 0x006D0630 (q4385): visible member-template find: 71-byte copy of ??$find@J@ v owner 0x005937E1 20 bytes; noinline form 21 v 20
    (`mov eax,[esp+8]; push eax` v `push [esp+8]`); decl-only DIFFER(479v459). HEAD debt: its object already defines a ??$_M_find@J@
    copy (owner 0x00592F92, not compared) and the non-template ?find@?$_Rb_tree@J... copy.
  - 0x005D66BA (q4386): the port emits map<unsigned long,long>::find (owner 0x005D8548): 22 v 20 bytes. Landed 0x005CDCCB already
    emits the same 22-byte copy at HEAD (debt).
  - Question, not an action: 0x005D8548's own header calls its map::find identity "probable"; the same 20-byte wrapper at 0x005937E1 /
    0x006E6990 is named _Rb_tree::find<K> under D1. If the coordinator rules 0x005D8548 the tree member template, the 0x005D66BA draft
    has no owner clash (its object defines no ??$find@K@).
- 0x005D8933 map<unsigned long,long>::operator[] is still bound as ??ACKillMap@@QAEAAJABK@Z by 0x005D66BA (blocked); the draft binds the
  STLport spelling.

## Tests
- 0x006E76E0: the landed test is a standalone model (it never links the row), so it stays unchanged and PASSes.

## q4389 (_M_find<unsigned __int64> 0x006F0250, CCombatManager family)
- Transformer tools/xcomb.py: deletes the row-local CRetailRbMap_UID_CombatantInfo stand-in (and, where present, its same-TU
  `__declspec(noinline)` find body), types `Combatants` as `std::map<unsigned __int64, CCombatantInfo>` from stlport/fable_map.h,
  rewrites `->value` / `->key` to `->second` / `->first`, and defines MFIND_DECL (stand-in find was declared only) or MFIND_NOINLINE
  (stand-in body was same-TU noinline) + CONSTFIND. The row-local `struct CCombatantInfo` (HEAD token, key U as the PDB) stays; no new
  class token. Proven by hand on 0x006E9270, scripted on the rest.
- 10 ported (RELOCATION_MATCH + PASS). The MFIND_NOINLINE rows (0x006E9680 0x006E9D10 0x006E9D90 0x006EA2C0) newly emit the _M_find
  COMDAT: byte-identical to owner 0x006F0250 (84 bytes; cmpbody against the owner row's object) and less<unsigned __int64>::operator()
  (0x00526120, 37 bytes, identical to retail). They used to define the stand-in body strongly under the stand-in name.
- 0x006E9270 / 0x006E92C0 RELOCATION_MATCH but their landed tests link the row against a CRetailRbMap_UID_CombatantInfo::find stub:
  LINK_FAIL; blocked until the tests move onto stlport/fable_map.h (drafts work/*_blocked.cpp).
- 18 rows left (STATUS lists each reason): DIFFER drafts 0x006E93C0 (137v139), 0x006E9700 (146v145), 0x006E9E40 (125v125), 0x006EDC50
  (328v328); the rest use other stand-in shapes (SCombatantMap / CCombatantMap::FindNode, CombatMap::FindLE, CCombatantInfo_Retail,
  local STLport pieces that clash with fable_containers.h).

## q4457
- 0x008D9530: the infix `previous != group` on the CCharString-derived stand-in locals binds STLport's rel-ops template operator!=
  (inline ==; DIFFER 287v278) unless fable_string.h declares the member: FABLE_RETAIL_CCHARSTRING does (only binding change at HEAD
  diff: 0x0099E900). Row keeps its other local models (pre-existing).

## q4478 (judgement for the coordinator)
- 0x00918640 binds vector<CThingGameObject*>::push_back at 0x00436C80 from the shared headers with its own element type (the function
  returns CThingGameObject*, PDB leaf). Treated as the nf104 folded identity (not a stand-in), so the entry is done-already. The local's
  element type is inferred from the return type and the casts, not from a PDB local record.

## q4385 / q4388 / q4386 shared-header opt-ins
- engine/CGameScriptInterface.h FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP; engine/CScriptConversation.h
  FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION (hdr/README.md).
- 0x006E76E0 self rename (RENAMES s1): July stand-in free function -> ?SetTimer@CGameScriptInterface@@UAEXJJ@Z (both PDBs, vtable slot 89);
  exe scan: no landed row references 0x006E76E0 (vtable only).
- 0x006E5800 / 0x006E5950 also move their conversation callees 0x006E5790 / 0x006E58A0 off the CScriptConversationCalls stand-in onto the
  spellings the self rows already define (narrowing). Their own leaves keep the pointer parameter forms; both PDBs take CThing& /
  const CCharString& (NAME_FIXES, a family with callers 0x00890710 / 0x00890750).

## Funclets / flags / descriptive names
- No landed EH funclet includes any changed row (grep of *nwind* sources). No row needed a new `// Flags:` line. No new made-up names.

## STATUS layout
- One line per row: ported / BLOCKED / deferred lines are explicit; census rows that bind every claim they were probed for are
  `not-a-binder`. A not-a-binder row that still binds a stand-in for an OPEN entry carries a `NOTE: for open entry ...` suffix.

## Locks claimed beyond rows.txt (release at landing)
- 006e5950 006e92c0 006e9310 006e9390 006e93c0 006e9450 006e9680 006e9700 006e97a0 006e9d10 006e9d90 006e9e40 006e9ec0 006ea050 006ea2c0 006ea450 006ea540 006eb9f0 006ed270 006ed9e0 006eda80 006edc50

## Questions for the coordinator
- Pick-any regime question (above), and the 0x005D8548 map::find vs _Rb_tree::find<K> identity question.
- engine/CGameScriptInterface.h (1,721 includers): FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP retypes the existing ScriptTimers
  member under an opt-in (default path unchanged). Does that need a claimed `header` item?
- q4478: 0x00918640's vector<CThingGameObject*> treated as a folded identity (two sources: the PDB return type + both PDB name sets hold
  that push_back instantiation).

## Resume hints
- q6208, q11709, q11773, q11955, q12141: every landed binder already binds the claim; only the self row needs re-authoring (a self
  rename with no caller split).
- q5223..q5236 (CCountedPointer dtors at unaligned /Os addresses): expect the declaration-only dtor form (pick-any across regimes).

## vland warnings in recheck_final.log (all pre-existing, none on a symbol this hand-back changed)
- 0x006E9EC0 STANDIN_LEAF (its HEAD self leaf ?ReassignAttackerOnVictim@CCombatManager_Retail@@...: catalog GetCombatantInfo).
- 0x006EA540 RELOC_NAME on OnActionPerformed (0x008B0600) / ResetAttackerSequence (0x006EA2C0); 0x008D9530 RELOC_NAME on
  NativeStringValue copy ctor (0x0099EC30) / AnimationGroupFilter::SetAndHash (0x0073DFF0).

## Rule slips
- Two Bash calls exited non-zero (a tool-script TypeError while patching xcomb.py; a stale path in an early src.py call).
- STATUS lines for the 10 q4389 ports were written in one batch after their shared vland run, not after each row.
- Census-only terminal states (done-already) were recorded right after the census, ahead of queue order.
- grades_used was first hand-entered, then rebuilt from work/grades.log + the vland payloads.
