# bwf1 author session (2026-11-12) -- row 0x00BCC800, in progress
- Ported: local CEngineTreeFilerBase / CEngineVertexBufferPool models moved into hdr/ overlay opt-ins
  FABLE_RETAIL_CENGINETREEFILER_INLINES (engine/CEngineTreeFiler.h) and FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
  (engine/CEngineVertexBufferPool.h); CListHeader Head/Tail; TrailType = CEnginePrimitiveWeaponTrail::EWeaponTrailType. Grade unchanged
  (DIFFER 3207v3205, 968/1078 exact, 1043 frame-masked) -> differ/00bcc800.cpp.
- KEY FINDING: this TU holds unreferenced out-of-line copies of the pool/tree inlines = small oracles for the helper texts:
  FindNode 0x00BC9B30, FindID 0x00BC9B80 (matches), AddKey 0x00BC9FB0, ReturnResource 0x00BCA140, GetResource 0x00BCA210,
  DrawBuffersNonIndexed(6-arg) 0x00BCA400, GetBuffer 0x00BCA510. Probe harness: $W/probe/pr.sh <probe.cpp> <leaf> <va> <len>
  (HDR=<overlay dir> to use a modified overlay; gbv.sh = GetBuffer variant runner). Our texts do NOT reproduce them:
  * GetBuffer 0x00BCA510: 136/153, only scratch-register rotation (retail edx/eax/ecx where ours ecx/edx/eax): the FindNode-push
    `lea edx,[key]` (= the row's 0x00BCCDA9 residue), AddNewKey push, BuildNewBuffer arg loads, `header->Head = resource->PoolNext`.
    NOT moved by: FindNode/FindID/AddKey text forms (ternary, named index, if(node){}, manual first level, FindNode body-less), GetKey
    operand order, clamp spellings, size/clamped locals, GetResource forms, AddNewResource unrolled, /G5 /G6 /G7 /Ob1 /arch, dummy fns.
  * AddKey 0x00BC9FB0: `id = FindID(key); if (id == 0) id = AddNewKey(key); return id;` puts node in the dead param slot like retail
    (form `if (id) return id; return AddNewKey(key);` keeps it in eax), BUT FableWin /Od size (53b) says the latter form; still this/key
    swapped (retail key=ebx, this=ebp).
  * ReturnResource 0x00BCA140: only tree/key esi<->edi swap. GetResource 0x00BCA210: retail keeps only tree (ebp), `this` spilled;
    ours this=ebp + tree=edi. A GetEntry body without the GetGenericObject call (or ternary GetGenericObject) gives retail's prologue
    but breaks the rest (and FableWin sizes say the current GetEntry/GetGenericObject texts are right).
- checkpoint (~20 row grades + ~120 probes): best still DIFFER(3207v3205) 968/1078. Proven: removing ReturnResource (diagnostic) puts the
  GFFloatToLongFloor `one`/vertex_count slot BELOW fraction/key as in retail -> the frame permutation is caused by the tail's node spill
  sharing that slot; fix the ReturnResource tail allocation and the frame order should follow. Header-text sweeps (rv.sh: AddKey forms,
  FindID if(node), FindNode ternary/early-return/named index, 1UL mask, IsBusy compare forms, GetResource local, member order, clamp forms)
  leave the row at 968 or worse.
- Hand-back (session end): best = differ/00bcc800.cpp (ported v7 + `// Retail-asm:` lines for 0x00BCC96A..92 / 0x00BCCF70..79 + callee line),
  DIFFER(3207v3205), 968/1078 exact, 1043 frame-masked. Residue: (1) frame permutation, (2) `lea edx` v `lea ecx` at 0x00BCCDA9,
  (3) ReturnResource tail 0x00BCD32D..0x00BCD3DF allocation. (1) follows (3) (diagnostic: without ReturnResource the `one`/vertex_count
  slot drops below fraction/key as in retail). (3) is an allocation equilibrium: diagnostics show removing FlushRender, or only its
  `TextureStagesInUse = 0` store, flips the tail toward retail (tree spilled to [ebp+7Ch], node in [ebp+7Ch]); spellings of the FlushRender
  loop (int/unsigned stage, while, `!= 0`, reference, assign-in-arg) do not.
- Tried without effect on the row (all 968 or worse): tangents via At(); vertex_count declared first / no segments local; skip before
  trail_size; AddKey forms; FindID `if (node) {}`; FindNode ternary / early return / named index / 1UL mask; GetGenericObject forms F2/F6/F7
  (FableWin-size 37b); GetEntry direct / ternary GetGenericObject (diag, 961); IsBusy compare spellings; GetResource local resource;
  member order; clamp spellings; pool reference locals; vertex_buffer reference for the draw; flags /Oa /GS /G7.
- Best next ideas: (a) find the source fact that makes the out-of-line copies match -- GetBuffer 0x00BCA510 (scratch rotation, 136/153),
  AddKey 0x00BC9FB0, ReturnResource 0x00BCA140, GetResource 0x00BCA210 -- with $W/probe/pr.sh + gbv.sh (4 s per try); everything inside
  GetBuffer's own text was ruled out (even GetResource / BuildNewBuffer / GetKey removed, FindNode body-less or static: the FindNode-push
  lea stays ecx), so look at declarations: CVertexBufferWin32 / CVertexBufferBase layout and virtuals, CEngineDynamic3DResource base
  (maybe a second base or a ctor/dtor), CEngineDynamic3DResourceManager's other members (FableWin lists ReleaseResources, a dtor), operator new
  declaration, EVertexType / EVertexBufferPool enum underlying types; (b) the row's CRenderManagerCore / CShaderRenderManager member types
  used by FlushRender and DrawBuffersNonIndexed (the tail equilibrium moves when that code changes); (c) FableWin says AddKey is the
  `if (id) return id; return AddNewKey(key);` form (53b); the overlay now has the `id = AddNewKey` form (row grade identical) -- pick by evidence.
