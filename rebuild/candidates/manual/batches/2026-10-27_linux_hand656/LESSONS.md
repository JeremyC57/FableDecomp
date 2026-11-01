# Chunk 656 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A 0-caller twin of an implicit destructor is often STLport's `_Destroy(_Tp*)` with that dtor inlined (VC7.1 emits both COMDATs
  and retail kept unreferenced COMDATs): 0x00CD4550 ~TeleportData vs 0x00CD4FA0 _Destroy<TeleportData>. Prove it by COMDAT
  placement against Ego_r's Dinkumware `_Dest_val<allocator<T>,T>` neighbours (Ego_r order = retail order), not by callers.
- Implicit-destructor rows (no own vtable store) match first try with no user dtor + a one-line emission helper
  `p->T::~T();` (0x00CD4550, 0x004C2DF0, 0x005E0A80, 0x0078ADF0, 0x00F270D0); the leaf must be the ??1 mangled name.
- CIVCountedPointer<T>::Release over engine/CIVCountedPointeeBase.h (retail opt-in, mutable IVRefCount) must copy Object into a
  named local before `object->Release() == 0` / `object->Delete()`; reading Object twice reloads it after the `dec` (38 v 35,
  0x004C2DF0) -- the same rule engine/CDefPointer.h already documents.
- A CDefClassBase-derived implicit dtor stores only the CDefPointeeBase vtable before `jmp ~CResource`: the inline
  ~CDefClassBase store is dead behind ~CDefPointeeBase's (retail CDefClassBase opt-in reproduces it, 0x00F270D0).
- Identical bodies for `__uninitialized_copy` in two TUs can be the const- and non-const-source instantiations (PBV vs PAV
  first iterator): read the callers -- _M_insert_overflow / _M_fill_insert pass `pointer`, _M_allocate_and_copy / operator= pass
  `const_pointer` (0x0086D530 vs 0x00674CE0).
- `if (ParentThing->Type == 1) SuspendAI(b, this)` in creature modes is CClass::Type (+0x10, FableWin inline GetType() returning
  unsigned long) and CThingAICreature::SuspendAI(bool, void*) 0x00833A10; `GetType() == 1` still compiles to `cmp byte ptr`.
- finish.py splits line 1 on `::` up to the first `(`: a template argument with `::` (std::_Destroy<NScript::..::TeleportData>)
  breaks the module/name split -- keep `::` out of the template argument on line 1 (`std::_Destroy<TeleportData>(...)`).

Review at landing:
- 0x005E0A80: the symbol is tentative. Three identical 0-caller bodies 0x005E0A80 / 0x005E0AB0 / 0x005E1790 belong to the
  map<CCharString, CIVCountedPointer<CAnimComponentBase> > family {~pair<const CCharString,V>, ~pair<CCharString,V>,
  _Destroy<pair<const CCharString,V> >}; this row takes the first as ~pair<const K,V>. The landed 0x005E0AB0 / 0x005E1790
  (CSharedOwner_* stand-ins) should be respelled to the other two once an order rule is settled (same pattern: 0x005E0160 /
  0x005E0C00 for pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >).
- NAME_FIXES.tsv (10 rows) respells landed callers: 0x007F80C0 SetDoorOpen -> SetDoorState (private; also its own landed source),
  0x007F7700 (ClearCutFade binding from 0x007F81E0; landed row name SetMemberConst_007f7700), 0x00979F70 (QAE dtor binding),
  0x0086E4B0 (UninitCopy stand-in), 0x00449C30 (CPlayer_Calls), 0x00487BD0 (CPlayer_Methods::ResetModes), the 17 wrong 0x00833A10
  bindings, the 8 wrong 0x00861D00 bindings, the 10 wrong 0x00693EF0 bindings, and 0x00CD4FD0 (extern "C" stand-in ->
  std::_Destroy<CDataMarkerOutput::CElement>).
- 0x00A01C10 (~CBaseIntelligentPointer, Ego_r ??1CBaseIntelligentPointer@@UAE@XZ) carries ~27 stand-in bindings; not respelled here
  (not proven per caller), worth a bindings audit.
- The pool's twin for 0x00487550 (0x00628F10) is not a masked twin: 0x00628F10 is `lea ecx,[esi+0A8h] ; call 0x00A01C10 ;
  jmp 0x00693EF0` (a creature-action implicit dtor, consistent with its landed source), 0x00487550 is `mov ecx,[esi+0F8h] ;
  call 0x0047AE50 ; jmp 0x00634870` -- the pool's masking also hides opcode/displacement bytes; check a pairing before trusting it.
- vland WARN RELOC_NAME on 0x007F8140 (two callee labels) is expected until the NAME_FIXES above land.
