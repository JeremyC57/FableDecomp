# Chunk 480 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU stand-in bodies for opaque callees.** Source: give the callee a body in the TU under `#pragma auto_inline(off)` or `__declspec(noinline)`, with its real text. Retail shape: VC7.1 then knows what the callee touches.
  - Dead zero stores before a string temporary's malloc disappear.
  - The temporary's block pointer stays in esi across the call.
  - Empty tags are homed on a live argument slot.
  - Seen at 0x00840B60, 0x0083F9B0, 0x008405F0, 0x00828360, 0x00859040 and 0x00845CE0.
  - A declaration-only callee makes the local escape: it is re-read from the frame and its block re-tested.
- **One missing stand-in can be the whole gap.**
  - 0x0083ECF0 toUpperCase matched in scratch only once basic_string::reserve 0x00777DB0 had a same-TU body.
  - Without that body: frame 0x18, esi zeroed, every string field in memory.
  - With it: frame 0x20, with _M_start/_M_finish in edi/esi.
- **0x0083F420 ParseLocationData** matched in scratch with the same TU model, plus `__declspec(noinline)` members for find(const char*, size_type) 0x00841790 and substr 0x00569330.
- **Outer test on a vector.**
  - `!v.empty()` keeps retail's separate loop-entry test.
  - `v.begin() != v.end()` lets VC7.1 fold it into the loop (0x00840B60).
- **Atoi into a field.** Use `int code = atoi(s.substr(...).c_str()); field = (Enum)code;`.
  - This frees the substr temporary before the store, as retail does.
  - Assigning the cast call directly stores first and spills status+4 (0x00840B60).
- **Literal-equality guards.** STLport `operator==(const string&, const char*)` compiles to the size test, then the memcmp intrinsic as `repe cmpsb`. Use `#pragma intrinsic(memcmp, strlen)` (0x0083F9B0, 0x008405F0).
- **CTC pointer chosen by if/else.** `if (HasTC(n)) p = GetConstTC(n); else p = 0; if (p) ...` gives retail's `mov ecx,[eax+4]; test ecx,ecx`. A ternary does not (0x00845CE0).
- **GetInterface through an arrow ternary.** The form is `return (it == end || type < it->type ? end : it)->component;`. It keeps the merged interface pointer in ecx across both arms (0x00845CE0).
- **Facing vector from a float angle.**
  - Write `float angle = GetFacingAngleXY(); float cosPitch = (float)cos(0.0);` first.
  - Then `(float)sin(angle * 6.2831854820251465) * cosPitch`, a float product, not `(float)(sin * cos)`.
  - Put the result in a named C3DVector, then a named CRightHandedSet passed by reference.
  - A temporary in the call argument hoists the vtable load (0x00845CE0).
- **Block-scoped CCharString for one call.** `{ CCharString name("..."); f(name); }` puts the construct/destroy pair right around the call. The string is homed on a dead argument slot (0x00845CE0, 0x008158D0).
- **C3DVector copy constructor.** C3DVector needs its member-wise copy constructor. A named `const CRightHandedSet&` copied into `C3DVector up` keeps x and y on the x87 stack and moves z through ecx. A direct `GetRHSet().Up` copy swaps the first multiply's operands (0x008158D0).
- **x87 accumulation order follows the statement form.**
  - At 0x00815F00 the x-first sum and the z-first sum come from two different statement forms.
  - At 0x0085F8D0, `X*o.X + (Y*o.Y + Z*o.Z)` loads z, y, x.
  - The flat sum's order depends on context (cf. 0x00856040).
- **fcos compares.** Compare the inline fcos result as a double. A `(float)` cast adds an fxch pair (0x0085F8D0).
- **Inline operator/ for vectors.** It forms its own reciprocal (`fst ; fmul st(2)`). A hand-written `sum * (1.0f / total)` pops the reciprocal instead (0x0084D0E0).
- **The atan argument.** Use `atan((float)sqrt(x))`. Without the float cast, fld1/fpatan run before the dead x87 slots are popped (0x0084D0E0).
- **STLport containers.**
  - `(max)` takes its arguments by reference, so it lands in the `push ecx` word.
  - Copy and fill helpers are same-TU noinline bodies, so all tags share the `atend` / `x` dead slot (0x00818CA0 family, 0x0081CBB0 family).
  - For trivially copyable elements, retail outlines the tag dispatchers (`uninitialized_fill_n`, `__copy_backward_ptrs`) and inlines the workers. The 24-byte pair at 0x0081CBB0 does the reverse (0x0081CD00, 0x0081D200).
- **Partial inlining of a predicate.** VC7.1 inlines an inner `__find_if` only at the first predicate call of the unrolled loop, and calls it at the other six (0x00841900). IS_THING_ALIVE behaves the same way: inline at the first call only (0x00836AD0).
- **__introsort_loop.**
  - `--depth_limit` is written back to its argument slot.
  - `__median` is inline and returns a reference.
  - The comparator `operator()` is a same-TU noinline stand-in (0x0081DB70 / 0x0081DC80 / 0x0081DD90, as the landed 0x009B7ED0).
