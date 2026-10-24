# Chunk 403 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport push_back callers (CVectorMap::Insert):
  - Make `_M_insert_overflow` a same-TU body under `auto_inline(off)` + `__declspec(noinline)`. Retail then keeps the
    `pair(key, value)` temporary in registers. With only a declaration, VC7.1 spills it, re-reads it, or emits
    `add [esi+4],N` (0x0086D740, 0x0086BB60, 0x0086BC10).
  - When Insert is called from another inline (0x0086BC10), Insert itself also needs `__forceinline`.
- STLport `__false_type` / `random_access_iterator_tag` dispatch temporaries need a user constructor, or VC zero-stores
  the empty tag (0x0086B900). A looked-up CCharString assigned to an entry wants a block-scoped NAMED local, which gives
  `lea` of the temporary where an expression gives `push eax` (0x0086B900).
- /OPT:ICF: a 16-byte `pair<long, CSkeletalMorph>` body is shared with a same-sized propagated label (C3DClothQuad).
  Name the row by its callers' element type (0x0086AC40, 0x0086CC50).
- `CVectorMap::Count` = `UpperBound(k) - LowerBound(k)`, UpperBound as the left operand, held in ebx across LowerBound (0x0086A9D0).
- STLport map `find` through a def pointer: spell `it->first->Map.find(k) != it->first->Map.end()` in full. A reference
  local to the map moves its address into edx (0x008686E0).
- `it->second / totalWeight * multiplier` with a loop-invariant divisor becomes a hoisted `1/totalWeight` reciprocal
  multiply (0x008687A0, 0x00868A80).
- STLport `vector::erase(pos)` of an element that holds a list, all inline:
  - `if (pos + 1 != end()) __copy(...)`; the element operator= is the out-of-line list operator= plus a word copy, and
    the result pointer is strength-reduced to `[esi-8]`;
  - then `--finish; _Destroy(finish)`;
  - the list destructor is clear() then `if (node) free(node)`.

  Retail holds `it` in its frame slot and `it + 1` in ebp (0x008650E0).
- The inline list destructor of a by-value CEnginePrimitiveWeaponTrail TEMPORARY frees the sentinel WITHOUT the null
  test (`mov eax,[esp+28h] ; push eax ; call free`) (0x00864FF0). Model each destructor as retail shows it.
- A def manager's global def read as `GetDefinitionManager()->GlobalDef->Field`: a template CDefPointer `operator->`
  gives `add eax,0D0h ; mov eax,[eax]`; a plain pointer member gives retail's `mov eax,[eax+0D0h]` (0x00864FF0).
- `engine->F(T(args))` with an out-of-line T constructor:
  - a loop-body `CIEngine* engine = info.Engine;` keeps the engine in esi across the constructor (retail);
    `info.Engine->F(...)` re-reads it after the call;
  - a loop-body `unsigned __int64 id = ParentThing->UniqueID;` loads eax:ecx before the pushes.

  (0x00864FF0)
- `(float)GetWorldFrame() / g_framesPerSecond` with the global read into a named local BEFORE the static call copies it
  to the frame and emits `fild ; fidiv` on the copy (0x00864FF0; same idea as the landed 0x007BED90).
- A 2D offset kept as a `C3DVector offset(pos.X - c.X, pos.Y - c.Y, 0.0f)` local lets the named float length take the
  dead reference-argument slot and spills offset.X to the struct's own slot. Two float locals swap those homes and add a
  frame word (0x00871D50). The position copy uses a memberwise C3DVector copy constructor: X/Y stay on the x87 stack and
  Z moves as a dword.
- Erase-or-advance loops over `GetAsCArray() + GetNoComponents()` recompute `end` every pass (0x00870930). The
  3-argument STLport `find_if` is inline over a declared-only 4-argument tag dispatch (0x008709C0).
- `if (a && b) return true; return false;` over two bool locals gives the `mov cl,1 / xor cl,cl` pair; `return a && b`
  widens to eax (0x00864170). `return talking || emoting;` is int-valued `mov eax,1` (0x008644E0).
- GetInterface lookups:
  - The GetInterface out-pointer template must be `__forceinline`. It leaves the component in eax/esi (0x00864580,
    0x00864CB0).
  - HasTC + an entry-returning FindEntry loads it straight into ecx for the call (0x00864840).
  - An empty CWideString argument is a NAMED local built before the pushes (0x00864580).
- `while (--index != -1)` gives `dec edi ; cmp edi,-1`; `while (index--)` swaps ebx/edi (0x00870C90).
- An if/else whose two arms are identical keeps the dead `test bl,bl` of the removed branch (0x00874EE0).

Review at landing:
- More than 4 grades:
  - 0x00867FE0 (5, parked);
  - 0x0086D740 (5, matched);
  - 0x0086BC10 (5, matched).

  0x00874EE0, 0x00864FF0 and 0x00871D50 used exactly 4.
- Local base models next to existing engine headers:
  - 00864170, 008644e0, 00864580, 00864620, 00864840, 00864cb0, 00865ac0 and 00865e40 model CThing /
    CThingCreatureBase / CThingPhysical locally. rebuild/include/engine/CThingCreatureBase.h pads the CThing bytes.
  - `engine/CThingPhysical.h` puts Scale at +0xB4, which is wrong for retail: retail Scale is +0x9C and Health is +0xB4.
  - 00864ff0 (parked) models CIEngine locally next to `engine/CIEngine.h` (only slot 20 is used).
  - The CTCCreatureHitNotification rows use `engine/CTCBase.h`.
- Errors in already-landed sources:
  - `rebuild/src/compiled/00/86/CWeaponTrailListRecord_CWeaponTrailListRecord_00864f80.cpp` is really
    `NCreatureHitNotification::CFlash::CFlash(const CFlash&)` (Ego_r `??0CFlash@NCreatureHitNotification@@QAE@ABV01@@Z`;
    CFlash = {list FlashGeometry, long CreationFrame}). Its comment calls 0x00864FF0 "CThingPhysical_RenderWeaponTrails".
  - `CThingCreatureBase_WorldUpdate_006666a0.cpp` calls 0x008650E0 "component 0xCD's update (names unproven)". It is
    CTCCreatureHitNotification::UpdateHitNotification.
  - `NCameraModesCStringAndCageBase_Update_008728c0.cpp` names:
    - 0x008722D0 / 0x00871D50 "UpdatePositionWithVelocity / UpdatePosition" and declares the second `void`. Ego_r names
      them `UpdateStringLength` / `UpdateCageRadius`, and both return bool.
    - The current-cage-radius member is retail +0xB8.
  - `global_CopySListHandle040076ff9000770230_00770230.cpp` / `global_FillCElement40076ff9000770200_00770200.cpp` call
    0x0076FF90 a 4-byte record's operator=. It is STLport `list<CWeaponTrailEntry>::operator=`, so those are
    list-of-lists copy/fill instantiations.
- Catalog rows holding several functions:
  - None were skipped.
  - ICF-shared bodies: 0x0086AC40 (also the C3DClothQuad fill) and 0x0086CC50. 0x00867FE0 has a byte twin at 0x00867F60.
