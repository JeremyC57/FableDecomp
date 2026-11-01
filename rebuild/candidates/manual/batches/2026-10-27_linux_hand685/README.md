# Chunk 685 (NA028, 477-936 band) -- 8 of 10 landed (2026-11-01)

Eight rows landed (`verify.log`; bindings multi-check 0), three of them vendor text compiled from source as retail did: DirectShow strmbase
`AMovieSetupRegisterFilter` 0x00CAA4A0 (dllsetup.cpp verbatim, MATCH, /EHsc), libvorbis 1.0.1 `_Laguerre_With_Deflation` 0x00CA3B80 (lsp.c) and
`_01forward` 0x00CA0BA0 (res0.c, /Gr); the DXSDK 2004 `ImeUi_Initialize` 0x00CB5180 (/Gr); and the engine rows NParticleEngine::CPSCAttractor::Update
0x00B179A0 (vtable slot 9), CTCShop::AddSaleItemPlacesFromThing 0x007748E0, CEaxContext::SetSlotLoadEffect 0x00C674B0 (libpk, descriptive) and the
private C3DAnimationBuilder::BuildBoneInfoTree sequence-chunk overload 0x00AB1F30. Seven stand-in labels were wrong; 16 labels applied
(`ghidra_out/labels_chunk685_2026-10-27.tsv`, incl. the COMDAT twins mislabelled as other instantiations and the landed
"CSkillTable_00c67470::SetSkillPropertiesEx" which is the CEaxContext slot-property dispatcher).

Decisions at landing (coordinator, under the owner delegation): descriptive libpk / ImeUi names (SetSlotLoadEffect, SetSlotProperty,
bImcObtained, SendCompString_Ansi, the CutsceneCameraPreloader parameters) stay descriptive until a vendor source names them; the local base
models beside engine headers (CCountedPointer ctor-arg block, C3DMeshInfo `Bank`, CTCShop::GoodsPlaces +0x28, CBoneInfo 0x24,
C3DAnimFileSequenceChunk -8) are header debt for migration wave 10 (NOTES.md "Header debt"); the landed callers spelling 0x007748E0
"InitGoodsPlaces" (0x00774B40, 0x00778210) and 0x00C689B0 are queued c685 for naming wave 19.

Parked: CEngineInternalPrimitiveMeshBase::CollectStats 0x00BEC680 (DIFFER 545v545, 17 probes: instruction-identical, retail homes six map-key
temporaries in the dead statIndex argument slot, frame 0x1C vs 0x28) and CutsceneCameraPreloader 0x00CBF29F (930v936, 12 probes: one register
web, j*4 kept in edi across the StartTimeCode test). Both look like LTCG decisions (lane gap). Best sources in
`../../pending/2026-10-27_linux_hand685/differ/`. NAME_FIXES.tsv: 20 rows (queued c685). Lessons in LESSONS.md (13).
