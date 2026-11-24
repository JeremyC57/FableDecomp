# Landing nf260 (Account A, 2026-11-22): GPLandscapeRenderer, units 1-2 of the L129 family (54 rows onto `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`)

Agent NF260 (L129 partial, two units; 0x009B0AC0 planned, not attempted). Hand-back: `pending/2026-11-22_nf260_handback/` (NOTES = the census
by spelling and the order of the remaining units).
- **Address** (two sources): Ego_r.pdb and FableWin.pdb both put GPLandscapeRenderer at GPResourceManager + 0x10, after GPLightingManager /
  GPCamera / GPEngine, which the table binds at 0x01436E98 / 9C / A0 / A4: 0x01436EA8.
- **Census:** the HEAD objects of all 100 family rows (91 retail referencers + 9 rows naming the old identifier) grouped by the symbol each
  names at 0x01436EA8: 19 spellings (the table's 18 + `?g_obj_01436ea8@@3PAUCGlobal@@A` from 0x00BE5C30, untabled); 17 rows not binders.
- **Unit 1 (47 rows)** off `?g_pLandscapeRenderer@@3PAVCEngineLandscapeRenderer@@A`, **unit 2 (7 rows)** off
  `?g_LandscapeRenderer_01436EA8@@...` (both already the right type: a name change only). 42 rows that include engine/CEngineLandscapeRenderer.h
  use a new add-only `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` (8 lines at the end, no existing line, no assert pinned;
  equiv_check over 1,462 includer rows: 0 differ); 12 rows keep a row-local `namespace NEngineInternal { extern ... }`, respelled in place (8
  model CEngineLandscapeRenderer locally: HEAD debt). Per row: the object differs from HEAD only in the old -> new symbol (all-section
  relocation lists and .text identical after substitution, so the 16 vtable-emitting objects emit the same vtables); pk.py unchanged from
  HEAD. 0x00B40260's test rewritten (it links the row object and defined the old global). Table: the two stand-in lines deleted; 19 -> 17
  spellings. Link: one LNK4006 (`??1CPatchTesselationEdgeStrip@@UAE@XZ`, 0x00BE8E60's local model v owner 0x00BFC410), identical at HEAD.
- Every remaining source mention of the old names is a comment; the other tests that still name them are self-contained host models that
  define their own global (they do not link the row object; their rewrite is queued with the other header-based tests).
- **Recheck** (`logs/recheck.log`, landed tree): the 54 changed rows + the PDB-spelling witness 0x00BEAE50 and 0x00BFC410 (56 rows): 56
  RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF260's questions
- **D1:** the mixed form is accepted: the 42 includers of the header take the opt-in; the 12 rows that model the class locally keep a
  row-local extern of the same symbol until their ports (queued). Both produce the one PDB spelling.
- **D2:** the `??1CPatchTesselationEdgeStrip` duplicate strong definition (0x00BE8E60 v owner 0x00BFC410): queued as a separate fix.
Open: 16 stand-in spellings on 29 rows (ports onto the header's members, MemoryManager +0x6B0 / GetSettings, ordered in NOTES); 0x009B0AC0
(a CDefinitionManager.h opt-in splitting `unsigned char NameCRCToIndexMap[16],DefClassInfo[16];`; the 0x009B9490 sort spelling first).
Slips (HEAD tests prechecked seconds after the first payloads launched; 0x00BDE390 graded despite a clang-only precheck failure that HEAD
shares; GOTCHAS read by grep): noted.
