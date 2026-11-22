# AUTH24 hand-off (2026-11-22). Row 1 done; rows 2..5 not started (context).
State: STATUS.tsv. Row 1 (0x00839420) hand-back: 00839420/ (NOTES.md first). Rows left, in the brief's order: 0x00651273, 0x00816410,
0x00A68DE0 (resume from $S/w22/auth23/00a68de0/ESCALATE.md UPDATE), 0x00502E90 (advisor's checks, brief item 5).

## Tools added (tools/, copies of AUTH23's plus)
- `lf.py <obj> <leaf> [out]`: leaf disassembly with relocation symbols, for ours-v-ours diffs between two builds (no retail, no grade).
- `fw.py <obj> <leaf> <addr>`: leaf relocation symbols that reloc_bindings.tsv does not yet bind at their retail target (first witnesses / splits).
- `bscan.py <hex>`: find a byte pattern in retail .text and say which hits are inside landed rows (found two landed rows with the wanted
  x87 order this way).
- `zscan.py <target>`: call sites of a target with a zero store right before them.
- `c2w/`: a workspace for the c2 probes (lane_tools/c2probe + pending/2026-11-20_c2_symbol_order kit, paths fixed):
  `cd c2w/tools/sched && python3 host.py ../il/dump2.py <src> <out.json>` (25 s) then `python3 c2w/tools/il/fm2.py <out.json> <func-substr>`
  prints every float product's operand symbol numbers and sort key; `host.py ../il/symlog.py` (3 min) + `il/lpool.py <sym.json>
  <dump.json>` lists the symbol pool in allocation order (edit the function name inside lpool.py); `c2w/tools/inl/inl_host.py <src>
  <out.json>` logs every inline decision (sz, c1, f, score, result). All need FABLE_HDR_OVERLAY exported like a grade.

## Traps
- `odall10.py` needs `PYTHONPATH=/home/user/FableDecomp/tools/decomp_pipeline` and cwd = repo root; it leaves a .cod listing next to the
  source (per-line bytes; delete it and the .obj afterwards, they are large).
- `mc.py` needs ABSOLUTE source paths (a relative path in a subfolder makes cl write nowhere and print D4027).
- `q.py` is not wrapped: `timeout 900 python3 $S/tools/q.py ...`. Its object stays in `$S/wine/q/<addr>/<addr>.obj` (pk.py / fw.py input).
- vland with LANDVERIFY_STRICT_RELOCS=1 fails any row that references DELTA_NOTIONAL_ZERO (label debt at 0x0129BA3C); strict headers only passes.
- A stubborn x87 commutative order: read the keys with fm2.py before trying spellings. On row 1 no header spelling could move it; the
  release dead arm of a debug assert in an Ego_d-attested helper did (symbol numbers past the 16-bit key wrap).
- pk.py prints NO-OWNER for unbound symbols; look the names up in reloc_bindings.tsv before trusting it (row 1: none had a target).
