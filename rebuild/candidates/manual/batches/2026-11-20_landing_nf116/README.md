# 2026-11-20 landing nf116: the STLport basic_string<char> member-template family (27 rows)

Records: `naming/2026-11-20_agentNF116/` (selfcheck 0 problems). 5 queue lines over 4 addresses: 3 done (q4186 0x00567400 `append<char*>`, q4187
0x00567D60 `append<const char*>`, q4188 0x00569140 `_M_assign_dispatch<char*>`: every referencing row binds the private AAE spelling), 2 blocked:
family (q4189 = q4192, 0x00567D00 `_M_range_initialize<char*>`: self rename staged; 0x00569E30 / 0x0056A2D0 need 0x00568F10 and the Leaf-less
0x00569A40 to move first). No folds (the pairs 0x00567400 / 0x00567D60 and 0x00567D00 / 0x005672D0 differ in rel32 bytes; the linker kept both).

## Rows (27): 27 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
13 in place (`private:` around the member in the row's existing local STLport model; 0x0083FCE0 also `_M_append` -> `append`); 14 ports onto
stlport/fable_string.h / fable_string_bodies.h and engine/LHHttp2.h, LHHttpHeaders_bodies.h, LHStringFunctions_bodies.h, LSocket.h,
LHHttpHeaderStatus.h, replacing the CRetailString / CNetString_Retail / non-template basic_string stand-ins. 14 self renames incl. two new PDB
names (Ego_r): 0x0083F5C0 LHHttpHeaderStatus::GetLocationHost, 0x0083F600 GetLocationURI. 41 caller respellings.
- 0x00567D00 stays blocked, but 14 of its binders moved with the header ports to the spelling four landed rows already bound (advisor rule:
  existing splits may shrink, none may be created): its table spellings go 4 -> 1.
- Headers: LHHttp2.h `FABLE_RETAIL_LHHTTP2_OPEN`, LSocket.h `FABLE_RETAIL_LSOCKET_LADDRESS_NAMEPORT_CTOR`, LHHttpHeaderStatus.h
  `FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION` (a typed retail branch in a generated header: keep it on regeneration; pin_asserts pinned 8).
  Default path token-identical; assert_names_check 0.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 75 good / 0 bad. No-new-split gate 0;
table spelling sets: 0x00567400 6 -> 1, 0x00567D60 2 -> 1, 0x00569140 3 -> 1, 0x00567D00 4 -> 1, 0x005672D0 3 -> 2, 0x00565360 2 -> 1.
0x0083EBD0 toLowerCase stays the existing two-spelling split (self Leaf is the Dinkumware DU form, the caller now binds STLport DV): NAME_FIXES.
