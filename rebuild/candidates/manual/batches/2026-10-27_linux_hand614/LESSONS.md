# Chunk 614 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A 24-byte struct copy with `add eax,0Ch` between two 3-dword copies is two member copies: the first member has a user copy
  ctor and the second is a trivially copied class. Here that is CScriptedMapBrush::CThingPosAndOrientation {C3DVector Position;
  C3DAngles FacingAngles;}. Two members that both have user copy ctors copy flat from [eax+0..14h]. (0x00820840)
- The STLport 4.6 list ctor shape (`_M_node(0)` store, malloc, self-link, then the `_M_node` store) gives retail's
  `mov [esp+x],0 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; mov [esp+x],eax`. Storing first and then self-linking
  through the member reloads it. (0x00820840)
- A named pointer local for only the first of several calls through the same member (`CTCPhysicsBase* physics =
  thing->PhysicsTC; physics->ResetPosition(...)`, then `thing->PhysicsTC->...` for the rest) keeps the pointer in ebp across
  the argument calls and moves `this` to ebx with a frame spill, as retail. (0x00820840)
- Arguments pushed before a virtual call that takes none mean the pushes belong to the call made on its result.
  `push hi ; push lo ; call [edx+0Ch] ; mov ecx,eax ; call X` is `World->GetThingManager().GetPThingWithUID(uid)`, not a
  two-argument virtual. The parked file had modelled it as GetThingAt(x, y). (0x00820840)
- A block-scoped bool out-argument (`{ bool reached; if (Move(..., reached, ...) != 2) ... }`) lets VC7.1 share its frame byte
  with an inlined callee's presence bool. This fixed a frame-slot-order residue that four other orders had not. (0x0090BD40)
- The out-parameter component lookup `bool PeekTC(const T** out, long id) const` loads the component into eax and the flag
  into cl. The returning form picks edx/al. Changing it can flip an unrelated x87 operand order; a named float temp restores
  it. (0x006AB5D0)
- A same-TU `static __declspec(noinline) R __stdcall F(const A&)` returning a class by value passes the hidden result pointer
  in ESI and pops with `ret 4`, as retail's static GetMarkerFileName does. (0x009C38C0)
- A by-value bool-returning `|`, `|=` or ternary in an inlined setter with a constant argument changes the whole body (359 v
  409). Retail's dead `mov al,[eax+7Ah]` survives only through the `if (a || error) error = true;` shape, and VC7.1 still drops
  it. (0x009C38C0)

- q.py masks jump-table relocations, so a switch whose case bodies are swapped still grades RELOCATION_MATCH. Only vland's
  RELOC_JUMPTABLE check catches it. At 0x006AB5D0 the parked body (and my first match) had band 3 -> jogging and 4 -> running.
  Retail's table 0x006AB758 sends 3 and default to GetMaxRunningSpeed (slot 81) and 4 to GetMaxJoggingSpeed (slot 80). Run
  vland on every switch row before calling it matched.

Review at landing:
- These rows needed more than 4 grades: 00854180 (8), 00a26ad0 (9), 00783a30 (4 here + sweeps), 009c38c0 (8), 006ab5d0 (10).
- The local models next to existing engine headers are listed under "Header debt" in NOTES.md: CThing, CWorld, CWorldMap,
  CTCPhysicsBase, CScriptedMapBrush, C3DAnimationInfo.
- 0x0085B5B0's Leaf is a prefix (anonymous-namespace hash). Check that the lander accepts the per-TU `?A0x` hash, as it
  did for the landed 0x0085B160.
- Errors in already-landed sources (see NAME_FIXES.tsv):
  - 0x00664CB0 calls 0x00747020 as CTCSneakMovement::GetMaxSneakingSpeed;
  - CActiveFile_OnReadFinished_009c3510.cpp is not nested in CFileInstaller;
  - 0x00A4BE00 is landed as C_00a4be00::M;
  - 0x009FC440 is landed / bound under five different names.
- 0x006AB5D0: band 3 is running and band 4 is jogging (jump table). Every landed caller or test that assumes 3 = jogging is wrong.
- 0x0082EEB0 (`FableSurfHelper_0082e4e0::compute`) is the ThingsToMove map `find` and wants a real name.
