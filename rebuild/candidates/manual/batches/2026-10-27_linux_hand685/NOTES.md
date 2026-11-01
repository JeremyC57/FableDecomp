# Hand chunk 685 (AMovieSetupRegisterFilter .. CutsceneCameraPreloader): 8 of 10 byte-exact

Ten never-attempted rows, 477-936 bytes, every one carrying a stand-in `_global::` catalog label. Identity was settled
first (two sources each: PDB symbol + caller / vtable / literal / vendor text); eight match, two are parked with
instruction-identical or near-identical bodies. All eight tests PASS under g++ and VC7.1 (`tall.sh`, `tall.py`).

## What matched (8)
- **0x00CAA4A0 `_AMovieSetupRegisterFilter@12`** (DirectShow base classes `dllsetup.cpp`, MATCH, /EHsc): registers a filter
  and its pins/types through IFilterMapper, mapping the "not registered" HRESULT 0x80070002 of UnregisterFilter to NOERROR.
  SDK text verbatim with `__declspec(novtable)` interfaces; the catalog "FileArchive_ValidateAndLoad" is a stand-in.
- **0x00CA3B80 `_Laguerre_With_Deflation@12`** (libvorbis 1.0.1 `lsp.c`): polynomial root finder by Laguerre's method with
  deflation; vendor text verbatim as a `static __declspec(noinline) __stdcall` inside `extern "C"` with a same-TU caller
  (the Newton_Raphson recipe); `EPSILON 10e-7`, `alloca` -> `_alloca`.
- **0x00CA0BA0 `__01forward@24`** (libvorbis 1.0.1 `res0.c`, /Gr): the residue encoder core. Two things mattered: the
  C functions must sit inside `extern "C" { }` (else the symbol is the C++ `?_01forward@@YGH...` and the Leaf matches
  nothing) and the vendor call passes `accumulator` (NULL, `push 0`), not `look` (`push ebx`, 578v579).
- **0x00B179A0 `NParticleEngine::CPSCAttractor::Update`** (vtable 0x012A04AC slot 9): attractor force on each live
  particle inside the influence radius with the pow fall-off; `__forceinline` CVectorMap::Find (VC7.1 does not auto-inline
  a largish template member into a big function), PDB C3DVector operators, `(float)pow(ratio, exponent)`.
- **0x007748E0 `CTCShop::AddSaleItemPlacesFromThing(const CThing*)`**: recursion through CTCThingOwner's object list,
  then "GoodsPlace1".."GoodsPlace10" / "GOODS_PLACE_01".."10" dummy objects via GetDummyObject into GoodsPlaces. Shape
  lessons: same-TU bodies for the STLport string's `reserve` and `assign` (the only callees that receive the string's
  address) remove the dead constructor stores and keep start/finish in registers; `for (i = 0; i < 10; i++)` with
  `number = i + 1` gives retail's frame-homed counter pair; VC7.1 inlines only the second of four `push_back` calls on
  its own.
- **0x00C674B0 `CEaxContext::SetSlotLoadEffect(unsigned long, SGuid)`** (descriptive; libpk EAX 4.0 context, beside the
  landed SetSlotLock / SetSlotVolume / SetSlotFlags): loads EAX_REVERB_EFFECT (SDK default reverb, committed on the
  primary environment slot with the volume folded into lRoom and clamped) or EAX_NULL_GUID (unload: context air absorption
  / HF reference, lRoom -10000) into a slot; other GUIDs -> last error -4. Shape: `if / else if / else` with ONE shared
  `return 0`; member-wise `m_reverb[slot].x =` stores (no local record pointer); null arm order air, room, HF, dirty.
- **0x00CB5180 `ImeUi_Initialize(HWND, bool)`** (DXSDK 2004 ImeUi.cpp, /Gr): the pre-DXUT text: OS-version
  `bUnicodeImm` choice of W / WA_ entry points, imm32 private exports, once-flag around ImeUi_SetWindow + ImmGetContext,
  DrawRect / Malloc / Free / DrawFans callback test, SetImeApi() + ImeUi_SetSupportLevel(2) (both inlined from same-TU
  bodies). The `if (!IsWindowUnicode(hwnd))` ANSI-first arm order needed a second probe; a third
  (after vland's RELOC_IDENTITY) put the ANSI arm's two entry-point stores in retail's order (CandidateList first).
- **0x00AB1F30 `C3DAnimationBuilder::BuildBoneInfoTree`** (private, sequence-chunk overload; callers in GetIntersection
  0x00AB2160): one CBoneInfo per chunk, parented by index, roots collected, first root returned. The decisive lever
  (probe 6) was a same-TU body for the one callee that receives the counted pointer's ADDRESS (vector
  `_M_insert_overflow` 0x00AB3300): VC7.1 then keeps Data/Info in esi/edi across the CCharString calls instead of
  reloading the address-taken local; `roots` declared before `bones.clear()`; the parent bone in a named `CBoneInfo*`.

