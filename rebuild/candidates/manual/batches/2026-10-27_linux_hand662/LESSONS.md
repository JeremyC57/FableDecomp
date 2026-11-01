# Chunk 662 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (2026-11-01, chunk 662) Identical {X, CCountedPointer} dtor bodies split by COMDAT placement inside their TU: VC7.1 RTM emits
  a TU's class-member COMDATs (inline ctors, implicit dtors, incl. class-template `~pair`) before its function-template
  instantiations (`_Destroy<T>`, sort/heap helpers), even when the function template is referenced first (lane experiments
  exp/order1.cpp, exp/order2.cpp; implicit `~pair<K,V>` is emitted right before `~pair<const K,V>`). So the copy next to the
  class's inline ctor is `~T`, the one among list/vector helpers is STLport `_Destroy(_Tp*)` (0x009F81C0 / 0x009F88C0,
  0x00A0A3F0 / 0x00A0A5D0, 0x00CB9700 / 0x00CBA6C0, 0x00695F40+0x00695F80 / 0x006966D0).
- A byte scan of the exe for the masked shape (16 bodies of {X, CCountedPointer at +4} here) plus "a COMDAT is kept once per
  image" bounds the candidate names: two copies in two TUs cannot share a symbol, so the copy in the class's own TU (next to
  its ctor / operator=) takes the class name (~C3DMeshVolume 0x00A32690, not the CMeshBankManager TU's 0x00A0A3F0).
- A `_Dest_val<..C3DSkeletalMorphTarget..>` label on a {CCharString, CCountedPointer} body is the Ego_r.pdb ICF representative,
  never a name: settle the element type from the TU (RegisterActionEventTypes 0x006932B0 shows the K and const-K pairs).
- CSubtitledMessage's implicit dtor (0x004D1DB0, 98 bytes, 22 callers, label ~CPatchTesselationEdgeStrip) matches first try
  over the real STLport 4.6 vector text (`_Destroy(first,last)`/`__destroy_aux`, `_Vector_base` + `_STLP_alloc_proxy`,
  malloc-allocator `free`) under CArray / CVectorMap wrappers; no hand model of the free sequence needed.
- 0x0099A300 and 0x0099A2E0 are byte-identical (`mov [ecx],CBaseClass::vftable; ret`): 0x0099A2E0 = ~CBaseClass (??_GCBaseClass
  0x004371F0 calls it), 0x0099A300 = ~CBaseClassNonCopyable (its own vtable store is dead); the catalog label on 0x0099A300
  is wrong (vland WARN RELOC_NAME).
- VC7.1 rejects `new (raw) std::pair<const K, V>` without a user default constructor (C2512) in a behaviour test; give the
  test's pair STLport's `pair() : first(_T1()), second(_T2()) {}`.

Review at landing:
- NAME_FIXES.tsv (4 rows): 0x009F88C0 (landed as ??1CGraphicDataBankInfo, is _Destroy<CGraphicDataBankInfo>; this chunk's
  0x009F81C0 takes the ??1 name -- land the respell together with 0x009F81C0 or the two rows collide), 0x00695F80 and
  0x00A0A3F0 (descriptive CBase_0099eae0_Derived_* -> ~pair<const CCharString, CCountedPointer<CActionEventBase>> /
  ~CMeshDataBankInfo), 0x00A0A3D0 (landed ??0C3DMeshVolume -> ??0CMeshDataBankInfo@CMeshBankManager).
- Tentative splits (type proven, ~pair vs _Destroy / K vs const K from emission order, no caller): 0x00695F40 vs 0x00695F80,
  0x00CB9700 vs 0x00CBA6C0. If a caller or EH table shows otherwise, swap the pair of names together.
- 0x00A325E0 (landed CUnprovenRecord_00a325e0) is most likely ??0C3DMeshVolume@@QAE@XZ (C3DMeshVolume's own TU, next to
  ~C3DMeshVolume 0x00A32690); not respelled here (no direct proof beyond placement).
- The other {CCharString, CCountedPointer} bodies not in this chunk: 0x004ABD40 / 0x004AD2A0 (CScriptThing region, landed
  descriptive) and 0x009EEA50 / 0x009EEA90 / 0x009EF260 (CConsole TU: likely ~pair<CCharString, CCountedPointer<
  CConsoleInputBase>>, ~pair<const ..>, _Destroy<pair<const ..>> by the same placement rule) -- worth a naming pass.
- The Emit*Destructor helpers are not retail functions (same convention as chunks 656-660).
