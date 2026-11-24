# Landing access 3 (Account A, 2026-11-23): 140 rows onto their PDB symbol by access / const / virtual / return edits (route 3a)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/access_fix.py --wide --virtual --ret`, landed with `land_scripted.py`. This is route
3a of `docs/journal/2026-11/2026-11-23_naming_finish_plan.md`.

- **Pool.** Measured selves exactly one allowed edit away from a unique PDB symbol of the same `?name@scope@@`: 924 candidates. Excluded
  at selection:
  - 2,410 addresses reserved in T3_PORT_QUEUE.tsv (earlier rejects included);
  - 185 rows with a binder on the old spelling;
  - 575 virtual targets the RTTI placement does not confirm;
  - 14 non-virtual targets missing from one PDB name set.
- **Edits.** These are made only in the row's own local class model, and never to the class or member name:
  - the access label;
  - `const`;
  - `virtual` (the target is RTTI-confirmed). When the model has no other virtual, the 4-byte vptr comes out of a leading `vtbl` member
    or a leading pad, and a model with a base clause is skipped;
  - a simple return type, taken from the PDB code.
- **Accepted, by access-letter change:** 108 non-virtual public -> virtual public, 15 -> virtual protected, 5 -> virtual private, 10
  public kept (const / return only), 1 -> protected, 1 -> private.
- **Proof per row.** objsec ALL_EQ under the one rename self -> PDB symbol (so no vftable or other section appears), the graded symbol is the
  PDB symbol, and the grade is MATCH / RELOCATION_MATCH PASS. All of this is recomputed on the landed tree by `land_scripted.py`.
- **Run `vr3`.**
  - Counts: 181 generated, 140 accepted (77%), 41 rejected (queued as `access3`), 743 skipped. The kill line is 60%.
  - The first pilot reached only 50%, because the pool retried earlier rejects. Excluding T3-reserved addresses raised it to 70%.
  - An early version of `access_fix.py` wrote a shrunk `unsigned char` pad back as `char`. `land_scripted.py --fix-unsigned-pad`
    restored the element type before the recheck, and the script is fixed.

## Recheck
`logs/recheck.log` (landed tree, 140 rows): 140 MATCH / RELOCATION_MATCH PASS, graded symbol = PDB symbol, none held.
