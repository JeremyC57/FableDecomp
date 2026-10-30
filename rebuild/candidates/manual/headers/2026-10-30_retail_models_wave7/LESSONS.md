# Wave 7 lessons (class-model migration, 2026-10-30)

1. **The level-script and entity-script base constructors are inline in retail; the headers now define them.**
   - Retail evidence: `call 0x00CB8110` then the two member stores in every level-script ctor and Alloc; the vtable
     0x012C3224 store, the Game store and `call 0x004ABE90` in every entity Alloc.
   - Both PDBs agree: Ego_r (release) has no out-of-line ctor; FableWin (debug) has one.
   - A declared-only base ctor made those rows keep a row-local base. 104 + ~600 rows moved once the ctors were inline.
2. **A template's master-data T is recoverable from retail, not only from the PDB.**
   - CScriptInfoManager::RegisterScripts 0x00CD52D0 pairs each script Alloc with a master-data Alloc in the CScriptInfo it
     passes to AddScript 0x00CB5C90.
   - The five data Allocs store data vtables 0x012C2718..0x012C2748, in the same order as the five bound
     CLevelScriptBase<T> vtables 0x012C2EEC..0x012C2F3C.
   - Use it with the Ego_r entity `PMasterData` types; scripts/regscripts.py and scripts/regmap.py.
   - It also exposed 4 landed rows whose T disagrees with retail (HEADERS.md).
3. **CScriptThing stand-ins: check the slot, not the name.**
   - Count the row's own virtual declarations in order; the dtor takes its own position.
   - Rename a call only when the signature matches the header at that index. IsValid at slot 75 is IsAlive; GetThing at
     slot 11 is GetPThing.
   - A stand-in that returned a local pointee type (`CThing_Retail*`) keeps its local type through a cast at the call;
     CThing has no shared header yet.
   - Pad stand-ins must sum to 0xc with the vptr: an 8-byte `_body[8]` with no virtual was a 12-byte object modelled short
     (0x00D292E0 DIFFER, offsets shifted by 4).
4. **Implicit members of a local CScriptThing are inline copies.**
   - A row-local model with no declared copy ctor / dtor / operator= gave VC7.1 implicit inline members. On the header,
     those become calls to 0x004ABE90 / 0x004AA840 / 0x008AB980.
   - Fix (9 rows, `mig_st.py --implicit --inline inline`): define them after the include as
     `inline CScriptThing::CScriptThing(const CScriptThing& o) : CBaseClass(o), PImp(o.PImp) { }`, plus
     `inline ~CScriptThing() { }` and the inline operator=.
   - Plain `inline` and `__forceinline` both matched these rows. Chunk 581's "__forceinline needed" was not needed here.
   - In-class `{ }` bodies moved out of class the same way (122 rows, plain inline; 2 needed the implicit trio too).
5. **Entity classes become abstract on the header.**
   - CEntityScriptBase.h has Main / GetParentScript pure (slots 1, 3), and the Alloc rows' local entity classes did not
     declare them. 463 of the first 642 ESB compile failures were "cannot instantiate abstract class".
   - Adding the PDB overrides as declarations (`private: virtual void Main();`, `virtual CScriptBase* GetParentScript() const;`)
     fixed 590 of 593.
   - Two more ESB failure shapes need a skip test: a local block inside a namespace (a header cannot be included there),
     and a derived class that models the base's vptr / Game / Me words itself (the header adds them a second time: same
     size, shifted offsets).
6. **GRO MovieBase residue (time-boxed, not solved).** On 0x00DF2D30, with the release body in a CCountedPointer<T>:
   - VC7.1 expands it (385 v 349) for an in-class dtor, `~() { ReleaseData(); }` with the full Ego_r member set (ReleaseData /
     Reset / ShareData / operator= / static DeleteData), and an out-of-class template dtor.
   - `#pragma inline_depth(1)` gets 347 v 349: the PImp release becomes a call, but the GROBase / CBaseObject ctor and dtor
     expansions are lost with it. Retail inlines those at the same depth 2.
   - inline_depth(0) gives 315; 2 and 3 give 385.
   - Retail inlines the identical release at depth 2 inside the expanded ScriptedThingBase dtor (0x00DF0713..0x00DF071B),
     and calls it (0x006E7AB0) inside the expanded MovieBase dtor. So neither the inline depth nor the release's size
     explains the split. The two pointer instantiations must differ in something the rows do not model.
   - Ego_r has an out-of-line `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` (0x007E2EA0) and none for ScriptedThingBase.
     Retail's expansion has no MovieBase vptr store, so the retail dtor is implicit; that conflicts with Ego_r's
     user-declared one.
   - /EHsc was not tried for MovieBase: these rows have no EH frame.
7. **Script hygiene.**
   - `find_block` must also find a class that starts mid-line, after `;` / `}` (the compact one-line models). When it removes
     one, it must keep the newline, or a following `#ifndef` joins the line (3 SRC_FAIL). The include it inserts there
     needs its own line.
   - Revert per family edit from a pre-image (scripts/fam.py), never from HEAD: the ST and ESB passes touch the same files.

8. **Per-row mcheck status must come from the families the row keeps.** A row edited by two families (ST then ESB)
   whose ESB edit was reverted still had the ESB DIFFER as its "latest" mcheck line, masking the kept ST SAME.
   `scripts/mstat_fam.py` filters the mcheck logs by the row's final families (ROWS.tsv col 3) before taking the latest.
