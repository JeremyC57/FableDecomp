# New findings

- 00a56670 GFGetHermiteSplinePosition: spelling the doubled squared parameter as `2.0f * s2` instead of `(s2 + s2)` preserves the fadd instruction sequence but fixes the final copy schedule. Naming the sum separately is byte-neutral and does not fix it. All295 bytes match after two verified float-literal relocations; both behaviour test compilers pass.
- 00a22ca0 GetAngleXZ: after the complete shared-header rebase, construct the forward copy with explicit X/Y/Z constructor arguments instead of the copy constructor. This changes the first source references and fixes one cross-product factor load order; all368 bytes match with16 audited relocations and both behaviour tests passing. A const-reference forward changes the frame and is worse.

- 00a358f0: remove the unproven by-value SetFromAxes helper, copy Up and Forward, bind GetRight() to a const reference, then assign matrix fields directly. The lifetime-bound temporary reproduces the last cross-product operand order: raw185/185 match with shared headers and no relocations.

- 0099bb80: Binding the wcslen return to `const unsigned int& length` before adding4096 changes mov-plus-add to retail lea; plain value local was inert. Real shared string/vector code and protected AllocStringData naming retained. Matched197B, seven relocation targets and60 controlled behavior cases validated.

- 006d6cb0/006d68d0: Rewrite `(f3+f4)*0.3f + f5*0.4f` as `f3*0.3f + f4*0.3f + f5*0.4f`. RTM refactors the same arithmetic but omits one no-code0x269 PAREN tuple. Actual scheduler trace: baseline cycle55 selects that PAREN, delaying the clamp store until after fmul in cycle56; the new form selects the store in cycle55 and fmul in56. Both495-byte rows then match. Each diagnostic object's leaf was checked identical to a normal, unmodified RTM build. No diagnostic compiler or object is part of the deliverable.

- 0x005b4eba CalculateArmorValues: the real, visible IS_THING_ALIVE<const CThing*> body removes reference capture without changing the native call. Combined with the original GetDef/GetSubDef bodies and genuine shared definition base classes, the full3372-byte method matches. Independently verified184 relocation/literal entries, both five-case mappings, and exact-body VC7.1/g++03 tests.
- 0x006d8830 AddSuccessfulMultistrikeHit: group the variation call result before multiplication: `(GetAttackVariationMultiplier()) * strength * found->second`. The additional no-code grouping tuple occupies scheduler cycle2; existing integer occupancy through5 moves the pending boolean push to6, after the x87 product. Plain C++/default flags;321bytes,119instructions,9bindings and both test builds independently verified. Parenthesizing the fabs result, the strength, or the whole product did not reproduce it.

- 0074e140: grouping only the zero sine argument `(float)(sin(0.0))` preserves one PAREN tuple and changes timing inside the same scheduler region, fixing the returned-vector copy order. It does not create a new scheduling region; normal RTM/proxy equality is recorded.
- 00868e50: raw donor locals can settle a source API form even when its earlier machine stream already looks right. `PeekTC(id,&const_pointer)` instead of separate `HasTC`/`GetTC` fixes a downstream x87 fld/fadd operand choice. Both donors also prove the definition-manager reference is the derived game type. Do not label a lowering residue impossible while original lookup/type evidence is still unexamined.


- 0x005F519F: the original early output-count zero stores and frame packing require complete authentic same-TU magic/expressions bodies. Narrow output scopes approached within four hunks but moved three initialisation instructions; full callee visibility restored every byte.
- 0x005F519F: decode wide literal data as UTF-16. The ASCII-oriented annotation at0x0122E070 showed a space, but the actual string is " - "; checking only the screen grade would miss a supporting-helper behavior defect.
- 0x005F519F: raw donor type records proved original SPELL_CATEGORY_ATTACK/SURROUND/PHYSICAL enumerators. An empty enum with typed ordinal casts failed UBSan; use the original complete enum.


# 0074e140 lesson

A natural grouping around the zero-angle sine, `(float)(sin(0.0))`, preserves VC7.1's no-code PAREN tuple and fixes the final Y-load/X-store interleave. Extra copy constructors and rearranged component statements did not fix it. The validated scheduling trace has the same two DAGs; the change is call/x87 timing within a region, not a new region boundary. Minimize a successful grouping:031's broad groups reduced to035's single pair without losing the exact match.


# 00b801f0 lessons

Inspect raw donor TPI before trusting a shared header's claim that layout is unavailable. Both original donors retain CTVertexSeaBackground's complete two-base hierarchy and all original X/Y/Z fields, even though older name-only extraction missed it. Correct hierarchy preserves full owning declarations and clears naming debt, although it alone does not change this row's emitted bytes.

For an implicit trivial element assignment loop, LICM produced independent preheader loads in Z,X,Y order. A local first iterator copied from the authentic by-value fill parameter changes that invariant order to native X,Y,Z without changing the library signature or assignment semantics. Binding the parameter by const reference is a useful diagnostic but must not replace the known original signature. Source loop rewrites and explicit element copy operators were ineffective or broadly worse.

Measure inline budget for actual call/expansion differences.185 expands the required helpers and matches;184 leaves helper calls and emits477 bytes. Preserve the original library template model, portable source, unchanged default paths and exact standalone body tests.


# 00ba48f0 lessons

A register-allocation gap can depend on the real body of a called function. The genuine Bump helper never touches failed, while Diffuse only writes on failure. With complete bodies visible, primitive priority28 outranks bank/level20; declaration-only bodies gave25 versus26. This restores native bytes while keeping initial block/material references stable across callbacks. A fake capture-only body would not prove the result.

Restore the actual inline/call split before diagnosing allocation. Real GetTexture expands into the native texture wrappers. Its portable FABLE_FORCEINLINE prevents the reconstructed wrappers from becoming small enough to expand into Setup; the latter stays four calls exactly as native.

Audit helper behavior independently of the target byte score and test fixture. Inspecting the entire native SetAsMRU showed that it only relinks; native touch separately updates LastUsedFrame. Both an initial matching source and its boundary stub omitted that distinction. Correcting source and fixture, and testing distinct resource clocks, preserved the match and verified the actual phase.

Raw PDB TPI supplied original const texture fields, reference locals, getters, real owner bases, full fields and virtual slots that older flattened headers obscured. Preserve every attempted strategy and compare default/prior header tokens after adding opt-ins.


-00c3d030: complete positive-count store branches for the first three fields expose the native entry load without changing output-store/alias order;94 raw bytes and35 instructions match. The vendor method name remains explicitly descriptive.
-Original enums: empty enum placeholders can make otherwise plausible portable tests undefined. Recover full original values from donor records and re-run strict-enum optimized sanitizer checks. Five selected rows were revalidated without changing native bytes.
-Default compatibility: compare against the INPUT package, not just an intermediate overlay. The sound-manager forward declarations needed to sit inside the existing opt-in; the two full enum headers were genuinely new files.
