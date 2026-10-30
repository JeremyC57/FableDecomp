# Wave 2 lessons (class-model migration, 2026-10-30)

1. **The CBaseClass ctor is never inlined outside its own TU.**
   - Every landed default-construction site calls 0x0099A2D0; 1,225 callers bind it.
   - The landed rows store the CBaseClass vtable 0x01231710 as an immediate 197 times in total.
     - 193 of those stores are in rows of the inline-ctor clusters (e87c283e, 15528643, 187d7de8, 404ddfd3, cd08cff0). Each
       is followed at once by the derived vtable store, CScriptThing 0x01238C8C. That pair is the implicit CBaseClass copy
       ctor inside an inline derived copy (a by-value CScriptThing argument), not a default ctor.
     - The other 4 are in CBaseClass.cpp's own TU, 0x0099A430-0x0099A4D0 (CBaseObject ctor and dtors, CInitBaseClass), where
       VC7.1 /Ob2 auto-inlines the out-of-line bodies.
   - Rule: a row-local `CBaseClass() { }` used as `: CBaseClass()` in a copy ctor becomes the shared engine/CBaseClass.h with
     `: CBaseClass(other)`. Proven SAME on 6 rows (00d6b6a0, 00d68e20, 00eb3e40, 00eb2070, 00eb6200, 00eb1ec0); 169 more
     rows are script-ready (`scripts/mig_bc_copy.py`).
   - A row whose retail bytes have neither the call nor the store doesn't construct a CBaseClass at all. That holds for all
     160 of the 238 exact-shape cd08cff0 rows that showed neither.
2. **Same-TU auto-inlining explains "out of line, yet inlined once."**
   - ~CParentDefClassBase is out of line at 0x00430300, which the derived deleting dtors call. The class's own deleting dtor
     0x004303F0 still carries the body inline.
   - The CBaseClass case in lesson 1 is the same.
   - Declare such a member out of line. Don't make it inline in the header to explain the one site in its defining TU.
3. **Def-chain copy members look implicit (open).**
   - PDCB::operator= 0x00431ED0 is called out of line from 67 landed rows: 64 are /Os TUs and 3 are /O2 rows with many
     members.
   - 5 /O2 rows expand it inline with its CVectorMap assignment: 0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980.
   - CDefClassBase::operator= 0x00431F10 shows the same split (row 0x00432E60 expands it).
   - This is the CScriptThing copy question again (wave 1 open item 1). The header keeps the out-of-line declaration, which
     every migrated row needs.
4. **CDefPointer<T> is one template.**
   - The landed GetSubDef rows needed two local release bodies. In fact the manager temporary's `mov edi,[eax+4]; dec edi;
     mov [eax],edi` and the typed pointer's `dec [ecx+4]; jne` both come from one inline Release that holds the pointee in a
     local: `CDefPointeeBase* object = Object; if (object->Release() == 0) object->Delete();`.
   - Size TUs need `__forceinline`. Retail never calls a CDefPointer member out of line.
5. **Abstract derived defs.** On the shared def chain, a row-local derived def becomes abstract (slots 18-20 are pure). That
   breaks rows that copy it into a local, for example `std::vector<T>::_M_fill_insert`'s `__x_copy`. Declaring its PDB
   overrides (Transfer / Copy / GetSizeofClass, from `scripts/add_def_overrides.py`) is byte-neutral: SAME on 12 rows.
6. **Interface-only CGameScriptInterface rows.**
   - Virtual calls carry no relocation, so a slot-index check between the local block and the header is enough
     (`scripts/mig_cgsi.py` checks names per slot index). 96/100 SAME.
   - 2 rows changed only compiler-internal `$L` jump-table labels, with the same bytes; vland grades them.
   - 2 rows fail because their local `struct std::vector` clashes with the header's `class` forward declaration.
7. **Lane gotcha:** `source $S/wine/env.sh` resets FABLE_ROOT to /home/user/FableDecomp. Always export FABLE_ROOT after
   sourcing it. One mcheck run here read the main checkout because of this; it only reads, and it reported NOCHANGE.
- Waiter loops built on `pgrep -f <pattern>` match their own command line and never end; wait on the job's own background notification instead.
