# Pass11 findings

- 0121e210 / 0121f280: recover static-array access and constness from raw PDB TPI/publics before compiling. The real private const CCharString[4] definitions give the 80-byte initializers on the first shape and correct data symbols; old mutable free-global bindings would have passed a masked byte comparison.

- 01229fc0/0122a010: a real handle destructor notifies first and lets the member destructor release; a named pointee local in the genuine CIVCountedPointer::Release avoids a redundant Object reload after decrement. No special inlining control is needed.
- 00a4b230: two edge reference aliases corrected the first x87 dot order; incremental Z,Y,X edge projections and X,Z,Y squared difference corrected the remainder without changing the seven-region algorithm or compiler flags.
- 00c6c0f0/00c6c530: original GameCODA debug locals reveal four explicit sample updates inside a32-iteration loop; trailing else zero-fill reproduces native block layout, while an early zero-fill return does not. Original names are CSalIIRFilter::Process and m_iA/m_iB/m_iZ.

-00531560: same-TU real vector copy construction restored register caches; default-constructed max/min then assignment preserved temporary reuse; explicit centered-Y float-product casts fixed spill scheduling and a later commutative getter order.

- 00430900: complete PDB member types plus direct native-order binary Transfer calls matched1824bytes immediately; retain value/key template order on TransferMap and full STLport map types.
- 004317d0: no owner/parent vtable stores means compiler-generated destructors; real original Alloc factory materializes the implicit body, eliminating9extra bytes without a fabricated support function.

- 006c9490 (parked): visible real bool-traits parsing body restored frame0x20 by proving the receiver does not escape. It did not fix later string comparison/integral-traits inlining; keep the complete genuine header model and document the remaining call-site differences.

- 008fb170 MATCH1037: original named new_list and new_guy_to_add plus real STLport list copy constructor nearly matched; making the true _M_insert_dispatch body visible under noinline with unnamed tag removed45bytes of escaping-local loads/tag writes and restored every register role. No flag change.

00521620: actual STLport overflow factoring plus visible non-inlined __uninitialized_copy with unnamed empty tag removes the spurious byte store and frame lifetime;1129/1129. Byte parity is separate from c624 nameless-string-overload identity review.

- 00840740: a by-value char parameter in the later STLport resize expands the inlined shared temporary slot and frame by four bytes. Authentic4.5.2 const-reference resize, corroborated by native char-vector004122d0, fixes all59 stack-related bytes at once. Keep older library source and native ABI evidence distinct from later donor PDB types; do not invent a signature solely to obtain parity.

- 00667450 MATCH1755: original CIVCountedPointeeBase::Release owns the conditional Delete call. CDefPointer::Release delegates to it and clears its pointer. Restoring those actual60-byte/33-byte helper boundaries in the same TU fixes the caller's register retention. The full caller was independently regraded, all44 operands checked and34 controlled cases passed both compilers with the actual epsilon bits.

- 00a4efc0 (parked,3475/3491): the real paletted_rotation_track.clear() after resizing the rotation track restores a missing object-lifetime boundary and improves the closest reconstruction. Original CArray constructor ownership was independently proved; the remaining16-byte length gap is not a byte-difference count or a match claim.

- Original full-debug procedure records can reveal locals and lexical blocks missing from optimized PDB records. On0055f710 they restore signed int encrypted_buffer_size/encrypted_data_size and eliminate four invented staging locals without changing the target bytes. Preserve this as a source-fidelity improvement, not an extra match.

- Verify the fixture's constant value as well as its symbol: DELTA_NOTIONAL_ZERO has native float bits0x38d1b717. Replacing convenient test-only epsilon values with those bits changed boundary expectations on several fixtures. Re-run both compiler lanes and bind each receipt to the exact current test and source hashes.

- 00666ef0 remains MATCH1055 with ordinary CThingFilter_OverlappingSphere::operator() using the original radius/test_pos locals. Main rechecked33 operands and170 controlled cases. The supporting search helper's inlining differs from its native counterpart; only the assigned caller is claimed as a match.

- 0078b7c0 MATCH1390: restore original global double fastcall GFTanh(double value) and include actual VC7.1 math.h, whose float fabs -> fabsf -> double fabs overload chain is absent from hand-written CRT declarations. Together with original debug-proven scalar/pair lifetimes this removes the final six scheduling differences. Main independently checked all39 operands and both62-case fixtures; no stronger inline annotation or changed arithmetic was needed.

- 00875fb0 (parked, 1315/1315): the same original GFTanh(double) signature fixes the final centre-X commutative x87 pair. GFSin/GFCos use double returns and float arguments. Three integer loop-setup instructions remain scheduled differently; equal size and corrected floating-point order are not a byte match.

- 00509a20 (parked, 3188/3120): restoring the actual ClipAndConvertWorldMapAreaToLocalArea body and original C2DBoxI copy construction recovers frame space. Original ToC2DCoordI/ContainsPoint restore native conversion order; real transition-vector copy context restores its out-of-line boundary. Keep the smaller 3184-byte alternative separately because its conversion order is less faithful.

- 006b97e0 (parked, 1450/1438): the original GetPDefFromGlobalIndex calls protected-const LoadDef and then reads InstantiatedDefs[index]->PDef. Its full header and access proof improve identity evidence, but exposing that genuine body worsens the assigned caller's inlining. A 1435-byte experiment is closer in length yet worse in structure and is not the selected source.

- 00a58130 (parked, 1272/1272): original GFArcCos is float fastcall with a double argument. Moving its return cast after the product changes native dword constants into qword constants and must be rejected despite plausible arithmetic. Original helper context did not remove the remaining five-instruction capacity-check scheduling difference.
