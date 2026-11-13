#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H
#include "fable_compiler.h"
// CMatrix3x4 (Ego_r TPI: a union whose struct arm is float E11..E43, rows of three, size 0x30; struct_layouts_egor.tsv
// CMatrix3x4_u_0_s_0). The flat members below are that struct arm (identical layout). TransformPoint overloads: FableWin
// ?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z (0x024543E0) and ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z
// (0x01D129F0), inline in retail; the bodies (in-place copy to xx/yy/zz locals, X as one expression, Y / Z accumulated) are the
// forms 0x00AE0070 ApplyTransform expands (Codex pass 12, FableWin locals xx/yy/zz).
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMPLANE
#include "engine/CPlane.h"
#endif
#if defined(FABLE_RETAIL_CMATRIX3X4_TRANSPOSE) && !defined(FABLE_SWAP_DEFINED)
#define FABLE_SWAP_DEFINED
// FableWin.pdb ??$SWAP@M@@YIXAAM0@Z (0x025C2DA0, 18 debug instructions): Lionhead's by-reference __fastcall swap template.
template<class T> inline void FABLE_FASTCALL SWAP(T& a, T& b) { T temp = a; a = b; b = temp; }
#endif
class CMatrix3x4 {
public:
#ifdef FABLE_RETAIL_CMATRIX3X4_DATA_UNION
    // Retail opt-in (Codex pass 17): original anonymous union and Data[4][3], both PDB TPI layouts.
    union {
        struct { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
        float Data[4][3];
    };
#else
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
#endif
    CMatrix3x4() { }
#ifdef FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR
    // Retail opt-in (row 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions): Ego_r.pdb / FableWin.pdb
    // ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z, out of line in retail at 0x00A56530 (landed: the 3x3 part times the vector, result by
    // hidden pointer, `ret 8`); the row calls it twice. A declaration only: no layout change, default path unchanged.
    C3DVector operator*(const C3DVector& v) const;                        // 0x00A56530
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
    // Retail opt-in (wf1, row 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix): FableWin.pdb ?SetIdentity@CMatrix3x4@@QAEXXZ (inline in
    // retail, no out-of-line copy): the nine zeros as one chained assignment (retail stores them last to first, E43..E12,
    // 0x00A114F8..0x00A11512), then the diagonal (E33, E22, E11, 0x00A1151A..0x00A11520). Body only: no layout change.
    FABLE_FORCEINLINE void SetIdentity() { E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f; E11=E22=E33=1.0f; }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETSCALE
    // Retail opt-in (chunk 2030, row 0x00BE9920 the CEngineWaterSettings constructor): FableWin.pdb ?SetScale@CMatrix3x4@@QAEXM@Z (debug
    // only, inline in release): the nine off-diagonal / translation entries cleared in one chained assignment (stored E43 first) and the
    // diagonal set to the scale (E33, E22, E11) -- the order 0x00BE9AAC..0x00BE9AEE stores for OtherShaderVars with scale 0, the same
    // body as the chunk 2005 SetIdentity with 1.0f.
    void SetScale(float scale) {
        E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f;
        E11 = E22 = E33 = scale;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
    // Nested opt-in (chunk 1016, row 0x00A55F90): Ego_r.pdb / FableWin.pdb ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z
    // (thiscall, out of line at 0x00A55F90; the landed C3DVector::Rotate 0x00A13C60 calls it). A declaration only: no layout change.
    void SetFromVectorAndAngle(const C3DVector& axis,float angle);         // 0x00A55F90
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
    // Retail opt-in (chunk 895, row 0x00BEBAA0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix): Ego_r.pdb / FableWin.pdb
    // ?SetTranslateOnly@CMatrix3x4@@QAEXABVC3DVector@@@Z, inline in retail (no out-of-line copy: the row stores the translation
    // row as three dword copies, 0x00BEBB89..0x00BEBB97). Body only: no layout change, default path unchanged.
#ifdef FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
    // Nested retail opt-in (chunk 2028, row 0x00B8DF50 CEngineOcclusionManager::FrameEnd): FableWin.pdb also has the float overload
    // ?SetTranslateOnly@CMatrix3x4@@QAEXMMM@Z (debug copy 0x02D78EA0; no Ego_r.pdb or Fable.exe copy). The vector form forwards to it:
    // the arguments are evaluated right to left, so the Z is read first, which is what lets VC7.1 drop the dead Z store of a copied
    // vector -- 0x00B8E231..0x00B8E27B: `fld y ; fld x ; rep movsd ; fstp E41 ; fstp E42` and E43 as one dword copy. A reference to
    // memory still gives the three dword copies of 0x00BEBB89..0x00BEBB97, so CalcObjectMatrix keeps its bytes. Default path unchanged.
    void SetTranslateOnly(float x,float y,float z) { E41=x; E42=y; E43=z; }
    void SetTranslateOnly(const C3DVector& translation) { SetTranslateOnly(translation.X,translation.Y,translation.Z); }
#else
    void SetTranslateOnly(const C3DVector& translation) { E41=translation.X; E42=translation.Y; E43=translation.Z; }
#endif
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B
    // Retail opt-in (chunk 2006, row 0x00B399A0 CEngineInternalAttachedPrimitive::UpdateOutputPrimitive): Ego_r.pdb / FableWin.pdb
    // ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z (this = m1 * m2: each rotation row of m1 times m2's rotation, the translation row of m1
    // times m2's rotation plus m2's translation) and ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z written through it on a copy of *this (the
    // `rep movsd`). Both inline in that TU (three expansions, no call). The x87 term order of each sum is VC7.1's own canonical
    // order for this shape (it does not depend on the order written here); this nesting -- a copy local handed to SetMultiply by
    // reference -- is the one that reproduces all 36 sums of the row (a single inline body with the copy inside, or SetMultiply
    // on an explicit caller copy, leave 2-11 of them in another order). FABLE_FORCEINLINE per the owner's 2026-11-01 helper rule
    // (no out-of-line copy of either in retail's TU; 0x00C1CDA0 is the math module's own __fastcall PreMultiply).
    FABLE_FORCEINLINE void SetMultiply(const CMatrix3x4& m1, const CMatrix3x4& m2) {
        E11 = m1.E11 * m2.E11 + m1.E12 * m2.E21 + m1.E13 * m2.E31;
        E12 = m1.E11 * m2.E12 + m1.E12 * m2.E22 + m1.E13 * m2.E32;
        E13 = m1.E11 * m2.E13 + m1.E12 * m2.E23 + m1.E13 * m2.E33;
        E21 = m1.E21 * m2.E11 + m1.E22 * m2.E21 + m1.E23 * m2.E31;
        E22 = m1.E21 * m2.E12 + m1.E22 * m2.E22 + m1.E23 * m2.E32;
        E23 = m1.E21 * m2.E13 + m1.E22 * m2.E23 + m1.E23 * m2.E33;
        E31 = m1.E31 * m2.E11 + m1.E32 * m2.E21 + m1.E33 * m2.E31;
        E32 = m1.E31 * m2.E12 + m1.E32 * m2.E22 + m1.E33 * m2.E32;
        E33 = m1.E31 * m2.E13 + m1.E32 * m2.E23 + m1.E33 * m2.E33;
        E41 = m1.E41 * m2.E11 + m1.E42 * m2.E21 + m1.E43 * m2.E31 + m2.E41;
        E42 = m1.E41 * m2.E12 + m1.E42 * m2.E22 + m1.E43 * m2.E32 + m2.E42;
        E43 = m1.E41 * m2.E13 + m1.E42 * m2.E23 + m1.E43 * m2.E33 + m2.E43;
    }
    FABLE_FORCEINLINE void PreMultiply(const CMatrix3x4& matrix) {
        CMatrix3x4 copy(*this);
        SetMultiply(matrix, copy);
    }
#ifdef FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
    // Nested opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects): FableWin.pdb
    // ?PostMultiply@CMatrix3x4@@QAEXABV1@@Z (this = this * matrix) written through SetMultiply on a copy of *this, the same nesting as
    // PreMultiply above (inline in retail: the `rep movsd` copy at 0x00BF159A, then the twelve sums into the original). Mutually
    // exclusive with FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY.
    FABLE_FORCEINLINE void PostMultiply(const CMatrix3x4& matrix) {
        CMatrix3x4 copy(*this);
        SetMultiply(copy, matrix);
    }
#endif
#endif
#if defined(FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION) && defined(FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B)
    // Retail opt-in (chunk 2014, row 0x00A4E300 C3DAnimation2::GetDeltaMovement): FableWin.pdb ?SetMultiplyNoTranslation@CMatrix3x4@@QAEXABV1@0@Z
    // (FableWin 0x032289C0, emitted right after ?GetDeltaMovement@C3DAnimation2 0x03228780: its first user) -- the 3x3 product
    // m1 * m2 with the translation row cleared (retail 0x00A4E71A..0x00A4E720 stores the three zeros into the result) -- and
    // ?PreScale@CMatrix3x4@@QAEXMMM@Z (FableWin 0x03228C60, after ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix3x4@@_N@Z: each rotation row
    // times its scale, the landed 0x00A52450 order). No retail out-of-line copy of either.
    // FITTED body (chunk 3010, coordinator's choice of the agent's two candidates; needs a ruling): the full product through
    // SetMultiply (FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B), then the translation row cleared. Evidence for: compiled out of line
    // it equals retail's unreferenced copy 0x00A4B820 (240 bytes, as the 9-sum body also does), and it is the only natural body
    // measured that gives GetDeltaMovement 0x00A4E300 retail's 13 x87 operand orders and size (the 9-sum body: DIFFER 1615v1619).
    // Evidence against: FableWin.pdb's debug COMDAT 0x032289C0 is 160 /Od instructions, which fits the 9-sum body (157), not this
    // one (about 15) nor a 12-sum-then-clear text (211); FableWin is a later revision of the source.
    FABLE_FORCEINLINE void SetMultiplyNoTranslation(const CMatrix3x4& m1, const CMatrix3x4& m2) {
        SetMultiply(m1, m2);
        E41 = E42 = E43 = 0.0f;
    }
    // PreScale(MMM) is expanded inside retail's out-of-line C3DKeyframe::ToMatrix 0x00A52450 (plain inline leaves it a call there:
    // 341 v 447 bytes; forced, 447 v 447 relocation-equal), so it is FABLE_FORCEINLINE (FableWin keeps its debug COMDAT
    // 0x03228C60: a real member, not a macro).
    FABLE_FORCEINLINE void PreScale(float x, float y, float z) {
        E11 *= x; E12 *= x; E13 *= x;
        E21 *= y; E22 *= y; E23 *= y;
        E31 *= z; E32 *= z; E33 *= z;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
    // Retail opt-in (chunk 2014, row 0x00A4E300; body chunk 3010): Ego_r.pdb / FableWin.pdb ?Transpose@CMatrix3x4@@QAEXXZ (the 3x3 part
    // in place; no retail out-of-line copy). The upper triangle goes through three named copies: GetDeltaMovement 0x00A4E567..0x00A4E571
    // spills exactly E12 / E13 (`fxch st(1) ; fstp [esp+8] ; fstp [esp+18h]`) and reloads E23 (`fld [esp+84h]`) right after the first
    // ToMatrix. Body (chunk 3010 session 3): three calls of FableWin's ??$SWAP@M@@YIXAAM0@Z (template, __fastcall, by reference,
    // `T temp = a; a = b; b = temp;`): VC7.1 /Od gives Transpose 22 + SWAP<float> 18 instructions = FableWin's debug COMDATs exactly
    // (0x025C21B0 / 0x025C2DA0); in GetDeltaMovement the code and spill set equal the upper-triangle-copies text.
    void Transpose() { SWAP(E12, E21); SWAP(E13, E31); SWAP(E23, E32); }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
    // Chunk 764 (row 0x00B451D0 CEngineLightingManager::CalculateFogTransformMatrix): Ego_r.pdb ??0CMatrix3x4@@QAE@MMMMMMMMMMMM@Z and
    // FableWin.pdb ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z, both inline in retail (no out-of-line copy in Ego_r.pdb or Fable.exe; the
    // copy of *this is the rep movsd). The bodies are the landed same-module sibling SetupDefaultFogStates 0x00B47630 model.
    CMatrix3x4(float e11,float e12,float e13,float e21,float e22,float e23,float e31,float e32,float e33,float e41,float e42,float e43) {
        E11=e11; E12=e12; E13=e13; E21=e21; E22=e22; E23=e23; E31=e31; E32=e32; E33=e33; E41=e41; E42=e42; E43=e43;
    }
    FABLE_FORCEINLINE void PreMultiply(const CMatrix3x4& l) {
        CMatrix3x4 a(*this);
        E11=a.E21*l.E12+a.E31*l.E13+a.E11*l.E11;
        E12=a.E22*l.E12+a.E32*l.E13+a.E12*l.E11;
        E13=a.E23*l.E12+a.E33*l.E13+a.E13*l.E11;
        E21=a.E21*l.E22+a.E31*l.E23+a.E11*l.E21;
        E22=a.E22*l.E22+a.E32*l.E23+a.E12*l.E21;
        E23=a.E23*l.E22+a.E33*l.E23+a.E13*l.E21;
        E31=a.E21*l.E32+a.E31*l.E33+a.E11*l.E31;
        E32=a.E22*l.E32+a.E32*l.E33+a.E12*l.E31;
        E33=a.E23*l.E32+a.E33*l.E33+a.E13*l.E31;
        E41=a.E21*l.E42+a.E31*l.E43+a.E11*l.E41+a.E41;
        E42=a.E22*l.E42+a.E32*l.E43+a.E12*l.E41+a.E42;
        E43=a.E23*l.E42+a.E33*l.E43+a.E13*l.E41+a.E43;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4
    // Retail opt-in (chunk 734, row 0x004A1CB0 CWorld::PreparePrimitivesForEngine): FableWin.pdb inline members (no Ego_r.pdb or retail
    // out-of-line copy) ?Set@CMatrix3x4@@QAEXABVC3DVector@@000@Z (the three axes as rows, then the translation; unused by the row's best source, which assigns E11..E43 directly),
    // ?PreScale@CMatrix3x4@@QAEXM@Z (the 3x3 part only; the translation row is untouched) and ?SetInverse@CMatrix3x4@@QAEXABV1@@Z
    // (the body of the landed 0x00B393D0 model: cofactors over the determinant, then the translation through the inverse).
    // __forceinline as the owner's 2026-11-01 rule for Lionhead math helpers with no out-of-line copy in Ego_r.pdb or Fable.exe:
    // plain inline leaves PreScale and SetInverse called out of line in PreparePrimitivesForEngine (934 v 1330 bytes).
#ifdef FABLE_RETAIL_CMATRIX3X4_SET_ORIGINAL_NAMES
    // Retail opt-in (Codex pass 17): original parameter names and direct field copies, donor0261c5d0.
    FABLE_FORCEINLINE void Set(const C3DVector& right,const C3DVector& forward,const C3DVector& up,const C3DVector& translate) {
        // Fidelity: address X through the original three-float Data row. The local
        // pointer step preserves the native X-before-Z copy order at007767d9.
        float* axis=Data[0]+1;
        *--axis=right.X; E12=right.Y; E13=right.Z;
        E21=forward.X; E22=forward.Y; E23=forward.Z;
        E31=up.X; E32=up.Y; E33=up.Z;
        E41=translate.X; E42=translate.Y; E43=translate.Z;
    }
#else
    FABLE_FORCEINLINE void Set(const C3DVector& x_axis,const C3DVector& y_axis,const C3DVector& z_axis,const C3DVector& translation) {
        E11=x_axis.X; E12=x_axis.Y; E13=x_axis.Z;
        E21=y_axis.X; E22=y_axis.Y; E23=y_axis.Z;
        E31=z_axis.X; E32=z_axis.Y; E33=z_axis.Z;
        E41=translation.X; E42=translation.Y; E43=translation.Z;
    }
#endif
    // Chunk 751 (row 0x007767A0): Ego_r.pdb / FableWin.pdb ?Set@CMatrix3x4@@QAEXABVCRightHandedSet@@ABVC3DVector@@@Z, out of line in
    // retail at 0x007767A0 (the axes from the orientation -- GetRight, Forward, Up -- and the translation through the Set above).
    void Set(const class CRightHandedSet& orientation,const C3DVector& translation);
    FABLE_FORCEINLINE void Set(float e11,float e12,float e13,float e21,float e22,float e23,float e31,float e32,float e33,float e41,float e42,float e43) {
        E11=e11; E12=e12; E13=e13; E21=e21; E22=e22; E23=e23; E31=e31; E32=e32; E33=e33; E41=e41; E42=e42; E43=e43;
    }
    FABLE_FORCEINLINE void PreScale(float scale) {
        E11*=scale; E12*=scale; E13*=scale;
        E21*=scale; E22*=scale; E23*=scale;
        E31*=scale; E32*=scale; E33*=scale;
    }
    // Chunk 742 (0x00BE0EB0 CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive): FableWin.pdb ?PostScale@CMatrix3x4@@QAEXM@Z
    // (all twelve elements, translation included, column by column as retail scales them 0x00BE0FB6..0x00BE102B) and
    // ?PostTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z (E4k += v; the by-reference operand gives retail's `fld E4k ; fadd v` order).
    // No Ego_r.pdb or retail out-of-line copy; __forceinline per the owner's 2026-11-01 helper rule.
    FABLE_FORCEINLINE void PostScale(float scale) {
        E11*=scale; E21*=scale; E31*=scale; E41*=scale;
        E12*=scale; E22*=scale; E32*=scale; E42*=scale;
        E13*=scale; E23*=scale; E33*=scale; E43*=scale;
    }
    FABLE_FORCEINLINE void PostTranslate(const C3DVector& v) {
        E41+=v.X; E42+=v.Y; E43+=v.Z;
    }
    FABLE_FORCEINLINE void SetInverse(const CMatrix3x4& m) {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4
    // Chunk 737 (CPSCLight::PreparePrimitives 0x00B19C60, through CParticleSystem::TransformPosIfWorld): every row in the X row's
    // form, `E1k*xx+(E2k*yy+E3k*zz)+E4k`, gives retail's E3k*z, E2k*y, E1k*x term order in all three rows (0x00B1A43A..0x00B1A4C0:
    // `fld E3k ; fmul z ; fld E2k ; fmul y ; faddp ; fld E1k ; fmul x ; faddp ; fadd E4k`, matrix at esp+0x84);
    // the default accumulated Y / Z rows put E12*xx first there, but are the form the landed 0x00AE0070 ApplyTransform needs (it
    // differs with this one, 1090v1090). Open: one retail body, two contexts (and the parked 0x00B087C0 orders its Z row E33, E13, E23).
    // __forceinline (owner 2026-11-01 helper rule): retail expands it in place in both PreparePrimitives rows and Fable.exe has no
    // out-of-line ?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z (the catalog has none; only the debug PDBs carry a copy); with
    // plain inline VC7.1 calls it out of line twice in 0x00B19C60 (landing 2026-11-03h probe).
#if defined(FABLE_RETAIL_CPARTICLE_SPRITE) || defined(FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR)
    // Donor024543E0: original non-incremental left-associated coordinate sums.
    // FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (chunk 3001, row 0x00A12EE0): the same plain left-to-right rows E1k*x + E2k*y + E3k*z + E4k.
    FABLE_FORCEINLINE void TransformPoint(C3DVector& point) const {
        const float xx=point.X,yy=point.Y,zz=point.Z;
        point.X=E11*xx+E21*yy+E31*zz+E41;
        point.Y=E12*xx+E22*yy+E32*zz+E42;
        point.Z=E13*xx+E23*yy+E33*zz+E43;
    }
#elif defined(FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT)
    // Retail opt-in (Codex pass21): original donor024543E0 scalar captures
    // and left-associated xx*E1k + yy*E2k + zz*E3k + E4k.
    FABLE_FORCEINLINE void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=xx*E11+yy*E21+zz*E31+E41;
        point.Y=xx*E12+yy*E22+zz*E32+E42;
        point.Z=xx*E13+yy*E23+zz*E33+E43;
    }
#elif defined(FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS)
    // Nested opt-in (chunk 758, row 0x00B24400 CEngine::ConvertScreenPosToWorldPosAtViewDistance): retail 0x00B2444C..0x00B244C4 sums the
    // X row as E21*y+E31*z, then E11*x, then E41, but the Y and Z rows left to right (`fld x ; fmul E12 ; .. fmul E22 ; faddp ; ..
    // fmul E32 ; faddp ; fadd E42`, and E13, E23, E33, E43) -- the parenthesised Y / Z rows above give E22*y+E32*z first there (dl 18).
    // A third retail context for the same inline body (see the Open note above).
    FABLE_FORCEINLINE void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx+E22*yy+E32*zz+E42;
        point.Z=E13*xx+E23*yy+E33*zz+E43;
    }
#elif defined(FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS)
    // Nested opt-in (wf3, row 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated): all three rows as plain left-to-right sums in
    // Z, Y, X order, `E3k*zz+E2k*yy+E1k*xx+E4k`. Both expansions there (0x00A89768.. on the bone's LocalizationMatrix, 0x00A89831.. on
    // the animated bone transformation) sum `fld E3k ; fmul z ; fld E2k ; fmul y ; faddp ; fld E1k ; fmul x ; faddp ; fadd E4k` per
    // row. The parenthesised default emits the same instructions there but schedules the epilogue's result-pointer load and
    // `pop esi` four x87 instructions early (389v389); this unparenthesised form is byte-exact. A fourth retail context for the
    // same inline body (see the Open note above). Body only: default path unchanged.
    FABLE_FORCEINLINE void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E31*zz+E21*yy+E11*xx+E41;
        point.Y=E32*zz+E22*yy+E12*xx+E42;
        point.Z=E33*zz+E23*yy+E13*xx+E43;
    }
#else
    FABLE_FORCEINLINE void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx+(E22*yy+E32*zz)+E42;
        point.Z=E13*xx+(E23*yy+E33*zz)+E43;
    }
#endif
#else
    void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx;
        point.Y+=E32*zz;
        point.Y+=E22*yy;
        point.Y+=E42;
        point.Z=E13*xx;
        point.Z+=E33*zz;
        point.Z+=E23*yy;
        point.Z+=E43;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
    // Chunk 756 (row 0x00A11C20 CPhysicsMeshObject::GetBoundingBoxXEdge): FableWin.pdb ?TransformVector@CMatrix3x4@@QBEXAAVC3DVector@@@Z
    // (0x01D9FFF0; no Ego_r.pdb or retail out-of-line copy): the rotation part in place through xx/yy/zz copies, each row
    // x*E1k + y*E2k + z*E3k (the X edge's `fst [esp]` keeps the half extent live for the X row, 0x00A11C3B).
    void TransformVector(C3DVector& v) const {
        const float xx=v.X;
        const float yy=v.Y;
        const float zz=v.Z;
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_GROUPED_X
        // Nested opt-in (chunk 2014 / 3010, row 0x00A4E300 C3DAnimation2::GetDeltaMovement): the X row grouped like TransformPoint's
        // (`E11*x+(E21*y+E31*z)`): GetDeltaMovement 0x00A4E8A5..0x00A4E8BC adds zz*E31 + yy*E21 first and xx*E11 last (the ungrouped
        // row puts the products in another x87 order there, 0x00A4E8AD). Body only: the default and the plain opt-in are unchanged.
        v.X=xx*E11+(yy*E21+zz*E31);
#else
        v.X=xx*E11+yy*E21+zz*E31;
#endif
        v.Y=xx*E12+yy*E22+zz*E32;
        v.Z=xx*E13+yy*E23+zz*E33;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMPLANE
    // Retail opt-in (chunk 3001 / 3020, row 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes): FableWin.pdb
    // ?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z (0x031D2030, 48i/139b at /Od, the FIRST COMDAT FableWin emits after
    // GetNavigationLevelVolumes = its first user), inline in retail (no out-of-line copy). Body FITTED (chunk 3020): this text is 48i/140b
    // at VC7.1 /Od (fwsize; the one-byte gap is the VC10 debug build) and gives retail's x87 operand order in the row: the normal is
    // copied FIRST (its field records are created before the point's, so CPlane::Initialise's dot product loads the point component
    // first, 0x00A13078..0x00A1308A), then the point on the plane (normal * distance), the normal rotated in place
    // (FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR) and renormalised (C3DVector::AccurateNormalise, out of line 0x00A14440; the returned
    // reference copied back, 0x00A13003..0x00A13025), the point transformed, the plane rebuilt through CPlane::Initialise(normal,
    // normal . point) (0x00A42140). Point first (the other order) leaves the Dot product operands swapped (10 lines).
    // FABLE_FORCEINLINE: retail expands it; VC7.1 calls the plain inline out of line (887-byte row, grade 21 of chunk 3020).
    // Needs FABLE_RETAIL_CMATRIX3X4 (TransformPoint), FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR, FABLE_RETAIL_CPLANE_NORMAL_DISTANCE,
    // FABLE_RETAIL_CPLANE_ACCESSORS and FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE.
    // Inline-budget: TransformPlane plain inline is refused at the row's site (grade 21: called, 887v1144); forced it expands (1144v1144).
    FABLE_FORCEINLINE void TransformPlane(CPlane& plane) const {
        C3DVector normal=plane.GetNormal();
        C3DVector point=plane.GetNormal()*plane.GetDistance();
        TransformVector(normal);
        normal=normal.AccurateNormalise();
        TransformPoint(point);
        plane.Initialise(normal,normal.Dot(point));
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
    // Retail opt-in (wf5, row 0x00A11D10 CPhysicsMeshObject::GetNavigationLevelAt): FableWin.pdb
    // ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z (debug copy 0x01D129F0, 164 bytes / 61 instructions = three plain
    // 18-instruction row sums; no Ego_r.pdb or retail out-of-line copy), each row one left-to-right sum
    // `E1k*x + E2k*y + E3k*z + E4k`. Expanded in place after the inline SetInverse it gives retail's `fld E3k ; fmul z ; fld E2k ;
    // fmul y ; faddp ; fld E1k ; fmul x ; faddp ; fadd E4k` rows (0x00A11E46..0x00A11E9F) and, through them, the 1,3,2 term order
    // of the inverse's determinant and translation sums. __forceinline (owner 2026-11-01 helper rule): retail expands it in place
    // and has no out-of-line copy; the plain-inline default below graded 388 v 422 in that row. The default path is unchanged.
    // Row 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox (same workflow, a different author agent) arrived at the
    // same three lines independently and matches with them too (386 v 428 with the plain-inline default).
    FABLE_FORCEINLINE void TransformPoint(const C3DVector& pSource,C3DVector& destination) const {
        destination.X=E11*pSource.X+E21*pSource.Y+E31*pSource.Z+E41;
        destination.Y=E12*pSource.X+E22*pSource.Y+E32*pSource.Z+E42;
        destination.Z=E13*pSource.X+E23*pSource.Y+E33*pSource.Z+E43;
    }
#elif defined(FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_COPY_SUMS)
    // Retail opt-in (chunk 2028, row 0x00B8DF50 CEngineOcclusionManager::FrameEnd, through the inline CEngineCamera::TransformWorldToView):
    // FableWin.pdb ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z has no Ego_r.pdb or Fable.exe out-of-line copy (no reloc binding);
    // retail expands it and keeps only the Z row: 0x00B8E2CB..0x00B8E2EA `fld E33 ; fmul z ; fld E13 ; fmul x ; faddp ; fld E23 ; fmul y ;
    // faddp ; fadd E43`. Each row accumulated from its Z term, then X, then Y gives that order after the row's earlier reads of the same
    // point (the translation copy at 0x00B8E231); the default body below (E13*x, then E33*z) loads E13*x first there and one-line row sums
    // load E23*y second (both graded, one-swap residues). Default path unchanged.
    // FABLE_FORCEINLINE per the owner's 2026-11-01 helper rule (plain inline leaves it called in that row).
    FABLE_FORCEINLINE void TransformPoint(const C3DVector& pSource,C3DVector& destination) const {
        destination.X=E31*pSource.Z;
        destination.X+=E11*pSource.X;
        destination.X+=E21*pSource.Y;
        destination.X+=E41;
        destination.Y=E32*pSource.Z;
        destination.Y+=E12*pSource.X;
        destination.Y+=E22*pSource.Y;
        destination.Y+=E42;
        destination.Z=E33*pSource.Z;
        destination.Z+=E13*pSource.X;
        destination.Z+=E23*pSource.Y;
        destination.Z+=E43;
    }
#else
    void TransformPoint(const C3DVector& pSource,C3DVector& destination) const {
        destination.X=E11*pSource.X+(E21*pSource.Y+E31*pSource.Z)+E41;
        destination.Y=E12*pSource.X;
        destination.Y+=E32*pSource.Z;
        destination.Y+=E22*pSource.Y;
        destination.Y+=E42;
        destination.Z=E13*pSource.X;
        destination.Z+=E33*pSource.Z;
        destination.Z+=E23*pSource.Y;
        destination.Z+=E43;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
    // Retail opt-in (chunk Q21, row 0x00B39080 CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh): FableWin.pdb
    // ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z (this = a * b; no Ego_r.pdb or retail out-of-line copy: the row expands it over a named copy
    // of the target, 0x00B3915B `rep movsd` then the k-order sums 0x00B39157..0x00B3930E). Same sums as PostMultiply below, with both
    // operands as parameters: with the copy inside PostMultiply VC7.1 swaps the operand order of the row-2..4 products (`fld b ; fmul a`).
    FABLE_FORCEINLINE void SetMultiply(const CMatrix3x4& a, const CMatrix3x4& b) {
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
        E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32 + b.E42;
        E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
    }
#endif
#ifdef FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY
    // Retail opt-in (chunk 794, row 0x007DE710 CTCWallMount::AttachObject): FableWin.pdb ?PostMultiply@CMatrix3x4@@QAEXABV1@@Z (this =
    // this * matrix; the landed out-of-line copy 0x00C1CF20 gives the k-order sums) and
    // ?SetInverseOfUniformScaleRotationTranslation@CMatrix3x4@@QAEMABV1@@Z (FableWin debug copy 0x0261C690; no retail out-of-line copy:
    // 0x007DE710 expands it -- one reciprocal of the X row's squared length (`fdivr [1.0f]`), the transposed 3x3 times it, then the
    // translation through the result). The copy of *this is implicit (AttachObject's `rep movsd` of 12 dwords).
    FABLE_FORCEINLINE void PostMultiply(const CMatrix3x4& matrix) {
        CMatrix3x4 a(*this);
        E11 = a.E11 * matrix.E11 + a.E12 * matrix.E21 + a.E13 * matrix.E31;
        E12 = a.E11 * matrix.E12 + a.E12 * matrix.E22 + a.E13 * matrix.E32;
        E13 = a.E11 * matrix.E13 + a.E12 * matrix.E23 + a.E13 * matrix.E33;
        E21 = a.E21 * matrix.E11 + a.E22 * matrix.E21 + a.E23 * matrix.E31;
        E22 = a.E21 * matrix.E12 + a.E22 * matrix.E22 + a.E23 * matrix.E32;
        E23 = a.E21 * matrix.E13 + a.E22 * matrix.E23 + a.E23 * matrix.E33;
        E31 = a.E31 * matrix.E11 + a.E32 * matrix.E21 + a.E33 * matrix.E31;
        E32 = a.E31 * matrix.E12 + a.E32 * matrix.E22 + a.E33 * matrix.E32;
        E33 = a.E31 * matrix.E13 + a.E32 * matrix.E23 + a.E33 * matrix.E33;
        E41 = a.E41 * matrix.E11 + a.E42 * matrix.E21 + a.E43 * matrix.E31 + matrix.E41;
        E42 = a.E41 * matrix.E12 + a.E42 * matrix.E22 + a.E43 * matrix.E32 + matrix.E42;
        E43 = a.E41 * matrix.E13 + a.E42 * matrix.E23 + a.E43 * matrix.E33 + matrix.E43;
    }
    FABLE_FORCEINLINE float SetInverseOfUniformScaleRotationTranslation(const CMatrix3x4& m) {
        float scale = 1.0f / (m.E11 * m.E11 + m.E12 * m.E12 + m.E13 * m.E13);
        E11 = m.E11 * scale; E21 = m.E12 * scale; E31 = m.E13 * scale;
        E12 = m.E21 * scale; E22 = m.E22 * scale; E32 = m.E23 * scale;
        E13 = m.E31 * scale; E23 = m.E32 * scale; E33 = m.E33 * scale;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
        return scale;
    }
#endif
};
#endif
