# Chunk 416 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport 4.6 `vector::reserve` with `_M_allocate_and_copy` inlined (0x00A13650, 0x00A2C2B0, 0x00A2C360): the out-of-line
  `__uninitialized_copy(first, last, result, const __false_type&)` needs a same-TU body (template under
  `#pragma auto_inline(off)` + `__declspec(noinline)`). Only then does VC7.1 spill the cached finish to the `push ecx`
  word and home the new block and the tag on the live `n` argument slot; a bare declaration swaps the slots.
  At 0x00A2C210 `_M_allocate_and_copy` and `_M_clear` are both out of line: declare them only.
- STLport 4.6 `_Rb_tree::_M_insert` ports verbatim (0x00A2C4E0, 0x00A21D90, 0x00A21E80). The iterator has a user copy
  ctor, so it returns through the hidden pointer (`ret 14h`). An out-of-line value copy (pair<const long, CAStarState>
  0x00A20670) is kept out of line with `template <> pair<...>::pair(const pair<...>&);`. With an x87 key compare
  (less<float>) VC7.1 stores the two null child links as immediates rather than through a zeroed eax.
- `C2DVector::Rotate` (0x00A141D0): GetAngle and WrapAngle must be `__forceinline` (a plain inline GetAngle stays a
  call); `#include <math.h>` + `#pragma intrinsic(atan2, sqrt, fmod, sin, cos)`; 2pi is the double literal
  6.2831854820251465 (the float 2pi widened).
- STLport list size() is `distance(begin(), end())` (O(n) loop) and ports with advance()/insert() verbatim
  (0x00A154D0 AddNavigatorToQueue).
- An out-of-line callee whose body is visible in the TU lets VC7.1 keep a member cached across the call (same-TU
  mod/ref): STLport `__find` at 0x00A14930 keeps `_M_finish` in esi only when its body is in the file.
- STLport 4.6's rb-tree increment is the static `_Rb_global::_M_increment(node)` returning the next node: the walk runs
  on a copy in eax (0x00A25B20, 0x00A26410); a member increment updating `_M_node` in place is 2 bytes shorter.
- STLport pair has user ctors, so `pair<iterator,bool>` returns through a hidden pointer (0x00A26410); an aggregate
  pair returns in eax:edx and loses the frame.
- A predicate taking `pair<K,V>` BY VALUE converts the map's `pair<const K,V>` into a frame temporary whose dead
  `second` store survives (0x00A21730); `const pair&` folds it away (95 bytes instead of 100).
- Put the real `CBasicString<char>::Compare` body above under auto_inline(off): VC7.1 then knows the call writes no
  memory and keeps counts / string blocks in registers across it (0x00A26530).
- A counted-pointer release must cache the pointee in a local (`--old->IVRefCount` sets the flags); releasing through
  the member reloads it after the decrement (0x00A25970).
- The STLport list iterator needs a user copy ctor in the push_front/push_back inlines, otherwise VC7.1 pushes edi
  inside the construct branch instead of the prologue (0x00A21FC0, 0x00A22010).
- An inline base constructor's dead vtable store is dropped by VC7.1 without `__declspec(novtable)` (0x00A2D650).
- CNavigatorAStar::UpdateGoal (0x00A1E030, parked 216v217): `float&` reference locals to the agent's BestDistLeft /
  BestDistRight stop the CSE of `bestRight - distance` into the else arm (keeps the limit in st(1)); a ternary SetGoal
  tail `(Route.size() > 1 ? *++Route.begin() : Route.front()).Position` reproduces retail's tail duplication.
- The engine's table trig is GFFastSin / GFFastCos / GFFastArcCos (FableWin.pdb, `@@YIMM@Z`: __fastcall, the float
  arg stays on the stack so the body ends `ret 4`) over the cosine table 0x013CD550 (1025 floats, cos(2*pi*k/1024),
  start-up loop 0x00A0DBB0) and the acos table 0x013CB530 (acos(k*2/1025 - 1)); GFFastSin = the cos lookup shifted a
  quarter turn back (-256). All round with the bare `fld/fistp dword` float-to-long helper, so pure C++ gives
  `__ftol2`: they are retail_asm rows (0x00A147E0, 0x00A14840, 0x00A22530, inlined twice in 0x00A13EB0).
- Free STLport templates in these TUs are /Gr __fastcall (`YI` mangling): `__copy`, `__uninitialized_fill_n`,
  `__find_if`, `for_each`, `__push_heap`; static members are `SI` (GetRandomUnitVector, ProcessLoading, Create).

Review at landing:
- The four GF table-trig rows (0x00A147E0 GFFastSin, 0x00A14840 GFFastCos, 0x00A22530 GFFastArcCos,
  0x00A13EB0 C2DVector::GetRandomUnitVector) are retail_asm candidates: land them with the FloatToLong helper
  already landed in rebuild/src/compiled/00/af/NParticleEngineCPSCLight_SmoothFadeValue_00af9000.cpp (it holds the
  inline asm this brief forbids the authoring agent to write) and a `// Retail-asm:` line; that file already calls
  the table `g_CosTable`.
- 0x013D2770 is CAsyncLoadingMemoryPool::LoadingCallbacks (STLport vector start; finish 0x013D2774, end 0x013D2778),
  but the landed DisplayProgress names it `g_AutoTinyOptionalObject_013d2770`: cross-row naming conflict.
- The three CAStarState live-state counters 0x013D27B8 / BC / C0 are `g_instances_013d27b8` / `g_instancesA_...` /
  `g_instancesB_...` in the landed 0x00A205C0 (a local `SCountedInstance_00a205c0` model of what is the
  pair<const long, CAStarState> scalar deleting destructor); our rows call them NoOfStates / NoOfStatesOnOpenList /
  NoOfStatesOnClosedList (descriptive names, not in the PDB name sets). Pick one naming at landing.
- Local base models next to existing engine headers: retail CSteeringAgent, CNavigatorAStar (0xC4) and CMeshDataBank
  layouts differ from the generated Ego_r headers (Dinkumware container sizes: 16/12-byte containers vs STLport
  12/4-byte), so the sources carry local models with retail offsets instead of `#include "engine/<Class>.h"`.
- 0x00A26900 was graded once more after correcting its leaf (CloseMeshes is private: `AAE`, now declared under
  `private:`); the earlier MATCH had resolved the leaf loosely.
- No row needed more than 4 grades to reach its match (0x00A21730 matched on its third grade and was graded twice
  more afterwards to measure the `const pair&` alternative; the final gall.sh confirmation runs come on top).
  0x00A1E030 used one grade plus about 40 non-grading od.py variants.
