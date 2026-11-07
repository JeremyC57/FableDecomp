# Hand chunk 850 (CWorldMap .. CTCShop): 2 of 6 byte-exact

## What matched (2)
- **C3DMeshFileEngineMaterialChunk::ReadFromFile** 0x00AC5770 (RELOCATION_MATCH, `?ReadFromFile@C3DMeshFileEngineMaterialChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z`,
  vtable 0x0129EFC8 slot 7): version 4 reads the map flags and an optional CMapFilenames set (EBOOL-gated), older versions always build
  the set, read the degenerate flag from version 3 and call CalculateMapFlags (0x00AC5260); both hand the bytes left to
  CChunkedFileSuperChunk::ReadFromFile (0x00AA8830). Shape: a named `header_size = in.GetPosition() - start_pos` local orders the
  GetPosition call before the pushes; CIVCountedPointeeBase has a user ctor `IVRefCount(1)` (stored before the derived vtable);
  CIVCountedPointer::Reset adopts the new object without AddRef. Retail class branch in engine/C3DMeshFileEngineMaterialChunk.h.
- **CConsole::Initialise** 0x009ED190 (RELOCATION_MATCH, `?Initialise@CConsole@@QAEXDW4EInputKey@@PBVCFontBank@@@Z`): stores toggle
  char / key / font, clears the registered-input map (_Rb_tree::_M_erase 0x009EF6E0), registers 14 CConsoleCommand<CConsole>
  commands with their parameter lists and the ConsoleAlpha CConsoleVariable, clears the history list (0x009EE7B0) and resets
  HistoryPos, then sends the greeting. Local model in the NGlobalConsole::Initialise 0x00419D90 / CConsole::CConsole 0x009ECD80
  precedent (no CConsole header): implicit `const char*` -> CCharString name temporaries; the one-type CConsoleCommandParameters ctor
  inline, the (t,t) 0x009EBB80, (t,t,t) 0x009EBC00 and copy 0x00419F50 ctors out of line; CConsoleCommandBase(name, const
  EGenericVarType&) out of line at 0x00414F30. The fix from c1 (2 diff lines) to c2 was the PDB member `CGenericVarRef Var` (+8) with
  its own ctor inside CConsoleVariable instead of flat Type / PValSLONG members (the lea was scheduled before the vptr store).
  Command callbacks: ConsoleAddStringBinding 0x009ECCE0, ConsoleRunStringBinding 0x009ECD40, ConsoleAddKeyBinding 0x009ECC70,
  ConsoleRunScript 0x009ECB70, ConsoleConsoleListContainingString 0x009EB590, ConsoleCommandList 0x009EB690,
  ConsoleCommandListContainingString 0x009EB760, ConsoleVariableList 0x009EB8E0, ConsoleVariableListContainingString 0x009EB9B0,
  SetConsole{Text,HistoryWindow,ClientWindow,HelpWindow,ScrollBar}Colour 0x009EAA50..0x009EAB10 (all static __fastcall, Ego_r spellings).
  `EGenericVarType` enumerator names in the local model (NONE, SLONG, ULONG, FLOAT, BOOL, STRING) are **descriptive** (values 1/2/5 used).

## Parked for Fable 5.1 (3)
- 0x00AE3C30 CParticleProgram::ReadFromFile DIFFER(2278v2310): retail shares the never-written default C3DTriangle2 resize temp with the saved-this slot (frame 0x10) and gives ebx the zero constant once `pool` is dead; ours puts the temp on the pair slot (0xc) and keeps `this` in ebx | tried: straight WriteToFile mirror (p1, best), explicit fill temp, named pair, named counts / total, C3DTriangle2 user ctor (p10, C3DTriangle2.h opt-in dropped), out-of-line vs expanded resize(n) instances | next: the CArray resize as a same-TU noinline instance of the real template, or reader-TU member order
- 0x00500540 CWorldMap::LoadRegion DIFFER(2396v2412): region_maps / c1 in ebp / ebx (retail ebx / ebp), the level_files end pointer kept in esi across the back() stores (retail reloads it after every store), frame 0x94 v 0x98, wait loop rotated (retail jumps back to an aligned top test) | tried: Codex pass-13 body rebased onto our headers + opt-ins (r2), CRegionDef offsets + IntelligentPointer assign (r3), same-TU helper bodies on/off (r4, r6), implicit CLevelFiles dtor per PDB 0x103 (r5: fixed the push_back copy), shared particle-driver local (r7), while / goto wait loops (r8, r9), back() reference (r10), declaration order (r11) | next: make the end pointer escape (by-pointer CLevelFiles* / iterator after push_back) or STLport back() through a by-value iterator; Codex's private-tree best was also 2396
- 0x00772BB0 CTCShop::AddSaleItem DIFFER(2585v2585): x87 scheduling only in the matrix block -- int/x87 interleave of the inline Set stores (0x00772DE7), first PostMultiply rep movsd after the fdivr (retail before), one inverse operand order (E13 = scale*m.E31), inverse translation rows 2-3 summed a,c,b | tried: chunk 794 CTCWallMount::AttachObject model, Set(rhset,pos) out of line / inline / __forceinline, SetMultiply restructure of PostMultiply (248 -> 102), source term orders (no effect), named matrices / scopes (13 grades) | next: a row-local __forceinline SetInverseOfUniformScaleRotationTranslation expansion with FableWin's term order, or a by-reference helper as SetMultiply was

