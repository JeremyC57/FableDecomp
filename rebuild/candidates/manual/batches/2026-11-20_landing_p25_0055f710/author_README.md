# 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest (p25A)

Grade: RELOCATION_MATCH (1122/1122), strict vland PASS (LANDVERIFY_STRICT_HEADERS=1, no warnings), behaviour test PASS.

- Source: `src/0055f710.cpp`. It uses the shared `engine/CTCHeroOnlineScoreboard.h` with the existing MEMBERS + NETWORK opt-ins, so there's no local pad struct and no header change (`hdr.diff` is empty).
- Names:
  - Parameters and locals are the Ego_d.pdb names.
  - Return values are the header's PDB enumerators `ENS_SUCCESS` / `ENS_FAILURE`.
  - `TaskState = 102` stays a literal: ETaskState's enumerator names are in no PDB, and the siblings use 100.
  - The globals use their PDB names, `ConsoleVarScoreboardServerIPAddress` (0x013B8AF0) and `ConsoleVarServerScoresAgent` (0x013B8AF4). These match the landed dynamic initializers 0x0121A750 / 0x0121A770 (Ego_r + FableWin + Anniv360).
- The fix over the codex draft (draft: DIFFER 1121v1122, register roles at the string temporaries):
  - The `std::vector<char> encrypted_body_text;` declaration comes before `unsigned int encrypted_buffer_size = body_text.size() + 32;`. Ego_d's line table shows this order: line 615 is 12 B (the vector ctor), line 616 is 14 B (size call, add 20h, store).
  - It also adds the same-TU sibling opt-in `FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE` (landed in 0x00563480).
- Inline-budget line: `FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE`, a shared header contract. Retail calls the resize COMDAT 0x004122D0.

## Ablation (each define removed in turn)

| removed | grade |
|---|---|
| FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (the inline control) | DIFFER(1122v1122), 7 hunks |
| FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE | DIFFER(1121v1122), 40 hunks |
| FABLE_RETAIL_STLP_VECTOR | DIFFER(1122v1122), 93 hunks |
| FABLE_RETAIL_CCHARSTRING / CWIDESTRING | SRC_FAIL |
| FABLE_RETAIL_CTCBASE | still matches; kept because the header says MEMBERS needs it |
| original declaration order (buffer size first) | DIFFER(1121v1122), 12 hunks |
| FABLE_RETAIL_STLP_STRING_SCOREBOARD, ..._ORIGINAL_ALIASES_PASS22, FABLE_RETAIL_STLP_LIST | unneeded (still matches), so dropped |

## Test

`test/0055f710.cpp` is a model copy (coordinator OK, 2026-11-20), with the row's statements copied verbatim over callee stand-ins, like the landed same-TU tests 0x0055F090 / 0x0055FB80.

- **Why not the real object:** the row calls the anonymous-namespace CryptoMatic 0x0055E2C0 (as the landed StdEncryptString / StdDecryptString do). Its `?A0x<hash>` name depends on the compile path (measured: three paths gave three hashes). A test linked against the real row object can't resolve it, and `/FORCE` is not accepted as a linker directive.
- **What it checks:**
  - the old connection is deleted and replaced;
  - the proxy ip and port (truncated to unsigned short) and the server and port reach the connection;
  - when SetConnectionDetails fails, the row returns 1001 before any header or encryption work;
  - the user-name header is always added, and the login header only for login packets;
  - the encryption gets the body, its size and the key into a zeroed buffer of body+32 bytes;
  - the request is a POST of the page, with the headers, the whole body+32 document and the encrypted length;
  - TaskState is 102 and the return value is 1000;
  - there are no leaks.
- **Mutants:** the mutant run mutated the MODEL text, i.e. the copy of the row's statements inside the test, not the row source. A model-copy test can't kill mutants of the row source itself. So the count (14 of 15 killed, `mutants/mutants.txt`) only measures how well the test's checks catch changes to that logic. The survivor, `resize(encrypted_buffer_size + 1)`, is equivalent: the document copies exactly encrypted_buffer_size bytes, so the extra zero byte in the internal buffer is never observable. Byte identity of the row source itself is established by RELOCATION_MATCH, not by this test.

## NAME_FIXES

The two globals need no fix: the landed spellings are already the PDB names (`CTCHeroOnlineScoreboard_CTCHeroOnlineScoreboard_00562ec0.cpp`, `staticinit_ConsoleVarScoreboardServerIPAddress_0121a750.cpp` / `staticinit_ConsoleVarServerScoresAgent_0121a770.cpp`). The codex draft's `g_ScoreboardServer` / `g_ScoreboardPage` were never landed; this source corrects them.


Five landed same-TU callers declare this function as `ResetAndOpenHTTPAndPrepareRequest(const LHString&, const CCharString&, bool flag)` (public, LHString stand-in). The correct symbol is the private Ego_r spelling in the Leaf line.
