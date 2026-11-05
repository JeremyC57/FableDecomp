# Chunk 779 -- five engine rows (2026-11-05)

| Row | Name (proven) | Result | Grades |
|---|---|---|---|
| 0x00C064D0 | `inflate_table` (zlib 1.2.1 inftrees.c) | RELOCATION_MATCH, test PASS | 1 |
| 0x00837E40 | `CThingTrackNode::ConstructFromLevelScript(const char*)` ("vfunc_4") | RELOCATION_MATCH at the true extent 1423, test PASS | 5 |
| 0x00542A30 | `NUISystem::CIconText::Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*)` ("vfunc_2") | DIFFER(1669v1714) parked, test PASS | 20 |
| 0x00AAD650 | `CWaveLoader::LoadWaveFile(CDataInputStream&)` | DIFFER(1575v1574) parked (true extent 1574), test PASS | 16 |
| 0x00BEAE50 | `CWaterPatchMesh::Load(CDataInputStream&)` | DIFFER(1592v1592) parked, test PASS | 14 |

Every name is proven from two sources (LABELS.tsv): the RTTI vtable slot and the Ego_r / FableWin name sets for the two
virtuals, the PDB name sets plus the strings / callers for the other three. No descriptive names in the sources; the
only descriptive spelling kept is `(EWaterType)N` numerals in the water row (no enumerator names recovered).

## Two mis-cut ledger rows (coordinator action)
- 0x00837E40 (ledger 1691 bytes) = ConstructFromLevelScript 0x00837E40..0x008383CE (1423 bytes, `ret 4`) + int3 + `CThingTrackNode::Create`
  0x008383D0 (265 bytes, LABELS.tsv). The source matches the 1423-byte extent (RELOCATION_MATCH, `ORACLE_LEN=1423`); against the
  batch oracle vland reports DIFFER(1423v1691). `oracle.tsv` in this directory carries the corrected 1423-byte row; the catalog row
  needs the same cut. Create is decoded (CThing allocation 0x004C7380(0xF0, false), ctor 0x00837C70, ConstructFromParams 0x00836C50,
  AddTC "CTCVillageMember" / "CTCPhysicsStandard", CTCParamsPhysicsBase 0x006A06E0, the physics TC's slot 3 ConstructFromParams,
  ~CBaseClassNonCopyable 0x0099A300, DoActivationSequence 0x004C9CA0) but not authored (not a ledger row).
- 0x00AAD650 (ledger 1840 bytes) = LoadWaveFile 0x00AAD650..0x00AADC75 (1574 bytes) + int3 + the vector<short> COMDATs that start at
  0x00AADD80 (landed as their own rows). Graded with `ORACLE_LEN=1574`; the differ note uses that extent.

## 0x00C064D0 inflate_table
Vendor zlib 1.2.1 `inftrees.c` text (ENOUGH 1440 / MAXD 154, the `lbase/lext/dbase/dext` tables, the 1.2.1 `inflate_table` with
its `here` code and the `incr`/`fill` loop), `extern "C"`, `/GS` as the landed `inflate_fast` row. First grade.

