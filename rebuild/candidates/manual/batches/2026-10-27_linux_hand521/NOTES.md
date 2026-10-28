# Chunk 521 notes (20 re-queued rows, 0x00433DF0 .. 0x004F9C07)

Result: 10 RELOCATION_MATCH (no MATCH, no retail_asm), 10 DIFFER parked, 0 SKIP. Every matched row passes its
test under g++ and VC7.1 (tall.sh / tall.py). vland dry run: 10 wins, no SHARED_HEADER / RELOC_CONFLICT /
RELOC_DUPLICATE warnings. The only warnings are RELOC_NAME, and each one is a catalog label proven wrong (see Label corrections).

## Names

### Proven
- 0x00433DF0 std::vector<CWideString>::operator= -- the Ego_r/FableWin.pdb name set holds `??4?$vector@VCWideString@@...QAEAAV01@ABV01@@Z`; the body is the STLport 4.6 _vector.c operator= (CWideString copy ctor / dtor / operator= callees); callers assign CDefinitionManager compile / symbol path lists.
- 0x0044A1A0 CPlayerManager::CreatePlayer(long) -- Ego_r `?CreatePlayer@CPlayerManager@@AAEXJ@Z` (private); the body is IsPlayer guard + new CPlayer (0x22C) + Players.push_back + PlayerInit with the Ego_r CPlayerInit members at PDB offsets.
- 0x004649A3 std::vector<unsigned long>::_M_fill_insert -- the STLport POD _M_fill_insert body (memmove, rep stosd); its callees are the landed __uninitialized_copy<ulong*> 0x00464AAB and _M_insert_overflow 0x00464ACE; same recipe as the landed vector<bool> 0x004676EF.
- 0x0046C18C std::vector<CReverbEnvironmentEntry>::_M_insert_overflow(__false_type) -- the STLport body for the 84-byte element (`push 15h; pop ecx; rep movsd`); sole caller _M_fill_insert 0x0046C022.
- 0x004701CE std::vector<CAppearanceModifierGraphics::CEntry>::_M_insert_overflow(__false_type) -- the STLport body for the 24-byte element; sole caller _M_fill_insert 0x0047005B (landed, correct spelling).
- 0x0048F939 CSubtitleRenderer::InitPlayerMode() -- Ego_r `?InitPlayerMode@CSubtitleRenderer@@AAEXXZ`; it reads and writes PlayerModeAdded / DisplayFlags / Button1Text / Button2Text at PDB offsets + 0x10 (STLport deque); its callees are CPlayer::AddMode / RemoveMode and GetTextFromTextTag with TEXT_GUI_NEXT / CONFIRM / CANCEL.
- 0x004BBA40 std::__introsort_loop<pair<CCharString,CPersistInfo>*, ...> -- the STLport _algo.c body; the landed sort 0x004BBB40 declares this exact callee; its callees are the landed __median / __unguarded_partition / __partial_sort over the same element.
- 0x004BD040 NGraphicAppearance::CTCGraphicAppearance::GetTintColour() const -- Ego_r / FableWin.pdb `?GetTintColour@...QBE?BVCRGBColour@@XZ`; it walks TintColours (map<void*,CRGBColour>, +0x18) and averages; caller OnAppearanceDraw 0x0077BA40.
- 0x004D16A0 std::sort_heap<pair<float,CSubtitledMessage::CCameraTag>*, CKeyPairCompareLess<...>> -- the STLport _heap.h body (28-byte rep movsd); it calls __adjust_heap 0x004D0660 over the same element; the existing binding already used this spelling.
- 0x004EC8C2 GFSerialiseMapBinaryIn<CObjectAugmentationParticleSet, EObjectAugmentationType, less<>> -- the exact mangled name is in Ego_r and FableWin.pdb; its only caller is 0x004EC851, whose sibling save arm is the landed GFSerialiseMapBinaryOut 0x004EC85A; its callees are the PDB-named traits readers / ParticleSet ctor and dtor.
- 0x00476024 / 0x0047652B / 0x004769D0 / 0x00478AE5 / 0x00479466 / 0x004798AB / 0x00479EF4 / 0x0047A2F5 / 0x004F9C07 CPersistContext::TransferVectorOfSubComponents<T> -- the Ego_r/FableWin template name. Each T comes from three things: the size() divisor / stride, the default element's vtable (RTTI), and the out-of-line resize / ctor / dtor callees. The elements in order are CQuickAccessItemButtonGuiDef, CAbilityButtonGuiDef, CLensFlareElementDef, CAbilityTrackLevelDef, COpinionDeedReactionDef, CEngineLocalDetailLayerDef, CAppearanceModifierScalingDef, CAbilityTrackDef and CCardPositionsDef. (These rows are parked; the names are still proven.)
- 0x0049ED80 CWorld::ConsoleSetFreeCameraLookVector -- Ego_r `?ConsoleSetFreeCameraLookVector@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@...@Z` (protected static fastcall); the sibling ConsoleSetFreeCameraPos 0x0049ED30 is the body just before it with the same prologue; it ends in CCamera::SetView on CurrentFreeCamera. (Parked.)
- Callees proven along the way (see NAME_FIXES.tsv): 0x00433D8A, 0x0099B7D0, 0x0099B720, 0x0099B510, 0x00449880, 0x004497E0, 0x00416359, 0x0044BE80, 0x00464ACE, 0x0047955B, 0x004F9CFC, 0x00478722, 0x00430370, 0x00430300, 0x00476123, 0x00478BE4, 0x004F9D32, 0x004EC9B5, 0x004E9642, 0x004E25F1, 0x00452D8E, 0x00404280, 0x0048F6BD, 0x006345C0, 0x00634190, 0x00449970, 0x00A0BE30, 0x004D0660.

