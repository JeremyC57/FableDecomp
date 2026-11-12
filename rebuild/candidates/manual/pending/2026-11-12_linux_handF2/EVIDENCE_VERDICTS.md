# Independent evidence verdicts (Opus verifiers, one per row; workflow verify-handback-evidence)

## 0x00406AE0 -- ok=True, name proven=True, test can fail=True, issues=0

I found nothing that must be fixed before landing. I did not compile or run anything; the coordinator does that.

1. Name and signature. The mangled name ?GetManualSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@J@Z appears in two independent places: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, 0x00405720) and FableWin (ghidra_out/_fablewin_api.tsv and FableWin.exe.signatures.tsv, 0x0187B720). Ghidra's retail Fable.exe signature table (naming_stragglers/Fable.exe.signatures.tsv), engine_api.tsv and functions.tsv (accepted) put CUserProfileManager::GetManualSaveDisplayName at 0x00406AE0 with length 314. The function also references "TEXT_GUI_SAVE_MANUAL_SAVE_NAME" (fp_fable.tsv). QBE means public, const and not virtual; it returns CWideString and takes a long. The overlay header declares it exactly that way, and `ret 8` matches the hidden result plus the long. xref.py shows no caller other than a jump inside its own body, so there is no vtable slot.

2. Callees. Every call target is named with its PDB or landed name and the right address:
- 0x0099EBF0: CCharString(const char*, long = -1); retail pushes -1.
- 0x0099AED0, 0x0099BA70 (Sprintf), 0x0099B720, 0x0099B510: the CWideString constructors, formatter and destructor.
- 0x009C95E0: NGameText::CDataBank::GetTextBySymbol, which is QBE and returns a const CWideString.
- 0x0099BF30 and 0x0099BE70: the landed fastcall global operator+ overloads.
- 0x00405F00: GFIntToWideString, the YI (fastcall) spelling in both PDBs.
- 0x00406610: GetManualSaveFileName. On that path retail calls it with ecx untouched, so `this` is passed through.
No made-up names are used.

3. Members. All five accesses check out:
- CMainGameComponent::PTextBank is at +0x14 (Egor 20), matching `mov ecx,[esi+14h]`.
- CNewFrontendGameComponent::m_pTextBank is at +0x60 (Egor 96), matching `mov esi,[esi+60h]`.
- The statics PMainGameComponent (0x013B86A0) and m_pNewFrontendGameComponent (0x013B871C) are read through the inline Get() accessors, which are in FableWin.
- GetTextBank is inline in FableWin for both classes.

4. Local classes: none. Everything comes from the shared headers.

5. Real bodies. The only extra body is the inline GFIntToWideString. Apart from the `inline` keyword and a local's name, its code is the same as the landed rebuild/src/compiled/00/40/global_GFIntToWideString_00405f00.cpp. Retail expands it in the first arm (constructor, Sprintf with L"%d", copy, destructor at 0x00406B02-0x00406B34) and calls the out-of-line copy in the second (0x00406BA1). The header comment states this.

6. The test is real and can fail. It checks four cases: with no component it returns the file name; with only the front end it uses the front end's bank text plus " " plus the number; when both exist the game's bank wins and the front end is not looked up; the slot can be negative. It also counts lookups on each bank.

7. Faithfulness. The symbol is written as an implicit CCharString temporary rather than a named local, and the source comment explains why with the DIFFER 344v314 result. The disassembly agrees: the frame is 0x14 bytes, the destructor tails merge at 0x00406BE2, and the CCharString constructor runs before the number in both arms. No "kept as is" oddities are claimed. Two defined macros, FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED and FABLE_RETAIL_DEF_CLASS_BASE, look unused by this body; they are harmless.

8. NAME_FIXES.tsv has no entries for this row's callees. LABELS.tsv lists 00405f00 GFIntToWideString, 00406610 GetManualSaveFileName (const) and 009c95e0 GetTextBySymbol, and all three agree with the PDB mangled names.

