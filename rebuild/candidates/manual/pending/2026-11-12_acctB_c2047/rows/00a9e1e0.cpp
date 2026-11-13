// C3DAnimationManager::ApplyParentTransformations(const C3DKeyframe*, CMatrix4x4*, const C3DMesh2&, bool) const -- bone keyframes to bone matrices, each concatenated with its parent's (transposed or row-vector form)
// Leaf: ?ApplyParentTransformations@C3DAnimationManager@@ABEXPBVC3DKeyframe@@PAVCMatrix4x4@@ABVC3DMesh2@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9E1E0, 2472 bytes (0x00A9E1E0..0x00A9EB85 `ret 10h`, next function 0x00A9EB90), thiscall
//   (`this` unused: ecx is overwritten by the bool at 0x00A9E1EC), 16-byte realigned frame (`and esp,-16`).
// Identity: Ego_r.pdb (0x00BE1D40) and FableWin.pdb (0x033825F0) ?ApplyParentTransformations@C3DAnimationManager@@ABEXPBVC3DKeyframe@@
//   PAVCMatrix4x4@@ABVC3DMesh2@@_N@Z (private const); callers 0x00A9FE01 (CalcMeshTransformations 0x00A9F540, landed-pending row whose
//   declaration this is) and 0x00AA09B7 (CalcBlendedMeshTransformations 0x00AA0090), four arguments = `ret 10h`. Parameter names are
//   descriptive (the PDB name sets carry none); `transpose` follows the caller rows.
// Callees: none (everything is expanded). Globals: g_SSEAvailable 0x013D2880 (?g_SSEAvailable@@3_NA, bound), 0.0f 0x0122DEDC,
//   1.0f 0x0122DED8, 2.0f 0x0122DCDC.
// Shape: FableWin.pdb lists ?ToMatrixTransposed@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z and ?ToMatrixTransposed@CQuaternion@@QBEXAAVCMatrix4x4@@
//   ABVC3DVector@@@Z right after this function (their first user), and ?ToMatrix@C3DKeyframe / ?ToMatrix@CQuaternion (4x4) after
//   CalcBoneTransformation in the same TU. Per bone: the parent index (C3DMesh2::GetBone(i)->GetParent(), Bones +0x9C, 0x3C stride,
//   Parent +4); a root (parent < 0) converts its keyframe straight into matrices[i]; any other bone converts into a 16-byte-aligned
//   local and concatenates it with its parent's matrix through CMatrix4x4::AlignedSetMultiply (the SSE kernel when g_SSEAvailable,
//   else the x87 row loop). The transposed loop uses ToMatrixTransposed + AlignedSetMultiplyTransposed(local, parent) = parent * local
//   (SSE operands 0x00A9E515..0x00A9E530: eax = parent, edx = local), the row-vector loop ToMatrix + AlignedSetMultiply(local, parent)
//   (0x00A9E9B9..0x00A9E9D4: eax = local, edx = parent). One loop counter `i` at function scope serves both loops and the bone pointer
//   is a named local: with `for (unsigned long i ...)` per loop the row-vector x87 loop's induction pointer starts at element 1 and its
//   column sums take another order (record-id keyed linearizer, see NOTES.md); both natural names give retail's exact orders.
// Inline bodies proven against retail's unreferenced out-of-line copies (free q.py checks, probe/ool/): C3DKeyframe::ToMatrix (4x4)
//   0x00AA39A0 and C3DKeyframe::ToMatrixTransposed 0x00AA3AF0 RELOCATION_MATCH with the overlay bodies (forced quaternion conversions,
//   no flag); GetParent 0x00A4BD30 / GetBone 0x00A4BD70 (r3010 overlay). AlignedSetMultiplyTransposed = AlignedSetMultiply(b, a):
//   FableWin debug COMDAT 29 bytes / 13 instructions = VC7.1 /Od of this body exactly. The AlignedSetMultiply x87 arm reads through the
//   CMatrix4x4 element accessors (FableWin emits ??RCMatrix4x4@@QBE?BMHH@Z / ??RCMatrix4x4@@QAEAAMHH@Z right after AlignedSetMultiply):
//   with the old `v +=` body both x87 loops take non-retail column orders.
// Inline-budget: FABLE_FORCEINLINE on CQuaternion::ToMatrix / ToMatrixTransposed (4x4) (FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE),
//   C3DKeyframe::ToMatrix / ToMatrixTransposed (4x4) (FABLE_RETAIL_C3DKEYFRAME_TOMATRIX4_INLINE) and
//   CMatrix4x4::AlignedSetMultiplyTransposed (FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE). c2 inline trace (tools/inl.sh):
//   plain, the four quaternion conversions score 283 / 286 (default threshold 74) and stay calls (845 bytes); with them forced the
//   C3DKeyframe wrappers score 297..313 and stay calls (1197 bytes); AlignedSetMultiplyTransposed (c1 431, under the 450 cap) scores 592.
//   Retail expands all of them here, and its out-of-line C3DKeyframe copies 0x00AA39A0 / 0x00AA3AF0 expand the conversions (they match
//   with the forced conversions and no flag; plain they need /d2inlT308). The flag alternative /d2inlT593 gives identical bytes.
// Retail-asm: 0x00A9E528 `mov eax,[esp+28h] ; mov edx,[esp+2Ch] ; mov edi,[esp+30h]` (pointers re-read from the slots just stored at
//   0x00A9E515..0x00A9E524) + 0x00A9E534 `movaps xmm0,[edx]` .. 0x00A9E54F `shufps xmm4,xmm4,0` .. 0x00A9E615 `movaps [edi+30h],xmm4`,
//   and the same block at 0x00A9E9CC..0x00A9EAB9 -- the shared engine/CMatrix4x4AlignedSSE.h GF_SSE_MATRIX_MULTIPLY kernel inside the
//   inline CMatrix4x4::AlignedSetMultiply (as the chunk-748 rows 0x00989230 / 0x00AE1320 / 0x00AE29D0 / 0x00988E80).
//   Per-site attempts (ASM_RULES rule 4; fragment harness probe/asm/gen.py + run.py + table.py, table in probe/asm/table.md and NOTES.md):
//   each spelling compiled as a fragment fed the site's own inputs (site A 0x00A9E528: dst = matrices[i], a = matrices[parent], b = the
//   aligned local from ToMatrixTransposed; site B 0x00A9E9CC: a = the local from ToMatrix, b = matrices[parent]), VC7.1 RTM 13.10.3077
//   /O2 /Oy /W3 and + /arch:SSE, /arch:SSE2: 35 plain C++ spellings (nested / unrolled / accessor / pointer-walk / broadcast / pairwise
//   add-tree / temporaries / memcpy / helpers / SIMD-emulating structs / retail's homed pointer locals ...) emit x87 fmul / fadd only (scalar
//   mulss / addss under /arch:SSE and SSE2), never a packed instruction; 13 intrinsic spellings (_mm_load_ps / _mm_shuffle_ps / _mm_mul_ps /
//   _mm_add_ps kernel unrolled or looped, _mm_set1_ps / _mm_load_ps1 / unpack broadcasts, unaligned and streaming forms, homed pointer
//   locals, __m128 unions, chained copies) emit packed SSE with the compiler's own xmm numbering and scheduling and never re-read the
//   homed pointers. Only the macro (the control fragment) gives retail's block, so the shared helper stays.
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CQUATERNION_MATRIX4
#define FABLE_RETAIL_CQUATERNION_MATRIX4_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CQUATERNION_MATRIX4_TRANSPOSED
#define FABLE_RETAIL_C3DKEYFRAME_TOMATRIX4_INLINE
#define FABLE_RETAIL_C3DMESH2_BONECOUNT
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_C3DBONE2_PEEK
#include "engine/CMatrix4x4.h"
#include "engine/C3DKeyframe.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DAnimationManager.h"

void C3DAnimationManager::ApplyParentTransformations(const C3DKeyframe* keyframes, CMatrix4x4* matrices, const C3DMesh2& mesh, bool transpose) const
{
    unsigned long bone_count = mesh.GetBoneCount();
    unsigned long i;
    if (transpose)
    {
        for (i = 0; i < bone_count; ++i)
        {
            const C3DBone2* bone = mesh.GetBone(i);
            long parent = bone->GetParent();
            if (parent < 0)
                keyframes[i].ToMatrixTransposed(matrices[i], false);
            else
            {
                __declspec(align(16)) CMatrix4x4 local;
                keyframes[i].ToMatrixTransposed(local, false);
                matrices[i].AlignedSetMultiplyTransposed(local, matrices[parent]);
            }
        }
    }
    else
    {
        for (i = 0; i < bone_count; ++i)
        {
            const C3DBone2* bone = mesh.GetBone(i);
            long parent = bone->GetParent();
            if (parent < 0)
                keyframes[i].ToMatrix(matrices[i], false);
            else
            {
                __declspec(align(16)) CMatrix4x4 local;
                keyframes[i].ToMatrix(local, false);
                matrices[i].AlignedSetMultiply(local, matrices[parent]);
            }
        }
    }
}
