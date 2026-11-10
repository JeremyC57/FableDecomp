# Confirmed matches from this pass

- 009e1960 / 009e1a80: a zero-line normalized difference concealed a wrong local branch displacement. Restore original top/n result flow and the rotated character-read loop; no artificial helper is needed. Original donor locals and disassembly supplied the decisive shape.
- 00a17620: GetCenter returns const C3DVector by value; bind its result to the original const reference. Capture GetRadius before the distance call and use it afterwards. This removes the prior double-cast workaround and matches the native schedule.
- 00826c90: original ConvertFromOld uses counted three-byte and two-byte loops. Writing out their stores altered the schedule. The real loops plus shared file records and pointer APIs match all288 bytes modulo audited operands.
- 006e8ce0 / 0080db70: named GetAccurateMagnitude results followed by division assignment reproduce original math; a generic normalisation substitute changed the x87 schedule. Recover original locals before changing operand spellings.

- 00a65720: recover the original allocator helper chain and static member owners. It resolves both the code shape and misleading global/free-function aliases.
- 00a22f80: real quaternion and vector transforms match rotation once the genuine helper graph fits the measured inline budget. Record the threshold separately from source strategies.
- 00f36860 / 00f36a30: preserve original sequential multifractal accumulation and the actual noise-object reference. The heterogeneous form also needs the real noise body visible.
- 00beea00: real STLport pair comparison and heap algorithms recover the intended nested pair/texture type and exact code. A lookalike heap loop is insufficient.
- 00660ae2 / 00660b5d / 00660bd8 / 00660c53 / 00660cce / 00660d49: full authentic find_if input dispatch and predicate visibility resolve the shared temporary-slot discrepancy. All six real task instantiations then match, with private const member signatures.
- 00807a90: restore the actual free intelligent-pointer equality overload, original reference members and STLport predicate adapter. Do not replace the comparison by a reduced local pointer model.
- 00a0bf20: original camera locals, float/int union representation and complete normalisation, including the parenthesized squared sum, recover the constructor. Keep the grouped sum as an explicit header option.
- 008884d0: a complete real theme-conversion helper preserves all external effects and fixes the caller's temporary lifetime; a reduced helper model did not.
- 00a22e10: the original expression plus grouping one dot-product term fixes the perpendicular-basis schedule.
- 00ac7200: retain the original GFRoot wrapper and actual ordered dot/normalisation graph; spelling every operation flat changes the generated sequence.
- 008f5970: original closest-point locals and GetAccurateMagnitude recover the exact projection and boundary logic.
- 00b396e0: full attached-primitive types and genuine handle operations need the measured inline budget. The conversion helper is UndoMatrixConversion, not the opposite operation.
- 0077c960: GetBoundingBox returns a value. Recover the original cosine interpolation and check the actual two-pi literal bits; a masked constant mismatch can survive an instruction-only comparison.
- 009350c0: original compound arithmetic, one grouped addition and the grouped getter result resolve the script-state callback.
- 00941fc0: exposing the real nearest-candidate comparator, together with the complete rough-area search, fixes filter temporary copies. A declaration or partial comparator does not give the same compiler facts.
- 007d5070: grouped card-height arithmetic recovers store order; use the true enum values in the behavior fixture rather than invalid representations.
- 0070d580: complete GetAllCombinationAnims visibility fixes the loop-end lifetime split while retaining the original uncached end expression. The anonymous functor identity has a compiler-generated per-TU hash.
- 00c35dd0: original IsEmpty means IsCreated() && format==0. Replacing it with a negated convenience predicate loses a native repeated check. The true API is CODA_SourcePlay.
- 0065b142: original GetParentThing().GetPos() and real definition/component helpers resolve the frame difference. The dummy-villager operation sets Respawnable, not an invented Enabled field.
- 00b92300: full occlusion helper bodies and measured inline budget92 reproduce the private CreateOcclusionGroup caller; budget91 still leaves a call.
- 00c5a9d0: original powf calls, float gain division and separate reset assignments match the LFO updater without precision-control workarounds. Native EAX pointer storage explains the base-layout difference from the later SDK.
- 00d259a0: a complete state setter written as an enum switch allows the caller's final parent-pointer choice to match. The full setter is behavior-checked but is not itself claimed as another byte match.

## Unresolved findings

- 006509f8: authentic shared types reduce the row to one final store/address ordering pair. Diagnostic scheduling shows a three-operation final region, not an81-tuple cut. Ten source families and later inline-budget diagnostics did not remove it.
- 008c7280: original const GetSubDef, real make_pair and original component chain leave only frame0x10 versus0x14 with related slot operands. Ten source mechanisms tried; no invented padding retained.
- 00c6b9d0: original sal_ftoi uses a native x87 integer conversion. Ten ordinary-C++/intrinsic mechanisms did not reproduce it. The selected finite-domain portable rounding behavior passes tests, but the target remains unmatched.

- 006509f8: when a float return merges before a string destructor and the final store/address-load pair is reversed, branch-local double result values followed by the float return keep a common no-code conversion tuple. Unlike widening the arithmetic itself, this preserves the native float literal and x87 operands. Full549/549 bytes and216 behavior cases pass.

-00712900: a final scratch-load mismatch came from earlier lifetime/control flow, not the getter. The original debug source keeps action=0 at function scope and guarded delete after both arms. Restoring that structure makes all362bytes match. Independently restore CSoundPair(0): its empty default constructor is a different original operation.

-008c7280: visible insert_unique alone was insufficient. Completing its real STLport _M_insert callee changes the same six locals from8+8 to4+8+8bytes, exactly retail. Check the full authentic template dependency chain before adding more source variations.
