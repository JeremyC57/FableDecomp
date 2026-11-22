# NF252 lessons
- Check the retail exe before calling an anonymous-namespace hash a rebuild artefact: Fable.exe's RTTI type descriptors carry retail's own
  `?A0x<hash>` (`.?AVCCreatureAction_ScreamerIntoAggressive@?A0x6cb5dcbb@@`), which differs from both PDBs' (their builds). The catalog's
  hash came from the RTTI, so it was right; the earlier plan's "correct it to the PDB's" was not.
- Read the header before planning a namespace opt-in: Codex pass 27 had already wrapped the class in `namespace { }` for the landed
  0x0096DF50, so the unit needed only an out-of-line-ctor opt-in.
- A TU-mate that already emits the class's vtable from the shared headers is the template for the whole unit: copy its #define / #include
  block into every row (harness, ctor, Clone, the ported OnReplace) and every vtable COMDAT comes out slot-for-slot equal (vtcheck diff 0),
  and every pick-any COMDAT is byte- and relocation-identical to the TU-mate's (an object-vs-object compare, hash-normalised, settles all
  the NO-OWNER copies at once).
- pk.py / owners.tsv need the anonymous-namespace hash stripped on both sides (each row emits its own `?A0x<tu>`), and the vtable data
  symbol needs an owner line too, or a ctor copy shows NEQ:relocs only because its vptr store names an unbound per-TU vtable.
- An out-of-line copy retail kept of an in-class inline ctor (no callers: its callers expand it) needs a declaration-only opt-in so the row can define it
  out of line (not measured: whether /Ob0 or inline_depth(0) would keep CSoundPair's inline ctor expanded; I did not try them).
- A 401-byte port off 13 local models graded RELOCATION_MATCH on the first form once the shared-header spelling of each callee was taken
  from the TU-mate (IsModeActive for the HasMode stand-in, the AAV base ctor) and CBrainOutput came from campfire_types.h with
  FABLE_RETAIL_CBRAINOUTPUT_ONLY (its out-of-line ctor and implicit inline dtor are what retail has).
