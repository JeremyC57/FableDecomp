# Landing nf227 (Account A, 2026-11-22): the CManager / CObservable vtable-slot self rows, the six STL map / tree rows, ChangeTextureColourDepth (20 rows)

Agent NF227. Hand-back: `pending/2026-11-22_nf227_handback/`. Entries q12573-q12585. No header change.

- **Recheck** (`logs/recheck.log`): 20 rows, 4 MATCH + 16 RELOCATION_MATCH, all PASS, no RELOC_CONFLICT. Link: 0 duplicates.
- **The 12 vtable-slot self rows:** each now emits its vtable slot's PDB spelling (all three PDBs), and every address has one spelling.
  No code references them; the only binders are the slots of CObservable 0x01230044 / CManager 0x01230134.
  - Onto engine/NUISystem_CObservable.h: 0x0055CA40, 0x0055C9C0, 0x0055CA90, 0x0055CA00, 0x0055CAE0, and 0x0055C950 (descriptive
    RemoveAllObservers -> PDB ClearObservers).
  - Onto engine/NUISystem_CManager.h: 0x0042D592; 0x0041E3B2 GetLayer; 0x0041DED2 ClearBastardChild; the placeholders 0x0042D4F0 /
    0x0042D5AA, which become GetMouseDisabled / GetRightStickForce. Files and catalog names move.
  - GetLayer and ClearBastardChild put their size / frame pragmas after the include (nf189). Above it, the header's
    `_Rb_tree_base_iterator::operator!=` copy differs from 0x0041C830.
  - 0x0041D98A GetIconsDef: in-place `virtual` on its existing local model (accepted, as 0x0041E1CD; only the leaf changes).
- **0x0055C950's row-tail `#pragma optimize("s", on)`:** accepted as scaffolding under the size-pragma rule. The owner 0x0042A1E3 sets
  `optimize("s",on)` itself (condition 2), and the pragma sits after the function, so only the `_List_base<CObserver*>::clear` copy
  changes. It now equals 0x0042A1E3.
- **0x0042BE50** (CAction ctor) is ported onto engine/NUISystem_CAction.h, so 0x0042AB01 binds the STLport `_List_base<CActionParam*>`
  ctor (4 spellings -> 3). Its unreferenced `list<CActionParam*>` ctor copy is a folded alias (landed, split queued).
- **The six STL rows** (~_Rb_tree 0x0042CE63 / 0x0042B7D7 / 0x0042B796, map() 0x0042BF67 / 0x0042BF85 / 0x0042D28B) become explicit
  instantiations of the header members (the 0x008353B0 pattern). Every owned copy is EQ; each address goes 2 -> 1. Catalog names are now
  the STLport demangled names (files not renamed).
- **0x00495610:** leaf `?ChangeTextureColourDepth@CFrontendGameComponent@@UAEXJ@Z` (all three PDBs; vtable 0x01238768 slot 3), on
  engine/CFrontendGameComponent.h. InitGraphics 2 -> 1. New test: the HEAD test included the HEAD source and hung under vland.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 6 stand-in lines deleted, 15 added. That includes the nf226-held `_List_base` line and
  the vtable-slot first witnesses.
  - One add was held (`logs/reloc_bindings_held.tsv`): map<long,EEngine2DLayer>::operator[] at 0x0042D246, where 0x0041E1CD's local
    model still binds `??ACLayerTree`.
- **Queued** (tag land1122nf227): 0x0042BEC0's implicit ~CObservable (no device yet); 0x0042D246; the folded alias; 19 tests still on HEAD
  models; the ready 0x0042BE7B port; NF227's NAME_FIXES (callee self rows on stand-ins, 0x004299A8, 0x013B871C).
