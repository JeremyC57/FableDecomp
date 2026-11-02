# Identity

Native caller and recursive call identify the SGI introsort loop (threshold 16 and median/partition/depth fallback), corroborated by original STLport 4.6 `_algo.c`. Ego_r PDB template spellings and donor order establish std::pair<CCharString,CScriptInfo> and CKeyPairCompareLess<CCharString,CScriptInfo>. Native stride 24 and memberwise copy confirm the element layout. Source is /Gr and ptrdiff type int as in STLport x86. Catalog labels were not used as identity proof.

## Layout and identity detail

The caller at cb7780 computes __lg(last-first)*2 then calls this target at cb77be and final insertion sort. The recursive call is cb7640. Native stride is 24; the copy writes the first CCharString at +0, then CScriptInfo callbacks at +4/+8, Name at +12, ID at +16 and MasterScript at +20. Ego_r struct_layouts entries CScriptInfo (20 bytes) and pair<CCharString,CScriptInfo> (24 bytes), plus PDB pair-copy symbol at donor c97990 and the surrounding script-manager template run, independently prove these types. The old source bound IntrosortLoop_00cb7310 over SElement24 and long; that label is wrong. x86 STLport distance_type is int.

## Every target relocation

The exact names below are the emitted objdump symbols. Each address comes from the native displacement at the corresponding relocation, not from catalog matching. Original STLport4.6 _algo.c proves the median/partition/partial-sort function names and template parameter order; the PDB pair-copy entries and memberwise native copies prove both copy constructors. Recursion returns to the authorized row. Each distinct mangled spelling maps to one address.

| Native address | Exact symbol |
|---|---|
| 0099ec30 | `??0CCharString@@QAE@ABV0@@Z` |
| 00cb5f50 | `??0?$pair@VCCharString@@VCScriptInfo@@@std@@QAE@ABU01@@Z` |
| 00cb66e0 | `??$__median@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIABU?$pair@VCCharString@@VCScriptInfo@@@0@ABU10@00V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z` |
| 00cb67b0 | `??$__unguarded_partition@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIPAU?$pair@VCCharString@@VCScriptInfo@@@0@PAU10@0U10@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z` |
| 00cb7140 | `??$__partial_sort@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@000V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z` |
| 00cb7310 | `??$__introsort_loop@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@U12@HV?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@00HV?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z` |

## Verification

The exact full Leaf exists once in objdump -t. Function extent ends at ret12, with no neighboring data included. All nonrelocated bytes match. Self-contained C++03 fixtures pass GNU64 and RTM /GR-; 375 scenarios cover sizes0–511, sorted/reversed/equal/duplicate/random keys, recursion depths0/1/2/4/16, unchanged short partitions, fallback calls, threshold partition order, payload key/value association and full multiset preservation. Final insertion is tested separately because introsort intentionally leaves <=16-element partitions unfinished. Fixtures implement dependency behavior only, not game storage or target bytes.
