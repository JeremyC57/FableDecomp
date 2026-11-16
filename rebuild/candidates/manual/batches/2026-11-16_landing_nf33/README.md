# 2026-11-16 landing nf33: name-fix pass NF33, 79 landed rows respelled (26 renamed)

Owner: "start 2 opus 5.5 agents on naming fix queue now". This is the second half of the queue since NF32 (see
`../2026-11-16_landing_nf34/README.md`). NF33 took tags land1011fj .. land1113fz (522 entries). Records:
`naming/2026-11-16_agentNF33/`.

## Queue (522 entries)

| Outcome | Entries |
|---|---|
| done | 24 |
| done-already | 33 |
| partial | 32 |
| rejected | 1 |
| deferred | 2 |
| carry | 430 |

- **carry** (never reached): land1113fy 382, fv 11, fr 8, fq 7, fx 7, fs 5, fu 3, and one or two each of fj, fp, fw and fz.
- **rejected:** land1011fk's by-value `resize` spelling of 0x00464931. That is FableWin's Dinkumware form; retail STLport takes
  `const T&`, which is applied under land1113fy. The fk spelling of 0x009D3DF0 is rejected the same way.
- **Held by the coordinator:** 0x00747330 (CTCStealth::InitialiseSneakSpeed) stays as landed. The agent had turned its
  pre-existing global `CTCGraphicAppearance` stand-in into a local model of `NGraphicAppearance::CTCGraphicAppearance`. That class
  has a shared header, and CLAUDE.md bars a local model of it. The row includes no header and models CCharString /
  CThingCreatureBase locally, so moving it onto the shared header is authoring work. As a result the GetFullScale entries (land1011fk
  / fp, 0x004BBFF0) are partial, not done: 0x00747330 still binds `?GetScale@CTCGraphicAppearance@@QAEMXZ`.

## Rows (79 landed; 5 tests changed)
- **Self renames (RENAMES_applied.tsv section 1; 26 rows):**
  - onto shared headers: 0x004BBFF0 GetFullScale, 0x0071A730, 0x00747030, 0x00ABFDC0, 0x00810F40, 0x006B2BD0, 0x006B5D80,
    0x004379E0, 0x006A7B90, 0x00A0D980, 0x006DE8B0;
  - access and signature fixes: 0x006DDCD0, 0x006DD7E0, 0x006DEE80, 0x006B0610, 0x00812340, 0x0074E500;
  - no shared header: 0x009D47C0 ~CBankCreatorException, 0x0055D4C0, and the CTCCoopSpirit rows 0x0066FB10 / 0x0066FB30 /
    0x0066FB40 / 0x00670000 (their tests link the row and follow the rename, PassPattern kept);
  - STLport `vector::resize` instances, descriptive-by-library: 0x0049B760, 0x00464931, 0x009D3DF0.
- **Caller respellings (section 2):** 71 lines. They include 24 SetHandled → SetAsPerformedEvent binders, 28
  IsEventGameAction / GetControlMovementFromGameActionEvent binders, and the CTCTrophy, GetFullScale, RemoveFromMapwho, DrawSquare,
  IsHuman and GetPhotojournalDir callers. 0x00581B33 GetSaveDir was a wrong caller hidden behind an ALREADY_OK screen.
- **Recheck (`logs/recheck.log`):** all 79, graded from the repo after landing: 8 MATCH and 71 RELOCATION_MATCH, every test PASS.
- **Header:** engine/NPlayerGui_CDrawNPCName.h `FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE`, a declaration only.
- **Bindings:** `naming/rebind.py` removed 153 lines and added 138. bindings_multi_check is at baseline, and
  funclet_includes_check reports 1,355 checked, 0 broken.

## Addresses still bound under two names (all recorded as partial or deferred)
- **0x00464931 / 0x0049B760 / 0x009D3DF0:** `CArray` / `X_009ceae0` stand-in binders (seven for 0x00464931).
- **0x004BBFF0:** 0x00747330, held above.
- **0x00A0D980 FormatDateTime:** 0x0047EEE4 still binds the free function. NF33 deferred it to NF34, which never reached it.

## Other open items (NOTES.md)
- **Derived-class binders:** 16 for 0x00687CF0 and 2 for 0x00687D00, waiting on engine/CInputProcessGameBase.h.
- **0x0075A530:** binds the old stand-in name of 0x00A358F0.
- **0x009CC750:** partial / PORT.
- **NF32 carry-overs:** the fm PORT / AUTHORING entries and the fn `GFSerialise*` template rows were not attempted.
- **Outside the queue:** 0x00657280 is landed as `CAIStateGroup_BuyFromSeller::BoughtSomething`. Both PDBs make it
  `NPlayerGui::CDrawNPCName::ShowScreenshotMessage`, and the new header opt-in declares exactly that.
