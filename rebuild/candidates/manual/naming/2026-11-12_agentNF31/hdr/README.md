# Header overlay (agentNF31 name-fix pass, 2026-11-12)

Full copies of the current repo headers with `#ifdef` additions only (no layout change). Grade with FABLE_HDR_OVERLAY=<this dir>.

- `engine/CManagedTextureData.h` -- `FABLE_RETAIL_CMANAGEDTEXTUREDATA_RELEASETEXTURE`: declares `void ReleaseTexture();` (0x00A69A80,
  Ego_r.pdb 0x00B7D190 / FableWin.pdb 0x03185260 ?ReleaseTexture@CManagedTextureData@@QAEXXZ, the landed row's own leaf). Used by
  row 0x00A69AD0 CManagedTexture::Release.
- `engine/CStartupDefinitionManager.h` -- `FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS`: the protected static
  `CCountedPointer<CStartupDefinitionManager> PClass` (0x013B8760; FableWin.pdb `dynamic initializer` / `dynamic atexit destructor
  for 'CStartupDefinitionManager::PClass'`, Anniversary Test PDB public ?PClass@CStartupDefinitionManager@@1V?$CCountedPointer@VCStartupDefinitionManager@@@@A);
  with it, the ISALLOCATED inline IsAllocated() reads PClass instead of the descriptive g_StartupDefinitionManagerInstance (the
  old spelling stays in the #else). Includes engine/CCountedPointer.h before the class when on. Used by rows 0x004085F0 and 0x0043368D.
- `engine/CThingManager.h` -- `FABLE_RETAIL_CTHINGMANAGER_GETWORLD`: inline `CWorld& GetWorld() { return World; }` (FableWin.pdb
  ?GetWorld@CThingManager@@QAEAAVCWorld@@XZ 0x01CF63E0, the non-const twin of the existing PEEKWORLD opt-in). Used by row 0x0073A680.
