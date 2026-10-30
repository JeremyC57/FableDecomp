# Wave 8 lessons (class-model migration, 2026-10-30)

1. **A vtable that only one function stores names its template argument.**
   - Each retail `CActiveEntityScript<T>::SuspendableProcess` is slot 4 of a 7-slot vtable that has no RTTI.
   - Only the inlined ctor inside `CEntityScriptBinding<T>::CreateActiveEntityScript` stores that vtable, and the binding's
     name gives T.
   - All 120 T came out this way and agree with both PDBs (scripts/aes_T.py). Reuse the trick for any RTTI-less template
     vtable whose ctor is inlined at a single site.
2. **A pointer-to-member member needs its inheritance model stated.**
   - `void (T::*)(CScriptThing)` is 4 bytes only when VC7.1 knows T uses single inheritance.
   - A forward-declared T gives the 16-byte unknown-inheritance form and shifts PParentScript.
   - Rows declare `namespace NScript { class __single_inheritance X; }` before the header's template is instantiated.
3. **`--implicit` and `__forceinline` explained most CScriptThing DIFFERs.**
   - A row-local model with in-class or implicit members gives inline copies. Moving out of class must keep that:
     - define the implicit copy ctor / dtor / operator= inline after the include;
     - keep `__forceinline` where the local model had it.
   - Without these, ST2's first pass DIFFERed on the rows whose leaf copies a CScriptThing inline. With them, the residue
     is the std vector / list arity clashes and the argument-type mismatches (REVERTED.tsv).
4. **Slot counting differs with a CBaseClass base.**
   - With `: public CBaseClass` the dtor is inherited at slot 0, so the row's first own virtual is slot 1.
   - Without a base, the dtor takes its declared position.
   - The off-by-one mapped MsgIsKilledBy onto MsgIsTriggeredBy in the first ST2 dry run. mcheck caught it (DIFFER); the
     rule now lives in mig_st2.py.
5. **Bindings are keyed by symbol, not by row.**
   - reloc_bindings.tsv is one global symbol -> address map; column 3 only records where the binding came from.
   - A rename adds a line only when the new symbol is unbound (bind_new.py). Replacing lines in place (bind_ren.py) is
     safe only for namespace-unique symbols (the 68 `EntityScriptBinding_<addr>` lines).
6. **Header overloads can silently change the callee.**
   - Example: a methods-only local CPlayerManager that declared only `const CPlayer& GetMainPlayer() const`.
   - On the header, its non-const call sites pick the QAE overload 0x00449970 instead of QBE 0x00449960.
   - The bytes stay the same, but the relocation names a different function (19 rows). mcheck reports it as
     RELOC_RENAMED.
   - **Always read RELOC_RENAMED details: only `$L` renumbering and leafsym renames are safe.** The same happened once
     in MBX (a B80 site turned into ~CBaseObject).
7. **VC7.1 makes per-site inline decisions that a uniform model reproduces.**
   - Retail destroys a MovieBase fully expanded, half expanded, or by a call, often mixed in one function.
   - With the implicit dtor and a plain-inline (not forced) release, VC7.1 itself produces the same mix in 15 of 24 rows.
     Those rows had scripted the mix with `#pragma inline_depth`.
   - Before adding per-site scaffolding, try the true model with no forcing.
   - Single-site rows that retail does not expand remain unexplained (HEADERS.md).
8. **Invented setters are operators.**
   - CTimer's `SetTime` / `Set` are `operator=(long)`: Ego_r has ??4 and neither PDB has a SetTime.
   - Call sites `x.SetTime(v);` become `x = v;` and stay byte-identical (TIMER, 185 rows: 180 SAME, 5 RELOC_RENAMED by
     `$L` labels or leafsym).
   - An out-of-line member defined later in the row (`CTimer::~CTimer() { ...(Index); }`) also needs the word respelled.
     That was the one TIMER compile failure, fixed by hand.
9. **The shared CScriptThing header still needs the includer's CCountedPointer.**
   - Flattened rows get a minimal global `template <class T> class CCountedPointer { T* Data; CCPPointerInfo* Info; };`.
   - Local CCountedPointer definitions went from 3,987 to 4,290 rows.
   - A shared engine/CCountedPointer.h, with the real release dtor as plain inline (lesson 7), would remove them. It
     must be checked against the rows that rely on the forced or the out-of-line form.
