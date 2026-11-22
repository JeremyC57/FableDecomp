# Native VC7.1 experiment log

Baseline: 1844 bytes, retail 1860, 274/550 normalized instructions.

| Probe | Source shape | Native result | Instructions aligned |
|---|---|---:|---:|
| `v4_shared_layout` | Use the private original registration method, scoreboard and STLport headers, and the same-TU /Gr calling convention. | 1848 | 306 |
| `v4_scoped_request` | Keep the protocol request and encryption strings alive together through the transaction setup call. | 1956 | 204 |
| `v4_request_range` | Use a separate concatenation result initialized as a byte iterator range. | 1860 | 316 |
| `v4_switch_session` | Express session assignment as part of status dispatch with cleanup falling through to the common success path. | 1848 | 304 |
| `v4_transaction_guard` | Move failure handling to an early guard and keep successful HTTP cleanup at function exit. | 1844 | 299 |

Selected `v4_request_range`. 1860 versus 1860 bytes. Different STLport constructor and assignment expansion, register homes and cleanup order remain. The original private LogOnToRegistrationServer identity and /Gr flags are verified.

The instruction count measures alignment independently of byte equality. See RESULT.json for the byte grade.

## Password evidence

The donor calls GetSignedInPassword for the second MD5 at +0x359. The retail load here uses the password field at +0x114. The former draft instead hashed the username a second time. The corrected source and RTM behaviour test pass with distinct username and password values. The frame, construction and release of the STLport string still differ.

## Continued main-chat grades

