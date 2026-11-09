# Chunk 892 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00807250 `__unguarded_partition` (pair<C3DVector,long>, targeting comparator): `call; lea eax,[esp+24h]; fstp [tmp]` (retail)
  v `call; fstp; lea` = one missing no-code PAREN tuple (0x269) on the first compare value; `float a = (Get(x));` (parenthesised
  initialiser) puts it there -- c2's scheduler then co-issues nothing with the call and the `lea` wins the next cycle. Measured
  with the DAG dump (hand890 t13.py): fstp prio 0x3801 v lea 0x4800, both released by the call in its own cycle.
- 0x008E8FF0 GetStrafingAnimationName(CThing&): a vtable load (`mov eax,[ecx]`) hoisted above a local's member stores in retail
  = the local's address goes to a callee whose body is IN the TU (here the 0x008E8E50 overload, defined first): c2 then knows
  it is not captured and drops the store->load alias edges. Declared-only callee: the load waits for the stores.
- 0x00808AA0 `__linear_insert`: the same lever pipelines the by-value `val` copy (`point` uncaptured through the TU's
  GetCompareValueForPoint 0x007F9660 body), md 22 -> 15.
- 0x0052EAF0 ChangeTargetPositionDelta: a by-value copy whose loads are hoisted one store ahead in retail (`mov ecx,[d]; mov
  edx,[d+4]; sub esp,8; ...`) means the copied local is NOT address-exposed in retail's source. `MoveTo(d + TargetPosition)`
  (operator+ temporary in d's slot) instead of `d += ...; MoveTo(d)`.
- 0x00BEEDA0 __pop_heap (decal pairs): model template containers with the shared STLport templates, not a local struct with
  declaration-only members: VC7.1 emits the COMDAT bodies of the members it calls (vector copy ctor, operator=) into the TU, c2
  sees that they do not capture, and by-value copies pipeline as in retail. Same lever as GOTCHAS chunk 422 (same-TU bodies).
- 0x0076E4D0 (list/vector helper): same-TU real STLport bodies (`__uninitialized_copy` 0x0076F780, `__uninitialized_fill_n`
  0x0076F7D0) under `#pragma auto_inline(off)` -- the no-capture lever again; no __declspec(noinline) needed.
- 0x00783850 (nearest-thing search): the landed 0x0048B160 CThingCompare_Nearest::operator() body in the TU (template member,
  defined where retail has its COMDAT) -> the deltas' stores stop ordering the next loads.
- 0x004BC800 GetPositionMatrix: an int slot visibly EMPTY in retail between a float chain's fmul and its fstp (a store that our
  build schedules there sits after the fstp in retail) = a PAREN tuple inside the float expression: `(a * b) * c`. Count
  cycles from the call that releases the chain: PAREN after the first fmul is ready at +3 and outranks the int nodes.
  The family pattern: when retail co-issues fewer int ops around an x87 chain than ours, look for a parenthesised sub-product.
- 0x0090BC60 GetFreeBench: a component found with `HasTC` + `GetTC(id)` (returned pointer) lets a later local's zero stores order
  the component's member loads; retail's `PeekTC/GetTC(id, &out)` (out pointer, inside the if -- FableWin's GetTC(id, T**)
  template) removes the edges, and the shared CVectorMap same-TU LowerBound folds the key compare. Use the shared
  GETTC_EXPANDED form, not a returned-pointer GetTC, wherever the row's component is followed by a by-value local.
- 0x008F99A0 LSocket::Receive(vector&): VC7.1's no-capture knowledge is transitive -- a same-TU callee that hands the local
  to a declared-only function still counts as capturing it. Rebuild the TU's call chain down to the leaves that only read
  (here 5 LSocket.cpp bodies). With it, STLport _String_base's zero mem-initialisers die (no need to drop them from the model).
- Overlay gotcha: cl.exe under wine hangs (no C1083) when an overlay header's relative include only resolves next to the repo
  copy; copy the unchanged neighbour into the overlay too.
- 0x008FC430 UpdateScan: c2 cuts a block into scheduling regions of at most 81 IL tuples (c2.dll 0x107B4E6A), counting the
  no-code PAREN tuples; when retail's instructions straddle a region boundary differently from ours in a long x87 block,
  count the tuples (tools/tup.sh) and add/remove parenthesised floating factors until the cut lands where retail's does.
