# AUTH10 tools (scratch; candidates for lane_tools/ once reviewed)
- inl_force.py: gdb-side c2 hook = p25N inl.py (pending/2026-11-20_c2_symbol_order/tools_inline) + forced inline result per
  (callee, occurrence) at c2 0x1078CFDA (`set $eax`), and optional per-phase frequency scaling ("fscale", phase = IS_THING_ALIVE count; crude:
  nested sites compound). Evidence instrument only: a forced build is never a landing.
- force_host.py <src> <out.json> <force.json|x.cfg.json> [flags]: runs it (OBJ_OUT keeps the object).
- fv.sh <src> <force.json> [flags]: force + masked diff summary (env ROW, LEAF, FABLE_HDR_OVERLAY).
- gobj.py <addr> <obj> <leaf>: q.py-style grade of an existing object (V.mask). mdiff.py: reloc-masked instruction diff + count.
- calls.py <obj> <leaf> <row> <len>: call sequence ours v retail (retail names from reloc_bindings.tsv: first witness, often an alias).
- eglines.py / eglocals.py "<undecorated name>": Ego_d line sizes / S_BPREL32 locals and blocks, carved raw from Ego_d.pdb.
- sites.py <inl.json> [regex]: decision table.
