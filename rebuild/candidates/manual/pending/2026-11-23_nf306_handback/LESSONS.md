# NF306 lessons (2026-11-23)
- **ENTRIES column 12 (spellings) is truncated for big entries.** It cuts mid-symbol, e.g. entries 13 and 16. A stay-off set built from it
  missed NF304 / NF305 witnesses. Build witness sets from reloc_bindings.tsv: every column-3 row whose target is one of their entries. Add their
  rows/ and rows.txt (tools/mkstayoff.py).
- **The table hides binders.** bk.py over the exe call sites found rows the table never lists:
  - 0x00602F30 also bound GetPMiniMapDisplay;
  - six rows name CSymbolMap::Lookup;
  - four rows name GetDummyObjectOrigin.

  Run it before any delete, and read the declaration of every caller that names the same identifier (0x004B5500, 0x0058578B).
- **A caller that names the stand-in identifier can still be fine.** GetQuestName is a real PDB member at another address (0x00702310); only
  the binder that attaches it to 0x00702470 is wrong. Read where each caller's declaration points.
- **Tiny targets with an oddly named self row.** Check for a fold before trusting or rejecting:
  - byte-twins elsewhere in the catalog oracle mean the linker did not fold that shape;
  - one caller context and one PDB member of the right arity mean one identity.

  0x00B8D0B0 (5 bytes), 0x00BACDD0 (12) and 0x00437CE0 (10) all passed.
- **Respelling onto a template.** Template-instance PDB spellings split two ways by how the binder holds the stand-in:
  - declared only: a `.drectve` change (T3 by ruling);
  - defined in its own TU: it would become a strong copy under the owner's name (T3).

  tools/tmplcls.py sorts them without a compile. Probes confirm the `.drectve` line for free function templates and for other classes'
  member templates, not only CDefClassBase::GetSubDef.
- **Pointer -> reference, long -> unsigned long, long -> bool (constant arguments) and plain -> const members** were byte-neutral in all 26
  rows. So was a private / protected access change when the caller is a member or a derived class.
- **pre.sh cds into work/**: pass absolute paths. **lock.sh needs S exported**: source env.sh in the same call.
