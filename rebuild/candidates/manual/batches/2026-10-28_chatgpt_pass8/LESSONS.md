# Lessons

- N014: std::fill over implicitly assigned elements matches with the actual member operator= declaration; /Gr and exact PDB class keys preserve the expected symbols. No stand-in bodies or register tricks needed.

## Review at landing

- N014 006d3af0/00ae5820 have probable, not proven, scalar identities; see ids evidence and NOTES.
- N015: an explicitly empty virtual CIntelligentPointer destructor introduces a second vptr store and merges exits (109 vs116). Let the derived destructor be implicit: the extra store disappears, esi/edi allocation returns and both exit copies match (00739b40,007b00c0). RTTI proves CThing and CThingAICreature separately.

- N046: merged CRC data/length locals rotated scratch registers and shortened caller2bytes; original GetPString/GetLength inline accessors restore exact151.
- N057: explicit empty derived intelligent-pointer destructor adds redundant vptr store; implicit destructor removes it and matches109.
- N041: equal_range shares one address-taken key; separate upper/lower calls with last then first iterator scopes restore late key construction, stack slots and262-byte target. Visible GetSubDef output wrapper permits def pointer promotion.
- N042: inline reference-returning GetParentThing fixes esi/edi priority swap even though the direct member spelling has identical operations.
- STLport nontrivial vector overflow: use the real inline _Destroy(first,last) helper. A direct loop re-reading this->_M_finish reallocates registers and reloads after every dtor; cached endpoints matched N153 all3 (232v248 ->248v248).
- String operator+ first-copy ref increment remains unresolved: N151 inc-memory vs load/inc/store, despite exact remainder. Original IncRef inline helper alone does not resolve it; representative exhausted4grades. Do not claim byte parity from behavioral success.
- Review at landing: CThingCreatureBase::GetPosition006a4d00 is distinct from CThing004c73d0; old008e07f0 binding uses wrong owner. CSymbolMap and CCRCSymbolMap actual RTTI bases are CBaseClassNonCopyable -> CBaseClass; old ctor models use invented CBase. Current owner method models preserve original hierarchy.

Pass8 main N161–N180: use SGI __uninitialized_copy current=result local to avoid split empty-return shape; initialize accumulator before critical-section entry when retail does; original CCharString GetAsCharArray null-first branch can reverse equal-length blocks; unused virtual slots should be explicitly documented projections. Recover counted-pointer DeleteData pointees from own callback setup/allocation/fields, not propagated DestroyWaterGenerator labels. CCompositeSurfaceInfo's final0099a2e0 call is CBaseClass destruction, not CCharString. Preserve original constness (UniqueIDsOpen LowerBound) even when bytes alone cannot distinguish it.
- N209 vector setters: implicit aggregate assignment scheduled dirty load/OR/store around the final vector word (103vs96); an explicit componentwise Vec::operator= yielded exact three-word copy then memory OR (96vs96). Merely changing unsigned int to unsigned long did not help.
- N216 tests: replacing CloseHandle with a local definition collides with CRT kernel32 import; rename the stub only in the standalone test via macro. Retail source retains exact imported CloseHandle symbol.
- N203: a visible noinline Release body fixes escaping guard slots, but an incomplete normal-path body gives false knowledge that EDX survives. Retaining the actual original diagnostic branch restores scratch clobbers and exact410-byte caller; do not add invented side effects.
- N201/N200: inspect destructors as well as constructors. Distinct base-vtable stores prove a genuine abstract base even when the visible object is only three words. Canonical archive types must agree across adjacent rows.
- N214/N215: keep compiler-intrinsic C++ separate from genuine hand-SIMD retail_asm. Working candidate tests do not prove a custom ECX-plus-four-stack-args ABI; blocked rows remain blocked.
- Review at landing: N192 old00bfd0b0 source claims CFanBaseVertex56; actual original type is CWaterFanBaseVertex56. Read-only finding, no out-of-scope edits. N204/N209 legacy CameraConstants and N206 HeadTurn spellings are existing bindings, not recovered semantic roles; these rows are libpk audio.

## N221–N240 and model-audit lessons
- An implicit synthesized predicate dtor can omit the derived-vtable write. Explicit original virtual dtor body restored the write and the EBX reuse across ctor/dtor, solving N2263 at grade2.
- CCountedPointer's user constructors/copy/dtor matter even when the target only passes a factory address: a return-by-value function-pointer test exposed the wrong POD/UDT calling convention. Restoring real declarations fixed N230's actual MSVC test; every affected source was regraded.
- For random timer assignment, use original CTimer::operator=(long). Cache only the definition pointer before the direct rand expression. Naming rand result or duration produced an extra callee-saved register; omitting the pointer cache produced a shorter stream. Representative cap can be exhausted before sibling strategy is learned: preserve cap and record lead, do not silently recompile representative.
- CTimer is long TimerIndex, never infer float storage from its conceptual use. Original enum nesting and original mutable/const signatures must come from namesets, not guessed API conventions.
- Unknown library element identity is not an automatic rejection: explicit CUnproven kind projections are allowed by EXTRA_RULES53–57, provided the real library container and proven operations/layout are preserved and uncertainty is stated.
- Byte/test success does not certify the surrounding declared source model. Complete shared-model audit before grading corrections, especially original pointer base storage, resource/definition chains, class keys, constructor default parameters and virtual-slot positions. Review holds stay excluded from fully approved totals until corrected checks pass.

- Wide-string return copying (0099c120): visible UnassignString alone is insufficient. Include genuine same-TU append, MakeOwnCopyOfData and AllocStringData bodies too, so the local string address is proven not to escape; cached data then survives return-copy and cleanup. Preserve true STLport storage and constructor APIs.

- N182 checked64-bit allocation guard: compound success `count && wide_product <= max32` left an impossible JB; separate failure guards removed it but inverted merged exits. `if(count){if(wide_product>max32)return0;returnAllocate(...);}return0;` preserves native success/failure block order. Proven on3 original typed pool-buffer accessors.

- N135 ECXload+ADD versus EAXload+LEA for Creature trackedbuilding member: true original CAIBrain::GetCreature()const reference-return inline getter fixes scratch-register address calculation; actual visible GetPItem body alone didnot. Twoinstances verified; capped third retained forcoordinator.

- N241116byte countedpointer allocation: original CCPPointerInfo(refcount,fastcall deleter,data) three-argument ctor with ordinarynew expression restores failure/success epilogues where hand-written fields yielded104. Twoinstancesproven; originalsignature foundinPDB nameset.
