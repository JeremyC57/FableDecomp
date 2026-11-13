# 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC -- author session 1 (bwf1)
- grades 1-20: full draft (src/gen.py generator) DIFFER(6962v6931): call list equal to retail except the pastEntry dtor, which retail
  CALLS (0x429C53) and ours expands; frame slots +8 follow from it (an expanded dtor lets pastEntry share the group slots).
- Inline-budget proof (probes): 2 dummy `CCountedPointer<CAction>(new CAction)` + 2 `CCountedPointer<CComponent>(frame)` at the top give
  exactly one called CComponent dtor; 3 CActions overshoot (a tail ctor + 2 dtors). REBIND overshoots with RELEASEDATA, no effect without.
- grades 21-60: inline trace (tools/inl, c2 0x1078D1AA, copied from agent2003) -- the last dtor needs pen 27 (a+g in (9325,9364] at
  sz 53), the curList CCountedPointer(T*) ctor must stay a+g <= 9000 (else called). Base: last dtor a+g 8638. RELEASEDATA layer is
  needed (without it the ctor->last-dtor distance is too short). REBIND overshoots (+~930), CONSTRUCT +192, SMART accessors +15.
  New overlay opt-in FABLE_RETAIL_STLP_MALLOC_ALLOC_LAYER (allocator::allocate -> __malloc_alloc<0>::allocate) + _RESULT +
  SMART = last dtor 9325 (best: still one short, needs +1..+39 more without moving the ctor past 9000).
- grades 61-79 (all DIFFER(6962v6931) or worse): budget knobs measured with the trace (last-dtor a+g, x1 = MALLOC_ALLOC_LAYER + SMART = 9085):
  +CONSTRUCT_HELPER +192, +MALLOC_ALLOC_RESULT +240, +ALLOCATE_NAMED +192, ALLOCATE_IF (with RESULT) -192, SMART +15, IsNull x2 +2,
  REBIND ~+930 (base world). No effect (0 g): ULONG ctor body-assign, alloc_create layer, CCP ctor ternary (sz 78->65, g same),
  copy-init '= new CAction', '(new CAction())', CCountedPointer<CComponent> x(0), dummy Get() calls. Code-changing (rejected): STLport
  4.6 _List_base store order (CAction ctor goes out of line: sc>74), CCOUNTEDPOINTER if(Data), CCP ctor Info(0), named descriptionText,
  no textBank local. Without RELEASEDATA (direct dtor) the window is infeasible (ctor->last-dtor distance 236 < 301).
- WINDOW (measured): last ~CCountedPointer<CComponent> needs pen 27 => a+g in (9327, 9364]; the dtor before it must stay below that
  (step 56); the pastList CCountedPointer(T*) ctor (first tail ctor, a+g = last - 483) must stay <= 9000. From x1 that means +279..+297
  (robust). No combination of the knobs above lands there (L+RS+M = 9325 is the closest: 3..39 short; L+RS+C+M overshoots).
- FableWin.pdb is a later build with Dinkumware STL (_List_val, $$Q push_back), so fwsize cannot judge the STLport helper texts.
- BEST NEXT IDEA: find a real (STLport 4.x or engine-header) text that adds ~+40..55 post-inline g on top of L+RS+M (16-site +3 or
  48-site +1), or ~+90..100 on top of L+C+M; e.g. the real STLport 4.5/4.6 allocator/_M_create_node/_List_base texts from the
  vendor tree (FABLE_THIRDPARTY) instead of guessed ones, then regrade (tools/gc.sh + tools/inl/tr.sh give call list + a+g per site).
  Everything else (statement order, slots, call list) already lines up; the +8 slot shift should vanish with the called dtor.
- last grades: L+C+IF+M and L+RS+C+IF+M = base decisions (short); L+C+NAMED+M overshoots (pastList ctor + 2 dtors called). Session ended at 82 grades; differ/ = v1 (no probe opt-ins).