### Probable
- 0x004E8212 std::_Rb_tree<EObjectAugmentationType, pair<const K,V>, ...>::clear -- this is what `map.clear()` resolves to once STLport's map::clear forwarder inlines; the out-of-line symbol could be map::clear instead (both relocate the same call).
- 0x004EC9D3 std::_Rb_tree<...>::insert_unique(const value_type&) -- the same forwarder argument for map::insert.

### Descriptive
- 0x013B86A0 g_pMainGameComponent -- the global CMainGameComponent pointer (no PDB global name dumped); the landed majority spelling is kept.
- 0x0129BA3C g_epsilon_0129ba3c -- the shared 1.0e-4f tolerance used by 0x0049ED80 (no PDB name); the landed majority spelling is kept.
- 0x013CE558 g_InvSqrtMantissaTable -- the fast inverse-sqrt mantissa table (landed spelling).
- EPlayerMode values 0x0E / 0x0F / 0x1A / 0x27 / 0x2A in 0x0048F939 are written as numbers; the enumerator names were not recovered.
- CAppearanceModifierGraphics::CEntry members in 0x004701CE are Word0..Word5 (the 24-byte record, member names not in the layout TSV).

## Label corrections (catalog -> evidence)
- 0x00433DF0 "Array_AssignFrom" -> std::vector<CWideString>::operator=.
- 0x0044A1A0 "CPlayerManager_AddPlayerOrAI" -> CPlayerManager::CreatePlayer.
- 0x004649A3 "std::vector_insert_range_undefined4" -> vector<unsigned long>::_M_fill_insert.
- 0x0046C18C "std::vector_insert_reallocate_84byte" / 0x004701CE "..._24byte" -> _M_insert_overflow (IAE, with the allocator).
- 0x00476024 .. 0x004F9C07 (9 rows) "TransferVectorOfSubComponents<CAbilityButtonGuiDef>" is propagated across the family; only 0x0047652B is really CAbilityButtonGuiDef.
- 0x0048F939 "CSubtitleRenderer_ProcessPlayerModes" -> CSubtitleRenderer::InitPlayerMode.
- 0x0049ED80 "_global::ConsoleSetFreeCameraLookVector" is missing its class: it is CWorld (protected static).
- 0x004BBA40 "std::introsort_Recursive" -> std::__introsort_loop<...>.
- 0x004BD040 "Appearance_FloatToCoord" is wrong -> CTCGraphicAppearance::GetTintColour.
- 0x004D16A0 "_Sort_heap<CDebugCone*>" has the wrong element (Dinkumware spelling) -> sort_heap<pair<float,CCameraTag>*>.
- 0x004EC8C2 "GFSerialiseMapBinaryIn<CCharString,long,...>" has the wrong K and V -> <CObjectAugmentationParticleSet, EObjectAugmentationType>.
- RELOC_NAME warnings from vland: 0x00433D8A (labelled _Uninit_copy<CArray<CRandomAppearanceMorph::CBodyPartMesh>...>), 0x00416359 (GetGamePlayerInterface; PDB GetPPlayerInterface), 0x00404280 (WideString_EqualsLen), 0x004BBA40 (introsort_Recursive) and 0x004EC9B5 (CSoundInit::CSoundInit; it is the pair<K,V> default ctor).
- 0x00452D8E landed as an unattributed stdcall `TransferBinaryIn_00452d8e`; it is CPersistTraits<EObjectAugmentationType>::TransferBinaryIn (thiscall, AAV/AAW4).

