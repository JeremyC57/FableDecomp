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

    /// 0x00A14510 — exact normalise (catalogued upstream as C2DVector::Dot).
    void Normalise() noexcept;
    /// 0x00A14540 — table-estimate normalise; returns this.
    C2DVector& FastNormalise() noexcept;
};

struct CMatrix3x4;

struct C3DVector {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;

    /// 0x00A14440 — exact normalise (catalogued upstream as GetScaled).
    void Normalise() noexcept;
    /// 0x00A14480 — exact normalise returning the previous length at register
    /// precision (catalogued upstream as GetScaled).
    double NormaliseAndGetLength() noexcept;
    /// 0x00A13C60 — rotate about `axis` by `angle` turns (via
    /// Matrix_RotationAroundAxis, which uses x87 fsin/fcos).
    void Rotate(const C3DVector& axis, float angle) noexcept;
};

struct C4DVector {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
    float W = 0.0f;
};

/// Rows E1x..E3x are the rotation/scale basis, E4x the translation
/// (row-vector convention: p' = p * M).
struct CMatrix3x4 {
    float E11 = 0.0f, E12 = 0.0f, E13 = 0.0f;
    float E21 = 0.0f, E22 = 0.0f, E23 = 0.0f;
    float E31 = 0.0f, E32 = 0.0f, E33 = 0.0f;
    float E41 = 0.0f, E42 = 0.0f, E43 = 0.0f;

    /// 0x00A55D80 — cross-product matrix of v; translation zeroed.
    void InitialiseSkewedSymmetric(const C3DVector& v) noexcept;
    /// 0x00A55DF0 — affine product this * rhs (rhs translation added).
    [[nodiscard]] CMatrix3x4 operator*(const CMatrix3x4& rhs) const noexcept;
    /// 0x00C1CF20 — in-place product (retail passes rhs in EDX; catalogued
    /// upstream as an operator* returning C3DVector).
    CMatrix3x4& operator*=(const CMatrix3x4& rhs) noexcept;
    /// 0x00A560A0 — each row's squared deviation from identity <= (1e-4)^2.
    /// NaN rows count as identical (retail flag test).
    [[nodiscard]] bool IsIdentity() const noexcept;
    /// 0x00A56180 — IsIdentity(rhs - this + I).
    [[nodiscard]] bool Equals(const CMatrix3x4& rhs) const noexcept;
    /// 0x00A56270 — Gram-Schmidt via cross products with the engine's fast
    /// inverse square root; zeroes the translation.
    void Orthonormalise() noexcept;
    /// 0x00A56530 — basis rows dotted with v (no translation).
    [[nodiscard]] C3DVector operator*(const C3DVector& v) const noexcept;
    /// 0x00A55F90 Matrix_RotationAroundAxis — rotation of `angle` turns about
    /// a unit axis; translation zeroed. Uses libm sin/cos for the retail x87
    /// fsin/fcos (identical except in rare last-bit cases).
    void InitialiseRotation(const C3DVector& axis, float angle) noexcept;
};

/// Plane n.p = Distance.
struct CPlane {
    C3DVector Normal;
    float Distance = 0.0f;

    /// 0x00A42140
    bool Initialise(const C3DVector& normal, float distance) noexcept;
    /// 0x00A42170 — plane through three points (edges normalised first).
    bool Initialise(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2) noexcept;
    /// 0x00A42280 (not in the upstream catalogue) — plane through `point`
    /// with `normal` (normalised).
    bool InitialiseFromPointAndNormal(const C3DVector& point, const C3DVector& normal) noexcept;
    /// 0x00A422C0 — intersection of the line point + t*dir; false if parallel.
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& dir, C3DVector& out) const noexcept;
    /// 0x00A42370 (not in the upstream catalogue) — as above against the
    /// plane shifted by `offset`, then pulled back by 1e-4.
    bool GetIntersectionWithLineOffset(const C3DVector& point, const C3DVector& dir, float offset,
                                       C3DVector& out) const noexcept;
    /// 0x00A42430 (not in the upstream catalogue) — |n.p - d| < maxDistance;
    /// writes the distance.
    bool IsWithinDistance(const C3DVector& point, float maxDistance, float& distance) const noexcept;
    /// 0x00A42470 — the two points where the triangle's edges cross the plane.
    /// Returns false only if no pair of vertices lies strictly on one side.
    bool GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& out1,
                                     C3DVector& out2) const noexcept;
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
    /// 0x00A9D480 — Data[r][0..2] *= s[r] (catalogued upstream as
    /// CMatrix3x4::PostScale; indices prove a 3x4 row-of-four layout).
    void ScaleRows(const C3DVector& s) noexcept;
    /// 0x00A9D580 — Data[r][c] *= s[c] for c < 3 (also catalogued as PostScale).
    void ScaleColumns(const C3DVector& s) noexcept;
};

namespace math {

/// Number of entries in the engine cosine table (one full turn).
inline constexpr int kCosineTableSize = 1024;

/// The engine's cosine table: entry j = cos(2*pi*j/1024) as float, plus a
/// wrap-around copy of entry 0 at index 1024 (retail 0x013CD550, filled by
/// Math_InitializeCosineLookup @ 0x00A0DB60).
[[nodiscard]] const std::array<float, kCosineTableSize + 1>& CosineTable() noexcept;

/// 128-entry mantissa table for the fast inverse square root (retail
/// 0x013CE558, first loop of Math_InitializeCosineLookup).
[[nodiscard]] const std::array<std::uint32_t, 128>& InvSqrtTable() noexcept;

/// Table estimate of 1/sqrt(x): exponent arithmetic + 7 mantissa bits.
[[nodiscard]] float InvSqrtEstimate(float x) noexcept;

/// The engine's inlined fast 1/sqrt(x): estimate plus one Newton step,
/// returned at register precision.
[[nodiscard]] double FastInvSqrt(float x) noexcept;

/// Interpolated table cosine of `x` table steps, kept at register precision
/// (the inlined lookup sequence used throughout the engine).
[[nodiscard]] double TableCos(float x) noexcept;
/// Same lookup shifted a quarter turn: sine of `x` table steps.
[[nodiscard]] double TableSin(float x) noexcept;

} // namespace math
} // namespace fable
