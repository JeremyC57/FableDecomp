# Chunk 658 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Twin-landed rows are identity work, not codegen work: 10/10 matched on the first probe once each row's own class /
  element type came from its caller (sole caller = a vtable's slot 0 `??_G` for destructors, slot 11 for Clones, the
  `_M_insert_overflow` caller for `_M_clear`, a CCountedPointer deleter chain for `~vector`). 8 of 10 labels were wrong.
- A Clone twin's member copies follow the PDB member TYPES, not the twin: `bool` + float/long copies as `mov al` / `mov ecx`
  (0x0084C490, 0x008F5740, 0x00925D90), two longs as `mov eax` twice (twin 0x006F8280); the masked-body "identity" ignored that.
- Count the unreferenced `jmp` thunks after a container destructor: one per wrapper level that adds nothing
  (CArray, CVectorMap, owner class). The body itself is the innermost non-inlined level, STLport `~vector`
  (0x00AECA80: three thunks = CArray / CVectorMap / implicit ~CPSCManager; twin 0x00687180: two, a global CVectorMap).
- Implicit TC destructors with a CDefPointer member match verbatim over engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE):
  `dec [ecx+4]; jne; call [eax+4]; mov [m],0` (0x007E24D0, 0x007DFAF0); define the TC's constructor to emit ??_G/??1.
- Anonymous-namespace Clone: a virtual body in `namespace { }` is emitted with no extra caller because the inlined copy
  constructor's vftable store references it (0x00925D90 / 0x00925E40; Leaf `?Clone@<Class>@?A0x`).

Review at landing:
- NAME_FIXES.tsv (6 rows): 0x00AECA80 stand-ins in 0x00AECBF0 / 0x00AECB60; 0x00AECBF0 (landed as CTCMapwho::OnDie) and
  0x00AECC30 (CCountedPointer_00aecbf0::Reset) are CCountedPointer<NParticleEngine::CPSCManager>::DeleteData / Reset;
  0x0062EBC0 / 0x0062F3B0 `CBase_00...` copy-constructor stand-ins in the generated Clone family; 0x00838A00 QAE /
  ScalarDtor stand-ins.
- Landed Kraken constructors 0x009255E0 / 0x00925780 name the members m_flag / m_value (descriptive); the PDB names are
  Interrupt (+0x114) / LoopNum (+0x118). Member rename only, no symbol change.
- The landed 0x00AF2910 is bound `?Add@CRegisteredPSCMap@NParticleEngine@@...` (a descriptive class); it is
  CVectorMap<CCharString, NParticleEngine::CRegisteredPSCInfo, CKeyPairCompareLess<...> >'s add member; FableWin.pdb lists
  that instantiation's Insert / Find / LowerBound / Sort / Update, so ?Insert@?$CVectorMap@... is the likely spelling (unproven) -- worth a naming pass.
- The landed twin 0x006F8280 and its generated K037 siblings model the base as `CBase_0062f3b0` with `CBase_0062f3b0* Clone()`
  (non-const, wrong return type spelling vs Ego_r `?Clone@..@@UBEPAVCCreatureActionBase@@XZ`).
- 0x0071A570 uses the EmitImplicitDestructor helper (not a retail function), same as the landed 0x0071A430.
