// CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale) -- static: the mesh's object matrix from an orientation frame: rows -Right*scale, -Forward*scale, Up*scale and the position as translation
// Leaf: ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEBAA0, 257 bytes, static `__fastcall` (matrix in ecx, pos in edx, rhset and scale on the
//   stack), `ret 8`. Ego_r.pdb / FableWin.pdb ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z.
// Headers: the shared engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR: operator^ and operator*=), engine/CRightHandedSet.h
//   (FABLE_RETAIL_CRIGHTHANDEDSET: GetForward / GetUp and GetRight = Forward ^ Up, the landed 0x0049CE00 body expanded here) and
//   engine/CMatrix3x4.h with the new FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY opt-in (hdr/README.md: PDB SetTranslateOnly(const C3DVector&),
//   inline in retail). The shared one-line operator^ gives retail's commutative operand order in every product (the z term's
//   `fld [eax+10h] ; fmul [eax]`, 0x00BEBAE3) where the October draft's hand-written Cross with named locals flipped it.
// Shape: copies of Forward, Right (= Forward ^ Up) and Up, each scaled in place, then the matrix rows stored negated / plain and
//   the translation. The three parenthesised negations of the Right row (`matrix.E11 = (-right.X);` ...) are three no-code
//   PAREN tuples in VC7.1's 81-tuple scheduling region: with them the region cut falls where retail's does and `pop esi` stays
//   in the epilogue (0x00BEBB9A); without them the scheduler hoists it after the last esi copy (0x00BEBACE, 52 lines). Three
//   parentheses on the Forward row work too; one is not enough.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CRightHandedSet.h"
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#include "engine/CMatrix3x4.h"
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale);
};

void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.GetForward();
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.GetUp();
    right *= scale;
    forward *= scale;
    up *= scale;
    matrix.E11 = (-right.X);
    matrix.E12 = (-right.Y);
    matrix.E13 = (-right.Z);
    matrix.E21 = -forward.X;
    matrix.E22 = -forward.Y;
    matrix.E23 = -forward.Z;
    matrix.E31 = up.X;
    matrix.E32 = up.Y;
    matrix.E33 = up.Z;
    matrix.SetTranslateOnly(pos);
}