Two observations that need no fix in this row:
- The landed CUserProfileManager_GetManualSaveFileName_00406610.cpp declares the method non-const, using a local CWS/CWSraw model, while the PDB spelling is QBE (const). The coordinator may want to retype that landed row later.
- The landed 0x00405F00 file's top comment still says "IntToWideString", while its Leaf line says GFIntToWideString.

The test calls `std::swprintf(buffer, L"%d", ...)` with two arguments. That is VC7.1's non-standard form; verify_and_land compiles tests with VC7.1 cl, so it should be accepted, but I did not confirm it builds.

## 00909a00 -- ok=True, name proven=True, test can fail=True, issues=0

NAME: ?IsValid@CCombatSequence_WillForcePushLongRange@@UBE_NXZ is proven by independent sources. (a) rebuild/manifest/rtti-vtables.tsv lists 0x00909A00 at slot 12 of the CCombatSequence_WillForcePushLongRange vtable 0x0127A30C. The parent CCombatSequence_WillForcePush has its landed IsValid 0x009098E0 at the same slot 12. (b) ghidra_out/labels_rtti_port.tsv, rebuild/manifest/functions.tsv and ghidra_out/engine_api.tsv carry the same mangled name. (c) The symbol exists in Ego_r.pdb (0x0092FE10) and in FableWin.pdb (0x01802230 / 0x02B464C0) with the same UBE_NXZ spelling: public virtual, const, returning bool. The source's signature and `// Leaf:` line agree. rtti_port_compare.tsv tags the row "disagree", but its proposed name is this same spelling. Length: retail runs 0x00909A00..0x00909B4F = 336 bytes (also 336 in Fable.exe.signatures.tsv). The 329 in the task text is wrong; the source's 336 is right.
CALLEES (checked against the disassembly):
- 0x004365B0 is CVectorMap::LowerBound, reached through the shared-header __forceinline PeekTC. The `test ah,40h` on [edi+38h] is bit 206 (0xCE) of UsingComponents (+0x20, word 6). TCI_AICREATURE_WILL_POWER_INDICATOR is 206 in ETCInterfaceType.h.
- 0x004C73D0 is CThing::GetPos. The manifest names it, and ?GetPos@CThing@@QBEABVC3DVector@@XZ exists in both PDBs.
- vtable +0x2C is CAIGameCameraBase::PeekCurrentPos, slot 11. The shared header declares it `const C3DVector` by value, which matches the hidden return pointer in retail. Ego_r and FableWin both have ?PeekCurrentPos@CAIGameCameraBase@@UBE?BVC3DVector@@XZ.
- vtable +0x04 is GetRenderCamera(float), slot 1. 1.0f (0x3F800000) is pushed. The name comes from the shared header, backed by the FableWin ?GetRenderCamera@CGameCameraManager/CGameCameraBase@@UBE?AVCCamera@@M@Z overrides.
- No address-suffixed or made-up callee names.
MEMBERS (spot-checked against struct_layouts_egor.tsv and the shared headers):
- CCombatSequenceBase::Creature is read at +0x1C. Ego_r has it at +0x20 because its vector is 16 bytes; the retail STLport vector is 0xC, so +0x1C is right.
- CTCAICreatureWillPowerIndicator::PoweredUp, bool at +0x0C.
- CMainGameComponent::PWorld at +0x24.
- CWorld::PCurrentGameCamera at +0x34.
- CCamera::RHSet (+12) plus CRightHandedSet::Forward (+12) = +0x18, read by PeekViewDirection (`fld [eax+18h]`).
- CThing::UsingComponents at +0x20 (bit test at +0x38).
LOCAL CLASSES:
- rebuild/include/engine/CCombatSequenceBase.h and CTCAICreatureWillPowerIndicator.h do not exist.
- The members carry Ego_r.pdb names: Running, NoAvailableActions, CurrentActionIndex, CombatActions, Creature, Target, CreateCalled; PoweredUp.
- Observation: the vector is an opaque byte array named `CombatActions_`, with a trailing underscore.
- The landed row 0x00909B50 models CCombatSequenceBase locally in the same way.
REAL BODIES: the only body besides the row's own is the inline IsPoweredUp() { return PoweredUp; }. FableWin.pdb has ?IsPoweredUp@CTCAICreatureWillPowerIndicator@@QBE_NXZ (QBE_NXZ). Retail expands it as `mov cl,[eax+0Ch]; test cl,cl; je`. All other inlines come from the shared headers.
TEST: it checks four things, so it can fail.
- No indicator gives true with no camera calls.
- An indicator that is not powered up gives false.
- When powered up, it runs the camera path once per call, and the flattened, normalised dot product is +0.8 (creature in front) or -0.8 (behind), returning true both times.
FAITHFULNESS: the odd shape (`else return false` plus two separate `return true` statements) has a comment naming the retail bytes. I confirmed in the disassembly that retail computes the dot product, then does `fcomp [0x0122DEDC]; fstp st(0); fnstsw ax` with no branch and falls into `mov al,1`. So both camera outcomes really return true. The creature reference local matches the single `mov edi,[ecx+1Ch]`.
HEADER OVERLAY: hdr/engine/CCombatSequence_WillForcePushLongRange.h only adds an opt-in retail branch (`: CCombatSequenceBase`, sizeof 0x28) and leaves the generated default unchanged.
NAME_FIXES: the 0x009098E0 entry holds. The landed rebuild/src/compiled/00/90/CCombatSequenceWillForcePush_IsValid_009098e0.cpp really uses `struct CTCWillPower { bool Active; }` and `m_pOwner` at +0x1C, while the PDBs give CTCAICreatureWillPowerIndicator::PoweredUp / IsPoweredUp and CCombatSequenceBase::Creature. No LABELS.tsv entries concern this row.

