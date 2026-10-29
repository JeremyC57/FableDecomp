# Chunk 579: lessons from the authoring agent (fold into GOTCHAS at landing)

- **Two inline-dtor exit copies in the wrong order mean the loop form is wrong (0x00F0EDA0).** A thread that takes a by-value
  CScriptThing and runs "loop until terminated, with a termination test inside the arm" matches with
  `for (;;) { if (IsActiveThreadTerminating()) return; ... }`. VC7.1 sinks both returns' inline dtor copies to the end in
  source order. `while (!IsActiveThreadTerminating())` defers only the arm's copy, so the two copies come out swapped
  (230v230 DIFFER).
- **A plain in-class `virtual ~CScriptThing() {}` gives retail's mixed expansion (0x00EF54A0 / 0x00EF5670).** The first
  return inlines the dtor and the later exits call the out-of-line 0x004AA840. `__forceinline` inlines every exit; an
  out-of-line declaration calls at every exit. No inline_depth pragma is needed.
- **The same STLport text inlines different helpers per instance.**
  - vector<CConversation>::_M_fill_insert 0x00F33810 inlines __uninitialized_copy, __copy_backward and fill
    (`__forceinline`) and keeps only uninitialized_fill_n out of line.
  - The landed vector<CWideString> twin 0x004126E0 keeps every helper out of line.
  - The rule: read retail's calls and declare exactly those.
- **The range destroy can be inlined as a virtual-dtor loop (0x00F27200).** vector<CHeroSoulsCreatureDef>::operator='s
  shrink arm inlines `p->~T()` over the surplus. That needs `__forceinline` _Destroy(first, last), __destroy and __destroy_aux.
  vector<CConversation> 0x00E54AA0 calls _Destroy 0x00CDB790 instead.
- **`FadeOutAndKillEntity(const CScriptThing&, float, bool, bool)` is __fastcall, with the thing in ecx, the first bool in dl,
  and the float and last bool on the stack (0x00CBE716).** The call site cannot tell `M_N1` from `_NM1` because both give the
  same register and stack layout. Take the PDB spelling `M_N1`.
- **Retail CScriptBase is 0x40 bytes, so script members sit at Ego_r - 0x14 (the PDB's CScriptBase is 0x54).** Subtract a
  further 4 for every Dinkumware vector before the member, since STLport is 12 bytes and Dinkumware 16. Example: CScriptDef
  HeroSoulsRounds is at retail 0x1050 (Ego_r 0x1114 - 0xc4).
- **Gap-scan rows name themselves from their vtable slot.** Find the Alloc or ctor that stores the vtable, read the slot, and
  match it to the Ego_r member set of that class. All ten catalog labels here were wrong (invented behavioural names or `sub_`).

Review at landing:
- **Local stand-ins for classes with a retail vtable**, because no shared header can express them (NOTES "Header debt"):
  - CScriptThing (0x01238C8C) is modelled on top of engine/CBaseClass.h in 0x00DDB570, 0x00EF54A0, 0x00EF5670, 0x00F0EDA0
    and 0x00F1A8F0. CScriptThing.h is flattened, with no virtuals.
  - CGameScriptInterfaceBase (0x01260F0C), padded by slot, is local in seven rows. CGameScriptInterface.h is flattened.
  - CScriptBase, CEntityScriptBase and CLevelScriptBase<T> have no headers.
  - CPredicateBase, CIsThingAlive (0x012C32F0) and CPredicate_And<CIsThingAlive,CIsThingAlive> (0x012E1B70) are local in
    0x00EF54A0 / 0x00EF5670.
  - CEntityScriptBinding<T> (0x012E1E10) sits over a `__declspec(novtable)` CEntityScriptBindingBase in 0x00E25AB0.
  - The def classes CArenaCreatureDef (0x012FB358), CArenaWaveDef (0x012FB2F0), CArenaRoundDef, CHeroSoulsRoundDef,
    CHeroSoulsCreatureDef (0x012FB500) and CConversation (0x012E5FF0) are declared locally over the shared
    FABLE_RETAIL_DEF_CLASS_BASE CDefClassBase.
  - No row models CBaseClass, CSpawnedFuncBase, CBaseClassNonCopyable, CASuspendableProcess or CSuspendableProcessMicrothread
    locally. 0x00F1A8F0 was regraded after switching to engine/CBaseClass.h.
- **Landed rows that still use local CDefClassBase stand-ins with invented `UnknownDefinitionSlotN` virtuals:** 0x00F25F90,
  0x00F26420, 0x00F26C30, 0x00F27060, 0x00F261F0 and 0x00D35070. The landed 0x00F08590 also defines CBaseClass locally.
- **Two globals to check:**
  - pGlobalScriptDef 0x0143E90C uses the landed `?pGlobalScriptDef@@3PAVCScriptDef@@A` spelling. Ego_r gives it a dynamic
    initializer, so it may really be a CDefPointer object.
  - g_pGameScriptInterface 0x0143E8F8 (0x00F26B90's inline CTimer::operator=) is a descriptive name with no PDB data symbol.
- **Callee spellings to respell (NAME_FIXES.tsv, 10 rows):**
  - the CConversation vector family: resize 0x00F33730, erase 0x00F337A0, _M_fill_insert 0x00F33810, uninitialized_fill_n
    0x00F339E0, and the copy ctor 0x00E54CA0 as bound by 0x00F339E0;
  - the CHeroSoulsCreatureDef helpers: _M_clear 0x00F273B0, _M_allocate_and_copy 0x00F273F0 (IAE, not QAE), and the copy
    ctor 0x00F27060;
  - FadeOutAndKillEntity 0x00CBE716 (`M_N1`);
  - IsActiveThreadTerminating 0x00CB7940 (six invented spellings).
- **0x00F25E70 emits the inline two-argument Transfer COMDAT beside the graded function.** The `// Leaf:` line pins the
  graded symbol.
