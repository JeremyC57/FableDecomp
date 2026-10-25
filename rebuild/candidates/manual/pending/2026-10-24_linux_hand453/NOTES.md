# Hand chunk 453 (_global .. _global): 33 of 40 byte-exact

All 40 rows are catalogued `_global`. Their real owners are NScript quest/entity scripts, STLport containers of
def / engine records, CRT helpers and the CTCInventoryItem static initialisers. The names below are the real ones.
Where the PDB has no name for a row, it says "descriptive name".

## What matched (33)

- **NScript quest-script Alloc factories (5).** These are 0x00DDACB0 CQ_PrisonEscapeRescueMotherScript, 0x00DFA020
  CQ_TraderConflictEvilScript, 0x00E0AF60 CQ_TraderEscortScript, 0x00E0E760 CQ_UndeadRisingScript and 0x00E81EB0
  CV_FishingCompetitionScript. Each does `new T(data, game)` and inlines the constructor.
  - Shape: retail offsets are the Ego_r offsets minus 0x14. CScriptBase is 0x40 bytes with its ctor at 0x00CB8110.
  - The argument pair is stored by an inlined `__declspec(novtable)` CLevelScriptBase<TData> before the derived
    vtable store.
  - CTimer members register through `g_pGameScriptInterface->RegisterTimer()`, slot 87, in the landed 88-slot model.
  - STLport vector members are 12 bytes. They are modelled with `_Vector_base` + `_STLP_alloc_proxy`.
- **NScript entity-script constructors (3).** These are 0x00DC3DF0
  CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton, 0x00E83920 CV_FisticuffsClubScript::CFisticuffsMember
  and 0x00ED7D50 CV_TalentlessBardScript::CTB_Bard.
  - Shape: `CEntityScriptBase(me, parent.Game)` stores Game before the base vtable.
  - ParentClass is an initialiser. PMasterData is a body assignment and is stored last.
  - CCharString arrays have a ctor and no dtor in the TU, so they become an inline counted loop.
- **NScript::CV_TourGuideScript ctor (0x00EE41F0).** Shape: WaypointData[18] (3 CCharStrings each), three
  CCharString[5] arrays and a CScriptThing. All of them are inline counted loops or inline ctors after the novtable
  base pair.
- **CV_BodyGuardScript::CBodyGuard::GetHenchmanGoldCost (0x00E5FC70; descriptive name).**
  - It is a dense switch: cases 0/2 return the cheap cost, cases 1/3 the expensive cost, the default returns 0.
  - The jump table pairing was verified by hand.
  - The retail CScriptDef offsets are the PDB offsets minus 0x34.
- **CV_FishingCompetitionScript::GetNextPrizeWeight (0x00E7F7D0; descriptive name).**
  - Shape: `next = PrizeNumber + 1; if (PrizeNumber == -1) ++next; return FishPrizeWeights[next];`.
  - The `?:` form folds the +1 into the fld displacement and does not match.
- **std::_Vector_base<CScriptThing>(n, a) (0x00DFDE90).** The ctor's out-of-line instantiation: three zeros, then
  `n ? malloc(n*12) : 0`.
- **std::vector<CQ_AmbushTradersScript::TeleportData>::push_back (0x00F0D490).** Shape: a `__false_type` tag with no
  user ctor is zero-stored in the `push ecx` slot.
- **std::vector<CArenaWaveDef / CArenaCreatureDef / CHeroSoulsCreatureDef> copy ctors (0x00F25BB0, 0x00F25D20,
  0x00F26FB0).**
  - Shape: `_Vector_base(n, a)` stays out of line.
  - The `__uninitialized_copy` loop is inline, with a placement-new null test, around the out-of-line element copy
    ctor.
- **std::vector<CHeroSoulsCreatureDef / CFeatDef>::erase(first, last) (0x00F27780, 0x00F345F0).** Shape: a
  `__forceinline` copy -> `__copy_ptrs(__false_type)` -> `__copy` chain calling the out-of-line element operator=, then
  a virtual-dtor destroy loop.
