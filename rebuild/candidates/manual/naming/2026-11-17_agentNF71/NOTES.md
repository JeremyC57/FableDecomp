# NF71 notes (round 7: the 20 rows NF66 / NF67 / NF68 left blocked)

Work dir: $S/w22/agentNF71 (grades in pS/ via g.sh, ledger grades.log, drafts work/att/, relocation dumps work/x_<addr>.txt (port) and
work/xo_<addr>.txt (baseline)). Tools copied from NF68 (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh, xref2.py, dd.py) and repointed.
Grade counts in STATUS.tsv are every q.py compile of the row by NF71 (grades.log), including regrades of earlier agents' drafts.

## How RENAMES section 2 was derived
- 0x00EE2490: from a full relocation dump of the landed-row baseline as recorded in rebuild/oracles/reloc_bindings.tsv (14 symbols at this
  row) against my final object's xref2 dump; every changed target paired 1:1. reloc_bindings keeps one example row per (symbol, target), so a
  symbol shared with other rows may be missing from the baseline; the pairs listed are all real changes.
- 0x00E12580: reloc_bindings lists only the three row-to-row bindings for this row; the landed source was read for the rest. The two
  changed spellings are WaspMenace -> EndMission (0x00E13310; the landed self row and NScript_CQ_WaspBossScript.h say EndMission, both
  made-up names, flagged by NF68) and the local `CScriptDef* pGlobalScriptDef` -> the PDB CDefPointer data symbol (q10541). The local
  CCharString / CDefString / vector models of the landed row mangle the same as the shared headers.
- 0x008F95D0: reloc_bindings lists all three of its call targets; ByteBuffer::Fill -> vector<char>::_M_fill_insert, the ByteBuffer
  Send -> the PDB Send. The self leaf is in section 1 (no direct callers).

## 0x008F95D0: the untested vector release
Retail frees SendBlock's local vector<char> with a bare `push ecx ; call free` (0x008F96B8) and ReceiveBlock's (0x008F9A70, not landed)
with `push esi ; call free` (0x008F9B20), while ~LSocket 0x008F9780 in the same TU keeps `test eax,eax ; je` before freeing its member
vectors, and the LAddress strings in SendBlock keep their tests. So the missing test is most likely VC7.1 knowledge in these two functions
(both dereference the buffer's start before freeing it), not a different ~vector in the source. Tried without effect: the visible
_M_fill_insert body (FABLE_RETAIL_STLP_VECTOR), the landed Send 0x008F8F50 body in the same TU, the Ego_d.pdb locals (retval /
data_length / templength) with memcpy. The row uses the brief-allowed opt-in FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED; the
coordinator may want to treat it as a fitted form until the mechanism is found.

## Locks
- 0x0057770A: deferred, held by NF70 (HELD.txt).

## Totals (from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Rows (21 in rows_all.txt): 4 ported (0x00E12580, 0x008F95D0, 0x00EE2490, 0x006A0150; recheck_final.log: 4 RELOCATION_MATCH + PASS),
  0 not-a-binder, 16 blocked, 1 deferred (0x0057770A, held by NF70).
  - Blocked after 15 NF71 grades: 0x004FE630 (DIFFER 453v440, register allocation only; STATUS has the forms tried).
  - Blocked, not reached by NF71 (0 grades): 0x00596CCE, 0x00597006, 0x004091C0, 0x00416953, 0x0041B550, 0x00549B20, 0x005778CA,
    0x00599E3F, 0x006D53C0, 0x008C2550, 0x008DDAD0, 0x008E0060, 0x008F3630, 0x00B9E950, 0x00BD1F70. Their STATUS notes keep the
    earlier agents' blockers (marked inherited) plus what NF71 read for five of them.
- Entries (29): 3 done (nf44:98, nf41:e161, q10502), 26 blocked. q10541 is blocked only by 0x00EE2A80 outside the slice (0x00E12580 binds it).
- RENAMES: section 1 has 2 leaf-only self renames (0x008F95D0 SendBlock, 0x006A0150 the Unsheathe ctor); no file moves (catalog names
  unchanged). Section 2 has 21 respelled bindings.
- recheck_final.log carries one WARN, RELOC_NAME on 0x00415D70: the catalog label 'CDefString::operator CCharString' is the demangled
  form of the bound ??BCDefString@@QBE?AVCCharString@@XZ (same address, same name).
- Funclets: no EH funclet under rebuild/src/compiled includes any of the four changed rows (grep of each row's file name); none of the
  four has an EH frame.

## Rows ported (how)
- 0x00E12580 NScript::CQ_WaspBossScript::DoMission: NF68's port100 text with the two dead STLport defines removed, // Flags: /d2inlT60
  (smallest matching; Inline-budget line lists T56 / T58 / T59 / T60 / T62). Replaces a "Revisit: inline-depth" row: no pragma left.
- 0x008F95D0 LHNetworkLib::LSocket::SendBlock: NF67's LSocket.h port with the Ego_d local names and the overlay release opt-in (above).
- 0x00EE2490 NScript::CV_TravellingHeroesScript::CTH_Thunder::Main: NF68's port with implicit CScriptThing copy, the landed CIsThingAlive
  predicate idiom (`#pragma inline_depth(0)` round the row-local ctor, Inline-budget line per the owner 2026-11-08 decision, preference 3
  after the flag form failed) and FableWin.pdb's GetMasterData (overlay opt-in).
- 0x006A0150 CCreatureAction_UnsheatheItemFromInventory ctor: fresh port (4 grades), overlay opt-in for CAnimationEntry's transition copy.

## Coordinator items
- fable_containers.h FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED is a retail-shaped (likely fitted) release; see the evidence above.
- 0x00E13310: EndMission (self row + header) vs WaspMenace (the old DoMission binder): now one name in the landed set once this lands;
  both are made-up names (NF68 queued it).
- 0x004FE630: a visible noinline CCharString::LoadFromFile opt-in would need the root fable_string.h (not tried; coordinator).

## Rule slips
- 14 rows ended with 0 NF71 grades (the brief's "carry each row to a terminal grade" rule), and 0x00E12580's 15 grades include NF68's two
  drafts regraded and one dump regrade.
- One Bash call's scratch script was named dis.py and broke capstone's import (rerun after renaming; no file outside my dir touched).
- Nothing written under the repo (checked /home/user/FableDecomp/work for my files); no git; no __asm / naked / _emit / #error in rows.
- 0x006A0150 (RENAMES): reloc_bindings lists only GetAnimationToUse for this row; the old spellings of section 1 (old_leaf) and of the
  other three section 2 pairs (GetAnimationFromName, the CCreatureActionBase pointer ctor, DeleteCreatureAction) are read from the landed
  source's local declarations (mangled by hand), not dumped from an object. 0x008F95D0's old_leaf is likewise read from its local
  ByteBuffer declaration (its section 2 pairs are dumped bindings).
- NF63's drafts for 0x008DDAD0 / 0x008E0060 / 0x008F3630 / 0x008C2550 (agentNF63/work/att) were read at the end: they are hybrids that
  still model CCharString / CIntelligentPointer vectors / CDefPointer / CCountedPointer locally and keep fake same-TU stand-in bodies
  (0x008C2550: FindRushPosition / IsRushPositionValid / NewPointerInfo), so adding the missing define would not make them a port.