All 1,860 bytes match, including the nine switch destinations. Every one of the 136 leaf relocations checks at its exact offset. All 21 mapped copies and ten additional function bodies were checked independently, including their 126 relocated operands. The native test executes the selected leaf with deterministic collaborators and checks both arguments to encryption and decryption, every return status in the registration switch, setup failure, pending transaction, success, and HTTP reset.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_score20_decrypt | 1860 | 536 | 21 / 0 |
| round2_score_first_only | 1848 | 305 | 9 / 1 |
| round2_score_first_only_flat_session | 1852 | 276 | 9 / 1 |
| round2_score_named | 1892 | 277 | 9 / 1 |
| round2_score_named_flat_session | 1896 | 275 | 8 / 1 |
| round2_score_one_tmp | 1856 | 302 | 9 / 1 |
| round2_score_one_tmp_flat_session | 1860 | 314 | 8 / 1 |
| round2_score_password | 1860 | 317 | 9 / 1 |
| round2_score_password_flat_session | 1868 | 308 | 9 / 1 |
| round2_score_temps | 1860 | 300 | 9 / 1 |
| round2_score_temps_flat_session | 1864 | 318 | 8 / 1 |
| round2_scorebuffers_both | 1832 | 379 | 18 / 2 |
| round2_scorebuffers_explicit | 1848 | 395 | 9 / 1 |
| round2_scorebuffers_plain | 1860 | 315 | 9 / 1 |
| round2_scorebuffers_plus | 1840 | 363 | 9 / 1 |
| round2_scorechar_ccbase | 1840 | 363 | 10 / 0 |
| round2_scorechar_ccbranch | 1840 | 372 | 10 / 0 |
| round2_scorechar_ccchoice | 1832 | 358 | 9 / 1 |
| round2_scorechar_ccctor | 1840 | 363 | 10 / 0 |
| round2_scorechar_ccnull | 1840 | 372 | 10 / 0 |
| round2_scorechar_explicitbase | 1840 | 424 | 20 / 0 |
| round2_scorechar_explicitbranch | 1840 | 398 | 20 / 0 |
| round2_scorechar_explicitchoice | 1836 | 418 | 18 / 2 |
| round2_scorechar_explicitctor | 1840 | 424 | 20 / 0 |
| round2_scorechar_explicitnull | 1840 | 398 | 20 / 0 |
| round2_scorechar_fullbase | 1832 | 379 | 20 / 0 |
| round2_scorechar_fullbranch | 1832 | 388 | 20 / 0 |
| round2_scorechar_fullchoice | 1824 | 374 | 18 / 2 |
| round2_scorechar_fullctor | 1832 | 379 | 20 / 0 |
| round2_scorechar_fullnull | 1832 | 388 | 20 / 0 |
| round2_scoreerase_original | 1860 | 317 | 10 / 0 |
| round2_scoreexit_case | 1860 | 519 | 21 / 0 |
| round2_scoreexit_common | 1860 | 519 | 21 / 0 |
| round2_scoreexit_convert | 1860 | 519 | 21 / 0 |
| round2_scoreexit_order | 1860 | 519 | 21 / 0 |
| round2_scoreexit_original | 1860 | 519 | 21 / 0 |
| round2_scoreexit_prior | 1860 | 519 | 21 / 0 |
| round2_scorelocals_bothlast | 1860 | 300 | 9 / 0 |
| round2_scorelocals_bothplain | 1860 | 300 | 9 / 0 |
| round2_scorelocals_bothsite | 1860 | 300 | 9 / 0 |
| round2_scorelocals_bothstl | 1860 | 300 | 9 / 0 |
| round2_scorelocals_hashlast | 1860 | 291 | 9 / 0 |
| round2_scorelocals_hashplain | 1860 | 283 | 9 / 0 |
| round2_scorelocals_hashsite | 1860 | 283 | 9 / 0 |
| round2_scorelocals_hashstl | 1860 | 283 | 9 / 0 |
| round2_scorelocals_impllast | 1856 | 302 | 9 / 0 |
| round2_scorelocals_implplain | 1852 | 304 | 9 / 0 |
| round2_scorelocals_implsite | 1852 | 304 | 9 / 0 |
| round2_scorelocals_implstl | 1852 | 304 | 9 / 0 |
| round2_scorelocals_keylast | 1860 | 286 | 9 / 0 |
| round2_scorelocals_keyplain | 1856 | 281 | 9 / 0 |
| round2_scorelocals_keysite | 1856 | 281 | 9 / 0 |
| round2_scorelocals_keystl | 1856 | 281 | 9 / 0 |
| round2_scorereq_allocator | 1860 | 519 | 21 / 0 |
| round2_scorereq_early | 1860 | 536 | 21 / 0 |
| round2_scorereq_function | 1860 | 519 | 21 / 0 |
| round2_scorereq_hashparen | 1868 | 435 | 21 / 0 |
| round2_scorereq_hashstatic | 1860 | 519 | 21 / 0 |
| round2_scorereq_keyparen | 1860 | 455 | 21 / 0 |
| round2_scorereq_keystatic | 1860 | 519 | 21 / 0 |
| round2_scorereq_opassign | 1860 | 519 | 21 / 0 |
| round2_scorereq_orig | 1860 | 536 | 21 / 0 |
| round2_scorereq_originalstl | 1860 | 536 | 21 / 0 |
| round2_scorereq_range | 1860 | 519 | 21 / 0 |
| round2_scorereq_ref | 1860 | 519 | 21 / 0 |
| round2_scorereq_selection | 1860 | 513 | 19 / 2 |
| round2_scorereq_static | 1868 | 455 | 21 / 0 |
| round2_scorereq_stl | 1860 | 519 | 21 / 0 |
| round2_scoresiblings_explicitboth | 1836 | 383 | 18 / 2 |
| round2_scoresiblings_explicitdoc | 1852 | 294 | 18 / 2 |
| round2_scoresiblings_explicitplus | 1840 | 320 | 9 / 0 |
| round2_scoresiblings_implicitboth | 1824 | 359 | 18 / 2 |
| round2_scoresiblings_implicitdoc | 1844 | 309 | 18 / 2 |
| round2_scoresiblings_implicitplus | 1832 | 353 | 9 / 0 |
| round2_scoresiblings_namedboth | 1856 | 317 | 19 / 2 |
| round2_scoresiblings_nameddoc | 1856 | 317 | 19 / 2 |
| round2_scoresiblings_namedplus | 1860 | 317 | 10 / 0 |
| round2_scorestate_cc | 1840 | 363 | 11 / 0 |
| round2_scorestate_explicit | 1840 | 424 | 21 / 0 |
| round2_scorestate_full | 1832 | 379 | 21 / 0 |
| round2_scorestate_only | 1852 | 304 | 10 / 0 |
| round2_scorestate_plus | 1832 | 353 | 10 / 0 |
