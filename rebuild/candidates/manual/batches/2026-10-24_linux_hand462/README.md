# Hand chunk 462 (std_list_remove<CIntelligentPointer<CThing>_NStealth .. vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>): 40 of 40 byte-exact

All 40 rows grade MATCH (6) or RELOCATION_MATCH (34) through gall.sh (grades.txt). Every behaviour test passes under both
g++ (tall.sh) and VC7.1 under Wine (tall.py). The call-target check (calls.txt) shows every relocation going to the retail
target, with no import mismatch. memmove in 0x004122D0 is a plain rel32 call to the 0x00BFEAE6 thunk, not dllimport.
Almost every catalog label in this chunk is propagated from an unrelated byte twin. The real identities come from callers,
landed neighbours and the Ego_r/FableWin PDB name sets (see Label corrections).

## What matched (40)

- **STLport `swap<T>` (14 rows: 006f15e0, 00cb6230, 009e6480, 00a3fdc0, 00af2cd0, 004b7800, 004cfbf0, 00566870,
  00cb9230, 008073d0, 0071fdf0, 004da6fc, 005938f1, 006232f0).**
  - Body: `_Tp __tmp = __a; __a = __b; __b = __tmp;` as a `__fastcall` template in `_STL`.
  - pair has STLport's user copy ctor and the implicit operator=. Callees are declared at their retail addresses: CCharString copy/assign/dtor 0x99EC30/0x99EFB0/0x99EAE0, CWideString 0x99B720/0x99B7D0/0x99B510, CIntelligentPointer base ctor/GetPItem/SetPItem/dtor 0xA01B10/0xA01B50/0xA01B90/0xA01C10.
  - Shape lessons:
    - In /O2 TUs the pair copy and assign inline into member calls. In /Os TUs (004da6fc, 005938f1) the pair copy ctor and operator= stay out of line (0x4D788D/0x4DA56A, 0x592DFB/0x59308A).
    - 005938f1 needs a `__forceinline ~CDefPointer`, so that the out-of-line `~pair` (0x5930F3) is called instead of an inlined def-pointer release.
    - C3DVector is pack(1) with a memberwise user copy ctor (006f15e0, 008073d0).
    - CTestQuest (006232f0) uses its out-of-line copy ctor 0x4A89D0 and operator= 0x4A8F50, then the implicit inline dtor (six ~CCharString).
    - CCompositeBlendDesc's copy ctor is out of line at 0x5BA48A (0071fdf0).
- **STLport `__pop_heap` (4 rows: 00497310, 009b3a40, 005e1b20, 005946b2).**
  - Body: `*__result = *__first; __adjust_heap(__first, 0, __last - __first, __val, __comp);`.
  - The by-value `__val` is copy-built in the argument area. `__adjust_heap` is declared at 0x497160 / 0x9B2EA0 / 0x5E1170 / 0x593E3E. The comparators are empty and passed by value.
- **STLport `__unguarded_insertion_sort_aux` (4 rows: 006a32b0, 005e1ac0, 006beb80, 009323d0).**
  - Body: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);`.
  - The comparator stays declaration-only. The linear insert is out of line at 0x6A2DC0 / 0x5E10B0 / 0x6BE930. In 009323d0, where the comparator is a `__fastcall` function pointer, the linear insert is defined inline.
- **STLport `__destroy_aux(first, last, __false_type)` (3 rows: 004da655, 00cd2725, 0043f850).**
  - Each element's virtual dtor is called through slot 0 with flag 0. 004da655 and 00cd2725 are /Os (loop entered at the bottom test).
  - Local polymorphic models: engine/CMultiStaticMeshEntryDef.h models the vptr as a data member.
- **`vector::erase` (2 rows).**
  - 008e3890 erase(pos): `if (pos+1 != end()) __copy_ptrs(pos+1, finish, pos, __false_type()); --finish; _Destroy(finish);`. The tag is homed in the dead argument slot, `__copy` is declared at 0x8E34C0, and the element dtor is two inline ~CBaseIntelligentPointer calls.
  - 00a00e60 erase(first, last): `__copy` (rep movsd) and `__destroy_aux` inline, with `~CGraphicInfo() { Bank = 0; Frames = 0; }`.
- **`vector::resize(n, const T& x)` (4 rows: 0067f220, 00a55670, 00a97a50, 004122d0).**
  - Body: `if (n < size()) erase(begin()+n, end()); else insert(end(), n-size(), x);`, with erase = `__copy_ptrs` + `_Destroy` inline and `_M_fill_insert` declared.
  - 004122d0 (1-byte element) uses the `__true_type` `__copy_trivial`. Both of its operands are `_M_finish`, and VC7.1 keeps the unfolded `cmp eax,eax ; jne` and `mov esi,eax ; sub esi,eax` with no special modelling.
- **`vector::resize(n)` (7 rows: 0049b7f0, 006253d0, 007b9ec0, 009c7850, 009cb7d0, 00ae7d10, 00bf97a0).**
  - Body: plain STLport `resize(n) { resize(n, _Tp()); }` over the inline two-argument resize, with out-of-line erase(first, last) and `_M_fill_insert`. The default element sits in the 8-byte frame and is destroyed on both exits. No `__forceinline` was needed.
  - The element's real default constructor sets the shape:
    - implicit ctor of `{long, CCharString}` / `{long, CWideString}`: only the string is built (CTextTag, CPaletteEntry, CQuestCardItem);
    - `Mem(0), Used(false)`: CBufferChunk;
    - STLport `pair() : first(_T1()), second(_T2())`: the zero store is scheduled after the compare;
    - CTexture `PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; }`: two zero words;
    - CIntelligentPointer: base ctor plus a late derived vtable store.
  - Explicit instantiation also emits the two-argument resize COMDAT, so each `// Leaf:` is the full `?resize@...QAEXI@Z` symbol.
