# Hand chunk 692 (C3DMeshFileLoader .. CSystem): 7 of 11 byte-exact

## What matched (7)
- **C3DMeshFileLoader::~C3DMeshFileLoader** 0x00A9A900 (1021): empty user dtor (stores vtable 0x0129E0D0) over the 22 Ego_r
  CCountedPointer chunk-type members, using engine/CCountedPointer.h; a local CChunkedFileLoader base with its out-of-line virtual
  dtor 0x00AA7540 gives the tail jump. First probe.
- **CEngineClothRenderer::~CEngineClothRenderer** 0x00BB6730 (537): compiler-generated; emitted by defining the real ctor
  (`PositionBufferPool(0,0x1000), NormalBufferPool(0,0x1000)`). Member types modelled locally with inline dtors (CSimpleMemoryPool
  `{ ClearMemoryBlocks(); }` private, CIndexBuffer `{ Uninitialise(); }`, CVertexShader shared-data release, STLport-style CArray).
- **CEngineSkyRenderer::~CEngineSkyRenderer** 0x00B621A0 (660): `{ VertexBuffer.Uninitialise(); IndexBuffer.Uninitialise(); }`, then
  the inline member dtors (CDefPointer<const CSkyDef>, CWideString, shaders) and ~CEngineComponent 0x00B59730.
- **NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript** 0x00EE8C30 (585): implicit dtor emitted by the PDB ctor
  (CScriptDataBase*, CGameScriptInterfaceBase&); local CLevelScriptBase<CQ_SunnyvaleMasterData> without a dtor declaration
  (ChickenKicking 0x00E68EB0 model), two std::vector<CScriptThing>, std::map<CCharString,bool>, seven scripted-thing handles.
- **NScript::CV_LostTraderScript::CLostTraderTrader::TraderYellForHelp / TraderYellAtHero** 0x00EA4980 / 0x00EA4BB0 (556 each,
  jump table inside the row): counter > 5 -> `IsActiveThreadTerminating()` bail else `rand() % 5`; conversation with the hero,
  one switch line, `++counter`. Same class, two members (not two classes as the brief guessed).
- **deflate_stored** 0x00C011A0 (529): zlib 1.2.1 verbatim with a same-TU `static flush_pending` (VC7.1 inlines 2 of 3 FLUSH_BLOCK
  sites like retail).

## Parked for Fable 5.1 (4)
- 0x009FB110 CTexture::SaveToDataStream DIFFER(737v732): row loop prologue -- retail keeps the row count in edi across the vector(n) call and spills it after, ours spills before; src/dst register roles swapped (eax=dst in retail) | tried: 27 probes -- dimension locals before/after the overflow checks (the lever that took residue 251 -> 26), down/up loops, indexed src/dst, declaration order, const src, /Ob1 /GS /EHsc | next: lane gap: register allocation
- 0x009FB3F0 CTexture::LoadFromDataStream DIFFER(850v850): stream reference in ebx where retail has ebp (size register swapped accordingly) and the src/dst slot pair swapped | tried: 23 probes -- forceinline Read (634 -> 850 bytes), down-count loop, width/height locals after use (p16 gets ebp but spills height, 863v850), declaration/advance order, long size/rowBytes, stream reference/pointer locals | next: lane gap: register allocation (retry once CDataInputStream.h is de-flattened)
- 0x00A68AE0 CVolumeTexture::SaveToDataStream DIFFER(754v753): the slice count is spilled before the vector(n) call instead of kept in edi as the outer-loop counter | tried: 18 probes -- the same local-placement lever (residue -> 42), while/for forms, unsigned depth, depth after the buffer | next: lane gap: register allocation (same family as 0x009FB110)
- 0x00C08D40 CSystem::LoadBank DIFFER(968v964): the two early-return paths delete the bank through the holder's reloaded m_pObject where retail reuses esi (libpk is LTCG: GetChecked 0x00C09920 provably leaves the holder alone), which also swaps the holder/name frame slots | tried: 15 probes -- desc without an array member (/GS buffer placement, 138 -> 66), const GetChecked, GetChecked defined in the TU (+noinline), declaration/scope placement, pack(1), switch, explicit Release+delete | next: lane gap: LTCG mod/ref

## Skipped
- none (every row attempted; all extents verified -- each row ends at its retail `ret`/tail jump; the trader jump tables are inside the rows).

## Label corrections
- 0x00EA4980 is TraderYellForHelp (catalog TraderYellAtHero); both trader rows are members of NScript::CV_LostTraderScript::CLostTraderTrader.
- 0x00EE8C30 "CParticleEmitter::~CParticleEmitter_0xe8c30" is ~CCS_OakValeRevisitedScript; its ??_G 0x00EE8C10 (landed as
  CLandscapeBackgroundPatch vector deleting dtor) and ctor 0x00EE8100 (landed as a global-namespace struct) are respelled in NAME_FIXES.
- 0x009E3830 is CPixelFormat::Initialise(_D3DFORMAT), not a ctor (no `mov eax,ecx`); many wrong bindings in reloc_bindings.
- 0x00A681F0 is CVolumeTexture::GetLevelByteLength (landed as GetByteLength); 0x005136B0 is STLport vector<unsigned char>(n).
- 0x00C09920 is PKCAutoDelete<CUnprovenPKObject_00c099f0>::GetChecked (landed as non-template CAutoDeletePointer::GetChecked).
- 0x00C08D40 "_global::CSystem_ProcessMainEvent": libpk CSystem (".\CSystem.cpp" assert, line 54 "EMEM") vtable 0x012B1E24 slot 10,
  a bank factory, descriptive name LoadBank.
- 0x00AA7540 is ??1CChunkedFileLoader (label DB says its ctor): the tail jump of ~C3DMeshFileLoader.
- Gapscan rows: 0x009FB110 / 0x009FB3F0 CTexture::SaveToDataStream / LoadFromDataStream, 0x00A68AE0 CVolumeTexture::SaveToDataStream,
  0x00C011A0 deflate_stored.
