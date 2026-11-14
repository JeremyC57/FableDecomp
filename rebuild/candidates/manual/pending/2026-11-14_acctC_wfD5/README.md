# acctC workflow hand-back wfD5 (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00F3BDD0 | CTriangleStripifier2::FillInTriStrip | 2,998 | RELOCATION_MATCH | PASS |
| 0x009D8CF0 | CRenderManager2D::ClearRenderTarget | 3,385 | RELOCATION_MATCH (`/GS`) | PASS |
| 0x009D9C80 | CRenderManager2D::DrawBufferedPolys | 3,435 | RELOCATION_MATCH | PASS |
| 0x00678760 | CPersistTraits<std::list<CReactionMatch*> >::TransferIn | 3,579 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00F3BDD0
Verifier audit: none. No __asm/naked/_emit/volatile, no inline control, no pragmas or flags, no dead guards or unused locals, no local class model (includes only engine/CTriangleStripifier2.h). The header change is the additive opt-in FABLE_RETAIL_CTRIANGLESTRIPIFIER2 (private-helper declarations from the PDB manglings, with addresses); no asserts touched. Note: the behaviour test is a standalone std::vector model of the row's logic checked against hand-traced strips (27 checks). It is real but does not call the compiled row.

Fitted items: FITTED (device a, statement order): the order of the three assignments in each scoring arm (arms 13 and 19 set best_score first, the others last), and last_vertex stored before `reversed` is flipped in the missing-vertex block (that one is also backed by retail store order 0x00F3C90D/0x00F3C911).

## 0x009D8CF0
Verifier audit: none. `// Flags: /GS` is backed by byte evidence (security cookie 0x0139C8A8 read at 0x009D8CF6). No asm, volatile, inline control, pragmas, dead calls or local class models. The file-static GVertexShaderConstantLayoutEmpty is the same text as the landed same-TU sibling 0x009DA9F0. Note: the behaviour test is a standalone model (struct R) of the clear logic with 3 scenarios (flags 0, flags 5, flags 2). It is real but does not call the compiled row.

Fitted items: FITTED (device a): repo opt-ins FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED, _SETRENDERSTATE_BOOKMARK and _ADDSTATE_DIRTY (already fitted in the repo header); CShaderRenderManager::Get() called again at each of the three shader calls instead of through a named reference; a named `display` reference for GetViewport / GetRenderTargetDimensions.

## 0x009D9C80
Verifier audit: No rule violations. vland warnings to report (warnings, not blockers): RELOC_NAME ?D3DTextureUVModeTable@CRenderStateManager@@0PAKA -> 01396dc8 is labelled 'g_textureUVModeToD3D'; RELOC_NAME ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z -> 00a0aa80 is labelled '_global::Forward_SetViewport_via0x3a3c_00a0aa80' (the author's LABELS.tsv proposes the PDB name for it); RELOC_NAME vector<CQuickDrawTriInfo>::erase -> 009e15e0 is labelled 'TextureCache_MoveAndCleanup'; RELOC_ALIAS _qsort -> import thunk MSVCR71.DLL::qsort. The comparator names CompareQuickDrawTriDepth / CompareTriangleAverageDepth match the landed rows. The header use is the additive opt-in FABLE_RETAIL_C2DBOXF_INITIALISE (from agentC43, citing the FableWin PDB Initialise). Note: the behaviour test models only the opaque-list batching and flush plus the clear. It does not cover the alpha-list qsort path and does not call the compiled row.

Fitted items: None marked FITTED. Initialise(0,0,0,0) for the alpha-window reset is marked EVIDENCED (retail 0x009DA64B..0x009DA665).

## 0x00678760
Verifier audit: No hard violations. Review items: (1) Header opt-in FABLE_RETAIL_COPINIONSET_CTOR_INDEX in CTCCreatureOpinionOfHero.h adds a visible COpinionSet() ctor marked `__declspec(noinline)` under #ifdef _MSC_VER. Its evidence is that retail calls 0x004538EB out of line at 0x006789B4 (same pattern as the STLport *_NOINLINE opt-ins). This is an inline-decision device in a shared header, and it has no `// Inline-budget:` marker. (2) Two new header files, engine/CReactionMatch.h (polymorphic base built from the Ego_r layout and RTTI) and engine/CReactionMatch_MoralityRenownBlob.h (absent from struct_layouts_egor.tsv; named by the PDB constructor). Both are additive new files, but they are not FABLE_RETAIL_* opt-ins. (3) The CReactionMatch_AsymmetricPyramid, _MRPyramid, _Sphere, _SecondaryAxisMRShifted and _SecondaryAxisCentredMRShifted headers gain additive FABLE_RETAIL_*_CLASS branches; their default asserts were pinned by FABLE_STATIC_ASSERT_AT conversion (pin_asserts), and the default text is otherwise untouched. (4) The CBasicString<T>::Compare / operator== template texts in the row source are identical to the landed same-TU sibling 0x006C9490. (5) The behaviour test is a standalone token-stream model with 7 scenarios. It is real but does not call the compiled row. NAME_FIXES.tsv corrects 00677620 to ??0CReactionMatch_AsymmetricPyramid@@... (the wrong spelling seen was a ...Retail class name).

Fitted items: FITTED: the float traits written as one named function-scope object `CPersistTraits<float> floatTraits;` (following the landed sibling 0x006C9490); the EOpinion / reaction-type traits stay temporaries.
