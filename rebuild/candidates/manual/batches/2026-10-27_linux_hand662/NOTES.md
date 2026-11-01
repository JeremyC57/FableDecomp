# Hand chunk 662 (std::pair / _Destroy of {CCharString, CCountedPointer} records .. CSubtitledMessage): 10 of 10 byte-exact

All 10 rows RELOCATION_MATCH, behaviour PASS under g++ and the lane's VC7.1 cl (tall.py); vland (read-only) 10/10 wins.
Every row matched on its first compile once its own identity was settled; the work was identity. All ten manifest labels
were wrong (nine `_Dest_val<..pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>..>`, which is only the
Ego_r.pdb ICF representative of the shape, and one `~CPatchTesselationEdgeStrip`).

Method for the nine 60/61-byte bodies: a byte scan of Fable.exe finds 16 bodies of the {X at +0, CCountedPointer at +4}
shape (13 with tail ~CCharString). A COMDAT symbol is kept once per image (no ICF, no /OPT:REF), so identical bodies are
different symbols. Each row's TU was identified from its landed neighbours; inside a TU the class-member COMDATs (inline
ctors, implicit dtors) come before the function-template instantiations (lane experiments `exp/order1.cpp`,
`exp/order2.cpp`: VC7.1 RTM emits implicit `~pair<K,V>`, then `~pair<const K,V>`, before any `_Destroy<..>`), which splits
"implicit dtor" from "STLport _Destroy(_Tp*)" copies.

## What matched (10)
- **CASoundBank** 0x00A0BD40 `~CASoundBank` -- implicit (no vtable store); engine/CBaseClassNonCopyable.h + engine/CCountedPointer.h.
- **NParticleEngine::CComponentInstance** 0x00AF6570 `~CComponentInstance` -- implicit; engine/CBaseClass.h + CCountedPointer.h.
- **C3DMeshVolume** 0x00A32690 `~C3DMeshVolume` -- implicit, emitted by a one-line `p->T::~T()` helper.
- **CGraphicBankManager::CGraphicDataBankInfo** 0x009F81C0 `~CGraphicDataBankInfo` -- implicit; the twin's source shape.
- **CSubtitledMessage** 0x004D1DB0 `~CSubtitledMessage` -- implicit dtor over the real STLport 4.6 vector text
  (`_Destroy(first,last)` / `__destroy_aux` / `_Vector_base` with `_STLP_alloc_proxy`, malloc-allocator `free`) inside
  CArray / CVectorMap wrappers: the three maps free in member-reverse order (+0x34, +0x24 with the CCharString loop, +0x14),
  then ~CCharString Narrator, `jmp ~CWideString` Text. First probe.
- **std::pair** 0x00695F40 `~pair<CCharString, CCountedPointer<CActionEventBase>>`, 0x00CB9700 `~pair<CCharString,
  CCountedPointer<CEntityScriptBindingBase>>` -- implicit pair dtor (STLport pair has none), emission helper.
