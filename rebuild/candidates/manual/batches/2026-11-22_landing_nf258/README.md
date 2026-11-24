# Landing nf258 (Account A, 2026-11-22): the L37 vector<unsigned long> copy-ctor family, 7 of 8 binders (0x00535470 7 -> 2 spellings)

Agent NF258 (L37 done but one blocked binder; L129 not started). Hand-back: `pending/2026-11-22_nf258_handback/`.
- **0x006250B0:** `CULongVector` -> the header `std::vector<unsigned long>` (VECTOR_COPY_DECL_ONLY); only the 0x00535470 relocation changes.
- **0x00AD9B20 / 0x00ADA240:** `CGroupArray` -> `CArray<unsigned long>` (Ego_r's type of `CVertexGroupBlock::Groups`) inside each row's local
  CVertexGroupBlock model; only the 0x00535470 relocation changes.
- **0x00ADCE10:** full port onto CVertexGroupBlock.h / CArray.h / fable_map.h / the header list; own leaf -> the two-parameter STLport list
  spelling the table already binds (2 -> 1); its 0x00A96E40 call onto `??0?$map@KJ...`; form 2 adds FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY.
- **0x009D4810:** onto the shared headers (0x009AC5D0 recipe) with `CArray<unsigned long>` as the mapped type (both PDBs; its one caller
  0x009CF3A0 already binds it): 2 -> 1; callees 0x009D2BA0 / 0x009D4380 onto the CArray tree spellings (count-neutral); its pick-any vector
  copy ctor now EQ 0x00535470 (95 = 95; NEQ at HEAD).
- **0x009D3CA0 / 0x00ADC310** (no callers anywhere in Fable.exe): STLport `_Construct<T, T>` explicit instantiations, `__fastcall` by the
  existing CONSTRUCT_FASTCALL opt-in (the NF87 0x007AE0C0 precedent): 0x009D3CA0 `pair<const CCharString, CArray<unsigned long> >` (its
  0x0099EC30 call onto `??0CCharString@@QAE@ABV0@@Z`), 0x00ADC310 `CVertexGroupBlock` (vector at +0, map at +0xC = the Ego_r layout; both
  PDBs carry the construct helper for CVertexGroupBlock), which fixes its catalog name (nf256's `pair<const vector, map>` reading:
  REJECTED.tsv). Catalog / files renamed.
- **0x009B0AC0 blocked** after 3 forms (all DIFFER 407v404: a dead `add eax, 8` from the inline indexer, esi / edi swapped); unchanged, still
  binds `??0?$CRetailVector@K@@QAE@ABU0@@Z`; next step queued (port onto CDefinitionManager.h's CDefClassInfo, as its landed siblings).
- The first forms of 0x00ADCE10 / 0x00ADC310 newly defined NEQ `map<unsigned long,long>` `_M_erase` / `clear` copies (owners 0x004507A1 /
  0x00451046); TREE_DTOR_DECL_ONLY removes both, RELOCATION_MATCH kept.
- Census: 0x00535470 7 -> 2, 0x009D4810 2 -> 1, 0x00ADCE10 2 -> 1, 0x00A96E40 loses `?ctor@CArrayUL`; nothing gains. Table: 5 deletes, 2
  replaces, 3 keeps. Link (7 new + 4 HEAD objects): 0 LNK2005 / LNK4006 (control 1).
- **Recheck** (`logs/recheck.log`, landed tree): the 7 changed rows + 0x00535470, the blocked 0x009B0AC0, the controls 0x00AD9770 / 0x00AD9980 /
  0x009CF3A0, the callees 0x00A96E40 / 0x009D2BA0 / 0x009D4380 and the unchecked TU-mates 0x00AD9F10 / 0x00ADC330 / 0x00ADC190 / 0x00ADCB90
  (19 rows): 19 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF258's questions
- **D1** (the unowned `??1?$map@KJ` COMDAT now NEQ 0x00AD9770's copy): left. An ownerless NEQ copy is not a hold (standing ruling); mirroring
  TREE_DTOR_DECL_ONLY into 0x00AD9770 / 0x00AD9980 queued.
- **D2:** land 7 of 8 (each move is count-neutral or reducing on its own; 0x00535470 ends on 2 with 0x009B0AC0 queued).
- **D3:** the 0x009D3CA0 / 0x00ADC310 catalog renames to the `_Construct` names: applied.
Queued: 0x009D4380 / 0x009D40B0 (own leaves spell vector<unsigned long> as the mapped type, both PDBs say CArray: a family pass with
0x009D41B0); the 7 rows' header-based tests; L129 to the next agent. Slips (HEAD tests of 0x00AD9B20 / 0x00ADA240 graded despite clang
precheck errors that a two-line VC7.1 `<cstdio>` / `<vector>` control reproduces; GOTCHAS / EXTRA_RULES read by grep): noted.
