# agent955 family pass 6 -- notes

Per family: matched n of m, the model (generator docstring), each instance's identity (proven / probable /
descriptive, evidence in gen/<FAM>_ids.tsv), label corrections and review items, callee names introduced (symbols the
graded objects reference that rebuild/oracles/reloc_bindings.tsv does not carry yet) and header debt (`// Layout:`).

Summary: 150 families, 147 complete, 3 with parked rows; identities 98 descriptive, 101 probable, 401 proven.


## L001 -- 4 of 4 matched (4 MATCH)
Model: L001: STLport 4.6 std::vector<T>::erase(iterator first, iterator last) over a 16-byte class with an implicit (memberwise) operator= and a trivial destructor: the inline random-access __copy loop moves the tail down four dwords at a time, the _Destroy loop of trivial destructors vanishes, finish = the copy's end, return first (81 bytes, thiscall, `ret 8`).
Identities:
- 00515bc0 std::vector<C2DBoxI>::erase(iterator, iterator) (proven) -- MATCH
- 009c2680 std::vector<CDisplayModeDesc>::erase(iterator, iterator) (proven) -- MATCH
- 00a748e0 std::vector<C2DLineF>::erase(iterator, iterator) (proven) -- MATCH
- 00bcbf20 std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::erase(iterator, iterator) (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L002 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L002: 20-byte /Gr STLport forwarders that pass (first, last, result) on to the out-of-line __copy / __copy_backward with a random_access_iterator_tag temporary (homed on the result argument's slot) and a null Distance*: __copy_backward_ptrs<T> (ret 8) and copy<const T*, T*> (ret 4).  All four have no caller in Fable.exe (unreferenced COMDATs); each identity comes from its own callee.
Identities:
- 00518f60 std::__copy_backward_ptrs<std::set<unsigned long>> (probable) -- RELOCATION_MATCH
- 005190b0 std::copy<const CMapInfo*, CMapInfo*> (probable) -- RELOCATION_MATCH
- 0086d960 std::__copy_backward_ptrs<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, ...>> (probable) -- RELOCATION_MATCH
- 0086e900 std::__copy_backward_ptrs<CArray<CRandomAppearanceMorph::CBodyPartMesh>> (probable) -- RELOCATION_MATCH
Label corrections / review:
- L002/L007: all eight rows have no caller in Fable.exe (unreferenced COMDATs); identities from their own callees.
Callee names introduced: `??$__copy_backward@PAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@PAV1@H@std@@YIPAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@H@std@@YIPAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@PAV12@H@std@@YIPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@0@PAV10@00ABUrandom_access_iterator_tag@0@PAH@Z`
Header debt: none

## L003 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L003: STLport 4.6 vector<T>::_M_insert_overflow(pos, x, __false_type, fill_len, atend) for class elements with an out-of-line copy constructor (286 bytes, `ret 14h`): the landed 0x0051C470 (vector<set<unsigned long> >) model verbatim with each instance's element type, copy constructor, tail __uninitialized_copy and __destroy_aux.
Identities:
- 00519a50 std::vector<CMapInfo>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 006b7130 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStack>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 0086eb70 std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh>>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 0088a700 std::vector<NEnvironmentInternal::CEnvironmentThemeSetDay>::_M_insert_overflow (probable) -- RELOCATION_MATCH
Callee names introduced: `??0CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAE@ABV01@@Z`, `??0CEnvironmentThemeSetDay@NEnvironmentInternal@@QAE@ABV01@@Z`
Header debt: none

## L004 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L004: STLport 4.6 vector<T>::_M_fill_insert(pos, n, x) for class elements with out-of-line copy constructor, assignment and destructor (300/302 bytes, `ret 0Ch`): __x_copy in the frame, out-of-line __uninitialized_copy / uninitialized_fill_n / __copy_backward / fill, __copy_backward_ptrs inline, the overflow arm calls _M_insert_overflow (family L003).
Identities:
- 0051a6d0 std::vector<CMapInfo>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 0051c920 std::vector<std::set<unsigned long>>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 0086eca0 std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh>>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 00a4b020 std::vector<CLipSyncFrameData>::_M_fill_insert (proven) -- RELOCATION_MATCH
Label corrections / review:
- L004: _M_fill_insert keeps the protected (IAE) spelling the existing bindings use (10 IAE vs 109 QAE across reloc_bindings); STLport access unverified.
Callee names introduced: `??$__copy_backward@PAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@PAV1@H@std@@YIPAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@PAV12@H@std@@YIPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@0@PAV10@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??1?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@QAE@XZ`, `??1?$vector@VCLipSyncPhoneme@@V?$allocator@VCLipSyncPhoneme@@@std@@@std@@QAE@XZ`, `??1CMapInfo@@QAE@XZ`, `??4?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@QAEAAV0@ABV0@@Z`, `??4?$vector@VCLipSyncPhoneme@@V?$allocator@VCLipSyncPhoneme@@@std@@@std@@QAEAAV01@ABV01@@Z`, `?_M_insert_overflow@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@V?$allocator@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@std@@@std@@IAEXPAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@ABV3@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@V?$allocator@V?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@2@@std@@IAEXPAV?$set@KU?$less@K@std@@V?$allocator@K@2@@2@ABV32@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCLipSyncFrameData@@V?$allocator@VCLipSyncFrameData@@@std@@@std@@IAEXPAVCLipSyncFrameData@@ABV3@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCMapInfo@@V?$allocator@VCMapInfo@@@std@@@std@@IAEXPAVCMapInfo@@ABV3@ABU__false_type@2@I_N@Z`
Header debt: none

## L005 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L005: STLport __uninitialized_copy over 16-byte {unsigned __int64, list<C2DLineF>} records (109 bytes, /Gr, `ret 8`): copy the 64-bit key as two dwords, then the inline list copy constructor (self-linked 0x18-byte header, out-of-line list<C2DLineF>::_M_insert_dispatch 0x005152C0).  Two element types (one per TU) x two const-nesses.  Model: the landed 0x00A82730 (__uninitialized_copy over list<C2DLineF>).
Identities:
- 0051a810 std::__uninitialized_copy<CNavigationAreaUpdate::CSwitchableBlockage*, CNavigationAreaUpdate::CSwitchableBlockage*> (proven) -- RELOCATION_MATCH
- 0051ac00 std::__uninitialized_copy<const CNavigationAreaUpdate::CSwitchableBlockage*, CNavigationAreaUpdate::CSwitchableBlockage*> (proven) -- RELOCATION_MATCH
- 00a83030 std::__uninitialized_copy<std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*> (proven) -- RELOCATION_MATCH
- 00a83340 std::__uninitialized_copy<const std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L006 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L006: STLport 4.6 __introsort_loop<T**, T*, int, Compare> over 4-byte pointer elements with a 4-byte by-value comparator (136 bytes, /Gr, `ret 0Ch`): out-of-line __median, __unguarded_partition, __partial_sort and the recursion.
Identities:
- 0051baf0 std::__introsort_loop<CThingBuilding**, CThingBuilding*, int, CIsHouseLargerThan> (proven) -- RELOCATION_MATCH
- 007496b0 std::__introsort_loop<const CThing**, const CThing*, int, CCompare_FurthestFromInitPos2D> (proven) -- RELOCATION_MATCH
- 0087e530 std::__introsort_loop<const CAnimationEntry**, const CAnimationEntry*, int, CTurningAnimCompare> (proven) -- RELOCATION_MATCH
- 0096e410 std::__introsort_loop<CThing**, CThing*, int, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>> (probable) -- RELOCATION_MATCH
Callee names introduced: `??$__median@PAVCThing@@V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@std@@YIABQAVCThing@@ABQAV1@00V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@Z`, `??$__median@PAVCThingBuilding@@VCIsHouseLargerThan@@@std@@YIABQAVCThingBuilding@@ABQAV1@00VCIsHouseLargerThan@@@Z`, `??$__median@PBVCAnimationEntry@@VCTurningAnimCompare@?A0x02c98578@@@std@@YIABQBVCAnimationEntry@@ABQBV1@00VCTurningAnimCompare@?A0x02c98578@@@Z`, `??$__median@PBVCThing@@VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@std@@YIABQBVCThing@@ABQBV1@00VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@Z`, `??$__partial_sort@PAPAVCThing@@PAV1@V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@std@@YIXPAPAVCThing@@000V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@Z`, `??$__partial_sort@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@@std@@YIXPAPAVCThingBuilding@@000VCIsHouseLargerThan@@@Z`, `??$__partial_sort@PAPBVCAnimationEntry@@PBV1@VCTurningAnimCompare@?A0x02c98578@@@std@@YIXPAPBVCAnimationEntry@@000VCTurningAnimCompare@?A0x02c98578@@@Z`, `??$__partial_sort@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@std@@YIXPAPBVCThing@@000VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@Z`, `??$__unguarded_partition@PAPAVCThing@@PAV1@V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@std@@YIPAPAVCThing@@PAPAV1@0PAV1@V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@Z`, `??$__unguarded_partition@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@@std@@YIPAPAVCThingBuilding@@PAPAV1@0PAV1@VCIsHouseLargerThan@@@Z`, `??$__unguarded_partition@PAPBVCAnimationEntry@@PBV1@VCTurningAnimCompare@?A0x02c98578@@@std@@YIPAPBVCAnimationEntry@@PAPBV1@0PBV1@VCTurningAnimCompare@?A0x02c98578@@@Z`, `??$__unguarded_partition@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@std@@YIPAPBVCThing@@PAPBV1@0PBV1@VCCompare_FurthestFromInitPos2D@?A0x87f83cb4@@@Z`
Header debt: none

## L007 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L007: STLport push_back(const T& x) over two-member records whose members have out-of-line copy constructors (78 bytes, thiscall, `ret 4`): placement copy at finish (null test, two member copies), else _M_insert_overflow(finish, x, __false_type, 1, true) with the tag homed on x's slot.  CArray<T>::push_back carries the body (Ego_r.pdb defines it out of line); the fourth row is the base vector's own push_back (one symbol per address).
Identities:
- 0051c800 CArray<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc>::push_back (probable) -- RELOCATION_MATCH
- 00ad8720 CArray<QuadricOptimiserInternals::COptimisedSubMesh>::push_back (probable) -- RELOCATION_MATCH
- 00ad9720 CArray<CVertexGroupBlock>::push_back (probable) -- RELOCATION_MATCH
- 00adcfd0 std::vector<CVertexGroupBlock>::push_back (probable) -- RELOCATION_MATCH
Label corrections / review:
- L002/L007: all eight rows have no caller in Fable.exe (unreferenced COMDATs); identities from their own callees.
- L007: 0x00AD9720 / 0x00ADCFD0 are byte-identical CVertexGroupBlock push_backs: one CArray<CVertexGroupBlock>::push_back, one std::vector<CVertexGroupBlock>::push_back -- which is which is by position only.
Callee names introduced: `??0?$map@KJU?$less@K@std@@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QAE@ABV01@@Z`, `??0?$vector@V?$CCountedPointer@VCOptimisedPrimitive@QuadricOptimiserInternals@@@@V?$allocator@V?$CCountedPointer@VCOptimisedPrimitive@QuadricOptimiserInternals@@@@@std@@@std@@QAE@ABV01@@Z`, `??0?$vector@VCOptimisedBone@QuadricOptimiserInternals@@V?$allocator@VCOptimisedBone@QuadricOptimiserInternals@@@std@@@std@@QAE@ABV01@@Z`, `?_M_insert_overflow@?$vector@UCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@V?$allocator@UCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@@std@@@std@@QAEXPAUCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@ABU34@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCOptimisedSubMesh@QuadricOptimiserInternals@@V?$allocator@VCOptimisedSubMesh@QuadricOptimiserInternals@@@std@@@std@@QAEXPAVCOptimisedSubMesh@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QAEXPAVCVertexGroupBlock@@ABV3@ABU__false_type@2@I_N@Z`
Header debt: none

## L008 -- 4 of 4 matched (4 MATCH)
Model: L008: STLport 4.6 _Rb_tree<unsigned __int64, pair<const unsigned __int64, V>, ...>::_M_find(const key&) const (84 bytes, thiscall, `ret 4`): the lower-bound walk with a 64-bit compare, then the not-less check; returns the node or the header. Model: the landed _M_find 0x00756350 with an unsigned __int64 key.
Identities:
- 00527d20 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>, ...>::_M_find (proven) -- MATCH
- 00527d80 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>::_M_find (proven) -- MATCH
- 00527e00 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, std::pair<CCharString, CWideString>>, ...>::_M_find (proven) -- MATCH
- 00812790 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::_M_find (proven) -- MATCH
Label corrections / review:
- L008: 0x00527E00 keeps its binding's `_Rb_tree_node_base*` return (?_M_find@...QBEPAU_Rb_tree_node_base@2@AB_K@Z, from landed 0x0051F040); the other three L008 rows (and 38 landed _M_find) return `_Rb_tree_node<_Value>*` -- one STLport text cannot have both; unify at landing.
Callee names introduced: none
Header debt: none

## L009 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L009: STLport 4.6 _Rb_tree<unsigned __int64, V, KoV, less<unsigned __int64> >::insert_unique(const value_type&) (198 bytes, thiscall, `ret 8`, pair<iterator, bool> through the hidden pointer): the landed 0x0052A130 model verbatim with each instance's value type, out-of-line iterator operator-- and _M_insert.
Identities:
- 005294b0 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, ...>::insert_unique (proven) -- RELOCATION_MATCH
- 006f1ef0 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThingCreatureBase*>, ...>::insert_unique (proven) -- RELOCATION_MATCH
- 008136a0 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CGroupMemberData>, ...>::insert_unique (proven) -- RELOCATION_MATCH
- 0082f4b0 std::_Rb_tree<unsigned __int64, unsigned __int64, ...>::insert_unique (proven) -- RELOCATION_MATCH
Callee names introduced: `?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KPAVCThingCreatureBase@@@std@@U?$_Select1st@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KPAVCThingCreatureBase@@@2@0@Z`, `?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Select1st@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@2@0@Z`, `?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Select1st@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KVCGroupMemberData@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KVCGroupMemberData@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KVCGroupMemberData@@@2@0@Z`, `?_M_insert@?$_Rb_tree@_K_KU?$_Identity@_K@std@@U?$less@_K@2@V?$allocator@_K@2@@std@@AAE?AU?$_Rb_tree_iterator@_KU?$_Nonconst_traits@_K@std@@@2@PAU_Rb_tree_node_base@2@0AB_K0@Z`
Header debt: none

## L010 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L010: STLport 4.6 _Rb_tree<K, pair<const K, CCountedPointer<T> >, ...>::_M_insert(x, y, v, w) with a signed 4-byte key and a counted-pointer value (211 bytes, `ret 14h`, hidden iterator): the landed 0x00A2C4E0 model with each instance's key/value.
Identities:
- 00553a00 std::_Rb_tree<NUISystem::ETableSprites, pair<const NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 00568500 std::_Rb_tree<long, pair<const long, CCountedPointer<CTexture> >, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 006f5320 std::_Rb_tree<EActionRegisteredType, pair<const EActionRegisteredType, CCountedPointer<CCreatureActionBase> >, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 00b96f00 std::_Rb_tree<long, pair<const long, CCountedPointer<CEngineSceneMap> >, ...>::_M_insert (proven) -- RELOCATION_MATCH
Label corrections / review:
- L010: _M_insert is private (AAE) per the owner's STLport 4.6 note; reloc_bindings binds 0x00553A00 / 0x00568500 under public QAE spellings (landed callers 0x00553AE0/0x00553C50, 0x005685E0/0x00568AD0) -- rebind/retype at landing.
Callee names introduced: none
Header debt: none

## L011 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L011: STLport __uninitialized_copy over 8/16-byte {4-byte scalar, member with an out-of-line copy constructor} records (54 bytes, /Gr, `ret 8`): the record's implicit copy constructor (dword move + member copy) under placement new's null test.
Identities:
- 005654b0 std::__uninitialized_copy<const std::pair<float, CWideString>*, std::pair<float, CWideString>*> (proven) -- RELOCATION_MATCH
- 00623660 std::__uninitialized_copy<CQuestCardItem*, CQuestCardItem*> (proven) -- RELOCATION_MATCH
- 009ca190 std::__uninitialized_copy<NGameText::CTextTag*, NGameText::CTextTag*> (probable) -- RELOCATION_MATCH
- 00ae7530 std::__uninitialized_copy<const C3DGroup2*, C3DGroup2*> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L012 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L012: CDefPointer<const T>::`scalar deleting destructor' in size-optimised TUs (42 bytes, unaligned addresses): the inline ~CDefPointer (release the counted def: `dec [ecx+4] ; jne ; call [vtbl+4]`, clear the slot with `and [esi],0`), then operator delete when bit 0 of the flags is set.  Emitted by the TU's /Os `_Destroy(p)` (explicit p->~T()) / delete.
Identities:
- 0057f9bf CDefPointer<const CUnprovenDef_0057f9bf>::`scalar deleting destructor' (descriptive) -- RELOCATION_MATCH
- 005b1b5e CDefPointer<const CThingBaseDef>::`scalar deleting destructor' (probable) -- RELOCATION_MATCH
- 005baa41 CDefPointer<const CUnprovenDef_005baa41>::`scalar deleting destructor' (descriptive) -- RELOCATION_MATCH
- 005f861e CDefPointer<const CHeroAbilityDef>::`scalar deleting destructor' (probable) -- RELOCATION_MATCH
Label corrections / review:
- L012/L015: CDefPointer pointee types unproven for 0x0057F9BF, 0x005BAA41 and all four L015 rows (CUnprovenDef_<addr>).
Callee names introduced: none
Header debt: none

## L013 -- 0 of 4 matched (none)
Model: L013: CTCInventory / CTCInventoryClothing ::CreateMenuDesc / CreateMenuDescPC (private, Ego_r.pdb AAE): build the description component from its UI definition, set its text from the game text bank by symbol, return it as a CCountedPointer<NUISystem::CComponent> (135 bytes, /Os frame TU, `ret 8`).  Parked: one register choice (retail loads the component pointer straight into esi, the lane into eax then `mov esi,eax`).
Identities:
- 005820e7 CTCInventory::CreateMenuDesc (proven) -- DIFFER 137v135
- 00582673 CTCInventory::CreateMenuDescPC (proven) -- DIFFER 137v135
- 005b358c CTCInventoryClothing::CreateMenuDesc (proven) -- DIFFER 137v135
- 005b3613 CTCInventoryClothing::CreateMenuDescPC (proven) -- DIFFER 137v135
Parked 005820e7: Grade: DIFFER(137v135) -- 137v135 the component pointer is loaded into eax then copied to esi (retail loads it into esi directly); 11 grades: GetInstance / bank / component locals, implicit vs explicit CCharString, operator* / .Data / reference forms, QFE 4035
Parked 00582673: Grade: DIFFER(137v135) -- 137v135 the component pointer is loaded into eax then copied to esi (retail loads it into esi directly); 11 grades: GetInstance / bank / component locals, implicit vs explicit CCharString, operator* / .Data / reference forms, QFE 4035
Parked 005b358c: Grade: DIFFER(137v135) -- 137v135 the component pointer is loaded into eax then copied to esi (retail loads it into esi directly); 11 grades: GetInstance / bank / component locals, implicit vs explicit CCharString, operator* / .Data / reference forms, QFE 4035
Parked 005b3613: Grade: DIFFER(137v135) -- 137v135 the component pointer is loaded into eax then copied to esi (retail loads it into esi directly); 11 grades: GetInstance / bank / component locals, implicit vs explicit CCharString, operator* / .Data / reference forms, QFE 4035
Label corrections / review:
- L013: Ego_r.pdb / FableWin.pdb make CreateMenuDesc / CreateMenuDescPC private (AAE); landed caller 0x005907AB binds 0x00582673 as public QAE.
Callee names introduced: none
Header debt: none

## L014 -- 4 of 4 matched (4 MATCH)
Model: L014: STLport __copy<pair<A, B>*, pair<A, B>*, int>(first, last, result, random_access_iterator_tag, int*) over 8-byte pairs with the implicit memberwise assignment (37 bytes, /Gr + /Os TU, `ret 0Ch`): counted loop of two dword moves.
Identities:
- 0059269b std::__copy<std::pair<long, long>*, std::pair<long, long>*, int> (proven) -- MATCH
- 005ba371 std::__copy<std::pair<EClothingSuitPart, NUISystem::CComponent*>*, std::pair<EClothingSuitPart, NUISystem::CComponent*>*, int> (proven) -- MATCH
- 005f7b9b std::__copy<std::pair<long, EHeroAbility>*, std::pair<long, EHeroAbility>*, int> (proven) -- MATCH
- 005f7c3a std::__copy<const std::pair<long, EHeroAbility>*, std::pair<long, EHeroAbility>*, int> (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L015 -- 4 of 4 matched (4 MATCH)
Model: L015: CDefPointer<const T>::operator=(const CDefPointer&) in size-optimised TUs (49 bytes, thiscall, `ret 4`): when the pointees differ, release the old def (Destroy at zero, `and [esi],0`), take the new one and add a reference.  All four have no caller in Fable.exe (unreferenced COMDATs), so the def type is unproven (descriptive).
Identities:
- 00592844 CDefPointer<const CUnprovenDef_00592844>::operator= (descriptive) -- MATCH
- 005c07ae CDefPointer<const CUnprovenDef_005c07ae>::operator= (descriptive) -- MATCH
- 005f7e99 CDefPointer<const CUnprovenDef_005f7e99>::operator= (descriptive) -- MATCH
- 006551ff CDefPointer<const CUnprovenDef_006551ff>::operator= (descriptive) -- MATCH
Label corrections / review:
- L012/L015: CDefPointer pointee types unproven for 0x0057F9BF, 0x005BAA41 and all four L015 rows (CUnprovenDef_<addr>).
Callee names introduced: none
Header debt: none

## L016 -- 4 of 4 matched (4 MATCH)
Model: L016: STLport 4.6 _Rb_tree<K, pair<const K, V>, ...>::_M_lower_bound(const K&) const with a signed 4-byte key in /Os TUs (37 bytes, thiscall, `ret 4`): the lower-bound walk, returns the node (or the header).  Bindings carry the address under a `map::_M_lower_bound` owner (map has no such member); this family names the tree's member.
Identities:
- 0059af83 std::_Rb_tree<FRONTEND_SCREEN, pair<const FRONTEND_SCREEN, NUISystem::CComponent*>, ...>::_M_lower_bound (proven) -- MATCH
- 005f8992 std::_Rb_tree<CTCInventoryAbilities::EUISpellCategory, pair<const CTCInventoryAbilities::EUISpellCategory, NUISystem::CComponent*>, ...>::_M_lower_bound (proven) -- MATCH
- 005f89b7 std::_Rb_tree<EHeroAbility, pair<const EHeroAbility, NUISystem::CComponent*>, ...>::_M_lower_bound (proven) -- MATCH
- 00cd28f4 std::_Rb_tree<long, pair<const long, CCharString>, ...>::_M_lower_bound (proven) -- MATCH
Label corrections / review:
- L016: reloc_bindings names the four _M_lower_bound addresses with owner `map` (?_M_lower_bound@?$map@...); _M_lower_bound is the _Rb_tree's member -- this family names the tree's.
Callee names introduced: none
Header debt: none

## L017 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L017: STLport 4.6 vector<T>::reserve(n) for 8-byte class elements (164 bytes, thiscall, `ret 4`): grow through _M_allocate_and_copy (malloc + out-of-line __uninitialized_copy) and _M_clear (out-of-line __destroy_aux + free), or a bare allocate for an empty vector.  The empty tags share the live n slot only with same-TU noinline helper bodies.
Identities:
- 005e24d0 std::vector<std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >>::reserve (proven) -- RELOCATION_MATCH
- 009cb1f0 std::vector<NGameText::CDataBankTextEntry>::reserve (proven) -- RELOCATION_MATCH
- 00a975e0 std::vector<CCountedPointer<C3DClothPrimitive>>::reserve (proven) -- RELOCATION_MATCH
- 00af0ef0 std::vector<CCountedPointer<CGuiVarTransferBase>>::reserve (proven) -- RELOCATION_MATCH
Label corrections / review:
- L017: landed NGameText::CDataBank::NotifyBankSize 0x009C82C0 binds 0x009CB1F0 as the stand-in ?reserve@?$CArray@VCDataBankTextEntry@NGameText@@@@QAEXK@Z; both PDBs have only vector<CDataBankTextEntry>::reserve(unsigned int).
- L017: 0x009CA4C0 is __uninitialized_copy<CDataBankTextEntry*> (called by vector<CDataBankTextEntry>::reserve 0x009CB1F0 and _M_insert_overflow 0x009CB4E0 in the CDataBank module). Landed 0x00A43B20 binds the same symbol to 0x00A43630 and calls its element CDataBankTextEntry; the catalog labels it Vector_Reallocate_CDataBankGroupEntry -- one of the two element identities at 0x00A43B20 is wrong (an 8-byte element in another module); re-derive it.
Callee names introduced: `??0?$CCountedPointer@VC3DClothPrimitive@@@@QAE@ABV0@@Z`, `??0?$CCountedPointer@VCGuiVarTransferBase@@@@QAE@ABV0@@Z`, `??0?$CIVCountedPointer@VCAnimComponentBase@@@@QAE@ABV0@@Z`, `??0?$CIVCountedPointer@VCTextEntry@NGameText@@@@QAE@ABV0@@Z`, `??1?$CCountedPointer@VC3DClothPrimitive@@@@QAE@XZ`, `??1?$CCountedPointer@VCGuiVarTransferBase@@@@QAE@XZ`, `??1?$CIVCountedPointer@VCAnimComponentBase@@@@QAE@XZ`, `??1?$CIVCountedPointer@VCTextEntry@NGameText@@@@QAE@XZ`
Header debt: none

## L018 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L018: thing-component def-field getters (52 bytes, thiscall, plain `ret`): fetch the parent thing's sub-def of type T through CDefClassBase::GetSubDef<T>, read one 4-byte field, release the def by hand (no null test) and return the field.
Identities:
- 005ea3c0 CTCTavernGame::GetBanter (proven) -- RELOCATION_MATCH
- 00705cc0 CTCCarryable::GetCarrySlotActive (proven) -- RELOCATION_MATCH
- 00705d00 CTCCarryable::GetCarrySlotPassive (proven) -- RELOCATION_MATCH
- 007f4f80 CTCStockItem::GetBuyersPhrase (proven) -- RELOCATION_MATCH
Label corrections / review:
- L018: landed 0x007F4F40 (CTCStockItem::GetSellersBanter) defines its function on a derived stand-in `CTCStockItem_Methods` (symbol ?GetSellersBanter@CTCStockItem_Methods@@, not the PDB ?GetSellersBanter@CTCStockItem@@QAEKXZ) and calls an invented FindDef_CDefClassBase_GetSubDef_1 from its GetSubDef stand-in body.
- L018: GetBanter: Ego_r.pdb is const (QBEJXZ); landed caller 0x0094CE60 binds QAE.
Callee names introduced: none
Header debt: 005ea3c0: engine/CTCBase.h (ParentThing +0x04); retail CThing holds its definition pointer at +0x70 (as the landed (same model for 00705cc0, 00705d00, 007f4f80)

## L019 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L019: NCameraModes::CScriptModule* `scalar deleting destructor' (vtable slot 0; 55 bytes, thiscall, `ret 4`): the inline ~CScriptModuleBase (base vptr store, POwner = PCameraManager = 0, the two intelligent-pointer members destroyed through CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10), then operator delete when bit 0 of the flags is set.  The derived classes' implicit destructors add nothing (no derived vptr store).
Identities:
- 005ed150 NCameraModes::CScriptModuleBase::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
- 005ed3e0 NCameraModes::CScriptModuleGoToLookInDirection::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
- 00854410 NCameraModes::CScriptModuleSelfTerminateBase::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
- 00854480 NCameraModes::CScriptModuleFOVZoom::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
Callee names introduced: `??_G?$CIntelligentPointer@$$CBVCThing@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`, `??_GCScriptModuleBase@NCameraModes@@UAEPAXI@Z`, `??_GCScriptModuleFOVZoom@NCameraModes@@UAEPAXI@Z`, `??_GCScriptModuleGoToLookInDirection@NCameraModes@@UAEPAXI@Z`, `??_GCScriptModuleSelfTerminateBase@NCameraModes@@UAEPAXI@Z`
Header debt: none

## L020 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L020: CThingSearchTools::GetAllThings<TThing, list<TThing*>, TFilter>(const TFilter&, list<TThing*>&) const (140 bytes, thiscall, `ret 8`): walk the class list, keep scan-visible things of TThing's type mask that pass the filter, push_back each into the list (inline STLport insert: 0x0C-byte node, prev loaded once).
Identities:
- 005ed7d0 CThingSearchTools::GetAllThings<CThingAICreature, list<CThingAICreature*>, CThingFilter_OverlappingCircle> (proven) -- RELOCATION_MATCH
- 006c9280 CThingSearchTools::GetAllThings<CThingAICreature, list<CThingAICreature*>, CThingFilter_IsEligibleWitness> (proven) -- RELOCATION_MATCH
- 007d4ce0 CThingSearchTools::GetAllThings<CThingCreatureBase, list<CThingCreatureBase*>, CThingFilter_IsHuman> (proven) -- RELOCATION_MATCH
- 0092b2d0 CThingSearchTools::GetAllThings<CThingGameObject, list<CThingGameObject*>, CThingFilter_HasScriptName> (proven) -- RELOCATION_MATCH
Callee names introduced: `??RCThingFilter_IsEligibleWitness@@QBE_NABVCThing@@@Z`, `??RCThingFilter_OverlappingCircle@@QBE_NABVCThing@@@Z`
Header debt: none

## L021 -- 4 of 4 matched (4 MATCH)
Model: L021: STLport __uninitialized_copy<T*, T*> over trivially copyable class records (47 bytes, /Gr, `ret 8`): placement new's null test, the implicit copy as `rep movsd` of the record's dwords, return the advanced result.
Identities:
- 006127d0 std::__uninitialized_copy<CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*> (proven) -- MATCH
- 009ffeb0 std::__uninitialized_copy<CGraphicInfo*, CGraphicInfo*> (proven) -- MATCH
- 00b49760 std::__uninitialized_copy<CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*> (probable) -- MATCH
- 00b6bb50 std::__uninitialized_copy<CLandscapeTheme*, CLandscapeTheme*> (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L022 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L022: STLport 4.6 vector<T>::erase(iterator first, iterator last) over polymorphic def classes (109 bytes, thiscall, `ret 8`): the inline random-access __copy loop through the element's out-of-line operator=, then the inline _Destroy loop of virtual destructor calls (`push 0 ; call [vtbl]`), finish = the copy's end, return first.  Model: family L001.
Identities:
- 00632f80 std::vector<NUISystem::CUIStateDef>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 00f26500 std::vector<CArenaCreatureDef>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 00f28170 std::vector<CArenaWaveDef>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 00f337a0 std::vector<CConversation>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L022: landed 0x00F25F10 is named CArenaCreatureDef::Copy and 0x00E54C50 CConversation::Copy-labelled; vector::erase calls them as the element operator= (??4X@@QAEAAV0@ABV0@@Z here) -- one address, two spellings.
- L022/L023: 0x00625350 is bound ??4CConfigOptionsDefaultsDef@@ (landed 0x006314C0) -- it is NUISystem::CUIStateDef::operator= (vector<CUIStateDef> 124-byte stride; CConfigOptionsDefaultsDef is 120 bytes in Ego_r).
Callee names introduced: `??4CArenaCreatureDef@@QAEAAV0@ABV0@@Z`, `??4CArenaWaveDef@@QAEAAV0@ABV0@@Z`, `??4CConversation@@QAEAAV0@ABV0@@Z`, `??4CUIStateDef@NUISystem@@QAEAAV01@ABV01@@Z`
Header debt: none

## L023 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L023: STLport 4.6 vector<T>::_M_insert_overflow(pos, x, __false_type, fill_len, atend) for polymorphic def classes with out-of-line copy constructors (325 bytes, `ret 14h`): every helper inline -- head copy, x / fill, tail copy, the virtual destructor loop and the free.  The sibling erase rows are family L022.
Identities:
- 006331f0 std::vector<NUISystem::CUIStateDef>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00f26770 std::vector<CArenaCreatureDef>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00f28400 std::vector<CArenaWaveDef>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00f33a10 std::vector<CConversation>::_M_insert_overflow (proven) -- RELOCATION_MATCH
Label corrections / review:
- L022/L023: 0x00625350 is bound ??4CConfigOptionsDefaultsDef@@ (landed 0x006314C0) -- it is NUISystem::CUIStateDef::operator= (vector<CUIStateDef> 124-byte stride; CConfigOptionsDefaultsDef is 120 bytes in Ego_r).
Callee names introduced: none
Header debt: none

## L024 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L024: CTCLight script setters (42 bytes, thiscall, `ret 4`): when the light is overridden, store the value into the counted CLightInfo, then tell the parent thing its graphic appearance changed (reason 4) and re-add it to the render list.
Identities:
- 00640c40 CTCLight::SetInnerRadius (proven) -- RELOCATION_MATCH
- 00640c70 CTCLight::SetOuterRadius (proven) -- RELOCATION_MATCH
- 00640ca0 CTCLight::SetFlicker (proven) -- RELOCATION_MATCH
- 00640cd0 CTCLight::SetInverted (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: 00640c40: engine/CTCBase.h (ParentThing +0x04); Ego_r.pdb CTCLight members PLightInfo +0x10, PLightDef +0x18, Active +0x1C, (same model for 00640c70, 00640ca0, 00640cd0)

## L025 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L025: NPlayerGui::CDrawAbilityButtonsPC mapping getters (209 bytes, thiscall, `ret 4`): clear the caller's vector<long> and push three fixed entries (STLport clear = erase(begin, end) over __copy_trivial; push_back with the POD growth path 0x00440990 = vector<long>::_M_insert_overflow(.., __true_type, 1, true)).
Identities:
- 006579e0 NPlayerGui::CDrawAbilityButtonsPC::GetAbilityPositionMapping (probable) -- RELOCATION_MATCH
- 00657ac0 NPlayerGui::CDrawAbilityButtonsPC::GetAbilityButtonMapping (probable) -- RELOCATION_MATCH
- 00657ba0 NPlayerGui::CDrawAbilityButtonsPC::GetExpressionPositionMapping (probable) -- RELOCATION_MATCH
- 00657c80 NPlayerGui::CDrawAbilityButtonsPC::GetExpressionButtonMapping (probable) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L026 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L026: STLport 4.6 _List_base<CIntelligentPointer<T> >::clear() in a size-optimised TU (50 bytes, thiscall, plain `ret`): walk the nodes, destroy each intelligent pointer through its virtual destructor, free the node, relink the sentinel. The four CTCVillage thing lists (the landed list-destructor stubs 0x0065F1C8.. call them from ~CTCVillage 0x0065B598).
Identities:
- 0065e4d1 std::_List_base<CIntelligentPointer<CThingBuilding> >::clear (proven) -- RELOCATION_MATCH
- 0065e503 std::_List_base<CIntelligentPointer<CThingObject> >::clear (proven) -- RELOCATION_MATCH
- 0065e535 std::_List_base<CIntelligentPointer<CThingTrackNode> >::clear (proven) -- RELOCATION_MATCH
- 0065e567 std::_List_base<CIntelligentPointer<CThingMarker> >::clear (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L027 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L027: STLport 4.6 list<CCountedPointer<CVillageTaskX> >::remove_if<CIsTask>(CIsTask) in the CTCVillage size TU (88 bytes, thiscall, `ret 4`, ebp frame): walk the list, test each task through CIsTask (a CCountedPointer<CVillageTaskBase> temporary from the converting copy, compared with the predicate's task, released), erase the matches out of line.
Identities:
- 00660240 std::list<CCountedPointer<CVillageTaskTorch> >::remove_if<CIsTask> (proven) -- RELOCATION_MATCH
- 00660298 std::list<CCountedPointer<CVillageTaskHorn> >::remove_if<CIsTask> (proven) -- RELOCATION_MATCH
- 006602f0 std::list<CCountedPointer<CVillageTaskSecuritySweep> >::remove_if<CIsTask> (proven) -- RELOCATION_MATCH
- 00660348 std::list<CCountedPointer<CVillageTaskCarryCrate> >::remove_if<CIsTask> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L028 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L028: release of one CCountedPointer member (49/64 bytes, thiscall, plain `ret`): drop the shared count (reloading Info after the decrement), on zero call the info's fastcall deleter on the data and delete the info, then clear Info and Data.  Two named member functions and two unreferenced destructors of {12 bytes, CCountedPointer} records.
Identities:
- 006633a0 CThingCreatureBase::ClearCurrentAction (probable) -- RELOCATION_MATCH
- 00b43ef0 CUnprovenRecord_00b43ef0::~CUnprovenRecord_00b43ef0 (descriptive) -- RELOCATION_MATCH
- 00be0810 CUnprovenRecord_00be0810::~CUnprovenRecord_00be0810 (descriptive) -- RELOCATION_MATCH
- 00f35a30 CSpawnedFuncBase::RemovePredicate (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L029 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L029: default constructor of an 8-byte record {CCharString at +0, scalar at +4}: construct the string (0x0099E4B0) then zero the scalar (dword, or byte for a bool).  All four are unreferenced COMDATs (no call, jump or data reference), so the record types are descriptive.
Identities:
- 00673090 CUnprovenRecord_00673090::CUnprovenRecord_00673090 (descriptive) -- RELOCATION_MATCH
- 007aba60 CUnprovenRecord_007aba60::CUnprovenRecord_007aba60 (descriptive) -- RELOCATION_MATCH
- 00a45c90 CUnprovenRecord_00a45c90::CUnprovenRecord_00a45c90 (descriptive) -- RELOCATION_MATCH
- 00ae47b0 CUnprovenRecord_00ae47b0::CUnprovenRecord_00ae47b0 (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L030 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L030: STLport 4.6 vector<T>::operator=(const vector&) for 16-byte, trivially destroyed class elements (253 bytes, thiscall, `ret 4`): grow through malloc + out-of-line __uninitialized_copy + free, else __copy_ptrs over the live part (+ __uninitialized_copy for the tail).  The empty __false_type tags share the x argument slot only when both tag-taking helpers have same-TU noinline bodies.
Identities:
- 00674300 std::vector<std::pair<long, CRandomAppearanceMorph::CSkeletalMorph>>::operator= (proven) -- RELOCATION_MATCH
- 00a54bb0 std::vector<CQuaternion>::operator= (probable) -- RELOCATION_MATCH
- 00a81690 std::vector<CNavigationTransitionPoint>::operator= (proven) -- RELOCATION_MATCH
- 00a81790 std::vector<C2DBoxF>::operator= (proven) -- RELOCATION_MATCH
Label corrections / review:
- L030 0x674300 bound stand-in ??4?$CArray@UCSkeletalMorph@@@@ is really std::vector<pair<long,CRandomAppearanceMorph::CSkeletalMorph>>::operator=; landed 0x515320 vector<C2DLineF>::operator= twin uses invented namespace FableAssign_00515320 + /Gr
Callee names introduced: none
Header debt: none

## L031 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L031: STLport 4.6 __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) over classes whose implicit operator= calls one out-of-line member assignment (a vector's or CCharString's) and copies one scalar (59 bytes, __fastcall, `ret 0Ch`).
Identities:
- 00674a40 std::__copy<const CTextureMorphMap*, CTextureMorphMap*, int> (proven) -- RELOCATION_MATCH
- 006d3b50 std::__copy<std::pair<CCharString, long>*, std::pair<CCharString, long>*, int> (proven) -- RELOCATION_MATCH
- 006dbd50 std::__copy<CAttackHistoryCombo*, CAttackHistoryCombo*, int> (proven) -- RELOCATION_MATCH
- 0086dc10 std::__copy<CTextureMorphMap*, CTextureMorphMap*, int> (proven) -- RELOCATION_MATCH
Callee names introduced: `??4?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@2@@std@@QAEAAV01@ABV01@@Z`
Header debt: none

## L032 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L032: STLport 4.6 vector<T>::operator=(const vector&) for 20/24-byte class elements with real destructors (311 bytes, thiscall, `ret 4`): __copy_ptrs inlines to the out-of-line __copy, and _M_clear / the _Destroy range stay out of line. The empty tags share the x argument slot only when __copy and __uninitialized_copy have same-TU noinline bodies.
Identities:
- 006755e0 std::vector<CRandomAppearanceMorph::CBodyPartMesh>::operator= (proven) -- RELOCATION_MATCH
- 00722f30 std::vector<CTCSmokeGenerator::CGenerationPos>::operator= (proven) -- RELOCATION_MATCH
- 00728830 std::vector<CEngineTextureMorph>::operator= (proven) -- RELOCATION_MATCH
- 00a46790 std::vector<C3DSkeletalBoneMorph>::operator= (proven) -- RELOCATION_MATCH
Label corrections / review:
- L032 0x6755e0 bound stand-in ??4CElement12_006755e0 is std::vector<CRandomAppearanceMorph::CBodyPartMesh>::operator=; 0xa46790 stand-ins ??4?$CArray@VC3DSkeletalBoneMorph / ??4CBoneMorphArray are vector<C3DSkeletalBoneMorph>::operator=; 0x5bbc93/0x6cb340 bound with malformed manglings (one-arg __uninitialized_copy, U-allocator _M_clear)
Callee names introduced: `??$_Destroy@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@std@@YIXPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@0@0@Z`, `??$_Destroy@PAVC3DSkeletalBoneMorph@@@std@@YIXPAVC3DSkeletalBoneMorph@@0@Z`, `??$_Destroy@PAVCBodyPartMesh@CRandomAppearanceMorph@@@std@@YIXPAVCBodyPartMesh@CRandomAppearanceMorph@@0@Z`, `??$_Destroy@PAVCCompositeBlendDesc@@@std@@YIXPAVCCompositeBlendDesc@@0@Z`, `??$_Destroy@PAVCEngineTextureMorph@@@std@@YIXPAVCEngineTextureMorph@@0@Z`, `??$_Destroy@PAVCGenerationPos@CTCSmokeGenerator@@@std@@YIXPAVCGenerationPos@CTCSmokeGenerator@@0@Z`, `??0CBaseIntelligentPointer@@QAE@ABV0@@Z`, `??4CBaseIntelligentPointer@@QAEAAV0@ABV0@@Z`, `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`, `?_M_clear@?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@2@@std@@IAEXXZ`, `?_M_clear@?$vector@VC3DSkeletalBoneMorph@@V?$allocator@VC3DSkeletalBoneMorph@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@VCBodyPartMesh@CRandomAppearanceMorph@@V?$allocator@VCBodyPartMesh@CRandomAppearanceMorph@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@VCCompositeBlendDesc@@V?$allocator@VCCompositeBlendDesc@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@VCGenerationPos@CTCSmokeGenerator@@V?$allocator@VCGenerationPos@CTCSmokeGenerator@@@std@@@std@@IAEXXZ`
Header debt: none

## L033 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L033: GFSerialiseMapBinaryOut<V, K, less<K> >(CDataOutputStream&, const map<K, V>&) (123 bytes, __fastcall, plain `ret`): write the node count, then each key and value in order through CPersistTraits<>::TransferBinaryOut (WriteSLONG for enums / long / CDefString, WriteFloat for float), walking the tree with the inline _Rb_global<bool>::_M_increment.
Identities:
- 0067b8b0 GFSerialiseMapBinaryOut<EOpinionAttitudeType, EOpinionReactionType, std::less<EOpinionReactionType> > (proven) -- RELOCATION_MATCH
- 0067b930 GFSerialiseMapBinaryOut<EOpinionTargetingConditionType, EOpinionReactionType, std::less<EOpinionReactionType> > (proven) -- RELOCATION_MATCH
- 0067b9b0 GFSerialiseMapBinaryOut<float, EOpinionReactionType, std::less<EOpinionReactionType> > (proven) -- RELOCATION_MATCH
- 006dae40 GFSerialiseMapBinaryOut<CDefString, long, std::less<long> > (proven) -- RELOCATION_MATCH
Label corrections / review:
- L033 0x67b8b0/0x67b930/0x67b9b0 bound stand-ins ?GFSerialiseMapBinaryOut@@YIXPAVCDataOutputStream@@PAUCReaction*Map@@@Z (invented struct types, non-template) -> real ??$GFSerialiseMapBinaryOut<V,K,less<K>> template; CDataOutputStream declared locally with methods (header has layout only)
Callee names introduced: none
Header debt: none

## L034 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L034: STLport 4.6 list<T>::erase(iterator first, iterator last) for trivially destroyed T (54 bytes, thiscall, `ret 0Ch`): unlink and free each node, store last through the hidden iterator result (the iterator has a user copy constructor, so it is returned in memory).
Identities:
- 0067cfb0 std::list<CReactionMatch*>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 0076fbb0 std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 00a219a0 std::list<CUnprovenElement_00a219a0>::erase(iterator, iterator) (descriptive) -- RELOCATION_MATCH
- 00a37720 std::list<CClass*>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L034 0xa37720 bound ?erase@?$list@PAVCClass@@@std@@ lacks the allocator template argument (stand-in mangling)
Callee names introduced: none
Header debt: none

## L035 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L035: STLport 4.6 vector<T>::resize(size_type n) for 8-byte POD-like T (85 bytes, thiscall, `ret 4`): build the value-initialised T() temporary, then erase(begin() + n, end()) or insert(end(), n - size(), x) -> the out-of-line erase(first, last) / _M_fill_insert(pos, n, x).
Identities:
- 00681340 std::vector<std::pair<EReactionSpeechType, long>>::resize(size_type) (proven) -- RELOCATION_MATCH
- 007b5210 std::vector<CCrimeWaitingForBribe>::resize(size_type) (proven) -- RELOCATION_MATCH
- 009b8350 std::vector<std::pair<unsigned long, unsigned long>>::resize(size_type) (probable) -- RELOCATION_MATCH
- 00a39ae0 std::vector<std::pair<unsigned long, long>>::resize(size_type) (probable) -- RELOCATION_MATCH
Label corrections / review:
- L035 0x7b5210: CCrimeWaitingForBribe modelled with an inline zeroing default ctor (value-init T() of a ctor-less class reorders the temp stores); 0x681340 stand-in ?resize@?$CArray@UCVoiceEntry is vector<pair<EReactionSpeechType,long>>::resize; 0x9b8350 stand-in ?resize@CHandleMapVector is vector<pair<ulong,ulong>>::resize
Callee names introduced: `?_M_fill_insert@?$vector@U?$pair@KJ@std@@V?$allocator@U?$pair@KJ@std@@@2@@std@@IAEXPAU?$pair@KJ@2@IABU32@@Z`, `?_M_fill_insert@?$vector@U?$pair@KK@std@@V?$allocator@U?$pair@KK@std@@@2@@std@@IAEXPAU?$pair@KK@2@IABU32@@Z`, `?_M_fill_insert@?$vector@U?$pair@W4EReactionSpeechType@@J@std@@V?$allocator@U?$pair@W4EReactionSpeechType@@J@std@@@2@@std@@IAEXPAU?$pair@W4EReactionSpeechType@@J@2@IABU32@@Z`, `?_M_fill_insert@?$vector@VCCrimeWaitingForBribe@@V?$allocator@VCCrimeWaitingForBribe@@@std@@@std@@IAEXPAVCCrimeWaitingForBribe@@IABV3@@Z`, `?erase@?$vector@U?$pair@KJ@std@@V?$allocator@U?$pair@KJ@std@@@2@@std@@QAEPAU?$pair@KJ@2@PAU32@0@Z`, `?erase@?$vector@U?$pair@KK@std@@V?$allocator@U?$pair@KK@std@@@2@@std@@QAEPAU?$pair@KK@2@PAU32@0@Z`, `?erase@?$vector@U?$pair@W4EReactionSpeechType@@J@std@@V?$allocator@U?$pair@W4EReactionSpeechType@@J@std@@@2@@std@@QAEPAU?$pair@W4EReactionSpeechType@@J@2@PAU32@0@Z`, `?erase@?$vector@VCCrimeWaitingForBribe@@V?$allocator@VCCrimeWaitingForBribe@@@std@@@std@@QAEPAVCCrimeWaitingForBribe@@PAV3@0@Z`
Header debt: none

## L036 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L036: scalar deleting destructor of a class whose only non-trivial members are {CCountedPointer at +4} (plus a CCharString at +0 or a CBaseClass-family base) (81 bytes, thiscall, `ret 4`): release the counted pointer (Data then Info cleared), run the base / CCharString destructor, free when bit 0 is set.
Identities:
- 006963f0 CUnprovenRecord_006963f0::`scalar deleting destructor' (descriptive) -- RELOCATION_MATCH
- 00a0bd80 CASoundBank::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
- 00af6b90 NParticleEngine::CComponentInstance::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
- 00beda30 CCompositeSurfaceInfo::`scalar deleting destructor' (proven) -- RELOCATION_MATCH
Callee names introduced: `??_GCASoundBank@@UAEPAXI@Z`, `??_GCComponentInstance@NParticleEngine@@UAEPAXI@Z`, `??_GCCompositeSurfaceInfo@@UAEPAXI@Z`
Header debt: none

## L037 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L037: four-argument forwarding constructors of leaf action classes (42 bytes, thiscall, `ret 10h`): call the direct base's matching constructor with the same arguments, then store the class's own vtable.
Identities:
- 0069f060 CCreatureAction_SheatheItemToInventory_ForEnflame::CCreatureAction_SheatheItemToInventory_ForEnflame (proven) -- RELOCATION_MATCH
- 0069f090 CCreatureAction_SheatheItemToInventory_ForDivineWrath::CCreatureAction_SheatheItemToInventory_ForDivineWrath (proven) -- RELOCATION_MATCH
- 0069f0c0 CCreatureAction_SheatheItemToInventory_ForUnholyPower::CCreatureAction_SheatheItemToInventory_ForUnholyPower (proven) -- RELOCATION_MATCH
- 00867990 CCombatAction_GenericStrikeResponse::CCombatAction_GenericStrikeResponse (proven) -- RELOCATION_MATCH
Label corrections / review:
- L037 0x69f060 bound as ??0CCombatAction_GenericStrikeResponse (wrong) is CCreatureAction_SheatheItemToInventory_ForEnflame ctor; 0x69f090/0x69f0c0/0x69ef80 bound with PAVCThingCreatureBase (pointer) but Ego_r.pdb has AAV (reference) -> our names use the PDB reference form
Callee names introduced: `??0CCombatAction_GenericStrikeResponseBase@@QAE@AAVCThingCreatureBase@@PBVCThing@@ABVC3DVector@@2@Z`, `??0CCreatureAction_SheatheItemToInventory@@QAE@AAVCThingCreatureBase@@ABVCCharString@@PAVCCreatureActionBase@@J@Z`, `??1CCombatAction_GenericStrikeResponseBase@@UAE@XZ`, `??_GCCombatAction_GenericStrikeResponse@@UAEPAXI@Z`, `??_GCCreatureAction_SheatheItemToInventory_ForDivineWrath@@UAEPAXI@Z`, `??_GCCreatureAction_SheatheItemToInventory_ForEnflame@@UAEPAXI@Z`, `??_GCCreatureAction_SheatheItemToInventory_ForUnholyPower@@UAEPAXI@Z`
Header debt: none

## L038 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L038: default constructors of records led by a CCharString (20 bytes, thiscall, plain `ret`): construct the string (0x0099E4B0), then zero two members -- a CCountedPointer's Data/Info pair (+4/+8) or two bools (+0x10/+0x11).
Identities:
- 006a4b30 CUnprovenRecord_006a4b30::CUnprovenRecord_006a4b30 (descriptive) -- RELOCATION_MATCH
- 007559c0 CUnprovenRecord_007559c0::CUnprovenRecord_007559c0 (descriptive) -- RELOCATION_MATCH
- 00a325e0 CUnprovenRecord_00a325e0::CUnprovenRecord_00a325e0 (descriptive) -- RELOCATION_MATCH
- 00a47f90 CUnprovenRecord_00a47f90::CUnprovenRecord_00a47f90 (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L039 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L039: a thing component's "add this thing to my list" method (122 bytes, thiscall, `ret 4`): build a CIntelligentPointer temporary from the CThing reference, push_back it onto a list<CIntelligentPointer<..> > member (malloc a 16-byte node, copy-construct the pointer into it, link it before the sentinel), destroy the temporary.
Identities:
- 006aedc0 CTCRegionFollower::AddFollower (probable) -- RELOCATION_MATCH
- 0071a940 CTCParticleAttachmentTracker::SetParticleAsAttached (probable) -- RELOCATION_MATCH
- 0071b300 CTCThingOwner::AddOwnedObject (probable) -- RELOCATION_MATCH
- 0076d270 CTCEnemy::AddAllyThing (proven) -- RELOCATION_MATCH
Label corrections / review:
- L039 stand-ins: 0x6aedc0 (CTCFollowerList::AddThing etc) -> CTCRegionFollower::AddFollower; 0x71a940 (?RemoveDependentThing@CTCDependentThings) -> CTCParticleAttachmentTracker::SetParticleAsAttached; 0x76d270 (?AddAlly@CTCAllies_Retail) -> CTCEnemy::AddAllyThing
Callee names introduced: `??_G?$CIntelligentPointer@$$CBVCThing@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`
Header debt: none

## L040 -- 4 of 4 matched (4 MATCH)
Model: L040: STLport 4.6 vector<T>::clear() for 8-byte class elements (43 bytes, thiscall, plain `ret`): erase(begin(), end()) inline -- the __copy_ptrs/__copy loop over the empty [finish, finish) range survives (n = finish - finish is not folded), trivial destructors, then _M_finish = begin.
Identities:
- 006b5b50 std::vector<std::pair<long, float>>::clear (probable) -- MATCH
- 006c85e0 std::vector<std::pair<long, COpinionDeed*>>::clear (probable) -- MATCH
- 00755320 std::vector<std::pair<float, const CAnimationEntry*>>::clear (proven) -- MATCH
- 007b4bb0 std::vector<CCrimeWaitingForBribe>::clear (proven) -- MATCH
Label corrections / review:
- L040 0x6b5b50/0x6c85e0/0x755320/0x7b4bb0 catalogued as propagated CComponent::SetParentColour; landed 0x6c9ee0 calls 0x6c85e0 vector<COpinionDeedRecord>::clear (stand-in element) -> vector<pair<long,COpinionDeed*>>; 0x6b3e60 bound ?SetCameraThemes@CEnvironment
Callee names introduced: none
Header debt: none

## L041 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L041: STLport 4.6 vector<T>::resize(size_type n, const T& x) for 7-byte-multiple class/struct elements (92 bytes, thiscall, `ret 8`): erase(begin() + n, end()) inline (__copy_ptrs -> the out-of-line __copy, tag homed on the n slot, trivial destructors) or insert(end(), n - size(), x) -> the out-of-line _M_fill_insert.
Identities:
- 006b81f0 std::vector<CGameMapCell>::resize(size_type, const T&) (proven) -- RELOCATION_MATCH
- 00b4a4c0 std::vector<CEngineLightingManager::CThemeChannel>::resize(size_type, const T&) (proven) -- RELOCATION_MATCH
- 00b6cb20 std::vector<CLandscapeTheme>::resize(size_type, const T&) (proven) -- RELOCATION_MATCH
- 00b8f780 std::vector<CEngineOcclusionManager::COccluderFlareEntry>::resize(size_type, const T&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L041 stand-ins: 0x6b81f0 ?resize@CGameMapCellVector -> vector<CGameMapCell>::resize(n,x); 0xb8f780 ?resize@COccluderVector(SOccluderBehaviour) -> vector<CEngineOcclusionManager::COccluderFlareEntry>::resize(n,x)
- L041: 0x006B7920 / 0x00B490F0 / 0x00B6B660 / 0x00B8E8B0 are __copy<T*, T*, int> with NON-const source iterators (erase passes iterator to the templated STLport __copy_ptrs<_InputIter, _OutputIter>); the byte-identical const-source twin 0x006B79A0 is vector<CGameMapCell>::operator='s (landed 0x006B7E20) -- fixed after vland RELOC_CONFLICT.
Callee names introduced: `?_M_fill_insert@?$vector@UCLandscapeTheme@@V?$allocator@UCLandscapeTheme@@@std@@@std@@IAEXPAUCLandscapeTheme@@IABU3@@Z`, `?_M_fill_insert@?$vector@UCOccluderFlareEntry@CEngineOcclusionManager@@V?$allocator@UCOccluderFlareEntry@CEngineOcclusionManager@@@std@@@std@@IAEXPAUCOccluderFlareEntry@CEngineOcclusionManager@@IABU34@@Z`, `?_M_fill_insert@?$vector@UCThemeChannel@CEngineLightingManager@@V?$allocator@UCThemeChannel@CEngineLightingManager@@@std@@@std@@IAEXPAUCThemeChannel@CEngineLightingManager@@IABU34@@Z`, `?_M_fill_insert@?$vector@VCGameMapCell@@V?$allocator@VCGameMapCell@@@std@@@std@@IAEXPAVCGameMapCell@@IABV3@@Z`
Header debt: none

## L042 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L042: CPersistContext_TransferableVector<T>::TransferOut / TransferIn (50 bytes, thiscall, `ret 0Ch`): build a CPersistTraits<T> (its zeroed T DefaultVal in the frame) and forward (context, text, (*TransVector)[index]) to the traits' out-of-line TransferOut / TransferIn.
Identities:
- 006c78b0 CPersistContext_TransferableVector<std::pair<long, COpinionDeed*>>::TransferOut (proven) -- RELOCATION_MATCH
- 006c9310 CPersistContext_TransferableVector<std::pair<long, COpinionDeed*>>::TransferIn (proven) -- RELOCATION_MATCH
- 007b4020 CPersistContext_TransferableVector<CCrimeWaitingForBribe>::TransferOut (proven) -- RELOCATION_MATCH
- 007b4770 CPersistContext_TransferableVector<CCrimeWaitingForBribe>::TransferIn (proven) -- RELOCATION_MATCH
Callee names introduced: `?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z`, `?TransferIn@?$CPersistTraits@VCCrimeWaitingForBribe@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCrimeWaitingForBribe@@@Z`, `?TransferOut@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z`, `?TransferOut@?$CPersistTraits@VCCrimeWaitingForBribe@@@@QAEXAAVCPersistContext@@AAVCCharString@@AAVCCrimeWaitingForBribe@@@Z`
Header debt: none

## L043 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L043: STLport push_back(const T& x) for 8-byte {CCharString, 4-byte scalar} records (72 bytes, thiscall, `ret 4`): placement copy at finish (null test, CCharString's out-of-line copy constructor, one dword copy), else _M_insert_overflow(finish, x, __false_type, 1, true) with the tag homed on x's slot.
Identities:
- 006d4970 std::vector<std::pair<CCharString, long>>::push_back (probable) -- RELOCATION_MATCH
- 006fc520 std::vector<std::pair<CCharString, float>>::push_back (probable) -- RELOCATION_MATCH
- 0086d7d0 std::vector<CRandomAppearanceMorph::CMorphSet::CMorphSetSkeletalMorph>::push_back (probable) -- RELOCATION_MATCH
- 00a2d8b0 CArray<CMeshDataBank::CLipSyncEntry>::push_back (proven) -- RELOCATION_MATCH
Callee names introduced: `?_M_insert_overflow@?$vector@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAEXPAU?$pair@VCCharString@@J@2@ABU32@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCLipSyncEntry@CMeshDataBank@@V?$allocator@VCLipSyncEntry@CMeshDataBank@@@std@@@std@@QAEXPAVCLipSyncEntry@CMeshDataBank@@ABV34@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@V?$allocator@VCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@@std@@@std@@QAEXPAVCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@ABV345@ABU__false_type@2@I_N@Z`
Header debt: none

## L044 -- 4 of 4 matched (4 MATCH)
Model: L044: STLport 4.6 _Rb_tree<unsigned __int64, V, KoV, less<unsigned __int64> >::_M_find(const key&) const (84 bytes, thiscall, `ret 4`) -- family L008's body (same bytes) for four more trees.
Identities:
- 006f0250 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>, ...>::_M_find (proven) -- MATCH
- 00782e90 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CIntelligentPointer<CThing>>, ...>::_M_find (proven) -- MATCH
- 0082eeb0 std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>, ...>::_M_find (proven) -- MATCH
- 0082ef20 std::_Rb_tree<unsigned __int64, unsigned __int64, _Identity<unsigned __int64>, ...>::_M_find (proven) -- MATCH
Label corrections / review:
- L044 0x82ef20 bound ?_M_find@?$_Rb_tree@_K_KU?$less... lacks _Identity<_K> (stand-in mangling); 0x6f0250/0x782e90 stand-ins (CombatMap::FindLE, CHeroCrowdMap::find) -> _Rb_tree::_M_find of CCombatManager::Combatants / CTCBoastingCrowdControl::HeroCrowdThings
Callee names introduced: none
Header debt: none

## L045 -- 4 of 4 matched (4 MATCH)
Model: L045: STLport 4.6 __copy_ptrs<const T*, T*>(first, last, result, const __false_type&) for 8-byte trivially-copied class elements (38 bytes, __fastcall, `ret 8`): the random-access __copy inlined -- n = (last - first) >> 3 two-dword assignments, return the advanced result.
Identities:
- 00708590 std::__copy_ptrs<const CTCHeroAttachableAppearanceModifiers::CModifierEntry*, CTCHeroAttachableAppearanceModifiers::CModifierEntry*> (proven) -- MATCH
- 009b3a10 std::copy<const std::pair<unsigned long, unsigned long>*, std::pair<unsigned long, unsigned long>*> (proven) -- MATCH
- 00a5de10 std::__copy_ptrs<const _D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9*> (proven) -- MATCH
- 00ae5a80 std::__copy_ptrs<const C3DVertexBlend2*, C3DVertexBlend2*> (probable) -- MATCH
Callee names introduced: none
Header debt: none

## L046 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L046: STLport 4.6 vector<T>::operator=(const vector&) for 8-byte trivially-destroyed class elements (198 bytes, thiscall, `ret 4`): grow through the out-of-line _M_allocate_and_copy (+ free), else __copy_ptrs over the live part (+ __uninitialized_copy for the tail); tags homed on the x slot via same-TU noinline helper bodies.
Identities:
- 00708ca0 std::vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry>::operator= (proven) -- RELOCATION_MATCH
- 00719470 std::vector<std::pair<float, EConversationAttitude>>::operator= (proven) -- RELOCATION_MATCH
- 00a5e2e0 std::vector<_D3DVERTEXELEMENT9>::operator= (proven) -- RELOCATION_MATCH
- 00ae6690 std::vector<C3DVertexBlend2>::operator= (probable) -- RELOCATION_MATCH
Label corrections / review:
- L046 stand-ins: 0x708ca0 ??4CElement12_00708ca0 -> vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry>::operator=; 0x719470 ??4?$CTagVector@W4EConversationAttitude -> vector<pair<float,EConversationAttitude>>::operator=; 0xa5e2e0 ??4?$CArray@U_D3DVERTEXELEMENT9 -> vector<_D3DVERTEXELEMENT9>::operator=; landed 0x9b54e0 comment correct that 0x9b3a10 is copy() ret 4
Callee names introduced: `??$_M_allocate_and_copy@PBU?$pair@MW4EConversationAttitude@@@std@@@?$vector@U?$pair@MW4EConversationAttitude@@@std@@V?$allocator@U?$pair@MW4EConversationAttitude@@@std@@@2@@std@@IAEPAU?$pair@MW4EConversationAttitude@@@1@IPBU21@0@Z`, `??$_M_allocate_and_copy@PBU_D3DVERTEXELEMENT9@@@?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@std@@@std@@IAEPAU_D3DVERTEXELEMENT9@@IPBU2@0@Z`, `??$_M_allocate_and_copy@PBVC3DVertexBlend2@@@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@std@@@std@@IAEPAVC3DVertexBlend2@@IPBV2@0@Z`, `??$_M_allocate_and_copy@PBVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@@?$vector@VCModifierEntry@CTCHeroAttachableAppearanceModifiers@@V?$allocator@VCModifierEntry@CTCHeroAttachableAppearanceModifiers@@@std@@@std@@IAEPAVCModifierEntry@CTCHeroAttachableAppearanceModifiers@@IPBV23@0@Z`
Header debt: none

## L047 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L047: implicit destructors of unreferenced records holding two STLport vectors of trivially-destroyed elements (37 bytes, thiscall, plain `ret`): free the second vector's block, then the first's (members destroyed in reverse).
Identities:
- 0070b800 CUnprovenRecord_0070b800::~CUnprovenRecord_0070b800 (descriptive) -- RELOCATION_MATCH
- 0070b870 CUnprovenRecord_0070b870::~CUnprovenRecord_0070b870 (descriptive) -- RELOCATION_MATCH
- 00b6c9e0 CUnprovenRecord_00b6c9e0::~CUnprovenRecord_00b6c9e0 (descriptive) -- RELOCATION_MATCH
- 00b7ed40 CUnprovenRecord_00b7ed40::~CUnprovenRecord_00b7ed40 (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L048 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L048: STLport 4.6 vector<T>::_M_allocate_and_copy<FwdIt>(size_type n, FwdIt first, FwdIt last) for 4-byte class / enum elements (72 bytes, thiscall, `ret 0Ch`): allocate n elements (malloc or null), then the inlined __uninitialized_copy(first, last, result, __false_type) -- a placement copy per element under the null test.
Identities:
- 0070fb40 std::vector<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >>::_M_allocate_and_copy<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >*> (proven) -- RELOCATION_MATCH
- 0082c450 std::vector<CMinimapData::MinimapTheme>::_M_allocate_and_copy<const CMinimapData::MinimapTheme*> (proven) -- RELOCATION_MATCH
- 0085dca0 std::vector<NThingMap::CThingMapCell>::_M_allocate_and_copy<const NThingMap::CThingMapCell*> (proven) -- RELOCATION_MATCH
- 009c1970 std::vector<ESurfaceMultisampleType>::_M_allocate_and_copy<ESurfaceMultisampleType*> (proven) -- RELOCATION_MATCH
Label corrections / review:
- L048 access: _M_allocate_and_copy emitted public (QAE) to match the two well-formed bindings (0x70fb40, 0x9c1970); STLport 4.6 declares it protected -- landed bindings are mixed QAE/IAE (38/14), settle at landing; 0x82c450/0x85dca0 bound with non-template manglings
Callee names introduced: none
Header debt: none

## L049 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L049: STLport 4.6 __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) over records {4-byte scalar, member with an out-of-line assignment} (61 bytes, __fastcall, `ret 0Ch`): per element copy the scalar, then call the member's operator= (CCharString or a vector).
Identities:
- 00718d10 std::__copy<const std::pair<float, CCharString>*, std::pair<float, CCharString>*, int> (probable) -- RELOCATION_MATCH
- 0074f640 std::__copy<std::pair<long, CCharString>*, std::pair<long, CCharString>*, int> (proven) -- RELOCATION_MATCH
- 00ae7660 std::__copy<const C3DGroup2*, C3DGroup2*, int> (probable) -- RELOCATION_MATCH
- 00bf8e50 std::__copy<CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, int> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L050 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L050: STLport 4.6 sort(first, last, comp) over CVectorMap pair elements with a by-value CKeyPairCompareLess (82/83 bytes, /Gr __fastcall, `ret 4`): when first != last, the out-of-line __introsort_loop with depth __lg(n) * 2, then the out-of-line __final_insertion_sort.
Identities:
- 00721fe0 std::sort<std::pair<long, CTCHeroMorph::CTextureLayer>*, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> > (proven) -- RELOCATION_MATCH
- 00827710 std::sort<std::pair<long, CCountedPointer<CContainmentVolume>>*, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume>> > (proven) -- RELOCATION_MATCH
- 00a84cf0 std::sort<std::pair<long, CCountedPointer<CNavConnectorNode>>*, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode>> > (proven) -- RELOCATION_MATCH
- 00cbd370 std::sort<std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*, CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> > (proven) -- RELOCATION_MATCH
Label corrections / review:
- L050 stand-ins: 0x721fe0 ?MakeCompositeHeap, 0x827710 ?GuiTarget_Set, 0xa84cf0 ?CGuiVarTransferBool_Apply/?Sub_Handle -> std::sort<pair<..>*, CKeyPairCompareLess<..>>; catalog 'CGuiVarTransferBool::TransferToGui' labels on the CVectorMap Sort callers are propagated
Callee names introduced: `??$__final_insertion_sort@PAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@std@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@0@0V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@Z`, `??$__final_insertion_sort@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@0V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@Z`, `??$__final_insertion_sort@PAU?$pair@JVCTextureLayer@CTCHeroMorph@@@std@@V?$CKeyPairCompareLess@JVCTextureLayer@CTCHeroMorph@@@@@std@@YIXPAU?$pair@JVCTextureLayer@CTCHeroMorph@@@0@0V?$CKeyPairCompareLess@JVCTextureLayer@CTCHeroMorph@@@@@Z`, `??$__final_insertion_sort@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@0V?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z`, `??$__introsort_loop@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@U12@HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@00HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z`
Header debt: none

## L051 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L051: STLport 4.6 __uninitialized_copy<In, T*>(first, last, result, const __false_type&) over 20-byte {C3DVector, CIntelligentPointer<CThing>} records (90 bytes, __fastcall, `ret 8`): placement copy-construct each element under the null test -- the three vector dwords, then the intelligent pointer's copy constructor (base ctor, vtable, SetPItem(other.GetPItem())).
Identities:
- 00722b70 std::__uninitialized_copy<CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*> (proven) -- RELOCATION_MATCH
- 00722c30 std::__uninitialized_copy<const CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*> (proven) -- RELOCATION_MATCH
- 007924f0 std::__uninitialized_copy<CTCDivineWrath::PerEffectInfo*, CTCDivineWrath::PerEffectInfo*> (proven) -- RELOCATION_MATCH
- 00797950 std::__uninitialized_copy<CTCUnholyPower::PerEffectInfo*, CTCUnholyPower::PerEffectInfo*> (proven) -- RELOCATION_MATCH
Callee names introduced: `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`
Header debt: none

## L052 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L052: STLport 4.6 fill<T*, T>(first, last, const T& value) over records {4-byte scalar, member with an out-of-line assignment} (46 bytes, /Gr __fastcall, `ret 4`): per element copy the scalar, then call the member's operator=.
Identities:
- 00728c10 std::fill<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts*, CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts> (probable) -- RELOCATION_MATCH
- 009ca1f0 std::fill<NGameText::CTextTag*, NGameText::CTextTag> (proven) -- RELOCATION_MATCH
- 00ae79f0 std::fill<C3DGroup2*, C3DGroup2> (probable) -- RELOCATION_MATCH
- 00bf8e20 std::fill<CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L053 -- 4 of 4 matched (4 MATCH)
Model: L053: STLport 4.6 __push_heap<T*, int, T, Compare>(first, holeIndex, topIndex, value, comp) over 8-byte records keyed by an unsigned first dword (79 bytes, /Gr __fastcall, `ret 10h`): sift the hole up while the parent's key is below the value's, then store the value.
Identities:
- 0073d020 std::__push_heap<std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >*, int, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >> (proven) -- MATCH
- 009b1d10 std::__push_heap<std::pair<unsigned long, long >*, int, std::pair<unsigned long, long >, CKeyPairCompareLess<unsigned long, long >> (proven) -- MATCH
- 009b1d80 std::__push_heap<std::pair<unsigned long, unsigned long >*, int, std::pair<unsigned long, unsigned long >, CKeyPairCompareLess<unsigned long, unsigned long >> (proven) -- MATCH
- 00b84fd0 std::__push_heap<CEngineSubPrimitiveRenderer::CSortElement*, int, CEngineSubPrimitiveRenderer::CSortElement, std::less<CEngineSubPrimitiveRenderer::CSortElement>> (proven) -- MATCH
Label corrections / review:
- L053 stand-ins ?_Push_heap@FableHeap_<addr>@@ (4 rows) -> std::__push_heap<..>; landed __adjust_heap callers (0x73d7c0 etc) describe the algorithms as Dinkumware with long distances and SKeyedPair stand-ins -- retail is STLport (__push_heap/__adjust_heap, int Distance)
Callee names introduced: none
Header debt: none

## L054 -- 4 of 4 matched (4 MATCH)
Model: L054: STLport 4.6 __unguarded_partition<T*, T, Compare>(first, last, pivot, comp) over 8-byte records keyed by an unsigned first dword (85 bytes, /Gr __fastcall, `ret 0Ch`): Hoare partition around the by-value pivot with iter_swap.
Identities:
- 0073de50 std::__unguarded_partition<std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >*, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >> (proven) -- MATCH
- 009b3890 std::__unguarded_partition<std::pair<unsigned long, long >*, std::pair<unsigned long, long >, CKeyPairCompareLess<unsigned long, long >> (proven) -- MATCH
- 009b3900 std::__unguarded_partition<std::pair<unsigned long, unsigned long >*, std::pair<unsigned long, unsigned long >, CKeyPairCompareLess<unsigned long, unsigned long >> (proven) -- MATCH
- 00b85430 std::__unguarded_partition<CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement, std::less<CEngineSubPrimitiveRenderer::CSortElement>> (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L055 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L055: constructors of leaf creature modes whose only own member is a CAnimFilterOf3Groups (129 bytes, thiscall, `ret 4`): the base mode constructor with the CThing reference, the class vtable, then the filter built from three CCharString temporaries (constructed right to left, destroyed after the call); no EH frame (retail is built without exception handling).
Identities:
- 0073e320 NCreatureMode::CModeStandFront::CModeStandFront (proven) -- RELOCATION_MATCH
- 0073e410 NCreatureMode::CModeStandLeft::CModeStandLeft (proven) -- RELOCATION_MATCH
- 0073e500 NCreatureMode::CModeStandRight::CModeStandRight (proven) -- RELOCATION_MATCH
- 00910830 NCreatureMode::CModeSitChairStudious::CModeSitChairStudious (proven) -- RELOCATION_MATCH
Label corrections / review:
- L055 0x73e410 bound ??0CModeCrouch@@QAE@PAUCThing@@@Z stand-in is NCreatureMode::CModeStandLeft::CModeStandLeft(CThing&); CModeStandBase.h is a flat struct (key U, __vftable data member) unusable as the polymorphic base -> declared locally (header debt)
Callee names introduced: `??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z`, `??0CModeStandBase@@QAE@AAVCThing@@@Z`, `??1CAnimFilterOf3Groups@@UAE@XZ`, `??1CModeSitBase@NCreatureMode@@UAE@XZ`, `??_GCModeSitChairStudious@NCreatureMode@@UAEPAXI@Z`, `??_GCModeStandFront@NCreatureMode@@UAEPAXI@Z`, `??_GCModeStandLeft@NCreatureMode@@UAEPAXI@Z`, `??_GCModeStandRight@NCreatureMode@@UAEPAXI@Z`
Header debt: none

## L056 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L056: STLport 4.6 _Rb_tree<CCharString, pair<const CCharString, V>, ...>::insert_unique(iterator position, const value_type& v) (668 bytes, thiscall, `ret 0Ch`) -- the landed hinted-insert model 0x00516AF0 (family H006) with four more value types.
Identities:
- 00756790 std::_Rb_tree<CCharString, pair<const CCharString, std::pair<long, long>>, ...>::insert_unique(iterator, const value_type&) (proven) -- RELOCATION_MATCH
- 007ae870 std::_Rb_tree<CCharString, pair<const CCharString, std::vector<CCharString>>, ...>::insert_unique(iterator, const value_type&) (proven) -- RELOCATION_MATCH
- 009abdb0 std::_Rb_tree<CCharString, pair<const CCharString, CCharString>, ...>::insert_unique(iterator, const value_type&) (proven) -- RELOCATION_MATCH
- 00a2def0 std::_Rb_tree<CCharString, pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>, ...>::insert_unique(iterator, const value_type&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L056 bindings ?insert_unique@?$map@... (4 rows) are stand-ins naming map members; real symbol is _Rb_tree::insert_unique(iterator, const value_type&); 0xa2def0 binding says vector<CLipSyncEntry> but the map value is CArray<CMeshDataBank::CLipSyncEntry> (Ego_r.pdb LipSyncAnimations)
Callee names introduced: `?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@0@Z`, `?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V1@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V1@@2@0@Z`, `?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@2@0@Z`, `?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@2@0@Z`, `?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@2@@Z`, `?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V1@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V1@@2@@Z`, `?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$CArray@VCLipSyncEntry@CMeshDataBank@@@@@2@@Z`, `?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@2@@Z`
Header debt: none

## L057 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L057: STLport 4.6 _Rb_tree<K, pair<const K, V>, ...>::equal_range(const key_type& k) (42 bytes, thiscall, `ret 8`): pair<iterator, iterator>(lower_bound(k), upper_bound(k)) -- the out-of-line const _M_upper_bound then _M_lower_bound (arguments evaluated right to left), stored through the hidden result pointer.
Identities:
- 00756b60 std::_Rb_tree<CCharString, pair<const CCharString, std::pair<long, long>>, ...>::equal_range (probable) -- RELOCATION_MATCH
- 007adbc0 std::_Rb_tree<CCharString, pair<const CCharString, bool>, ...>::equal_range (proven) -- RELOCATION_MATCH
- 008aca20 std::_Rb_tree<CCharString, pair<const CCharString, std::vector<CCharString>>, ...>::equal_range (proven) -- RELOCATION_MATCH
- 009906b0 std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, pair<const CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>, ...>::equal_range (proven) -- RELOCATION_MATCH
Callee names introduced: `?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z`, `?_M_upper_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@U?$pair@JJ@std@@@std@@@3@@std@@QBEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z`
Header debt: none

## L058 -- 4 of 4 matched (4 MATCH)
Model: L058: STLport 4.6 __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) over trivially-assigned records of 40..52 bytes (68 bytes, __fastcall, `ret 0Ch`): n by the magic divide, each assignment a `rep movsd` block copy.
Identities:
- 00758290 std::__copy<std::pair<EBodyLocation, CWoundMorphs::CEntry>*, std::pair<EBodyLocation, CWoundMorphs::CEntry>*, int> (probable) -- MATCH
- 008193f0 std::__copy<CUnprovenRecord_008193f0*, CUnprovenRecord_008193f0*, int> (descriptive) -- MATCH
- 009f6120 std::__copy<CInputEvent*, CInputEvent*, int> (probable) -- MATCH
- 009f6170 std::__copy<const CInputEvent*, CInputEvent*, int> (probable) -- MATCH
Callee names introduced: none
Header debt: none

## L059 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L059: STLport 4.6 vector<T>::_M_fill_insert(pos, n, x) for 12-byte trivially-destroyed class elements (311 bytes, thiscall, `ret 0Ch`) -- family L004's model (__x_copy in the frame, out-of-line __uninitialized_copy / uninitialized_fill_n / __copy_backward / fill, _M_insert_overflow for the grow arm) with 12-byte records.
Identities:
- 0075bb00 std::vector<C3DVector>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 0081ce50 std::vector<std::pair<unsigned long, CSkeletalMorphs::CEntry>>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 0086ceb0 std::vector<std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>>::_M_fill_insert (proven) -- RELOCATION_MATCH
- 0086fd20 std::vector<std::pair<EExpressionType, CExpressionSet::CEntry>>::_M_fill_insert (probable) -- RELOCATION_MATCH
Label corrections / review:
- L059 landed helpers around 0x81ce50 / 0x86fd20 name the elements pair<ulong,CSoundPair>/SPayload8 and pair<long,CLongMapValue8> (stand-ins); real CSkeletalMorphs::CEntry / CExpressionSet::CEntry
Callee names introduced: `??$__copy_backward@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@PAU12@H@std@@YIPAU?$pair@KVCEntry@CSkeletalMorphs@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@PAU12@H@std@@YIPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `?_M_insert_overflow@?$vector@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@V?$allocator@U?$pair@KVCEntry@CSkeletalMorphs@@@std@@@2@@std@@IAEXPAU?$pair@KVCEntry@CSkeletalMorphs@@@2@ABU32@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@2@@std@@IAEXPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@2@ABU32@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$allocator@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@@2@@std@@IAEXPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@2@ABU32@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@IAEXPAVC3DVector@@ABV3@ABU__false_type@2@I_N@Z`
Header debt: none

## L060 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L060: CThingSearchTools::GetAllThingsInRoughArea<T, std::vector<T*>, CFilter_Bind2<F1, F2> > (602 bytes, thiscall, `ret 0xc`): walk every thing map in the area, every mapwho layer and cell, stamp each mapwho with the search id, keep the things both filters accept, then clear the AreaMaps scratch vector. Twin: landed 0x00746560 (Bind3 over const CThing).
Identities:
- 0075bc40 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_CanCollideWithThing> > (proven) -- RELOCATION_MATCH
- 007b6300 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind2<CThingFilter_HasDefGlobalIndex, CThingFilter_In2DRange> > (proven) -- RELOCATION_MATCH
- 0091c9c0 CThingSearchTools::GetAllThingsInRoughArea<const CThing, std::vector<const CThing*>, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot> > (proven) -- RELOCATION_MATCH
- 00971530 CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CFilter_Bind2<CThingFilter_CanBeUsedAsCover, CThingFilter_MakesLineWithPosInDirectionOfLine> > (proven) -- RELOCATION_MATCH
Label corrections / review:
- L060: CThingSearchTools::GetAllThingsInRoughArea<T, std::vector<T*>, CFilter_Bind2<F1,F2> > x4 (0x0075BC40 IsNot+CanCollideWithThing, 0x007B6300 HasDefGlobalIndex+In2DRange, 0x0091C9C0 const CThing IsNot+IsNot, 0x00971530 CanBeUsedAsCover+MakesLineWithPosInDirectionOfLine); each instantiation is in the FableWin symbol set. Label corrections: 0x0049C6F0 is ?GetAllThingMapsInArea@CThingSearchTools@@IBE... and 0x0049C700 ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE... (Ego_r.pdb; bindings carry the stand-in CThingSearchTools_Calls and invented GetMapsInArea/GetCellExtents spellings); 0x0049C370 is std::vector<const CThing*>::_M_insert_overflow, bound with a spelling that drops the allocator argument (?$vector@PBVCThing@@@std@@) -- this pass uses the STLport 4.6 protected IAE spelling with allocator; 0x00436AC0 carries both QAE and IAE std spellings -- settle one. Layer table 0x0137DAD8 is an array of NThingMap::CLayerInfo (PDB), still spelled ?LayerInfo_0137dad8@@3PAUCLayerInfo@@A. The landed twins (0x00746560, 0x007F9C00, 0x009793D0, ...) still use the CThingSearchTools_Calls / RetailVector / Filters stand-ins.
Callee names introduced: `??RCThingFilter_CanCollideWithThing@@QBE_NABVCThing@@@Z`, `??RCThingFilter_MakesLineWithPosInDirectionOfLine@@QBE_NABVCThing@@@Z`, `?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z`, `?GetAllThingMapsInArea@CThingSearchTools@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@NThingMap@@V?$allocator@PAVCThingMap@NThingMap@@@std@@@std@@@Z`
Header debt: 0075bc40: CThingSearchTools as engine/CThingSearchTools.h (0x18, retail STLport vector AreaMaps at +0xc), modelled locally (same model for 007b6300, 0091c9c0, 00971530)

## L061 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L061: STLport 4.6 sort(first, last, comp) with __final_insertion_sort inline (117/120 bytes, /Gr __fastcall, `ret 4`): __introsort_loop with depth 2 * __lg(n), then __insertion_sort over the first 16 and __unguarded_insertion_sort_aux over the rest (or __insertion_sort over all when n <= 16).
Identities:
- 00778540 std::sort<long*> (proven) -- RELOCATION_MATCH
- 00a6c080 std::sort<std::pair<unsigned long, unsigned long>*> (probable) -- RELOCATION_MATCH
- 00ae7dc0 std::sort<std::pair<long, unsigned long>*> (probable) -- RELOCATION_MATCH
- 00b868c0 std::sort<CEngineSubPrimitiveRenderer::CSortElement*> (proven) -- RELOCATION_MATCH
Label corrections / review:
- L061 0xa6c080 bound stand-in ?SortPoolSizes@@YIXPAUSPoolSize@@0@Z is std::sort<pair<ulong,ulong>*> (2-arg); landed CTextureManager::Initialise uses SPoolSize stand-in
Callee names introduced: `??$__insertion_sort@PAJU?$less@J@std@@@std@@YIXPAJ0U?$less@J@0@@Z`, `??$__insertion_sort@PAU?$pair@JK@std@@U?$less@U?$pair@JK@std@@@2@@std@@YIXPAU?$pair@JK@0@0U?$less@U?$pair@JK@std@@@0@@Z`, `??$__insertion_sort@PAU?$pair@KK@std@@U?$less@U?$pair@KK@std@@@2@@std@@YIXPAU?$pair@KK@0@0U?$less@U?$pair@KK@std@@@0@@Z`, `??$__introsort_loop@PAU?$pair@JK@std@@U12@HU?$less@U?$pair@JK@std@@@2@@std@@YIXPAU?$pair@JK@0@00HU?$less@U?$pair@JK@std@@@0@@Z`, `??$__introsort_loop@PAU?$pair@KK@std@@U12@HU?$less@U?$pair@KK@std@@@2@@std@@YIXPAU?$pair@KK@0@00HU?$less@U?$pair@KK@std@@@0@@Z`, `??$__unguarded_insertion_sort_aux@PAJJU?$less@J@std@@@std@@YIXPAJ00U?$less@J@0@@Z`, `??$__unguarded_insertion_sort_aux@PAU?$pair@JK@std@@U12@U?$less@U?$pair@JK@std@@@2@@std@@YIXPAU?$pair@JK@0@00U?$less@U?$pair@JK@std@@@0@@Z`, `??$__unguarded_insertion_sort_aux@PAU?$pair@KK@std@@U12@U?$less@U?$pair@KK@std@@@2@@std@@YIXPAU?$pair@KK@0@00U?$less@U?$pair@KK@std@@@0@@Z`, `??$__unguarded_insertion_sort_aux@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@std@@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@00U?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z`
Header debt: none

## L062 -- 3 of 4 matched (3 RELOCATION_MATCH)
Model: L062: 11-byte `mov dword ptr [global], imm32; ret` bodies. Two are ordinary external functions in their modules' code (called directly by Init_RegisterGameComponents 0x004EE23F, so not compiler thunks despite the propagated `$E2` label); two are genuine compiler thunks in the .text$yc / .text$yd groups at the end of .text.
Identities:
- 007899c0 ResetUnprovenFloat_007899c0() (descriptive) -- RELOCATION_MATCH
- 007fffa0 ResetActionSpeedMultiplierOverride_007fffa0() (descriptive) -- RELOCATION_MATCH
- 01229a10 `dynamic atexit destructor for 's_NoGroupAnimFilter_01382438'' ($E2) (probable) -- RELOCATION_MATCH
- 01223cb0 `dynamic initializer for 'g_UnprovenFloat_013bc3ac'' ($E) (descriptive) -- DIFFER 12v11
Parked 01223cb0: Grade: DIFFER(12v11) -- 12v11 compiler dynamic initializer (.text$yc) storing 5.0f into the .bss float 0x013BC3AC; global and initializer unrecovered; VC7.1 RTM folds every constant float initializer tried to static data, an invented initializer object reproduces the bytes
Label corrections / review:
- L062: label corrections -- 0x007899C0 and 0x007FFFA0 are NOT compiler `$E2` thunks (the label is shape-propagated): they are ordinary external void() functions at the head of the CTCHeroRangedAttacks / CTCHaste code, called directly by Init_RegisterGameComponents 0x004EE23F (0x004EE272/0x004EE277) among its run of per-module init calls (0x00749D60, 0x007330F0, 0x006CBA00 are empty `ret` siblings, 0x0075C0E0 / 0x00802370 store bytes) -- names unrecovered, landed descriptive. 0x01229A10 is a real $E2: the atexit destructor of a CAnimFilterNoGroup static 0x01382438 (registrant $E1 0x0121ED80, CRT$XCU entry 0x0137458C; the getter 0x00739E50 is a GetAnimFilter override); landed with Leaf `_$E2` -- confirm at landing that the verifier/finish accept a local `$E` leaf and a backtick line 1. 0x01223CB0 ($E in .text$yc, 5.0f -> .bss 0x013BC3AC) parked.
Callee names introduced: `??RCAnimFilterNoGroup@@UBE_NABVCAnimationEntry@@@Z`, `??_GCAnimFilterBase@@UAEPAXI@Z`, `??_GCAnimFilterNoGroup@@UAEPAXI@Z`, `?Clone@CAnimFilterNoGroup@@UBEPAVCAnimFilterBase@@XZ`, `?g_UnprovenFloat_013bae04@@3MA`
Header debt: none

## L063 -- 0 of 4 matched (none)
Model: L063: a spell / bullet-time component's StopSound(unsigned long sound) (71 bytes, thiscall, `ret 4`): when the parent thing has the sound-player interface (UsingComponents bit 16), find it in the parent's Components map and stop the attached sound.
Identities:
- 0078ec80 CTCDivineWrath::StopSound (proven) -- DIFFER 69v71
- 00794180 CTCUnholyPower::StopSound (proven) -- DIFFER 69v71
- 0079cee0 CTCBulletTime::StopSound (proven) -- DIFFER 69v71
- 0079ddb0 CTCPhysicalShieldSpell::StopSound (proven) -- DIFFER 69v71
Parked 0078ec80: Grade: DIFFER(69v71) -- the component pointer is loaded straight into ecx after the argument push; retail loads it into eax before the push and then moves it (mov eax,[eax+4] ; push ecx ; mov ecx,eax) -- tried bool/unsigned HasTC, key copy + constant compare (matched), named map pointer (matched), named component/thing locals, static_cast from CTCBase, __forceinline
Parked 00794180: Grade: DIFFER(69v71) -- the component pointer is loaded straight into ecx after the argument push; retail loads it into eax before the push and then moves it (mov eax,[eax+4] ; push ecx ; mov ecx,eax) -- tried bool/unsigned HasTC, key copy + constant compare (matched), named map pointer (matched), named component/thing locals, static_cast from CTCBase, __forceinline
Parked 0079cee0: Grade: DIFFER(69v71) -- the component pointer is loaded straight into ecx after the argument push; retail loads it into eax before the push and then moves it (mov eax,[eax+4] ; push ecx ; mov ecx,eax) -- tried bool/unsigned HasTC, key copy + constant compare (matched), named map pointer (matched), named component/thing locals, static_cast from CTCBase, __forceinline
Parked 0079ddb0: Grade: DIFFER(69v71) -- the component pointer is loaded straight into ecx after the argument push; retail loads it into eax before the push and then moves it (mov eax,[eax+4] ; push ecx ; mov ecx,eax) -- tried bool/unsigned HasTC, key copy + constant compare (matched), named map pointer (matched), named component/thing locals, static_cast from CTCBase, __forceinline
Callee names introduced: none
Header debt: none

## L064 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L064: STLport 4.6 _Rb_tree<CCharString, pair<const CCharString, V>, ...>::_M_insert(x, y, v, w) (250 bytes, thiscall, `ret 14h`) -- the landed _M_insert model 0x00756150 with mapped types whose copy is out of line.
Identities:
- 007ae1d0 std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString>>, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 009ab2f0 std::_Rb_tree<CCharString, pair<const CCharString, CCharString>, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 009d40b0 std::_Rb_tree<CCharString, pair<const CCharString, vector<unsigned long>>, ...>::_M_insert (proven) -- RELOCATION_MATCH
- 00a2dc90 std::_Rb_tree<CCharString, pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>, ...>::_M_insert (proven) -- RELOCATION_MATCH
Callee names introduced: `??0?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@ABV01@@Z`, `??0?$vector@VCLipSyncEntry@CMeshDataBank@@V?$allocator@VCLipSyncEntry@CMeshDataBank@@@std@@@std@@QAE@ABV01@@Z`
Header debt: none

## L065 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L065: STLport 4.6 vector<T>::_M_insert_overflow(pos, x, __false_type, fill_len, atend) for vector-of-vector elements (203 bytes, thiscall, `ret 14h`): as family L003 but with the head copy, the fill and the tail copy all out of line (__uninitialized_copy / __uninitialized_fill_n, same-TU noinline bodies) and _M_clear out of line.
Identities:
- 007dc9a0 std::vector<std::vector<CCard*>>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00a75010 std::vector<CArray<C2DLineF>>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00afe6f0 std::vector<CArray<long>>::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 00f3f890 std::vector<CArray<unsigned short>>::_M_insert_overflow (proven) -- RELOCATION_MATCH
Callee names introduced: `??0?$vector@GV?$allocator@G@std@@@std@@QAE@ABV01@@Z`, `??0?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@QAE@ABV01@@Z`, `??0?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@QAE@ABV01@@Z`, `?_M_clear@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@V?$CArray@J@@V?$allocator@V?$CArray@J@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@V?$CArray@VC2DLineF@@@@V?$allocator@V?$CArray@VC2DLineF@@@@@std@@@std@@IAEXXZ`, `?_M_clear@?$vector@V?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@V?$allocator@V?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@@2@@std@@IAEXXZ`
Header debt: none

## L066 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L066: CTCActionUse<X>::OnActionUse(CThing& user) (90 bytes, thiscall, `ret 4`): run the base OnActionUse, then, when the user is a live creature, hand it a stack-built creature action (the 0xB0-byte action temporary: base action + CIntelligentPointer<CThing> target) via SetCurrentAction, destroying the temporary afterwards.
Identities:
- 007eaf50 CTCActionUseTorch::OnActionUse (proven) -- RELOCATION_MATCH
- 007eafb0 CTCActionUseSign::OnActionUse (proven) -- RELOCATION_MATCH
- 007eb570 CTCActionUseMapTable::OnActionUse (proven) -- RELOCATION_MATCH
- 007ee0d0 CTCActionUseSwitch::OnActionUse (proven) -- RELOCATION_MATCH
Callee names introduced: `??0CCreatureAction_PlayerInteractionViewQuests@@QAE@AAVCThingCreatureBase@@PAVCThing@@@Z`, `??0CCreatureAction_Talk@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z`, `??0CCreatureAction_UseLamp@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z`, `??0CCreatureAction_UseSwitch@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z`
Header debt: none

## L067 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L067: STLport 4.6 forwarders that pass the 52-byte CThingCompare_BestForTargeting2 by value (111 bytes, /Gr __fastcall, `ret 34h`): __unguarded_insertion_sort(first, last, comp) -> __unguarded_insertion_sort_aux(first, last, (T*)0, comp) and pop_heap(first, last, comp) -> __pop_heap_aux(first, last, (T*)0, comp); the comparator's user copy constructor copies it member by member into the outgoing argument area.
Identities:
- 007fa610 std::__unguarded_insertion_sort<CThing**, CThingCompare_BestForTargeting2> (proven) -- RELOCATION_MATCH
- 007fa910 std::pop_heap<CThing**, CThingCompare_BestForTargeting2> (probable) -- RELOCATION_MATCH
- 0080b130 std::__unguarded_insertion_sort<NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2> (proven) -- RELOCATION_MATCH
- 0080b890 std::pop_heap<NPlayerTargeting::CTargetPoint*, CThingCompare_BestForTargeting2> (probable) -- RELOCATION_MATCH
Callee names introduced: `??$__pop_heap_aux@PAPAVCThing@@PAV1@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@00VCThingCompare_BestForTargeting2@@@Z`, `??$__pop_heap_aux@PAVCTargetPoint@NPlayerTargeting@@V12@VCThingCompare_BestForTargeting2@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@00VCThingCompare_BestForTargeting2@@@Z`, `??$__unguarded_insertion_sort_aux@PAPAVCThing@@PAV1@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@00VCThingCompare_BestForTargeting2@@@Z`, `??$__unguarded_insertion_sort_aux@PAVCTargetPoint@NPlayerTargeting@@V12@VCThingCompare_BestForTargeting2@@@std@@YIXPAVCTargetPoint@NPlayerTargeting@@00VCThingCompare_BestForTargeting2@@@Z`
Header debt: none

## L068 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L068: STLport 4.6 vector<T*>::_M_range_insert<It>(pos, first, last, const forward_iterator_tag&) for pointer / 4-byte POD elements (496 bytes, thiscall, `ret 10h`) -- the landed byte-vector model 0x009B3630 with 4-byte elements (every copy a memmove).
Identities:
- 00806f30 std::vector<CUnprovenPointee_00806f30*>::_M_range_insert<CUnprovenPointee_00806f30* const*> (descriptive) -- RELOCATION_MATCH
- 008f28a0 std::vector<CCard*>::_M_range_insert<CCard**> (proven) -- RELOCATION_MATCH
- 009b4240 std::vector<unsigned long>::_M_range_insert<const unsigned long*> (probable) -- RELOCATION_MATCH
- 009b4430 std::vector<CInstantiatedDefInfo*>::_M_range_insert<CInstantiatedDefInfo* const*> (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L069 -- 4 of 4 matched (4 MATCH)
Model: L069: CVectorMap<K, V, CKeyPairCompareLess<K, V> >::LowerBound(const K&) -- the STLport-iterator binary search (83 bytes, thiscall `ret 4`), one entry stride per instance (24, 40, 36, 24 bytes). Model: the landed CVectorMap<unsigned long, CSmallVector<...> >::LowerBound 0x006FAF80 with a signed (enum / long) key.
Identities:
- 00819d50 CVectorMap<EHeroMorphType, CTextureMorphEntry, CKeyPairCompareLess<...> >::LowerBound(const EHeroMorphType&) const (proven) -- MATCH
- 00819e60 CVectorMap<EHeroMorphType, CParticleMorphs::CEntry, CKeyPairCompareLess<...> >::LowerBound(const EHeroMorphType&) const (proven) -- MATCH
- 00992290 CVectorMap<long, CUnprovenValue_00992290, CKeyPairCompareLess<...> >::LowerBound(const long&) const (descriptive) -- MATCH
- 00af60c0 CVectorMap<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance, CKeyPairCompareLess<...> >::LowerBound(const NParticleEngine::EPSCType&) (proven) -- MATCH
Label corrections / review:
- L069 0x00992290: the landed caller 0x00991840 (and 0x00992590) bind ?LowerBound@?$CVectorMap@JPAVCASoundBank@@@@QBEPBUSEntry@1@ABJ@Z, but the entry stride is 36 bytes (value 32 bytes, first dword returned) -- the map's value type is not CASoundBank*; rebind when the owner of the global 0x013B8394 map is proven. 0x00819E60's caller 0x0081B160 binds the stand-in ?LowerBound_CVectorMap_00819e60@CVectorMap_00819ec0@@...; 0x00AF60C0's callers bind NParticleEngine::CComponentMap::LowerBound (stand-in) -- the PDB-backed symbols are in L069_ids.tsv.
Callee names introduced: none
Header debt: none

## L070 -- 4 of 4 matched (4 MATCH)
Model: L070: STLport 4.6 __uninitialized_copy(first, last, result, const __false_type&) for 12-byte pair<K, V> elements (53 bytes, /Gr `__fastcall`, `ret 8`): pair's user copy constructor under the null-testing placement new.
Identities:
- 0081a330 std::__uninitialized_copy<pair<unsigned long, CSkeletalMorphs::CEntry>*, pair<unsigned long, CSkeletalMorphs::CEntry>*> (proven) -- MATCH
- 0086ab80 std::__uninitialized_copy<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*> (proven) -- MATCH
- 0086f290 std::__uninitialized_copy<pair<EExpressionType, CExpressionSet::CEntry>*, pair<EExpressionType, CExpressionSet::CEntry>*> (probable) -- MATCH
- 009b2550 std::__uninitialized_copy<pair<unsigned long, CDefClassBase::CSubDefInfo>*, pair<unsigned long, CDefClassBase::CSubDefInfo>*> (proven) -- MATCH
Label corrections / review:
- L070 0x009B2550: reloc_bindings carries two spellings, both from the landed 0x009B5310 (__uninitialized_copy with PAU and PBU first arguments); retail calls the one address for both the in-place moves and the range copy, so only the (pair*, pair*) instantiation exists and 0x009B5310's forward iterator is non-const -- drop the PBU row and retype 0x009B5310's first/last.
Callee names introduced: none
Header debt: none

## L071 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L071: STLport 4.6 sort_heap(first, last, CKeyPairCompareLess<K, V>) for 12-byte pair<K, V> elements (179 bytes, /Gr `__fastcall`, `ret 4`): pop_heap / __pop_heap_aux / __pop_heap inline around the out-of-line __adjust_heap.
Identities:
- 0081c7d0 std::sort_heap<pair<unsigned long, CSkeletalMorphs::CEntry>*, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry> > (proven) -- RELOCATION_MATCH
- 0086c890 std::sort_heap<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (proven) -- RELOCATION_MATCH
- 0086fb40 std::sort_heap<pair<EExpressionType, CExpressionSet::CEntry>*, CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> > (proven) -- RELOCATION_MATCH
- 009b5af0 std::sort_heap<pair<unsigned long, CDefClassBase::CSubDefInfo>*, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > (probable) -- RELOCATION_MATCH
Label corrections / review:
- L071 0x0081AC20 (__adjust_heap callee of 0x0081C7D0) is bound to the stand-in ?__adjust_heap@std@@YIXPAU?$pair@KUCTextureMorph@@@1@HHU21@UCKeyPairCompareLess@1@@Z (non-template, wrong element: the instantiation is pair<unsigned long, CSkeletalMorphs::CEntry>); rebind to the template spelling 0x0081C7D0's object declares.
Callee names introduced: `??$__adjust_heap@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@HU12@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@std@@YIXPAU?$pair@KVCEntry@CSkeletalMorphs@@@0@HHU10@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@Z`, `??$__adjust_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@std@@HU12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@std@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@HHU10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z`, `??$__adjust_heap@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@HU12@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@std@@YIXPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@0@HHU10@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@Z`
Header debt: none

## L072 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L072: STLport 4.6 vector<pair<K, V> >::_M_insert_overflow(pos, x, __false_type, fill_len, atend) for 12-byte pairs (274 bytes, thiscall, `ret 14h`): family L003's model with a trivially-destroyed element -- head copy and fill inline, the tail __uninitialized_copy out of line (family L070), _M_clear's destroy loop empty.
Identities:
- 0081ca90 std::vector<std::pair<unsigned long, CSkeletalMorphs::CEntry> >::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 0086cb30 std::vector<std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::_M_insert_overflow (proven) -- RELOCATION_MATCH
- 0086fc00 std::vector<std::pair<EExpressionType, CExpressionSet::CEntry> >::_M_insert_overflow (probable) -- RELOCATION_MATCH
- 009b60d0 std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >::_M_insert_overflow (proven) -- RELOCATION_MATCH
Label corrections / review:
- L072 0x0086CB30 / 0x0086FC00: bound as public (QAE), allocator-less spellings (and 0x0086FC00 as the stand-in pair<long, CLongMapValue8>); retail STLport's _M_insert_overflow is protected (IAE) with the allocator argument -- rebind to the L072 objects' symbols.
Callee names introduced: none
Header debt: none

## L073 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L073: CVectorMap<K, V, CKeyPairCompareLess<K, V> >::Insert(const K& key, const V& value) for 12-byte pairs (114 bytes, thiscall, `ret 8`): `Data.push_back(pair(key, value)); Dirty = true;` -- STLport push_back inline around the out-of-line _M_insert_overflow (family L072).
Identities:
- 0081d8a0 CVectorMap<unsigned long, CSkeletalMorphs::CEntry, CKeyPairCompareLess<...> >::Insert(const unsigned long&, const CSkeletalMorphs::CEntry&) (proven) -- RELOCATION_MATCH
- 0086d6c0 CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<...> >::Insert(const unsigned long&, const CRandomAppearanceMorph::CTextureMorph&) (proven) -- RELOCATION_MATCH
- 0086ffc0 CVectorMap<EExpressionType, CExpressionSet::CEntry, CKeyPairCompareLess<...> >::Insert(const EExpressionType&, const CExpressionSet::CEntry&) (probable) -- RELOCATION_MATCH
- 009b7930 CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<...> >::Insert(const unsigned long&, const CDefClassBase::CSubDefInfo&) (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L074 -- 4 of 4 matched (4 MATCH)
Model: L074: STLport 4.6 __uninitialized_fill_n(first, n, x, const __false_type&) for 12-byte pair<K, CCountedPointer<T> > (52 bytes, /Gr `__fastcall`, `ret 8`): placement-copy x n times; CCountedPointer's copy constructor bumps the shared CCPPointerInfo's RefCount.
Identities:
- 00827f00 std::__uninitialized_fill_n<pair<long, CCountedPointer<CRegionConnectionNode> >*, unsigned int, pair<...> > (proven) -- MATCH
- 00a135c0 std::__uninitialized_fill_n<pair<long, CCountedPointer<CContainmentVolume> >*, unsigned int, pair<...> > (proven) -- MATCH
- 00a7f960 std::__uninitialized_fill_n<pair<long, CCountedPointer<CNavConnectorNode> >*, unsigned int, pair<...> > (proven) -- MATCH
- 00b8ee20 std::__uninitialized_fill_n<pair<unsigned long, CCountedPointer<CVertexBufferWin32> >*, unsigned int, pair<...> > (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L075 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L075: CCreatureAction_PlayerInteraction{Insult,Threaten,Bribe,Flirt}::PerformAction() (136 bytes, vtable slot 3): when the creature and the target are alive, post message 0x14 (flags 0x10, a per-class last argument 1..4) at the creature's position from the creature to the target -- the landed Greet 0x00845900 tail without its entity events.
Identities:
- 00844260 CCreatureAction_PlayerInteractionInsult::PerformAction (proven) -- RELOCATION_MATCH
- 008442f0 CCreatureAction_PlayerInteractionThreaten::PerformAction (proven) -- RELOCATION_MATCH
- 00844380 CCreatureAction_PlayerInteractionBribe::PerformAction (proven) -- RELOCATION_MATCH
- 00844410 CCreatureAction_PlayerInteractionFlirt::PerformAction (proven) -- RELOCATION_MATCH
Label corrections / review:
- L075 0x00844260..0x00844410: callee spellings follow the PDB (GetPItem QBE, CCreatureActionBase::GetWorld IAE returning CWorld&, AddMessage(EMessageEventType, ...)); reloc_bindings carries many stand-ins for 0x00692940 / 0x00687540 / 0x00A01B50.
Callee names introduced: `?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z`, `?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ`
Header debt: none

## L076 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L076: two-argument combat-action constructors (95 bytes, thiscall `ret 8`): CCombatActionBase(creature, target, code, CSoundPair()), this class's vtable, then SetCombatAnimation(CCharString(<animation name>, -1)) through a scoped local -- the landed 0x00908000 shape with the Ego_r.pdb reference signatures.
Identities:
- 00855c40 CCreatureAction_BlockCounter::CCreatureAction_BlockCounter(CThingCreatureBase&, CThingCreatureBase&) (proven) -- RELOCATION_MATCH
- 008bc0c0 CCreatureAction_CastWillHarvestSoulSpell::CCreatureAction_CastWillHarvestSoulSpell(CThingCreatureBase&, CThing&) (proven) -- RELOCATION_MATCH
- 008ce3f0 CCreatureAction_BuildUpBattleCharge::CCreatureAction_BuildUpBattleCharge(CThingCreatureBase&, CThing&) (proven) -- RELOCATION_MATCH
- 0094e5f0 CCreatureAction_TrollGroundPound::CCreatureAction_TrollGroundPound(CThingCreatureBase&, CThingCreatureBase&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L076 0x0094E5F0: reloc_bindings names it ??0CCombatAction_StabThing@@QAE@PAVCThingCreatureBase@@PAVCThing@@@Z, but it stores CCreatureAction_TrollGroundPound's vtable 0x012931CC -- it is ??0CCreatureAction_TrollGroundPound@@QAE@AAVCThingCreatureBase@@0@Z (Ego_r.pdb); the landed 0x00908000-style twins use pointer parameters where the PDB has references.
Callee names introduced: `??1CCreatureAction_BlockCounter@@UAE@XZ`, `??1CCreatureAction_BuildUpBattleCharge@@UAE@XZ`, `??1CCreatureAction_CastWillHarvestSoulSpell@@UAE@XZ`, `??1CCreatureAction_TrollGroundPound@@UAE@XZ`, `??_GCCreatureAction_BlockCounter@@UAEPAXI@Z`, `??_GCCreatureAction_BuildUpBattleCharge@@UAEPAXI@Z`, `??_GCCreatureAction_CastWillHarvestSoulSpell@@UAEPAXI@Z`, `??_GCCreatureAction_TrollGroundPound@@UAEPAXI@Z`
Header debt: none

## L077 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L077: CVillageTask{Torch,Horn,PickCrops,CarryCrate}::RemoveThingFromTask(const CThing& thing, bool& removed) (158 bytes, vtable slot 6, `ret 8`): if the task's object is the thing, clear it and report true; else clear the assigned creature and report false (each clear is `member = 0` through a CIntelligentPointer temporary).
Identities:
- 008605b0 CVillageTaskTorch::RemoveThingFromTask (proven) -- RELOCATION_MATCH
- 008607a0 CVillageTaskHorn::RemoveThingFromTask (proven) -- RELOCATION_MATCH
- 00860c60 CVillageTaskPickCrops::RemoveThingFromTask (proven) -- RELOCATION_MATCH
- 00861030 CVillageTaskCarryCrate::RemoveThingFromTask (proven) -- RELOCATION_MATCH
Callee names introduced: `??_G?$CIntelligentPointer@$$CBVCThing@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThingObject@@@@UAEPAXI@Z`
Header debt: none

## L078 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L078: STLport 4.6 vector<T>::resize(size_type n, const T& x) for 12-byte class elements (87 bytes, thiscall `ret 8`): `n < size()` erases the tail (erase's __copy_ptrs -> the out-of-line __copy, trivially-destroyed elements leave no destroy loop), else insert(end(), n - size(), x) -> the out-of-line _M_fill_insert.
Identities:
- 0086d900 std::vector<std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>>::resize(unsigned int, const std::pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>&) (proven) -- RELOCATION_MATCH
- 00870040 std::vector<std::pair<EExpressionType, CExpressionSet::CEntry>>::resize(unsigned int, const std::pair<EExpressionType, CExpressionSet::CEntry>&) (proven) -- RELOCATION_MATCH
- 00889440 std::vector<CBrainBehaviour>::resize(unsigned int, const CBrainBehaviour&) (probable) -- RELOCATION_MATCH
- 009d4010 std::vector<CBankFile::CRuntimeData>::resize(unsigned int, const CBankFile::CRuntimeData&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L078 0x00889440: bound only as the stand-in ?resize@CRetailVector_CBrainBehaviour@@QAEXKABVCBrainBehaviour@@@Z from the landed 0x00887330 (whose comment calls the vector Dinkumware); it is STLport ?resize@?$vector@VCBrainBehaviour@@V?$allocator@VCBrainBehaviour@@@std@@@std@@QAEXIABVCBrainBehaviour@@@Z. 0x0086D900 is bound twice (with and without the allocator); keep the allocator spelling.
Callee names introduced: `?_M_fill_insert@?$vector@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$allocator@U?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@@2@@std@@IAEXPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@2@IABU32@@Z`, `?_M_fill_insert@?$vector@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$allocator@U?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@@2@@std@@IAEXPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@2@IABU32@@Z`, `?_M_fill_insert@?$vector@VCBrainBehaviour@@V?$allocator@VCBrainBehaviour@@@std@@@std@@IAEXPAVCBrainBehaviour@@IABV3@@Z`, `?_M_fill_insert@?$vector@VCRuntimeData@CBankFile@@V?$allocator@VCRuntimeData@CBankFile@@@std@@@std@@IAEXPAVCRuntimeData@CBankFile@@IABV34@@Z`
Header debt: none

## L079 -- 4 of 4 matched (4 MATCH)
Model: L079: STLport 4.6 __uninitialized_copy(first, last, result, const __false_type&) for 12-byte classes with an implicit (bitwise) copy constructor (56 bytes, /Gr `__fastcall`, `ret 8`): the three-dword block copy through ebx/esi/edi under the placement null test.
Identities:
- 00885ca0 std::__uninitialized_copy<CBrainBehaviour*, CBrainBehaviour*> (probable) -- MATCH
- 008f67c0 std::__uninitialized_copy<CHeightMapCell*, CHeightMapCell*> (probable) -- MATCH
- 00ada7e0 std::__uninitialized_copy<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*> (proven) -- MATCH
- 00ae4ea0 std::__uninitialized_copy<CUnprovenRecord12_00ae4ea0*, CUnprovenRecord12_00ae4ea0*> (descriptive) -- MATCH
Label corrections / review:
- L079 0x00ADA7E0: the landed reserve 0x00ADB960 binds it with a const first argument (??$__uninitialized_copy@PBVCBlockInfo@...: its model casts to const_pointer), but retail's grow insert 0x00ADBD50 calls the same address with _M_start / position (non-const) -- one instantiation, so it is the (CBlockInfo*, CBlockInfo*) one and STLport 4.6 reserve passes plain pointers; the landed (const_pointer) reserve models (also the 0x009B5310 range insert, L070) need their callee spelling corrected.
- L079 0x008F67C0 / 0x008F6A90: the catalog label CTCSmokeGenerator::CGenerationPos (20 bytes) on 0x008F6A90 is propagated; its caller is CHeightMap::Resize and the element is the 12-byte CHeightMapCell.
Callee names introduced: none
Header debt: none

## L080 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L080: CGameScriptInterface::IsPlayerHolding*Button() const (38 bytes, vtable slots 65-68): ask the main player's CGamePlayerInterface whether a game-action event (button state 2) for the player's joystick is queued.
Identities:
- 008905b0 CGameScriptInterface::IsPlayerHoldingUnsheatheRangedWeaponButton (proven) -- RELOCATION_MATCH
- 008905e0 CGameScriptInterface::IsPlayerHoldingLockTargetButton (proven) -- RELOCATION_MATCH
- 00890610 CGameScriptInterface::IsPlayerHoldingFireRangedWeaponButton (proven) -- RELOCATION_MATCH
- 00890640 CGameScriptInterface::IsPlayerHoldingFirstPersonTargetingButton (proven) -- RELOCATION_MATCH
Label corrections / review:
- L080/L085/L086: CPlayerManager::GetMainPlayer overloads were swapped in this batch and are now fixed: 0x00449960 is the const QBE one (calls the const GetPlayer 0x00449910) and 0x00449970 the non-const QAE one (calls GetPlayer 0x004498C0) -- Ego_r.pdb donor order GetPlayer, GetPlayer const, GetMainPlayer const, GetMainPlayer matches retail 0x4498C0/0x449910/0x449960/0x449970. reloc_bindings carries both wrong pairings (e.g. 0x00702810 QBEPAV at 0x449960 is right in const-ness, 0x007E78E0 QAEPAV at 0x449970 right, 0x007B0950 `CMainPlayer` at 0x449960 wrong); found by vland RELOC_CONFLICT against landed 0x0069AD10.
Callee names introduced: `?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z`
Header debt: none

## L081 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L081: CGameScriptInterface::MsgIs{Level,Region}{Loaded,Unloaded}(const CCharString& name) const (135 bytes, vtable slots 16/17/20/21): resolve the map / region number from the name (0 -> false), then look for the most recent message of type 0x24..0x27 that occurred in this frame window and carries that number.
Identities:
- 008931b0 CGameScriptInterface::MsgIsLevelLoaded (proven) -- RELOCATION_MATCH
- 00893240 CGameScriptInterface::MsgIsLevelUnloaded (proven) -- RELOCATION_MATCH
- 008932d0 CGameScriptInterface::MsgIsRegionLoaded (proven) -- RELOCATION_MATCH
- 00893360 CGameScriptInterface::MsgIsRegionUnloaded (proven) -- RELOCATION_MATCH
Label corrections / review:
- L081 0x008931B0..0x00893360: the catalog's Bind3 label for FindMostRecentMessage 0x008ABE70 (IsOfType, HasExtraData, OccuredBetween) has the operand order wrong -- the frame shows IsOfType, OccuredBetween, HasExtraData (the other Ego_r.pdb instantiation). GetMaxWorldFrameForMessages 0x006E7530 is spelled with the PDB's protected IBE here; landed callers bind QBE.
Callee names introduced: `??$FindMostRecentMessage@V?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_OccuredBetween@@VCMessageEventFilter_HasExtraData@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABV?$CFilter_Bind3@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_OccuredBetween@@VCMessageEventFilter_HasExtraData@@@@@Z`, `?GetMapNumberFromScriptName@CWorldMap@@QBEJABVCCharString@@@Z`, `?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ`
Header debt: none

## L082 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L082: CGameScriptInterface::MsgOnQuest{Completed,Failed,CompletedBeforeScreenShown,FailedBeforeScreenShown}(const CCharString& name) const (154 bytes, vtable slots 25/27/28/29): the most recent message of type 0x33..0x36 since the previous frame, then whether its quest (CQuestManager::GetRegisteredQuestFromIndex(extra data)) is the named one.
Identities:
- 00893570 CGameScriptInterface::MsgOnQuestCompleted (proven) -- RELOCATION_MATCH
- 00893690 CGameScriptInterface::MsgOnQuestFailed (proven) -- RELOCATION_MATCH
- 00893730 CGameScriptInterface::MsgOnQuestCompletedBeforeScreenShown (proven) -- RELOCATION_MATCH
- 008937d0 CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown (proven) -- RELOCATION_MATCH
Callee names introduced: `?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ`, `?GetRegisteredQuestFromIndex@CQuestManager@@QBEABVCCharString@@J@Z`
Header debt: none

## L083 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L083: CGameScriptInterface bool-setters on a script thing's component (86 bytes, `ret 8`): if the script thing is valid and its thing has the component interface, store the bool into one of the component's flags (SetTrophyAsMountable, EntitySetDeathContainerAsEnabled, EntitySetAsForSale, SetBedAvailability) -- the landed DisableFishingSpot 0x00895950 shape with a value argument.
Identities:
- 00896790 CGameScriptInterface::SetTrophyAsMountable (proven) -- RELOCATION_MATCH
- 0089e880 CGameScriptInterface::EntitySetDeathContainerAsEnabled (proven) -- RELOCATION_MATCH
- 008a00e0 CGameScriptInterface::EntitySetAsForSale (proven) -- RELOCATION_MATCH
- 008a1bf0 CGameScriptInterface::SetBedAvailability (proven) -- RELOCATION_MATCH
Label corrections / review:
- L083: callee 0x0040F020 is spelled with its Ego_r.pdb template (CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&), STLport pointer iterator); reloc_bindings carries only stand-ins for it (CVectorMapLong, CInterfaceMap_Retail, ...).
Callee names introduced: none
Header debt: none

## L084 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L084: CGameScriptInterface::EntitySetOpinion{Reaction,Deed}Mask(const CScriptThing& thing, long | const CCharString& mask) const (77 bytes, `ret 8`): when the script thing's thing exists and has a CTCCreatureOpinionOfHero (interface 0x4F), forward the mask to the component's setter.
Identities:
- 0089c1f0 CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing&, long) const (proven) -- RELOCATION_MATCH
- 0089c240 CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const (proven) -- RELOCATION_MATCH
- 0089c290 CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing&, long) const (proven) -- RELOCATION_MATCH
- 0089c2e0 CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L085 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L085: CGameScriptInterface::UpdateOnlineScore_<Activity>(float score) const (113 bytes, vtable slots 725-728): fetch the main player's controlled creature's CTCHeroOnlineScoreboard (interface 0x10D; the lookup result is not tested) and call UpdateScoreForActivity(activity, score, false).
Identities:
- 008a1040 CGameScriptInterface::UpdateOnlineScore_Archery (proven) -- RELOCATION_MATCH
- 008a10c0 CGameScriptInterface::UpdateOnlineScore_ChickenKick (proven) -- RELOCATION_MATCH
- 008a1140 CGameScriptInterface::UpdateOnlineScore_ChapelOrTemple (proven) -- RELOCATION_MATCH
- 008a11c0 CGameScriptInterface::UpdateOnlineScore_FishingCompo (proven) -- RELOCATION_MATCH
Label corrections / review:
- L085: UpdateOnlineScore_* ignore GetInterface's result and call through the uninitialised scoreboard local on a miss (retail behaviour, reproduced as written).
- L080/L085/L086: CPlayerManager::GetMainPlayer overloads were swapped in this batch and are now fixed: 0x00449960 is the const QBE one (calls the const GetPlayer 0x00449910) and 0x00449970 the non-const QAE one (calls GetPlayer 0x004498C0) -- Ego_r.pdb donor order GetPlayer, GetPlayer const, GetMainPlayer const, GetMainPlayer matches retail 0x4498C0/0x449910/0x449960/0x449970. reloc_bindings carries both wrong pairings (e.g. 0x00702810 QBEPAV at 0x449960 is right in const-ness, 0x007E78E0 QAEPAV at 0x449970 right, 0x007B0950 `CMainPlayer` at 0x449960 wrong); found by vland RELOC_CONFLICT against landed 0x0069AD10.
- L085: CPlayer::GetPControlledCreature() const is 0x00487DC0 and PeekPControlledCreature() const 0x00487DD0 (byte-identical `add ecx,2Ch; jmp CIntelligentPointer get`; Ego_r.pdb donor order 0x493650 GetP / 0x493660 Peek). Landed 0x00896EE0 binds GetPControlledCreature to 0x00487DD0, which also carries PeekPControlledCreature -- correct the landed caller.
Callee names introduced: `?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z`
Header debt: none

## L086 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L086: CGameScriptInterface::GetBestScore<Game>() const (94 bytes, vtable slots 732-735): the main player's live controlled creature's CTCHeroStats best score for a tavern game, truncated to long (__ftol2); 0 when there is none.
Identities:
- 008a1650 CGameScriptInterface::GetBestScoreBlackjack (proven) -- RELOCATION_MATCH
- 008a16b0 CGameScriptInterface::GetBestScoreCoinGolfOakVale (proven) -- RELOCATION_MATCH
- 008a1710 CGameScriptInterface::GetBestScoreCoinGolfSnowSpire (proven) -- RELOCATION_MATCH
- 008a1770 CGameScriptInterface::GetBestScoreShoveHaPenny (proven) -- RELOCATION_MATCH
Label corrections / review:
- L080/L085/L086: CPlayerManager::GetMainPlayer overloads were swapped in this batch and are now fixed: 0x00449960 is the const QBE one (calls the const GetPlayer 0x00449910) and 0x00449970 the non-const QAE one (calls GetPlayer 0x004498C0) -- Ego_r.pdb donor order GetPlayer, GetPlayer const, GetMainPlayer const, GetMainPlayer matches retail 0x4498C0/0x449910/0x449960/0x449970. reloc_bindings carries both wrong pairings (e.g. 0x00702810 QBEPAV at 0x449960 is right in const-ness, 0x007E78E0 QAEPAV at 0x449970 right, 0x007B0950 `CMainPlayer` at 0x449960 wrong); found by vland RELOC_CONFLICT against landed 0x0069AD10.
Callee names introduced: none
Header debt: none

## L087 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L087: CGameScriptInterface::Get{Action,TargetName,TableName,SeatName}(CScriptThing thing) (201 bytes, vtable slots 797/803/836/839, `ret 10h`): an empty string for a null script thing, else a copy of one CCharString of the thing's CTCAIScratchpad (interface 0xAA; the lookup result is not tested); the by-value CScriptThing is destroyed on exit.
Identities:
- 008a42c0 CGameScriptInterface::GetAction(CScriptThing) (proven) -- RELOCATION_MATCH
- 008a4840 CGameScriptInterface::GetTargetName(CScriptThing) (proven) -- RELOCATION_MATCH
- 008a6680 CGameScriptInterface::GetTableName(CScriptThing) (proven) -- RELOCATION_MATCH
- 008a6950 CGameScriptInterface::GetSeatName(CScriptThing) (proven) -- RELOCATION_MATCH
Callee names introduced: `??_GCScriptThing@@UAEPAXI@Z`, `?v001@CScriptThing@@UAEXXZ`, `?v002@CScriptThing@@UAEXXZ`, `?v003@CScriptThing@@UAEXXZ`, `?v004@CScriptThing@@UAEXXZ`, `?v005@CScriptThing@@UAEXXZ`, `?v006@CScriptThing@@UAEXXZ`, `?v007@CScriptThing@@UAEXXZ`, `?v008@CScriptThing@@UAEXXZ`, `?v009@CScriptThing@@UAEXXZ`, `?v010@CScriptThing@@UAEXXZ`
Header debt: none

## L088 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L088: implicit copy-assignment operators of 16/20/12/12-byte records ending in a CCountedPointer (80 bytes, `ret 4`): the leading base / member's out-of-line operator=, then CCountedPointer's inline operator= (release the old block, adopt, add-ref).
Identities:
- 008abd10 CScriptGameResourceObjectScriptedThingBase::operator= (proven) -- RELOCATION_MATCH
- 00a2cf50 CMeshBankStateBlock::CLODStateBlock::operator= (proven) -- RELOCATION_MATCH
- 00a32960 C3DMeshVolume::operator= (proven) -- RELOCATION_MATCH
- 00cba700 std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >::operator= (proven) -- RELOCATION_MATCH
Label corrections / review:
- L088: the four copy-assignment operators are compiler-generated in retail; the sources spell the memberwise body out (a user operator= with the same body) so the TU emits the symbol -- for std::pair this adds an operator= to the local STLport pair model (STLport has none; the implicit one is identical). 0x00A2CF50 is CMeshBankStateBlock::CLODStateBlock::operator= (Ego_r.pdb), bound only as the stand-in CResourceRef_00a2d6c0 by the landed CMeshBankStateBlock::Set.
Callee names introduced: `??4?$map@VCCharString@@JU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@QAEAAV01@ABV01@@Z`
Header debt: none

## L089 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L089: combat-action Initialise() overrides (51 bytes, vtable slot 12): set the combat animation through a scoped CCharString, then the CCombatActionBase::Initialise() base call.
Identities:
- 008b6550 CCreatureAction_LeapBack::Initialise (proven) -- RELOCATION_MATCH
- 008f2d40 CCreatureAction_DragonFireBlastLeft::Initialise (proven) -- RELOCATION_MATCH
- 00970280 CCreatureAction_SummonerLightningIdle::Initialise (proven) -- RELOCATION_MATCH
- 00970320 CCreatureAction_SummonerCharge::Initialise (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L090 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L090: three-argument forwarding constructors (37 bytes, `ret 0Ch`): the base constructor with the same three arguments, then the class's vtable.
Identities:
- 008b80e0 CCreatureAction_PlayCombatAnimationWithLooking::CCreatureAction_PlayCombatAnimationWithLooking(CThingCreatureBase& creature, const CCharString& animation, bool flag) (proven) -- RELOCATION_MATCH
- 00b81ea0 CEngineClothSimulationStaticMesh::CEngineClothSimulationStaticMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveStaticMesh* primitive) (proven) -- RELOCATION_MATCH
- 00b81ed0 CEngineClothSimulationAnimatedMesh::CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive) (proven) -- RELOCATION_MATCH
- 00b81f00 CEngineClothSimulation2DAnimatedMesh::CEngineClothSimulation2DAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L090 0x00B81ED0: reloc_bindings spells the index parameter `unsigned int` (I); Ego_r.pdb has `unsigned long` (K) for all three cloth-simulation constructors and the MeshBase one (0x00B81AD0, unbound). GOTCHA: a derived constructor whose (inline) base constructor is inlined leaves only the most-derived vtable store (VC7.1 drops the dead intermediate store) and calls the grand-base constructor directly (0x00B81F00).
Callee names introduced: `??0CEngineClothSimulationMeshBase@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveMeshBase@@@Z`, `??1CCreatureAction_PlayCombatAnimationWithLooking@@UAE@XZ`, `??1CEngineClothSimulation2DAnimatedMesh@@UAE@XZ`, `??1CEngineClothSimulationAnimatedMesh@@UAE@XZ`, `??1CEngineClothSimulationStaticMesh@@UAE@XZ`, `??_GCCreatureAction_PlayCombatAnimationWithLooking@@UAEPAXI@Z`, `??_GCEngineClothSimulation2DAnimatedMesh@@UAEPAXI@Z`, `??_GCEngineClothSimulationAnimatedMesh@@UAEPAXI@Z`, `??_GCEngineClothSimulationStaticMesh@@UAEPAXI@Z`
Header debt: none

## L091 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L091: two-argument combat-action constructors with one member (100 bytes, `ret 8`): CCombatActionBase(creature, code, CSoundPair()), the vtable, the member from the second argument, then SetCombatAnimation(CCharString(name)).
Identities:
- 008bbf90 CCreatureAction_CastWillGhostSwordSpell::CCreatureAction_CastWillGhostSwordSpell(CThingCreatureBase&, long) (proven) -- RELOCATION_MATCH
- 008bc660 CCreatureAction_CastWillSummonSpell::CCreatureAction_CastWillSummonSpell(CThingCreatureBase&, long) (proven) -- RELOCATION_MATCH
- 008bd040 CCreatureAction_TeleportIn::CCreatureAction_TeleportIn(CThingCreatureBase&, bool) (proven) -- RELOCATION_MATCH
- 00925920 CCreatureAction_KrakenRoarLoop::CCreatureAction_KrakenRoarLoop(CThingCreatureBase&, long) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L091 0x00925920: reloc_bindings names it ??0CCreatureAction_KrakenRoarLoop@@QAE@AAVCThingCreatureBase@@J@Z (global); RTTI (.?AVCCreatureAction_KrakenRoarLoop@?A0x29922355@@) and Ego_r.pdb put the class in an anonymous namespace.
Callee names introduced: `??1CCreatureAction_CastWillGhostSwordSpell@@UAE@XZ`, `??1CCreatureAction_CastWillSummonSpell@@UAE@XZ`, `??1CCreatureAction_KrakenRoarLoop@?A0x5257ad35@@UAE@XZ`, `??1CCreatureAction_TeleportIn@@UAE@XZ`, `??_GCCreatureAction_CastWillGhostSwordSpell@@UAEPAXI@Z`, `??_GCCreatureAction_CastWillSummonSpell@@UAEPAXI@Z`, `??_GCCreatureAction_KrakenRoarLoop@?A0x5257ad35@@UAEPAXI@Z`, `??_GCCreatureAction_TeleportIn@@UAEPAXI@Z`
Header debt: none

## L092 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L092: setters taking a CCountedPointer<T> by value (106 bytes, `ret 8`): `Member = p;` (CCountedPointer's inline operator=: release, adopt, add-ref) and the parameter's destructor (release).
Identities:
- 008ceae0 CTCAssassinRush::SetFinalAction(CCountedPointer<CCreatureActionBase>) (proven) -- RELOCATION_MATCH
- 00a240e0 CMeshDataBank::SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool>) (proven) -- RELOCATION_MATCH
- 00a241e0 CMeshDataBank::SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool>) (descriptive) -- RELOCATION_MATCH
- 00b3e820 CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L092: 0x00B3E820 is CBankFileAsync::SetLoadingMemoryPool (Ego_r.pdb), bound only as the stand-in ?SetMemoryPool@CStaticMapBankFile@@; 0x008CEAE0 is bound with the stand-in parameter UCCountedAction (Ego_r.pdb: CCountedPointer<CCreatureActionBase>); the catalog labels all four CMeshDataBank pool setters 0x00A24050..0x00A241E0 "SetAnimationMemoryPool" (only 0x00A24050 is; 0x00A240E0 SetClothMemoryPool, 0x00A24150 the MeshData one, 0x00A241E0 the Xbox one with a second unread argument, `ret 0Ch`).
Callee names introduced: none
Header debt: 008ceae0: Ego_r.pdb FinalAction (PDB +148) at +0x90. (same model for 00a240e0, 00a241e0, 00b3e820)

## L093 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L093: CMaterialDef::GetRandom<Effect>() (85 bytes): a random entry of one of the material's effect-id vectors through the world's local seed (the inlined GFRandom: LCG step, GFROR13, `n ? r % n : 0`), 0 for an empty list.
Identities:
- 008ee5a0 CMaterialDef::GetRandomSlideFootParticleEffect (proven) -- RELOCATION_MATCH
- 008ee600 CMaterialDef::GetRandomSwordHitParticleEffect (proven) -- RELOCATION_MATCH
- 008ee660 CMaterialDef::GetRandomKnockdownHitFloorParticleEffect (proven) -- RELOCATION_MATCH
- 008ee6c0 CMaterialDef::GetRandomWindBlowingDustEffect (proven) -- RELOCATION_MATCH
Label corrections / review:
- L093: 0x0049D800 is CWorld::GetLocalSeed() const (Ego_r.pdb ?GetLocalSeed@CWorld@@QBEPAKXZ, &Component->LocalSeed), bound only as the stand-in CEngineRandomOwner::GetRandomGenerator; the global 0x013B86A0 is the CMainGameComponent (+0x1C PPlayerManager, +0x24 PWorld) -- the landed 0x008EE720 calls it g_pEngine.
Callee names introduced: `?GetLocalSeed@CWorld@@QBEPAKXZ`
Header debt: none

## L094 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L094: default constructors of AI state groups with two intelligent-pointer members (52 bytes): CAIStateGroupBase(), the class's vtable, then each member's CBaseIntelligentPointer() and CIntelligentPointer<T> vtable; no other member is initialised.
Identities:
- 008fec9c CAIStateGroup_MinionReactToCriminal::CAIStateGroup_MinionReactToCriminal() (proven) -- RELOCATION_MATCH
- 0090059c CAIStateGroup_NymphFloatAround::CAIStateGroup_NymphFloatAround() (proven) -- RELOCATION_MATCH
- 00900eec CAIStateGroup_RangedCombatUsePreferredFirePoint::CAIStateGroup_RangedCombatUsePreferredFirePoint() (proven) -- RELOCATION_MATCH
- 009018b8 CAIStateGroup_Thunder::CAIStateGroup_Thunder() (proven) -- RELOCATION_MATCH
Callee names introduced: `??1CAIStateGroup_MinionReactToCriminal@@UAE@XZ`, `??1CAIStateGroup_NymphFloatAround@@UAE@XZ`, `??1CAIStateGroup_RangedCombatUsePreferredFirePoint@@UAE@XZ`, `??1CAIStateGroup_Thunder@@UAE@XZ`, `??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThingBuilding@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThingCreatureBase@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCThingPlayerCreature@@@@UAEPAXI@Z`, `??_GCAIStateGroup_MinionReactToCriminal@@UAEPAXI@Z`, `??_GCAIStateGroup_NymphFloatAround@@UAEPAXI@Z`, `??_GCAIStateGroup_RangedCombatUsePreferredFirePoint@@UAEPAXI@Z`, `??_GCAIStateGroup_Thunder@@UAEPAXI@Z`
Header debt: none

## L095 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L095: CCombatSequence*::OnCreate() queueing the same combat action twice (237 bytes, vtable slot 1): one CCombatAction local, Reset to a new inline-constructed action, PushAction -- twice -- then the base OnCreate and the local's release.
Identities:
- 009066c0 CCombatSequence_BanditAttackMiddle::OnCreate (proven) -- RELOCATION_MATCH
- 00907750 CCombatSequence_TrollDoubleSwipe::OnCreate (proven) -- RELOCATION_MATCH
- 00907840 CCombatSequence_TrollDoubleSwipeNonMelee::OnCreate (proven) -- RELOCATION_MATCH
- 00909cb0 CCombatSequenceWhisperAttackCombo::OnCreate (proven) -- RELOCATION_MATCH
Callee names introduced: `??1CCreatureAction_InterruptableMidAttack@@UAE@XZ`, `??1CCreatureAction_TrollSwipe@@UAE@XZ`, `??_GCCreatureAction_InterruptableMidAttack@@UAEPAXI@Z`, `??_GCCreatureAction_TrollSwipe@@UAEPAXI@Z`
Header debt: none

## L096 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L096: AI state-group village-time predicates (93 bytes, const): the brain's creature's CTCVillageMember (interface 0x23), its village, then one CTCVillage time test (school / work / eat).
Identities:
- 0090fbf0 CAIStateGroup_MinionSitAtWork::IsStateGroupStillValid() const (proven) -- RELOCATION_MATCH
- 00916790 CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInSchoolHours() const (proven) -- RELOCATION_MATCH
- 00940120 CAIStateGroup_VillageTaskPickCrops::IsTimeForWork() const (proven) -- RELOCATION_MATCH
- 009455b0 CAIStateGroup_SitForMeal::IsMealTime() const (proven) -- RELOCATION_MATCH
Label corrections / review:
- L096: label corrections -- 0x00940120 is CAIStateGroup_VillageTaskPickCrops::IsTimeForWork (Ego_r.pdb; bound as ?IsInSchoolHours@CAIStateGroup_WorkBase@@), 0x00916790 is CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInSchoolHours (bound under the stand-in owner CAIStateGroup_StayNearSchool), 0x009455B0 CAIStateGroup_SitForMeal::IsMealTime (bound as IsInMealHours@CAIStateGroup_WorkBase / Query_009455b0), 0x0090FBF0 CAIStateGroup_MinionSitAtWork::IsStateGroupStillValid. CTCVillage time tests by def offsets: 0x00658A35 IsSchoolTime, 0x00658B20 IsWorkTime (catalog: IsCookTime), 0x00658BB9 the eat-time test (no PDB name; Cook would be def +0x74).
Callee names introduced: `?IsEatTime@CTCVillage@@QBE_NXZ`, `?IsSchoolTime@CTCVillage@@QBE_NXZ`, `?IsWorkTime@CTCVillage@@QBE_NXZ`
Header debt: none

## L097 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L097: AI state-group InitStateGroup(const CWorld&, CThingAICreature&) overrides (75 bytes, vtable slot 13): when the creature has a CTCIdleScheduler (interface 0xD3), register this state group with it under one EIdleStateGroup.
Identities:
- 00914bc0 CAIStateGroup_KidWanderNearKids::InitStateGroup (proven) -- RELOCATION_MATCH
- 00938f20 CAIStateGroup_MinionSitOutside::InitStateGroup (proven) -- RELOCATION_MATCH
- 00939130 CAIStateGroup_MinionSitAtHome::InitStateGroup (proven) -- RELOCATION_MATCH
- 009395a0 CAIStateGroup_SitRoundCampfire::InitStateGroup (proven) -- RELOCATION_MATCH
Callee names introduced: `?RegisterIdleStateGroup@CTCIdleScheduler@@QAEXPAVCAIStateGroupBase@@W4EIdleStateGroup@@@Z`
Header debt: none

## L098 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L098: CThingSearchTools::GetPBestLeafThing<TFilter, CThingCompare_Nearest, TThing>(filter, compare, out) const (116 bytes, `ret 0Ch`): the nearest scan-visible thing of TThing's own type list that passes the filter.
Identities:
- 0092c260 CThingSearchTools::GetPBestLeafThing<CThingFilter_In2DRange, CThingCompare_Nearest, CThingBuilding> (proven) -- RELOCATION_MATCH
- 00936100 CThingSearchTools::GetPBestLeafThing<CThingFilter_ObjectHasProperty, CThingCompare_Nearest, CThingObject> (proven) -- RELOCATION_MATCH
- 00941c40 CThingSearchTools::GetPBestLeafThing<CIsFreeHopscotchBoard, CThingCompare_Nearest, CThingObject> (proven) -- RELOCATION_MATCH
- 009823d0 CThingSearchTools::GetPBestLeafThing<CThingFilter_AICreatureHasProperty, CThingCompare_Nearest, CThingAICreature> (proven) -- RELOCATION_MATCH
Label corrections / review:
- L098: callee spellings follow Ego_r.pdb (CThingSearchTools::PeekTypeList(long) const returning const std::list<CClass*>& -- CClass is the PDB's element class name; filter operators take const CThingObject& / const CThingAICreature&); the landed 0x00936070 uses the stand-in CThingSearchToolsMethods.
Callee names introduced: `??RCIsFreeHopscotchBoard@@QBE_NABVCThingObject@@@Z`, `??RCThingFilter_AICreatureHasProperty@@QBE_NABVCThingAICreature@@@Z`, `??RCThingFilter_ObjectHasProperty@@QBE_NABVCThingObject@@@Z`, `?PeekTypeList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@J@Z`
Header debt: none

## L099 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L099: STLport find_if over list<CIntelligentPointer<CThing> > const iterators with a by-value predicate (65 bytes, /Gr `__fastcall`, the iterator returned through the hidden pointer, `ret 0Ch`).
Identities:
- 0092e790 std::find_if<list<CIntelligentPointer<CThing> >::const_iterator, CIsFreeBed> (proven) -- RELOCATION_MATCH
- 00945fd0 std::find_if<list<CIntelligentPointer<CThing> >::const_iterator, CIsCookingPot> (proven) -- RELOCATION_MATCH
- 00946020 std::find_if<list<CIntelligentPointer<CThing> >::const_iterator, CIsSeatWithSpaceOwnedBy> (proven) -- RELOCATION_MATCH
- 00946d80 std::find_if<list<CIntelligentPointer<CThing> >::const_iterator, CIsCandleNeedingChanging> (proven) -- RELOCATION_MATCH
Label corrections / review:
- L099: retail find_if keeps STLport's forward to the inline __find_if(first, last, pred, iterator category) -- the by-value predicate is copied for the inner call and the loop head is aligned; the landed 0x00946AE0 same-TU stand-in body (plain loop) is not the retail shape. 0x00946020 is bound as ?FindIfFreeChair@@ (stand-in); 0x00945FD0 as ?FindIfCookingPot@@ (stand-in).
Callee names introduced: `??RCIsCookingPot@?A0x19cb855a@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z`, `??RCIsFreeBed@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z`, `??RCIsSeatWithSpaceOwnedBy@?A0x6f67a830@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z`
Header debt: none

## L100 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L100: village-task state groups' Cleanup / OnReplace (140 bytes): when the group still holds its task, hand it back to the village (CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>, const CThing&)) and clear PTask.
Identities:
- 0093ea00 CAIStateGroup_VillageTaskTorch::Cleanup (proven) -- RELOCATION_MATCH
- 0093ee40 CAIStateGroup_VillageTaskPatrol::OnReplace (proven) -- RELOCATION_MATCH
- 0093fb30 CAIStateGroup_VillageTaskSecuritySweep::Cleanup (proven) -- RELOCATION_MATCH
- 009408d0 CAIStateGroup_VillageTaskHorn::Cleanup (proven) -- RELOCATION_MATCH
Callee names introduced: `??_G?$CIntelligentPointer@VCVillageTaskBase@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCVillageTaskHorn@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCVillageTaskPatrol@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCVillageTaskSecuritySweep@@@@UAEPAXI@Z`, `??_G?$CIntelligentPointer@VCVillageTaskTorch@@@@UAEPAXI@Z`, `?RemoveThingFromTask@CTCVillage@@QAEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@@Z`
Header debt: none

## L101 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L101: village-task state groups' Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) (96 bytes, vtable slot 1): the CAIStateGroupBase::Initialise base call, then PTCVillageMember = the creature's CTCVillageMember (interface 0x23) through the inlined GetInterface.
Identities:
- 0093ec60 CAIStateGroup_VillageTaskTorch::Initialise (proven) -- RELOCATION_MATCH
- 0093eed0 CAIStateGroup_VillageTaskPatrol::Initialise (proven) -- RELOCATION_MATCH
- 0093fc70 CAIStateGroup_VillageTaskSecuritySweep::Initialise (proven) -- RELOCATION_MATCH
- 00940b10 CAIStateGroup_VillageTaskHorn::Initialise (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L102 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L102: maze state groups' IsStillRunnable() (55 bytes): the maze target is alive and the group is not stopping.
Identities:
- 009542b0 CAIStateGroup_MazeDefensiveMeleeCombat::IsStillRunnable (probable) -- RELOCATION_MATCH
- 009542f0 CAIStateGroup_MazeTutorialBattle::IsStillRunnable (proven) -- RELOCATION_MATCH
- 00954330 CAIStateGroup_MazeTutorialBattleRespondToLightning::IsStillRunnable (probable) -- RELOCATION_MATCH
- 00954370 CAIStateGroup_MazeTutorialBattleUseShield::IsStillRunnable (probable) -- RELOCATION_MATCH
Label corrections / review:
- L102: the catalog labels all four maze IsStillRunnable bodies 0x009542B0..0x00954370 CAIStateGroup_MazeDefensiveMeleeCombat::IsStillRunnable; only 0x009542F0 is caller-proven (CAIStateGroup_MazeTutorialBattle::Update); the RespondToLightning / UseShield assignment of 0x00954330 / 0x00954370 follows the TU's Initialise order (probable).
Callee names introduced: `?IsStopping@CAIStateGroupBase@@IBE_NXZ`
Header debt: none

## L103 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L103: STLport swap(T& a, T& b) for 8-byte pair<CCharString, 4-byte> elements (74 bytes, /Gr `__fastcall`, a in ecx, b in edx): `T tmp = a; a = b; b = tmp;` -- pair's user copy constructor, the implicit assignment, the temporary's CCharString destructor.
Identities:
- 009b1b40 std::swap<pair<CCharString, long> > (probable) -- RELOCATION_MATCH
- 009b2e00 std::swap<pair<CCharString, unsigned long> > (probable) -- RELOCATION_MATCH
- 00a3f750 std::swap<pair<CCharString, CSoundBankLH*> > (probable) -- RELOCATION_MATCH
- 00a3fd60 std::swap<pair<CCharString, CUnprovenPointee_00a3fd60*> > (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L103: the four std::swap<pair<CCharString, dword> > bodies have no callers (kept out-of-line copies); the catalog's swap<C3DSkeletalBoneMorph> label is propagated. Element types are from the neighbouring bound helpers (probable) except 0x00A3FD60 (descriptive).
Callee names introduced: none
Header debt: none

## L104 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L104: STLport __median(a, b, c, CCompareString<T>) for pair<CCharString, T> elements (94 bytes, /Gr `__fastcall`, a in ecx, b in edx, c and the empty comparator on the stack, `ret 8`): the median of three by the CCharString key.
Identities:
- 009e6c20 std::__median<pair<CCharString, CSymbolMap::CSymbolInfo>, CCompareString<CSymbolMap::CSymbolInfo> > (probable) -- RELOCATION_MATCH
- 00a6f5b0 std::__median<pair<CCharString, C3DHelperPoint>, CCompareString<C3DHelperPoint> > (probable) -- RELOCATION_MATCH
- 00a6f690 std::__median<pair<CCharString, C3DDummyObject>, CCompareString<C3DDummyObject> > (probable) -- RELOCATION_MATCH
- 00cba620 std::__median<pair<CCharString, CCountedPointer<CEntityScriptBindingBase>>, CCompareString<CCountedPointer<CEntityScriptBindingBase>> > (proven) -- RELOCATION_MATCH
Label corrections / review:
- L104: 0x009E6530 is CCharString::operator< (Ego_r.pdb ??MCCharString@@QBE_NABV0@@Z); the catalog calls it FastCompare and labels the four __median bodies CMessageEventFilter_CanBeSeenOrHeard::operator() (propagated).
Callee names introduced: none
Header debt: none

## L105 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L105: Sizeof() const of mesh records with two counted members (62 bytes): sizeof(*this) plus each member's pointee Sizeof() and its pointer-object size (or just the pointer-object size when null).
Identities:
- 00a244e0 C3DMeshLODInfo::Sizeof() const (probable) -- RELOCATION_MATCH
- 00a25000 CPhysicsMeshInfo::CMeshData::Sizeof() const (probable) -- RELOCATION_MATCH
- 00a2b1c0 CUnprovenMeshRecord_00a2b1c0::Sizeof() const (descriptive) -- RELOCATION_MATCH
- 00a2b200 CUnprovenMeshRecord_00a2b200::Sizeof() const (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L105: two pairs of byte-identical unreferenced Sizeof bodies (0x00A244E0 / 0x00A2B1C0 with C3DMeshLODInfo's layout, 0x00A25000 / 0x00A2B200 with CPhysicsMeshInfo::CMeshData's); the first of each is given the Ego_r.pdb name (probable), the second a descriptive CUnprovenMeshRecord owner; the catalog labels all four C3DMeshLODInfo::Sizeof.
Callee names introduced: none
Header debt: none

## L106 -- 4 of 4 matched (4 MATCH)
Model: L106: STLport uninitialized_fill_n(first, n, x) for CCountedPointer<T> elements (46 bytes, /Gr `__fastcall`, first in ecx, n in edx, x on the stack, `ret 4`): the public forwarder with __uninitialized_fill_n(..., __false_type) inline -- placement copies bumping the shared count.
Identities:
- 00a2a650 std::uninitialized_fill_n<CCountedPointer<CMeshDataBankEntry>*, unsigned int, CCountedPointer<CMeshDataBankEntry> > (proven) -- MATCH
- 00a959e0 std::uninitialized_fill_n<CCountedPointer<C3DClothPrimitive>*, unsigned int, CCountedPointer<C3DClothPrimitive> > (proven) -- MATCH
- 00b82dc0 std::uninitialized_fill_n<CCountedPointer<CParticleSimulation>*, unsigned int, CCountedPointer<CParticleSimulation> > (probable) -- MATCH
- 00b82e90 std::uninitialized_fill_n<CCountedPointer<CEngineSimulationBase>*, unsigned int, CCountedPointer<CEngineSimulationBase> > (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L107 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L107: STLport fill(first, last, const CCountedPointer<T>& value) (94 bytes, /Gr `__fastcall`, first ecx, last edx, the value on the stack, `ret 4`): CCountedPointer's inline operator= on every element.
Identities:
- 00a2b120 std::fill<CCountedPointer<CMeshDataBankEntry>*, CCountedPointer<CMeshDataBankEntry> > (proven) -- RELOCATION_MATCH
- 00a95b00 std::fill<CCountedPointer<C3DGroup2>*, CCountedPointer<C3DGroup2> > (probable) -- RELOCATION_MATCH
- 00b82e30 std::fill<CCountedPointer<CEngineSimulationBase>*, CCountedPointer<CEngineSimulationBase> > (proven) -- RELOCATION_MATCH
- 00b96760 std::fill<CCountedPointer<CUnprovenPointee_00b97cb0>*, CCountedPointer<CUnprovenPointee_00b97cb0> > (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L107: 0x00A95B00 is bound only as the stand-in ??$fill@UElement@N00a97f70@@ (the landed 0x00A97F70 is N00a97f70::vector<Element>); its element is CCountedPointer<C3DGroup2> by the bound sibling __uninitialized_copy 0x00A95820.
Callee names introduced: none
Header debt: none

## L108 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L108: release of one CCountedPointer member (49/64 bytes, thiscall, plain `ret`): drop the shared count (reloading Info after the decrement), on zero call the info's fastcall deleter on the data and delete the info, then clear Info and Data.  Two named member functions and two unreferenced destructors of {12 bytes, CCountedPointer} records.
Identities:
- 00a45330 C3DSkeletalMorph::NotifyParamUpdate (probable) -- RELOCATION_MATCH
- 00a5f250 CStreamingFontDataBankEntry::Unload (proven) -- RELOCATION_MATCH
- 00a63580 CFontDataBankEntry::Unload (proven) -- RELOCATION_MATCH
- 00b44120 CUnprovenRecord_00b44120::~CUnprovenRecord_00b44120 (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L109 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L109: default constructors of polymorphic 16-byte classes (20 bytes): the vtable, then three zeroed member dwords.
Identities:
- 00a752d0 CContainmentVolume::CContainmentVolume() (proven) -- RELOCATION_MATCH
- 00c34b30 CMember_00c34b30::CMember_00c34b30() (descriptive) -- RELOCATION_MATCH
- 00c34dd0 CMember_00c34dd0::CMember_00c34dd0() (descriptive) -- RELOCATION_MATCH
- 00c572f0 CGroupA_00c3ec80::CGroupA_00c3ec80() (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L109: reloc_bindings binds vtable 0x012B8DF8 as ??_7CDisplayManager@@6B@ (from the landed destructor 0x00C57310), but Fable.exe RTTI gives CDisplayManager the vtable 0x0129B3F8 and Ego_r.pdb CDisplayManager (748 bytes, D3DDevice at +0x58) does not fit an object whose COM pointer is at +8; the landed 0x00C56D30 also calls its base (this constructor, 0x00C572F0) C3DGroup2, which has no vtable in Ego_r.pdb. Both names are label corrections; 0x00C572F0 keeps its descriptive binding CGroupA_00c3ec80.
Callee names introduced: `??1CContainmentVolume@@UAE@XZ`, `??1CGroupA_00c3ec80@@UAE@XZ`, `??1CMember_00c34b30@@UAE@XZ`, `??1CMember_00c34dd0@@UAE@XZ`, `??_GCContainmentVolume@@UAEPAXI@Z`, `??_GCGroupA_00c3ec80@@UAEPAXI@Z`, `??_GCMember_00c34b30@@UAEPAXI@Z`, `??_GCMember_00c34dd0@@UAEPAXI@Z`
Header debt: none

## L110 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L110: CNavQuadTreeNode child setters (57 bytes, thiscall, `ret 4`): delete the old child unless it is the shared blocked node, then store the new one.
Identities:
- 00a75ca0 CNavQuadTreeNode::SetTLNode(CANavQuadTreeNode*) (proven) -- RELOCATION_MATCH
- 00a75ce0 CNavQuadTreeNode::SetTRNode(CANavQuadTreeNode*) (proven) -- RELOCATION_MATCH
- 00a75d20 CNavQuadTreeNode::SetBLNode(CANavQuadTreeNode*) (proven) -- RELOCATION_MATCH
- 00a75d60 CNavQuadTreeNode::SetBRNode(CANavQuadTreeNode*) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L110: 0x013D2D2C is bound under three names (?g_BlockedNavNode@@3VCNavBlockedQuadTreeNode@@A from 0x00A76060, ?g_BlockedNode@@3VCANavQuadTreeNode@@A from 0x00A472F0, ?g_AutoTinyFieldObject@@3UAutoTinyFieldTarget@@A from 0x0122AAD0); the setters use the landed RemoveRegionZero's g_BlockedNavNode. The other two are label corrections.
Callee names introduced: none
Header debt: none

## L111 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L111: STLport 4.6 vector<CCountedPointer<T> >::_M_fill_insert(pos, n, x) (300 bytes, thiscall, `ret 0Ch`) -- family L004's model with an 8-byte CCountedPointer element: the copy constructor (count bump) and the destructor (release, fastcall deleter, delete the info) of __x_copy are inline.
Identities:
- 00a97e40 std::vector<CCountedPointer<C3DClothPrimitive> >::_M_fill_insert (proven) -- RELOCATION_MATCH
- 00b83b00 std::vector<CCountedPointer<CParticleSimulation> >::_M_fill_insert (probable) -- RELOCATION_MATCH
- 00b83e70 std::vector<CCountedPointer<CEngineSimulationBase> >::_M_fill_insert (proven) -- RELOCATION_MATCH
- 00b97e90 std::vector<CCountedPointer<CUnprovenPointee_00b97cb0> >::_M_fill_insert (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L111: _M_fill_insert is public here (QAE, the spelling the bound 0x00A97E40 / 0x00B83E70 use; STLport 4.6 declares it next to insert(pos, n, x)), while family L004 kept the protected IAE spelling -- settle one access for the whole repo at landing. 0x00B83970 is bound as the stand-in CRetailVector<CCountedPointer<CParticleSimulation> >::InsertOverflow and 0x00B82DF0 with the pointee CUnprovenPointee_00b83c30 (it is CEngineSimulationBase by the bound fill insert 0x00B83E70): label corrections.
Callee names introduced: `??$__copy_backward@PAV?$CCountedPointer@VC3DClothPrimitive@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VC3DClothPrimitive@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$CCountedPointer@VCEngineSimulationBase@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCEngineSimulationBase@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `??$__copy_backward@PAV?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z`, `?_M_insert_overflow@?$vector@V?$CCountedPointer@VC3DClothPrimitive@@@@V?$allocator@V?$CCountedPointer@VC3DClothPrimitive@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VC3DClothPrimitive@@@@ABV3@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@V?$CCountedPointer@VCEngineSimulationBase@@@@V?$allocator@V?$CCountedPointer@VCEngineSimulationBase@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VCEngineSimulationBase@@@@ABV3@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@V?$CCountedPointer@VCParticleSimulation@@@@V?$allocator@V?$CCountedPointer@VCParticleSimulation@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VCParticleSimulation@@@@ABV3@ABU__false_type@2@I_N@Z`, `?_M_insert_overflow@?$vector@V?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@V?$allocator@V?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VCUnprovenPointee_00b97cb0@@@@ABV3@ABU__false_type@2@I_N@Z`
Header debt: none

## L112 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L112: C3DAnimFileLoader::Add<Parent>SubChunkType(const CChunkedFileChunk& prototype) (157 bytes, thiscall, hidden return pointer, `ret 8`): wrap a fresh prototype copy (virtual CreateNewInstance) in a CCountedPointer, register it with the parent chunk type's super chunk (virtual GetAsSuperChunk) and return it by value.
Identities:
- 00a98be0 C3DAnimFileLoader::AddRootSubChunkType(const CChunkedFileChunk&) (proven) -- RELOCATION_MATCH
- 00a98c80 C3DAnimFileLoader::AddObjectSubChunkType(const CChunkedFileChunk&) (proven) -- RELOCATION_MATCH
- 00a98d20 C3DAnimFileLoader::AddHelpersSubChunkType(const CChunkedFileChunk&) (proven) -- RELOCATION_MATCH
- 00a98dc0 C3DAnimFileLoader::AddMovementVectorSubChunkType(const CChunkedFileChunk&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L112: 0x00AA87E0 is bound ?RegisterSubChunkType@CChunkedFileSuperChunk@@ (used here) but labelled C3DMeshFileXLoader_AddMeshToQueue / FUN_00aa87e0 in ghidra_out -- label correction.
Callee names introduced: none
Header debt: none

## L113 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L113: chunk presence tests -- C<..>Chunk::Has<X>() const (118 bytes, thiscall, plain `ret`): find the first sub chunk of a four-character type, keep it in a counted pointer and report whether it was found.
Identities:
- 00a99280 C3DAnimFileRootChunk::HasHelpers() const (proven) -- RELOCATION_MATCH
- 00a9b2c0 C3DMeshFileRootChunk::HasHelpers() const (proven) -- RELOCATION_MATCH
- 00abb470 C3DMeshFilePrimitiveChunk::HasSmoothingGroups() const (probable) -- RELOCATION_MATCH
- 00abb610 C3DMeshFilePrimitiveChunk::HasUniqueVertices() const (proven) -- RELOCATION_MATCH
Label corrections / review:
- L113: the manifest labels 0x00A9B2C0 / 0x00ABB470 / 0x00ABB610 C3DAnimFileRootChunk::HasHelpers (propagated); by their tags (HLPR / SMTH / UNIV) and Ego_r.pdb they are C3DMeshFileRootChunk::HasHelpers, C3DMeshFilePrimitiveChunk::HasSmoothingGroups and ::HasUniqueVertices.
Callee names introduced: none
Header debt: none

## L114 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L114: default constructors of small polymorphic classes (17 bytes, thiscall, plain `ret`): the vtable, then two member dwords zeroed.
Identities:
- 00ac1ec0 CIndexBuffer::CIndexBuffer() (proven) -- RELOCATION_MATCH
- 00c39d30 CUnprovenBase_00c39d30::CUnprovenBase_00c39d30() (descriptive) -- RELOCATION_MATCH
- 00c3a520 CUnprovenBase_00c3a520::CUnprovenBase_00c3a520() (descriptive) -- RELOCATION_MATCH
- 00c4cf20 CBase_00c4cf20::CBase_00c4cf20() (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L114: 0x00C39D30 is bound as a free __fastcall ?CThingNoiseDef_baseTransfer@@YIXPAX@Z (landed 0x00C3A700) but is a constructor; 0x00C4CF20 carries four stand-in constructor names (CAIStateGroupBase_00c42d60, CActionBase_00c3dcd0, CActionBase_00c3dde0, CBase_00c4cf20) and the landed 0x00C596A0 calls it CIndexBuffer (CIndexBuffer's RTTI constructor is 0x00AC1EC0); both 0x00C596A0 and 0x00C56D30 are landed as C3DAnimationUtil::C3DAnimationUtil (no RTTI backs either).
Callee names introduced: `??1CBase_00c4cf20@@UAE@XZ`, `??1CIndexBuffer@@UAE@XZ`, `??1CUnprovenBase_00c39d30@@UAE@XZ`, `??1CUnprovenBase_00c3a520@@UAE@XZ`, `??_GCBase_00c4cf20@@UAEPAXI@Z`, `??_GCIndexBuffer@@UAEPAXI@Z`, `??_GCUnprovenBase_00c39d30@@UAEPAXI@Z`, `??_GCUnprovenBase_00c3a520@@UAEPAXI@Z`
Header debt: none

## L115 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L115: engine primitive manager constructors that only add their enable console variable (62 bytes, thiscall, `ret 4`, no EH frame): base constructor on the scene list, the vtable, AddEnableConsoleVariable("Enable...").
Identities:
- 00ba1ac0 CEnginePrimitiveManager2DStaticMeshes::CEnginePrimitiveManager2DStaticMeshes(CArray<CEngineScene*>&) (proven) -- RELOCATION_MATCH
- 00bc5570 CEnginePrimitiveChangerManager::CEnginePrimitiveChangerManager(CArray<CEngineScene*>&) (proven) -- RELOCATION_MATCH
- 00bd1ef0 CEnginePrimitiveManagerAnimatedMeshes::CEnginePrimitiveManagerAnimatedMeshes(CArray<CEngineScene*>&) (proven) -- RELOCATION_MATCH
- 00bd1f30 CEnginePrimitiveManager2DAnimatedMeshes::CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L115: 0x00BACD80 is bound under three names (?RegisterEnable@CEnginePrimitiveManager@@, ?RegisterEnable@CEnginePrimitiveManagerOcclusionBase@@, ?AddEnableConsoleVariable@CEnginePrimitiveManager@@); Ego_r.pdb has only AddEnableConsoleVariable (used here). 0x00B9ECE0 is bound ??0CEnginePrimitiveManagerStaticMeshesBase@@QAE@J@Z (a long parameter); Ego_r.pdb / FableWin.pdb spell it (CArray<CEngineScene*>&) (used here). 0x00BCFF10 (unbound) is ??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z by RTTI base + PDB.
Callee names introduced: `??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z`, `??0CEnginePrimitiveManagerStaticMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z`, `??1CEnginePrimitiveChangerManager@@UAE@XZ`, `??1CEnginePrimitiveManager2DAnimatedMeshes@@UAE@XZ`, `??1CEnginePrimitiveManager2DStaticMeshes@@UAE@XZ`, `??1CEnginePrimitiveManager@@UAE@XZ`, `??1CEnginePrimitiveManagerAnimatedMeshes@@UAE@XZ`, `??_GCEnginePrimitiveChangerManager@@UAEPAXI@Z`, `??_GCEnginePrimitiveManager2DAnimatedMeshes@@UAEPAXI@Z`, `??_GCEnginePrimitiveManager2DStaticMeshes@@UAEPAXI@Z`, `??_GCEnginePrimitiveManagerAnimatedMeshes@@UAEPAXI@Z`
Header debt: none

## L116 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L116: CEngineSubPrimitive<X>(CEngineInternalPrimitive<X>* parent) (34 bytes, thiscall, `ret 4`): CBaseClassNonCopyable's constructor, the inline CEngineSubPrimitiveBase constructor (Parent, NextSubPrimitive cleared), this class's vtable -- family K056's model (landed 0x00BB1780).
Identities:
- 00bace10 CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite*) (proven) -- RELOCATION_MATCH
- 00bace50 CEngineSubPrimitive2DSprite::CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite*) (proven) -- RELOCATION_MATCH
- 00bbbdb0 CEngineSubPrimitiveParticleGroup::CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase*) (proven) -- RELOCATION_MATCH
- 00bcc400 CEngineSubPrimitiveWeaponTrail::CEngineSubPrimitiveWeaponTrail(CEngineInternalPrimitiveWeaponTrail*) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L116: 0x00BBBDB0 is bound ??0CEngineSubPrimitiveParticleGroup@@QAE@AAVCEngineInternalPrimitiveBase@@@Z (reference); Ego_r.pdb spells the parameter as a pointer (PAV, used here).
Callee names introduced: `??_GCEngineSubPrimitive2DSprite@@UAEPAXI@Z`, `??_GCEngineSubPrimitiveBase@@UAEPAXI@Z`, `??_GCEngineSubPrimitiveParticleGroup@@UAEPAXI@Z`, `??_GCEngineSubPrimitiveSprite@@UAEPAXI@Z`, `??_GCEngineSubPrimitiveWeaponTrail@@UAEPAXI@Z`
Header debt: none

## L117 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L117: CPatchTesselationEdgeStrip::LoadVerts<T>(CEdgeStipVertexArray<T>&, CDataInputStream&, CMovableResource*) (402 bytes, thiscall with `this` unused, `ret 0Ch`): read the count, resize the array, then read the range-compressed block and decompress it into the vertices.
Identities:
- 00bfc880 CPatchTesselationEdgeStrip::LoadVerts<CPatchTesselationEdgeStrip::CVertex> (proven) -- RELOCATION_MATCH
- 00bfca20 CPatchTesselationEdgeStrip::LoadVerts<CPatchTesselationEdgeStrip::CFanBaseVertex> (proven) -- RELOCATION_MATCH
- 00bfcbc0 CPatchTesselationEdgeStrip::LoadVerts<CPatchTesselationEdgeStrip::CWaterVertex> (proven) -- RELOCATION_MATCH
- 00bfcd60 CPatchTesselationEdgeStrip::LoadVerts<CPatchTesselationEdgeStrip::CWaterFanBaseVertex> (proven) -- RELOCATION_MATCH
Label corrections / review:
- L117: the four LoadVerts<T> are bound public with pointer parameters (QAE, PAV CEdgeStipVertexArray, CMemoryDataInputStream*, CMovableResourceHandler*); Ego_r.pdb spells them private with references (AAE, AAV CEdgeStipVertexArray<T>&, CDataInputStream&, CMovableResource*) -- used here. 0x00514E20 is bound only under stand-ins (CByteArray / CByteVector / CDataBuffer(unsigned long)); Ego_r.pdb names it ??0?$CArray@E@@QAE@I@Z (used here). LANDED ERROR: 0x00BFD0B0 is landed as SaveVerts<CFanBaseVertex>, but the landed Save 0x00BFC770 calls it for this+0x20 (WaterFanBaseVerts) and its 0x38 stride matches LoadVerts<CWaterFanBaseVertex> 0x00BFCD60 (CFanBaseVertex is 0x10 bytes by LoadVerts<CFanBaseVertex> 0x00BFCA20): it is SaveVerts<CWaterFanBaseVertex>.
Callee names introduced: `??0?$CArray@E@@QAE@I@Z`
Header debt: none

## L118 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L118: slot 0 of small non-RTTI comparator vtables (36 bytes, thiscall with `this` unused, `ret 8`): read an unsigned member of two objects through a constant pointer to a virtual getter (the shared vcall thunk) and return whether they are equal, as an int.
Identities:
- 00c09d20 CUnprovenCompare_00c09d20<CUnprovenObject_00c09d20, unsigned long, &CUnprovenObject_00c09d20::Get>::Equal (descriptive) -- RELOCATION_MATCH
- 00c0a010 CUnprovenCompare_00c0a010<CUnprovenObject_00c0a010, unsigned long, &CUnprovenObject_00c0a010::Get>::Equal (descriptive) -- RELOCATION_MATCH
- 00c24d10 CUnprovenCompare_00c24d10<CUnprovenObject_00c24d10, unsigned long, &CUnprovenObject_00c24d10::Get>::Equal (descriptive) -- RELOCATION_MATCH
- 00c24e70 CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>::Equal (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L119 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L119: slot 1 of the family-L118 comparator vtables (31 bytes, thiscall with `this` unused, `ret 8`): read an unsigned member of two objects through a constant pointer to a virtual getter (the shared vcall thunk) and return whether they are equal, as an int.
Identities:
- 00c09d50 CUnprovenCompare_00c09d20<CUnprovenObject_00c09d20, unsigned long, &CUnprovenObject_00c09d20::Get>::Less (descriptive) -- RELOCATION_MATCH
- 00c0a040 CUnprovenCompare_00c0a010<CUnprovenObject_00c0a010, unsigned long, &CUnprovenObject_00c0a010::Get>::Less (descriptive) -- RELOCATION_MATCH
- 00c24d40 CUnprovenCompare_00c24d10<CUnprovenObject_00c24d10, unsigned long, &CUnprovenObject_00c24d10::Get>::Less (descriptive) -- RELOCATION_MATCH
- 00c24ea0 CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>::Less (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L120 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L120: slot 2 of the family-L118 comparator vtables (27 bytes, thiscall with `this` unused, `ret 8`): read an unsigned member of two objects through a constant pointer to a virtual getter (the shared vcall thunk) and return whether they are equal, as an int.
Identities:
- 00c09d70 CUnprovenCompare_00c09d20<CUnprovenObject_00c09d20, unsigned long, &CUnprovenObject_00c09d20::Get>::EqualToValue (descriptive) -- RELOCATION_MATCH
- 00c0a060 CUnprovenCompare_00c0a010<CUnprovenObject_00c0a010, unsigned long, &CUnprovenObject_00c0a010::Get>::EqualToValue (descriptive) -- RELOCATION_MATCH
- 00c24d60 CUnprovenCompare_00c24d10<CUnprovenObject_00c24d10, unsigned long, &CUnprovenObject_00c24d10::Get>::EqualToValue (descriptive) -- RELOCATION_MATCH
- 00c24ec0 CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>::EqualToValue (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L121 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L121: libpk PKCTRMorphable.cpp forwarders (206 bytes, thiscall, `ret 8`, EH-framed): when the implementation pointer at +4 is null log `.\PKCTRMorphable.cpp(<line>) : Error: NULL implimentation` and raise the trace temporary, then forward both arguments to the implementation's virtual of the same slot.
Identities:
- 00c17e60 CMorphableNode_00c185b0::Slot05(long, long) (descriptive) -- RELOCATION_MATCH
- 00c17f30 CMorphableNode_00c185b0::Slot06(long, long) (descriptive) -- RELOCATION_MATCH
- 00c18000 CMorphableNode_00c185b0::Slot07(long, long) (descriptive) -- RELOCATION_MATCH
- 00c180d0 CMorphableNode_00c185b0::Slot08(long, long) (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L122 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L122: libpk PKCTRMorphable.cpp no-argument forwarders (194 bytes, thiscall, plain `ret`, EH-framed) -- family L121's shape: when the implementation pointer at +4 is null log `.\PKCTRMorphable.cpp(<line>) : Error: NULL implimentation` and raise the trace temporary, then forward the call to the implementation's virtual of the same slot.
Identities:
- 00c18270 CMorphableNode_00c185b0::Slot10() (descriptive) -- RELOCATION_MATCH
- 00c18340 CMorphableNode_00c185b0::Slot11() (descriptive) -- RELOCATION_MATCH
- 00c18410 CMorphableNode_00c185b0::Slot12() (descriptive) -- RELOCATION_MATCH
- 00c187a0 CMorphableNode_00c185b0::Slot01() (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L123 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L123: libpk pointer-array append (39 bytes, thiscall, `ret 4`): grow by one slot when full, store at the count, bump the count -- the inline CPtrArray<T>::Add of the landed CBlendOwner::AddBlend 0x00C1E780, out of line.
Identities:
- 00c1e8d0 CPtrArray<CBlendEntry*>::Add(CBlendEntry*) (descriptive) -- RELOCATION_MATCH
- 00c1ebc0 CPtrArray<CUIComponentBase*>::Add(CUIComponentBase*) (descriptive) -- RELOCATION_MATCH
- 00c35050 CPtrArray<CPlaybackEntry*>::Add(CPlaybackEntry*) (descriptive) -- RELOCATION_MATCH
- 00c42b50 CUnprovenPtrList_00c42b50::Add(CBlendEntry*) (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L123: the landed Remove 0x00C1EBF0 names its element CDisplayElement while the Grow next to it (0x00C1EB30) is bound CPtrArray<CUIComponentBase*>::Grow -- one array, two descriptive element names; 0x00C42B50 calls the same Grow 0x00C1E640 as CPtrArray<CBlendEntry*>::Add 0x00C1E8D0, so it is modelled as a wrapper (CUnprovenPtrList_00c42b50) over that array.
Callee names introduced: `?Grow@?$CPtrArray@PAVCPlaybackEntry@@@@QAEXK@Z`
Header debt: none

## L124 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L124: libpk CItemAction type-byte setters (61 bytes, thiscall, plain `ret`): read the target's packed type, replace one byte with a small value and, when it changed, apply it through SetTypeBits.
Identities:
- 00c1f380 CItemAction::SetVariant2() (descriptive) -- RELOCATION_MATCH
- 00c1f3c0 CItemAction::SetFamily2() (descriptive) -- RELOCATION_MATCH
- 00c1f400 CItemAction::SetVariant1() (descriptive) -- RELOCATION_MATCH
- 00c1f440 CItemAction::SetFamily1() (descriptive) -- RELOCATION_MATCH
Callee names introduced: `?SetTypeBits@CItemAction@@QAEXK@Z`
Header debt: none

## L125 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L125: libpk CSoundEvent property setters (38 bytes, thiscall, `ret 4`): settle the state (ChangeState(-1)), hand the value to the event's parameter block (+0x38) and, when it changed, mark the property dirty in the flags at +0xA8.
Identities:
- 00c20c50 CSoundEvent::SetParam70(float) (descriptive) -- RELOCATION_MATCH
- 00c20c80 CSoundEvent::SetUserGain(float) (descriptive) -- RELOCATION_MATCH
- 00c20e50 CSoundEvent::Set3DPosition(const CUnprovenVector_00c345b0&) (descriptive) -- RELOCATION_MATCH
- 00c20eb0 CSoundEvent::Set2DPosition(float) (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L125: 0x00C20720 carries six bound names (CStateMachineObject::ChangeState, CThemeDisplay_00c21a20::Dispatch, CScriptModuleBase::Init, Object::Row00c21c20::Clear, CSoundEvent::CheckState, CPanelItem::ChangeState); the sweep1-renamed landed CSoundEvent::CountInto 0x00C20F60 spells it CSoundEvent::ChangeState(long) (used here) -- settle one at landing. The L125 setter names come from the event describer 0x00C21040's labels ("user gain(", "2D(", "3D("); 0x00C20C50's field (+0x70) is unlabelled (SetParam70).
Callee names introduced: `?ChangeState@CSoundEvent@@QAEXJ@Z`, `?Set2DPosition@CUnprovenSoundParams_00c344d0@@QAEJM@Z`, `?Set3DPosition@CUnprovenSoundParams_00c344d0@@QAEJABVCUnprovenVector_00c345b0@@@Z`, `?SetFloat38@CUnprovenSoundParams_00c344d0@@QAEJM@Z`, `?SetUserGain@CUnprovenSoundParams_00c344d0@@QAEJM@Z`
Header debt: none

## L126 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L126: libpk two-slot callback objects, slot 1 (27 bytes, thiscall, `ret 4`): when the host's key field matches the held owner's, tail-call the host's per-owner operation with the owner (argument slot reused).
Identities:
- 00c2a580 CUnprovenCallback_00c2a580::Invoke(CResourceWorld*) (descriptive) -- RELOCATION_MATCH
- 00c2a5a0 CUnprovenCallback_00c2a5a0::Invoke(CResourceWorld*) (descriptive) -- RELOCATION_MATCH
- 00c2bab0 CUnprovenCallback_00c2bab0::Invoke(CEntryHost_00c37990*) (descriptive) -- RELOCATION_MATCH
- 00c2bad0 CUnprovenCallback_00c2bad0::Invoke(CEntryHost_00c37b90*) (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L126: the landed CEntryHost_00c37990::CreateEntry 0x00C37990 and CEntryHost_00c37b90::DestroyEntry 0x00C37B90 are inverse members of one host but carry two descriptive class names; their other bindings (?ReleasePredicate@CTCActivationTrigger_00c37da0@@ for 0x00C37B90, ?Release@CResourceMapOwner_00c36590@@ for 0x00C36290) are stand-ins -- the callbacks here use the landed leaf names.
Callee names introduced: `?AllocateFor@CResourceWorld@@QAEXPAX@Z`, `?CreateEntry@CEntryHost_00c37990@@QAEXPAX@Z`, `?DestroyEntry@CEntryHost_00c37b90@@QAEXPAX@Z`, `?FreeAllocation@CResourceWorld@@QAEXPAX@Z`
Header debt: none

## L127 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L127: libpk vector deleting destructors (87 bytes, thiscall, `ret 4`): array form through `eh vector destructor iterator` with the element size, single form through the out-of-line destructor; both free through the class operator delete, which is the audio free 0x00C3C370.
Identities:
- 00c3d8b0 CUnprovenObject_00c3d8b0::`vector deleting destructor' (descriptive) -- RELOCATION_MATCH
- 00c3d910 CUnprovenObject_00c3d910::`vector deleting destructor' (descriptive) -- RELOCATION_MATCH
- 00c3f0b0 CUnprovenObject_00c3f0b0::`vector deleting destructor' (descriptive) -- RELOCATION_MATCH
- 00c3f6b0 CUnprovenObject_00c3f6b0::`vector deleting destructor' (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L127: the four vector deleting destructors' own destructors are landed under propagated names (0x00C3D0D0 and 0x00C3D0E0 both ??1CAIStateGroup_CreatureUseCoverBase@..., 0x00C55130 CLandscapeBackgroundTreeNode -- Ego_r.pdb's is 0xF0 bytes, not 0x248 --, 0x00C3F630 a vector<bool> destructor); the vtables 0x012B7B88 / 0x012B5AB0 / 0x012B8AE0 carry stand-in bindings (CAIStateGroup_Derived_00c44580, C3DAnimationUtil, CLandscapeBackgroundTreeNode_00c555e0). Here each class is CUnprovenObject_<its ??_E address>.
Callee names introduced: `??1CUnprovenObject_00c3d8b0@@UAE@XZ`, `??1CUnprovenObject_00c3d910@@UAE@XZ`, `??1CUnprovenObject_00c3f0b0@@UAE@XZ`, `??1CUnprovenObject_00c3f6b0@@UAE@XZ`
Header debt: none

## L128 -- 4 of 4 matched (4 MATCH)
Model: L128: libpk settings-block dword setters (19 bytes, thiscall, `ret 4`): store a changed value and set its dirty bit in the mask at +0x78.
Identities:
- 00c40860 CUnprovenSettings_00c40860::SetField24(long) (descriptive) -- MATCH
- 00c40880 CUnprovenSettings_00c40860::SetField28(long) (descriptive) -- MATCH
- 00c408a0 CUnprovenSettings_00c40860::SetField2C(long) (descriptive) -- MATCH
- 00c40920 CUnprovenSettings_00c40860::SetField3C(long) (descriptive) -- MATCH
Callee names introduced: none
Header debt: none

## L129 -- 4 of 4 matched (4 RELOCATION_MATCH) [retail_asm]
Model: L129 (retail_asm): libpk millibels -> amplitude (109 bytes, __cdecl, float in st(0)): 0 at or below -10000 mB, else 10^(mB / 2000) through libpk's inline-assembly 2^x block (frndint / fscale / f2xm1 and two `wait`s, which VC7.1 cannot emit from C++). Byte-identical copies of the landed retail_asm 0x00C40EE0 in four more libpk TUs.
Identities:
- 00c42c90 MillibelsToAmplitude_00c42c90(long) (descriptive) -- RELOCATION_MATCH
- 00c53b60 MillibelsToAmplitude_00c53b60(long) (descriptive) -- RELOCATION_MATCH
- 00c54ae0 MillibelsToAmplitude_00c54ae0(long) (descriptive) -- RELOCATION_MATCH
- 00c5c130 MillibelsToAmplitude_00c5c130(long) (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L130 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L130: libpk ambient-sound list walks (72 bytes, thiscall, plain `ret`, EH-framed): build a one-word stack visitor around `this` (out-of-line constructor) and hand it to the list's ForEach (virtual slot 10); the visitor's inline destructor leaves only the EH state.
Identities:
- 00c44e30 CAmbientEmitter::Finish() (descriptive) -- RELOCATION_MATCH
- 00c47210 CAmbientSource::UpdateChannels() (probable) -- RELOCATION_MATCH
- 00c47260 CAmbientSource::UpdateTriggers() (probable) -- RELOCATION_MATCH
- 00c472b0 CAmbientSource::Finish() (probable) -- RELOCATION_MATCH
Label corrections / review:
- L130: 0x00C44E30 (CAmbientEmitter, unbound) is named Finish by analogy with CAmbientSource::Finish 0x00C472B0 (both run when the definition's IsFinished 0x00C384E0 holds); the landed visitor constructors (CVtable_012b7e50 / 7e58 / 7e60 / 7bbc) are called root polymorphic classes, but their destructors store a base vtable (0x012B7E40 / 0x012B7BAC, slot 0 _purecall).
Callee names introduced: `??_GCPKVisitor@@UAEPAXI@Z`, `??_GCVtable_012b7bbc@@UAEPAXI@Z`, `??_GCVtable_012b7e50@@UAEPAXI@Z`, `??_GCVtable_012b7e58@@UAEPAXI@Z`, `??_GCVtable_012b7e60@@UAEPAXI@Z`, `?Visit@CVtable_012b7bbc@@UAEXPAX@Z`, `?Visit@CVtable_012b7e50@@UAEXPAX@Z`, `?Visit@CVtable_012b7e58@@UAEXPAX@Z`, `?Visit@CVtable_012b7e60@@UAEXPAX@Z`
Header debt: none

## L131 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L131: CAudioContext OpenAL wrappers with a (count, ids) signature (65 bytes, thiscall, `ret 8`): resolve the al entry point on first use (alGetProcAddress when +0x104 is set, else the DLL export) and call it -- the landed SourceQueueBuffers 0x00C57690's model.
Identities:
- 00c4d870 CAudioContext::GenBuffers(long count, unsigned long* buffers) (probable) -- RELOCATION_MATCH
- 00c4d8c0 CAudioContext::DeleteBuffers(long count, const unsigned long* buffers) (probable) -- RELOCATION_MATCH
- 00c574b0 CAudioContext::GenSources(long count, unsigned long* sources) (probable) -- RELOCATION_MATCH
- 00c57500 CAudioContext::DeleteSources(long count, const unsigned long* sources) (probable) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L132 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L132: C3DSoundSource pending-parameter block clamped float setters (89 bytes, thiscall, `ret 4`): clamp the value into [lo, hi], store it and raise the field's dirty bit in the block's first word.
Identities:
- 00c53740 CUnprovenPendingParams_00c53740::SetField1C(float) (descriptive) -- RELOCATION_MATCH
- 00c537a0 CUnprovenPendingParams_00c53740::SetField20(float) (descriptive) -- RELOCATION_MATCH
- 00c53980 CUnprovenPendingParams_00c53740::SetField38(float) (descriptive) -- RELOCATION_MATCH
- 00c539e0 CUnprovenPendingParams_00c53740::SetField34(float) (descriptive) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L133 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L133: CAudioContext OpenAL wrappers with a (source, param, value) signature (70 bytes, thiscall, `ret 0Ch`): resolve the al entry point on first use (alGetProcAddress when +0x104 is set, else the DLL export) and call it -- the landed SourceQueueBuffers 0x00C57690's model.
Identities:
- 00c57550 CAudioContext::Sourcei(unsigned long source, long param, long value) (probable) -- RELOCATION_MATCH
- 00c575a0 CAudioContext::GetSourcei(unsigned long source, long param, long* value) (probable) -- RELOCATION_MATCH
- 00c72510 CAudioContext::Sourcef(unsigned long source, long param, float value) (probable) -- RELOCATION_MATCH
- 00c88050 CAudioContext::Sourcefv(unsigned long source, long param, const float* values) (probable) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L134 -- 4 of 4 matched (4 RELOCATION_MATCH) [retail_asm]
Model: L134 (retail_asm): libpk MMX mix kernels (173 bytes, __cdecl, /GS, 16-byte aligned frame): fill an aligned four-word gain vector, then the hand MMX loop accumulating `count` blocks of eight Q15-scaled 16-bit samples into the 32-bit mix buffer. Byte-identical copies of the landed retail_asm ScaleAccumulate16MMX 0x00C6F0F0.
Identities:
- 00c5be20 ScaleAccumulate16MMX_00c5be20(const short* src, unsigned long count, long* dst, short gain) (descriptive) -- RELOCATION_MATCH
- 00c6aa10 MixSamplesMMX(const unsigned char* source, long quadCount, long* accumulator, unsigned short volume) (probable) -- RELOCATION_MATCH
- 00c6f730 ScaleAccumulate16MMX_00c6f730(const short* src, unsigned long count, long* dst, short gain) (descriptive) -- RELOCATION_MATCH
- 00c6fbb0 ScaleAccumulate16MMX_00c6fbb0(const short* src, unsigned long count, long* dst, short gain) (descriptive) -- RELOCATION_MATCH
Label corrections / review:
- L134: 0x00C6AA10 is bound twice: ?MixSamplesMMX@@YAXPBEJPAJG@Z (landed CSoundMixer::MixBlocks 0x00C6ACC0; argument order matches the body, used here) and ?ScaleSamplesByPackedVolume@@YAXPAXJPBXG@Z (landed CSampleScaler_00c6ac60::Scale 0x00C6AC60; destination and source swapped) -- label correction for the latter.
Callee names introduced: none
Header debt: none

## L135 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L135: CEaxSource EAX 4.0 ratio setters without a deferred copy (90 bytes, thiscall, `ret 4`): range-check, on a change store the current value and raise dirty bit 0x400000; returns 0, or -3 with the context's last error -2.
Identities:
- 00c64bb0 CEaxSource::SetOcclusionLFRatio(float) (probable) -- RELOCATION_MATCH
- 00c64c10 CEaxSource::SetOcclusionRoomRatio(float) (probable) -- RELOCATION_MATCH
- 00c64c70 CEaxSource::SetOcclusionDirectRatio(float) (probable) -- RELOCATION_MATCH
- 00c64d10 CEaxSource::SetExclusionLFRatio(float) (probable) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L136 -- 4 of 4 matched (4 MATCH)
Model: L136: libvorbis 1.0.1 static ilog2 (24 bytes, LTCG __fastcall: v in ecx, plain `ret`): the bit length of v - 1 (0 for 0 and 1). One static copy per source file: info.c, block.c, mapping0.c, floor1.c.
Identities:
- 00c8db60 libvorbis::ilog2 (info.c) (proven) -- MATCH
- 00c8f060 libvorbis::ilog2 (block.c) (probable) -- MATCH
- 00c9ec20 libvorbis::ilog2 (mapping0.c) (probable) -- MATCH
- 00ca17a0 libvorbis::ilog2 (floor1.c) (probable) -- MATCH
Label corrections / review:
- L136: four libvorbis static ilog2 copies (info.c 0x00C8DB60, block.c 0x00C8F060, mapping0.c 0x00C9EC20, floor1.c 0x00CA17A0) share the real symbol @ilog2@4 (one name, four addresses, as the source has four statics) -- if landing needs unique names, suffix by file.
Callee names introduced: none
Header debt: none

## L137 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L137: entity-script OnPredicateFail (60 bytes, thiscall, plain `ret`, virtual slot 5): when the entity's own thing was killed (by anyone), decrement the parent quest's counter (GuardInWoodsCount, SummonerCount, FinalSummonersAlive).
Identities:
- 00cfdd60 NScript::CQ_AwakeningTheOracleScript::CFinalSummoner::OnPredicateFail() (proven) -- RELOCATION_MATCH
- 00d86450 NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::OnPredicateFail() (proven) -- RELOCATION_MATCH
- 00d86620 NScript::CQ_HeroSoulsGuildmasterScript::CWaitingGuardInWoods::OnPredicateFail() (proven) -- RELOCATION_MATCH
- 00d884e0 NScript::CQ_HeroSoulsNostroScript::CSummonerToKill::OnPredicateFail() (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L138 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L138: CQ_BanditCampBossBattleScript entity Init (120 bytes, thiscall, plain `ret`, entity vtable slot 2): mark the thing as having no information, put it in a faction, make it unpushable by the hero.
Identities:
- 00d04a60 NScript::CQ_BanditCampBossBattleScript::CAssassin1::Init() (proven) -- RELOCATION_MATCH
- 00d059e0 NScript::CQ_BanditCampBossBattleScript::CAssassin2::Init() (proven) -- RELOCATION_MATCH
- 00d060b0 NScript::CQ_BanditCampBossBattleScript::CAssassin3::Init() (proven) -- RELOCATION_MATCH
- 00d07410 NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Init() (proven) -- RELOCATION_MATCH
Callee names introduced: `??_GCScriptThing@@UAEPAXI@Z`, `?v00@CBaseClass@@UAEXXZ`, `?v00@CScriptThing@@UAEXXZ`
Header debt: 00d04a60: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18 (as the landed entity scripts). (same model for 00d059e0, 00d060b0, 00d07410)

## L139 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L139: entity-script Init that spawns a parent-script thread on the entity's thing (258 bytes, thiscall, plain `ret`, entity vtable slot 2): ParentClass->SpawnFunction(new CSpawnedFunc1<Quest, CScriptThing>("ParentClass." + "<F>", 0, ParentClass, &Quest::<F>, Me)).
Identities:
- 00d16460 NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Init() (proven) -- RELOCATION_MATCH
- 00e0daa0 NScript::CQ_UndeadRisingScript::CUndeadRisingZombieAttacker::Init() (proven) -- RELOCATION_MATCH
- 00f09420 NScript::CQ_AmbushTradersScript::CAT_Phase3Archer::Init() (proven) -- RELOCATION_MATCH
- 00f09530 NScript::CQ_AmbushTradersScript::CAT_Phase3Guard::Init() (proven) -- RELOCATION_MATCH
Label corrections / review:
- L139: 0x00F08590 is bound ?WatchForEnemyDeath@CQ_AmbushTradersScript@(NScript@)@QAEXUCScriptThingRuntime@@@Z (stand-in parameter type); Ego_r.pdb spells ?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z (used here). LANDED ERROR: 0x00D169E0 is landed as CAT_Phase3Archer::Init (comment "WatchForDeputy2Killed"), but it sits in CQ_BountyHuntScript's code and the real CAT_Phase3Archer::Init is 0x00F09420 (the entity vtable stored by CAT_Phase3Archer::Alloc 0x00F04BD0) -- 0x00D169E0 is probably CBH_GangDeputy2::Init.
Callee names introduced: `??1CSpawnedFuncBase@@UAE@XZ`, `??_G?$CSpawnedFunc1@VCQ_AmbushTradersScript@NScript@@VCScriptThing@@@@UAEPAXI@Z`, `??_G?$CSpawnedFunc1@VCQ_BountyHuntScript@NScript@@VCScriptThing@@@@UAEPAXI@Z`, `??_G?$CSpawnedFunc1@VCQ_UndeadRisingScript@NScript@@VCScriptThing@@@@UAEPAXI@Z`, `??_GCScriptThing@@UAEPAXI@Z`, `?LookAfterHealth@CQ_UndeadRisingScript@NScript@@QAEXVCScriptThing@@@Z`, `?Restart@CSpawnedFuncBase@@UAEXXZ`, `?RunSuspendableProcess@CSpawnedFuncBase@@UAEXXZ`, `?SuspendSuspendableProcess@CSpawnedFuncBase@@UAEX_N@Z`, `?WatchForDeputy1Killed@CQ_BountyHuntScript@NScript@@QAEXVCScriptThing@@@Z`, `?WatchForEnemyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z`, `?v00@CBaseClass@@UAEXXZ`, `?v00@CScriptThing@@UAEXXZ`
Header debt: none

## L140 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L140: script threads that wait until every thing with a script name is dead, then set a flag (214 bytes, thiscall, plain `ret`): the landed CV_TempleOfLightScript::WatchForAcolyteExit 0x00EDD060's inline-once vector destructor band.
Identities:
- 00d18b20 NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled() (proven) -- RELOCATION_MATCH
- 00d19af0 NScript::CQ_BountyHuntScript::WatchForDeputy2GruntsKilled() (proven) -- RELOCATION_MATCH
- 00d1c9e0 NScript::CQ_BountyHuntScript::WatchForLeaderGruntsKilled() (proven) -- RELOCATION_MATCH
- 00e2fc50 NScript::CV_AmbushScamScript::WatchForAllAmbushersDead() (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L141 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L141: quest-script OnPersist over four members (131 bytes, thiscall, `ret 4`, script vtable slot 4): save / load each by name through CPersistContext::Transfer<T>(name, value, default).
Identities:
- 00d34720 NScript::CQ_FireHeartScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00e82190 NScript::CV_FisticuffsClubScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00ed1010 NScript::CV_SingingStonesScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00ee5720 NScript::CV_TourGuideScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L142 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L142: STLport vector<T>::_M_allocate_and_copy(n, first, last) for def-class elements (74 bytes, thiscall with `this` unused, `ret 0Ch`): malloc n elements (0 for n == 0) and copy [first, last) into them through the out-of-line /Gr _Construct.
Identities:
- 00d35020 std::vector<CFireHeartPatternDef>::_M_allocate_and_copy<const CFireHeartPatternDef*> (probable) -- RELOCATION_MATCH
- 00f261a0 std::vector<CArenaRoundDef>::_M_allocate_and_copy<const CArenaRoundDef*> (probable) -- RELOCATION_MATCH
- 00f27440 std::vector<CHeroSoulsRoundDef>::_M_allocate_and_copy<const CHeroSoulsRoundDef*> (probable) -- RELOCATION_MATCH
- 00f27d50 N00f27bc0::vector<CHeroSoulsWaveDef>::_M_allocate_and_copy (proven) -- RELOCATION_MATCH
Label corrections / review:
- L142: 0x00F27D50 keeps the stand-in namespace N00f27bc0 of its landed caller's binding (?_M_allocate_and_copy@?$vector@UCHeroSoulsWaveDef@N00f27bc0@@@N00f27bc0@@...); its siblings 0x00D35020 / 0x00F261A0 / 0x00F27440 use the STLport std::vector<T>::_M_allocate_and_copy<const T*> spelling and introduce std::_Construct<T,T> names for the landed descriptive 0x00D34EC0 (Construct_DefWithByteVector_00d34ec0) and the unbound 0x00F25B00 / 0x00F26DB0 -- settle one spelling at landing. Element types by the vector operator= bindings (CFireHeartPatternVector, CArenaRoundVector) and the def vtables; CHeroSoulsRoundDef by adjacency.
Callee names introduced: `??0CArenaRoundDef@@QAE@ABV0@@Z`, `??0CFireHeartPatternDef@@QAE@ABV0@@Z`, `??0CHeroSoulsRoundDef@@QAE@ABV0@@Z`, `??0CHeroSoulsWaveDef@N00f27bc0@@QAE@ABU01@@Z`
Header debt: none

## L143 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L143: quest-script WatchForLeaving threads (114 bytes, thiscall, plain `ret`): while the hero is alive and the mission is neither failed nor succeeded, run script frames (stopping when the thread is terminated); then, unless terminated or succeeded, fail the mission.
Identities:
- 00d64000 NScript::CQ_GuildTrainingWoodsDepartureScript::WatchForLeaving() (proven) -- RELOCATION_MATCH
- 00d68c50 NScript::CQ_GuildTrainingWoodsWillScript::WatchForLeaving() (probable) -- RELOCATION_MATCH
- 00df0180 NScript::CQ_SecretPassageScript::WatchForLeaving() (proven) -- RELOCATION_MATCH
- 00df4f80 NScript::CQ_TentacleKrakenBossFightScript::WatchForLeaving() (proven) -- RELOCATION_MATCH
Label corrections / review:
- L143: the manifest stretches the landed CWillWhisper::Main 0x00D68810 (719 bytes) over CQ_GuildTrainingWoodsWillScript's DoMission 0x00D68AE0, which spawns WatchForLeaving 0x00D68C50 -- catalogue boundary fix.
Callee names introduced: none
Header debt: none

## L144 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L144: quest / entity OnPersist over five members (144 bytes, thiscall, `ret 4`, vtable slot 4).
Identities:
- 00d785e0 NScript::CQ_HeroSoulsScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00d7c800 NScript::CQ_HeroSoulsArenaScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00ebeda0 NScript::CV_OracleScript::CVO_OracleMarker::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
- 00ec4970 NScript::CV_RockTrollFirstEncounterScript::OnPersist(CPersistContext&) (proven) -- RELOCATION_MATCH
Callee names introduced: none
Header debt: none

## L145 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L145: NScript::CPredicate_And<L, CIsThingAlive>(const L&, const CIsThingAlive&) for nested alive-predicate chains (51 bytes, thiscall, `ret 8`): the vtable, the left operand's copy constructor (out of line), the right CIsThingAlive copied inline (its vtable, then CScriptThing's out-of-line copy).
Identities:
- 00ef5bc0 NScript::CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CIsThingAlive, CIsThingAlive>&, const CIsThingAlive&) (proven) -- RELOCATION_MATCH
- 00ef6f00 NScript::CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>&, const CIsThingAlive&) (proven) -- RELOCATION_MATCH
- 00ef6fa0 NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>&, const CIsThingAlive&) (proven) -- RELOCATION_MATCH
- 00ef7020 NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>::CPredicate_And(const CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>&, const CIsThingAlive&) (proven) -- RELOCATION_MATCH
Callee names introduced: `??0?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@QAE@ABV01@@Z`, `??0?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@QAE@ABV01@@Z`, `??0?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@QAE@ABV01@@Z`, `??RCIsThingAlive@NScript@@UBE_NXZ`, `??_G?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z`, `??_G?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z`, `??_G?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z`, `??_G?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@UAEPAXI@Z`, `??_GCIsThingAlive@NScript@@UAEPAXI@Z`, `??_GCPredicateBase@NScript@@UAEPAXI@Z`, `?Clone@?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ`, `?Clone@?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ`, `?Clone@?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ`, `?Clone@?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@UBEPAVCPredicateBase@2@XZ`, `?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ`
Header debt: none

## L146 -- 4 of 4 matched (4 MATCH)
Model: L146: CQ_ArenaScript hero-entity Init (53 bytes, thiscall, plain `ret`, entity vtable slot 2): clear EarlyTalk / ChamTalk / HintNumber and switch off the entity's opinion and deed reactions.
Identities:
- 00f14610 NScript::CQ_ArenaScript::CRoth::Init() (proven) -- MATCH
- 00f14a30 NScript::CQ_ArenaScript::CFlick::Init() (proven) -- MATCH
- 00f15600 NScript::CQ_ArenaScript::CNeedle::Init() (proven) -- MATCH
- 00f16250 NScript::CQ_ArenaScript::CShadow::Init() (proven) -- MATCH
Callee names introduced: none
Header debt: none

## L147 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L147: STLport _Vector_base<T>(size_type n, const allocator<T>&) for def-class elements (56 bytes, thiscall, `ret 8`): zero the three pointers, allocate n elements (malloc, or 0 for n == 0), set start / finish / end of storage.
Identities:
- 00f25c20 std::_Vector_base<CArenaWaveDef>::_Vector_base(size_type, const allocator&) (proven) -- RELOCATION_MATCH
- 00f25d90 std::_Vector_base<CArenaCreatureDef>::_Vector_base(size_type, const allocator&) (proven) -- RELOCATION_MATCH
- 00f26ec0 std::_Vector_base<CHeroSoulsWaveDef>::_Vector_base(size_type, const allocator&) (probable) -- RELOCATION_MATCH
- 00f27020 std::_Vector_base<CHeroSoulsCreatureDef>::_Vector_base(size_type, const allocator&) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L147: 0x00F26EC0 is bound only as the stand-in ??0CRetailVectorBase_CClanMember@@QAE@KABUCRetailAllocator@@@Z; it is _Vector_base<CHeroSoulsWaveDef> (CHeroSoulsRoundDef's wave vector) -- label correction.
Callee names introduced: none
Header debt: none

## L148 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L148: STLport vector<T>::resize(size_type n) for the arena / hero-souls def records (61 bytes, thiscall, `ret 4`): build a default T (out-of-line constructor), call resize(n, x), destroy the temporary inline (its member vector's out-of-line destructor, then the def base chain down to CResource's destructor).
Identities:
- 00f280a0 std::vector<CArenaWaveDef>::resize(size_type) (probable) -- RELOCATION_MATCH
- 00f28810 std::vector<CHeroSoulsWaveDef>::resize(size_type) (probable) -- RELOCATION_MATCH
- 00f34bf0 std::vector<CArenaRoundDef>::resize(size_type) (probable) -- RELOCATION_MATCH
- 00f353f0 std::vector<CHeroSoulsRoundDef>::resize(size_type) (probable) -- RELOCATION_MATCH
Label corrections / review:
- L148: the callees of the four vector<def>::resize(n) bodies are landed under stand-ins -- default constructors CVtable_012fb2f0 / 012fb498 / 012fb288 / 012fb430 (0x00F280E0 / 0x00F28850 / 0x00F34C30 / 0x00F35430 = CArenaWaveDef / CHeroSoulsWaveDef / CArenaRoundDef / CHeroSoulsRoundDef::ctor), CRetailVector_elem60/56::resize (0x00F28100 / 0x00F28870 / 0x00F34C50 / 0x00F35450 = vector<T>::resize(n, x)) and CRetailVector_poly0x38/0x34/0x3c dtors (0x00F25CE0 / 0x00F26F70 / 0x00F25B70 / 0x00F26E20 = vector<member def>::~vector); this family introduces the std spellings -- label corrections. The def vtables give the element types: 0x012FB2F0 CArenaWaveDef, 0x012FB498 CHeroSoulsWaveDef, 0x012FB288 CArenaRoundDef, 0x012FB430 CHeroSoulsRoundDef.
Callee names introduced: `??0CArenaRoundDef@@QAE@XZ`, `??0CArenaWaveDef@@QAE@XZ`, `??0CHeroSoulsRoundDef@@QAE@XZ`, `??0CHeroSoulsWaveDef@@QAE@XZ`, `??1?$vector@VCArenaCreatureDef@@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAE@XZ`, `??_GCDefPointeeBase@@UAEPAXI@Z`, `?resize@?$vector@VCArenaRoundDef@@V?$allocator@VCArenaRoundDef@@@std@@@std@@QAEXIABVCArenaRoundDef@@@Z`, `?resize@?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAEXIABVCArenaWaveDef@@@Z`, `?resize@?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@QAEXIABVCHeroSoulsRoundDef@@@Z`, `?resize@?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAEXIABVCHeroSoulsWaveDef@@@Z`
Header debt: none

## L149 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L149: CPersistContext_TransferableVectorOfSubComponents<T>::Resize(long n) for the arena / hero-souls def records (99 bytes, thiscall, `ret 4`, wrapper vtable slot 1): PVector->resize(n, T()) with T's implicit default constructor inline.
Identities:
- 00f28550 CPersistContext_TransferableVectorOfSubComponents<CArenaWaveDef>::Resize(long) (proven) -- RELOCATION_MATCH
- 00f28d90 CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsWaveDef>::Resize(long) (proven) -- RELOCATION_MATCH
- 00f35170 CPersistContext_TransferableVectorOfSubComponents<CArenaRoundDef>::Resize(long) (proven) -- RELOCATION_MATCH
- 00f35970 CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsRoundDef>::Resize(long) (proven) -- RELOCATION_MATCH
Label corrections / review:
- L149: 0x0042D9E0 is bound only as stand-ins (??0CBase_0042d9e0@@, ??0CPSCBase@@); the def bodies call it as CDefClassBase::CDefClassBase() (??0CDefClassBase@@QAE@XZ introduced here).
- L149: ??0CDefClassBase@@QAE@XZ is 0x0042D9E0 (stores the CDefClassBase vtable 0x01230BCC); landed 0x004550AC (CInventoryDef::CInventoryDef) binds it to 0x00430370, which stores CParentDefClassBase's vtable 0x01230CBC (= ??0CParentDefClassBase@@QAE@XZ, bound so by 0x00455572) -- correct the landed caller.
Callee names introduced: `??0?$_STLP_alloc_proxy@PAVCArenaCreatureDef@@V1@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAE@ABV?$allocator@VCArenaCreatureDef@@@1@PAVCArenaCreatureDef@@@Z`, `??0?$_STLP_alloc_proxy@PAVCArenaWaveDef@@V1@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAE@ABV?$allocator@VCArenaWaveDef@@@1@PAVCArenaWaveDef@@@Z`, `??0?$_STLP_alloc_proxy@PAVCHeroSoulsCreatureDef@@V1@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@ABV?$allocator@VCHeroSoulsCreatureDef@@@1@PAVCHeroSoulsCreatureDef@@@Z`, `??0?$_STLP_alloc_proxy@PAVCHeroSoulsWaveDef@@V1@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAE@ABV?$allocator@VCHeroSoulsWaveDef@@@1@PAVCHeroSoulsWaveDef@@@Z`, `??1?$vector@VCArenaCreatureDef@@V?$allocator@VCArenaCreatureDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@XZ`, `??1?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAE@XZ`, `??_GCArenaRoundDef@@UAEPAXI@Z`, `??_GCArenaWaveDef@@UAEPAXI@Z`, `??_GCDefPointeeBase@@UAEPAXI@Z`, `??_GCHeroSoulsRoundDef@@UAEPAXI@Z`, `??_GCHeroSoulsWaveDef@@UAEPAXI@Z`, `?resize@?$vector@VCArenaRoundDef@@V?$allocator@VCArenaRoundDef@@@std@@@std@@QAEXIABVCArenaRoundDef@@@Z`, `?resize@?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAEXIABVCArenaWaveDef@@@Z`, `?resize@?$vector@VCHeroSoulsRoundDef@@V?$allocator@VCHeroSoulsRoundDef@@@std@@@std@@QAEXIABVCHeroSoulsRoundDef@@@Z`, `?resize@?$vector@VCHeroSoulsWaveDef@@V?$allocator@VCHeroSoulsWaveDef@@@std@@@std@@QAEXIABVCHeroSoulsWaveDef@@@Z`
Header debt: none

## L150 -- 4 of 4 matched (4 RELOCATION_MATCH)
Model: L150: STLport vector<T>::erase(iterator first, iterator last) for the polymorphic def records (78 bytes, thiscall, `ret 8`): __copy_ptrs the tail down (out of line), destroy the vacated elements through their virtual destructor, set the new finish, return first.
Identities:
- 00f288e0 std::vector<CHeroSoulsWaveDef>::erase(iterator, iterator) (probable) -- RELOCATION_MATCH
- 00f33e50 std::vector<CFireHeartPatternDef>::erase(iterator, iterator) (proven) -- RELOCATION_MATCH
- 00f34cc0 std::vector<CArenaRoundDef>::erase(iterator, iterator) (probable) -- RELOCATION_MATCH
- 00f354c0 std::vector<CHeroSoulsRoundDef>::erase(iterator, iterator) (probable) -- RELOCATION_MATCH
Callee names introduced: `??$__copy_ptrs@PAVCArenaRoundDef@@PAV1@@std@@YIPAVCArenaRoundDef@@PAV1@00ABU__false_type@0@@Z`, `??$__copy_ptrs@PAVCFireHeartPatternDef@@PAV1@@std@@YIPAVCFireHeartPatternDef@@PAV1@00ABU__false_type@0@@Z`, `??$__copy_ptrs@PAVCHeroSoulsRoundDef@@PAV1@@std@@YIPAVCHeroSoulsRoundDef@@PAV1@00ABU__false_type@0@@Z`, `??$__copy_ptrs@PAVCHeroSoulsWaveDef@@PAV1@@std@@YIPAVCHeroSoulsWaveDef@@PAV1@00ABU__false_type@0@@Z`
Header debt: none

