# NF21 lessons (one line each)
- reloc_bindings.tsv lists only the first caller: scan rows whose retail bytes reference the target (rel32 / abs dword), then compile them to read the real bound symbols.
- Shared headers that use FABLE_STATIC_ASSERT may need `#include "rebuild_abi.h"` first (CMessageEventManager.h does not include it).
- Retail-branch headers (CClass, CBaseObject, CCreatureActionBase) need FABLE_RETAIL_CCLASS + FABLE_RETAIL_BASE_OBJECT, or the size asserts fail ("negative subscript").
- A pointer parameter respelled to a const reference keeps the bytes (`push reg` either way); a value-copy stand-in (C2DCoordF) needs an explicit temporary to keep its copy.
- Forward-declared enums (`enum E;`, an MSVC extension the shared headers use) let a row take the PDB enum types without importing the full enum.
- A derived temporary with no user destructor makes VC7.1 call the base destructor directly: that is how a folded 0x0062E900 binds ~CCombatActionBase.
- Templates and inline helpers carry the const of the PDB instance (FindMostRecentMessage is QBEPBV): check the template instance in the name set, not just the filters.
- Replace a header line by its first occurrence only: retail and default branches often repeat the same declaration text.
- Compare old and new objects for every changed row (bound pairs plus defined symbols) before writing RENAMES: it catches unintended respellings.
