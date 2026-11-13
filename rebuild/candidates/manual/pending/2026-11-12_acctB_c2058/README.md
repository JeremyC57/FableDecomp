# acctB hand-back, row 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI: matched

| address | result | notes |
|---|---|---|
| 0x00563920 `?DisplayScoreboardGUI@CTCHeroOnlineScoreboard@@AAEXXZ` | RELOCATION_MATCH 3143v3143, test PASS (g++ and VC7.1) | no flags. 15 grades (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win; two `WARN RELOC_CONFLICT` and four `WARN RELOC_NAME` lines (below).

## What closed it

The first draft had the right shape but expanded 17 `~CCountedPointer` calls, the `pair<float,CWideString>` copy
and the outer list `find`, all of which retail calls. One cause: the static frequency in front of the activity
loop was twice retail's. The source is `if (frame) { for ... }`: `frame` was already dereferenced, so VC7.1 removes
the test, but its split still halves the frequency estimate, and every inline decision and the whole 0x120 frame
then come out exact. Also byte-proven: the guard `size() > 0`, and the outer `std::find` result in a named iterator.

## Review items

- **FITTED, please look at this one: `if (frame)`** is a test the compiler folds away. Equivalent folded tests also
  match (`if (list)`, a repeated menu test); tests VC7.1 cannot prove redundant emit code (3151 / 3159 bytes). The
  bytes prove only that the original had one more branch level there. The inner-loop destructor sites score exactly
  74 (the threshold), so the row sits on that edge.
- **RELOC_CONFLICT, landed bindings are wrong:** 0x00429C53 / 0x00429B72 are the `~CCountedPointer<CComponent>` /
  `~CCountedPointer<CAction>` thunks (`jmp ReleaseData`); landed 0x00570332 and 0x0056F615 bind those destructor
  symbols to the ReleaseData bodies 0x004291DE / 0x00429160. `NAME_FIXES.tsv` has all four addresses (13 entries
  in all, among them `ActivityEnumToCharString` 0x0055E7C0, landed as `CNameTable_0055e7c0::GetEntryName`, and the
  nested `CActivity::GetScoreAsString(float)`).
- **PROVEN:** `GetMousePointer` (FableWin COMDAT right after the row, same size); the destructor text through
  `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` (the row also matches without it); the STLport `find` / `__find` bodies.
- **Headers:** two opt-ins (`CTCInventory.h` `_MOUSEPOINTER`, `stlport/fable_algorithm.h` `FABLE_RETAIL_STLP_FIND_INPUT`).
  No made-up names apart from locals.