## 0x00837E40 CThingTrackNode::ConstructFromLevelScript
Shape: `AddTC("CTCVillageMember", false, 0)`; `if (CThingGameObject::ConstructFromLevelScript(script)) { AddTC("CTCPhysicsStandard",
false, 0); CCharString scriptText(script); GetTC(TCI_PHYSICS).SerialiseLevelState(false, scriptText, !CMainGameComponent::Get()->
IsEditorActive()); NextNodeUID = PreviousNodeUID = 0; PreviousNodeThingFileID = NextNodeThingFileID = 0; CStringParser parser(script);
while (parser.NextItemExists()) { CCharString key = parser.ReadAsString(); if (key == "LinkedToUID1") PreviousNodeUID =
GetPCurrentThingManager()->RemapUIDForSerialisation(false, IsLevelLess(), GetCurrentMapNumber(), parser.ReadAsUHUGE()); else if
"LinkedToUID2" ... NextNodeUID; "LinkedToFileID1" / "LinkedToFileID2" = parser.ReadAsInteger(); "Start" / "End": a CCharString value
with EqualsCaseInsensitive("TRUE") / ("FALSE") setting IsStart / IsEnd; "ScriptName": TrackName = parser.ReadAsString(); "EndThing":
break; parser.SkipPastString(";"); } return true; } return false;`.
Key spellings: the positive-branch form (the negative early return reorders the exits); FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
plus the new FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI (hdr/fable_string.h) and the row-local `CBasicString<T>::Compare` /
`operator==(const T*)` bodies (landed 0x009BB110 precedent) so the eight compares and the four EqualsCaseInsensitive expand in place.
Callees (reloc_bindings / landed rows): CThing::AddTC 0x004C9D60 `?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z`,
CThingGameObject::ConstructFromLevelScript 0x00838930 (NAME_FIXES), CTCBase::SerialiseLevelState 0x00686E10, CMainGameComponent::
IsEditorActive 0x004197B0 (slot 9 of vtable 0x0122F180), CThingManager::RemapUIDForSerialisation 0x0051E210, CThing::IsLevelLess
0x004C7930, CStringParser 0x009B9BA0 SkipPastString / ReadAsUHUGE 0x009BA5B0 / ReadAsString / ReadAsInteger / NextItemExists,
_stricmp 0x00BFEBA8, the globals CMainGameComponent::PMainGameComponent 0x013B86A0 and CThing::PCurrentThingManager 0x013B8A1C.
Headers: nested opt-ins FABLE_RETAIL_CTHINGTRACKNODE, _CTHINGGAMEOBJECT_LEVELSCRIPT, _CTCBASE_SERIALISELEVELSTATE,
_CMAINGAMECOMPONENT_EDITOR, _CSTRINGPARSER_READASUHUGE (hdr/README.md).

## 0x00542A30 NUISystem::CIconText::Draw (parked, differ/00542a30.cpp)
Shape (retail bytes read in full): `if (RenderColour.A <= 0 || RenderZoom.X <= 0.0f) { if (NoRenderNextFrame == 1) { parentHandle ?
ClearChildPrimitive x2 : (TextHandle.Clear(); DropShadowHandle.Clear(); for_each(IconHandles, mem_fun_ref(&Clear))); return; }
NoRenderNextFrame = 1; } else NoRenderNextFrame = 0;` position = RelativeRenderPosition (+0xF8), colour = RenderColour (+0x94);
off screen when `position.X >= GetBackBufferWidth() || position.Y >= GetBackBufferWidth()` (both against the width: the globals
0x013B8768 / 0x013B876C / 0x01375CD4 twice) -> the same three clears; drawLayer = Layer + layer unless IsIndependent() (slot 101) or
IsLayerIndependent() (slot 104); `CRGBColour shadowColour(0,0,0,colour.A)`; two default CEnginePrimitive2DText (Type 0x27); switch
(GetAlignment() slot 151): CENTER `position.X -= Size.X * 0.5f`, RIGHT `position.X -= Size.X` (no scale, unlike CText); x / y
rounded through the shared GFFloatToLongNear helper (bare `fistp dword` at 0x00542C97 / 0x00542CA7: Retail-asm line); `C2DVector
scale = RelativeRenderZoom` (+0x108, integer copies); Unavailable: text in shadowColour / drop shadow in colour, else the reverse;
the drop shadow at (x+2, y+2) on GetLayer(drawLayer + 1) (CManager::GetInstance 0x0041E5F2, slot 36); `if (parentHandle &&
childIndex)` SetChildPrimitive x2 (slot 28, flags 0x40) else SetPersistentPrimitive(TextHandle / DropShadowHandle, ..., 0x40) (slot
23), `IconHandles.resize(Icons.size())` (0x00443F90), then per icon (stride 0x1C, `Icons.size()` recomputed each iteration, index
kept as a byte offset in a frame slot) a C2DBoxF from position + icon.Offset and scale * icon.Size, a CEnginePrimitive2DSprite
(0x0041BEB0: 0, box, GetLayer(drawLayer), icon.Graphic, 0, 2, 0, 0.0f, 0.0f, colour.A, colour.R, colour.G, colour.B, 2, uvStart,
uvEnd) and SetPersistentPrimitive(IconHandles[i], sprite, 0xC0); finally the two ~CWideString of the text objects.
What was learned (STATUS.tsv has every grade): both clear sites need `do { } while (0)` (EXTRA_RULES 2026-11-03 statement wrapper,
uniform at both sites) to keep both Clear pairs inline; the box must be the 4-float ctor over named x/y/w/h locals; the two zero
C2DVector locals of the sprite are declared INSIDE the loop body and VC7.1 hoists their stores into the preheader only when the
sprite ctor's body is visible in the TU -- so retail's CEnginePrimitive2DSprite 16-argument ctor is defined in its header (a COMDAT,
0x0041BEB0 the linker's copy); the differ source carries that body above Draw (the landed ctor row's stores, same spelling) and
hdr/engine/CEnginePrimitive2DSprite.h should get it as an inline definition when the row lands (header debt below).
Residue: frame slot assignment. Retail: position 0x10, shadowColour 0x18, [hole 0x1C], colour 0x20, [hole 0x24], rounding temp /
scale 0x28, drawLayer 0x30, uvEnd 0x34, uvStart 0x3C, text 0x44, dropShadow 0x70, box 0x9C, h-spill 0xB0, sprite 0xB4 (frame 0xF8);
the Unavailable branch's shadow position reuses 0x18, the other branch's 0x34. Ours: position 0x10, shadowColour 0x18, colour 0x1C,
[hole 0x20], temp 0x24 (both shadow positions 0x24), drawLayer 0x2C, scale 0x30, uvEnd 0x38, uvStart 0x40, text 0x48, dropShadow
0x74, box 0xA0, h-spill 0xB4, sprite 0xB8 (frame 0xFC). Declaration-order probes move colour (0x1C -> 0x24) and the temp but never
produce the two retail holes; the holes are not the IsIndependent bools (o2) nor the second rounding (o1). Consequences of the
different packing: the constant registers swap (retail ebx = 0x27 / ebp = 0 -> imm8 byte zero stores), the cross-jumped tail of the
two text-constructor branches starts earlier in ours, and `this` / icon pointer colouring. Next idea in the file's line 3.
Headers: new engine/NUISystem_CIconText.h (retail layout, slots 142-151 named) and engine/CEnginePrimitive2DSprite.h; the C2DBoxF.h
CORNERS guard fix; hdr/fable_definition_table.h (unchanged copy, overlay search order only).

