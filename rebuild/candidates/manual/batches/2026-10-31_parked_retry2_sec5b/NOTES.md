# Section 5b parked-row retry sweep (wave 2) -- notes

39 rows: 5 MATCHED (all retail_asm: 00af90c0, 00b0ffc0, 00b0fbf0, 00b00db0, 00afb7c0; dry vland 5 RELOCATION_MATCH PASS),
16 IMPROVED, 7 HELD, 11 UNATTEMPTED (session budget; listed in STATUS.tsv). Plain `src/` is empty (no plain-C++ match).

## Per-lever win counts (a "win" = the lever lowered the graded diff on that row)
| Lever | Wins | Rows |
|---|---|---|
| Landed FloatToLong / RoundToDecimals helpers + `// Retail-asm:` line (bare fistp GameCODA drafts) | 5 | af90c0 b0ffc0 b0fbf0 b00db0 afb7c0 (all matched) |
| aligned (align(16)) matrix / C4DVector locals | 3 | ae1320 ae29d0 a5cee0 |
| inline value-returning stream Read helpers (temps share the dead stream-argument slot) | 2 | a495f0 9afb90 |
| PDB bit fields set in the mem-initialiser list | 2 | afb7c0 b012e0 |
| /Zi flag (prologue shape) | 2 | a5c360 adf960 |
| stepwise sum in retail operand order | 2 | ae1320 ae29d0 |
| __forceinline on helpers VC leaves out of line (CRC loop, map Add/push_back) | 2 | 9bd460 9afb90 |
| compare polarity rewrite (`!(a <= b)`, `!(x > 0)`, short-circuit bool helper) | 2 | a880c0 9ae7c0 |
| if/else around a failure path (moves the error block to the tail) | 1 | 9ae7c0 |
| out-parameter virtual getters (retail reads the output slot, not eax) | 1 | a880c0 |
| `long` loop index (lands in edi) | 1 | 9afb90 |
| virtual method on the base layout (`_base[0x30]` + vptr) | 1 | b079b0 |
| block-scoped named CCharString local (push order after construction) | 1 | b079b0 |
| CRC helper as two length-first ternaries | 1 | 9bd460/b079b0 family |
| STLport layered erase + `__fastcall` __destroy_aux + noinline _M_fill_insert stand-in | 1 | a495f0 |
| inline static type-test helpers (FableWin IsRenderMesh / IsAnimation) | 1 | a276d0 |
| memset intrinsic for array zeroing | 1 | a276d0 |
| GFFloatAbs integer-and + forced-inline trig | 1 | b16410 |
| intrinsics in retail order / C4DVector weight (SSE rows, not retail_asm) | 2 | ae0e60 a5c360 |
| double max converted to float before subtract | 1 | a781a0 |
| default literal from retail (EmitterLifeSecs 10 s, TrailBlendMode 4) | 2 | b103f0 b079b0 |

## Recurring residue (no lever found)
- one-byte / tag-object stack-slot packing (a495f0 11 lines, 9afb90 frame 0x24 vs 0x1c);
- `xor cl,[mem]` bit-field write-back fold vs and/or insert (b079b0, 9ae7c0 Template bit);
- register / x87 slot homes (a276d0, 9f57a0, 9bd460, a880c0);
- shared-header ~CDefPointer zeroes Object, so an address-taken CDefPointer stays in memory and VC tail-merges the
  following blocks (9ae7c0) -- retail keeps it in a register.

## Commands
`bash v.sh <addr> <file>` (grade), `python3 sbs.py tmp/v_<base>.txt` (side by side), `python3 ann.py 0xADDR LEN`
(annotated disassembly), `python3 fr.py` (esp -> frame offsets). finish.py is not needed (no plain matches; it
expects test/ for retail_asm rows and fails on them); finish_rasm.py authored the 5 retail_asm rows.