- 0x00B482A0 CEngineLightingManager ctor: a hand-rolled container stand-in (3 pointer members zeroed in its ctor) is NOT the
  same IL as STLport's `vector(const allocator_type& = allocator_type())` -> `_Vector_base(a)` -> `_STLP_alloc_proxy(a, 0)`;
  the allocator temporary changes the member-init stores' heights enough to reorder a later member store against the
  vector's address lea. Use the shared CArray / fable_containers.h vector in constructors, never a local vector model.
- 0x005A887D RefreshBuyPossibilityPC: a store to a local whose address went to a declared-only ctor gets an alias edge to
  every later store through a computed pointer (the in-place by-value iterator argument `mov eax,esp ; mov [eax],ecx`), which
  pins it early. Retail's TU has the COMDAT bodies (CDeletion ctors, list insert / copy / dtor) visible and out of line; make
  them visible + noinline (hdr opt-ins *_BODIES / *_OUTOFLINE) and the store drops to its natural last place. Every callee
  that receives the address must be visible (transitively), or nothing changes (e1 vs e2).
- 0x00EE2490 CTH_Thunder::Main: `mov eax,[esi+14h] ; lea ebp,[eax+48h]` (ParentClass load kept apart from the handle address)
  comes from a `CV_..Script& parent = ParentClass;` reference local (or an inline parent member helper); a free helper taking the
  handle by reference coalesces them (`mov ebp,[esi+14h] ; add ebp,48h`). The landed 0x00EE36A0 names that +0x48 handle
  "RockPile" (Ego_r.pdb: seh_Rival) and +0x7D "IntroPlayed" (Ego_r.pdb: SnowTrollSceneDone).
- 0x00C6E790 / 0x00C6E820 (unrolled-loop pointer-step order): VC7.1 couples the strength-reduced pointers' preheader
  order and loop-end order in reverse (inits appended, updates inserted after `++i`), so no single-index spelling gives
  "A,B / A,B". Indexing ONE array through a copy of the counter (`const int sample = i; accumulator[sample] += ... source[i]`)
  makes that pointer's loop-end step come first while the preheader keeps IL order. The parked libpk twins 0x00C6DA40 /
  0x00C6DAD0 (chunk 443) share the residue and should take the same spelling.
- 0x00B5E0F0 (bookmark test/or flip, parked twice on a "temp-count tie" that only dead loads fixed): rewriting the local model
  on the shared headers (scoped capture / layout / shader-enable helpers, the PDB-named inline CRenderStateManager setters, a
  typed member opt-in) matched first try. Try the shared-header rewrite BEFORE any count lever on these rows.
- **Exposure through a declaration-only STLport member reorders loads around a temporary's store** (0x00B21A50): a `const T&`
  temporary passed to `resize` (out of line in retail) gets memory edges to every later load through a loaded pointer, so
  the store schedules first. FABLE_RETAIL_STLP_VECTOR (visible `_M_fill_insert`) + FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
  remove the edge; a template body alone (`resize` defined in class, `_M_fill_insert` declared only) does not. Diagnose
  with tools/sched.sh: a latency-0 store->load edge the retail order contradicts.
- **LTCG inline decisions come from visible bodies** (0x00B21A50): retail expands CCRC::Calc in a loop and calls it once
  outside; a plain inline body (hdr engine/CCRC.h, FABLE_RETAIL_CCRC_CALC_BODY) reproduces both by VC7.1's frequency rule, with
  no forceinline device. The early-return GetAsCharArray (FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN) gives retail's
  `[index+text]` addressing in the expanded loop.
- **Region-cap PAREN count, second hit** (0x00BE1C10, after 0x008FC430): a long x87 block is cut into 81-tuple regions; where
  retail's statement straddles the cut differently, count tuples with tools/tup.sh and add parenthesised float operands.
  Parentheses around a C4DVector CONSTRUCTOR argument keep a PAREN tuple (function-call argument parens do not, 0x008FC430);
  parens around a folded constant or an initialiser, explicit `(float)` casts and inline accessors add none.
