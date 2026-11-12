# 0x009ADC90 repair 2 and its verification (landing ej)


## 0x009ADC90 CDefinitionManager::InstantiateDef -- RELOCATION_MATCH (5 grades, first authoring attempt)
- What made it match: port onto the shared headers with the dossier's identities (parameter roles class name / instance name / is_template; `info->Name` instead of
  the class-name argument after the lookup; inline `CDefString(const CCharString&) { SetString(); }`), then three source facts: (1) `if (info) { ... return true; }
  return false;` (an early `return false` shrink-wraps the prologue); (2) the two indices are signed `long` -- pushing the long into `CArray<unsigned long>` makes the
  conversion temporary retail stores in the dead is_template slot; (3) the record is built in its OWN statement (`CInstantiatedDefInfo* def_info = new ...;` then
  `push_back(def_info)`): with the `new` inside the push_back call the class-name CDefString temporary cannot share the is_template slot and the frame grows by 4.
  SetInstantiationName takes the name by implicit conversion (built in the argument slot, `push ecx ; mov ecx,esp`).
- Headers / flags: no compiler flag. Overlay $W/hdr (4 headers, see hdr/README.md): FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF, FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING,
  FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY, FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR (FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES is no longer used since Repair 2).
- Unproven detail: the body of FableWin's `CDefPointer(T*)` ctor is modelled from this one expansion (store only, the creation reference is adopted -- see
  Repair 2; the first version's "store + AddRef" was wrong). CInstantiatedDefInfo's constructors ARE in FableWin.pdb (corrected in Repair 2):
  `??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z` (FableWin 0x03061650) and the default
  `??0CInstantiatedDefInfo@CDefinitionManager@@QAE@XZ` (FableWin 0x0305D840); they are inline in retail and absent from Ego_r.pdb's list. Only the
  parameter NAMES are descriptive (public symbols carry none).
- Names: no made-up function names. Landed spellings to fix: stdvector_Minsertoverflow_009b4bd0.cpp uses the element placeholder `CUnprovenElement_009b4bd0` (exact landed spelling: forward declaration line 17, `// Leaf:` mangled name `PAVCUnprovenElement_009b4bd0@@`, explicit instantiation line 92); this row's call
  site 0x009ADD90 passes InstantiatedDefs, so the element is `CDefinitionManager::CInstantiatedDefInfo*` (NAME_FIXES.tsv). The old draft's FindClassInfo / Create /
  NeedsName / SetName / Destroy / SInstantiatedDefInfo / InsertGrow are gone (GetPDefClassInfo / AllocFunc / IsParentDef / SetInstantiationName / Delete /
  CInstantiatedDefInfo / _M_insert_overflow).
- Repair (verifier finding 1): NAME_FIXES.tsv wrong_spellings_seen column now holds exactly the landed spelling `CUnprovenElement_009b4bd0` (the parenthetical moved to the evidence column); source unchanged, regraded RELOCATION_MATCH (also strict headers), test PASS.

## Repair 2 (evidence verifier: constructor signature, false PDB claim, test-only branch) -- RELOCATION_MATCH kept, 1 new spelling graded
- Item 1 (constructor signature) FIXED by respelling, first variant: the overlay's record constructor now has the FableWin.pdb signature
  `CInstantiatedDefInfo(CDefString, CDefString, unsigned long, const CDefPointer<CDefClassBase>&)` (0x03061650; the default ctor 0x0305D840 is declared too,
  body not reconstructed). Row spelling: `CDefPointer<CDefClassBase> pdef(info->AllocFunc());` -- a handle local built by the pointer constructor
  (`??0?$CDefPointer@VCDefClassBase@@@@QAE@PAVCDefClassBase@@@Z`, FableWin 0x030644A0) -- used through `operator->`, passed to the record constructor, and
  NO explicit `pdef->Release()`: the handle's destructor at the end of the `if` block is retail's final `dec [esi+4] ; jne ; call [edx+4]` (0x009ADDEA).
  The pointer constructor's body changed from "store + AddRef" to STORE ONLY (it adopts the creation reference): retail has no `inc` after
  `mov esi,eax` (0x009ADCD6); the single `inc dword ptr [esi+4]` (0x009ADD4C) is the record's copy of the handle. Second source for the adoption: a new
  pointee is born with IVRefCount 1 (CIVCountedPointeeBase ctor, row 0x00AC5770 / FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR), so 1 (adopted) + 1 (record) - 1
  (handle dtor) leaves the record's reference. VC7.1 emits neither the copy constructor's nor the destructor's `if (Object)` test for this local.
  Graded: a1 (this spelling) RELOCATION_MATCH at once, regraded as 009adc90.cpp and with LANDVERIFY_STRICT_HEADERS=1; a2 / a3 (raw pointer + conversion
  temporary + explicit Release) were queued in the same call and not needed. FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES dropped from the row.
- Item 2 (false "not in any PDB list" claim) FIXED: NOTES.md (above) and DOSSIER.md (identity-gaps line) now cite both FableWin.pdb constructor symbols.
- Item 3 (test-only `if (DropRecordReference) pdef->Release();`) FIXED: the test's body is the row's body verbatim, with a faithful CDefPointer model (adopting
  pointer ctor, counting copy, releasing dtor). The delete path is reached through the harness's `CInstantiatedDefInfo::operator new`, which can return null
  (retail's own `test ebp,ebp ; je` path at 0x009ADD18): no record constructor runs, a null record is pushed, and the handle's destructor deletes the def
  exactly once. vt.py PASS (g++ too); it can fail: three mutants graded with vt.py all FAIL (class_name instead of info->Name; a counting pointer ctor;
  a destructor that does not release).
- Overlay headers changed in Repair 2: hdr/engine/CDefinitionManager.h (record ctor signature + default ctor declaration + comment), hdr/engine/CDefPointer.h
  (pointer ctor body and comment). CDefString.h / CSymbolMap.h unchanged. The previous matching state is kept in repair2_backup/ (hdr, source, test).
- Made-up names: none (parameter names class_name / instantiation_name / class_index / pdef and the local def_info are descriptive).


## Coordinator correction at landing (2026-11-12, after two independent verifiers)

AllocFunc is __fastcall (FableWin `P6I` in ??0CDefClassInit; all Ego_r `?Alloc@C*Def@@SI...` are static __fastcall); every "__cdecl" above is wrong (the struct-layout dump prints __cdecl for every function pointer). The row now uses CDefClassInit::TAllocFunc. The pointer constructor opt-in is FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT, proven on two retail sites (0x009ADC90 and InstantiateSubDef 0x009ADAE0).
