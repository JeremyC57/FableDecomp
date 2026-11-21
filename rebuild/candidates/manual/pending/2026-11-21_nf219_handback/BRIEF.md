# Name-fix agent NF219 (2026-11-21, round 10): the NullString static pair, two catalog names, the CCountedPointer ReleaseData family,
# and the size-TU operator== copies as a header opt-in

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; no authoring agent is running
right now (one may start; its locks will read AUTH5).

## Your brief is NF215's / NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section applies unchanged) and `$S/w22/agentNF219/NF214_BRIEF.md` in full with everything it
says to read first. Paths `agentNF214` / `agentNF215` -> `$S/w22/agentNF219`, names -> `NF219`. On a conflict: this file > NF215 BRIEF >
NF214 BRIEF. Read NF218's hand-back (`rebuild/candidates/manual/pending/2026-11-21_nf218_handback/`) and NF216's
(`pending/2026-11-21_nf216_handback/`), and `batches/2026-11-21_landing_nf218/README.md`. Lane caveat (FINDINGS 2026-11-21): a bare
CompilerFlags catalog value makes recheck.py add /Oy-; grade such rows with q.py and the row's own `// Flags:` line. Locks of finished agents
read `released`: claim them with plain mkdir/echo.

## Entries (ENTRIES.tsv), in this order; rows locked to you: rows.txt
1. **q12387: the 0x01223EC0 + 0x0122A5E0 static initializer / atexit pair, as ONE unit** (EXTRA_RULES "static initializer / atexit pairs").
   0x0122A5E0 is the dynamic atexit destructor for `CCharString::NullString` (0x013BD804) that 0x01223EC0 registers. NF218's probe
   (`pending/2026-11-21_nf218_handback/work/0122a5e0_probe.cpp`): compiling 0x01223EC0's source unchanged and grading its `_$E2` against
   0x0122A5E0 gives RELOCATION_MATCH binding `?UnassignString@CCharString@@IAEXXZ`. Both rows hand back together; fix 0x0122A5E0's
   propagated `_Cons_val<...>` catalog name (RENAMES section 1; the coordinator renames files). Link-check the DATA definitions: the pair
   must not define NullString / the live-string count twice against each other or against any landed row (objdump -t on both objects).
   0x0099E9B0 then ends on one spelling (removes `?Invoke@AutoTinyMethodTarget@@QAEXXZ`).
2. **q12388** (0x00C44550): the self row is the July placeholder `?Release@CMarkedBuffer_00c44550@@QAEXXZ`; binders use other spellings.
   Settle one descriptive name (libpk, no PDB; `CircularQueue::Release` is the candidate, matching its siblings GetInfo 0x00C442F0 and
   Clear 0x00C443D0) and respell the self row and every binder. Mark it descriptive.
3. **q12389** (0x0099B4D0): the catalog says `CCharString::UnassignString`, but all its reloc_bindings spellings are CWideString.
   Settle from both PDBs and fix the catalog name (RENAMES section 1) and the row's leaf if needed.
4. **q12226-q12229** (0x004190B6 / 0x00419108 / 0x0042F974 / 0x0042F9A0, `CCountedPointer<T>::ReleaseData` for CLUA, CGraphicDataBank,
   CIEngine, CASoundBank; FableWin.pdb names): a family pass. Census first, and include the `??1CCountedPointer<NUISystem::CComponent>` conflict
   the nf214 recheck warned about (0x00429C53 v 0x004291DE: `RELOC_CONFLICT ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ -> 00429c53
   here, 004291de in landed 00570332`). Lock every family row up front; the unit is a row.
5. **q12383, 0x005A4CDD and the four size-compiled TUs (0x00CBFB7D, 0x005A5B31, 0x005A04F7, 0x0065A075)**: advisor (provisional): do NOT land
   NF216's row-local explicit specialization of `==`. Recast it as an add-only member-specific opt-in in fable_string.h that mirrors the
   SIZE_BODY scaffolding (2026-11-20 size-pragma rule; the nf184 / nf210 / nf213 pattern in stlport/fable_containers.h), so the size-mode TU's
   pick-any copy is the owner's 88-byte body. Link-check every such row against owner 0x004115A0. NF216 FINDINGS: a pick-any template copy
   takes the optimize mode in force at the END of the TU.
One entry to a terminal state before the next. Report briefly when done or when your context gets heavy (hand-back files current first).
