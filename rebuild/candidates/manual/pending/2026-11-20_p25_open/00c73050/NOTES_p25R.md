# 0x00C73050 (libpk CAudioSource slot 9) -- p25R 2026-11-21, rotated
Base: p25N c7_best.cpp (local models as every landed CAudioSource row), /d2inlT150 (a DIAGNOSTIC c2 switch, not a retail flag:
a match that needs it is not landable without proof the TU used it). DIFFER(672v672); the residue is one register: items[tail]
(c2prio #32) edi v retail edx (0x00C731D2/D5/F4); the [0x68]/[0x6c]/[0x90]/[4]/[0x104] hunks are g.py masking of g_AudioContext.

## Condition-spelling lever: CLOSED (IL-neutral), src/c7s2 (71 variants = full product)
| dimension | values | result |
|---|---|---|
| IsEmpty body | `cap != 0 && stats0 == 0` / `cap && !stats0` / `if (cap == 0) return false; return stats0 == 0;` | 672 edi / 672 edi / 677 edi |
| m_tail type | unsigned long / long | 672 edi |
| items[tail] test | `== 0` / `!` | 672 edi |
| IsLooping body | `(f >> 8) & 1` / `(f & 0x100) >> 8` / `((f >> 8) & 1) != 0` | 672 edi |
| m_current | ternary / if-else | 672 edi |
Every combination: 672 edi (the a3 IsEmpty arms 677 edi).

## Other spellings (src/c7s1)
| change | grade |
|---|---|
| `processed--` / `m_queued--` (w3) | 672 edi |
| `while (processed > 0) {...; --processed;}` (w1), guarded do-while (w2) | 603 (the T150 inline decisions change) |
| item assigned inside the condition, `m_current = cap ? item : 0` (w4) | 672 edi |
| named `tail` local (w5) | 674 (CSE changes: cmp [edi+eax*4]) |
| `!items[tail]`, `!tail` (w6) | 672 edi |
| `buffer = 0` (w7) | 680 |
| buffer declared before `--m_queued` (w8) | 672 edi |
| `processed = 0` (w9) | 683 |

## Next (main 2026-11-21)
Candidate #30 (the processed-loop temp, lines 118-119 of c7_best) is the one p25N's mask analysis points at: try a text where its
range is shorter / its last use is before the IsLooping tuples. Find a source-level equivalent of /d2inlT150 (or proof the TU used it).
