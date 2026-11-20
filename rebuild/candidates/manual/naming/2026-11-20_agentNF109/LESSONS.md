# NF109 lessons (round 10, GameCODA follow-ups)

- **Do not trust reloc_bindings witnesses for "already split".** The table said 0x00C75000 was bound three ways; compiling the
  binders from HEAD (work/tools/oldsyms.py) showed all four rows on one spelling. Compile the binders before deciding whether a
  rename may be partial: here it forced the whole 11-row family.
- **A member swap drags in the destructor.** Retyping a stand-in member to the shared-header class is free in rows that only call a
  method, but the owner's ctor / dtor rows then bind the member class's ctor / dtor, whose own vtables and element ctor / dtor follow.
  Walk ctor -> dtor -> ??_G -> element class -> vtable with refs.py before writing the first row.
- **In-place swap transformer.** Removing the local stand-in classes, adding `#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE` + the
  opt-in + `#include "engine/<Class>.h"`, and retyping one member graded at the first compiling form on four 160-line rows. Check
  member-name clashes first (the local models already had `m_diffuser` for another stand-in: one failed form on three rows).
- **SDK release vs retail build skew shows up in ctor / dtor inlining.** The CODAApi.lib CFloatDiffuser ctor / dtor expand the section
  ctor / dtor; retail calls them out of line through the eh vector iterators, the CODAApiDbg.lib shape. Use the Dbg body for the
  statement shape and declare the element ctor out of line (header opt-in) to get the five-argument iterator.
- **Expanding a TU-mate.** A same-TU `inline` redefinition of an out-of-line member matches but leaves a COMDAT of that member in the
  object next to its strong row; writing the body out (with a comment naming the TU-mate) gives the same bytes without it.
- **Funclet regrades without touching the funclet rows.** Put the parent's new text in the funclet's payload entry (funclet_offset from
  the funclet's `+0x.. of the parent's block` comment, leaf `Unwind@<addr>`, the catalog flags); vland grades it like the NF38 recipe.
- **Folded vtables need a ruling, not a pick.** A 7-byte vptr-only dtor matches every SDK class with that shape (30 hits for
  0x00C53AD0); the vtable address carries all their names.
