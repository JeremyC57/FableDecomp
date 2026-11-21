# NF214 overlay headers (add-only opt-ins; base = repo working tree at the start of NF214, copies in ../orig/)

## engine/C3DMeshStats.h (copy of rebuild/include/engine/C3DMeshStats.h)
- New opt-in `FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES`, placed right after the class head (`public:`), 14 added lines, nothing else changed.
  Declarations only (no layout change); the block ends with `public:` so the members after it keep their access. Added
  declarations and their PDB spellings (each in both ghidra_out/egor_pdb_names.tsv and fablewin_pdb_names.tsv):
  - `void GetAllDummyObjectNames(std::vector<CCharString>&, bool) const;` (public) =
    `?GetAllDummyObjectNames@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_N@Z` (0x00A2F0F0)
  - `void GetAllHelperPointNames(std::vector<CCharString>&, bool) const;` (public) =
    `?GetAllHelperPointNames@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_N@Z` (0x00A2F120)
  - `void GetDummyObjectNamesStartingWith(std::vector<CCharString>&, const CCharString&, bool) const;` (public) =
    `?GetDummyObjectNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@_N@Z` (0x00A2F150)
  - `void GetHelperPointNamesStartingWith(std::vector<CCharString>&, const CCharString&, bool) const;` (public) =
    `?GetHelperPointNamesStartingWith@C3DMeshStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@ABVCCharString@@_N@Z` (0x00A2F170; declared for completeness: no object in this hand-back defines or binds it; the spelling rests on both PDB lines and the same signature as the object-verified GetDummyObjectNamesStartingWith; its landed row still models the class locally)
  - `void UnpackNames(std::vector<CCharString>&, bool, const CCharString&, bool) const;` (private) =
    `?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@_NABVCCharString@@1@Z` (0x00A2ED10)
  Four of the five are proven by objects (0x00A2F0F0 / 0x00A2F120 / 0x00A2F150 define their leaves and bind UnpackNames; 0x00A2ED10 defines
  UnpackNames from its local model with the same spelling; vland work/b1.log, binds.py pairs); the fifth is noted above.
- std::vector comes from fable_string.h -> stlport/fable_string.h -> stlport/fable_containers.h (already included by the header).
- Default path token-identical: without the macro the preprocessed header is the repo header (the new lines are all inside the #ifdef).
- pin_asserts.py: `engine/C3DMeshStats.h pinned 0` (its one assert is FABLE_STATIC_ASSERT_AT(86, ...), id-pinned; not moved by hand).
- No other agent overlay holds this header (no $S/w22/agentNF2*/hdr copy). No macro of that name exists anywhere under rebuild/.
- Header users: landed direct includers (grep -rlF 'engine/C3DMeshStats.h' rebuild/src/compiled): 0x00B39080, 0x00680050,
  0x00BF12A0, 0x00BF18B0, 0x00670D50, 0x00A36100, 0x004C24C0; header includers engine/C3DMeshObject.h, engine/C3DMesh2.h. None
  defines FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES, so the opt-in changes none of them.
- Users of the opt-in: rows/00a2f0f0.cpp, rows/00a2f120.cpp, rows/00a2f150.cpp (each includes engine/C3DMeshStats.h directly).

## stlport/fable_containers.h (copy of rebuild/include/stlport/fable_containers.h)
- New opt-in `FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY` (22 added lines), placed directly before NF213's landed
  `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY` block; NF213's blocks are token-identical (their fill_n declaration
  becomes a redeclaration of the defined specialization). Added definition: the explicit specialization
  `template<> inline __declspec(noinline) NUISystem::CComponent** FABLE_FASTCALL fill_n(NUISystem::CComponent**, unsigned int, NUISystem::CComponent* const&)`
  = `??$fill_n@PAPAVCComponent@NUISystem@@IPAV12@@std@@YIPAPAVCComponent@NUISystem@@PAPAV12@IABQAV12@@Z` (an STLport instance: no PDB
  holds STL instance names; this is the spelling the repo header already produces and that the owner 0x00485DBC's copy calls,
  reloc_bindings line -> 0x0048514C). Under `#pragma optimize("s", on)` ... `#pragma optimize("", on)` (size-pragma scaffolding,
  `Fidelity: scaffolding, not retail`). EXTRA_RULES 2026-11-20 size-pragma conditions: (1) control compile with the two pragma lines
  removed (scratch overlay, deleted after): 0x007E4B60 still RELOCATION_MATCH, its Activate body byte- and reloc-identical (165 = 165),
  the overflow copy unchanged (137 = 137), and only the fill_n COMDAT changes (30 bytes /O2 -> 23 with the pragma); the same control on
  0x006116E0: RELOCATION_MATCH, OpenInventory body identical (535 = 535, bytes / relocs True), overflow 137 = 137, fill_n 30 v 23; (2) the owner row
  0x0048514C sets `#pragma optimize("s", on)` itself; (3) the comment says the block ends with `optimize("", on)`. The body's
  visibility (not the pragma) is what the callers need: declaration-only, 0x007E4B60 DIFFERs 158v165 (NF213). Needs
  FABLE_RETAIL_STLP_FILL_N_FASTCALL.
- Default path token-identical (everything inside the #ifdef). pin_asserts.py: `stlport/fable_containers.h pinned 0`.
- No other running agent overlays this header (NF213's work/hdr_d copy is a read-only diagnostic, not a hand-back).
- Header users: 2186 landed rows include stlport/fable_containers.h directly (grep -rlF); none defines
  FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY (grep of rebuild/), so the opt-in changes none of them. 0x005414A0 (NF213; its test
  defines its own fill_n specialization) does not define the macro and is unaffected.
- Users of the opt-in: rows/007e4b60.cpp, rows/006116e0.cpp (each includes stlport/fable_containers.h directly, before any repo header
  that includes it, per the NF213 include-order lesson).
