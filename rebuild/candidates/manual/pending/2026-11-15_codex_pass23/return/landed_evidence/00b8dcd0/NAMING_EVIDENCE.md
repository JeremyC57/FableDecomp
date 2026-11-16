# Original identity, layout, and native bindings

`00B8DCD0` is private `void CEngineOcclusionManager::SelectFlareTestVertexBuffer(unsigned long circle_segments)`, decorated `?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z`. Both original PDB publics agree: Ego_r segment1+00634510 =00A35510, FableWin segment2+01637580 =02DC6580. Original FableWin module973 is Debug\Engine_Occlusion_Manager.obj. ORIGINAL_LOCALS.json records vertexbuffer, found, vert0, anglefactor, verts, c_vert and vert with their original types/scopes. ORIGINAL.disasm preserves its1037-byte debug body. Native RET4 at00B8DF4C ends the whole639-byte function; FrameEnd00B8DF50 calls this exact entry with COccluderFlareEntry::CircleSegments. No fragment or padding is part of the output.

The original body uses lower_bound with anonymous MapperPred, Reset(new CVertexBufferWin32), Initialise(VERTEX_NULL, circle_segments+2, sizeof(CTVertexNormTex1), 0, VB_POOL_MANAGED), Lock/Unlock, CTVertexNormTex1 setters, GFFastCos/-GFFastSin, make_pair/insert, copy assignment on cache hit, and SelectVertexBuffer. Its declarations and source structure are retained. Actual original near-helper calls resolve to02DCFC30 GFFastSin,02DCFCD0 GFFastCos and018C4020 GFFloatToLongNear; raw public evidence is in *_TRIG_NAMES.json. Ego_r contains no retained out-of-line trig publics; the existing canonical shared headers and original debug caller/body independently bind those names. No descriptive fallback name is introduced.

MapperPred is the original anonymous namespace struct name in FableWin's lower_bound and its templated operator() public names. ORIGINAL_MAPPER.disasm at02DCDCE0 proves unsigned pair.first<key, const member, pair by const reference and key by value. Its struct is empty. The original Dinkumware debug iterator wrappers are replaced by the actual retail STLport4.6.2 pointer iterators, with the original upstream lower_bound/__lower_bound algorithm and tag dispatch. The anonymous namespace's generated hexadecimal TU key is normalized in audits; this field is not a renamed source symbol.

## Layout and types

Both complete PDB classes name every manager member and all composite types. Ego_r manager size240D4 has16-byte containers; FableWin debug size240F4 has20-byte containers and larger debug buffer bases. Retail's12-byte STLport containers yield the already established240C0 manager layout. The native constructor/destructor, FrameStart/FrameEnd and this row independently place FlareVertexBuffers at24054, VertexShaderFlareTest at24060, UnitBoxVB at24068, UnitBoxIB at2409C, and the two shaders at240B0/240B8. The opt-in types all these existing members and both handle maps, retaining every other member. No packing is added.

FlareVertexBuffers is std::vector<std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >, element stride12. CCountedPointer is Data then Info, eight bytes. Real shared CVertexBufferWin32 is34bytes, with its complete inherited compression params; the native allocation and all constructor stores agree. CTVertexNormTex1 is32bytes and its actual CGenericVertex setters are used. Both PDB enum records (Ego_r2D72, FableWin22CB) prove EVertexBufferPool and all three exact enumerator names/values. Pool1 is VB_POOL_MANAGED. VERTEX_NULL0 is the existing original EVertexType enumerator; this is a raw32-byte vertex format, not a fixed FVF.

## Globals, calls, and literals

* `GFastCosTable`, `?GFastCosTable@@3PAMA`, native013CD550: both typed original float[1025] globals. FableWin segment4+17DC40 =04BBDC40 is read by both original trig bodies; native interpolation reads013CD550 and013CD554. The audit retains the actual operand/addend as well as symbol base. Old g_CosTable spellings are wrong.
* `NEngineInternal::GPShaderRenderManager`, native01436E14: both typed pointer/public records, explicitly distinguished from the other unqualified GPShaderRenderManager global. Original02DC6957 reads04AC7FF4, exactly the namespace-qualified donor data. Native reads01436E14 then D3DDevice+1D4 and standard SetStreamSource slot100.
* Both raw PDB signatures and canonical shared headers prove CBaseClassNonCopyable's protected ctor0099A2F0, CVertexBufferWin32's vtable0129BA20, Initialise00A63150, and inherited CVertexBufferBase::Lock00AC20A0 / Unlock00AC20B0. Old Win32/stand-in Lock return/class variants are rejected. Native Lock/Unlock are the base's virtual DoLock/DoUnlock forwarding methods.
* Allocations and frees use their actual retail CRT stubs: operator new00BFEA1A, operator delete00BFE9BC, malloc00BFEA0E, free00BFEA14. Every site is independently checked.
* Native float-pool literals are read and compared exactly:2^32 (4F800000),1.0 (3F800000),1024.0 (44800000),0.5 (3F000000). No descriptive constant aliases are substituted.

