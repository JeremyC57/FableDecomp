#ifndef FABLE_ENGINE_CMATRIX3X4_H
#define FABLE_ENGINE_CMATRIX3X4_H
// CMatrix3x4 (Ego_r TPI: a union whose struct arm is float E11..E43, rows of three, size 0x30; struct_layouts_egor.tsv
// CMatrix3x4_u_0_s_0). The flat members below are that struct arm (identical layout). TransformPoint overloads: FableWin
// ?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z (0x024543E0) and ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z
// (0x01D129F0), inline in retail; the bodies (in-place copy to xx/yy/zz locals, X as one expression, Y / Z accumulated) are the
// forms 0x00AE0070 ApplyTransform expands (Codex pass 12, FableWin locals xx/yy/zz).
#include "engine/C3DVector.h"
class CMatrix3x4 {
public:
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    CMatrix3x4() { }
#ifdef FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
    // Chunk 764 (row 0x00B451D0 CEngineLightingManager::CalculateFogTransformMatrix): Ego_r.pdb ??0CMatrix3x4@@QAE@MMMMMMMMMMMM@Z and
    // FableWin.pdb ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z, both inline in retail (no out-of-line copy in Ego_r.pdb or Fable.exe; the
    // copy of *this is the rep movsd). The bodies are the landed same-module sibling SetupDefaultFogStates 0x00B47630 model.
    CMatrix3x4(float e11,float e12,float e13,float e21,float e22,float e23,float e31,float e32,float e33,float e41,float e42,float e43) {
        E11=e11; E12=e12; E13=e13; E21=e21; E22=e22; E23=e23; E31=e31; E32=e32; E33=e33; E41=e41; E42=e42; E43=e43;
    }
    __forceinline void PreMultiply(const CMatrix3x4& l) {
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
    __forceinline void Set(const C3DVector& x_axis,const C3DVector& y_axis,const C3DVector& z_axis,const C3DVector& translation) {
        E11=x_axis.X; E12=x_axis.Y; E13=x_axis.Z;
        E21=y_axis.X; E22=y_axis.Y; E23=y_axis.Z;
        E31=z_axis.X; E32=z_axis.Y; E33=z_axis.Z;
        E41=translation.X; E42=translation.Y; E43=translation.Z;
    }
    // Chunk 751 (row 0x007767A0): Ego_r.pdb / FableWin.pdb ?Set@CMatrix3x4@@QAEXABVCRightHandedSet@@ABVC3DVector@@@Z, out of line in
    // retail at 0x007767A0 (the axes from the orientation -- GetRight, Forward, Up -- and the translation through the Set above).
    void Set(const class CRightHandedSet& orientation,const C3DVector& translation);
    __forceinline void Set(float e11,float e12,float e13,float e21,float e22,float e23,float e31,float e32,float e33,float e41,float e42,float e43) {
        E11=e11; E12=e12; E13=e13; E21=e21; E22=e22; E23=e23; E31=e31; E32=e32; E33=e33; E41=e41; E42=e42; E43=e43;
    }
    __forceinline void PreScale(float scale) {
        E11*=scale; E12*=scale; E13*=scale;
        E21*=scale; E22*=scale; E23*=scale;
        E31*=scale; E32*=scale; E33*=scale;
    }
    // Chunk 742 (0x00BE0EB0 CEnginePrimitiveManager2DParticleGroup::RenderSubPrimitive): FableWin.pdb ?PostScale@CMatrix3x4@@QAEXM@Z
    // (all twelve elements, translation included, column by column as retail scales them 0x00BE0FB6..0x00BE102B) and
    // ?PostTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z (E4k += v; the by-reference operand gives retail's `fld E4k ; fadd v` order).
    // No Ego_r.pdb or retail out-of-line copy; __forceinline per the owner's 2026-11-01 helper rule.
    __forceinline void PostScale(float scale) {
        E11*=scale; E21*=scale; E31*=scale; E41*=scale;
        E12*=scale; E22*=scale; E32*=scale; E42*=scale;
        E13*=scale; E23*=scale; E33*=scale; E43*=scale;
    }
    __forceinline void PostTranslate(const C3DVector& v) {
        E41+=v.X; E42+=v.Y; E43+=v.Z;
    }
    __forceinline void SetInverse(const CMatrix3x4& m) {
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
#ifdef FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
    // Nested opt-in (chunk 758, row 0x00B24400 CEngine::ConvertScreenPosToWorldPosAtViewDistance): retail 0x00B2444C..0x00B244C4 sums the
    // X row as E21*y+E31*z, then E11*x, then E41, but the Y and Z rows left to right (`fld x ; fmul E12 ; .. fmul E22 ; faddp ; ..
    // fmul E32 ; faddp ; fadd E42`, and E13, E23, E33, E43) -- the parenthesised Y / Z rows above give E22*y+E32*z first there (dl 18).
    // A third retail context for the same inline body (see the Open note above).
    __forceinline void TransformPoint(C3DVector& point) const {
        const float xx=point.X;
        const float yy=point.Y;
        const float zz=point.Z;
        point.X=E11*xx+(E21*yy+E31*zz)+E41;
        point.Y=E12*xx+E22*yy+E32*zz+E42;
        point.Z=E13*xx+E23*yy+E33*zz+E43;
    }
#else
    __forceinline void TransformPoint(C3DVector& point) const {
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
        v.X=xx*E11+yy*E21+zz*E31;
        v.Y=xx*E12+yy*E22+zz*E32;
        v.Z=xx*E13+yy*E23+zz*E33;
    }
#endif
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
};
#endif
