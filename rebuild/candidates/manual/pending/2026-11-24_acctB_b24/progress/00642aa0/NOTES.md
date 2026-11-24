# 0x00642AA0 CMinimapData::PopulateMinimapThemeList -- B24A8 extent check only (row not worked)

ORACLE LENGTH IS WRONG (tell coordinator; not re-cut here): the registered oracle is 3376 bytes and spans several functions.
Retail: code 0x00642AA0..0x00642FBF (`ret 8` at 0x00642FBD, 1312 bytes), then the function's own 7-entry switch jump table at
0x00642FC0..0x00642FDB (entries 0x00642BEC, ..BF3, ..BFA, ..C01, ..C08, ..C0F, ..C16; referenced by the `jmp [*4+0x00642FC0]`
at 0x00642BE8 per xref.py), then int3 pad 0x00642FDC..DF; the next function starts at 0x00642FE0.
Correct length: 1340 bytes (0x53C) = code 1312 + table 28. This agrees with the record DIFFER(1308v1340).
