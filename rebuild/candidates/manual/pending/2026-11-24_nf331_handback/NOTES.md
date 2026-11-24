# NF331 (2026-11-24): the CScriptThing::operator= pick-any unit (T3 `nf328 0x008AB980 unit`)
Brief: BRIEF.md + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read UNIT.md first (census, proof, debt, decisions).

## Result
- **Ready to land: one row, 0x008AB980** (rows.txt, rows/008ab980.cpp; its landed test is unchanged). The self's strong operator= becomes
  `inline` and is emitted pick-any through ONE `?A0x` helper in NF328's landed device. The census finds no other strong definer, so no
  other row changes.
- **Proof:** pair grade at one work path RM / RM; uproof OK on the pair-grade objects AND on vland's recheck objects (selection 1 -> 2, the
  74-byte member equal to HEAD and to Fable.exe, only new DEF = the helper, UND / `.drectve` / every other non-debug section equal; no
  `??0` / `??1` / `??_7` / `??_G` emitted); vland --recheck-landed RELOCATION_MATCH + test PASS on both sides, no warnings; census after
  0 strong definers; link of all 497 census objects: exactly 90 `??4CScriptThing` duplicate lines removed, none added, unresolved sets
  identical; pickany_vs_retail on the self object EQ (HEAD and new alike).
- **Debt:** 20 pre-existing pick-any copies differ from retail (DEBT.tsv; 17 call-shape, 1 stand-in callee, 2 ShareData). Unchanged; queue.
- **Decisions:** UNIT.md D1 (B-claimed census hole 0x00E97070, text-only), D2 (how to queue the debt).

## Process
- Reading list done before the first compile. Tools copied from agent_tools/nf328 and adapted (tools/; new: srccensus_op.py, census_op.py,
  census_report_op.py, stayoff.py, zsbatch.py (passes the row's CompilerFlags; NF328's zsbatch.sh passed none), linkdelta.py; uproof.py's
  member set and hcensus.py's resumability changed; NF328's srccensus2 / census2 / census_report copies were not used and are removed).
- Gate closure greps after the census (UNIT.md): no includer of the self's file, no IMPLICIT_ASSIGNMENT define in a header, and all 183 files
  with out-of-class `CScriptThing::operator=(` text are gate rows. DEBT.tsv carries a referenced-in-own-object column (all 20: no).
- Stay-off screen (tools/stayoff.py, evidence/stayoff.tsv): Codex RESERVED.tsv (not landed), B claims (newest line), acctB_b24 rows,
  UNMATCHED acctB, other agents' locks, the held list, other T3 lines. In the gate: 1 B claim (0x00E97070, not compiled), 45 rows on other
  T3 lines (compiled census-only, not edited), 0 Codex-reserved, 0 NF330 locks.
- Prechecks: 497 HEAD sources (435 clang PASS; the other 62 fail clang only on MSVC-only text in their own unchanged source -- deleted
  virtual dtors of local models, a std::map dtor specialization, non-const reference binding -- and passed `cl /Zs` with their own flags in
  an earlier call, D1 / NF300 extension; evidence/pre.log, pre_zs.log). The new row and its unchanged landed test: clang PASS.
- Locks: 0x008AB980 stays NF331's for the landing; the census-only locks are released.
- orig/008ab980.cpp equals the repo at hand-back.

## Names
- The helper `ReferenceScriptThingAssignment` had 0 hits under rebuild/ and tools/ before this run; it lives in the TU's anonymous namespace
  (`?A0x...` hash per compile path), so it never meets another object's symbol. No symbol is renamed; the member spelling is in both PDB
  name sets (Ego_r, FableWin) and is the one reloc_bindings.tsv binds (witness 0x00CBF9DE).
- T3_PORT_QUEUE screen: 0x008AB980 appears only in its own `nf328 ... unit` line (column 2).

## Rule slips
- Two rescan-sample locks (0x00DDB720 Codex pass 27, 0x00E76E90 B claim) were taken before that sample was screened against stay-off;
  released at once, never compiled, dropped from the sample.
- The self's landed test was prechecked after its lock was claimed (the brief says before). No grade compiled it before the precheck passed.
- The first census link ran with only the self object (a script quoting slip, LESSONS.md); it was rerun with all 497 objects. Only the
  full runs are in evidence/.
