# Parked-row retry batch 7

Three genuine C++ recoveries, 524 bytes, independently verified with VC7.1/Wine and actual-source behavior tests. Host g++ fixtures also pass. Final canonical results are in landing_evidence.txt; the corrected shared-header map candidate has its separate dry-run evidence in verify_map_header.log.

- 00992630: counted-pointer and four-string copy helper, 179 bytes. Tests cover replacement, shared ownership, destruction, self-copy, and string-copy order. The retail prefix is 0x20 bytes; the larger enclosing camera subobject is contextual evidence, not the copied extent. No matching shared CCamera header exists.
- 004ff390: CWorldMap::UnloadAllMapsExceptPlayerPos, 164 bytes. Tests cover distance boundaries, unloaded maps, empty arrays, and notification before unload. Uses the matching shared CWorld header. CWorldMap retains a local model because its PDB containers put Maps at +0x24 rather than retail +0x20; retail map records are 0x48 bytes.
- 009577e0: CAIStateGroupBehaviourBase::IsRunnable, 181 bytes. Explicit temporary scope restores retail cleanup and instruction order. Shared headers are included; the opaque intelligent-pointer member and virtual calls use typed retail views. Tests script external query results to cover reset, null/dead targets, rejected predicates, and both perception outcomes.
