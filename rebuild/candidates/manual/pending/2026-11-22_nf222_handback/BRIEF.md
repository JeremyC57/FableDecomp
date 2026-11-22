# Name-fix agent NF222 (2026-11-22, round 10): finish the CCountedPointer<T>::ReleaseData family and hand it back whole

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. An authoring agent (AUTH9) is
running; its locks read AUTH9: leave them alone. Codex pass 27 rows (UNMATCHED_ROWS "reserved (Codex pass 27 ...)") are off limits.

## Your brief is NF221's with overrides
Read `$S/w22/agentNF221/BRIEF.md`, `$S/w22/agentNF215/BRIEF.md` ("Lane" applies unchanged) and `$S/w22/agentNF222/NF214_BRIEF.md` in full,
with everything they say to read first. Paths -> `$S/w22/agentNF222`, names -> `NF222`. On a conflict: this file > NF221 > NF215 > NF214.
Read NF221's hand-back first: `rebuild/candidates/manual/pending/2026-11-22_nf221_handback/` (NOTES.md above all, "Open" and the coordinator
decisions; STATUS / RENAMES / NAME_FIXES; rows/ = its 42 ported rows plus the 4 that landed) and `batches/2026-11-22_landing_nf221/README.md`.
NF221's working files: `$S/w22/agentNF221/work/` and `tools/` (read a tool before using it).

## Landed since NF221 (do not redo)
q12403 landed as nf221: 0x0042D45D, 0x0058F016, 0x005C8459, 0x005C9A95 now bind ReleaseData<CComponent>, and the reloc_bindings
`??1<CComponent>` line now targets 0x00429C53 (witness 0x00563920). Their locks are released. Start from the repo tree as it is now.

## Coordinator rulings on NF221's two decisions
1. **0x005BFF07 is held** under the in-place rule, condition (2): its respelled local model defines pick-any `??1<CGraphicDataBank>`
   (owned by 0x004194B5) and `??1<void>` from a local-model body. Port it onto engine/CCountedPointer.h (the SerialisedItems pointees first),
   or use a row-local explicit specialization of only the `<CGraphicDataBank>` destructor if the port cannot be done. Its object must
   define no symbol naming CCountedPointer beyond what a shared header defines identically; compare every pick-any copy it defines with
   its owner's retail bytes (relocations masked).
2. **0x0041947A: Reading A.** The family hands back whole: every referencing row ends on the new spelling. So do the two AcquireFont rows.

## Entries, in order
1. **0x00A5F1C0** CStreamingFontDataBankEntry::AcquireFont and **0x00A634F0** CFontDataBankEntry::AcquireFont: NF221's recipe (NOTES
   "Open"). Self signatures change only with two sources (the QBE const fix needs both PDBs plus the callers 0x009E2C10 / 0x009E2BA0:
   census and respell them in the same hand-back, or leave the self leaf as every referencer binds it and queue QBE).
   0x00AB8E10 (CStaticFontBank ctor): census its binders before the CStaticFontBank.h port.
2. **0x005BFF07** per ruling 1.
3. **Whole-family recheck** on today's tree: all of NF221's 42 rows plus yours, in one vland `--recheck-landed` run, every landed caller of
   the family addresses, and the census of every family address's spellings (one each, 0x0041947A included). Compare the new pick-any copies
   (`DeleteData<T>`, `??1` / `??4` copies, ReleaseData copies) with their owners' retail bytes, which NF221 did not do. List NEQ copies with
   their HEAD state (a copy that differed at HEAD may stay as debt, nf203; a new NEQ copy holds its row).
4. Hand back the whole family: rows/ (all rows, NF221's included, re-diffed against the tree of the day), STATUS, APPLIED, RENAMES, NAME_FIXES,
   NOTES, LESSONS, recheck_final.log, and the table lines (reloc_bindings) that go stale or move.
Never write into /home/user/FableDecomp; never run git. Report briefly when done or when your context gets heavy (hand-back files current).
