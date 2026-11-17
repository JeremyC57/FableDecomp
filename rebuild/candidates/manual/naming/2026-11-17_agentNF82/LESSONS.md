# NF82 lessons (round 7)

1. **Probe the old row first, then grade the new one, then pair them.** oldprobe.sh and g.sh write the same pS object. Probing
   after the new grade makes s2.py compare old with old: it finds no renames and gives no warning. This cost two regrades.
2. **Retail has no ICF, so two addresses with byte-identical bodies are two element types.** When old rows give the same
   `pair<CCharString, unsigned long>` to two families (0x009B4940 and 0x00A403F0), walk the callers up to a named function
   (callers.py) and check its TU. The PDB CVectorMap member lists (struct_layouts_egor.tsv; Ego_d per-object carved names) list
   every CCharString-keyed map. RTTI strings in Fable.exe (`.?AV..@@`) settle names that are in no PDB.
3. **FableWin.pdb and Ego_d.pdb are Dinkumware builds** (`_Vector_iterator`, `std::_Push_heap`). Use them for class, member and
   method names and for TU membership (Ego_d object paths), never for STL template spellings. Retail is STLport 4.6.
4. **Copy the include and opt-in set from a row that already compiles against the same header** before the first grade. The
   CQuestManager family needed CScriptInfo.h and CScriptInfoManager.h ahead of it, and finding that one error at a time cost three
   SRC_FAILs. Run `cpp` with the overlay as a free syntax pre-check, but it does not catch C++ parse errors.
5. **A map at a non-zero offset can change load and store folding.** Insert + Update matched the +0 map setter but not the +0x10
   one. Before reaching for a may-alias trick, try the other PDB member spelling (Insert + Sort), and keep both rows on one form.
6. **Check the base header before naming a new opt-in.** One of mine reused an existing NF68 macro in another scope. It was
   token-safe, but it broke the "new, member-specific name" rule. Diff the macro counts in orig/ against hdr/ for each overlaid
   header to catch this.
7. **Generated rows copy the old first-line comment**, including its wrong types (`long`, `_global::_Push_heap`). Regenerate the
   first line from the real element and comparator, and sweep rows/ before the final vland so it costs no extra grade.
