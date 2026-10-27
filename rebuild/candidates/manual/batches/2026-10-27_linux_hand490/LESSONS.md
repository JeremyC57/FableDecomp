# Chunk 490 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A float-returning virtual forward needs a named float local (`float s = f(d); return s;`) to keep retail's `call ; ret 4`. A bare `return f(d);` becomes a tail `jmp` (0x0082FF60).
- Over a bool callee, `if (!f()) return false; return true;` gives retail's `test al,al ; setne al`. A bare `return f();` passes al through unchanged (0x00838950, 0x0085BD60, 0x00865B50).
- A `0.0` double argument compiles to `fld qword [pool] ; fstp` into the slot, not `fldz`. Float clamps also reload their 0/1 constants from the pool (0x00991670, 0x00886430).
- Under /EHsc, `throw;` is `push 0 ; push 0 ; call _CxxThrowException` (0x0081E9C0).
- Under /Gr, STLport `__find_if` takes the hidden iterator result in ecx and the tag reference in edx (0x00877FD0).
- A __pop_heap* whose pair has a user copy ctor rebuilds the by-value argument through `mov eax,esp`. With an out-of-line record copy (0x00992DB0), the callee destroys its own by-value parameter at the end (0x0086F9A0, 0x00992DB0).
- An implicit copy constructor is emitted out of line by a placement-new helper (`new (p) T(x)`). Retail's own `_Construct` does the same (0x00884D40, 0x008FC840).
- A static `__fastcall` that returns a class by value takes the hidden result pointer in ecx (0x008B0000).
- `bool r; if (c) r = false; else r = true; Unlock(); return r;` makes VC7.1 duplicate the unlock call into both arms (retail 0x0098E590). Pre-initialising `r = true` gives one shared tail with `mov bl,1` hoisted.
- An STLport iterator returned by value goes through the hidden pointer only when the iterator has user constructors. A POD iterator struct comes back in eax (0x0098E590).
- STLport 4.6 `_List_iterator(const iterator&)` is a user copy constructor only for `iterator`, not `const_iterator`. So a by-value `iterator` argument is built in its slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`) while const_iterators are pushed as words (0x008FC840).
- STLport 4.6 `_List_base` builds the sentinel in a local (`__n = allocate(1); __n->next = __n->prev = __n; _M_data = __n;`). Writing through `_M_node._M_data` reloads it (0x008FC840).
- Reading a member into a local before the identity test (`T* cur = Object; if (cur != other.Object)`) gives retail's load order (the other side first) and keeps one register (0x008F0000).
- For a "store constant, then read another member" pair, write the store first. VC hoists the read but keeps the spill below the store (0x008F9D00, LTimer::Start).
- Emit a private STLport member (`_M_find`) with an explicit instantiation of that member. Access checks do not apply to explicit instantiations (0x008EE850).
- Tooling: q.py does not reject a leaf that matches no symbol; it WARNs and grades the largest function. To get the exact leaf without spending a grade, run a compile-only `verify_and_land.cl` + `objdump -t` (agent490 `cc.sh`) (0x0086F9A0).

Review at landing:
- Local base models sit next to an existing engine header, because the header carries the vptr as data or lacks the method:
  - CThingAICreature.h (0x0082FF60)
  - CCombatAction_ControlledStrafeJump.h / CCreatureActionBase (0x0085BD60, 0x00865B50)
  - CThing (0x0082D0C0, 0x00838950, 0x00918050)
  - CCreatureAction_BlockBase (0x00855B90)
  - CWorldMap (0x00838950), CWorld (0x009054F0)
  - CCombatSequence_BanditKingSpinAround (0x008B0000)
  - CScriptedMapBrush (0x0081E9C0)
- Grade budget: tmp/grades.log has 5 entries for 0x0082FF60. The /GS probe 82ff60_gs.cpp is logged twice; the others are the /EHsc probe and two grades of the source. If the duplicate was a real run, this row is one over the 4-grade cap. 0x0098E590 (one of its 4 was SRC_FAIL) and 0x008EDC70 (parked) used exactly 4.
- Already-landed sources with wrong names (vland RELOC_NAME warnings and neighbours):
  - 0x00992630 is landed as CCamera::UpdateCleanupFlags; it is the sound-bank record's operator=.
  - 0x009922F0 is landed as SRecord::SRecord; it is that record's copy ctor.
  - 0x009FC410 is landed as _Cons_val<..CAnimationEntry..>; it is CResourceBank::ProcessLoadingQueue(bool, double).
  - 0x0086E5C0 is landed as std::vector::Resize_CSharerParserEntry_24byte; it is vector<CRandomAppearanceMorph::CBodyPartMesh>::_M_fill_insert.
  - 0x00855CF0 (CCreatureAction_BlockBase::IsOverrideableByActionOfSamePriority), 0x0084EB90 (_Uninit_copy<pair<uint64, CCountedPointer<CActiveEntityScriptBase>>>) and 0x00836780 (CThingSoundEmitter::WorldUpdate, stand-in ?GetState) looked like junk or wrong-name models while I was naming neighbours. Re-check them.
- LTimer methods carry wrong catalog labels: 0x008F8030, 0x008F80D0, 0x008F9CA0 and 0x008FA000 (see NOTES "Label corrections").
- Catalog rows that are not function entries: 0x008F92EF (catch funclet), 0x008F92FD (funclet continuation) and 0x009277CB (mid-function fragment).
- 0x008EE850 and 0x008F0000 each have dozens of byte-identical unreferenced COMDAT copies elsewhere: 47 and 187, all uncatalogued. The same sources would cover them once their instantiation types are proven.
