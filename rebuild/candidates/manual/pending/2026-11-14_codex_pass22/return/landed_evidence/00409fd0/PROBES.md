# 00409fd0 probes

Supplied best was a partial behavior fixture with no Leaf. Real source recovered from prior differ. Baseline/port/final regrade excluded from source-shape attempts. Ten-attempt floor applies only to parking; this row matched.

| Attempt | Result | First diff | Interpretation |
|---|---|---|---|
| sol00_recovered_baseline | COMPILE_FAIL |  | Excluded: current header compilation failure |
| sol01_shared_port | DIFFER 1888v1892 | 1651 | Excluded: port; existing CRC branch has first-loop exit mismatch |
| sol02_crc_early_index | DIFFER 1892v1892 | 1735 | Early zero-length return restores1892bytes; onlysecondSIB remains |
| sol03_crc_pointer | DIFFER 1891v1892 | 1661 | Pointer/end loop changes bothCRCregions |
| sol04_crc_dowhile | DIFFER 1888v1892 | 1660 | Do/while drops redundant zero test |
| sol05_crc_byte_local | DIFFER 1892v1892 | 1735 | Named widened byte leaves sameSIB |
| sol06_crc_seed_local | DIFFER 1888v1892 | 1651 | Separate running result altersfirstloop |
| sol07_crc_from_data | DIFFER 1892v1892 | 1735 | Direct cast indexing diagnosticallysameSIB |
| sol08_crc_shift_first | DIFFER 1892v1892 | 1735 | Named high/low pieces stillsameSIB |
| sol09_accessor_select | DIFFER 1904v1892 | 1701 | Preset-and-assign accessor changes12bytes, notretail |
| sol10_accessor_nullreturn | MASKED_MATCH_PENDING_AUDIT 1892v1892 | None | Existing early-return accessor fixesSIB; match |
| sol11_final_union | MASKED_MATCH_PENDING_AUDIT 1892v1892 | None |  |
| sol12_review_metadata | MASKED_MATCH_PENDING_AUDIT 1892v1892 | None | Excluded: final unified-header regrade; match |

The CRC own56bytecopy also matches. All145relocations includingliteralcontent checked; TransferVector templates proven by bothPDBs andtypedretailcalls. Full SaveGameState body is copied verbatim into portablefixture, with independent expected91entry serialization, FableProheader, bitwiseCRC, <16KBspacepadding and>16KBuntruncated output. GCC64C++03PASS; VC71 /Od/W3noEHscPASS. Final sourcehash unchanged after test.
