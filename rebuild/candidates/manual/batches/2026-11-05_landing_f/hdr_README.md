# Chunk 779 header overlay (FABLE_HDR_OVERLAY)

Every change is an opt-in branch or a new header; no default path changes bytes (SIBLINGS.tsv). Evidence lines are in each header.

| Header | Change | Kind | Rows |
|---|---|---|---|
| engine/CThingTrackNode.h | `FABLE_RETAIL_CTHINGTRACKNODE`: ConstructFromLevelScript (virtual, 0x00837E40), static __fastcall Create 0x008383D0, SetTrackName, PeekTrackName, IsTrackStart/End, protected ConstructFromParams 0x00836C50 | opt-in | 00837e40 |
| engine/CThingGameObject.h | `FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT`: virtual ConstructFromLevelScript(const char*) 0x00838930, protected ConstructFromParams 0x008388D0 | nested opt-in | 00837e40 |
| engine/CTCBase.h | `FABLE_RETAIL_CTCBASE_SERIALISELEVELSTATE`: SerialiseLevelState(bool, CCharString&, bool) 0x00686E10 | nested opt-in | 00837e40 |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR`: protected virtuals Update/GetInputs/Render/PostInit (slots 5-8) and `virtual bool IsEditorActive() const` slot 9 0x004197B0 (vtable 0x0122F180) | nested opt-in | 00837e40 |
| engine/CStringParser.h | `FABLE_RETAIL_CSTRINGPARSER_READASUHUGE`: `unsigned __int64 ReadAsUHUGE()` 0x009BA5B0 | nested opt-in | 00837e40 |
| fable_string.h | `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI`: `__forceinline` on EqualsCaseInsensitive | nested opt-in | 00837e40 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE`: `explicit vector(size_type)` declaration-only | per-TU opt-in | 00beae50, 00aad650 |
| engine/CWaterPatchMesh.h | `FABLE_RETAIL_CWATERPATCHMESH`: retail class (CMovableResource base, vtable 0x012A8134 slots, members +4..+0x1C, sizeof 0x20, PDB member functions, nested CVertexHeightData) round the generated default | opt-in (generated header keeps its RETAIL mark) | 00beae50 |
| engine/CEngineLandscapeMemoryManager.h | `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER`: retail class (CFailedAllocationHandler + CMovableResource bases, members +8..+0x40, sizeof 0x44, AllocVirtual / FreeVirtual / AllocateIndexBuffer / AllocateVertexBuffer) round the generated default | opt-in (RETAIL mark) | 00beae50 |
| engine/CTVertexWaterForeground.h | new: the 66-byte-packed / locked water foreground vertex (fields read by Load) | new | 00beae50 |
| engine/CDataInputStream.h | `FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING`: `CCharString ReadString(unsigned long)` 0x00996BE0; plus `__forceinline` ReadUWORD / ReadSWORD / ReadUBYTE wrappers next to ReadULONG / ReadSLONG inside the FABLE_RETAIL_CDATAINPUTSTREAM branch (chunk-737 precedent; unused inline members, byte-neutral) | nested opt-in + branch addition | 00aad650 |
| engine/CExceptionBase.h, CWaveFileException.h | new: empty base and the derived exception (ThrowInfo 0x01369580: CWaveFileException : CExceptionBase, size 1) | new | 00aad650 |
| engine/CWaveFile.h | new: Channels +0, SampleLength +4, SamplingFrequency +8, `CArray<short> AudioData` +0xC (sizeof 0x18), the PDB member functions | new | 00aad650 |
| engine/CWaveLoader.h | new: nested CWaveFormatHeader, LoadWaveFile / SaveWaveFile (PDB reference spellings) | new | 00aad650 |
| engine/NUISystem_CIconText.h | new: retail layout (Text +0x15C .. NoRenderNextFrame +0x1B0, sizeof 0x1B4), slots 1/2/43/48/88/92/95/111/142-151 named, nested CEmbeddedIcon | new | 00542a30 |
| engine/CEnginePrimitive2DSprite.h | new: PDB layout (sizeof 0x54) and the 16-argument ctor declaration 0x0041BEB0 (header debt: should be an inline definition, NOTES.md) | new | 00542a30 |
| engine/C2DBoxF.h | the chunk-751 out-of-line corner-ctor declaration is skipped under `FABLE_RETAIL_C2DBOXF_CORNERS` (it redeclared chunk 749's inline copy: C2535) | bug fix, default path unchanged | 00542a30 |
| fable_definition_table.h | UNCHANGED copy: only so that the overlay's fable_string.h is the single copy included (#pragma once + own-directory lookup, LESSONS.md). Do not land. | helper | 00542a30 |

Overlay confirmation: a deliberate byte change in hdr/engine/CThingTrackNode.h turned 0x00837E40 DIFFER during authoring (then
reverted); the C2DBoxF.h guard is proven by d/e (SRC_FAIL C2535 with the repo header, compiling with the overlay copy).
Includers rechecked: none required (no default-path change; grep for FABLE_RETAIL_C2DBOXF_CORNERS in rebuild/src/compiled is empty).
