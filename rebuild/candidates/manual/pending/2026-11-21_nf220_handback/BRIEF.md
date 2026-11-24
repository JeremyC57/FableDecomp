# Name-fix agent NF220 (2026-11-21, round 10): the CCountedPointer ReleaseData family, 0x0099C120's header port, the size-TU
# operator== opt-in, and a measure-first census of the CCountedPointer<CComponent> destructor conflict

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; authoring agent AUTH6 runs at
the same time on 0x007602F0 / 0x00763060 (locks `AUTH6`; leave them alone).

## Your brief is NF215's / NF214's with overrides
Read `$S/w22/agentNF215/BRIEF.md` (its "Lane" section applies unchanged) and `$S/w22/agentNF220/NF214_BRIEF.md` in full with everything it
says to read first. Paths `agentNF214` / `agentNF215` -> `$S/w22/agentNF220`, names -> `NF220`. On a conflict: this file > NF215 BRIEF >
NF214 BRIEF. Read NF219's hand-back (`rebuild/candidates/manual/pending/2026-11-21_nf219_handback/`, esp. NOTES.md "q12226-q12229" census and
LESSONS.md) and `batches/2026-11-21_landing_nf219/README.md`. Lane caveat: a bare CompilerFlags catalog value makes recheck.py add /Oy-;
grade such rows with q.py / rg1.sh and the row's own `// Flags:` line. Locks of finished agents read `released`: claim with mkdir/echo.

## Entries (ENTRIES.tsv), in this order; rows locked to you: rows.txt
1. **q12226-q12229** (0x004190B6 / 0x00419108 / 0x0042F974 / 0x0042F9A0, `CCountedPointer<T>::ReleaseData` for CLUA, CGraphicDataBank,
   CIEngine, CASoundBank; FableWin.pdb names; thunk 0x004194B5): family pass over the 18 landed caller rows NF219 counted. Lock every
   family row up front; the unit is a row.
2. **q12401** (0x0099C120 operator+(const CWideString&, wchar_t), /Gr): port the row off its local CWideString model onto fable_string.h
   (FABLE_RETAIL_CWIDESTRING + the 0x0099BA70 setup) so its UnassignString copy is the pick-any 62-B header body (removes the
   0x0099B4D0 duplicate). Bytes of 0x0099C120 itself must stay RELOCATION_MATCH.
3. **q12383, 0x005A4CDD and the four size-compiled TUs (0x00CBFB7D, 0x005A5B31, 0x005A04F7, 0x0065A075)**: advisor (provisional): do NOT
   land NF216's row-local explicit specialization of `==`. Recast it as an add-only member-specific opt-in in fable_string.h mirroring the
   SIZE_BODY scaffolding (2026-11-20 size-pragma rule; the nf184 / nf210 / nf213 pattern in stlport/fable_containers.h), so the size-mode
   TU's pick-any copy is the owner's 88-byte body. Link-check every such row against owner 0x004115A0. NF216 FINDINGS: a pick-any template
   copy takes the optimize mode in force at the END of the TU.
4. **MEASURE FIRST, change nothing: the `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` conflict** (0x00429C53 thunk v 0x004291DE:
   `RELOC_CONFLICT ... -> 00429c53 here, 004291de in landed 00570332`). 0x004291DE has 576 call sites in 108 landed rows, 0x00429C53 26 in 6.
   Count how many of those rows bind `??1` (and to which address) vs `ReleaseData<CComponent>`, settle from both PDBs which spelling each
   address is, and write the census + a proposed plan to NOTES.md. Do not lock or edit those rows; the coordinator decides the pass.
One entry to a terminal state before the next. Report briefly when done or when your context gets heavy (hand-back files current first).
