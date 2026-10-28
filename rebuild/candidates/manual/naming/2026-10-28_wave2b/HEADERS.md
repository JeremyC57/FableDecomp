# Header items (wave 2, agent B)

No file under `rebuild/include/` was edited.

## engine/CScriptThing.h cannot emit the retail destructor (0x004AA840)
- Retail: `??1CScriptThing@@UAE@XZ` (public virtual destructor; Ego_r.pdb, FableWin.pdb, DECISIONS.tsv proven).
- The generated header models the vptr as a data member and declares no destructor:
  `void*           __vftable;                               // +0x00 vptr, ...`
  so a TU that includes it cannot call `CScriptThing::~CScriptThing` at all.
- Wanted line, replacing the `__vftable` member (same +0 slot, sizeof stays 0xC):
  `    virtual ~CScriptThing();                                 // 0x004AA840`
  (and the `offsetof(CScriptThing, __vftable)` assert dropped). The header is generated, so the fix belongs in
  `tools/decomp_pipeline/gen_class_headers.py` (emit a virtual destructor for PDB classes whose vtable slot 0 is the
  scalar deleting destructor, instead of a `__vftable` member).
- Affected rows: the 158 rows of `fam_autotiny` use a local `class CScriptThing { public: virtual ~CScriptThing(); };`
  and print `WARN SHARED_HEADER_AVAILABLE CScriptThing` (fatal only under LANDVERIFY_STRICT_HEADERS=1). 2,325 landed
  sources already carry a local CScriptThing class for the same reason. The 321 rows that include
  `engine/CScriptThing.h` would all need a recheck after the header change (their models touch `__vftable`).
- Rows: `fam_autotiny.rows.txt`.

## No header is involved in the other families
CPersistContext, CCharString, CBaseIntelligentPointer and CInterfaceMap/CVectorMap have no `engine/` header.
