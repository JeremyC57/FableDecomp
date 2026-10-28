# Chunk 528 lessons

## GOTCHAS one-liners
- (chunk 528) STLport vector `_M_insert_overflow` / `_M_fill_insert` / `_M_range_insert` rows close on the 1st-2nd grade once EVERY helper COMDAT retail emitted in the same TU (`__uninitialized_copy`, `__uninitialized_fill_n` / `uninitialized_fill_n`, `__copy_backward`, `fill`, `copy`, `__destroy_aux`) is a same-TU `auto_inline(off)` + `__declspec(noinline)` body; declared-only helpers move new_start / the tags out of the dead fill_len / atend / x slots and merge the epilogues (0xA3D060, 0xA39610, 0xA37F40).
- (chunk 528) A vector element derived from CIVCountedPointeeBase needs the user inline empty `virtual ~CIVCountedPointeeBase() {}`, or VC7.1 adds a dtor call for x_copy (0xA3D190); pair<ulong, C3DMeshVolume> needs CCountedPointer's inline release body so the implicit `~pair` stays an out-of-line call as in retail (0xA35010 -> 0xA326D0).
- (chunk 528) STLport's pair `operator<` is `inline __cdecl` (_STLP_CALL): VC7.1 inlines it in __push_heap / __adjust_heap / __unguarded_partition, but inside an inlined `__median` only the first compare; the rest call the out-of-line COMDAT (0xA41EA0 -> 0xA40250).
- (chunk 528) CIVCountedPointer, Ego_r member set: CIVCountedPointer(T*) adopts (no AddRef; the pointee ctor sets IVRefCount = 1); a template converting ctor from CIVCountedPointer<U>; CIVCountedPointeeBase::Release() deletes at zero and returns the count, which gives the temporaries' `mov edi,[eax+4] ; dec edi ; mov edx,edi ; test edx,edx` release; operator= written as `old = Object; if (old != o.Object) { if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } ... }` gives `dec [ecx+4]` (0xA3BF10).
- (chunk 528) A first deref through a smart pointer's `operator->` is enough for VC7.1 to drop the later null test of the converting ctor / dtor pair (`return trans;` in the transition arm of 0xA3BF10); the arm whose only deref sits inside an `if` keeps the test.
- (chunk 528) Retail inlining a member at one call site that VC7.1 declines (CVectorMap::Add -> push_back in CSoundManagerLH::AddBank 0xA3F140): `__forceinline` both levels.
- (chunk 528) A CCharString terminator written as `stream.WriteNullTerminatedString("")` (implicit conversion) re-forms the temporary's address with `lea`; an explicit `CCharString("")` temporary passes the ctor's eax (0xA2EEF0).
- (chunk 528) Two early `return false` tests whose epilogues retail duplicates are two separate `if (...) return false;` statements; `a == 0 || b() != 0` merges them into one `jne` (0xA3F140).

## Review at landing
- 0x00A3FDC0 is landed as `swap<CSymbolMap::CSymbolInfo>` (a layout twin); this chunk's partition 0xA40DF0 calls it as `swap<pair<pair<CCharString, CCharString>, CSoundBankLH*>>` (NAME_FIXES.tsv). Its only callers (iter_swap 0xA40100 and 0xA40EB8) belong to the NamePairMap sort family. Respell the landed STL_swap_00a3fdc0.cpp at the same landing. vland raised no conflict because reloc_bindings has no row for 0xA3FDC0.
- 0x00A41480 / 0x00A41390 / 0x00A3D060 are bound `QAE` (public) `_M_insert_overflow`. STLport 4.6 declares it protected (`IAE`, 196 landed bindings), and 0xA3F140 / 0xA3D190 / 0xA3D060 here use IAE (vland RELOC_NAME warnings only). Respell the callers 0xA418A0 / 0xA41840 / 0xA3D380 and the landed leaves.
- 0x00A3ED30 (landed CSoundManagerLH::FindByName, descriptive) also carries the bindings `?Find@CSoundManagerLH@@QAEPAUCSoundBankEntry_00a3ed90@@J@Z` (by 0xA3ED90) and `?FindSample@CSoundBankLH@@QAEPAUCSoundEmitterSource@@J@Z` (by 0xAAC1D0). The `J` (long key) is wrong: the body is a CCharString lookup. No proven name exists to put in NAME_FIXES, so this chunk uses the landed FindByName spelling.
- 0x00A438E0 (landed Copy_DataBankTextEntry placeholder) is probably `copy<CLipSyncDataBankEntry*>` of the same chain as 0xA43500..0xA43B20. It has no caller in this chunk, so it is not in NAME_FIXES.
- 0x00A41E20 / 0x00A42060 (the two CSoundManagerLH map sorts) are landed as placeholder free functions (global_SortSElement8 / 12) with junk bindings (GuiVarSetBool, Village_Sub, SetGuiValue); NAME_FIXES.tsv gives the std::sort spellings.

## Why the earlier attempts stalled (chunks 418 / 462 / 484 / 967 / 968)
- The vector rows need the same-TU noinline helper bodies (first lesson above). With declared-only helpers they stay at
  frame-slot DIFFERs that look like lane gaps.
- The catalog labels pointed at the wrong element types: CDisplayModeDesc, CShopItem, CCharStringPair, CSymbolInfo,
  CDataBankTextEntry, Float_Attitude, and CParticleMorphs for a CSoundManagerLH vtable slot. The 12-byte sort family
  and 0xA3F140 only fall into place from the callers (sort 0xA42060 <- CSoundManagerLH commit 0xA3F368) and from RTTI.
- The three parked rows are residues a body-only author cannot close by guesswork (x87 schedule, RMW vs
  load/dec/store, dot operand order).
