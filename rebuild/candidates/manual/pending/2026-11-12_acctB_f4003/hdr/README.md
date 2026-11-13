# Overlay headers of chunk 4003 (row 0x00CD52D0 CScriptInfoManager::RegisterScripts)

Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent4003/hdr`.

## engine/CScriptInfo.h (copy of rebuild/include/engine/CScriptInfo.h + one opt-in; default text unchanged)
- New opt-in `FABLE_RETAIL_CSCRIPTINFO_CTOR`:
  - the constructor `CScriptInfo(const CCharString& name, long id, bool master_script, tAllocFunc alloc_func,
    tMasterScriptDataAllocFunc master_script_data_alloc)`, inline, body `: Name(name) { ID = id; MasterScript = master_script;
    AllocFunc = alloc_func; MasterScriptDataAlloc = master_script_data_alloc; }`;
  - the two allocator members with their real types (typedefs `tAllocFunc` = `CScriptBase* (__fastcall*)(CScriptDataBase*,
    CGameScriptInterfaceBase&)`, `tMasterScriptDataAllocFunc` = `CScriptDataBase* (__fastcall*)()`); without the opt-in the
    members keep the old `__cdecl` spellings, so the landed includers are untouched. No layout change either way (sizeof 0x14).
- Evidence (two sources): FableWin.pdb `??0CScriptInfo@@QAE@ABVCCharString@@J_NP6IPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@ZP6IPAV3@XZ@Z`,
  `?GetAllocFunc@CScriptInfo@@QBEP6IPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@ZXZ` and
  `?GetMasterScriptDataAllocFunc@CScriptInfo@@QBEP6IPAVCScriptDataBase@@XZXZ`; retail 0x00CD52D0 expands the constructor 161
  times with the Name copy first (0x00CD531E) and the stores in the parameter order (0x00CD5337 ID, 0x00CD533B MasterScript,
  0x00CD5340 AllocFunc, 0x00CD5348 MasterScriptDataAlloc), and the row matches all 6602 instructions with this body; the landed
  script allocators (0x00CDD550 ...) are `static __fastcall` with the interface by reference.
- Check that the default path is unchanged: `g++ -E -P -D_MSC_VER=1310` of the landed includers 00/cb/stdvector_vector_00cb6e20.cpp
  and 00/cb/std_pairCCharStringCScriptInfointCKeyPairCompareLess_00cb7310.cpp gives identical text with and without the overlay.
- The typedef names and the parameter names are descriptive (no PDB source for them).

## engine/CScriptInfoManager.h (NEW; default path, declarations only)
- Layout: Ego_r.pdb CScriptInfoManager with retail's STLport sizes: CurrentScriptID +0, Scripts +4 (CVectorMap<CCharString,
  CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >: vector +4..+0xF, Compare +0x10, Dirty +0x11), CodeSectionNames
  +0x14 (std::vector<CCharString>); the landed AddScript 0x00CB5C90 and Init 0x00CB5D80 use exactly these offsets.
- Members and access from the Ego_r.pdb + FableWin.pdb name sets: public ctor / dtor / Init / GetPScriptInfo / PeekPScriptInfo;
  protected AddScript, CheckSectionIsNotAlreadyRegistered, RegisterScripts, RegisterDefs.
- Used by: 0x00CD52D0. Debt: the landed rows 0x00CB5AD0, 0x00CB5C70, 0x00CB5C90, 0x00CB5D80 still carry local models.
