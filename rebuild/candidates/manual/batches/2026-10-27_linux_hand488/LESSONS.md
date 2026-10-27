# Chunk 488 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- q.py does NOT reject a `// Leaf:` that names no symbol: it grades another function and can still say MATCH. After
  every grade, check the leaf with `objdump -t $S/wine/q/<addr>/<addr>.obj` (filter `(sec N)(fl 0x00)(ty 20)`; note
  that `?$` symbols contain `$`, so do not `grep -v '\$'`). 0x006BB940 graded RELOCATION_MATCH with a public-static
  (`SI`) symbol while the PDB says private static (`CI`): declare it under `private:`.
- `__true_type` / `__false_type` tag temporaries need a USER constructor, exactly like `random_access_iterator_tag`:
  without one VC7.1 zero-initialises the `_IsPODType()` temporary (`mov byte [esp+..],0`) before
  `_M_insert_overflow` (0x006DC2E0 vector<long>::push_back inside a back_insert_iterator copy).
- Counted-pointer release in an assignment: `T* obj = m_pObject; if (obj) { if (--obj->RefCount == 0) obj->Destroy(); m_pObject = 0; }`
  gives retail's `dec [ecx+4] ; jne`; releasing through the member re-loads it and costs 6 bytes (0x006A6BE0
  CPhysicsMeshObject::operator=).
- Implicit memberwise operator= of a polymorphic class = a user operator= that assigns each member in order (vptr not
  touched); a 0x30-byte POD member struct copies with `rep movsd` x12 (0x006A6BE0).
- `CCountedPointer<T>::DeleteData` (`static void __fastcall DeleteData(void* p) { delete (T*)p; }`) is the shape of
  the per-type deleter stored at `[block+4]` by Reset; model the object with its members in declaration order and the
  non-trivial member destructors out of line (0x006FD400, 0x007222B0).
- An internal-linkage (anonymous namespace) function stays emitted with `#pragma auto_inline(off)` around it plus a
  small external caller; its leaf keeps only the `?Name@?A0x` prefix because the namespace hash differs per build
  (0x00736910).
- The inlined CThing::GetInterface template with an UNINITIALISED out-local reproduces retail's "miss path reads the key
  temporary's slot" (`mov ecx,[esp+8]`) when the caller does not check the result (0x00736910, landed 0x008386E0).
- STLport `__find_if(first, last, pred, const input_iterator_tag&)` from a /Gr TU: hidden iterator result in ecx, the
  tag REFERENCE in edx (a class iterator never goes in edx); the caller passes the address of one of its own argument
  slots as the tag (0x0073D5A0 / caller 0x0073DCE0).
- `__final_insertion_sort` threshold 16 over 8-byte elements compiles to `((last - first) & ~7) > 0x80`, and the inline
  `__unguarded_insertion_sort` calls `__unguarded_insertion_sort_aux(first, last, (T*)0, comp)` (`push 0`) (0x00743B70).
- Inline by-value helpers (`GetNoOfSampleIndexVariations(type)`, `GetSampleIndexVariation(type, i)`) re-store the
  enum to an argument slot before each `const K&` call; which dead slot VC7.1 picks for the LAST copy is not steered
  by locals, inlining or evaluation order in any form tried (0x00680840, parked at 2 bytes).

Review at landing:
- 00680840 used 13 grades (well over the 4-grade budget; the probe variants were graded before I re-checked the
  budget) and stays parked; its best source is differ/00680840.cpp.
- 006dc2e0 needed 3 grades (one SRC_FAIL for a missing `value_type`).
- Local polymorphic CTCBase models sit next to engine/CTCBase.h (vptr flattened) in eight rows; CTCCombat,
  CTCPhysicsStandard, CTCPhysicsBase and CCreatureActionBase have headers but are modelled locally (see NOTES Header debt).
- Landed sources with wrong or stand-in names: 0x006804C0 (says CVectorMap<ETCInterfaceType, CTCBase*>, is CSimVoice's
  CVectorMap<EReactionSpeechType, long>::LowerBound), 0x00445920 (landed as `_Cons_val`, is
  CGamePlayerInterface::GetMouseWheelMovement), 0x008328D0's model calls 0x00487DC0 `GetHeroThing` (Ego_r:
  CPlayer::GetPControlledCreature), 0x006F1280 (invented CThingCompareMap::Insert), 0x00743430 / 0x006F4470 /
  0x006F44E0 (descriptive SElement8 / SVectorRecord1C / SPointerListRecord1C stand-ins for the CRegisteredModeInfo pair
  and CDefendingCombatantInfo::CCombatRing).
- vland RELOC_CONFLICT on 00736910: landed 0x00896EE0 binds `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ`
  to 0x00487DD0; Ego_r order (GetPControlledCreature 0x00493650 then PeekPControlledCreature 0x00493660) says 0x00487DD0
  is the Peek twin and 0x00487DC0 the Get -- fix the landed row's declaration.
- 006c9c60 is catalogued as `Catch@004ab920` but is an ordinary thiscall function (`ret 8`) -- skipped here under the
  Catch@ rule; it should get a real row.
- No catalog row in this chunk held two functions.
