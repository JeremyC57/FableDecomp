# acctB hand-back, chunk 2051 (the five smallest remaining rows): 2 matched, 3 not yet authored

Chunk worked one row at a time (project owner's rule). Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x004A6E30 `?Init@CWorld@@UAEXXZ` | RELOCATION_MATCH 2259v2259, test PASS (g++ and VC7.1) | matched at its first full grade. `// Flags: /Gr` (as the landed sibling CWorld::PostInit 0x004A6550's TU) |
| 0x0042EC7C `?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z` | RELOCATION_MATCH 2191v2191, test PASS (g++ and VC7.1) | 34 candidate grades over two sessions. `// Flags: /Gr` |
| 0x0087F3A0, 0x00A7AF60, 0x00821A80 | NOT_ATTEMPTED | 0x0087F3A0 has its retail control flow decoded in `NOTES.md` (no graded draft); all three go to the next batch |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test
runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; `WARN RELOC_NAME` lines are all wrong
catalog labels, below, plus one `RELOC_ALIAS` for the `_rand` import thunk). Purity and padded-struct greps clean.

## 0x004A6E30 CWorld::Init

Same shape as the landed PostInit: each step is StartProfile / DisplayProgress then `P<Member>.reset(new T(...))` on
members typed as Ego_r.pdb declares them (`boost::scoped_ptr<T>` / `CCountedPointer<T>`). With the real implicit
destructors visible, the split between expanded and called `boost::checked_delete<T>` came out by itself. Nothing was
fitted.

Review items:
- **Typed CWorld members are an opt-in** (`FABLE_RETAIL_CWORLD_TYPED_MEMBERS` in the overlay copy of `CWorld.h`). The
  agent's note: this should become the retail default once the 16 landed CWorld-retail includers are respelled. Your
  call.
- **Wrong catalog labels** (the three vland warnings; `LABELS.tsv`, two sources each): 0x006B97E0 is the
  `CFactionManager(const CWorld&)` constructor (catalog `CWorld_InitFactions`); 0x006E8300 is
  `?Init@CCombatManager@@QAEXXZ` (catalog `CCombatManager::ResetDialogue`); 0x004AD9E0 is the STLport
  `__destroy_aux` for `pair<unsigned long, CCountedPointer<CFaction>>`.
- **`NAME_FIXES.tsv`** (wrong landed spellings): the three `checked_delete` stand-ins are
  `??$checked_delete@V<T>@@@boost@@YIX...`; `SetInput`'s enum is nested in `CManager` (`W4EInputType@12@`);
  0x013BAB10 is `CTBaseSingleton<CSkeletalMorphResourceManager>::PClass` (landed as `Instance` and others);
  `CCreatureActionBase::StatRegisterClass` 0x006F5C10; `CanStartAttractMode` 0x00595A03. 0x0129CBA4 has three
  different landed names; one must be chosen.
- **Headers** (`hdr/README.md`): new `CGameNavigatorManagerExternalInterface.h`, `CScriptConversationManager.h`,
  `CSkeletalMorphResourceManager.h`; constructor / implicit-destructor / virtual opt-ins in copies of the manager
  headers Init constructs. No default text and no shared inline body changed. The overlay also carries the headers
  for the parked Run row.

## 0x0042EC7C CNewFrontendGameComponent::Run

What closed the 12 bytes: one statement in the init block. The menu-font glyph string is a named local in its own
block, `{ CWideString glyphs(L"ABC...()"); m_pMenuFont->Preload(glyphs, true); }`. With an unnamed temporary VC7.1
loads the font's vtable before the string constructor and keeps it in a callee-saved register, which pushes the
bank-manager address out of edi for the init block and the main loop; named, the address stays in edi from
0x0042EE7D with the skip-path load at 0x0042EF2E and the reload at 0x0042F20A, as retail. The misplaced return-false
block and the frame-slot swaps were consequences and went with it. FITTED (the full-row grade is the evidence).

Review items:
- **`// Flags: /Gr`** with its evidence in the source: `GPlayMusic` and `InstallationComplete` are defined in this
  file without a calling-convention keyword, yet both PDBs mangle them `YI` (fastcall); the CXMVPlayer statics are `SI`.
- **Forced-inline `CRGBColour` copy constructor** (opt-in `FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY`), with a measured
  `// Inline-budget:` line (size 31, savings 5, cost 26, over the size-TU plain-inline limit of 15; plain copy grades
  568/624).
- **Labels** (vland warnings): 0x00595A03 is `?CanStartAttractMode@CFrontEndManager@NUISystem@@QAE_NXZ` (catalog
  `ReturnFalse_ret0`); 0x013B8394 is `?PSoundManager@@3PAVCASoundManager@@A`; 0x004131D0 binds as
  `??1CMainGameComponentInit@@QAE@XZ` but is labelled `Dtor_CCharStringBase` (probably a folded identical body:
  worth a look).
- **`NAME_FIXES.tsv`** gains five rows: `GFPlayFullScreenXBoxMovie` (takes `NGameText::CDataBank`),
  `CRenderManager2D::ClearRenderTarget`, and the private `ReleaseXMVCode` / `Shutdown` / `CleanUpInterfaces`.
- **Made-up names** (no PDB data symbols): `CXMVPlayer::ClearColour` 0x013961E0, `CXMVPlayer::SkipOnAnyButton`
  0x013CA7E9; fitted bodies `CSystemManager::GetForceMusicPlay` / `ResetForceMusicPlay`, the
  `CFrontendGameComponentInit` constructor, `CMainGameComponent::PeekEngineGraphics`.

## How to land

`rows/004a6e30.*` and `rows/0042ec7c.*` with `hdr/engine/`, then `payload.json` through your usual vland.