## Header debt
- engine/CMemoryDataInputStream.h is a flat 0x1C layout with invented member names (position / chunkBase). It does not derive from CDataInputStream, and CDataInputStream.h has no methods. 0x004EC8C2 and the 9 CPersistContext rows therefore keep a local Ego_r CDataInputStream model with its inline Read / ReadType / ReadSLONG.
- engine/CPlayerManager.h keeps Players (vector<CPlayer*>) opaque; 0x0044A1A0 casts it at each use.
- engine/CSubtitleRenderer.h keeps Button1Text / Button2Text as raw data pointers and PlayerModeAdded as uchar[4]; 0x0048F939 casts them at each use.
- engine/CPlayer.h keeps CurrentFreeCamera opaque (0x0049ED80).
- No engine header exists for CTCGraphicAppearance, CCamera, CMainGameComponent, CPersistContext methods, CReverbEnvironmentEntry or CAppearanceModifierGraphics::CEntry; local models are used, with Ego_r names and retail offsets.
- The shared-header define trick (`#define X XLayout` / include / `#undef X` / `class X : public XLayout { methods }`) is what lets a row carry methods on a header-only layout; a generator flag that emits method-free `XLayout` names would make it unnecessary.

## Parked rows
- 0x00476024 CPersistContext::TransferVectorOfSubComponents<CQuickAccessItemButtonGuiDef> DIFFER(254v255): the inline CDataInputStream::Read of the load count computes the new position into eax with the high dword in edx (`add eax,4 ; xor edx,edx`); retail uses `lea edx,[eax+4] ; xor eax,eax ; ... cmp edx,7FFFFFFFh`, one byte longer. This is a register-allocation tie. | tried: helper vs inline IsValidPosition, `||` vs `!(&&)`, unsigned / __int64 casts, long members, y off, ReadType vs ReadLong, named count, if vs switch, private / public overflow, flat element, simple vector, base-typed stream pointer, QFE-4035 compiler, a GetPosition virtual, `remaining` local, member helper, CanRead helper (3 grades + many probes) | next: the same residue as the lane parks 00476E3E / 00477CD2; bisecting showed the lea form appears with a 0x40 element (`sar`) and goes away with the `idiv` element sizes, so look for the real CDataInputStream::Read text (FableWin.pdb line info / an Ego_d inline) rather than more forms.
- 0x0047652B CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef> DIFFER(254v255): same inline Read residue. | tried: generated from the shared gen_xvsc.py template (1 grade) | next: as 0x00476024.
- 0x004769D0 CPersistContext::TransferVectorOfSubComponents<CLensFlareElementDef> DIFFER(254v255): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x00478AE5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackLevelDef> DIFFER(254v255): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x00479466 CPersistContext::TransferVectorOfSubComponents<COpinionDeedReactionDef> DIFFER(244v245): same (variant B, inline element ctor / dtor). | tried: shared template, 2 grades | next: as 0x00476024.
- 0x004798AB CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef> DIFFER(244v245): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x00479EF4 CPersistContext::TransferVectorOfSubComponents<CAppearanceModifierScalingDef> DIFFER(254v255): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x0047A2F5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackDef> DIFFER(244v245): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x004F9C07 CPersistContext::TransferVectorOfSubComponents<CCardPositionsDef> DIFFER(244v245): same. | tried: shared template (1 grade) | next: as 0x00476024.
- 0x0049ED80 CWorld::ConsoleSetFreeCameraLookVector DIFFER(310v310): x87 operand order in the cross product only. Retail forms up's middle term as `fld right.X ; fmul look.Z` and the last as `fxch st(1) ; fmulp st(2)`; ours gives `fld look.Z ; fmul right.X` / `fxch st(2) ; fmul st(1)`. The lengths and the rest of the body are exact. | tried: all 64 operand orders of operator^ (all compile the same), `r = Y*Y; r += X*X` length form (fixed the length), C2DVector side vector (1 grade) | next: the operand order is probably set by the source of the real C3DVector::operator^ / CrossProduct (a temporary-based or member-wise form, not just the order of the factors); try a `C3DVector result; result.X = ...;` body with named temporaries, or the Ego_r inline from FableWin.pdb line info.
