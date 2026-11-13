# hdr/ -- shared-header overlay of chunks 4004 + 4005 (rows 0x00F2D6E0 CScriptDef::Transfer and 0x00F2A250 CScriptDef::operator=)

Five files under `engine/`. Four are copies of the repo headers with opt-in blocks added (default text unchanged: `diff` against
`rebuild/include/engine/` shows additions only, no removed line); one is new. Both rows grade RELOCATION_MATCH on this one set.

## Chunk 4005 (row 0x00F2A250)

### engine/CScriptDef.h (copy + 1 opt-in)
- `FABLE_RETAIL_CSCRIPTDEF_IMPLICIT_ASSIGN`: drops the declaration `CScriptDef& operator=(const CScriptDef&);` so the compiler
  writes the assignment. Evidence: (1) Ego_r.pdb / FableWin.pdb `??4CScriptDef@@QAEAAV0@ABV0@@Z` is an out-of-line COMDAT reached
  only from the inline Copy 0x00F2A240, laid out right after it (the layout of an implicit function emitted for its first user);
  (2) its 11,711 bytes are exactly what VC7.1 writes for the implicit operator= of this header: all 943 members in declaration
  order, each by its type's own assignment, the parent-def chain implicit (RELOCATION_MATCH, tools/map2.py: 943 events v 943
  members). Same situation as CCutsceneDef (engine/CCutsceneDef.h declares none) and NUISystem::CUIDef.
- Default path (no macro): unchanged; the landed includers (e.g. CScriptDef::Copy 0x00F2A240) keep their text.
- Review at landing: the declaration can simply go (make the opt-in the default) once the landed includers are regraded; no landed
  row DEFINES CScriptDef::operator=.

### engine/CRegionScriptDef.h (copy + 1 opt-in)
- `FABLE_RETAIL_CREGIONSCRIPTDEF_IMPLICIT_ASSIGN`: drops `CRegionScriptDef& operator=(const CRegionScriptDef&);`. Evidence:
  (1) FableWin.pdb `??4CRegionScriptDef@@QAEAAV0@ABV0@@Z`; retail 0x00F29F70 (45 bytes) is the tail-jump target of the inline Copy
  0x00F29F60 and sits right behind it; (2) the implicit operator= compiled in row 0x00F2A250's unit equals those 45 bytes
  (tools/sib.py: `call 0x00431ED0`, then the four longs).
- Default path: unchanged.
- Review at landing: the landed 0x00F29F60 row's SPLITS note treats 0x00F29F70 as a separate function; with this opt-in it is the
  implicit one and needs no source of its own.

Rows regraded after the last header change: 0x00F2A250 (q.py, gall.sh, read-only vland) and 0x00F2D6E0 (prior/00f2d6e0.cpp compiled
under this overlay into tmp/b/t_transfer: 23755v23755 RELOCATION_MATCH by tools/bm.py, 965 relocation symbols pair one to one; not
run through q.py so that the shared q/ work directory of the coordinator's agent4004 verification was not touched).

## Chunk 4004 (row 0x00F2D6E0) -- carried over unchanged (files byte-identical to prior/hdr_f4004)

Three files under `engine/`. Two are copies of the repo headers with opt-in blocks added (default text unchanged: `diff` against
`rebuild/include/engine/` shows additions only); one is new. Rows that include `fable_persist_context.h` first get the repo
copies, so a row that wants these blocks includes `"engine/CPersistContext.h"` and `"engine/CPersistTraits.h"` first.

## engine/CPersistContext.h (copy + 2 opt-ins)
- `FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY` (needs `FABLE_RETAIL_CPERSISTCONTEXT`):
  - protected declaration `void TransferVectorSaveText(const char*, CPersistContext_TransferableVectorGenericType&);`
    (0x009BAC80; Ego_r.pdb + FableWin.pdb `?TransferVectorSaveText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorGenericType@@@Z`);
  - the shared body of the named `TransferVector<T>(const char*, std::vector<T>&)`: CheckCRC, closed switch on Mode
    (`default: __assume(0)`, the spelling of the landed 0x00F333B0 row), global serialisers `GFSerialiseVectorBinaryIn / Out`
    (declared only), `CPersistContext_TransferableVector<T>` wrapper for the two text modes. Retail instances 0x00F333B0
    (CDefString), 0x0049B8D0 (CCharString), 0x00569FE0 (float), 0x005815D6 (long). Row 0x00F2D6E0 needs it VISIBLE: its call
    sites are inline candidates for c2's budget rule (c1 size 151) even though the switch body never expands.
  - Review at landing: the landed instance rows (0x00F333B0, 0x0049B8D0, 0x00569FE0, 0x005815D6) were not regraded against
    this shared body (they do not define the macro); one body should serve them all.
- `FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY`: `template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal()
  { CPersistTraits<T> traits; return traits.DefaultVal; }` -- both PDBs name `??$GetDefaultVal@<T>@CPersistContext@@KI?A<T>XZ`;
  the body is the one whose c1 size (48; CCharString 64) reproduces retail's two inlined long defaults (0x00F30E97,
  0x00F30EF7), and whose CDefString instance compiles to retail's 9 bytes at 0x007E37C0.
- Default path (no macro): unchanged.

## engine/CPersistTraits.h (copy + 1 opt-in)
- `FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS`: `template<> class CPersistTraits<float>` as an explicit class specialisation
  (PDB evidence: its TransferBinaryOut / TransferOut take `const float&`, the generic takes T by value; user ctor and dtor named
  in both PDBs). Exclusive with `FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY` (`#error`).
  **FITTED, NOT PROVEN:** `const float DefaultVal;` -- the const is what makes GetDefaultVal<float>'s c1 size 49 (48 without),
  and 49 is what retail's split needs (float default refused where the long default is accepted). Review at landing.
- Default path: unchanged.

## engine/CPersistContext_TransferableVector.h (new)
- `template<class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType` -- RTTI
  `.?AV?$CPersistContext_TransferableVector@J@@` (vtable 0x0124F33C) + PDB. Constructor inline (stores the vector's address);
  the four overrides Size / Resize / TransferOut / TransferIn are DECLARED ONLY (retail bodies e.g. 0x00F33470.. for
  CDefString); member `TransVector` +4 is the Ego_r.pdb layout name (ghidra_out/struct_layouts_egor.tsv). Review at landing: give
  the overrides bodies when a wrapper-virtual row needs them.

Rows regraded after the last header change: 0x00F2D6E0 (the only row of this chunk).
