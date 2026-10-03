#pragma once

// Portable reconstruction of Fable's core math types.
//
// Layouts follow the retail PDB (ghidra_out/struct_layouts_egor.tsv); member
// functions carry the retail address they reproduce. Every function listed
// here is checked bit-for-bit against the retail code by
// rebuild/modern/oracle/tests/CoreMathOracleTests.cpp.

#include <array>
#include <cstdint>

namespace fable {

struct C2DVector {
    float X = 0.0f;
    float Y = 0.0f;
};

struct C3DVector {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
};

struct C4DVector {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    float W = 0.0f;
};

/// Rows E1x..E3x are the rotation/scale basis, E4x the translation.
struct CMatrix3x4 {
    float E11 = 0.0f, E12 = 0.0f, E13 = 0.0f;
    float E21 = 0.0f, E22 = 0.0f, E23 = 0.0f;
    float E31 = 0.0f, E32 = 0.0f, E33 = 0.0f;
    float E41 = 0.0f, E42 = 0.0f, E43 = 0.0f;
};

/// Direct3D-compatible row-major 4x4 matrix.
struct CMatrix4x4 {
    std::array<float, 16> M{};
};

struct CQuaternion {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    float W = 1.0f;

    /// 0x00A88B60
    [[nodiscard]] CQuaternion operator*(const CQuaternion& rhs) const noexcept;
    /// 0x00A88C10
    CQuaternion& operator*=(const CQuaternion& rhs) noexcept;
    /// 0x00A88C50 — every component within 1e-4.
    [[nodiscard]] bool Equals(const CQuaternion& rhs) const noexcept;
    /// 0x00A88CB0 — (0,0,0,1) within 1e-4.
    [[nodiscard]] bool IsIdentity() const noexcept;
    /// 0x00A88D10 — angle in engine angle units (1.0 = one full turn), using
    /// the engine cosine table rather than libm.
    void SetFromAxisAngle(const C3DVector& axis, float angle) noexcept;
};

/// One animation key: rotation, position and scale (catalogued upstream as
/// CQuaternion methods; the layout proves the receiver is C3DKeyframe).
struct C3DKeyframe {
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;

    /// 0x00A52450 — rotation + translation, optionally scaled per row.
    void ToMatrix(CMatrix3x4& out, bool applyScaling) const noexcept;
    /// 0x00AA39A0 — rotation + translation as a 4x4; scale is ignored and the
    /// second retail argument is unused.
    void ToMatrix(CMatrix4x4& out) const noexcept;
};

/// Skinning matrix stored transposed: three rows of four.
struct CPreTransposedBoneMatrix {
    float Data[3][4] = {};

    /// 0x00987BF0 — out = M * (in.xyz, w); out.W = w.
    void Transform(const C4DVector& in, C4DVector& out, float w) const noexcept;
    /// 0x00ADDFE0 — in-place variant of Transform.
    void TransformInPlace(C4DVector& v, float w) const noexcept;
};

namespace math {

/// Number of entries in the engine cosine table (one full turn).
inline constexpr int kCosineTableSize = 1024;

/// The engine's cosine table: entry j = cos(2*pi*j/1024) as float, plus a
/// wrap-around copy of entry 0 at index 1024 (retail 0x013CD550, filled by
/// Math_InitializeCosineLookup @ 0x00A0DB60).
[[nodiscard]] const std::array<float, kCosineTableSize + 1>& CosineTable() noexcept;

/// Interpolated table cosine of `x` table steps, kept at register precision
/// (the inlined lookup sequence used throughout the engine).
[[nodiscard]] double TableCos(float x) noexcept;
/// Same lookup shifted a quarter turn: sine of `x` table steps.
[[nodiscard]] double TableSin(float x) noexcept;

} // namespace math
} // namespace fable
