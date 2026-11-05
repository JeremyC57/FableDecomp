# Landing 2026-11-05f: chunk 779 (2 of 5 ledger rows)

Authored by one expert agent after the owner's pause was lifted (the user asked to continue where the last session left off). Both
rows are plain C++ and grade `genuine`.

| Address | Name | Grade | Grades | Key spelling |
|---|---|---|---|---|
| 0x00C064D0 | inflate_table | RELOCATION_MATCH PASS | 1 | zlib 1.2.1 `inftrees.c` text, `extern "C"`, `/GS` (the landed inflate_fast flags) |
| 0x00837E40 | CThingTrackNode::ConstructFromLevelScript | RELOCATION_MATCH PASS | 5 | vtable 0x01272CE4 slot 4 (was "vfunc_4"); eight inlined key compares need the forceinline EqualsCaseInsensitive opt-in |

- **Extent corrections.** The groups ledger's byte counts are spans, not lengths.
  - 0x00837E40 is 1423 bytes (`ret 4`), then int3, then CThingTrackNode::Create 0x008383D0 (265 bytes). Create was missing from
    `rebuild/manifest/functions.tsv` and is added there (Ego_r `?Create@CThingTrackNode@@SIPAV1@JABVC3DVector@@VCCharString@@@Z`).
  - 0x00AAD650 LoadWaveFile is 1574 bytes, then int3, then the vector<short> COMDATs from 0x00AADD80. Its parked source is graded
    against 1574.
- **Headers** (`hdr_README.md`, `logs/hdrcheck2.txt`): every changed header reads `default=same`. Seven new headers (CWaveFile,
  CWaveLoader, CWaveFileException, CExceptionBase, CTVertexWaterForeground, CEnginePrimitive2DSprite, NUISystem_CIconText) and new
  opt-ins (CTHINGTRACKNODE, CTHINGGAMEOBJECT_LEVELSCRIPT, CTCBASE_SERIALISELEVELSTATE, CMAINGAMECOMPONENT_EDITOR,
  CSTRINGPARSER_READASUHUGE, CCHARSTRING_FORCEINLINE_EQUALSCI, STLP_VECTOR_N_OUTOFLINE, CWATERPATCHMESH,
  CENGINELANDSCAPEMEMORYMANAGER, CDATAINPUTSTREAM_READSTRING). C2DBoxF.h no longer declares the corner ctor twice under
  FABLE_RETAIL_C2DBOXF_CORNERS (it failed C2535 for any row that opted in).
  - FABLE_RETAIL_CDATAINPUTSTREAM reads DIFF (new ReadUWORD / ReadSWORD / ReadUBYTE forceinline wrappers): all 25 landed sources
    that define it recheck RELOCATION_MATCH / MATCH PASS with the overlay on a full include mirror
    (`logs/includer_recheck_cdatainputstream.log`). The other DIFF macros have no landed user.
- **Landing:** `vland --land` (`logs/land.log`). Labels `ghidra_out/labels_landing_2026-11-05f.tsv` (10, incl. CIconText GetValue /
  GetFont / AddIcon / InitIcons and CThingTrackNode::Create); `bindings_multi_check.py` prints 0; `sync_manifest_from_catalog.py
  --dry-run` prints 0. sync_catalog_names.py was run and its 4,220-row churn discarded (GOTCHAS 2026-11-04).

## Parked (`pending/2026-11-05_linux_hand779/`, each with its residue and next idea)
- **0x00542A30 NUISystem::CIconText::Draw** (slot 2): DIFFER(1669v1714) after 20 grades. The instruction stream matches except
  the frame-slot assignment (retail keeps two unassigned 4-byte slots around the colour). Uses the shared GFFloatToLongNear helper
  for retail's bare `fistp dword` (0x00542C97 / 0x00542CA7).
- **0x00AAD650 CWaveLoader::LoadWaveFile:** DIFFER(1575v1574) after 16 grades; two sites of add-vs-lea register colouring.
- **0x00BEAE50 CWaterPatchMesh::Load** (private): DIFFER(1592v1592) after 14 grades; an esi/edi role swap only.

## Names
10 NAME_FIXES queued (tag `land0511f`): the CWaterPatchMesh ctor and Load, vector<short> ctor / operator=, CCountedPointer<CWaveFile>::Reset,
SaveWaveFile, vector<unsigned char> ctor, CThingGameObject::ConstructFromLevelScript, the `__fastcall` std::for_each 0x0043F360,
vector<CEnginePrimitiveHandle>::resize 0x00443F90.
