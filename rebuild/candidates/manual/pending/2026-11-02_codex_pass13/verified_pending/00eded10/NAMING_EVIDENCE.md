# 00eded10 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 6. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00eded10/NAMING_EVIDENCE.md

# 00eded10 identity and contracts

The row is `NScript::CV_TravellingHeroesScript::Main`, public virtual thiscall, exact leaf `?Main@CV_TravellingHeroesScript@NScript@@UAEXXZ`. Both original PDB name sets contain it (Ego_r donor 00f58d30, FableWin donor 03ac0f10). Retail vtable 012f407c slot 2 points at the assigned row, between the independently identified RegisterMain and Init slots. The registration factory identifies the script as V_TravellingHeroes.

The entire assigned 1411-byte extent ends in ret at 00edf292. All 108 relocated operands were checked independently, including each literal, 13 original nested-class Alloc addresses, scalar allocation, both string constructors, the string destructor, AddEntityScriptBinding, PostAddScriptedEntities, and the binding vtable. The binding vtable slots are GetEntityScriptName and CreateActiveEntityScript; original FableWin names and native slots prove them. Game virtual slots 64 and 558 are PostAddScriptedEntities and SetCreatureGeneratorsEnabled.

The new shared CV_TravellingHeroesScript header retains all original fields and all nested classes' fields. Original Ego_r TPI 0x1134a proves the owner. The nested ParentClass members are protected REFERENCES, not pointers as older neighbouring source had declared. Original types and native allocation/member offsets prove the class models. No existing shared-header default was edited. The owner uses the existing retail CLevelScriptBase, resource-object, string and STLport map headers.

The behavior fixture embeds the exact production Main body with instrumented dependencies. Fifteen scenarios cover normal registration, failure of each allocation individually, and failure of all allocations. It checks all 13 names, factories, parent pointers, flags, temporary lifetimes, final notification order, and the LostBay generation switch. It passes GNU C++03 /fno-rtti and actual VC7.1 /GR-. This is a dependency-isolated behavioral test; native class ABI and emitted symbols are checked by the production compiler/audit.
