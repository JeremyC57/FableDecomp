# Landing 2026-11-04e: chunk 743 (11 libvorbis / libogg rows) and 4 invented Vorbis rows re-modelled from the Xiph text

## Part A: chunk 743, 11 rows (`logs/land.log`, every row RELOCATION_MATCH + PASS)
Each row is the shipped Xiph source text unchanged: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909") and libogg 1.1.1,
statically linked into libpk. The catalog names were propagated junk.

| Address | Xiph name (file) | Was catalogued as | Flags |
|---|---|---|---|
| 0x00C8AB70 | ov_raw_seek (vorbisfile.c) | CreatureHitNotification_Process | /GS |
| 0x00C8AE90 | ov_pcm_seek_page (vorbisfile.c) | CreatureHitNotification_StateProcess | /GS |
| 0x00C8B490 | ov_pcm_seek (vorbisfile.c) | Subsystem_LoadWorldMapAndNotifications | /GS |
| 0x00C8D690 | ogg_stream_pagein (framing.c) | WeatherRenderer_ProcessStream | none |
| 0x00C8FF30 | vorbis_synthesis_blockin (block.c) | AudioDSP_Process | /TC /Gr |
| 0x00C94AE0 | _vp_couple (psy.c) | Audio_ProcessWeightedCoefficients | /TC /Gr |
| 0x00C9C620 | mdct_backward (mdct.c) | Mesh_ComputeDetailLOD | none |
| 0x00C9C870 | mdct_forward (mdct.c) | Mesh_Process | none |
| 0x00C9DD60 | _ve_amp (envelope.c static) | Mesh_ComputeNormalMap | /TC /Gr |
| 0x00C9E280 | _ve_envelope_search (envelope.c) | Mesh_ResizeDetail | /TC /Gr |
| 0x00C9EF90 | mapping0_forward (mapping0.c static, address-taken) | CPlayer::ProcessAudioFrame | /TC /Gr |

- No asm, no helper headers, no header changes.
- The Xiph tarballs were fetched from gitlab.xiph.org and read only; nothing from them was run.

## Part B: four landed rows were invented models of Xiph functions; re-modelled from the Xiph text (`remodel/`)
The new 0x00C9EF90 and 0x00C8FF30 bind 0x00C9CFD0 and 0x00C9EA70 under their Xiph spellings. Each of those addresses already had a
second, invented spelling, coming from earlier landed rows that modelled Vorbis functions as engine classes. Retail evidence was
used to rewrite those rows verbatim from Xiph:

| Address | Xiph row now | Landed before as |
|---|---|---|
| 0x00C90C80 | `vorbis_window` (block.c 962-970) | `CPoolEntryTable_00c90c80::GetEntryAllocatorSize`, callee `?LookupAllocatorSize@@YIKK@Z` |
| 0x00C90CB0 | `vorbis_analysis` (analysis.c 32-64) | `InitializeDrawing(SVideoPlayer*, SDrawingInfo*)` with `SSurface::Reset / GetWidth / GetHeight`, `SVideoPlayer::IsBusy`, `g_pPlayerBackend` |
| 0x00C9CFD0 | `vorbis_bitrate_managed` (bitrate.c 171-178) | `S::M()` (catalog CPlayer::GetCurrentMode) |
| 0x00C91800 | `oggpack_get_buffer` (libogg bitwise.c 442-444) | `DrawGetEnvironment(CWorld*)` (catalog GetMember_dword0x8) |

- **Grades.** Read-only `--recheck-landed` first (`logs/remodel_pre*.log`):
  - 0x00C90C80 and 0x00C90CB0 are RELOCATION_MATCH + PASS;
  - 0x00C9CFD0 and 0x00C91800 are MATCH + PASS.
- **Re-landing.** `repair_remove.py` then `vland --land` (`logs/remodel_land*.log`).
- **Tests.** The new behaviour tests copy the Xiph bodies over scripted callees; they pass under g++ and VC7.1.
- **Bindings.** After the re-land, 0x00C9CFD0, 0x00C9EA70, 0x00C91800, 0x00C917B0, 0x00C912F0 and 0x013A3DDC (`__mapping_P`) each carry
  exactly one spelling, the Xiph one. `bindings_multi_check.py` prints 0.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04e.tsv`, 17 Xiph names. `apply_labels.py` made 15 catalog renames and 15 manifest
  renames.
- **Queued PRIORITY** (`land0411e`): 0x00C90FC0 `@oggpack_write@12`. Its binders 0x00C911B0 (oggpack_writealign) and 0x00C96140
  still use invented `?WriteBits_00c90fc0@@...CBitstream_*` spellings.
- **Open, already queued for the owner:** the per-file `todB` copies (scales.h static): 0x00C9DD40 / 0x00C9EF70 against the landed
  0x00C90D40.
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,571 of 49,564 (93.961%); 5,775,877 function bytes (56.472%); 54,123 landed rows.
