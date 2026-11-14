# acctC hand-back c40b (agentC40, re-cut row, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00564D50 | std::char_traits<char>::copy | 42 | MATCH | PASS |

Re-cut of the reserved row 0x00564D5D (see `2026-11-13_acctC/RECUTS.md`). Verified by the coordinator with
`lane/coordverify.sh` (read-only vland, strict headers) AFTER the start was added to the manifest: `coordverify.log`
(before that it reported OUTSIDE_MANIFEST). Needs the additive opt-in `FABLE_RETAIL_STLP_CHAR_TRAITS_COPY_OUTOFLINE`
(`hdrdiff/fable_string.h.diff`). Name: FableWin.pdb `?copy@?$char_traits@D@std@@SAPADPADPBDI@Z` + the STLport 4.6.2 body.
Nothing in the image references the function.
