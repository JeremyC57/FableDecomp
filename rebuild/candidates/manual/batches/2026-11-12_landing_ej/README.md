# 2026-11-12 landing ej -- 0x009ADC90 CDefinitionManager::InstantiateDef (workflow wf2's tenth row, after repair)

Source: `pending/2026-11-12_linux_handWF2/` (row notes under "0x009ADC90" plus `REPAIR2_009adc90.md`). RELOCATION_MATCH, test PASS under
VC7.1, LANDVERIFY_STRICT_HEADERS=1.
**Repair (one Fable agent):** the nested record constructor now has the FableWin.pdb signature
`CInstantiatedDefInfo(CDefString, CDefString, unsigned long, const CDefPointer<CDefClassBase>&)`; the row holds the new def in a
`CDefPointer<CDefClassBase>` local whose destructor is retail's final release; the test body equals the row body and reaches the delete
path through a harness operator new that returns null (retail's own null-record branch at 0x009ADD18).
**Verification (two independent Opus verifiers on the repaired row):** evidence lens -- one finding, fixed at landing: AllocFunc is
`__fastcall` (FableWin `P6I`; all Ego_r `?Alloc@C*Def@@SI...`), the row now uses the shared `CDefClassInit::TAllocFunc` (regraded).
Adversarial lens on the adopting pointer constructor -- claim PROVEN on a second retail site (InstantiateSubDef 0x009ADAE0) and on the
def birth count (CResource::CResource 0x009FBEC0 stores 1); the opt-in is renamed FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT and its
comment now cites both sites, says the FableWin debug body length (356 bytes) proves nothing, and warns that a counted handle on an
existing pointee is the copy constructor (four landed rows have local models that count in a raw-pointer constructor: 0x009AE150,
0x00970610, 0x0069ADC0, 0x00774100 -- they must use the copy constructor when they move onto the shared header).
**Headers** (4, additions only; new opt-ins unused elsewhere, defaults unchanged): CDefinitionManager.h (INSTANTIATEDEF), CDefPointer.h
(POINTER_CTOR_ADOPT), CDefString.h (CTOR_SETSTRING), CSymbolMap.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