## Skipped
- 0x0050A650 CWorldMap::GetMapNavigationAreaInit SRC_FAIL (2 attempts, below the 10-attempt bar -- time): the Codex pass-13 body
  (2233v2233, 4 bytes, on its private header tree) needs typed models our tree lacks: CThingFilter_IsInQuestOrNullQuest /
  CThingFilter_OverlappingArea / CThingFilter_InArea, GFFilter_Bind3, CThingSearchTools::GetAllLeafThings / GetAllTCsInRoughArea /
  GetAllTCDsInRoughArea template bodies, CNavigationAreaInit / CNavigationPosition, C2DBoxF, CThingPhysical::UseHighDetailQuadTree,
  and CNavigationAreaInit as a CNavigationAreaUpdate for GetMapNavigationAreaUpdateInfo. n2 fixes the C2DVector / C2DCoordI part
  (new opt-in C2DCoordI::ToC2DCoordF); 56 errors remain. Partial Codex headers exist in
  rebuild/candidates/manual/pending/2026-11-02_codex_pass13/verified_pending/objects/ (044362ce...h CNavigationAreaInit, 573541825b...h
  GetAllLeafThings / GetAllTCDsInRoughArea, 1f9cef88...h UseHighDetailQuadTree); the filters are in none of them.

## Label corrections
See LABELS.tsv: 0x009EE7B0 std::_List_base<CConsoleCommandLine>::clear (was _unattributed::clear_CRetailList_009ee7b0),
0x006BB2F0 CGameTimeManager::SetAsUsingVillageTime (was CXMVPlaybackInfo::CXMVPlaybackInfo; body ignores its argument -- check for an
identical-COMDAT fold), 0x009964E0 CDataInputStream::CopyTo (was ReadWithChunking), 0x00ADF460 CParticleProgram::AllocateArrays (was
CMovableResource::AllocateParticles).

## Names and bindings (one symbol, one address)
- Every name is an Ego_r.pdb / FableWin.pdb spelling checked against a second source (call site argument types, vtable slot,
  member offsets); NAME_FIXES.tsv lists 11 addresses where reloc_bindings.tsv or landed sources carry a wrong or placeholder spelling.
- CONFLICT 0x009EF900: src/009ed190.cpp binds `vector<EGenericVarType>::_M_insert_overflow(..., const __false_type&, ...)` (STLport
  4.6 has no __type_traits for an enum); reloc_bindings binds the `__true_type` spelling at the same address and the landed 0x009EBB30
  row uses it. The coordinator must pick one (check the body: element loop = __false_type, memmove = __true_type).
- 0x00A97A50 is bound as `?resize@?$CArray@VC3DTriangle2@@@@QAEXKABVC3DTriangle2@@@Z`; neither PDB has a CArray resize, the STLport
  vector<C3DTriangle2>::resize(n, x) is the likely real name (unverified; only used by the parked 0x00AE3C30).
- Descriptive (no PDB name found): the local EGenericVarType enumerators; the CTCDParticleEmitter Create / AttachToThing parameter names.

## Header debt
- engine/CTCDParticleEmitter.h, engine/CRegionDef.h, engine/CGameTimeManager.h and engine/CAsyncLoadingMemoryPool.h do not exist;
  differ/00500540.cpp carries local models (CRegionDef at the retail offsets: Ego_r less 4 before SoundMap, less 12 after).
- engine/CWADFile.h was data-only (Ego_r layout); the FABLE_RETAIL_CWADFILE class branch should become the retail default once its
  vtable and override addresses are bound.
- C3DTriangle2: no shared resize model.
