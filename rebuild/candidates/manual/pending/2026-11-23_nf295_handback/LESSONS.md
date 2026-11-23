# NF295 lessons (2026-11-23)

- **A flat generated state-group header is not always derived from the flat CAIStateGroupBase.** `CAIStateGroup_StealthSeekEnemy.h` and
  the default branch of `CAIStateGroup_TavernKeeper.h` are standalone byte views with `_pad_0x04` where PBrain would be. Open the
  derived header before you plan a base-class opt-in. NF294's "the Adapter derives the flat base" was a reading slip.
- **A row that includes a header under a rename macro cannot use that header's member opt-ins.** 0x0093CB20 does
  `#define CAIStateGroupBase CAIStateGroupBaseLayout`, so any member there mangles on the Layout name. Declare the member on the row's
  real-name wrapper class instead.
- **Respelling only the binders can leave unresolved a reference that HEAD resolved.** This happens when the owner self row defines
  the old stand-in spelling (0x0073A9B0 defined the global `??0CAnimEntryLite@@`). Read each owner object's defined symbols before you
  decide whether a binder-only fix is safe.
- **A port that swaps a template argument renames every COMDAT the template emits.** That includes made-up inline members of a local
  model (a `Clear`). Pair the COMDAT sets explicitly, and run a pick-any check against retail for each one that now carries an owned
  PDB name (tools/pickany.py).
- **The sibling-shape re-port of the input-process rows makes VC7.1 emit unreferenced `scoped_ptr::operator->` COMDATs.** Section counts
  then differ even when the function is identical, so compare sections by symbol, not by index.
- **cls.py / tri4.py (like the old flags.sh) read `ps[i:i+700]` after the Address line**, which can cross into the next catalog block.
  Both are now bounded to the row's own `[pscustomobject]` block.
- **Stale witnesses show up only in objects.** objbind.py over HEAD objects found 4 of them in 60 entries (0x00E8B9C0 x3, 0x00CF8970).
  The token scan (tri4) found none of these. Compiling every free witness of a 60-entry slice took about 3 minutes in two background jobs.
