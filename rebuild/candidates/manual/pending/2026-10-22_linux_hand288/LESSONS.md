# Chunk 288 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport map size.** The map is 12 bytes: {header, node count, empty `less<>` padded to 4}. With an 8-byte model, VC7.1 sees several 8-byte locals and emits `and esp,-8` dynamic frame alignment (0x006DD1C0).
- **Map-valued traits objects.** `CPersistTraits<T>` holds `T DefaultVal`, so a map-valued traits object is a constructed and destroyed map (0x006DD1C0).
- **Inline read helpers.** A count read through an inline helper with its own local (`long v; s->Read(&v,4); *out = v;`) removes retail's missing dead store and frees the frame slot for sharing (0x006DD1C0).
- **Default `const CCharString&` arguments.** Write the default as `""`, not `CCharString("")`. The first builds the converted temporary before the copies and passes it by `lea`; the second passes the ctor's eax (0x008A9100).
- **Declaration order.** Declaring `result` before the def pointer decides retail's store order and registers (0x008A9100 CreateCreature).
- **Intrusive releases.** Write them as member functions on a cached `this` (`void DecRef(){ if(--RefCount==0){...} }`) to get `dec [esi]; jne` with no re-read. `delete p` on a class with a non-virtual non-trivial dtor emits a ??_G call; `p->~T(); operator delete(p);` inlines it (0x0041C260).
- **Branch direction and shared tails.** In an if/else, the arm holding the early return sets the jcc direction. A tail retail shares between paths (BeginLoading) comes from ONE statement placed after the if, not one per arm (0x0041C260).
- **Two conditional temporaries.** `if (t->GetDef() && t->GetDef()->GetSubDef(out))`, with an inline by-value `GetDef()` (counted copy), reproduces retail's two conditionally destroyed temporaries: flag word bits 1/2, released 2-then-1 (0x008A1360).
- **Float load order.** Three float members copied to named locals load in declaration order. Match retail's load order, not the member order (0x008A1360: +A8, +AC, +A4).
- **The +0x900 virtual.** In GetNearestEnabledDiggingSpot the +0x900 virtual is slot 576 IsDiggingSpotEnabled (ghidra_out/cgsi_slots_aligned.tsv maps GSI slots to names). The landed GetFurthestWithDefName 0x0089AA70 model (vector<CScriptThing>, inline devirtualised GetPos, `_Destroy` + free) carried over verbatim (0x0089EF90).
- **HasTC inlining.** Mark HasTC `__forceinline`. In long bodies VC7.1 otherwise leaves it as a call with `push 0x22` (0x0089FEB0).
- **Intelligent-pointer copies.** A copy whose GetPItem comes AFTER the derived vtable store is the copy ctor `{ SetPItem(other.GetPItem()); }`. A ctor taking `CThing*` would evaluate GetPItem first. Mark it `__forceinline` in long bodies (0x008A7A70).
- **x87 sums.** VC7.1 canonicalises `X*X+Y*Y+Z*Z`: the `Y*Y+X*X+Z*Z` order produces identical code. Retail's Z, X, Y square order is not reachable that way (0x0089CE70).
- **Facing temporaries.** A slot-70 GetFacing temporary read through eax (`mov esi,eax`) after the call is NOT a named local or a const-ref. Both of those read the frame copy. It is still open (0x0089CE70).
- **Register-caching tell.** A thiscall made right after another call without reloading ecx (0x00A02DF0: 0x00A01F50 then 0x00A02020) is the whole-program register-caching gap. Recognise it at triage and skip.

Review at landing:
- **Grade counts.** 0x008A1360 matched in 2 grades and 0x0089EF90 in 1. 0x0041C260 (gf_a..gf_g) and 0x009FF8D0 needed more than 4 grades.
- **Removed operator new declarations.**
  - 0x0041C260 and 0x009FF8D0 had a redeclared `void* __cdecl operator new(unsigned int)`. It was removed before finish.py, and both regraded RELOCATION_MATCH.
  - The landing session should confirm that no other agent's TU still carries one.
- **Line-1 names that differ from the catalog.** The leaves were checked with objdump on `$S/wine/q/<addr>/<addr>.obj`.
  - 0x0041C260 is CGraphicInfoFrame::GetFrame, with leaf `?GetFrame@CGraphicInfoFrame@@`.
  - 0x00CEFCC0 is Tutorial_CombatMultiplier, with leaf `Tutorial_CombatMultiplier`.
- **Local base models with a header available.** CGameScriptInterface is modelled locally in 0x008A9100, 0x008A1360 and 0x0089EF90. Retail STLport maps are 12 bytes, so retail member offsets sit 4 below `engine/CGameScriptInterface.h` from +0x4C on. The header does not fit.
- **Same-TU stand-ins for the lander to bind.**
  - 0x008A1360: CDefClassBase::GetSubDef 0x005130A0 (landed elsewhere as `GetSubDef_005130a0` for CShopDef) and 0x0059350C, both under `GetSubDef_*` names.
  - 0x008A9100: GetDef 0x00513160.
- **The 0x004365B0 const overload.** The LowerBound declared const in 0x008A1360 is meant to bind to 0x004365B0.
- **Stale label.** ghidra_out has 0x006CB6E0 as `?DeactivateThemeDensityDisplay@CEditControlCentre@@` in one labels file. It is CTCCreatureNavigation::GetNearbyWanderPosition.
