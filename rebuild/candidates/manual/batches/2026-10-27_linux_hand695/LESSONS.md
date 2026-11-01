# Chunk 695 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (2026-11-01, chunk 695) Byte-twin implicit destructors under one propagated label can be TWO symbols that both land: name each from its
  slot-0 ??_G caller's RTTI COL (0x005ED210 = ~CScriptModuleGoToLookInDirection via TavernGame's ??_G, 0x00854380 = ~CScriptModuleFOVZoom via
  FOVZoomStayRelative's ??_G, which destroys its own member first); only hold a twin when the callers prove the SAME symbol.
- (2026-11-01, chunk 695) `Write(s.GetAsCharArray(), s.GetLength())` with both accessors null-tested evaluates the length first (right-to-left)
  and splits the tails on it; retail's data-first threading (`mov edx,""; je`) needs `const char* data = s.GetAsCharArray(); Write(data,
  s.GetLength());` (CDataOutputStream::WriteString 0x009959F0).
- (2026-11-01, chunk 695) A virtual call made inside a destructor through a real same-file member (RemoveFromInterface 0x00A0D2D0 called from
  ~CAInputProcess) compiles to retail's `call [vtable+N]`; no `__forceinline` dtor or invented helper is needed when the PDB has the member.
- (2026-11-01, chunk 695) Base-class virtual slots in packed (1-byte aligned) TUs can be named by aligning the retail bodies with the Ego_r /
  FableWin donor order and checking each body's first `ret N` against the PDB parameter list (NInventory::CTCInventoryBase 28..85,
  0x0061B590 matched first time on that vtable).
- (2026-11-01, chunk 695) Two PDB members of different size with one retail pair of bodies: rank by size in BOTH PDBs (CSymbolMap::UpdateDirty
  0x40 > UpdateDirtyQuick 0x30 in Ego_r and FableWin) and match the retail sizes (39 sort+trim vs 33 sort-only).
- (2026-11-01, chunk 695) Empty `ret 4` stubs in the release build keep their PDB names: the Ego_r donor order of CThingManager
  Add/Remove...List equals the retail order 0x0051DF60..0x0051E000, and each stub's sole caller (ctor / dtor of CThingPhysical /
  CThingGameObject) confirms it.

Review at landing:
- 0x009E5C60 must be respelled to UpdateDirtyQuick together with this chunk's 0x009E5C30 (UpdateDirty); see NOTES.md.
- Landed rows with wrong names found while authoring (NAME_FIXES.tsv): CTCScriptedControl_OnRemove_00713740 (is OnDie, Leaf vfunc_13),
  CActiveFile_OnReadFinished_007196b0 (is ~_List_base of the action list), CRetailListCountedPointer_clear_00719280 (invented CRetailList model,
  no Leaf), CSymbolMap 0x009E5350 RemoveUnusedMemory, CArray DeallocateUnusedMemory 0x009E7C70 (mangles CArray<SSymbolPair>),
  CTCInventoryQuests_ProcessButtonBReleased_0061b5f0 (__fastcall free stand-in), CTCInventoryClothing_ProcessButtonAReleased_005b34d5
  (v00..v31 placeholder slots: the 0x0061B590 vtable model can replace them).
- reloc_bindings: 0x00A0D3D0, 0x0099EAE0, 0x00838A00, 0x006A5D50 carry many per-caller stand-in spellings; only the PDB ones should stay.
