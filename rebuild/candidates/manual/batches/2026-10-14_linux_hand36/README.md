# Batch `2026-10-14_linux_hand36` — coordinator hand-authoring chunk 36 (CEngineResourceManager .. CEngineScreenEffectOutlineGlow): 39 of 40 landed

Chunk 36 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: CEngineResourceManager's
FrameEnd (a double budget added straight from memory, a counted pool loop, the data bank reload), the engine scene's
RenderPrimitiveList chain walk and ClearNonPersistent (a reference-counted chain), four list-insert forwarders (by-value
4-byte iterators through reserved slots, a zero-initialised or uninitialised one-byte tag), CEngineSceneGridCell::AddPrimitive,
CEngineSceneMap::CalculateLevel (x87 level loop), the CScriptModuleDollyThing / CSoundEmitterLH (an inlined VC6 list sentinel
malloc) / CTCHeroSpecialMovement constructors, the colour filter's corner render, the glow renderer's Render, and a run of
nineteen small destructors and release bodies: free-buffer pairs/triples, NShape::CShape (three copies) and CBasicShape,
CScreenFilterEntry's entry list, vector-of-objects destroy-range + free chains, a recursive tree node, two non-virtual scalar
deleting destructors, a COM holder (CoTaskMemFree + Release), a doubly linked unlink, the displacement renderer's
FinishDisplacementRendering / texture release, and a frame-pointer screen shutdown.

Label corrections: 00b59840 / 00c44550 / 00ca4990 "CEngineResourceManager::WorldFrameUpdate" are a map-strip delete, a
marked-buffer release of the 0x00C4 module and an appearance rebuild of the 0x00CA module (named by address / effect);
00c458a0 "UnloadShaderCode" applies a falloff curve to eight table entries (named by address); 004638a1 / 00471c19 /
0053cb02 / 00579786 "CEngineSceneGrid::GetCellBoundingBox" are retail list range-insert forwarders; 008aaef0 / 00aac580
"CEngineScenePrimitiveStatsEnumerator ctor" are NCameraModes::CScriptModuleDollyThing (vtable 0x01278188) and CSoundEmitterLH
(vtable 0x0129E6EC); the twenty-one "FinishWithDynamicTextures" rows are destructors / releases of unrelated classes
(NShape::CShape ×3 at 00761bd0 / 00764f70 / 00765050, NShape::CBasicShape at 00762480, NScreenEffect
CScreenFilterEntry at 00884b60, CIVCountedPointeeBase-derived pairs at 00a97850 / 00a978c0, non-virtual scalar deleting
destructors at 004ad8d0 / 0070b830, the rest named by address); 00594fa9 is a virtual-keyboard shutdown with a bool
(GotoPreviousScreen); 00b86a30 is the displacement renderer's dynamic-texture release; 00c4cef0 is a linked-item unlink;
00cb1cc0 a COM holder release. Propagated callee labels: 0x00A23AD0 "_Dest_val" (a pool release), 0x009FD100 /
0x00F36F00 "$E2"-style (bank reload; an empty pool release), 0x00BDED60 "CPatchTesselationEdgeStrip dtor", 0x00C3C370
"InformOfEvent" (a block free), 0x00C1BDA0 "Render_CalculateAlphaFalloff", 0x008834F0 / 0x004A9CB0 "BuildTreeArray" (list
erase), 0x00B5EC90 "TransferBinaryOut" (the glow finish).

Lessons (also in GOTCHAS, wave 24g): a 12/16-byte member copied word by word through ecx/edx/eax in a constructor is
separate scalar members (a struct member goes through the inline copy with `lea`); the range-destroy helper's allocator tag
is `const SScalarTag&` (`lea eax,[esp+7]`), by value it loads the slot; an inline class destructor whose TU also emits a
`??_G` needs `// Leaf: ??1Class`; a loop-invariant float expression computed after the loop-entry test is written inside the
loop (hoisted into the preheader); a VC6 list constructor inlined is `m_pHead = 0 ; node = malloc(16) ; node->next = node->prev =
node ; m_pHead = node`; three stack arguments each read once are three distinct parameters (a cached register means a reuse).

Parked: 00baca20 (store scheduling of the two trailing constructor words).
