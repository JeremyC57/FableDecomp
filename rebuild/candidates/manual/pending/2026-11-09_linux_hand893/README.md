# Chunk 893 hand-back (stopped early, owner 2026-11-09): shared tails / block placement family, 17 rows

The second playbook family push (rows in FAMILY.tsv, from PLAYBOOK_CANDIDATES.tsv family `xj`). Stopped by the owner after
about 15 minutes of work. Brief: CHUNK_BRIEF.md. Log: TRACE.md (findings) and ATTEMPTS.log (every grade).

## Matched, not landed (need a C++03 behaviour test, then vland)
Each regrades RELOCATION_MATCH from `matched/` (q.py, current rebuild/include, no overlay); 0x0085FBC0's October
`__forceinline`s were respelled FABLE_FORCEINLINE before the regrade.
- 0x00C73830 CRegistryKey::QueryDWord: `if (a) return 0; else if (b) return 0; else return c;`.
- 0x0085FBC0 NCreatureMode::IsAbleToBeEngagedInCombat: the same else-if chain ending in `else { ...; return dragon ? false : true; }`.
- 0x009D06F0 CBankFile::OpenReadOnly: the dev-path tail wrapped in `else { ...; return OpenFileReadOnly(path, flags); }`.

## Findings (micro probes probe/xj1..xj7.txt, tools/micro.py: many functions in one TU, jump skeleton per function)
- Duplicate `return 0` blocks: when the function's last statement is a plain top-level `return`, the LAST duplicate keeps the
  tail; when the last statement is an if/else whose arms all return (an else-if chain, or a final `else { ... return x; }`), the
  FIRST duplicate keeps the tail and later ones jump back to it. Labels and locals do not matter.
- Return-block deferral: `if (c) return 0;` with one condition stays inline; `if (A || B) return 0;` with simple A, B defers the
  return block to the function end; `if ((A && B) || C)` and two separate ifs keep it inline; `if (c) X(); else return 0;` moves
  the else arm past the join, and inside an else-if chain the whole enclosing arm moves.

## Baseline (all prior sources regraded as-is on the current headers): every row still DIFFER at its October size.
Smallest residues next: 0x00D8A240 (md 33, two probes), 0x00C11230 (md 24, the deferral rule above applies), 0x00D39960 (md 61),
0x00D6C830 (md 66). 0x00839940 needs the hand877 overlay.

## Resume
Rebuild the work dir from this folder plus prior sources (FAMILY.tsv `source` column), regenerate the oracle with
`$S/w22/oracle_from_exe.py rows.tsv chunk893` and the scripts with chunk_pool/mkprompt25.py 893 (needs $S/w22/chunk893.tsv
built from rows.tsv + the oracle bytes; never commit those bytes).