## 0x009D5BF0 -- ok=True, name proven=True, test can fail=True, issues=0

I found nothing that must be fixed before landing.

1. Name and signature: `?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ` (public, non-virtual, non-const, returns bool, no parameters) is in two independent sources. It appears in ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv (Ego_r 0x00B29B20) and in ghidra_out/_fablewin_api.tsv (FableWin 0x017981E6). The source's Leaf line and the header overlay declaration match that spelling. The manifest (rebuild/manifest/functions.tsv) also lists 009d5bf0 as CBankFileAsync::HandleFailedAllocation. The only caller is 0x009D5F3B.

2. Callees match the disassembly:
- 0x00A62920 is CCriticalSectionLock(_RTL_CRITICAL_SECTION&, bool), matching PDB `??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z`. Retail pushes 0 and &[this+0x14C], matching `lock(CriticalSection, false)`.
- 0x00A629A0 is the destructor.
- 0x00A62ED0 is CMovableResourceMemoryPool::FreeResource(long), matching PDB `?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z`.
- 0x009D7CF0 is the STLport _Rb_tree<CBankFileAsyncData*>::insert_unique, which is landed (stdRbtree_insertunique_009d7cf0.cpp).
- No made-up names are used.

3. Members match the retail offsets, using the base 0x110 plus three 8-byte CCountedPointers and 12-byte STLport sets:
- CBankFileAsync: AsyncDataInstances +0x128 (retail [esi+128h]), size word +0x12C, ReadsToRestart +0x134, LastRestartingData +0x140, CriticalSection +0x14C.
- CBankFileAsyncData: Data +8, Buffer +0xC, MemoryPool +0x1C (CMovableResourceMemoryPool*), MemoryHandle +0x20 (long), Finished +0x44, AllowRestart +0x46. These match struct_layouts_egor.tsv exactly.
- The Ego_r offsets for CBankFileAsync differ (16-byte sets in that build). The header uses the retail layout, which the disassembly confirms.
- Finished and AllowRestart are volatile, which the header justifies from the PDB's LF_MODIFIER record.

4. Local classes: none.

