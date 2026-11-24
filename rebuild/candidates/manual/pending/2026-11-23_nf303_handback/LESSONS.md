# NF303 lessons (2026-11-23)
- **Never truncate a census read with `head`.** The first read of entry 94's binders (vdecl.py | head -12) hid three rows that declared
  the ctor public, and the table delete looked safe. Write census output to a file and read all of it (work/vd94.txt).
- **Virtual targets have binders that xsites cannot see.** 0x00859A20 has 170 absolute (vtable) references in the exe, and 11 other landed rows
  declare the virtual public on their local base model. Any of them that emits a vtable binds the UAE spelling. Before deleting a table line for a
  virtual function, run absrefs.py on the target and tools/srcwide.py for the declaration source-wide.
- **filt.py "STALE" is not proof.** 4 of 5 STALE lines (entries 81, 140, 173, 543) were still bound in the witness's HEAD object. Their
  identifier did not appear in the code because it was a base dtor / ctor reached through another class name. Read the object.
- **Declared-only member templates add a `.drectve` line.** VC7.1 emits `/alternatename:<instance>=<non-template name>` for every member template
  instance it references but does not define, so a non-template -> template respell is never objsec ALL_EQ on `.drectve`. Landed rows already
  carry such lines (0x00894490). Ruling asked (Q2).
- **A same-TU stand-in body under a made-up name becomes a pick-any copy when respelled.** Grade the copy against the owner (tools/grc.sh) before
  keeping the respell. The `__sort_heap` -> `sort_heap` bodies were 166 v 179 bytes, so they are T3.
- **A base respell can be hidden by the derived class.** Respelling a base `Init()` to `Initialise()` inside a class that declares its own
  Initialise needs the qualified call `CInitBaseClass::Initialise()`. The bytes are the same.
- **Owner self rows can block a binder access fix.** Making CSmallVector::Clear protected in the binders would have left IAE unresolved where
  HEAD resolved QAE, because the owner defined it public. Fix the owner self in the same unit (wrap + full Leaf), or leave the binders.
- **Stdcall -> fastcall with only float arguments is byte-neutral** (nothing goes in registers, the callee pops the same count): 11
  AngleDifference binders were ALL_EQ, as NF285's GFAYToVY.