- **NInventory::CCategory::SetCategoryThingAssociation (005bd183, /Os).**
  - `CIntelligentPointer<CThing> associated(thing); PAssociatedThing = associated;`.
  - A named local is needed: retail re-forms the address with `lea`. The unnamed temporary passed the ctor's returned eax and graded 40v43.
- **STLport `__upper_bound` (0070fd30).**
  - Copy of the landed lower_bound model 0x0070FCF0, over list<CAnimSet> iterators with CAnimSetItPriorityLessThan.

## Parked for Fable 5.1 (0)

## Skipped

None. No row holds two functions.

## Label corrections

All the catalog labels below are propagated. The real names come from callers, landed neighbours and the PDB name sets.

| Address | Catalog label | Real name |
|---|---|---|
| 008e3890 | std_list_remove<CIntelligentPointer<CThing>,NStealth::CDangerStatus> | `_STL::vector<CIcicleData>::erase(CIcicleData*)` |
| 00cb6230 | swap<CMusicManager::CRegisteredMusicEntry> | `swap<pair<CCharString,CScriptInfo>>` |
| 009e6480 | swap<CSymbolMap::CSymbolInfo> | `swap<pair<CCharString,CSymbolMap::CSymbolInfo>>` |
| 00af2cd0 | swap<CSymbolMap::CSymbolInfo> | `swap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>>` |
| 00497310 | swap<CTCBase::CRegisteredTCInfo> | `__pop_heap<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare, int>` |
| 009b3a40 | swap<CTCBase::CRegisteredTCInfo> | `__pop_heap<CWideString*, CWideString, less<CWideString>, int>` |
| 005e1b20 | swap<CTCBase::CRegisteredTCInfo> | `__pop_heap<CCharString*, CCharString, less<CCharString>, int>` |
| 005946b2 | swap<CTCBase::CRegisteredTCInfo> | `__pop_heap<pair<long,CDefPointer<const CContextSensitiveItemDef>>*, ..., CContextSensitivePriorityCompare, int>` |
| 004da6fc | swap<CTCBase::CRegisteredTCInfo> | `swap<pair<CCharString,CTCBase::CRegisteredTCInfo>>` (the pair, not the bare info) |
| 005938f1 | swap<CTCBase::CRegisteredTCInfo> | `swap<pair<long,CDefPointer<const CContextSensitiveItemDef>>>` |
| 005bd183 | swap<CTCBase::CRegisteredTCInfo> | `NInventory::CCategory::SetCategoryThingAssociation(CThing*)` (Ego_r.pdb) |
| 006232f0 | swap<CTCBase::CRegisteredTCInfo> | `swap<CTestQuest>` |
| 0071fdf0 | swap<CTCHeroMorph::CTextureLayer> | `swap<pair<EHeroMorphType,CTextureMorphEntry>>` |
| 004cfbf0 | swap<NParticleEngine::CRegisteredPSCInfo> | `swap<pair<float,CCharString>>` |
| 00566870 | swap<NParticleEngine::CRegisteredPSCInfo> | `swap<pair<float,CWideString>>` |
| 00cb9230 | swap<NParticleEngine::CRegisteredPSCInfo> | `swap<pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>>` |
| 004b7800 | swap<NQuestManager::CRuntimeQuestPersistence::CPersistInfo> | `swap<pair<CCharString, ...CPersistInfo>>` |
| 0070fd30 | Dinkumware upper_bound<_List_const_iterator...> | STLport `__upper_bound(first, last, val, comp, int*)` |
| 0067f220 | vector<CBrainBehaviour>::resize | `vector<CDegradableInfo>::resize(n, x)` |
| 00a55670 | vector<CBrainBehaviour>::resize | `vector<C3DAnimationSequenceData::C3DVectorWord>::resize(n, x)` |
| 00a97a50 | vector<CBrainBehaviour>::resize | `vector<C3DTriangle2>::resize(n, x)` |
| 004122d0 | vector<CEditRegion>::resize | `vector<char>::resize(n, x)` |
| 005e1ac0 | vector<CGraphicInfo>::_Destroy | `__unguarded_insertion_sort_aux<pair<unsigned long,CAnimationEntry>*>` |
| 006beb80 | vector<CGraphicInfo>::_Destroy | `__unguarded_insertion_sort_aux<pair<long,CMusicManager::CRegisteredMusicEntry>*>` |
| 006a32b0 | vector<CIntelligentPointer<CParticleEmitter>>::_Destroy | `__unguarded_insertion_sort_aux<pair<long,CIntelligentPointer<CThing>>*>` |
| 009323d0 | vector<CIntelligentPointer<CParticleEmitter>>::_Destroy | `__unguarded_insertion_sort_aux<CThing**, CThing*, bool(__fastcall*)(CThing*,CThing*)>` |
| 004da655 | vector<CIntelligentPointer<CThing const>>::_Destroy | `__destroy_aux<CMultiStaticMeshEntryDef*>` |
| 00cd2725 | vector<CIntelligentPointer<CThing const>>::_Destroy | `__destroy_aux` over a 16-byte polymorphic record (descriptive name CScriptRecord16) |
| 0043f850 | vector<CIntelligentPointer<CParticleEmitter>>::_Destroy | `__destroy_aux<CIntelligentPointer<NParticleEngine::CParticleEmitter>*>` |
| 0049b7f0 | vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize | `vector<CTexture>::resize(n)` |
| 006253d0 | (same) | `vector<CQuestCardItem>::resize(n)` |
| 007b9ec0 | (same) | `vector<CIntelligentPointer<CThingObject>>::resize(n)` |
| 009c7850 | (same) | `vector<CFileInstaller::CBufferChunk>::resize(n)` |
| 009cb7d0 | (same) | `vector<NGameText::CTextTag>::resize(n)` |
| 00ae7d10 | (same) | `vector<pair<CCharString,unsigned long>>::resize(n)` |
| 00bf97a0 | (same) | `vector<CEngineTexturePalette::CPaletteEntry>::resize(n)` |

