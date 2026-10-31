# Hand chunk 629 (CTCHeroExperience .. CMainGameComponent): 0 of 10 byte-exact

All ten rows are parked same-length misses from the 2026-10-30/31 retry sweep. Each was regraded from its best attempt
(reproduced first), re-modelled on the shared headers / PDB spellings where they exist, and retested with new levers.
None flipped. Every differ/ source below grades the same length as retail with a full-symbol `// Leaf:` (no leaf warning).

## What matched (0)

(none)

## Parked for Fable 5.1 (10)

- 006d8300 CTCHeroExperience::AddTrainableStatExperience DIFFER(173v173): `lea eax,[ecx+edx]` (scaled index + begin) v ours `lea eax,[edx+ecx]` | tried: sweep forms + vector reference local, signed index, `begin()[n]`, `v[c] = v[c] + g` / `g + v[c]`, inline ModifyTrainableStatExperience helper, char-offset address `*(T*)(n*sizeof(T) + (char*)begin())`, explicit clamp (168) | next: lane gap: SIB base/index order of a CSE'd scaled index (VC canonicalises every pointer+index spelling)
- 009d5d30 CBankFileAsyncData::CBankFileAsyncData(void*, unsigned long, CBankFileAsync&) DIFFER(179v179): flag stores +0x44/+0x45 between `add edx,14Ch` and `push edx`, +0x46/+0x47 after `lea ecx` in retail; ours all four after `lea ecx` | tried: sweep forms + /G5 /G6 /G7 /GB, PDB `_RTL_CRITICAL_SECTION&` lock parameter, flags after the lock, real std::set / _Rb_tree::insert_unique model | next: lane gap: instruction scheduling (same instructions)
- 00619560 GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo> DIFFER(180v180): traits `lea ecx,[esp+18h]` (dead count word) v retail `[esp+1Ch]` (over the dead T() temporary) | tried: sweep forms + traits with `T DefaultVal` (ctor element: ten extra stores; POD element: overlays the temporary but zero-fills through ecx and opens edi inside the guard), function-scope traits, traits declared first / between, traits temporary per iteration, traits in the loop body, explicit `resize(count, T())` | next: a CUIRegionInfo whose ctor zeroes through `= 0` assignments AND a 0x28-byte traits without init (unproven real shape; the BinaryOut twin 0x00614700 shows an uninitialised 0x28-byte traits)
- 009d5df0 CBankFileAsyncData::CBankFileAsyncData(unsigned long, unsigned long, CMovableResourceMemoryPool*, CBankFileAsync&) DIFFER(183v183): flag stores interleaved with `push ebx ; push ecx ; lea ecx` in retail, ours after them | tried: as 009d5d30 | next: lane gap: instruction scheduling
- 00699130 CGameCameraManager::ResetFlags DIFFER(183v183): `mov ebp,[ebp+0ACh] ; mov [ebp+4],ebp` v retail `mov eax,[ebp+0ACh] ; pop esi ; mov [eax+4],eax` | tried: sweep STLport clear shapes + header accessor (reference local / per use), header stores in an inline `_M_set_empty()`, a `self` local | next: lane gap: register choice for a reload into a dying register at the epilogue
- 00408820 CUserProfileManager::ResetAssignedInputsWASD DIFFER(185v185): this/def/&Controls in ebx/edi/esi (esi pushed inside the found block) v retail edi/esi/ebx (ebx pushed before the name ctor) | tried: sweep forms + real CDefPointer<const CControlsDef> local (202, re-reads Object), def-before-scheme order (189), `const vector&` / pointer local for Controls at block top / after the name / in the found block, goto out of the block, named found bool | next: lane gap: callee-saved register rotation
- 00a358f0 C3DMeshObject::GetPositionMatrixFromRHSet DIFFER(185v185): z product `fld [edx+4] ; fmul [edx+0Ch]` v retail `fld [edx+0Ch] ; fmul [edx+4]` | tried: sweep forms + raw float pointer spellings of the z operands (`(&a.X)[0]`, `*(&a.Y-1)`, pointer locals, all six terms through pointers), `-(a.Y*b.X) + a.X*b.Y`, named x/y/z locals, member C3DVector::CrossProduct, no copy ctor (249) | next: lane gap: x87 operand order (both loads are also by-value copy loads)
- 008930f0 CGameScriptInterface::GetThingWithScriptName(const CCharString&, const CCharString&) DIFFER(186v186): found block first (`jne null` x3) v retail null fall-through then found (`je found`); `mov [thing],0` before `push eax` v after; `mov [esp+1Ch],eax` before `mov ecx,[ecx+20h]` | tried: sweep forms + CGameScriptThing::CreateNullThing (bound) for the null result, failure-first / success-first / separate not-found return, `found` bool, IS_THING_ALIVE helper, thing declared after the map number (store moves below the call), ternary return (236) | next: lane gap: return-block placement (both orders lay out the same)
- 00ad8af0 CVertexGroupBlock::SharesGroupWith DIFFER(188v188): this's map header in ebx and other's in ebp v retail other's in ebx (loaded first) and this's in ebp | tried: sweep forms + end iterators as named locals (moves the `other` pointer load first -- kept), every order of the two ends / two begins, ends after begins, reference locals to both maps, for-init with b first, `return true` in the loop (worse) | next: lane gap: callee-saved register rotation
- 0041726d CMainGameComponent::UpdateFromEventPackageSet DIFFER(189v189): package / new server frame in edi and &CurrentServerFrame in ebx v retail ebx / edi | tried: sweep forms + PDB signatures (set by pointer, const package pointers, CWorld::Update by reference), `long&` local for CurrentServerFrame, package declared outside the loop, swapped compare, Max result stored into the member (190) | next: lane gap: callee-saved register rotation

