# NF45 lessons (one line each)
- A tracked-state stand-in that reads one bookmark mask into a named local first (SetStateConst / MaskFirst / StateMaskFirst) is the register-tie signature: 5 of 5 such rows kept a one-site register swap on the shared SetRenderState. Check the original for that helper before porting, and stop at 2 grades.
- Spell `g_renderStateDefault_*` / `g_renderStateValue_*` reads through the D3D*Table enumerator. Take the table bases from reloc_bindings.tsv and the DeviceState offsets from the header (work/rsmap.py); never use hand tables.
- An open-capture / RestoreCaptureBlock pair on 0x013CB508 that brackets a scope, including early-return exits, is CRenderStateScopedCaptureBlock (repo SCOPED_FORCEINLINE). A store of Open = 0 before the restore is a member CRenderStateCaptureBlock with INLINE + RESTORE_OPEN_FIRST.
- Include engine/CRenderManager.h before anything else that reaches CRenderManagerCore.h or CRenderStateManager.h. Include CVolumeTexture.h before it when the row uses VOLUMETEXTURE_INLINE.
- CDisplayManager.h needs FABLE_RETAIL_SCOPED_PTR / CSYSTEMMANAGER / CTEXTURE / CPIXELFORMAT for its SDK types (HWND, D3DCAPS9).
- Local "views" of a GP global often mislabel the member. Take the name from the class header at that offset: 0x01436E9C +0xD0 is MistEffectColour, not a mist position; 0x01436EA0 +0xE4 is InverseViewTransform.
- Resolve a short `// Leaf:` to its mangled name (strings on the object) before running xref or callers. A short leaf can match a callee and produce false NOREL probe hits.
- A June/July row on candidates/*_compiled.h with stand-in helper bodies has no binder line to respell. Mark it blocked/authoring with absrefs counts and one honest compile.
- Write APPLIED.tsv as each entry closes, not at the end (I wrote it at the end this round).
