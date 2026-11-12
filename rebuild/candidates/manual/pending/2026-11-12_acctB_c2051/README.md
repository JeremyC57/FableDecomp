# acctB hand-back, chunk 2051 (the five smallest remaining rows): 1 matched, 1 near miss in work, 3 not started

Chunk worked one row at a time (project owner's rule). Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x004A6E30 `?Init@CWorld@@UAEXXZ` | RELOCATION_MATCH 2259v2259, test PASS (g++ and VC7.1) | matched at its first full grade. `// Flags: /Gr` (as the landed sibling CWorld::PostInit 0x004A6550's TU) |
| 0x0042EC7C CNewFrontendGameComponent::Run | DIFFER 2179v2191, 587/624 instructions equal (`rows/parked/`) | 18 grades. A continuation agent is on it |
| 0x0087F3A0, 0x00A7AF60, 0x00821A80 | NOT_ATTEMPTED | queued behind 0x0042EC7C in the same directory |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test
runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; three `WARN RELOC_NAME`, all wrong
catalog labels, below). Purity and padded-struct greps clean.

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

## 0x0042EC7C Run (parked state, for reference)

One compiler decision makes the 12 bytes: ours loads the bank-manager address before the `je` of the bank test block
(0x0042EE43); retail loads it inside the bank block and again on the skip path, and keeps it in edi through the main
loop. The return-false block placement, two frame-slot swaps and the edi uses in the init block follow from it.
`NOTES.md` has the measurements and the next ideas.

## How to land

`rows/004a6e30.cpp` + `rows/004a6e30.test.cpp` with `hdr/engine/`, then `payload.json` through your usual vland.