## Skipped

(none)

## Label corrections

- 0x00619560: catalog `GFSerialiseVectorBinaryIn<CBrainUpdateZone>` is propagated; FableWin.pdb `??$GFSerialiseVectorBinaryIn@VCUIRegionInfo@CTCInventoryMap@@@@YIX...` (0x28 stride, CTCInventoryMap caller 0x006197D0, bound resize 0x006190C0).
- 0x00AD8AF0: catalog `global::TreeNodes_HaveIntersection` is descriptive; Ego_r/FableWin `?SharesGroupWith@CVertexGroupBlock@@QBE_NABV1@@Z`.
- 0x00A358F0: catalog `global::GetPositionMatrixFromRHSet`; Ego_r/FableWin `?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z` (static __fastcall).
- 0x006D8300: the parameter is EHeroExperienceStatCategory (PDB), not EHeroTrainableStatType (the parked source's type).
- 0x00408820: the flag at 0x013B85F3 is the bound `g_bAggressiveModeAvailable` (parked source: "pad in use"), 0x013B864F is `g_KeyRedefinerFlag`, 0x013B8760 is `g_value_013b8760` (CStartupDefinitionManager::Get loads it); the def is CControlsDef (Controls at retail +0x3C); 0x00410820 is GetDef<CControlsDef> (catalog `GetDef<CHeroPostcardGeneratorDef>`).
- 0x008930F0: the null result 0x008CFF40 is the bound `CGameScriptThing::CreateNullThing()` (static __fastcall, hidden result in ecx), not a CScriptThing ctor.
- 0x00699130: ResetFlags is private (`?ResetFlags@CGameCameraManager@@AAEXXZ`); 0x0041726D is protected and takes the set by pointer.
- 0x009D7CF0: catalog `BinaryTree_FindOrInsert` / `InsertUnique_CRetailTree_009d7cf0` is set<CBankFileAsyncData*>'s `_Rb_tree::insert_unique(const value_type&)` (pair<iterator,bool> by hidden pointer; called from both ctors' inline set::insert) -- spelling from STLport 4.6, not from a PDB name.

## Names and evidence

- Proven (two sources each): every row name above (PDB name set + caller / layout / sibling order); callees per NAME_FIXES.tsv
  and the differ/ headers. Descriptive only: `CTCHeroExperience::GetParentThing` / `GetStatExperiencePoints` accessors (inline
  views over the header's flattened bytes), `GetControlsDef` (file-static __forceinline, no symbol), CaughtUpWithInput +0x161E1
  (no PDB member at that retail offset).

## Header debt

- engine/CTCHeroExperience.h flattens CTCBase into `_base_0x04[8]` and keeps StatExperiencePoints as opaque bytes (row reads
  ParentThing and the vector through inline casts); engine/CMovableResource.h is a flat `void* __vftable` struct, so the
  CBankFileAsyncData rows keep a local polymorphic CMovableResource base.