5. Real bodies: the only other body is the inline CBankFileAsyncData::FreeBuffer (Ego_r `?FreeBuffer@CBankFileAsyncData@@AAEXXZ`, private). Its code is the same as the landed rebuild/src/compiled/00/9d/CBankFileAsyncData_FreeBuffer_009d5410.cpp. The landed row uses older member spellings (m_pPool, m_handle, m_pReadBuffer, m_pData, plus a cast); the meaning and order of operations are identical. The expansion at 0x9D5CCF-0x9D5CF2 is in retail (test +1C, cmp +20 with -1, call FreeResource, store -1 to +20, 0 to +0C, 0 to +08), and the comment cites this inline expansion. The private FreeBuffer and the bank's access to its flags need the overlay's `friend class CBankFileAsync`. The PDB extracts I checked cannot confirm the friend record, but nothing else would let the access compile.

6. Test: it checks the empty set, nothing finished, and finished-but-no-restart / restart-but-not-finished filtering. It also checks starting after LastRestartingData through upper_bound, wrap-around to begin(), and FreeBuffer's pool and handle effects including the null-pool case. It checks that ReadsToRestart is filled and that false is returned once no candidate is left. I traced every step by hand and it is consistent; a wrong search start, wrap or filter would make it fail.

7. Faithfulness: I found no steering-only constructs. The FABLE_RETAIL_STLP_MAP + ORIGINAL_INSERT_BODY opt-in is explained by the retail `mov [edi+140h],esi` with no reload of `data`, and 16 landed rows use the same define. Retail does test `if (count == 0)` before upper_bound, and the loop does compare against end() and reload begin(), as the source says.

8. LABELS.tsv entries for 009d7cf0 insert_unique and 009d5410 CBankFileAsyncData::FreeBuffer agree with the manifest and the PDB (Ego_r 0x00B29480). The NAME_FIXES 009d5a90 RestartReads -> `?RestartAbortedReads@CBankFileAsync@@AAEXXZ` concerns a sibling, not a callee of this row.

Observation: the landed 0x009D5410 FreeBuffer still uses pre-PDB member names (m_pPool and the others), which the coordinator might respell later.

## 00b939e0 -- ok=True, name proven=True, test can fail=True, issues=0

