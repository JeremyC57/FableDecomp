# Chunk H40j (Opus 5.5): 23 of the longest parked rows, tenth pass (resumed after the pause)

One row matched and landed in landing cg:
- 0x00E40E80 NScript::CV_BordelloScript::CMagicman::Main, RELOCATION_MATCH, 15,104 bytes.
- What fixed it: dropping the out-of-line STB destructor opt-in. With the implicit destructor, retail's two late epilogue copies expand `~seh_me`.
- It grades against the repo headers alone.

Seven more rows improved:

| Row | Masked diff lines |
|---|---|
| 0x00E3BB70 | 595 -> 198 |
| 0x00D973D0 | 1170 -> 190 |
| 0x00E5FD40 | 1057 -> 163 |
| 0x00D13580 | 280 -> 119 |
| 0x00DB97A0 | 248 -> 125 |
| 0x00E4B5B0 | 697 -> 591 |
| 0x00CF68C0 | 1472 -> 864 |

0x006096E0 now compiles (12864v12832).

- Open: 0x00603000 never finishes compiling within 900 s, and 0x00502E90 / 0x0078FD60 do not compile on HEAD (header debt).
- Some differ/ drafts still spell bare `__forceinline`. Respell them FABLE_FORCEINLINE before any of them is promoted.
- The agent's hdr/ overlay is stale against HEAD: several of its copies would remove repo lines. Only hdr/README.md is filed here. Its two new draft opt-ins for 0x006096E0 (FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN, FABLE_RETAIL_CTCINVENTORYITEM_STARTINGRIGHTHANDEDSET) must be re-applied to current headers when that row is next worked.
