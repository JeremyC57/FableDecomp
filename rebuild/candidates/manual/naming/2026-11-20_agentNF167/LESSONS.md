# NF167 lessons

- probe.py skips rows already present in its output TSV. When probing a second entry whose REFS overlap an earlier entry, use a fresh
  output file. I missed 0x00774D20 for q6155 until I re-probed.
- Shared headers that derive from CTCBase with virtuals (CTCBob.h, CTCEnemy.h, ...) need `#define FABLE_RETAIL_CTCBASE` before the TU's
  FIRST CTCBase.h include. Otherwise the result can still RELOCATION_MATCH, but on the wrong (flat) CTCBase model, which I rejected for 0x008A7F60.
- Before adding a header opt-in, grep the header (and its existing opt-ins) for the member. A redundant declaration gives C2535.
- A row whose test mocks stand-in members cannot be respelt alone: vland links the row object, and the test LINK_FAILs. Check the test
  first and budget for the test rewrite (q11768).
- A local template specialisation (CVectorMap<...>) conflicts with the shared header's own one. Drop it and recheck the member offsets:
  the pads shift (0x0089A070: +0x99 vs +0x91).
- Python string replaces on row text: anchor on leading indentation. Comment lines often carry the same text.
- Ego_d.pdb PUB32 (egod_carved_names.tsv) settles the access letters of static data members that Ego_r / FableWin do not mangle.
