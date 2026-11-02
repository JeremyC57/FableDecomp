# Chunk 714 (NA057) -- 10 of 10 byte-exact (RELOCATION_MATCH), one opt-in branch, one retail-branch declaration (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; eight of ten labels wrong): CTCActionUseScriptedHook::~CTCActionUseScriptedHook 0x00729A50 and
GetRegionEntranceDisplayName 0x00729CA0, CDisplayManager::PreDeviceReset 0x009BF500 (private), and seven destructors named from their vtables /
slot-0 ??_G callers: NGraphicAppearance::~CTCGraphicAppearanceReplaceableMesh 0x007A66F0, ~CTCCreatureTintEffect 0x007CF0B0 and
NGraphicAppearance::~CTCGraphicAppearanceMultipleStaticMeshes 0x007E1510 (the latter two carried the ReplaceableMesh label propagated across byte
twins), and the compiler-generated ~CTCAIScratchpad 0x007ADAA0, ~CShaderBankEntry 0x00A628D0, ~C3DAnimationManager 0x00AA4170 and
NScript::CQ_TraderEscortScript::CDarkwoodTrader::~CDarkwoodTrader 0x00E01710.

Headers (hdr/README.md): new opt-in FABLE_RETAIL_CSIMPLEMEMORYPOOL in engine/CSimpleMemoryPool.h (: CBaseClassNonCopyable, vtable 0x0129A064,
sizeof 0x1C, inline virtual dtor calling the private ClearMemoryBlocks 0x00A5C600; the old flattened text unchanged in `#else`), and
`class CWorldMap;` + protected `const CWorldMap& PeekWorldMap() const;` (0x00686D20, IBE in both PDBs) inside the existing FABLE_RETAIL_CTCBASE
branch of engine/CTCBase.h. Default text of each unchanged under `cpp`.

Second spellings at landing: CDisplayManager::PreDeviceReset / PostDeviceReset are private (`AAE`) in both PDBs and CSystemManager's hook
0x009A52C0 is `?OnPostDeviceReset@CSystemManager@@QAE_NXZ`; the landed ResetDevice 0x009BFB30 (both calls) and the landed PostDeviceReset
0x009BF550 (its own definition and its OnDisplayDeviceReset callee) were respelled in this commit (access / name only, the bool result unused as
retail; bindings updated). Recheck of the 62 landed FABLE_RETAIL_CTCBASE / CSimpleMemoryPool.h includers plus 0x009BFB30 and 0x009BF550: 65 of 65 =
5 MATCH + 60 RELOCATION_MATCH, all PASS (includer_recheck_grades.tsv). Queued PRIORITY (NAME_FIXES_QUEUE.tsv, c714): PeekWorldMap 0x00686D20
bound under three stand-ins, GetPos 0x004C73D0 under eight, the seven ??1 rows bound under stand-ins by their landed ??_G callers (whose own
names are also stand-ins), the 0x00B260B0 SimpleMemoryPoolNative / NativeInitBaseClass local models, and ResetDevice 0x009BFB30's own spelling
(retail `AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z`; binder 0x009BFE70). The agent's 22 NAME_FIXES rows are queued as c714;
labels in ghidra_out/labels_chunk714_2026-10-27.tsv.