- **CCutsceneDef::Transfer (0x00F2A1D0).** Eight `TransferVectorBinary` (0x004331F9) calls in source order.
- **std::__copy_ptrs / __copy_backward_ptrs<CFireHeartPatternDef*> (0x00F33EA0, 0x00F34110).**
  - Shape: these are `__fastcall` templates.
  - `__copy` / `__copy_backward` and CFireHeartPatternDef's memberwise operator= need `__forceinline`. Plain inline
    leaves them as separate COMDATs.
- **CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize (0x00F342B0).**
  - Shape: `TransVector->resize(count)` with a `__forceinline resize(n) { resize(n, _Tp()); }`. Plain inline turns
    Resize into a `jmp`.
  - The CResource <- CDefPointeeBase <- CDefClassBase chain is modelled locally.
- **CGPUScratchBufferTextureManager::CResourcePage containers (0x00F38440, 0x00F384E0, 0x00F38770, 0x00F38860).**
  These are `__uninitialized_copy`, `__uninitialized_fill_n`, `_M_insert_overflow` and `push_back`.
  - Shape: STLport 4.6 `__copy_trivial` is `last == first ? result : memmove(...) + n`. The `n ? ...` form gives
    `sub ; je` instead of `cmp ; je ; sub` and does not match.
  - vector<long>'s copy ctor is inlined inside the two `__fastcall` helpers. In _M_insert_overflow and push_back it is
    called out of line (0x00514160). VC7.1 makes that same split on its own.
  - Empty tag temporaries are homed on live argument slots because the callee template bodies are in the same TU.
- **CRangeCompressor::CScriptEntry containers (0x00F3B1D0 __uninitialized_copy, 0x00F3B330 _M_insert_overflow).**
  - The 24-byte all-scalar entry has no type traits in VC7.1, so it takes the `__false_type` paths. Its implicit copy
    is one block copy through esi/edi/ebx.
  - `_M_clear` is inline: its trivial destroy loop vanishes, leaving `if (p) free(p)`.
- **std::vector<unsigned short>::operator= (0x00F3B640) and CArray<unsigned short> helpers (0x00F3B760
  __uninitialized_copy, 0x00F3B7D0 __uninitialized_fill_n).** Shape: the real STLport 4.6 operator= with
  `__type_traits<unsigned short>` = `__true_type`. The helpers `_M_allocate_and_copy`, `_M_clear`, `__copy_ptrs` and
  `__uninitialized_copy` are all inline. Two of the memmove calls are tail-merged.
- **CTCInventoryItem orientation CRightHandedSet dynamic initialisers, cases 6 and 7 (0x0121AD20, 0x0121AD50).**
  - These are the `$E` initialisers of two of the 12 globals copied by GetRHSetFromOrientation 0x005D8EF0.
  - Shape: VC7.1 folds the constant words into the object's static image and keeps only the sqrt(0.5)-derived words
    dynamic.
  - That folding needs C3DVector's memberwise *user* copy ctor. With the implicit bitwise copy, the constants go
    through the temporaries and nothing is folded (115 bytes v 40).

## Parked for Fable 5.1 (7)