## Emitted real helper closure

All twelve whole bodies, including target, match independently with recursively audited relocations. Native returns and following bodies establish the exact extents:

| Address | Original helper | Bytes |
|---|---|---:|
|00B8F1C0|std::__lower_bound<Pair*, unsigned long, anonymous MapperPred, ptrdiff_t>|75|
|00B8EBC0|CCountedPointer<CVertexBufferWin32>::operator=|72|
|00B8EC10|CCountedPointer<CVertexBufferWin32>::Reset|103|
|00B8F950|std::vector<Pair>::insert|293|
|00B8E690|CCountedPointer<CVertexBufferWin32>::DeleteData|11|
|00B8EF60|std::__copy_backward random-access dispatch|149|
|00B8EC90|std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> >::operator=|78|
|00B8F840|std::vector<Pair>::_M_insert_overflow non-POD overload|240|
|00B8EDE0|std::__uninitialized_copy non-POD overload|56|
|00B8EE20|std::__uninitialized_fill_n non-POD overload|52|
|00B8F470|std::__destroy_aux non-POD overload|77|

The real STLport sources in stl/_algo.h, _algobase.c, _iterator_base.h, _vector.h/_vector.c and _uninitialized.h settle the release template names; the original debug PDB's Dinkumware names do not override that release implementation. Existing canonical bindings already prove these STLport bodies except lower_bound. Both PDBs also explicitly name counted assignment, Reset and DeleteData for CVertexBufferWin32. Old Destroy_00b8e690 and _Destroy_range aliases are wrong for the reached bodies. No unproven pointee class is retained. Unreferenced shared COMDATs are listed separately and assigned no retail addresses.

Main review: exact normalized STLport instantiation `??$__lower_bound@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@KUMapperPred@?A0xTU@@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@0ABKUMapperPred@?A0xTU@@PAH@Z` at00B8F1C0. Anonymous MapperPred is proven by ORIGINAL_MAPPER.disasm and original publics; pointer iterator, unsigned-long key, int ptrdiff_t, by-value comparator and final int* tag match vendor lower_bound and the native75-byte body. Only compiler-generated anonymous TU hex is normalized.

Exact emitted helper symbols for the independently proven bodies above (same original/vendor types and calling conventions):

- 00b8dcd0 `?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z`
- 00b8f1c0 `??$__lower_bound@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@KUMapperPred@?A0xTU@@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@0ABKUMapperPred@?A0xTU@@PAH@Z`
- 00b8ebc0 `??4?$CCountedPointer@VCVertexBufferWin32@@@@QAEAAV0@ABV0@@Z`
- 00b8ec10 `?Reset@?$CCountedPointer@VCVertexBufferWin32@@@@QAEXPAVCVertexBufferWin32@@@Z`
- 00b8f950 `?insert@?$vector@U?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@V?$allocator@U?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@@2@@std@@QAEPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@2@PAU32@ABU32@@Z`
- 00b8e690 `?DeleteData@?$CCountedPointer@VCVertexBufferWin32@@@@SIXPAX@Z`
- 00b8ef60 `??$__copy_backward@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@PAU12@H@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z`
- 00b8ec90 `??4?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@QAEAAU01@ABU01@@Z`
- 00b8f840 `?_M_insert_overflow@?$vector@U?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@V?$allocator@U?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@@2@@std@@IAEXPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@2@ABU32@ABU__false_type@2@I_N@Z`
- 00b8ede0 `??$__uninitialized_copy@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@PAU12@@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@00ABU__false_type@0@@Z`
- 00b8ee20 `??$__uninitialized_fill_n@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@IU12@@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z`
- 00b8f470 `??$__destroy_aux@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@@std@@YIXPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@0ABU__false_type@0@@Z`
