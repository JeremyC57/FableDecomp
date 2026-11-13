# Landing fz (2026-11-13): the open items of landing fy closed; two held rows landed

The owner's instruction was "fix these now" for the three items left open after the acctB hand-over (landing fy): the 26 `_B`
opt-in names, the two held rows, and the review items. All header work was done by hand and proven over the whole tree.

## 1. The 26 `_B` opt-in names: none left

| what the name was | names | what was done |
|---|---|---|
| plain alias of an existing opt-in | C3DVECTOR_ROTATE_ANGLES_B, CAISTATEGROUP_JACKDRAGON_B2, CAISTATEGROUP_MINIONSLEEP_B, CCREATUREACTION_UNSHEATHEWEAPONS_B, CDEFSTRING_NOTEQUAL_B, CTCDPARTICLEEMITTER_CREATE_B / _B2 / _B3, CTCHEROSTATS_RENTMONEY_B, CVECTORMAP_INSERT_FORCEINLINE_B, CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR_B, CPIXELFORMAT_B | alias removed, rows respelled to the base name (the UnsheatheWeapons row now includes its two prerequisites itself) |
| second declaration of the same member | CAFILE_ISROOMFORFILE_B, CAISTATEGROUPBASE_MOVETOPOSPROXIMITY_B, CCREATUREACTIONBASE_ANIMCTOR_B, CMESSAGEEVENTMANAGER_FINDRECENT_DECL_B, CTCINVENTORYTRADE_TRADEDESCRIPTION_B | one declaration under the base name |
| the same member as ANOTHER existing opt-in | CAISTATEGROUPBASE_FINDAGUARD_B | it declared `AIAction_FindAGuardAndYellForHelp`, which `..._FINDAGUARD_YELL` already declares: row 0x0097FEA0 uses that |
| a copy of a class model with additions | CGRAPHICFRAME_GETTEXTURE_B | the nested `FLOATTL` additions moved into the base `GETTEXTURE` blocks; the 100-line copy is gone |
| different content, now named for it | CAISTATEGROUP_JACKDRAGON_B -> `..._JACKDRAGON_UPDATEGROUND`; CQUESTMANAGER_SAVEGAMESTATE_B -> `..._SAVEGAMESTATE_CONTEXT`; CTCTAVERNTABLE_LAYOUT_B -> `..._FULL_LAYOUT`; CPERSISTTRAITS_FLOAT_CLASS_B -> `..._FLOAT_CLASS_CONST_DEFAULTVAL` | renamed in header and row |
| one retail inline with two bodies | C3DVECTOR_MAGNITUDE2D_B, CMATRIX3X4_SETMULTIPLY_B, CRGBCOLOUR_GETD3DCOLOR_B | see below |

- **C3DVector::GetMagnitude2D.** The two-statement body (the FableWin spelling) is now the one `FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D`
  selects; rows 0x009711D0, 0x00915B60, 0x0094E7B0 and 0x0097BDA0 all match with it. Row 0x0092ACE0 differs with it at the same size
  (389 v 389), so it keeps the one-expression body under the nested `..._MAGNITUDE2D_ONE_EXPRESSION`, marked FITTED in the header:
  open question is that row's own spelling of `direction /= direction.GetMagnitude2D()`.
- **CMatrix3x4::SetMultiply.** The two bodies were the same text with other parameter names: one definition under
  `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY`; PreMultiply through SetMultiply on a copy is the nested `..._SETMULTIPLY_PREMULTIPLY`.
- **CRGBColour::GetD3DCOLOR.** FableWin.pdb's debug COMDAT 0x02CCFDA0 fills one 0x20-byte slot, like GetAsInt and GetR: it is the
  one-load accessor (`return IntValue`), which is now the only body. The channel packing in CRenderManager2D::Draw2DLine 0x009DD960
  and Draw2DTexture 0x009DC6E0 is those rows' own expression over GetA / GetR / GetG / GetB (both still RELOCATION_MATCH).

## 2. The two held rows: landed

| address | function | bytes | how |
|---|---|---:|---|
| 0x00622A00 | CTCInventoryQuests::OpenInventory | 877 | held since chunk Q24 behind a stand-in; now with the landed ConstructQuestList 0x0061B610 text ahead of it |
| 0x00641C30 | CTCLight::OnAppearanceDraw | 490 | held since wf10 behind an unproven DrawLight; now with the landed DrawLight 0x006414A0 text ahead of it |

Each file carries the callee's landed text unchanged, and each file also grades RELOCATION_MATCH against the callee's own address.
A duplicate `SELECTEDCATEGORYINDEX` block in `NInventory_CTCInventoryBase.h` (left by the unlanded acctB draft of 0x006224C0) made
the first row uncompilable; the second copy is removed (no landed row used the opt-in).

