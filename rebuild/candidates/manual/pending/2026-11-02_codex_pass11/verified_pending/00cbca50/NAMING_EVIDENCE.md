# Identity

Native caller and recursive call identify the SGI introsort loop (threshold 16 and median/partition/depth fallback), corroborated by original STLport 4.6 `_algo.c`. Ego_r PDB template spellings and donor order establish std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> and CKeyPairCompareLess<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>. Native stride 24 and memberwise copy confirm the element layout. Source is /Gr and ptrdiff type int as in STLport x86. Catalog labels were not used as identity proof.

## Layout and identity detail

The caller cbd370 is the persist-info sort path, not the adjacent active-script-pointer vector. Native key comparisons are unsigned 64-bit, stride24, and the value copy contains UID at pair+8 and CCharString at pair+16. Ego_r layout CPersistInfo has UID+0 and PersistScript+8, size16/alignment8; PDB donor pair-copy c9a930 and the surrounding CRuntimeScriptedEntityPersist template run corroborate both the nesting and class keys. Active-script pointer pairs are only16bytes and cannot fit this row.

## Every target relocation

The exact names below are the emitted objdump symbols. Each address comes from the native displacement at the corresponding relocation, not from catalog matching. Original STLport4.6 _algo.c proves the median/partition/partial-sort function names and template parameter order; the PDB pair-copy entries and memberwise native copies prove both copy constructors. Recursion returns to the authorized row. Each distinct mangled spelling maps to one address.

| Native address | Exact symbol |
|---|---|
| 0099ec30 | `??0CCharString@@QAE@ABV0@@Z` |
| 00cb90f0 | `??0?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@QAE@ABU01@@Z` |
| 00cb9d30 | `??$__median@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIABU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@ABU10@00V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z` |
| 00cba580 | `??$__unguarded_partition@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@U12@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@PAU10@0U10@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z` |
| 00cbbeb0 | `??$__partial_sort@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@U12@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@000V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z` |
| 00cbca50 | `??$__introsort_loop@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@U12@HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@00HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z` |

## Verification

The exact full Leaf exists once in objdump -t. Function extent ends at ret12, with no neighboring data included. All nonrelocated bytes match. Self-contained C++03 fixtures pass GNU64 and RTM /GR-; 375 scenarios cover sizes0–511, sorted/reversed/equal/duplicate/random keys, recursion depths0/1/2/4/16, unchanged short partitions, fallback calls, threshold partition order, payload key/value association and full multiset preservation. Final insertion is tested separately because introsort intentionally leaves <=16-element partitions unfinished. Fixtures implement dependency behavior only, not game storage or target bytes.
