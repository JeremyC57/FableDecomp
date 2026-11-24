# NF249 overlay headers

Base copies: `orig/engine/` (the repo working tree, 2026-11-22, nf247 / nf244 landed). Every change is inside an `#ifdef` opt-in.

## engine/CThingCreatureBase.h (q12690): `FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON` declares UnsheatheWeapon protected
- The NF247 opt-in block (one definer before this hand-back: row 0x00664A10; grep of rebuild/src, rebuild/tests, rebuild/include) declared
  `void UnsheatheWeapon(long, bool);` public. It now reads `protected: void UnsheatheWeapon(long weapon_def, bool primary); public:` on the
  same line, and its four comment lines are reworded in place: the file keeps 808 lines and no line below the block moves, so no
  `__LINE__`-named assert changes (pin_asserts: pinned 0; assert_names_check: 0 mismatches).
- Evidence: ?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z in Ego_r.pdb (0x00540710; bsim pairs it with retail 0x006648D0), FableWin.pdb
  (0x01D94EA0, 18 records), Ego_d.pdb (18 records); no PDB has a QAE spelling. The self row 0x006648D0 and binders 0x006649C0 / 0x00664A50 move
  to IAE in the same hand-back (rows/), so the address keeps one spelling.
- The `public:` after the declaration restores the region's access (the region is public from line 366; the next block, SETHANDEDNESS,
  declares public members).
- Includers: 996 landed rows (tools/incl.py: rows whose source or test includes CThingCreatureBase.h or one of the 15 headers that include
  it, generated from today's tree; work/eq2_addrs.txt). equiv_check repo tree v this overlay (work/equiv2.tsv): 3 DIFF = 0x00664A10 (the
  macro's definer: intended) and 0x0093AB70 / 0x00839420, which are `__FILE__` in GFDebugPDynamicCast.h expanding to the overlay path; rerun with
  an empty old tree (both sides resolve that header through the repo path) they are identical (work/equiv1e.tsv empty, NF247's finding).
- Users after this hand-back: rows 0x00664A10 and 0x00890D80 (q12691, which does not call UnsheatheWeapon).

## q12692: engine/CEngine.h, engine/CIEngine.h, engine/CEngineVideoOptions.h (add-only opt-ins; no landed file defines any of them)
| Header | Opt-in | What it adds | Evidence |
|---|---|---|---|
| CEngine.h | `FABLE_RETAIL_CENGINE_TYPED_MEMBERS` | before the class: CArray.h / CCamera.h / CEngineVideoOptions.h / retail CSimpleMemoryPool.h (it defines `FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR` and `FABLE_RETAIL_CSIMPLEMEMORYPOOL` if unset); in the class: the nested `CEngine::CPreloadStats` (4 members, inline ctor) and the seven members typed, each as `#ifdef TYPED <typed> #else <the existing line verbatim> #endif`: PreloadCameras CArray<CCamera> +0x3C, PreloadStats +0x48, VideoOptions CEngineVideoOptions +0xC0, FrameMemoryPool CSimpleMemoryPool +0x130, ScratchZBuffersD24S8 CArray<CSurface*> +0x14C, RenderLayers CArray<CEngineLayerRenderer*> +0x15C, EngineComponents CArray<CEngineComponent*> +0x168; GetFrameMemoryPool returns &FrameMemoryPool under it; seven new offset / size asserts inside an `#ifdef` | Ego_r.pdb CEngine member records (struct_layouts_egor.tsv: names, types, order); CPreloadStats members Ego_r.pdb, ctor ??0CPreloadStats@CEngine@@QAE@XZ FableWin.pdb + Ego_d.pdb; the retail ctor 0x00B260B0 / dtor 0x00B261D0 bodies (member order, 0.0 from 0x0122ED70, the inline pool dtor) |
| CIEngine.h | `FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE` | slot 1 is not redeclared (`#ifndef` around the existing two lines), so it binds ?Validate@CInitBaseClass@@UBEXXZ from CInitBaseClass.h (the includer defines FABLE_RETAIL_CINITBASECLASS_VALIDATE) | retail vtable 0x012A0F3C slot 1 = 0x0099A340; Ego_r / FableWin / Ego_d have ?Validate@CInitBaseClass@@UBEXXZ and no CIEngine::Validate |
| CIEngine.h | `FABLE_RETAIL_CIENGINE_CTOR` | protected inline `CIEngine() : Active(false) { }` | FableWin.pdb / Ego_d.pdb ??0CIEngine@@IAE@XZ; retail 0x00B260C1 `mov byte ptr [esi+8],0` after the CInitBaseClass ctor |
| CIEngine.h | `FABLE_RETAIL_CIENGINE_INLINE_DTOR` | `virtual ~CIEngine() { }` (`#ifdef` / `#else` around the existing declaration) | ??_GCIEngine 0x00B23770 and ~CEngine 0x00B261D0 both expand it (vtable store 0x012A0D24, then ~CInitBaseClass 0x0099A410); FableWin / Ego_d ??1CIEngine@@UAE@XZ |
| CEngineVideoOptions.h | `FABLE_RETAIL_CENGINEVIDEOOPTIONS_CTOR` | the class is also visible under it (`#ifdef X` -> `#if defined(X) \|\| defined(NEW)`), plus the inline default ctor | FableWin.pdb / Ego_d.pdb ??0CEngineVideoOptions@@QAE@XZ; retail 0x00B260F6..0x00B26145 stores (0 x 0, 4.0f x 3.0f, sync on, five NORMAL, 0 / 0 / 0) |
- pin_asserts: CEngine.h / CIEngine.h / CEngineVideoOptions.h pinned 0 (their existing asserts are all `_AT` already); assert_names_check 0 mismatches.
- Includers (tools/incl.py over today's tree, work/eq3_addrs.txt): 151 rows whose source or test includes one of the three headers (directly or via
  rebuild/include); NF244 cited a 159-row CIEngine.h closure, which also counted rows that include another row's .cpp. equiv_check repo tree v this
  overlay: 0 differing (work/equiv3.tsv empty). The basis for the rest: every change is under a macro no landed file defines (grep), so no
  includer's preprocessed text can change except through line numbers, and every existing assert in the three headers is already `_AT`.
- The seven new asserts inside the TYPED_MEMBERS block are plain FABLE_STATIC_ASSERT (pin_asserts lists them as new / unpinned): they exist only for
  the macro's definers, so no landed typedef name changes.
- Users after this hand-back: rows 0x00B260B0, 0x00B261D0 (both define TYPED_MEMBERS, OVERRIDES, the three CIEngine opt-ins, CIENGINE_RENDER, CINITBASECLASS_VALIDATE).
