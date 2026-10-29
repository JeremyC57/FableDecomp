# Lessons, chunk 565

1. A TC interface type can be read from vtable slot 21 even when the getter is not a plain `mov eax,N ; ret`.
   NThingShot::CTCShotDefinitionBase's slot 21 is 0x004D5472, `push 77h ; pop eax ; ret` (size-optimised). Searching
   only for `mov eax,77h` turned up a dead getter (0x00661920) with no references. Read slot 21 of every candidate
   vtable instead of searching for the immediate.
2. A direct call where the manifest says the callee is virtual is a wrong label. 0x007BED50 was labelled
   CTCPhysicsNavigator::EnableNavigator (`UAEX_N` in the PDB), but CastFireball calls it directly. The neighbouring
   setter/getter run (+0x66/+0x67/+0x68), the PDB bool members (PDB less 4) and the donor order named it
   SetCanBeDeflected.
3. TC member offsets after an embedded helper that holds a vector shift by 4. CTCFireballSpell embeds
   CWillButtonHelperTimer (PDB 0x38, retail 0x34), so every later member is PDB less 4, even though the TC itself
   declares no vector before them.
4. VC7.1 homes a late lookup key in a dead argument slot on its own. CastFireball's third GetTC key lands in the
   `target` slot after CreateShot, as in retail; no device is needed.
5. The default `CDefPointer<T>()` temporary passed to CreateShot releases in the "count returned" shape
   (`mov edi,[eax+4] ; dec edi ; mov edx,edi ; cmp edx,ebx ; mov [eax],edi`), then nulls the pointer. The model that
   gives it is `long Release() const { long c = --IVRefCount; if (c == 0) Delete(); return c; }` on the pointee plus
   `~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }`.
6. A 4-way switch on a level, where each case builds a CCharString literal, needs no device. VC gives each case's
   temporary its own frame slot and merges the shared `~CCharString` tail with the result move (`mov edi,eax`) just as
   retail does.
7. Tool note: objdump splits a function with a jump table at its `$L` case labels. Any tool that pairs relocations with
   a leaf (symbol maps, prefix compares) must merge the `$L` blocks that follow the leaf, or it silently misses every
   call after the first case (tools/symmap.py, tools/exp.py).
8. Over-spanned rows: when a row ends in other small functions (FrameUpdate 0x007A8BE0 runs into 0x007A8E20/30/40),
   compare only the true extent with masked relocations (pfx2.py). If that extent is clean, park the row with
   "re-split at" instead of spending grades on bytes the source cannot produce.
