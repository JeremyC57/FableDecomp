# 0047E196 SaveGame probes

Selected `sol10_shared22_final`, source `0047e196.cpp`. Target1211 bytes and seven transitively called STLport helpers333 bytes are independently exact. No parked target and no remaining called-helper debt. Seven distinct source families; compile corrections, metadata, namespace portability, and shared-header rebases are not extra families.

|Family|Attempts|Observation|
|---|---|---|
|Baseline diagnostic|sol00|Correct full leaf only; old stand-ins retained as diagnostic.1211 bytes,17 strict hunks. Not a source family.|
|1 Complete shared model|sol01/01b|Replaced all local strings/UI/world/profile/TC/container stand-ins with real shared classes, method names and original native layouts. Compile correction supplied the WORLD dependency. Target remains1211,17 hunks.|
|2 Natural position expression|sol02|Established sibling's X-product parentheses fixes push/FPU scheduling.1211,16 hunks.|
|3 Genuine string visibility|sol03|Actual CCharString constructor/destructor bodies, original PStringData/NoStrings names and real allocation/release. Target inert. Not carried.|
|4 Original locals and lexical lifetimes|sol04/04b|Actual donor PDB shows pComponent directly from CreateComponent, CComponent*/CText* inner scopes, named final pos, and no named message origin. Reconstruct those source lifetimes; exact1211. First compile failed only from mechanical namespace typo; corrected without semantic change.|
|5 Original erase helper profile|sol05|Actual STLport pointer erase body in an ordinary specialization before target's frame-pointer pragma.35 bytes exact. Existing generic body was43 and wrong. Genuine std::__copy_trivial external declaration retains native fastcall ABI; default header unchanged. Target exact.|
|6 Allocator primitive inlining|sol06|Original allocator<CComponent*> allocate/deallocate operations expanded in the overflow helper. Full overflow became139 versus native137, exposing only the generic fill_n convention. Target exact.|
|7 Original generic helper ABIs/profiles|sol07|Native fastcall fill_n and actual list insert/_M_create_node source specializations under their original frame-omitted profile. All eight carried bodies exact. This closes the complete transitive visible helper graph, rather than suppressing differing bodies.|
|Portability / final evidence|sol08/09/10|Put ordinary standard-template specializations inside namespace std for GCC C++03, pin shifted assertion IDs, make new date query work under every date branch, add layout dependency diagnostic, normalize indentation and refresh to merged22. Target/helpers unchanged and exact.|

All prior drafts and direct neighbours were read before authoring; `ROW_READING.json` records immutable input hashes. Every attempt remains under history/0047e196. CCharString visibility was abandoned after the original local/scope reconstruction solved the target without carrying those definitions.

Main review: worker original source used unguarded compiler controls and forced both allocator primitives before the complete ladder was measured. Private diagnostics subsequently tested plain original specialization, /d2inlT80/100/120/180, inline_depth255 scope and definition order: overflow135 retains allocate call; other seven bodies exact. Main replaces raw extensions with portable macros, guards all pragmas, retains force only on allocate, makes deallocate plain inline and declares actual /O1 /Oy- target profile. These are inline-policy/portability validations, not extra target-shape families.
