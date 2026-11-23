# NF313 lessons (2026-11-23)

- **The owner's self spelling decides a binder fix.** Read the owner's Leaf / HEAD object first: when it defines the PDB form, a
  pointer -> reference binder fix lands alone (link: unresolved -> resolved); when it defines the binder's own stand-in, the binder is
  resolved at HEAD and only an owner unit can move it.
- **One table witness hides many binders.** reloc_bindings keeps one witness per spelling; the exe scan + a declaration grep of every
  landed caller (tools/delchk.py) found 3 / 5 / 8 / 58 binders behind single witnesses. Fix all of them or rewitness, never delete on the
  witness alone.
- **A declarer is not a binder.** A local model can declare the stand-in without calling it (0x007CC060: the inline that would call
  SetPItem is never emitted), so a source grep overstates binders; compile before naming a rewitness.
- **`typedef long E...` in a binder is a self issue.** The same typedef spells the row's own override, so the enum retype is not
  binder-only (0x0090F0D0, 0x0095A890, 0x007BB150).
- **census311.py's MADEUP tag misfires on enum names** (`W4E...` tokens are never in the PDB token set): re-read MADEUP lines whose only
  unknown token is an `E...` type (0x006EBEE0 is pointer v reference only).
- **The NF309 / NF312 ENTRIES witness lists are cut with `...`:** offl.py v2 adds every reloc_bindings witness of their targets.
- Shell slips: prep.sh needs absolute paths (relative ones fail inside work/); never pipe a payload builder into `head` (BrokenPipe
  before json.dump leaves the old payload).
