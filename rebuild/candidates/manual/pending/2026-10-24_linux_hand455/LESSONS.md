# Chunk 455 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- map<K, vector<T>>::operator[] (and any map whose default value holds an STLport vector): retail never destroys the
  default vector temporary (its ~vector folds: start == finish == 0). VC7.1 only folds it when it can see that the
  out-of-line vector copy constructor does not write its argument: put that copy ctor's body in the TU under
  `#pragma auto_inline(off)` (declared only: the temporary is re-read and freed, 187v166 / 255v224). 0x007AEBE0,
  0x009D4810, 0x00A2E290, 0x00B2F5E0.
- map::operator[] default value with an OUT-OF-LINE default ctor (size TU, 0x00CD3D2E): STLport's
  `__default_constructed((_Tp*)0)` re-addresses the temporary with `lea` after the ctor and gives the insert result its
  own frame word; `T def;` sends the result to the dead `k` slot (frame 0x24 v 0x28) and a bare `T()` pushes the
  ctor's eax. Re-check the landed 0x005812DF note ("only `T def;` matches") against this spelling.
- STLport 4.6 pair = `pair()` + `pair(a, b)` + template converting ctor + USER copy ctor `first(o.first),
  second(o.second)`: with a pair<long, long> mapped value it keeps the default as two `and [ebp-x],0` stores ahead of
  the key in a size TU (0x005B322B; the implicit copy uses one xor-zeroed register, 76v74) and turns a whole-pair
  `rep movsd` into a memberwise copy at /O2 (0x00721640, 0x00756BE0).
- Size-TU map<enum, bool>::operator[] (0x005BBC4E): the `mov byte [ebp-4],0` after the hidden-result push needs the
  chunk-422 insert stand-in (same-TU noinline `insert(hint, v)` reading v), exactly as the /O2 twins.
- In a size TU, CEnginePrimitiveHandle's inline ctor/dtor and the CIVCountedPointer helpers stay out of line unless
  `__forceinline` (0x00485D30: 61v135); that TU's uniqueness test is the ternary `(Object ? Object->RefCount : 0) == 1`
  (`xor eax,eax` arm; only 0x00485D30 and 0x005BBFD6 in all of retail), not `Object != 0 && RefCount == 1`.
- STLport `vector(size_type n)` (0x00485D30): the allocator and `__false_type` tag temporaries overlay the high byte of
  the LIVE `n` slot (`lea eax,[ebp+0Bh]`) only when the `_Vector_base(n, a)` ctor and `__uninitialized_fill_n` bodies are
  visible (same-TU stand-ins under auto_inline(off)); a `__false_type` without a user ctor is zero-stored with `stosb`.
- An inline key operator< whose two operand chains retail loads one after the other (k's object -> file -> sort key,
  then the node's) wants the two sort keys in named locals (`unsigned long key = ...; unsigned long otherKey = ...;`):
  the direct expression interleaves the chains (0x00991350; same spelling as landed find 0x0098FE70).
- A default value whose implicit ctor initialises only its first member (CMeshEffectPriorityEntry: the counted pointer)
  is overlaid with the value_type temporary: the memberwise copy then reads the uninitialised members back from the
  just-written words (`mov eax,[esp+10h] ; mov [esp+14h],eax`, three bool bytes) -- reproduced by the plain implicit
  default ctor + implicit copy, no trick needed (0x004C5B90, parked for its release only).
- A map whose value type is a vector of 16-byte PODs is copied through a size-TU COMDAT (0x005BB346) even from an /O2
  TU: COMDAT addresses tell you which TU emitted the body, not the caller's optimisation.
- Class keys matter for leaves: CSubTitle and CVsyncCallbackTimer::CPendingQuery are `struct` (U) in the PDB name sets,
  CShopItem / CEngineSceneRenderStats / CEngineTextureMorph / CCompositeBlendDesc / CThreadedFileManager::CFileObject /
  CMeshDataBank::CLipSyncEntry / CScriptInfo are `class` (V). A leaf naming `VCScriptInfo` over a `struct` model matched
  no symbol and silently graded the largest function (0x00CB5F50, fixed).

Review at landing:
- 00756be0 needed 5 grades (incl. one probe) -- over the 4-grade budget; 005b322b and 004c5b90 used exactly 4.
- Same-TU stand-in bodies (accepted precedent 0x0075D2B0 / chunk 422; each header says so): vector copy ctors in
  007aebe0, 009d4810, 00a2e290, 00b2f5e0 (retail 0x007ACEA0 landed, 0x00535470, 0x00A2B970, 0x005BB346); `_Vector_base`
  ctor and `__uninitialized_fill_n` in 00485d30 (retail 0x00485779, landed 0x0043F6C0); insert stand-ins in 005bbc4e
  and 00991350; the noinline less<CCharString> COMDAT (landed 0x0042AAB3) in 00cd39b1 / 00cd3d2e. These bodies are
  not the retail bytes of their own addresses; the lander must bind the calls to the retail callees, not to them.
- Local models next to existing engine headers: engine/CScriptGameResourceObjectScriptedThingBase.h is a flat packed POD
  (`void* __vftable`, no ctor/dtor), unusable as a polymorphic mapped value with out-of-line ctor/dtor (00cd3d2e);
  engine/CMeshDataBank.h has no nested CLipSyncEntry and carries a later-build CArray map (00a2e290);
  engine/CQuestManager.h has no nested CBoast / CQuestCard (004b87f0, 004b91a0, 004b9250); engine/CWorld.h has no
  static Frame member (0049d880).
- Already-landed rows that look misnamed: 0x009F77F0 / 0x009F7810 ("operator&&<CPredicate_And<...CIsThingAlive>>",
  CPredicateAndBuffer model) are /Od-TU `_List_iterator(_List_node_base*)` constructors of list<CVsyncCallbackTimer::
  CPendingQuery> calling the optimised `_List_iterator_base(_List_node_base*)` 0x0041C780 (catalogued "__EH_epilog3",
  also wrong: `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; ret 4`). Landed 0x0042D131 / 0x0042D1B7 / 0x005812DF
  model STLport in namespace `std` (cosmetic; `_STL` per the wave rules).
- 0x004DD728 is one COMDAT shared (ICF) by ~CTCShotDefinitionProjectile and ~pair<const long, CEnginePrimitiveHandle>.
