# NF302 lessons (2026-11-23)

- **Read what the OWNER object defines before moving a binder.** A binder fix can turn a resolved stand-in reference into an
  unresolved PDB one when the owner row itself defines the stand-in:
  - 0x007C21A0 defines the global `?Update@CTCShotDefinitionBase@@`;
  - 0x005BD273 defined the non-const form;
  - 0x005BCAE3 defines a global placeholder.
  - Check: objdump the owner's HEAD object first. When the owner's fix is only `const` or access (0x005BD273), fix the owner in the
    same batch: it also resolves the callers that already bind the PDB spelling.
- **The witness table under-reports binders.** xsites found 0x00581FA4 binding the same free stand-in as 0x00581CFD with no table line.
  Census by call site, not by table line.
- **Pin unpinned asserts BY LINE INDEX.** CResource.h has two textually identical `FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);`
  lines (one per branch), so a text replace hits the wrong one or fails.
- **Plant the equiv_check control at line 1, outside every guard.** In CPixelFormat.h the first include guard sits inside
  `#ifdef FABLE_RETAIL_CPIXELFORMAT`. A token planted after it reaches only the retail-branch includers.
- **A flat-branch static opt-in plus `#include` lets a free `__fastcall` stand-in become the PDB static member.** The binder needs no
  local model of the class, the bytes are equal, and the include goes after the row's own local definitions of the return types or
  enums.
- **A row-local `FABLE_FORCEINLINE` template member (`GetTC<T>`) can still leave a pick-any COMDAT** named after its type argument.
  A namespace move of `T` renames it, so read the objsec rename map for second renames.
- **Stale witnesses again:** 3 of 4 stale-shaped candidates were stale (0x00EB15C0, 0x00EB23E0, 0x0056ECCB).
  - Signs: the source already models the class inside its PDB namespace, or the stand-in names a neighbouring member.
  - Exception: 0x00788198 was live; tri4's IDMISSING misparses a `_Name_addr@8` stdcall token.
