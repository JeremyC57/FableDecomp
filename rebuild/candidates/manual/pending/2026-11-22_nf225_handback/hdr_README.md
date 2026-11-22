# NF225 hdr/ (base copies in ../orig/, taken from the repo working tree 2026-11-22)
Both overlays are add-only: `diff orig hdr` has no `<` line in either file; every added line sits under `#if` / `#ifdef` of a new
macro that no file in rebuild/, tools/ or docs/ defines, so the default path (and every existing opt-in path) is token-identical.
No other agent overlay holds either header (AUTH12 has none; NF62 / NF73 / NF76 / NF222 overlays are older rounds, not live).

## engine/CCountedPointer.h (entry 1, q12562): five size-body opt-ins, one block after the class template
- FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY     ReleaseData<const CFontBank>  owner 0x0042F948 (44 B)
- FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_SHAREDATA_SIZE_BODY       ShareData<const CFontBank>    owner 0x0042F9F2 (38 B)
- FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_CONVERTING_ASSIGN_SIZE_BODY  ??$?4<CFontBank> of <const CFontBank>  owner 0x0042FE34 (23 B)
- FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_SHAREDATA_SIZE_BODY            ShareData<CFontBank>          owner 0x004190E2 (38 B)
- FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_ASSIGN_SIZE_BODY               ??4<CFontBank>                owner 0x00419463 (23 B)
Explicit specializations (`template<> inline`, the member template one `template<> template<>`) of the header's own bodies
(ReleaseData = the _RELEASEDATA body, Data zeroed first; ShareData / operator= = the defaults) between `#pragma optimize("s", on)` and
`#pragma optimize("", on)`, marked `Fidelity: scaffolding, not retail`. Size-pragma rule (EXTRA_RULES 2026-11-20 / -21):
1. the row's leaf grades the same with the macros removed: leaf and ??1 bytes are identical with and without them (work/e1/syms_draft_*
   v syms_new_*); only the owner COMDATs change;
2. each owner row sets `#pragma optimize("s", on)` itself (0x0042F948, 0x0042F9F2, 0x0042FE34, 0x004190E2, 0x00419463; catalog
   flags /O2), cited in the comment;
3. the comment says the block ends with `optimize("", on)`; neither row carries a row-head optimize pragma.
ReleaseData<CFontBank> (owner 0x004057A0, an /O2 body) needs no new opt-in: row 0x0054ED90 takes the existing
FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (a first draft with a sixth opt-in also matched and was dropped: REJECTED.tsv).
Not with FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22 (no member-template operator= there); the macros must be
defined before the first include of the header (the rows include it first, before engine/CFontManager.h).
pin_asserts.py: pinned 0. assert_names_check.py rebuild/include hdr: mismatches 0.
Includers: 8436 landed rows include engine/CCountedPointer.h (240 headers); none defines the new macros, so none changes.

## engine/NUISystem_CManager.h (entries 2 / 3, q12563 / q12564): two opt-ins after the existing FABLE_RETAIL_CMANAGER_CLEAR line
- FABLE_RETAIL_CMANAGER_CLEAR_INLINE (only `#if ... && !defined(FABLE_RETAIL_CMANAGER_CLEAR)`): `public: static void __fastcall Clear()
  { delete GPUniqueInstance; GPUniqueInstance = 0; }` -- the PDB member (?Clear@CManager@NUISystem@@SIXXZ, public static fastcall),
  inline: Ego_r.pdb and FableWin.pdb S_FRAMEPROC "marked inline", its only copy in new_frontend_game_component.obj. Needs _GPUNIQUEINSTANCE.
- FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE: `protected: static NUISystem::CManager* GPUniqueInstance;` then `public:` (the existing
  CLEAR line carries no access label). ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A in all three PDBs. Declaration only, no layout change.
Users: rows 0x0042DB20 and 0x0042FB40 (both macros). The existing FABLE_RETAIL_CMANAGER_CLEAR user 0x005954AD is untouched (recheck PASS).
pin_asserts.py: pinned 0. assert_names_check.py: mismatches 0. Includers: 274 landed rows and 1 header
(engine/CEnginePrimitive2DSprite.h); none defines the new macros.
