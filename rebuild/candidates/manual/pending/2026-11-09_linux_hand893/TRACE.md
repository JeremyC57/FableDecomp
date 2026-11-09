# Chunk 893 TRACE (agent893) -- shared tails / cross-jumping and block placement (17 rows)

Tools: tools/gq.py (gr.sh wrapper; md = masked differing instruction lines, first = first differing instruction index),
tools/mc.py + tools/skel.py (micro compile / jump skeleton), tools/rdis.py, tools/cmpfn.py, c2probe (c2.sh).

## Step 1: baseline regrade of every prior source as-is (empty hdr/ overlay = current rebuild/include)
Baseline (11:40, all 16 non-hand877 rows, empty overlay): every row still DIFFER, same sizes as October (wk/baseline.out).
0x00C73830 md 5, 0x0085FBC0 md 41, 0x00C11230 md 24, 0x009D06F0 md 13, 0x005F6793 md 151, 0x00896B60 md 130, 0x00E36750 md 212,
0x00D6C830 md 66, 0x00D8A240 md 33, 0x00922010 md 285, 0x00683C10 md 324, 0x00EB5750 md 315, 0x00589402 md 496, 0x00EB40C0 md 429,
0x00D39960 md 61, 0x005A4CDD md 1008. (0x00839940 needs the hand877 overlay; graded later.)

## 0x00C73830 CRegistryKey::QueryDWord (72 bytes)
- Residue: two identical `return 0` blocks; retail keeps the FIRST (query failure) in place, the type test jumps back; ours keeps the second.
- Micro probe probe/qd/m1.cpp (14 spellings, one TU, mc.py): separate ifs (Q01), result local (Q04), do/while(0) with the second exit a
  break (Q08), bool local (Q11), bool return (Q13): second survives. Nested positive (Q02/Q03/Q07), ternary (Q06), `||` (Q12): one
  return-0 block at the end. **else-if chain (Q05)**, break-first/return-second (Q09) and switch on the result (Q10): FIRST survives = retail.
- v1_elseif (prior + `else if` / `else`): RELOCATION_MATCH. Lever: `if (a) return 0; else if (b) return 0; else return c;`.

## Survivor rule probes (probe/xj1.txt, xj2.txt; tools/micro.py = many functions in one TU, jump skeleton per function)
- `if (a) return 0; if (b) return 0; return c;` (E1, E5, E20-E22, E26): the LAST `return 0` keeps the tail.
- The same chain whose FINAL `return` sits in an `else` arm (`if (b) return 0; else return c;` E14, E18; full else-if chain E3, E16;
  `else { ...; return x; }` E13, E23, E29): the FIRST `return 0` keeps the tail and later ones jump back to it.
- So the survivor is decided by whether the function's last statement is a plain `return` (last survives) or an if/else whose
  arms all return (first survives). Labels/locals irrelevant (E30).

## 0x0085FBC0 NCreatureMode::IsAbleToBeEngagedInCombat
- v1 else-if only on the first two tests (final `return` still top level): md 41 unchanged.
- v2 `if (A) return false; else if (U) return false; else { mode test...; return dragon ? false : true; }`: RELOCATION_MATCH.

## Return-block deferral probes (probe/xj3..xj7.txt, the 0x00C11230 shape)
- `if (c) return 0;` with a single condition: the return block stays inline (`jcc` over it).
- `if (A || B) return 0;` with simple A, B: both tests jump to the return block, which is deferred to the function end (H1-H9, G1, G9).
- `if ((A && B) || C) return 0;` (H12-H14, G6, D7): last test inverted (`jae` over), block inline. Two separate ifs (G8): same.
- `if (c) X(); else return 0;` (F4, C2, C6): the else arm is moved past the join; with an else-if chain the WHOLE enclosing arm moves (C2).

## 0x009D06F0 CBankFile::OpenReadOnly
- v1 dev-path tail wrapped in `else { ... return OpenFileReadOnly(path, flags); }` (the function's last statement becomes an if/else whose
  arms both return): RELOCATION_MATCH (first `return false` keeps the tail). v2 else-if on the version test only: md 13 (unchanged).
