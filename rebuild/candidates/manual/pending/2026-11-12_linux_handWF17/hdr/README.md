## 0x0075A7A0

# Overlay for row 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions (repair 3, 2026-11-12)
Every file is a full copy of the CURRENT repo header (rebased 2026-11-12 22:2x) plus `#ifdef` additions only: `diff <repo> <overlay> | grep -c '^<'` = 0 for each.
- engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP -- operator^ with the X and Z components parenthesised (Y bare); keeps the first CMatrix3x4::operator* argument set-up at 0x0075A8CF..0x0075A8D5. The new arm wraps the existing CROSS_* chain in `#ifdef ..._XZ_GROUP / #else / #endif` (the current header's NORMALISE_PAREN_SQUARES_ZYX arm etc. untouched).
- engine/CMatrix3x4.h: FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR -- declares C3DVector CMatrix3x4::operator*(const C3DVector&) const (Ego_r/FableWin ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z, landed 0x00A56530). Declaration, no layout change.
- engine/CTCPhysicsRigidBody.h: FABLE_RETAIL_CTCPHYSICSRIGIDBODY_RESOLVECOLLISIONS (nested in the existing _BODY opt-in, private section) -- declares ?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z (0x0075A7A0). No layout change.
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_INDEX_START -- vector::operator[] as `*(this->_M_start + n)` instead of `*(begin() + n)` (one inline level less; with begin() the row is 729 v 726: retail's [esp+14h] index slot 0x0075A7BA/0x0075A7E0 and the ebx/ebp holds 0x0075A7CB/0x0075A7CE are lost). Inferred, see the row header.
- engine/CRigidBody.h is the repo's (no overlay copy); the row also defines FABLE_RETAIL_C3DVECTOR_CLEAR for its Reset().
- (coordinator, at landing) The instructions that decide STLP_VECTOR_INDEX_START are 0x0075A7BA `mov [esp+14h],ecx`, 0x0075A7D4 `mov [esp+10h],ecx` and 0x0075A7E0 `mov ecx,[esp+14h]` (the overlay comment now cites these, as the row source does).

## 0x00854DD0 (not landed)

- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR declares the 9-arg ctor 0x00789238 (Ego_r ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z) and AddDamageWithAttribute(EDamageAttribute,float) 0x00868CA0 (row 0x00854DD0; USED by the best source).
- engine/C3DVector.h: experimental operator- bodies FABLE_RETAIL_C3DVECTOR_SUBTRACT_MEMBERWISE / _SUBTRACT_COPY_ASSIGN / _SUBTRACT_FRIEND / _SUBTRACT_LOCALS, ctor spellings _CTOR_ASSIGN / _CTOR_FORCEINLINE / _CTOR_ASSIGN_FORCEINLINE, copy ctor _COPY_ASSIGN_BODY (row 0x00854DD0 probes; NOT used by the best source -- drop at landing unless a later attempt needs one).
- engine/CTCPhysicsBase.h: FABLE_RETAIL_CTCPHYSICSBASE_GETRADIUS_DEFERRED (GetRadius defined after the class; probe only, not used by the best source).
- engine/C3DVector.h (attempt 2 probes, NOT used by the best source -- drop at landing): FABLE_RETAIL_C3DVECTOR_IMPLICIT_COPY (no user copy ctor), _SUBTRACT_DEFERRED / _SUBTRACT_DEFERRED_MEMBERWISE (operator- defined after the class), _PLAIN_INLINE (FABLE_VECTOR_INLINE = inline), _SUBTRACT_PLAIN / _SUBTRACT_PLAIN_MEMBERWISE, _SUBTRACT_LOCALS_ZYX / _SUBTRACT_LOCAL_X / _SUBTRACT_LOCALS_XY, _CTOR_DEFERRED / _CTOR_DEFERRED_ASSIGN, _CTOR_CONST / _CTOR_CONSTREF.
- engine/CTCPhysicsBase.h (attempt 3 PROBE, NOT used by the best source -- drop at landing unless a second source proves the access): FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_PUBLIC_PROBE makes Radius (+0x34) public so the row can read physics->Radius as an lvalue (fixes the 0x00854ED2 fadd order).