## 0x00AAD650 CWaveLoader::LoadWaveFile (parked, differ/00aad650.cpp)
Shape: `CCountedPointer<CWaveFile> result; if (stream.ReadString(4) != "RIFF") throw CWaveFileException(); unsigned long riffSize =
stream.ReadULONG(); unsigned long riffEnd = stream.GetPosition() + riffSize; if (stream.ReadString(4) != "WAVE") throw ...;
CWaveFormatHeader format; while (stream.GetPosition() < riffEnd) { CCharString chunkId = stream.ReadString(4); unsigned long chunkSize
= stream.ReadULONG(); unsigned long chunkStart = stream.GetPosition(); if (chunkId == "fmt ") { FormatTag = ReadUWORD(); Channels =
ReadUWORD(); SamplesPerSec = ReadULONG(); AvgBytesPerSec = ReadULONG(); BlockAlign = ReadUWORD(); BitsPerSample = ReadUWORD(); } else
if (chunkId == "data") { if (format.FormatTag != 1) throw ...; unsigned long sampleCount = chunkSize / BlockAlign * Channels;
CArray<short> samples(sampleCount); long bits = BlockAlign * 8 / Channels; 16: samples[i] = ReadSWORD(); 8: samples[i] =
(short)(((unsigned short)ReadUBYTE() - 0x80) << 8); else throw; result.Reset(new CWaveFile); result->Channels = Channels;
result->SamplingFrequency = SamplesPerSec; result->AudioData = samples; result->SampleLength = result->AudioData.size() /
result->Channels; } stream.SetPosition(chunkStart + chunkSize); } return result;` (hidden result pointer; throw temporaries in the
dead `stream` slot; _CxxThrowException 0x00BFEB84 with ThrowInfo 0x01369580 CWaveFileException : CExceptionBase, no /EHsc).
Key spellings: FABLE_RETAIL_CDATAINPUTSTREAM inline reads (__forceinline ReadType; plain inline calls Read out of line), the new
READSTRING opt-in and the ReadUWORD / ReadSWORD / ReadUBYTE wrappers (FableWin names), FABLE_RETAIL_STLP_VECTOR_ASSIGN +
_N_OUTOFLINE (declaration-only vector(n) / operator=: retail reloads the local's start from the stack), CCountedPointer::Reset out of
line (0x00AADFF0), row-local CBasicString bodies, CCharString `operator==(const char*)` null arm = the strcmp intrinsic against "".
Callees: ReadString 0x00996BE0, ReadWithSrcChunkOverflow 0x00993CA0, CBasicString<char>::operator==(const char*) 0x004115A0,
vector<short>(n) 0x00AADD80 / operator= 0x00AADE80 (NAME_FIXES), operator new 0x00BFEA1A, CCountedPointer<CWaveFile>::Reset
0x00AADFF0, GetPosition slot 2 / SetPosition slot 1, ~CCharString 0x0099EAE0, free 0x00BFEA14.
Residue: two of the six `fmt ` reads (FormatTag, BlockAlign) lower StreamPos + 2 as `mov ecx,[ebp+4] ; add ecx,2` in retail, `mov
eax,[ebp+4] ; lea ecx,[eax+2]` in ours (register colouring); the other four reads match. Next idea in the file's line 3.
Headers: new engine/CWaveLoader.h (nested CWaveFormatHeader), CWaveFile.h, CWaveFileException.h, CExceptionBase.h; CDataInputStream.h
additions.

## 0x00BEAE50 CWaterPatchMesh::Load (parked, differ/00beae50.cpp)
Shape: WaterBodySpan = ReadFloat(); WaterType from a jump-table switch on ReadSLONG() (8, 2, 4, 5, 3, 6 kept; 0, 1, 7, > 8 -> 1);
VertexBuffer = landscape MemoryManager->AllocateVertexBuffer(this, VERTEX_NULL, 289, sizeof(CTVertexWaterForeground), resource
manager GetVertexBufferUsage()) or return false; compressedSize = ReadSLONG(); CArray<unsigned char> decompressed(289 * 66),
compressed(compressedSize); Read; CRangeCompressor::Decompress(compressed, 289, 66, decompressed) 0x00F39ED0; CMemoryDataInputStream
over decompressed; Lock; per vertex X, Y (u16), Z (float), SwellData0/1 (u16), SetTargetVertexDepth(short / 16384 clamped [0,2]),
ShoreUCoord (float), 48-byte ShoreLookUpArray; Unlock; return true.
Callees: 0x00993CA0, CEngineResourceManager::GetVertexBufferUsage 0x00B3B030 (@0x01436E98), AllocateVertexBuffer 0x00BDA3D0
(MemoryManager +0x6B0 of @0x01436EA8), vector<unsigned char>(n) 0x00514E20 (NAME_FIXES), CMemoryDataInputStream ctor 0x00994930 /
~CDataStream 0x00993B10, CVertexBufferBase::Lock 0x00AC20A0 / Unlock 0x00AC20B0, free 0x00BFEA14.
Residue: the esi / edi role swap between the locked vertex pointer and the memory stream's this through the 289-vertex loop (all 236
diff lines are register names). Next idea in the file's line 3.
Headers: retail opt-in branches engine/CWaterPatchMesh.h and CEngineLandscapeMemoryManager.h (generated defaults untouched), new
engine/CTVertexWaterForeground.h, FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE.