- **std::_Destroy** 0x006966D0 `<pair<const CCharString, CCountedPointer<CActionEventBase>>>`, 0x00CBA6C0 `<pair<CCharString,
  CCountedPointer<CEntityScriptBindingBase>>>`, 0x00A0A5D0 `<CMeshBankManager::CMeshDataBankInfo>` -- STLport 4.6
  `_Destroy(_Tp*)` explicitly instantiated, `// Flags: /Gr` (each TU's other free templates are __fastcall).

## Proven
- 0x00A0BD40 ??1CASoundBank@@UAE@XZ -- TU run ctor 0x00A0BD00 / vtable 0x0129C9AC slots 9-10 / this / slot 0 ??_G 0x00A0BD80 which inlines the same sequence; RTTI CASoundBank : CBaseClassNonCopyable; Ego_r.pdb PSymbolMap +4; tail 0x0099A300 = ??1CBaseClassNonCopyable@@MAE@XZ (reloc_bindings).
- 0x00AF6570 ??1CComponentInstance@NParticleEngine@@UAE@XZ -- right after the landed ctor 0x00AF6550 (vtable 0x0129F478), before ~pair<EPSCType,CComponentInstance> 0x00AF65B0 which inlines it; slot 0 ??_G 0x00AF6B90 inlines it; FableWin.pdb ??1CComponentInstance@NParticleEngine@@UAE@XZ; Ego_r.pdb layout.
- 0x00A32690 ??1C3DMeshVolume@@QAE@XZ -- the class's own TU: default ctor 0x00A325E0, this, ~pair<long,C3DMeshVolume> 0x00A326D0 (inlines it), pair<unsigned long,C3DMeshVolume>() 0x00A327E0, C3DMeshVolume::operator= 0x00A32960 (Ego_r.pdb ??4); FableWin.pdb ??1C3DMeshVolume@@QAE@XZ; Ego_r.pdb Name +0 / ConvexVolume +4.
- 0x009F81C0 ??1CGraphicDataBankInfo@CGraphicBankManager@@QAE@XZ -- follows the inline copy ctor 0x009F80E0 and default ctor 0x009F8170 at the head of the CGraphicBankManager TU; the other copy 0x009F88C0 sits among the list function templates (= _Destroy, NAME_FIXES); FableWin.pdb name; Ego_r.pdb layout (0x38).
- 0x004D1DB0 ??1CSubtitledMessage@@QAE@XZ -- 22 call sites in CSubtitledMessage's message lookups (0x004CEA60, 0x004CEBB0), the vector<CSubtitledMessage> helpers and the CActionTalkBase family (Ego_r.pdb CActionTalkBase::Messages = CArray<CSubtitledMessage>); Ego_r.pdb ??1CSubtitledMessage@@QAE@XZ; the body tears down exactly Ego_r's members.
- 0x00A0A5D0 ??$_Destroy@VCMeshDataBankInfo@CMeshBankManager@@@std@@YIXPAVCMeshDataBankInfo@CMeshBankManager@@@Z -- CMeshBankManager TU (Ego_r list<CMeshDataBankInfo>; erase 0x00A0A650 inlines the release at node+8; _Construct 0x00A0A4A0 __fastcall); second copy among the list helpers, the first (0x00A0A3F0, after the ctor 0x00A0A3D0) is the implicit dtor; mirrors the graphics TU.
- 0x006966D0 ??$_Destroy@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@std@@YIXPAU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@0@@Z -- element type proven (CCreatureActionBase TU, RegisterActionEventTypes 0x006932B0, _M_erase 0x00696900); _Destroy vs ~pair from COMDAT placement (function-template cluster beside 0x00696710, before _Destroy_range 0x00696790 / _M_erase 0x00696900).
- 0x00695F40 ??1?$pair@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@QAE@XZ -- type proven (RegisterActionEventTypes builds this pair at [esp+0x1C] and destroys it inline); which of the two adjacent ~pair copies is K vs const K is order-derived (exp/order1.cpp: ~pair<K,V> emitted first). TENTATIVE on that split.
- 0x00CB9700 ??1?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@XZ -- type proven (script TU's CScriptBase::EntityScriptBindings vector; landed __median 0x00CBA620 / operator= 0x00CBA700 / swap 0x00CBA8E0); ~pair vs _Destroy from placement (class-member cluster after the ~CCountedPointer bodies 0x00CB9560..0x00CB9680, beside the u64 pair 0x00CB96C0). Order-derived.
- 0x00CBA6C0 ??$_Destroy@U?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@@Z -- same TU; function-template cluster (after __median, beside the u64 pair 0x00CBA680). Order-derived.

Descriptive (not retail functions, never graded): the emission helpers Emit*Destructor in the seven implicit-dtor / ~pair rows.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00695F40, 0x006966D0, 0x00A0A5D0, 0x00A0BD40, 0x00A32690, 0x00AF6570, 0x00CB9700, 0x00CBA6C0, 0x009F81C0
  "_Dest_val<..pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>..>" -> the names above.
- 0x004D1DB0 "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" -> CSubtitledMessage::~CSubtitledMessage
  (CPatchTesselationEdgeStrip is a 44-byte CBaseClassNonCopyable with no string members; its real dtor is elsewhere).
- 0x0099A300 catalog "CBaseClass::~CBaseClass" -> CBaseClassNonCopyable::~CBaseClassNonCopyable (??1CBaseClassNonCopyable@@MAE@XZ,
  as reloc_bindings / engine/CBaseClassNonCopyable.h already say; ??_GCBaseClass 0x004371F0 calls 0x0099A2E0, the real
  ~CBaseClass; both bodies are `mov [ecx],CBaseClass::vftable; ret`). vland's WARN RELOC_NAME on 0x00A0BD40 is this label.
- Landed rows respelled in NAME_FIXES.tsv: 0x009F88C0, 0x00695F80, 0x00A0A3F0, 0x00A0A3D0.

## Header debt
- engine/CMeshBankManager.h keeps MeshDataBanks opaque and has no nested CMeshDataBankInfo; 0x00A0A5D0 declares the nested
  record locally (Ego_r.pdb layout). No engine header exists for CASoundBank, CComponentInstance, C3DMeshVolume,
  CGraphicBankManager, CSubtitledMessage, CCharString / CWideString: those rows model the Ego_r layouts locally over the real
  engine/CCountedPointer.h, engine/CBaseClass.h and engine/CBaseClassNonCopyable.h.
