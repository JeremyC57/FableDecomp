# Chunk 674 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU STLport list bodies (`_M_insert_dispatch` / `insert` / `_M_create_node`) let VC7.1 drop a dead local list object: `this` is passed as a dummy address (0x005FD410).
- STLport `_List_base` ctor in the `_Node* __n = allocate(1)` form, plus a copy ctor written `insert(begin(), x.begin(), x.end())`, gives retail's `mov edx,edi` node handling (0x005FD410).
- `__forceinline unsigned long HasTC` returning a masked word gives `test ch,10h` (0x005FD410).
- A named `CThing* thing = ParentThing;` local fixes prologue register choice and scheduling (0x005FD410).
- VC7.1 drops FreeVirtual's inlined `if (block)` null test after a virtual-dtor call, but keeps it after a non-virtual inline dtor (0x00BD9CE0, DeleteTexture vs DeleteVertexBuffer/IndexBuffer).
- zlib in Fable.exe is 1.2.1: `deflate_state.data_type` is a Byte at +0x1C (`cmp byte [esi+1Ch],2`). zlib 1.2.2 moved it to `strm->data_type` (0x00C057D0).
- A `.text$yc` CRT initializer at a packed (unaligned) address comes from a /Os TU. `#pragma optimize("s", on)` turned DIFFER(182v177) into a match (0x0121A7CA).
- A DECLARE_IUNKNOWN forwarder of a base filter is emitted only when some derived class's vtable is emitted. Define that class's ctor with an empty body (0x00CA62D0).
- A `__fastcall` register-parameter home (`[ebp-14h]` for edx) is not shared with temporaries. A temporary copy of the key shares the slot (frame 0x3C), but the store is scheduled after `mov edi,ecx` (0x00CBF7FE, parked).

Review at landing:
- 0x00CBF7FE: the catalog row holds two functions. PlayCutsceneSound is 465 bytes; GetScriptActiveStatus 0x00CBF9CF is 15 bytes, no callers. Split the row; the parked source already emits both, and the tail bytes match.
- The landed zlib trees.c TU (`global_trstoredblock_00c05530.cpp` and siblings) uses zlib 1.2.2's `s->strm->data_type`. Retail is 1.2.1 (`s->data_type`). The c674 0x00C057D0 source restores it; check the other landed trees.c rows still match after aligning the TU.
- 0x01228ED8 is landed as `CDebugManager_ThrowException_01228ed8.cpp` (FableThrow_1228ed8). It is CrimeText's dynamic atexit destructor and should be respelled/remodelled.
- 0x00F29DA0 is landed as `CLandscapeBackgroundPatch_vectordeletingdestructor_00f29da0.cpp`, calling `CLandscapeBackgroundPatch_dtor_00f29dc0`. It is ??_GCCutsceneDef; respell it to bind `??1CCutsceneDef@@UAE@XZ`.
- 0x00CBFAB8 is landed as `global_SetMiniMapRegionExitTextOffsetX_00cbfab8.cpp`. It is SetScriptActiveStatus. Respell its callers and the 0x0143E910 map binding (`g_UnprovenMap_0143E910` -> ScriptActiveStatuses).
- Local base models next to engine headers: CCutsceneDef uses a local packed CParentDefClassBase. CEngineLandscapeMemoryManager is local, because its list bodies must be in the same TU.
- 0x00CA5200 / 0x00CA62D0 lean on the landed DirectShow model (no engine header).