## Parked for Fable 5.1 (2)
- 00bec680 CEngineInternalPrimitiveMeshBase::CollectStats DIFFER(545v545): identical instruction stream; retail homes the six map-key temporaries and the operator[] result spill in the dead `statIndex` argument slot ([esp+0x24], frame 0x1c) where ours get frame words (frame 0x28) | tried: 17 probes -- block-scope keys, a function-scope unsigned long / long key, conversion temporaries (`long` bound to `const unsigned long&`), count-first vs operator[]-first evaluation, inline PeekTextureBank accessor, same-TU dtor body | next: a form where the enum argument dies before the first temporary (switch on a copied local, or a by-value helper whose parameter slot is the enum's) so VC7.1 reuses the slot; else lane gap: dead-argument-slot homing of successive temporaries.
- 00cbf29f CutsceneCameraPreloader DIFFER(930v936): `sub esp,0x2c` vs 0x30; retail keeps the scaled line index j*4 in edi through the StartTimeCode test and the four-way `||` chain and materialises the "" address 0x0129AAF4 only inside the chain (`mov ebx,G` in both arms of its first diamond); ours extends the "" web back into the StartTimeCode test (edi) and homes j*4 in a frame word (one store, two reloads) | tried: 12 probes -- second helper / second accessor copy with their own "" literals (VC pools literals: no split), "" as an extern global, named chain bool, `CCharString& line` reference (worse), inlined per-line helper, `_M_start[i]`, /O1 /Oy- flags, `== 0` helper, declaration order (fixed the entry stores) | next: lane gap: allocator-level web split (likely retail LTCG); or compile the first prefix test as an out-of-line static with a same-TU body.

## Skipped
None.

## Label corrections
- 0x007748E0 is `?AddSaleItemPlacesFromThing@CTCShop@@QAEXPBVCThing@@@Z` (Ego_r.pdb); the catalog
  "CTCGraphicAppearance_InitGoodsPlaces" and the landed GetFreeSaleItemPlaces 0x00774B40 / push_back 0x00778210 notes
  ("CTCShop::InitGoodsPlaces") are wrong spellings (NAME_FIXES).
- 0x00AB1F30 is the private `?BuildBoneInfoTree@C3DAnimationBuilder@@AAE...` (catalog "GetIntersection_CollectEntities");
  its callees 0x00AB3640 (`CCountedPointer<CBoneInfo>::DeleteData`, catalog "CTCMapwho::OnDie"), 0x00AB1480 (STLport
  `__copy` for CCountedPointer<CBoneInfo>*, catalog `_Move<...CGuiVarTransferBase>`), 0x00AB2BD0 (`__destroy_aux`, catalog
  `_Destroy_range<...CEntityScriptBindingBase>`) are propagated COMDAT twins.
- 0x00BEC680 is the static `?CollectStats@CEngineInternalPrimitiveMeshBase@@SIX...`; the landed callers 0x00BEC8B0 /
  0x00B98DE0 call it "Inner_CollectStats" / "SubMesh_CollectStats"; 0x00A36EC0 is `??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ`
  (catalog `_Dest_val<...COptimisedPrimitive>`); 0x00A89320 / 0x00A89350 are `C3DMesh2::GetVertexCount / GetTriangleCount`;
  0x009CC240 is `CBankFile::GetEntryDataSize`.
- 0x00C674B0: the landed 0x00C689B0 ("CSkillTable_00c67470::SetSkillPropertiesEx", calling "SetSkillGUID") is the CEaxContext
  slot-property dispatcher calling SetSlotLoadEffect; 0x012B9438 is EAX_NULL_GUID.
- 0x00CB5180 is `ImeUi_Initialize` (the landed CIME::Initialise 0x00405650 spells it "ImeUI_Initialize"); 0x0143CDFC is
  `ImeUiCallback_DrawFans` ("g_ImeDrawFans" there); 0x00CB2DD0 (unlanded) is `?ImeUi_SetSupportLevel@@YIXK@Z`.
- 0x0099F560 is `?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ` (landed as global_Destval_0099f560.cpp "Dest_val", labels
  tsv "CCharString::Trim").
- 0x00CA0BA0 / 0x00CA3B80 / 0x00CAA4A0 are vendor functions (libvorbis `_01forward`, `Laguerre_With_Deflation`, strmbase
  `AMovieSetupRegisterFilter`), not engine code; 0x00B179A0 is `NParticleEngine::CPSCAttractor::Update`.

## Descriptive names (no PDB)
- CEaxContext::SetSlotLoadEffect (0x00C674B0) and its parameters; the ImeUi once flag 0x0143E8DD (`bImcObtained`, a
  function static); `SendCompString_Ansi` for 0x00CB2230 (the Unicode twin 0x00CB25F0 is the PDB's `SendCompString`);
  CutsceneCameraPreloader's parameters and its inline prefix helper `LineStartsWith`.

## Header debt
- engine/CCountedPointer.h builds the count block from `(void*)Data` (re-read after operator new); retail passes the
  constructor argument (`mov [eax+8],esi`) -- 0x00AB1F30 models CCountedPointer locally.
- engine/C3DMeshInfo.h names the bank pointer `f4`; it is `CMeshDataBank* Bank` (+4) with the inline `PeekTextureBank()`.
- CTCShop::GoodsPlaces is +0x28 in retail (Ego_r +0x2c); C3DAnimationBuilder::CBoneInfo is 0x24 in retail (Ego_r 0x28);
  C3DAnimFileSequenceChunk's BoneIndex / ParentIndex / BoneName are Ego_r minus 8 (the super chunk's CArrays are 12 bytes).
- The shared engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) carries the five slots the cutscene row uses (7, 13, 15,
  403, 412) with the right signatures.