- 0x00F36F40 CGPUScratchBufferTextureManager::FreeResourceEntry DIFFER(233v233): erase's `position + 1 != end()` emits `cmp ecx,eax` where retail has `cmp eax,ecx`; every other byte matches | tried: reference push_back with if-guarded deletes (281), unguarded deletes (275), by-value push_back (242), same-TU noinline `_M_insert_overflow` stand-in (233) | next: use the STLport 4.6 `__copy_trivial` (`__last == __first ? __result : ...`), which fixed the same compare at 0x00F38440 (4-grade budget spent)
- 0x00F3FB96 CRT::__ehvec_ctor (`eh vector constructor iterator') DIFFER(98v74): no byte differs; the catalogue row stops at `ret 14h`, but VC7.1 emits the 24-byte `__finally` funclet (retail 0x00F3FBE0..0x00F3FBF7) in the same COMDAT, and it matches too | tried: CRT ehvecctr.cpp body under `#pragma optimize("s", on)` (gives __SEH_prolog and the unrotated loop) with pointer-to-member ctor/dtor in place of the `__thiscall` pointers | next: re-cut the row to 98 bytes and land (the test is differ/00f3fb96.test.cpp, PASS)
- 0x00F3FC50 CRT::__alldiv DIFFER(112v170): retail is MASM alldiv.asm (`or eax,eax ; jge` sign fix-ups, `shr/rcr` normalising loop); ours is a C++ quotient calling the helper | tried: C++ reference body | next: lane gap: CRT MASM (retail_asm candidate, not compiler output)
- 0x00F3FD10 CRT::__allshr DIFFER(119v33): retail `shrd eax,edx,cl ; sar edx,cl` with values in edx:eax and the count in cl; ours is a cdecl halves-based shift | tried: C++ reference body | next: lane gap: CRT MASM (register calling convention with no C++ spelling)
- 0x00F3FD50 CRT::__aulldvrm DIFFER(72v149): retail returns the quotient in edx:eax and the remainder in ebx:ecx; ours is a C++ reference body | tried: C++ reference body | next: lane gap: CRT MASM (two-register-pair return)
- 0x0121AC30 _global::$E2 (CRightHandedSet orientation case 1, 0x013B8BCC) DIFFER(66v64): retail negates sqrt(0.5) twice (`fld st(0) ; fchs` for Forward.X, `fld st(1) ; fchs` shared by Up.Y/Forward.Y); ours CSEs one `-s` for all three | tried: `C3DVector(s,-s,0), C3DVector(-s,-s,0)` with the matching ad20 model | next: find a source spelling where Forward.X is a different expression from the Y words, e.g. a helper or macro taking the components as floats, or a sibling row in this TU
- 0x0121ACD0 _global::$E2 (CRightHandedSet orientation case 5, 0x013B8BE4) DIFFER(66v70): same CSE gap; retail Up.X/Forward.X share one `-s` and Forward.Y has its own `fld st(1) ; fchs`; the Forward temporary sits at esp+0 | tried: natural form (66), `-(float)sqrt(0.5)` (70, rounds through memory), `Up ^ (0,0,-1)` cross product (114, keeps fmuls) | next: same as 0x0121AC30; solve both together

## Skipped

None. Every row is one function. 0x00E5FC70 has a 16-byte jump table after its code, inside the row, which is still
one function. 0x00F3FB96's row is cut *short*, not long (see Parked).

## Label corrections

| Address | Catalog / manifest label | Real identity |
|---|---|---|
| 0x00DC3DF0 | "CScriptThing_Ctor" | CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton ctor |
| 0x00E83920 | "ScriptThingContext_Construct_A" | CV_FisticuffsClubScript::CFisticuffsMember ctor |
| 0x00ED7D50 | "ScriptThingContext_Construct_B" | CV_TalentlessBardScript::CTB_Bard ctor |
| 0x00EE41F0 | "HeroMorphDefScreen_Construct" | NScript::CV_TourGuideScript ctor |
| 0x00DDACB0 / DFA020 / E0AF60 / E0E760 / E81EB0 | "Alloc" (no class) | the quest-script Allocs named above |
| 0x00DFDE90 | "Std_Vector_Initialize" | `std::_Vector_base<CScriptThing>(n, a)` |
| 0x00E5FC70 | "GetHenchmanCostByAlignment" | CBodyGuard member (descriptive name) |
| 0x00E7F7D0 | "GetFloatTableEntryByIndex" | CV_FishingCompetitionScript member (descriptive name) |
| 0x00F25BB0 / F25D20 / F26FB0 | "Container_*" | vector copy ctors |
| 0x00F25C60 | "CParentDefClassBase" | CArenaWaveDef copy ctor |
| 0x00F25DD0 | "COpinionDeedReactionDef" | CArenaCreatureDef copy ctor |
| 0x00F27060 / 0x00F27190 | `pair<CCharString,CSymbolMap::CSymbolInfo>` | CHeroSoulsCreatureDef copy ctor / operator= |
| 0x00F27780 | "vector_erase_impl_pair_CCharString_CSymbolInfo" | vector<CHeroSoulsCreatureDef>::erase |
| 0x00F2A1D0 | "InitialiseDefaultCameraTags" | CCutsceneDef::Transfer |
| 0x00F33EA0 / 0x00F34110 | "vector_shift_down/up_CThingBuildingDef" | `__copy_ptrs` / `__copy_backward_ptrs<CFireHeartPatternDef*>` |
| 0x00431F10 | "CThingBuildingDef::operator=" | CDefClassBase::operator= |
| 0x00462E78 | "CSimpleMemoryPool_AssignFrom" | vector<bool>::operator= |
| 0x00F342B0 | "CRandomAppearanceMorph_TextureMorphs_Init" | TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize |
| 0x00F33DE0 | `vector<CVectorMap<..CTextureMorph..>>::resize` | vector<CFireHeartPatternDef>::resize |
| 0x0045036F | "__EH_epilog3" | the ICF-shared `_STLP_alloc_proxy(const alloc&, T*)` ctor |
| 0x00F36F40 | "TexturePool_FreeSlot"; the landed FreeMemoryPage 0x00F37030 calls it "FreeTextureSlot" | CGPUScratchBufferTextureManager::FreeResourceEntry (Ego_r `?FreeResourceEntry@CGPUScratchBufferTextureManager@@AAEXJ@Z`, private) |
| 0x00F38440 | "Vector_CopyMultipleElements" | `__uninitialized_copy<CGPUScratchBufferTextureManager::CResourcePage*>` |
| 0x00F384E0 | "Vector_CopyMultiple_Deep" | `__uninitialized_fill_n<CResourcePage*>` |
| 0x00F38770 | "Vector_Reallocate_NestedVector" | vector<CResourcePage>::_M_insert_overflow |
| 0x00F38860 | "Vector_Insert_Or_Reallocate" | vector<CResourcePage>::push_back, called from AllocteMemoryPage at 0x00F3770C |
| 0x00F38710 | (none) | vector<CResourcePage>::_M_clear |
| 0x00514160 | (none) | vector<long> copy ctor (ICF-shared) |
| 0x00F3ABF0 | (none) | CRangeCompressor::CalcCompressionScript (recursive, `ret 14h`) |
| 0x00F3B1D0 | `_Move<CTVertexLandscapeBackground*,...>` | `__uninitialized_copy<CRangeCompressor::CScriptEntry*>` |
| 0x00F3B330 | "Vector_Reallocate_EHeroMorphType" | vector<CRangeCompressor::CScriptEntry>::_M_insert_overflow |
| 0x00F3B220 (landed) | `std::__uninitialized_fill_n<pair<EHeroMorphType, CTextureMorphEntry>>` | `std::__uninitialized_fill_n<CRangeCompressor::CScriptEntry*>` |
| 0x00F3B420 (landed) | "CRetailVector_SRecord24::PushBack" | vector<CRangeCompressor::CScriptEntry>::push_back |
| 0x00F3B640 | "std_vector_assign_with_realloc" | `std::vector<unsigned short>::operator=` |
| 0x00F3B8A0 / 0x00F3B8F0 (landed) | "CElement12_00f3b640" | CArray<unsigned short> |
| 0x00F3B760 | "std_vector_copy_range" | `__uninitialized_copy<CArray<unsigned short>*>` |
| 0x00F3B7D0 | "Vector_CopyMultipleUshortElements" | `__uninitialized_fill_n<CArray<unsigned short>*>` |
| 0x0121AD20 / 0x0121AD50 | "ResetButtonCounts" | `$E` dynamic initialisers of CTCInventoryItem orientation CRightHandedSets 6 (0x0137CFD4) and 7 (0x0137CFEC) |
| 0x0121AC30 / 0x0121ACD0 | "$E2" | `$E` initialisers of orientation sets 1 (0x013B8BCC) and 5 (0x013B8BE4); the 12-entry table is behind the jump table at 0x005D8FFC; descriptive names |
| 0x00F3FB96 | `eh_vector_constructor_iterator` | correct; VC7.1 maps a `__ehvec_ctor` definition to `??_L` itself |
| 0x0042D9E0 | "CPSCBase" | CDefClassBase ctor (already in GOTCHAS) |
| 0x009FC550 (landed) | "~CMemoryBlock" | likely ~CResource |
