# Naming pass, wave 7 (one agent; the chunk-564 authoring agent shares the lane)

Landed: operator=, waves 1, 2A, 2B, 3, LowerBound, 4A, 4B, 5, and wave 6 (applied in the repo WORKING TREE, being
committed: conflicts settled, Transfer member templates, CDataOutputStream.h member section, CCharString protected
members, GetPItem / GetMainPlayer / GetPlayer reference forms; `_$E2` binding dropped). Records:
`rebuild/candidates/manual/naming/2026-10-2*`. Owner decision (2026-10-29): wide text is `unsigned short` (`G`); never
`/Zc:wchar_t`.

## Scope, in this order (stop at ~500 edited rows)
1. Chunks 562-563 NAME_FIXES (`naming/w7/QUEUE_562_563.tsv`: chunk, address, correct_symbol, evidence,
   wrong_spellings_seen): merge into DECISIONS like waves 1 and 6 did, check against the PDB name lists, apply the proven.
2. One address, several bound names (from chunk 562/563 "Review at landing"): 0x013B8AB8 (CMovie::PEngine /
   CMovie::Engine / g_pRenderDevice), 0x013B8AC0 (CMovie::MovieTexture / two g_pMovieTexture), 0x0070B8A0, 0x0070BA00,
   0x00527910, 0x004378C0 (several unrelated spellings: check for ICF-folded bodies -- identical bytes reached from
   unrelated callers -- before collapsing; if folded, keep the names and say so).
3. `naming/w6/DEFERRED.tsv` (130): GetPItem rows with a `CIntelligentPointer<T>` wrapper (46) and non-const GetPItem
   declarations (34), GetPlayerManager / PeekPlayerManager stand-ins (~18), GetMainPlayer one-off helpers (~74), the cheap
   rest; then the cheap `naming/w5/DEFERRED.tsv` items.

## Rules and hand-back
As `naming/BRIEF_W6.md` (idempotent `fam_*.py` / hand scripts + verified file lists, a composed overlay re-made from the
CURRENT repo before the final recheck, BINDINGS.tsv with action/symbol/target/by_row/note -- RETIRE / KEEP / RETARGET /
ADD, LABELS.tsv with proven names only, DEFERRED.tsv, RENAMES.tsv, recheck logs; final message = report). Work only in
`naming/w7/`. Never run git; never write under /home/user/FableDecomp; never kill a process; no recursive find/glob over
`/` or the whole scratchpad; leave no background job running when you report.
