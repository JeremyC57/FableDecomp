# chunkQ32 notes

All six rows RELOCATION_MATCH under `LANDVERIFY_STRICT_HEADERS=1` with `FABLE_HDR_OVERLAY=$S/w22/chunkQ32/hdr`; every behaviour
test PASS under VC7.1 (Wine cl /Od) and g++. Sources in src/<addr>.cpp, tests in tests/<addr>_test.cpp, payload.json / oracle.tsv
hold all six. The inherited Q29 drafts are kept in differ_inherited/ (superseded); differ/ is empty.

- 0x00EB3BA0 NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero (module = the nested class; the oracle's
  "CMurderTwistSheriff::Init" is the propagated label -- NAME_FIXES.tsv). Q29's lead was right; the missing fact: IsValid 0x00CD23B9
  sits unaligned among COMDATs, i.e. an inline member emitted out of line. Its real body (`return PImp.Data != 0;`, the retail
  `xor eax,eax ; cmp [ecx+8],eax ; setne al`) defined in the row TU as `inline __declspec(noinline)` + `seh_Guards[ID]` written twice
  matches: c2 uses the select-any COMDAT's mod/ref (CSE of &seh_Guards[ID] across the call, retail's lea) but not its register
  summary (ecx reloaded). Q29's plain non-inline noinline body gave 179 bytes (register summary used).
- 0x00BD2A90 CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings: identity from Ego_r/FableWin (QBE, const C3DMesh2&) and the
  caller (GetBoneScalings' miss path at 0x00B83798). Ported on CEngineInternalPrimitiveAnimatedMesh.h ORIGINAL_CACHE (Anim.StartState),
  C3DAnimationBlendState (HasSkeletalMorph / PeekSkeletalMorph, FableWin inline names), C3DMesh2 (GetBoneCount / GetBone / GetMeshID),
  CCountedPointer, CArray. Local classes (no shared header exists): C3DBone2 (Ego_r 60 bytes, GetNameCRC), C3DSkeletalMorph /
  C3DSkeletalMorphTarget (declarations of the landed PeekCombinedMorph 0x00A45260 / GetBoneScaling 0x00A44CD0). Last residue was a
  swapped frame-slot pair (key temp vs bone count); FABLE_RETAIL_STLP_MAP (STLport's visible in-class operator[], kept out of line)
  fixed it.
- 0x00958B20 CAIStateGroup_MinionReactToCrime::OnReplace: vtable slot 10 of 0x0128C580 (+ the FromSleep subclass vtable). Two grades
  after the port. Local class COpinionOfHeroDef (Ego_r 60 bytes; no shared header) for PDef->DeedMask +0x38.
- 0x00465C54 CCombatAbilityBlockDefBase::Transfer: chunk 1018's shared-header source + a new per-TU opt-in
  FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE (STLport's tree ctor/dtor bodies visible but `__declspec(noinline)`: retail calls
  0x00451ADD / 0x00451B07). Declaration-only gave 226v226 with the empty less/allocator temporaries and the cached vector end in
  their own frame words; the visible bodies let c2 home them in the dead `context` slot. Flags: /d2inlS39 (smallest S; S38 and
  below call IsSaving / the range constructors: 189 bytes) with `optimize("s")` + `("y", off)`. Hint for the coordinator: landed
  0x00451B07 is labelled std::list<T_00451b07>::~list but is called here as the set<EWeaponClass> tree destructor (same shape:
  clear + free the header node) -- not proven which spelling owns the address, so no NAME_FIXES row.
- 0x00722740 CTCSmokeGenerator::FrameUpdate: matched on the first compiling port. Out-of-line real body of the landed callee
  CDefClassBase::GetSubDef<CSmokeGeneratorDef> 0x00722D30 is defined in the TU under `#pragma auto_inline(off)` (copied from the
  landed row, as hand758 did). Local classes CTCSmokeGenerator / CSmokeGeneratorDef (no shared header exists).
- 0x009FC950 CGraphicFrameData::InitialiseAsSurface: on CGraphicFrame.h's CGraphicFrameData (STATICBLOCKS_TEXTURE_HELPERS) + CTexture.
  Retail compares Height with the texture's level-0 X as well as Width (both `mov eax,[eax]`); spelled with CTexture::GetWidth(0)
  twice (Ego_r inline name), so no wrong GetHeight body is invented. Width, Height, Pitch statement order fixed the last flag-merge
  register difference.

Made-up names: none. Out-of-line real bodies in row TUs: IsValid 0x00CD23B9 (00eb3ba0), GetSubDef<CSmokeGeneratorDef> 0x00722D30
(00722740, landed row). Probes used: 0 (all ideas tried on the rows).