1. Name and signature: the Leaf `?CheckVisibilityResults@CEngineSceneGrid@@QAE_NW4EEngineOcclusionLayer@@KKKK_N@Z` appears in Ego_r.pdb (ghidra_out/egor_pdb_names.tsv and egor_signatures.tsv, 0x00a460b0) and in FableWin.pdb (_fablewin_api.tsv and FableWin.exe.signatures.tsv, 0x02e1b590). These are two independent sources. The address binding also holds up. In Ego_r the cell's CheckVisibilityResults (0xa45de0) comes first, then the grid's (0xa460b0), then CEngineScene::CheckVisibility (0xa46380). Retail has the same order: 0xb936e0, 0xb939e0, 0xb93cc0. xref shows the only callers are the function itself (0xb93a95) and the landed CEngineScene::CheckVisibility at 0x00B93CC0, whose source already names this callee. rebuild/manifest/functions.tsv carries the same name. The source declares the function public, non-virtual, returning bool, with EEngineOcclusionLayer and four unsigned long parameters plus a bool. That fits `ret 18h` and agrees with the PDB spelling. CEngineSceneGrid has an RTTI vtable (012a3e58, slot 0 = 00b97a60), which is consistent with the CBaseClassNonCopyable base.
2. Callees: retail makes three calls. 0xb939e0 is the recursive call to itself. 0xb936e0 is CEngineSceneGridCell::CheckVisibilityResults: it is landed, its Ego_r mangling is AAE (private), and the overlay declares it private. 0xb92300 is CEngineSceneGridCell::CreateOcclusionGroup: it is landed and its PDB name matches. The Callees comment gives each address correctly. GetCell is the FableWin.pdb inline `?GetCell@CEngineSceneGrid@@QAEPAVCEngineSceneGridCell@@KK@Z`. Its body `GridCells[Size*y+x]` matches the retail bytes `imul [edi+10h],y ; add x ; mov esi,[edx+eax*4]` at both lookup sites. The source contains no made-up names.
3. Members: every one checked matches Ego_r struct_layouts_egor.tsv:
- cell->OccludedLayers is at +0x40, read by `test ebx,[cell+40h]`.
- HasOcclusionResults is bit 4 of +0x44, consistent with `test al,10h` and `and al,0xEF`.
- OccludedFrames is at +0x45, read by `movzx [esi+45h]` and cleared by `mov byte [esi+45h],0`.
- LocalCount is at +0x34, compared with `jb`, so the comparison is unsigned.
- Parent is at +0x00.
- On the grid: LevelDown +0x0C, Size +0x10, GridCells +0x14.
4. Local classes: CEngineSceneGrid is the only one, and rebuild/include/engine/CEngineSceneGrid.h does not exist. Its members (Level, LevelUp, LevelDown, Size, GridCells) use the PDB names and types. Ego_r gives the class size as 36; the source comment says retail is 0x20. That difference is the MSVC 16-byte vector versus the retail STLport 12-byte vector, and it does not affect anything.
5. Bodies: the only body besides the row's own is the inline GetCell, which is checked above. The overlay header adds declarations only: the private cell CheckVisibilityResults and `friend class CEngineSceneGrid`. hdr/README.md documents both.
6. Test: it runs a model copy of the logic and can fail. It checks:
- an empty cell returns false;
- children are visited over the 2x2 block, and a child occluded on the layer is skipped;
- visibility propagates up from a child;
- results already consumed are not consumed again;
- a cell occluded for too long has its counter reset, with no group created when there is no parent;
- min_group_size gates group creation, and the x/y passed to CreateOcclusionGroup are correct;
- a cell that is visible itself is not reset;
- the early return when the cell is occluded on the layer.
7. Faithfulness: retail does test the raw enumerator (`mov eax,[esi+40h] ; test ebx,eax`), as NOTES says. The single `return visible` is in retail: the visible flag lives in the layer argument slot at [esp+20h], and the arm where the cell itself is visible duplicates the return with `mov byte [esp+18h],1 ; mov al,[esp+18h]`. The Shape comment explains why both choices are needed. The `(unsigned long)` cast on LocalCount is redundant but matches retail's `jb` semantics. The occluded-frames reset is skipped when visible (`jne 0xb93b22`) and taken otherwise, which agrees with the source.
8. LABELS.tsv has two entries for this row's callees: 00b936e0 CEngineSceneGridCell::CheckVisibilityResults and 00b92300 CEngineSceneGridCell::CreateOcclusionGroup. Both agree with the landed rows and the PDB names. NAME_FIXES.tsv has no entry that concerns this row.

## 0x00BCFB00 -- ok=True, name proven=True, test can fail=True, issues=0

