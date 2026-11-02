# Chunk 701 (NA044) -- 10 of 10 byte-exact; seven catalog labels wrong (2026-11-02)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0; 1-3 probes each; no asm, volatile, flags or
inline_depth): CTCSummonableCreature::SetSummoner 0x0079BC80 (chunk 700's CIntelligentPointer setter shape, pack(1) on the pointer classes for the
+0x11 member; the temporary's vtable RTTI names CIntelligentPointer<CThingCreatureBase>), CManagedTextureData::ReleaseTexture 0x00A69A80,
CBankFileAsync::Close 0x009D58A0 (slot 5 of vtable 0x0129B8D4; CBankFile declared locally -- engine/CBankFile.h untouched, on HOLD), the implicit
~CArenaCreatureDef 0x00F25E40, the byte twins ~CCreatureAction_PerformExpression 0x00718D50 / ~CCreatureAction_PerformExpressionExtended 0x0084ADC0 /
~CCreatureAction_SendMessage 0x009822E0 (three classes, each from its own ??_G / ctor), ~CShaderDataBank 0x00A620D0 (`{ CloseShaders(); }`), ~CHeightMap
0x008F6170 (`{ WorldSeed = 0; }`) and NUISystem::CDynamicList::~ 0x00541350. Four implicit-dtor rows carry the DestroyOne helper (accepted).

25 labels applied (`ghidra_out/labels_chunk701_2026-10-27.tsv`); NAME_FIXES.tsv 25 rows queued c701 for naming wave 30: the private
CShaderDataBank::CloseShaders 0x00A62050 (bound DeleteShaderEntries QAE) with the landed CShaderDataBank::Close 0x00A62110 re-authored as `CloseShaders();
CBankFile::Close();`, CManagedTextureData::ReleaseTexture's binder, the CArenaCreatureDef neighbours (GetSizeofClass 0x00F25E10, ??_G 0x00F25E20, Copy
0x00F25F00, ctor 0x00F263D0 -- landed as stubs / CLandscapeBackgroundPatch / under a CFireheartMinigameDef file name), CResource::SetAsLRU 0x009FC450
(coverage.tsv said SetAsMRU), stray callee bindings. Decision (coordinator): the CBankFileAsync::Close / CBankFile::Close binders (0x00502170, 0x00A26C50,
0x009CE360's three spellings) stay on the CBankFile engine-header HOLD; the other respells go to naming wave 30 rather than this landing (no collision).
Header debt (NOTES.md): CBankFile.h, CCreatureActionBase.h, CObserver.h, CTCBase.h flatten the vptr; CResource.h lacks SetAsLRU; CManagedTextureData.h /
CShaderDataBank.h / CCreatureAction_PerformExpression(Extended).h are base-less raw-byte records (CShaderDataBank.h also carries Ego_r-only members).