- **Region-cap PAREN count, third hit -- one inline-helper spelling fits two rows** (0x00963880, 0x0096E830): the cut of an
  81-tuple region can fall inside an inline GFFastOneOverSqrt estimate (retail's second `mov edx,17Ch` after the table load) or
  just before a subtraction's last `fsub` (pops and `mov ecx,17Ch` filling the fsub latency ahead of `fld st(0)`). Find k with
  throw-away parens first (0x00963880: exactly +4; 0x0096E830: exactly +1), then choose spellings that hold in BOTH rows: the
  first Newton step's product parenthesised (hdr FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN, +1 in each) fits both; a
  parenthesised C3DVector::operator- would supply 0x00963880's other +3 but breaks 0x0096E830 (`enemyPos - preferred`, md 6), so
  0x00963880 writes toMe's three components out parenthesised instead. Integer parens and `r*(3-x)*0.5f` add no PAREN.
  Review at landing: whether NEWTON_PAREN can become the default for every FABLE_RETAIL_C3DVECTOR includer.
- 0x0096E830: a planar direction built as `a - b; v.Z = 0.0f;` is not the same IL as the constructor `(x, y, 0.0f)`: the
  latter keeps the zero Z product live on the x87 stack and reorders the dot product (552 v 544 bytes).
- 0x0096E830: an Ego_r private inline accessor (`GetCombatManager()` = `GetWorld().GetCombatManager()`) evaluates its call
  before the argument pushes, where `GetWorld().PThingCombatManager_ptr->F(arg)` pushes first; check Ego_r for a
  class-local accessor whenever retail's call precedes the pushes.
- 0x005B6767: a global pointer read retail schedules before an unrelated subtraction while the member load through it comes
  after = IL order global, sub, member (the scheduler tie goes to the lower IL index). VC7.1 forwards a single-use global read
  to its use, so a named pointer alone does not hold it IL-early; a second use does, and a null test written after the
  dereference is dropped without emitting code. Review at landing: the never-taken `if (game == 0) return;` is the only
  spelling found (25 grades); look for a natural second use before folding it into GOTCHAS.
- **Inline-COMDAT callee bodies give capture knowledge without register knowledge** (0x00808AA0): with the comparator's
  scoring bodies as plain out-of-line definitions in the TU, c2 learns both that `this` is not captured AND the callees'
  register usage (`last` moves to edx, 218 v 229 bytes); with the same bodies as `inline` definitions it learns only the first
  (the by-value copy pipelines one store ahead) and keeps the standard eax/ecx/edx kills at the calls -- retail's shape. hdr
  CVectorCompare_BestForTargeting.h `..._BODIES`.
- **STLport algorithm rows are explicit instantiations of the shared templates** (0x00807250, 0x00808AA0, 0x00BEEDA0):
  `template R FABLE_STLP_FASTCALL std::__x(...);` over stlport/fable_containers.h opt-ins (SORT_INTERNALS, HEAP); the /Gr
  convention sits on the template definition, never a local copy of the algorithm. Their leaves end in the anonymous
  namespace's `?A0x` (comparator class) and match by prefix (GOTCHAS 1314); every other leaf is spelled in full.
- **gall.sh / q.py read the hdr/ overlay only when FABLE_HDR_OVERLAY is EXPORTED** (`export FABLE_HDR_OVERLAY=$A/hdr`); without
  it every opt-in row silently compiles against rebuild/include and reports a misleading DIFFER or SRC_FAIL.
- **Commutative operand order follows c2's candidate ids** (0x008A8010, proven with c2prio_op.py): the operand with the HIGHER
  candidate id goes first (lea base / `or` destination); a lea whose base dies is computed in place, otherwise the register
  comes from rotation. Ids follow the first definition in forward IL order -- declaring a named local early does not give it an
  early id. A single-use named local is forwarded into its use: `long x = (long)p.X; long y = (long)p.Y; C2DBoxI
  area(C2DCoordI(x, y), (long)radius);` keeps the X, Y, radius ftol order AND gives x / y higher ids than the radius, so
  retail's `lea ecx,[eax+esi]` (`x + r` in place) falls out; the same values as a temporary C2DCoordI argument list or as
  locals used twice reorder the ftols or swap the lea operands.
- **An out-of-line filter call operator exposes the filter temporary** (0x008A8010, hdr FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE,
  retail 0x006A80A0): the temporary's address escapes into a call c2 cannot see, so its `mov [esp+N],4Eh` store gets memory
  edges to every later load through a pointer and issues before the search-tools load, as in retail. An inline operator()
  lets the store sink below the loads.
- **0x00B6FA40's commutative `test`/`or` bookmark key is the IL tuple count before the site** (52 grades): retail's
  `test eax,esi ; or esi,eax` v ours `test esi,eax ; or eax,esi` at UV setters 2-5 flips with the number of IL TUPLES emitted
  before the setter (a same-symbol `dsum +=` chain moves it; unused symbols / extra locals do not); the key is
  function-local and periodic (~280 dead-load units per cycle). Calibration by deletion: one attach removed gives A A A B B B,
  two give A A A A B B, retail is A A A A A B. Measure such a key by deleting real statements before chasing spellings.
- **VC7.1 block layout has two answers for an if/else chain in a loop** (0x005FD5E0, parked; ~60 reduced-model probes with
  wk/5fd/skel.py): the then-arm is laid out first and the join waits for every forward predecessor, so a chain gives either the
  source order or, with the condition inverted (also via `goto`, `switch (bool)`, a do/while(0) or for(;;) `break`), the
  then-arm-first order; c2 rebuilds the order from the CFG, so where a label or goto target sits in the source never matters.
  Flags (/Os /O1 /Ot /Og /Oa /GS /G5-/G7 /Gy /Gr), the QFE 4035 compiler and a real /GL+/LTCG link change nothing. Only a block
  ending in a real function `return` was ever deferred past the epilogue (its whole enclosing arm moves). Retail's third layout
  (a non-return subtree after `ret`, jumping back into the loop) is a lane gap until the marking is found in c2.
- **0x00F05A10 (parked)**: a nested `Game->GetHero()` vtable load lands in edx in ours v eax in retail
  (`push eax ; mov eax,[ecx] ; call [eax+118h]`); 31 argument / object / cast / local / index spellings are inert -- the
  register comes from c2's +0x30 hint link on the `&ConvoyMembersVector[i]` lea temporary, not from the source shape seen so
  far. `inline_depth(0)` at the terminating returns is kept (0x00F05F23).
- **Local STL stand-ins hide real STLport shapes** (0x00783850): the row's local `namespace std { pair; bitset }` was replaced
  by stlport/fable_containers.h with no byte change; prefer the shared header even when a local model already matches.
- **Name the callee by its PDB symbol, not the catalog** (0x0041E5F2): the 005FD5E0 model's "CFrontEndManager::GetInstance"
  binds to the same address as ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (Ego_r 0x00438C20); reloc_bindings.tsv carries three
  spellings for that one address.

- **q.py and g.sh ignore the `// Leaf:` line** (only gall.sh reads it): without a leaf argument they grade the catalog name's last
  component and take the LARGEST symbol containing it, so a TU with two overloads grades the wrong one (0x008E8FF0 read
  DIFFER(405v135) until the leaf was passed). Pass the full leaf, and spell every Leaf in full: three rows here carried
  prefixes, one of them hiding an access mismatch (0x008FC430 `QAE` built v `IAE` in both PDBs).
## Review at landing
- Local models next to existing engine headers (each matched; rebase or justify when landing):
  - 0x00783850: local CThing / CTCBase / CThingSearchTools models and a 12-byte CHeroCrowdMap stand-in.
  - 0x004BC800: local CTCBase, CTCPhysicsBase.
  - 0x005A887D: local CThingPlayerCreature.
  - 0x008E8FF0: local CThing, CTCPhysicsBase, CPlayer.
  - 0x00EE2490: local script classes (the InlineCopy model of CV_TravellingHeroesScript).
  - 0x00BEEDA0: local CCompositeBlendDesc / CEngineTextureDecal (no shared header exists yet).
  - 0x0076E4D0: local CWeaponTrailEntry and g_WeaponTrailInterpolationSteps.
- 0x005B6767: the never-taken `if (game == 0) return;` (see above) is the only spelling found.
- Whether FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN can become the default C3DVector.h path (needs a regrade of every
  GFFastOneOverSqrt includer).
- Matched rows that needed more than 4 logged grades (ATTEMPTS.log; each lever was found by measurement): 0x008A8010 (38),
  0x00C6E790 (30), 0x00808AA0 (30), 0x00963880 (27), 0x0096E830 (25), 0x005B6767 (25), 0x00807250 (23), 0x008FC430 (20),
  0x0090BC60 (17), 0x00EE2490 (14), 0x00B21A50 (14), 0x00BE1C10 (12), 0x005A887D (11), 0x00BEEDA0 (10), 0x008F99A0 (8),
  0x0076E4D0 (7), 0x0052EAF0 (6), 0x004BC800 (6), 0x00B482A0 (5), 0x00783850 (5).
- 0x00807250 / 0x00808AA0 leaves end in `?A0x` (anonymous-namespace comparator, hash changes per build) and match by prefix.
