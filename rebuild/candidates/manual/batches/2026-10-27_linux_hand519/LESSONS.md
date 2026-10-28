# Chunk 519 lessons

## GOTCHAS one-liners
- `mov eax,[..] ; test ; cmp [eax+4] ; mov ecx,eax ; call` = a conditional inline (`Object ? Object->IVRefCount : 0`, CEnginePrimitiveHandle::GetReferenceCount) feeding the test; the plain null test + member compare loads ecx directly (0x00B3A0D0).
- `vector::empty()` written as `_M_start == _M_finish` loads begin first; `begin() == end()` loads end first (0x00B84990).
- A dead 16-byte zeroed frame temporary after a reset = `member = C4DVector(0,0,0,0)` inside an inline Reset() (C4DVector has no user copy, the copy is folded, the temp stores stay) (0x00BE2190).
- ~CBaseClass is 0x0099A2E0; 0x0099A300 is ??1CBaseClassNonCopyable@@MAE@XZ (same bytes, own COMDAT): model CBaseClassNonCopyable with a protected out-of-line virtual dtor or derived ??_G rows bind the wrong symbol (vland RELOC_CONFLICT).
- "Ctor-looking" catalog rows labelled SetLocalizationMatrix at 0x00AF0880/0x00AFEAD0 are the CEnginePrimitive create-info ctors (type 0x20 / 0x1C); search struct_layouts_egor.tsv by member offsets + types to find the class.
- PDB class keys matter in the leaf: STLport `map`/`allocator` are V (class) in the PDB spelling, the landed 0x00A789F0 model uses struct (U).
- 2026-10-14 QFE-lane parks (hand31..hand59) mostly match at once under RTM 13.10.3077; re-try them before re-deriving.

## Review at landing
- 00a787d0: insert_unique 0x00A819B0 now has a third spelling (`?insert_unique@_Rep_type@?$map@...V?$allocator@...`); the landed 0x00A789F0 binding uses `U?$allocator` and 0x00A80C20 an invented CRetailTree; the real STLport symbol is `_Rb_tree<...>::insert_unique` -- unify.
- 00be0420: 0x00BE08E0 bound as the invented `CRetailBlockSet::FindInsertionPoint`; this row uses the real STLport `?find@?$_Rb_tree@J...` spelling. Global 0x0139A728 named descriptively (`g_HighLoadingPriority`).
- 00aa8400 / 00b84990: need `// Flags: /Gr`; 00abbd70 needs `/GS`.
- 00b80f90: ConvertVectorToLocal 0x00B80AD0 is protected `IBE` in Ego_r.pdb but bound `QBE`.
- 00bf3180: DeleteTree 0x00BF29E0 uses the PDB `AAEXPAUCNodeHeader@1@` spelling; the landed binding is the invented `?DeleteTree@CEngineTreeFilerBase@@QAEXPAUSTreeNode_00bf29e0@@@Z`.
- Globals with several existing spellings: g_pOcclusionManager (0x01436E5C), g_pEngineSimulationManager (0x01436E68), g_pShadowRenderer_01436e60, g_pEnginePrimitiveTypeInfo (0x01436E84), g_pEngineLightingManager (0x01436E9C).
- CBaseClassNonCopyable ctor declared protected (`IAE`, PDB) while some landed bindings use `QAE`.
- ??_G rows (00b84120, 00b8fb60, 00bb6520, 00bc7a80) define a real ctor in the TU purely to emit the vtable and ??_G.
- 00aa8400 RemoveAllSubChunks: bound `CChunkedFileSuperChunk_Retail` spelling is invented; this row uses the PDB name.