## 3. The review items of landing fy

| item | outcome |
|---|---|
| CAIStateGroupBase::CAIFilterBase modelled twice | one model under `FILTER || FINDAGUARD`: inline empty constructor and non-virtual destructor, virtual `operator()` (FableWin.pdb lists all three). Declared-only constructor / destructor made row 0x009810B0 541 v 521; four inline shapes all match the three rows, the one kept is the one the PDB supports |
| CInventoryDef FULL / TRANSFER (ASSIGN nested) | the two 374-member lists were identical except the enum's namespace; one list, `NInventory::EInventoryIdentifier` as both PDBs mangle it |
| GetDummyObjectPositionAnimated out parameter | `C3DDummyObject&` everywhere (C3DMeshStats.h, C3DMesh2.h); the `DUMMYOBJECT_RESULT` switch is gone |
| five render-switch globals with address suffixes | `g_particleMeshRenderEnabled` / `BlendOpEnabled` / `BlendModeEnabled` / `BatchDrawEnabled` / `TailDrawEnabled` (descriptive, no data symbol in either PDB) in row 0x00BC25F0, its test and the bindings |
| row-local `GFDebugPDynamicCast` template | one definition in the new `engine/GFDebugPDynamicCast.h`; `engine/CThing.h` and rows 0x00BB8EF0 / 0x00BC25F0 include it |
| 0x00B5FAB0 `volatile` | within the owner's ruling of 2026-11-13 (cited `// Retail-volatile:` line); nothing to change |
| 0x00563920 `if (frame)`, 0x006D1250 noinline find | both are declared inline-budget devices with their measurements in the row (`// Inline-split:` / `// Inline-budget:`, EXTRA_RULES, owner decision 2026-11-08); left as landed |
| CMatrix3x4 MULTIPLYNOTRANSLATION "FITTED, needs a ruling" | **ruled by the owner, 2026-11-13: use the body retail TLC favours (the landed one) unless later evidence says otherwise.** Retail bytes favour the landed body (it alone gives 0x00A4E300 its 13 operand orders, and out of line it equals retail's unreferenced 0x00A4B820); FableWin.pdb's debug size favours the 9-sum body, but FableWin is a later source revision. Header comment updated; nothing else changes |

## What the proof found: static-assert typedef names are compiler input

Row 0x007E2A60 CTCFineDialogue::GlobalUpdate (landed in wf17) graded DIFFER(707 v 707, two stack slots swapped) although today's
cleanup had not changed its preprocessed text. Bisecting over header snapshots: it matches at wf21, differs after wf22, matches in
the middle of landing fy, differs at its end. The only textual difference between those trees is the name of the
`FABLE_STATIC_ASSERT` typedefs, `fable_static_assert_<__LINE__>`. Adding six comment lines to `CPlayer.h` in a matching tree flips
the row; so does naming the typedefs by `__COUNTER__`; shifting lines in headers without asserts does not. Five respellings of the
row's two locals do not change the behaviour, so the row itself cannot be made robust by its own text.

Consequences, all applied in this landing:
- `FABLE_STATIC_ASSERT_AT(n, expr)` (rebuild_abi.h) pins a typedef name; `lane_merge/pin_asserts.py` respells every assert an edit
  moved (127 asserts in 9 headers today), so today's cleanup changes no assert name.
- `lane_merge/equiv_check.py` now compares exactly (it used to normalise those names, which is how landing fy missed this).
- Measured exposure: a random sample of 240 header-heavy landed rows all still match at HEAD, so the effect is rare.
- **Row 0x007E2A60 differs at HEAD and before this landing** (since wf22). It is left as landed and listed here: its text is right
  by every other measure, and no header state is canonical. The lasting fix is a line-independent assert name plus one whole-tree
  VC7.1 recheck (about a day of compute); that is a decision for the owner.

## Proof
- Exact comparison (`equiv_check.py`, assert names included) of all 56,185 landed sources and tests against the header tree of
  commit 4fbc61c9a: 56,147 rows compile from the same text as before; 38 rows have changed input (`logs/equiv_exact_rows.tsv`).
- VC7.1 recheck (byte match and behaviour test) on the final tree of those 38 rows, the 18 other rows whose source or test was
  edited, the two new rows and their two callees: 60 of 61 pass (`logs/recheck_final_good.txt`). The one that does not is
  0x007E2A60, whose input this landing did not change (above).
- Random sample of 240 landed rows with eight or more engine includes, rechecked at HEAD: 240 match (`logs/sample_240.txt`).

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