## Header debt
- engine/CEnginePrimitive2DSprite.h: the 16-argument ctor should become an inline (in-class) definition with the landed 0x0041BEB0
  body (the hoisting evidence above); left as a declaration until the coordinator decides how the landed ctor row and the COMDAT
  view coexist (the ctor row then documents the COMDAT instance).
- engine/C2DBoxF.h: the CORNERS double declaration (fixed in hdr/, trivially landable).
- engine/NUISystem_CComponent.h: slot 151 (`GetAlignment`) and slots 142-150 exist only on CText / CIconText; no base change needed.
- The CDataInputStream wrappers (ReadUWORD / ReadSWORD / ReadUBYTE) follow the chunk-737 unconditional-in-branch precedent; if the
  coordinator prefers a nested opt-in, differ/00aad650.cpp needs the macro added.

## Read-only verifier (vland, final overlay, 2026-11-05)
- Against `$S/batch/chunk779_oracle.tsv` (tmp/vland.log): 00c064d0 RELOCATION_MATCH PASS; 00837e40 DIFFER(1423v1691) -- the mis-cut row.
- Against `oracle.tsv` here, 00837e40 trimmed to the 1423-byte extent (tmp/vland_own_oracle.log): both rows RELOCATION_MATCH PASS.
  No RELOC_IDENTITY. Two WARNs on 00837e40, both pre-existing bindings in reloc_bindings.tsv: RELOC_NAME `std::_List_base<CStringParserCommentBoundaries>::~_List_base`
  -> 0x004046B0 (the folded `std::list<...>::~list` body, same binding as landed 0x00998570) and RELOC_ALIAS `_stricmp` -> 0x00BFEBA8 import thunk.
