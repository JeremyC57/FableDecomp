
## Shared headers: correct them while you work (owner 2026-11-02)
When a row needs a class whose `rebuild/include/engine/<Class>.h` is wrong for retail (vptr flattened into a data member, Ego_r-only members or
offsets, a missing base, a missing member function), fix the header instead of writing a local model, whenever you can:
- Copy the repo header to `$S/w22/agentNNN/hdr/engine/<Class>.h` (same relative path; `hdr/<file>.h` for a non-engine header) and edit the
  copy. Grade with `export FABLE_HDR_OVERLAY=$S/w22/agentNNN/hdr` set before g.sh / gall.sh / tall.py / the vland check: the verifier then
  searches hdr/ before rebuild/include. A repo header that includes the one you corrected still finds the repo copy (MSVC looks in the including
  file's own directory first), so copy that includer into hdr/ too. Confirm once that your copy is the one used
  with a deliberate byte change that must turn a row DIFFER (never `#error`: VC7.1 under Wine spins forever on it).
- Prefer an opt-in retail branch: `#ifdef FABLE_RETAIL_<CLASS>` (retail layout) `#else` (the existing text, unchanged) `#endif`, with a
  `// Retail opt-in (chunk NNN): ...` evidence comment (RTTI bases, vtable address and slot count, member offsets from the PDB checked against
  retail bytes -- two sources). Your row `#define`s the macro before the include. Existing includers keep the default path, so their bytes cannot
  change. Precedents: FABLE_RETAIL_CTCBASE (engine/CTCBase.h), FABLE_RETAIL_CTCDOOR, FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM, FABLE_RETAIL_NSCRIPT.
- If you change the default path instead, list every landed includer (real `#include` lines, transitively through other headers) in
  `hdr/INCLUDERS_<Class>.txt` and recheck all of them read-only with the overlay (`vland.py --recheck-landed` on a payload for those rows, in
  parts of <= 800, LANDVERIFY_WORK under your directory and deleted after): every one MATCH / RELOCATION_MATCH + PASS, logs in tmp/. If any fails,
  use the opt-in branch.
- Generated headers keep a `// Retail opt-in` / `// Retail layout` line or a FABLE_RETAIL_ macro (gen_class_headers.py's RETAIL_MARK), or the
  generator overwrites the edit.
- Another chunk agent may correct the same header in parallel: keep each edit minimal and confined to what your rows need. Never edit
  engine/CBankFile.h (HOLD) or rebuild_abi.h.
- Hand back hdr/ with `hdr/README.md`: per header what changed, the evidence, opt-in or default path, includers rechecked. The coordinator
  copies hdr/ into the repo at landing and rechecks every includer again. A fix too large for this chunk goes under "Header debt" in NOTES.md
  with the exact change.
