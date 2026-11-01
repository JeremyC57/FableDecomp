# Hand chunk 655 (std::find .. CTCHeroPodium::~CTCHeroPodium): 10 of 10 byte-exact

Never-attempted pool NA001: ten rows whose masked body equals a landed row. Every row was re-authored from its twin with
its OWN identity (element type / enum / vtable / class proven from callers, callees and the PDB name sets); none kept the
twin's names. 4 MATCH, 6 RELOCATION_MATCH, all tests PASS under g++ and the lane's VC7.1; vland: 10 WINS, no
RELOC_IDENTITY (one advisory WARN, see Header debt). Probe logs in probe/<addr>.log (every row matched within 1-2 probes,
so the 12-probe park rule never applied).

## What matched (10)
- **std (STLport 4.6)**
  - 0x004B9E20 `std::find<CCharString*, CCharString>` -- the `find` front end of the landed `__find` 0x004B8E40 (empty
    `random_access_iterator_tag` homed in the value slot). Element proven from the sole caller CTCInventoryExperience::
    IsTutorialPlayed 0x0057598A (PDB member `vector<CCharString> TutorialsPlayed`) and the callee. The tag hierarchy needs no
    user ctor here.
  - 0x00A95890 / 0x00B96150 `std::uninitialized_fill_n<CCountedPointer<T>*, unsigned int, CCountedPointer<T>>` with
    T = C3DGroup2 / CEngineSceneGrid. Chains: 0x00A95890 <- _M_fill_insert 0x00A97F70 <- resize 0x00A985A0 (bound
    C3DGroup2; PDB CDeviceIndependent::Groups `CArray<CCountedPointer<C3DGroup2> >`); 0x00B96150 <- _M_fill_insert 0x00B97E90
    <- resize 0x00B98170 <- CEngineSceneMap::Initialise 0x00B949A0 (PDB CEngineSceneMap::GridLevels
    `CArray<CCountedPointer<CEngineSceneGrid> >`). `ret 4` = the 3-argument front end, not the tagged helper. Uses
    engine/CCountedPointer.h and `<new>`'s placement new (a local placement new gives the same bytes).
  - 0x00ADCA30 `std::vector<CVertexGroupBlock>::operator=` -- STLport 4.6 `_vector.c` operator= verbatim over the real
    two-parameter `vector<_Tp, _Alloc = allocator<_Tp> >` (PDB spelling `??4?$vector@VCVertexGroupBlock@@V?$allocator@...`),
    callees declaration-only, `/Gr`. Matched on the first probe.
- **CPersistContext**
  - 0x00E1DF60 `Transfer<NScript::CQR_EscortTraderScript::EscortTraderQuestStage>` (caller CQR_EscortTraderScript::OnPersist
    0x00E1DCF0, key "QuestStage") and 0x00E38DD0 `Transfer<NScript::CV_AssassinAttacksScript::AA_LocationIdentifier>` (caller
    CM_AssassinTrigger::OnPersist 0x00E38D80, key "regionId"); both symbols are in Ego_r.pdb and FableWin.pdb and already bound.
    PDB signatures used: `CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream&, T&)`, inline
    `TransferBinaryOut(CDataOutputStream&, T)` -> WriteSLONG (engine/CDataOutputStream.h), the TransferableComponent ctor(T&)
    and private virtual TransferIn/TransferOut. Shape lesson: `DefaultVal()` on a NESTED enum makes VC7.1 fail (C2064);
    `DefaultVal((T)0)` gives the same zero store.
  - 0x00CDCF80 `TransferVector<bool>(const char*, vector<bool>&)` (Ego_r.pdb `??$TransferVector@_N@...`, bound from the
    landed CFireHeartPatternDef::Transfer 0x00D35070). Retail STLport `vector<bool>` is a plain byte vector (12 bytes).
- **_global / NProfileTimer**: 0x009D8250 `NProfileTimer::EndProfile(long)` -- empty `__fastcall` stub, Ego_r.pdb pair with
  the landed StartProfile 0x009D8240 (donor 0x00B2B460 / 0x00B2B540).
