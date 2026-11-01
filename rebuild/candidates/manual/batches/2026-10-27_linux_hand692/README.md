# Chunk 692 (NA035 + 0x00C08D40, 529-1021 band) -- 7 of 11 landed (2026-11-01)

Seven rows landed (`verify.log`, every row RELOCATION_MATCH + PASS; bindings multi-check 0), all on the first probe: ~C3DMeshFileLoader 0x00A9A900
(RTTI vtable 0x0129E0D0, engine/CCountedPointer.h members), ~CEngineClothRenderer 0x00BB6730 (compiler-generated, from the real ctor in the TU),
~CEngineSkyRenderer 0x00B621A0, NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript 0x00EE8C30 (the label said CParticleEmitter; sole
caller the ??_G at slot 0 of vtable 0x012F53C0), the two NScript::CV_LostTraderScript::CLostTraderTrader members TraderYellForHelp 0x00EA4980 and
TraderYellAtHero 0x00EA4BB0 (one class, Ego_r donor order -- not two classes as the brief guessed), and zlib 1.2.1 `deflate_stored` 0x00C011A0
(only reference: configuration_table[0].func at .rdata 0x012AE268). 18 labels applied (`ghidra_out/labels_chunk692_2026-10-27.tsv`), incl. the
OakValeRevisited ??_G / ctor, CPixelFormat::Initialise 0x009E3830 (not a ctor), CVolumeTexture::GetLevelByteLength, ~CChunkedFileLoader 0x00AA7540
(labelled as the ctor), CIndexBuffer / CVertexBufferWin32::Uninitialise and the four `_gapscan` rows' names.

Decisions (coordinator, under the owner delegation): deflate_stored keeps the agent's `extern "C"` linkage. The coordinator tried the deflate.c
`static __declspec(noinline)` form: VC7.1 drops an unreferenced static, and retail's only reference is the configuration_table, so the static form
has nothing to grade unless the whole table (and deflate_fast / deflate_slow) comes with it; the body bytes are identical either way, and the landed
res2_forward 0x00CA1360 (also reached only through a table) set the same precedent. 0x00C08D40 takes the descriptive libpk name CSystem::LoadBank
(the class from its assert string ".\CSystem.cpp"); its helper types stay stand-ins. 0x00F35B30 keeps the header's
?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ (the label DB's IsDone is queued).

Parked: CTexture::SaveToDataStream 0x009FB110 (737v732, 27 probes), CTexture::LoadFromDataStream 0x009FB3F0 (850v850, 23), CVolumeTexture::
SaveToDataStream 0x00A68AE0 (754v753, 18) -- register / spill choices across the buffer allocation -- and CSystem::LoadBank 0x00C08D40 (968v964, 15:
esi reuse on the early returns). Best sources in `../../pending/2026-10-27_linux_hand692/differ/`. NAME_FIXES.tsv: 18 rows (queued c692). Header
debt: local models for CVertexShader / CShaderBase / CPixelShader / CIndexBuffer / CMovableResource / CSimpleMemoryPool / CVertexBufferWin32 /
CEngineComponent / CEngineSkyRenderer / CTexture / CPixelFormat / CDataOutputStream / CDataInputStream (generated headers flattened);
engine/CLevelScriptBase.h declares an out-of-line dtor where retail's is implicit; Ego_r's CEngineClothRenderer has 16-byte CArrays, retail 12.