- **Where a named action is declared.** A named action declared in an else block shares the frame slot of the other branch's temporary. At function scope it gets its own 0xb0-byte slot: frame 0x1ac against retail's 0xfc (0x0085F6F0).
- **Out-of-line ~_List_base at one exit.** VC7.1 puts ~_List_base out of line at one exit (the first-loop return, 0x00827E40) and inline at the others, where the sentinel's null test is dropped (0x00828360).
- **The int3-after-noreturn row.** At 0x0083FCE0 the catalog span stops before the trailing 0xCC after `call _M_throw_length_error`. The body grades RELOCATION_MATCH against the N+1 span (qn1.py).
- **Inline-budget gap, not monotonic.** At 0x0083F650, retail inlines the 0x00567D00 range initialise at two sites and calls it at the third (`return line`). VC7.1 RTM inlines all three, and `#pragma inline_depth(1..3)` does not reproduce the per-site choice.
- **Frame-slot packing gap.** At 0x008411D0, retail packs a materialised `a()==X && b()==Y` bool into the empty allocator temporary's byte ([esp+0xb]). VC7.1 gives the bool [esp+0xa]. Every other byte matches.
- **The x87 stackifier gap.** At 0x00856310, retail keeps dying operands (`fld st(i) ; fmul st(j)`, then `fstp` clean-up). C2DVector temporaries ICE VC7.1 (C1001 fppeeps.c:1368).
- **Tooling: calls.py.** calls.py finds no relocations for functions that objdump splits at `$L` labels. Strip `$L*` symbols first (objcopy `--strip-symbol='$L*'`, as in probe/calls_s.py) before trusting an empty call list.
- **Tooling: norm2.** norm2 prints extra symbol lines (`_atoi`, `_atol`, `_toupper`) that shift a side-by-side diff by one line. Filter `^_[a-z_]+$` before counting mismatches. A "mism=82" at 0x0083ECF0 was in fact an identical body.

Review at landing:
- **Rows that needed more than 4 grades:** 0083f420 had 4 grades plus 1 re-grade of the parked copy, which is 5 q.py runs logged. Rows at the 4-grade budget, not over it: 0081d200, 00836ad0 and 0083ecf0.
- **0083ecf0 and 0083f420 (parked):**
  - differ/0083ecf0.cpp and differ/0083f420.cpp now hold TU models that compile instruction-identical to retail modulo relocations in a scratch compile.
  - They are NOT graded (budget spent), so grade them first. Neither has a test yet.
- **0083fce0:** land by hand against the N+1 span, using differ/0083fce0.cpp and differ/0083fce0.test.cpp. Both lanes PASS.
- **Local base models next to an existing engine header:**
  - CBaseObject: 0080e060, 0080ef50, 00836ad0, 0085f6f0.
  - CTCPhysicsBase: 008158d0, 00836ad0, 00845ce0, 0085f8d0.
  - CScriptedMapBrush: 0082f930.
  - CThingPhysical: 0083d800.
  - CCarryableDef / CDefClassBase / CDefinitionManager / CTCBase / CTCWeapon: 00845ce0.
  - CAIBrain / CCreatureActionBase / CCreatureAction_GetOutOfBed / CThingCreatureBase: 0085f6f0.
  - CPlayer: 0085f8d0.
  - LHHttp2 / LHHttpHeaderStatus: 0083f9b0, 008405f0, 00840b60, 008410d0. The engine LHHttp2 header is Ego_r's layout with 28-byte Dinkumware strings, but TLC uses 12-byte STLport strings (ReceivedHeaders +0x40, DocumentEncoding +0x890). Do not retype these rows to the engine header without a TLC layout.
- **008410d0 models.** It uses global non-template `CRetailString` / `CRetailStringVector` models, following the landed 0x0083FB20 / 0x00841E90 precedent, not std templates. Check the mangling and the call identity of 0x005693B0, 0x00841FC0 and 0x00569940.
- **Same-TU stand-in bodies in 00840b60 and 00845ce0.**
  - 00840b60 carries: LHHttpHeaders::GetHeader 0x0083F650, StartsWith, toLowerCase, `_M_insert_overflow`, `_M_assign_dispatch` and `_M_append`.
  - 00845ce0 carries: GetSubDef<CCarryableDef> and GetSubDef<CWeaponDef>, and both LowerBound variants.
  - Grade the leaf only. Keep the 0x0083F650 stand-in consistent with that row when it lands.
- **Errors in already-landed sources:** the landed 0x0083FB20 "SetConnectionDetails" acts as Close/Reset + Open(host, port) at the HelperGetDocument call site. Re-check the name.
- **Coverage / manifest label errors:** listed in NOTES.md (Label corrections, callee tables). They include:

  | Address | Wrong label | Correct name |
  |---|---|---|
  | 0x0099EAE0 | `_Cons_val` | ~CCharString |
  | 0x0088B8F0 | RemoveAvailableQuestCardType | CAIBrain::SetBrainOutput |
  | 0x0073A6E0 | CountReferences | CTCCreatureModeManager::GetCurrentMode |
  | 0x00415FBC | GFGetPreMainMemoryUsed | CMainGameComponent::PeekDefinitionManager |
  | 0x00414EA0 | ResetRings | ~vector\<CCharString\> |
  | 0x008F9780 | Kill | ~LSocket |
  | 0x00A14440 | GetScaled | C3DVector::Normalise |
  | 0x004C73D0 | CVertexBufferWin32::DoSizeof | CThing::GetPosition |
  | 0x005CC10E / 0x0042B1A6 | GetSubDef\<CAbilityDef\> | GetSubDef\<CCarryableDef\> / GetSubDef\<CWeaponDef\> |
- **Catalog rows that hold several functions:** none in this chunk.
