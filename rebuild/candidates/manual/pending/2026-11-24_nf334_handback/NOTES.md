# NF334 hand-back (2026-11-24)

Scope: the 329 T1 `other` tracker entries in ENTRIES.tsv. Tools: NF332's helpers copied into tools/ and retargeted (agentNF334, lock owner NF334).
New tools: mkoff334.py (stay-off lists incl. NF333's targets / witnesses), tri334a.py / tri334b.py (per wrong table line triage + scope compare),
class334.py (HELD / PORTC / MAN script classes), wl334.py (witness lines), pdb2.py (two-PDB check), lr334.py (line-anchored zero-delta edits),
st334.py (census STATUS lines), assemble334.py (hand-back files).

## Counts (STATUS.tsv: 325 lines; 4 entries not reached)
- done 32, done-partial 6 (38 entries; 31 changed rows, rows.txt; every changed row's lock is kept for landing, all control / census locks released).
- rejected 1 (entry 174) + 2 tried-and-reverted rows recorded as skipped-port (entries 274 / 276; REJECTED.tsv lists all three).
- skipped-port 150: 79 by census (st334.py, NF299 Q4; 1-in-10 sample opened by hand: entries 13 76 134 183 217 241 280 302, all stand-in
  scopes, noted in their STATUS lines; header clause scripted by tools/hdrchk334.py over 83 reachable binders: 3 hits opened, 176 / 219
  stand, 238 was fixable and is done), the rest opened by hand with a reason each.
- skipped-held 136: 86 all-witnesses-off-limits (census), 33 destructor family (decision D1), 17 suspect identities / folds (decision D3).
- Not reached: 10, 12, 25, 40 (multi-binder entries: 38 / 70 / 30 / 100+ binders in several forms incl. enum / virtual / stand-in types).

## Proof set (every changed row)
HEAD and new graded at one path (nf.py grade, RELOCATION_MATCH: work/gr_*.log), objsec ALL_EQ under the rename map (.drectve included,
DEF / UND / COMDAT sets equal: evidence/proof_*.txt), link check HEAD set v new set (evidence/link_*.txt: the old spelling unresolved at
HEAD where the watch regex caught it, the new one resolves against the self; entry 149's self 00cb8960 is an NF333 witness and was not
linked in; the g6 watch regex did not match `?Clear@?$CVectorMap`, so g6 shows only that the new set resolves DeallocateMemory), vland
--recheck-landed PASS for all 31 rows (recheck_final.log). apply_table.py --dry: 32 replace + 8 delete, 0 misses, no new multi-target
symbol (evidence/apply_table_dry.txt); every replace was checked against the table so none creates a second line for an existing
(symbol, target) pair (three became deletes: 232, 8, 39). Exe-census binders of the g8 entries compiled at HEAD and read with bindaddr
(evidence/bindaddr_g8census.txt): this found 007266c0 still binding the entry-221 pointer form (no table line), now respelled. Two-PDB checks: evidence/pdb2_g2.txt, pdb2_g48.txt.

Comment-only edits after a grade: 009e5250 line 10 (callee name in the header comment; before the g48 recheck) and 00abf840 line 94
(evidence wording; rechecked again alone: evidence/recheck_g8c.log).

## Groups
- p1: 004017b0 (1, 2: extern "C"), 009a6610 (3, 271), 00786700 (7).
- g2: 14 rows (74 75 97 100 104 112 140 178 184 187 198 226p 262p 263).
- g3: table-only stale witnesses (NF309 Q4): 6 33 48 49 50 53, 52 partial.
- g4: 0096f240 (18), 004afa60 + 004b26b0 (149), 00967180 (232).  g5: 0069ffd0 (51), 00af5900 (125 partial).  g6: 009e5250 (256 partial).
- g7: 004b3900 (22, CActiveQuest re-scoped into CQuestManager, NF300 Q2).
- g8: 00999110 (8), 008597a0 (39, nested CStrikeEvent), 00abf840 (73, nested CFilenames), 00724f50 (221 partial, pointer -> reference).
- g10: 006d1de0 (238, HaveSex declared on the local real CTCWife). g11: 007266c0 (221, the same pointer -> reference as 00724f50).

## Decisions I need
- D1 (destructor family, 33 entries incl. 109): binders call free / fastcall `X_Destruct` stand-ins (or `??_G` row stubs) for the PDB
  destructors. The respell needs the class model (the shared headers are flat, vm=0) and the landed tests stub X_Destruct and link the row.
  One family decision (class port, or a sanctioned in-place form), not mass-edited here.
- D2 (entry 52): 006a0c90 / 006a0cc0 / 006a0cf0 / 006dfce0 bind 0x006A0C50 as ??0CScriptGameResourceObjectScriptedThingBase@@QAE@ABV0@@Z.
  A folded copy ctor body carrying several names (NF104), or wrong binders? I fixed only the stale table line.
- D3 (suspect identities / folds, held WITHOUT an identity check: no label_trust run; open questions, since the brief's default for a
  wrong-function binder is to fix it): 4, 15, 23, 56, 79, 92, 103, 136, 150, 194, 204, 206, 229, 278, 282, plus 110 (Refresh() v the
  PDB UpdateMarkers(bool)), 144 (fastcall binder v thiscall PDB member) and 004d297b in 221 / 00af5870 + 00af5890 in 125. Each binder calls a
  different real function (or a free stand-in) at the target. Either retail folded the bodies (several names per body, leave), or the
  self label is propagated (fix the self), or the binder is wrong. This needs a per-address ruling.
- Q1 (entry 174): re-scoping 00b21630's CSplineRandomOffsetData into CPSCSplineBase (+ const) graded RELOCATION_MATCH and every section
  stayed byte-equal. The only change: the TU's non-inlined COMDAT CCountedPointer<CSplineRandomOffsetData>::operator-> takes the
  re-scoped (PDB-correct) type name, so the COMDAT set changes. Should a template-instance COMDAT renamed onto the PDB type count as T1?
  I reverted it (REJECTED.tsv).
- Q2 (entry 42): the self row takes `const CThingCreatureBase&`; the PDB is the private PerformSequenceReset(CThingCreatureBase&). The
  owner self fix comes first (NF302 Q5), then the binder 006ea540.

## Other findings
- 004b26b0 (entry 149) emits pick-any COMDATs of CScriptThing members (??_7CScriptThing, ~CScriptThing). NF328's CENSUS lists it as a
  pick-any (bytes-equal) definer, not a strong definer. My change only respells the OnScriptedEntityActivated call and leaves those COMDATs
  byte-equal (objsec ALL_EQ, COMDAT set SAME). If the stay-off on "CScriptThing definers" covers pick-any definers too, drop 004b26b0 and
  make 149 done-partial.
- 0x00997F80 (T3 nf285) has wrong caller spellings that the table does not show (NAME_FIXES.tsv).
- T3_PORT_QUEUE lines name binders in their prose column. nf308 names 00757760 (entry 266), so 266 is skipped-port; nf306 names 007868a0
  for 0x00787060, which is another callee in a row I changed (entry 226: only AddSkeletalMorph changed; 0x00787060 untouched).
- 0x0047D6F2 (the IsListReceivingInputA sibling of entry 198) is probably IsSaveMenuReceivingInput: one source only, not in ENTRIES, left.
- Entries 59 / 266 / 43 / 72 / 93 / 108 / 114 / 142: one class rename or re-scope would also move another call or definition in the same
  TU onto a different wrong form, or change a made-up global's or the TU's own definition's mangled type (STATUS reasons; NF305 Q1 / NF300 Q2).

## Progress log
- Read: standing brief, prefix, CLAUDE.md, COORDINATION, ASM_RULES, EXTRA_RULES, GOTCHAS (all), NF332 landing + hand-back, nf305 / nf308 READMEs.
- exe census of all 329 entries: work/census.tsv.
- p1, g2, g3, g4-g8 as above; recheck_p1 / g2 / g48 / g8c all RELOCATION_MATCH PASS.
- Entries 274 / 276 tried and reverted: a member-template declaration adds /alternatename to .drectve (NF303 Q2).
- 00af59a0 was edited before I saw it was an NF333 entry. I removed it with clean.py before any lock or compile.
