# NF197 lessons (one line each)
- Check every queue claim against the name sets before acting: q6296's "HasInputFocus (Ego_r)" is in no PDB; the direction was reversed (respell the binders, keep the self Leaf).
- A compiler-generated destructor stores no vptr; a user-written empty body does. Emit the implicit one with the nf86 qualified-dtor helper (0x00687210 CMessageEvent: 33v27 -> RELOCATION_MATCH).
- Before trusting a table line that maps a new pick-any COMDAT to another address, read that address's body: 0x00517850's tree dtor erases through 0x004A9CB0, not set<unsigned long>'s 0x004CF810.
- Explicit instantiation of a function template is a LINK_ONCE_DISCARD COMDAT (objdump -h), so a self row can own a template instance another row also emits noinline, as long as cmpbody says equal.
- A test that `#include`s the row by repo path keeps compiling the OLD row; after a self rename, declare the symbol and let vland link the row object instead.
- The STLport `_Rb_tree` / `set` copy constructors at 0x00513700 and 0x00AD3A10 are byte-identical shapes for two 4-byte-key trees (they differ only in their _M_copy callee): judge which instance by the callee, never by the body bytes alone.
- Header centre ctor C2DBoxF(const C2DVector&, float) does not reproduce a by-value (x, y, radius) stand-in's x87 order (0x00730F90: 290v292 twice): leave such binders blocked rather than fit a new header body.
