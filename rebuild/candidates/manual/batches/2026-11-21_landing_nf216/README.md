# Landing nf216 (Account A, 2026-11-21): q12378, eight caller rows of CBasicString<char>::operator== onto fable_string.h

Agent NF216 (owner, 2026-11-21: "you can continue the naming queue agent too"). Hand-back, state files and blocked drafts:
`pending/2026-11-21_nf216_handback/`.

- 8 rows land, all RELOCATION_MATCH, every test PASS (`logs/recheck.log`; the owners 0x004115A0 / 0x00411570 / 0x00403A10 regraded too).
  - 0x004A0D90 CWorld::LoadQuestsFromFile, 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols: TEXT_EQ_NOINLINE -> TEXT_EQ_NOINLINE_COMPARE_ZERO.
  - 0x007D2CC0 CTCTavernGameCoinGolf::EnableCourse: the same swap plus COMPARE_BODY (its local Compare specialization is gone).
  - 0x00870E00 / 0x00871290 SetDataFromString: their local `==` specializations are gone.
  - 0x004FDCB0 CWorldMap::IsSectionToHaveNavigationMap, 0x009AF1E0 CDefinitionManager::CompileString: local CBasicString / CCharStringData /
    CCharString models replaced by fable_string.h. 0x009AF1E0's 0x0129AAF4 binds `?NullChar@CCharString@@1DB` (was g_emptyLiteral_0129aaf4).
  - 0x00870FC0 NCreatureAction::CActionEventCameraShake: on fable_string.h + engine/NCreatureActionEvents.h; self leaf `UAE` -> `MAE` (both PDBs;
    no landed row references 0x00870FC0). Catalog name `vfunc_4` -> `SetDataFromString` (both PDBs); source and test files renamed.
- No header change (the rows use nf215's TEXT_EQ_NOINLINE_COMPARE_ZERO + COMPARE_BODY), so the recheck is the rows and their owners only.
- Pick-any (NF216, every object): the `==` copy is the owner's 88-byte body in bytes and relocation targets; Compare equals 0x00411570;
  0x009AF1E0's GetAsCharArray equals 0x00403A10. Binding diff vs HEAD: only the 0x0129AAF4 respelling.
- Link (`logs/link_check.txt`): `==` duplicates 4 -> 0; Compare 4 -> 9 against the strong explicit specialization 0x00411570 (accepted debt).
- reloc_bindings (`logs/tablecheck.txt`): 0x00870C90 loses its struct-CCharString stand-in (now one spelling).
- Not landed (queued, tag land1121nf216): 0x004FE630 (DIFFER(453v440)), the size-compiled 0x00CBFB7D / 0x005A5B31 / 0x005A04F7 /
  0x0065A075 (FINDINGS 2026-11-21: a pick-any template copy takes the optimize mode in force at the end of the TU), and 0x005A4CDD
  (a passing draft, `work/005a4cdd_f1.cpp`, held for a ruling on its inline explicit specialization between optimize("t") / ("s")).