These rows' catalog labels are right: 006f15e0 swap<CDefendingCombatantInfo::CProng>, 00a3fdc0 swap<CSymbolMap::CSymbolInfo>,
008073d0 swap<NPlayerTargeting::CTargetPoint> and 00a00e60 vector<CGraphicInfo>::erase.

Callee labels found wrong on the way:

| Address | Catalog label | Real name |
|---|---|---|
| 0x0049B5A0 | Std_Vector_Insert_Range_CTexture | vector<CTexture>::_M_fill_insert |
| 0x007B9870 | StdVector_Clear | vector<CIntelligentPointer<CThingObject>>::erase(first, last) |
| 0x00BF9420 | CParticleMorphs::CopyAndConstruct | vector<CPaletteEntry>::erase(first, last) |
| 0x00BF95D0 | Std_Vector_ResizeWithFillAndCopy | vector<CPaletteEntry>::_M_fill_insert |
| 0x00A552B0 | Vector_InsertN_Triangle2 | vector<C3DVectorWord>::_M_fill_insert |
| 0x00A96F00 | Vector_Assign_C3DTriangle2_Vertices | vector<C3DTriangle2>::_M_fill_insert |
| 0x00A557C0 | — | the resize(n) wrapper of vector<C3DVectorWord> |
| 0x00CB63F0 | — | iter_swap, a bare `jmp` to swap 0x00CB6230 |