NAME: the name rests on three separate pieces of evidence. (a) Ego_r.pdb (egor_pdb_names.tsv) puts ?RenderPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@AAEXAAVC3DPrimitive2@@PBVCMatrix4x4@@@Z at 0x00A84370. Its neighbours there are GetTransformFrameEnd(2D), then this function, then SetAnimation, InitialiseSimulation x2 and the ctor, which is exactly retail's order 0x00BCF930 / 0x00BCFB00 / 0x00BCFC50 / 0x00BCFCB0 / 0x00BCFDE0 / 0x00BCFF10. Its Ego_r length, 0xA844C0-0xA84370 = 0x150, equals the 336-byte row. (b) FableWin.pdb has the same mangled symbol (_fablewin_api.tsv and FableWin.exe.signatures.tsv at 0x02E81320). (c) Callers 0x00BD5760 and 0x00BD5A94 sit in CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive and 0x00BD6774 in RenderPrimitiveToShadowBuffer, and the body does SetIndices plus DrawIndexedPrimitive, which fits the IndexBuffer variant and not PushBuffer. The signature matches the PDB: private (AAE), non-virtual, void, C3DPrimitive2&, const CMatrix4x4*, and `ret 8`. rtti-vtables has no slot for it, as expected for a non-virtual function.
CALLEES: each call target's spelling and address match its landed row and the PDB names. 0x0098B930 is CShaderRenderManager::SetVSConstants, reached through the inline SetBoneTransforms. Ego_r has an out-of-line SetBoneTransforms (0x00AE6440) but retail has no copy, and the call pushes count*3 (`lea eax,[eax+eax*2]`), the buffer, then the offset. 0x0099A2E0 is ~CBaseClass. 0x00A058C0 is CRenderStateManager::RealiseRenderState, with ecx = [mgr+0x1D0] RenderManager. COM +0x1A0 is SetIndices and +0x148 is DrawIndexedPrimitive. LABELS.tsv has entries for 0098b930, 0099a2e0, 00a058c0, 01436e98 and 01436e14, and all agree with the manifest. NAME_FIXES.tsv has nothing for this row.
MEMBERS: five accesses checked against the shared headers and the retail bytes. BoneMatrixBuffer is at +0x330, matching Ego_r (816, CPreTransposedBoneMatrix[18]; retail `add ecx,330h`). CAnimatedBlock: GroupCount at +0x17 (uchar) and Groups at +0x18 (uchar[18]) match Ego_r, and the block is 0x30 bytes (retail stride `add ecx,30h`). CBaseBlock: PrimitiveCount +0, StartIndex +4, IsStrip +8 match Ego_r. C3DPrimitive2 uses retail offsets from the shared header (Ego_r's offsets differ by 4 because it is another build): IndexBuffer +0x34, AnimatedBlockCount +0x48, AnimatedBlocks +0x4C, VertexCount +0x58. CIndexBuffer: PD3DIndexBuffer +4, PrimitiveType +0xC. CShaderRenderManager: RenderManager +0x1D0, D3DDevice +0x1D4. VSConstantLayoutLightsBones lands at +0x2D7C (recomputed from the header's member sizes), plus BoneMatrices +0xD4, plus Offset +4 = 0x2E54, which is what retail reads.
LOCAL CLASS: CEnginePrimitiveManagerAnimatedMeshesBase has no rebuild/include/engine header (checked). Its one modelled member, BoneMatrixBuffer, has its PDB name.
REAL BODIES: the inline DrawBuffersIndexed in the hdr overlay is code-identical to the one landed in 0x00B8EB00 (SetIndices, PT_NULL fallback to the index buffer's PrimitiveType, RealiseRenderState, DrawIndexedPrimitive(type,0,min,num,start,count)), and the expanded bytes match it. Retail keeps the `cmp esi,-1` test even though 4+(IsStrip!=0) can never be -1, which shows the generic inline body was expanded. SetBoneTransforms, GetBoneMatrices, PeekVSConstantLayoutLightsBones, GetAnimatedBlock, GetAnimatedBlockCount, GetVertexCount, GetGroupCount, GetGroup, GetPrimitiveCount, GetStartIndex and GetPrimitiveType are all FableWin.pdb inline symbols (the debug COMDATs at 0x02E8E4F0..0x02E8EA60), and each one's body is the trivial access the retail bytes show. The CShaderConstantRange implicit copy and destructor are justified in the header comment by the retail bytes (the vftable store at 0x00BCFB79, then a bare ~CBaseClass call at 0x00BCFBAE).
TEST: it checks, for two blocks plus an empty primitive, that bone groups are gathered from the 4x4 array in Groups order (first and last float of each upload), that upload register and row count are count*3, that draw vertex count, start index, primitive count and strip-vs-list type come out right, the palette contents left by the last block, and that nothing is drawn when there are no blocks. It returns FAIL on any mismatch, so it can fail.
FAITHFULNESS: the vertex/primitive/start locals are explained by the retail load order. Retail actually loads [ebx] (primitive count) first; the comment names that instruction sequence, and the grader settles the bytes. The reinterpret_cast copy of 48 of the 64 matrix bytes matches `mov ecx,0Ch ; rep movsd`.
Observation, no fix required: FableWin.pdb also has an inline ?GetIndexBuffer@C3DPrimitive2@@QAEPAVCIndexBuffer@@XZ (0x02E8E510) in the same debug COMDAT cluster, so the original probably wrote *primitive.GetIndexBuffer() where the source writes *primitive.IndexBuffer. That also makes the comment "everything goes through the PDB's inline accessors" slightly overstated. Switching would be a fidelity improvement, not something the bytes require.

## 00be06b0 -- ok=True, name proven=True, test can fail=True, issues=0

NAME: ?AddRequest@CEngineStaticMapFileBlockManager@@QAEXJJ_N@Z is in Ego_r (egor_pdb_names.tsv / egor_signatures.tsv) and FableWin (_fablewin_api.tsv, FableWin.exe.signatures.tsv). rebuild/manifest/functions.tsv and ghidra_out/engine_api.tsv map 0x00BE06B0 to CEngineStaticMapFileBlockManager::AddRequest as __thiscall void(long,long,bool). The source declaration matches the PDB spelling: public, non-virtual, void, (long,long,bool), ret 0Ch. xref.py lists 5 callers. The function is not virtual, so it has no vtable slot.
CALLEES (from xdis): 0xBE08E0 is Blocks.find (landed as the _Rb_tree<...CFileBlock...>::find<long> member template). 0xBE0BF0 is reached through Blocks.insert. 0x9D6100 is CBankFileAsyncEntry::BeginReadDataAutoAlloc and its PDB mangling gives exactly the declared signature (CCountedPointer<CBankFileAsyncData>, ulong, ulong, EThreadedFilePriority). 0x9D5630 is CBankFileAsyncData::ChangeLoadingPriority, whose PDB mangling matches the overlay declaration. 0xBFE9BC is operator delete, reached through the CCountedPointer releases. All trailing addresses are correct.
g_HighLoadingPriority 0x0139A728 is not in any PDB extract. It is the existing landed spelling, from reloc_bindings.tsv and the landed row 0x00BE0420 SetBlockToHighPriority, so it is used consistently.
Observation: the landed row for 0x00BE0BF0 is still spelled with the address-suffixed name _unattributed::InsertUnique_CRetailTree_00be0bf0. LABELS.tsv gives it the proper std::_Rb_tree<long,pair<const long,CFileBlock>,...>::insert_unique. That evidence holds: AddRequest's map::insert compiles to this call. The coordinator may want a matching NAME_FIXES row for that landed spelling. This row's source never spells that name, so nothing blocks landing.
MEMBERS (Ego_r struct_layouts and the overlay header):
- AsyncBankEntry: CCountedPointer<CBankFileAsyncEntry> at +0 (retail `mov ecx,[edi]`).
- Blocks: map at +8 (retail `lea ebx,[edi+8]`).
- CFileBlock.RefCount: long at +0, node +0x14 (retail `inc [esi+14h]`).
- CFileBlock.LowPriority: bool at +4, node +0x18.
- CFileBlock.LoadingData: CCountedPointer at +8, node +0x1C (retail `mov ecx,[esi+1Ch]`).
All match, and sizeof(manager) is 20.
LOCAL CLASS: the source declares CBankFileAsyncEntry locally with only BeginReadDataAutoAlloc, which is a PDB name. rebuild/include/engine/CBankFileAsyncEntry.h does not exist, in the repo or in the overlay.
REAL BODIES: the source defines no function body other than AddRequest. The insert_unique / _M_insert bodies come from the shared stlport/fable_map.h under the existing FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY opt-in.
TEST: it checks that a new low-priority block starts a BELOW_NORMAL read with RefCount 1 and the flag set. A repeat low request only bumps the count. A normal request on a low block raises the read to g_HighLoadingPriority once and leaves LowPriority set. A new normal block reads at HIGH. A normal request on a normal block makes no priority change. This exercises both branches and the guard, and it fails if any of them are wrong.
FAITHFULNESS: the source comment explains the two odd-looking constructs from the retail bytes. First, `data` is default-constructed and then assigned (`xor ebp/esi` before the call, then the ShareData null-Info test). Second, CFileBlock is aggregate-initialised (`test esi,esi; je; inc [esi]` copy construction). The three releases after the insert (pair, block, data) are visible at 0xBE077A to 0xBE07B7. The retail behaviour that keeps LowPriority unchanged after the raise is confirmed: nothing writes [esi+18h]. The overlay header additions are opt-in only and declaration-only.
NOTES.md says "no flags", meaning compiler flags. The STLP_MAP defines are header opt-ins that are explained in the source.

