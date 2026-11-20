# 0x004FEB70 CWorldMap::HeightAreaChanged (p25N, 2026-11-20)

Landing order 2 of 2 (after `batches/2026-11-20_worldmap_report_004fc4d0`).

**Grade:** RELOCATION_MATCH (261/261). No flag, no pragma, no inline device, no Inferred line.
- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, fresh LANDVERIFY_WORK, behaviour PASS.
- **Base:** the Q9 / 1013b drafts. Their best was DIFFER(260v261), with `long map` / `CMap* pmap` locals that Ego_d does not have. The
  Ego_d-faithful text graded DIFFER(299v261).

## The residue and its fix
Retail loads `*it` once into ebx (0x004FEBD7) and uses it again after the ClipAndConvertWorldMapAreaToLocalArea call, for `Maps[*it]`
(0x004FEBEB). VC7.1 does this only when it knows the call cannot write the vector's buffer. It knows that here because both bodies are
visible in this TU:
- ClipAndConvertWorldMapAreaToLocalArea 0x004FC4D0 is this file's TU-mate: Ego_d CWorldMap.cpp lines 1387..1402, before this function's
  1524. The row carries `// Fidelity: same-TU strong definition of 0x004FC4D0 (retail TU-mate)`, verbatim from the companion re-port.
- Its one callee, C2DBoxI::ClipTo, is an inline header body: Ego_d file id 2736, lines 97..102. It is out of line in retail as the COMDAT
  0x0049C810. It comes from the new opt-in FABLE_RETAIL_C2DBOXI_CLIPTO_BODY (see the companion batch).

Ablation (`evidence/results.txt`):
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| ClipAndConvert declaration-only (`no_samtu_clipandconvert.cpp`) | DIFFER(265v261) |
| ClipTo declaration-only (`no_clipto_body.cpp`) | DIFFER(265v261) |
| the old `CMap* pmap` local (`pmap_local.cpp`) | RELOCATION_MATCH, **rejected**: not in Ego_d |
| `C2DBoxF area_changed = pworld_box->ToC2DBoxF();` (`tocboxf.cpp`) | RELOCATION_MATCH, **rejected**: Ego_d's 1557 = 47 B is the four-line constructor (47); ToC2DBoxF is 12 |

## Text oracle (Ego_d)
- **Locals:** this, pworld_box, map_list (`std::vector<unsigned long>`, TPI 0x73B6), world_box (C2DBoxI), and it
  (`_Vector_iterator<..unsigned long..>`, TPI 0xF037). There is one iterator for both loops, and two blocks: { local_box } and
  { map_box, area_changed }.
- **The source follows them**, except for map_box. map_box is dead in release: retail converts `*pworld_box` straight to floats at
  0x004FEC20.., and nothing shows its initialiser, so it is not written rather than invented.
- **Line sizes at /Od** (`evidence/od_lines_row.txt`):

| Ego_d line | Ego_d bytes | ours | ours + 8 per `*it` |
|---|---|---|---|
| 1540 | 30 | 22 | 30 |
| 1542 | 39 | 31 | 39 |
| 1544 | 46 | 38 | 46 |
| 1557 | 47 | 47 | 47 (no `*it`) |
| 1558 | 36 | 28 | 36 |

  Each `*it` costs 8 more bytes under Dinkumware's out-of-line iterator operator*, so with that added every loop line matches.
  area_changed is the four-line C2DBoxF constructor; Ego_d records the statement at its last line, 1557.
- **The function head:** 1525..1536 is not reconstructible line by line under Dinkumware (vector and iterator constructors). The
  statement order there is the drafts', and the bytes confirm it.

## Header change (`hdr/`, `hdr.diff`; add-only)
- C2DBoxI.h: **new** FABLE_RETAIL_C2DBOXI_CLIPTO_BODY. It is the same overlay as the 0x004FC4D0 batch.
- newly_flagged_rows: none.

## Defines
CWorldMap.cpp's common block, shared with 0x004FC4D0.
- **Round-1 ablation** (`evidence/define_ablation_round1.txt`, plus direct regrades): every define gives SRC_FAIL without it, except:
  - CLIPTO_BODY: DIFFER(265v261).
  - C2DBOXI_COPY: byte-neutral here, but needed by the 0x004FC4D0 copy (148 v 142).
  - CCHARSTRING and SCOPED_PTR: byte-neutral. They are dropped, and the undefined-symbol set is unchanged.
- The dropped CWORLDMAP_GETMAPBOX was used only by the rejected map_box spelling.

## Tests
- **`test/004feb70.cpp`** (payload test, passes in vland) links the REAL row object. The same-TU ClipAndConvert, the ClipTo COMDAT and
  CTBaseSingleton<CEngineManager>::Get run for real.
- **Stand-ins:** `/alternatename` __fastcall stand-ins for AddChangedArea, CMap::HeightAreaChanged, UpdateAllEdges and
  UpdateNavigationMapArea, and a vtable for GetDimensions / GetAllMapsInArea. The singleton's PClass is also bound with `/alternatename`.
- **What it checks:** one ordered event log across 7 scenarios: null area (dimensions), inner area, maps with and without a CMap, an
  unordered list, a corner box, a single map, and an empty list. It also checks that nothing in the CWorldMap fixture is written.
- **Mutants:** 12/12 KILLED (`mutants/004feb70.out`).

## NAME_FIXES.tsv
- Catalogue: this row's own name is already the PDB spelling.
- The landed 0x004FC4D0's local names are respelled in the companion re-port (not symbols).