- **CDynamicOutputPin**: 0x00CA9710 `CDynamicOutputPin::Release()` -- IPin slot 2 of the IPin table 0x012BF760, target of the
  landed adjustor thunk 0x00CAA770 (already bound as `?Release@CDynamicOutputPin@@UAGKXZ`); the landed AddRef 0x00CA9700
  DirectShow model verbatim.
- **CTCHeroPodium**: 0x00771F30 the compiler-generated `~CTCHeroPodium` (slot-0 `??_G` 0x00771F10 calls it; Ego_r.pdb lists
  no CTCHeroPodium destructor, and the body stores no vtable). Implicit dtor = inlined `~vector<long>` (free) + tail jump to
  `~CTCBase` 0x00686830; the ctor is defined in the TU so the vftable / `??_G` / `??1` are emitted.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Names: proven vs descriptive
All ten row names are proven (two sources each, listed in each source's Identity line). No descriptive names. Callee
names used: `std::__find<CCharString*,CCharString>` 0x004B8E40, `NProfileTimer` pair, `CPersistContext::CheckCRC`
0x00404500 / TransferObject{Save,Load}Text / TransferVector{Save,Load}Text (PDB), `CDataOutputStream::WriteSLONG`
0x00993EB0 (engine header), `GFSerialiseVectorBinary{In,Out}<bool>` (PDB), `std::vector<bool>::erase(bool*, bool*)`
0x004676BB (STLport spelling; PDB has only the Dinkumware one), `__uninitialized_copy<const CVertexGroupBlock*,
CVertexGroupBlock*>` 0x00ADC440 / `__destroy_aux<CVertexGroupBlock*>` 0x00ADC190 (bound) / `__copy<const CVertexGroupBlock*,
CVertexGroupBlock*, int>` 0x00ADC120 (respelled, NAME_FIXES), `CTCBase::~CTCBase` 0x00686830, `CTCBase::CTCBase(CThing&)`
0x00686800 (non-leaf, PDB ctor spelling), `free` 0x00BFEA14 / `malloc` 0x00BFEA0E (imports, checked with calls.py).

## Label corrections
- 0x004B9E20 "vector<EWeaponClass>::_Umove<EWeaponClass*>" -> std::find<CCharString*, CCharString>.
- 0x00A95890 / 0x00B96150 "_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>...>" (Dinkumware,
  16-byte pair) -> std::uninitialized_fill_n over 8-byte CCountedPointer<C3DGroup2> / <CEngineSceneGrid>.
- 0x00E1DF60 / 0x00E38DD0 / 0x00CDCF80 "Transfer<signed char>" -> Transfer<EscortTraderQuestStage> / Transfer<AA_LocationIdentifier>
  / TransferVector<bool>.
- 0x009D8250 "$E2" -> NProfileTimer::EndProfile.
- 0x00ADCA30 "Vector_Reserve_Copy" -> std::vector<CVertexGroupBlock>::operator=.
- 0x00CA9710 "Thunk_VirtualMethod_2" -> CDynamicOutputPin::Release.
- 0x00771F30 "CTCInventoryItem::OnDie" -> CTCHeroPodium::~CTCHeroPodium.
- Twin 0x00529CC0 (landed as "CTCInventoryItem::OnDie", 0 xrefs): its body frees [this+0x10] and tail-jumps to ~CBaseClass
  0x0099A2E0 with no vtable store -- the shape of an implicit destructor of a CBaseClass-derived class with a vector at
  +0x10, not an OnDie. Unproven which class; review at landing.
- Twin 0x0049B8D0 is TransferVector<CCharString>, not Transfer (NAME_FIXES).

## Header debt
- 0x00CA9710: vland WARN SHARED_HEADER_AVAILABLE CBaseObject -- false positive: the DirectShow strmbase `CBaseObject`
  (release build, data-less) is a different class from Fable's engine/CBaseObject.h; the landed 0x00CA9700 carries the same
  model.
- engine/CTCBase.h flattens the vptr into a data member, so 0x00771F30 uses the landed CTCChest polymorphic chain
  (engine/CBaseClass.h -> CBaseClassNonCopyable -> CTCBase), as CTCChest 0x00750850 does.
- No engine/CPersistContext.h exists: the three Transfer rows keep the Ego_r.pdb local model (UseDefaults +0x14, Mode +0x18,
  PLoadStream +0x24, PSaveStream +0x28).
