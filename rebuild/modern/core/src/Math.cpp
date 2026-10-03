// Bit-exact portable reconstruction of retail Fable.exe math routines.
// Comments name the retail address; `store()` marks every retail `fstp DWORD`.

#include "fable/core/Math.hpp"

#include "fable/core/X87.hpp"

#include <cmath>

namespace fable {
namespace {

using x87::store;

constexpr double kEpsilon = static_cast<double>(1.0e-4f);  // retail 0x0129BA3C
constexpr float kPiOver1024 = 0.0030679617f;               // retail 0x0129CA40 (0x3B490FDB)

// Shared inlined lookup: x87 floor via fistp(x - 0.5), linear interpolation.
double tableLookup(float x, std::uint32_t quarterShift) noexcept {
    const auto& table = math::CosineTable();
    const float xm = store(static_cast<double>(x) - 0.5);
    const std::int32_t i = x87::fistp(xm);
    const double frac = static_cast<double>(x) - static_cast<double>(i);
    const std::uint32_t idx = (static_cast<std::uint32_t>(i) - quarterShift) & 0x3FFU;
    return (1.0 - frac) * table[idx] + frac * table[idx + 1];
}

// Products and the sum order shared by both C3DKeyframe::ToMatrix variants.
struct RotationTerms {
    float m00, m01, m02, m10, m11, m12, m20, m21, m22;
};

RotationTerms rotationTerms(const CQuaternion& q) noexcept {
    const double X = q.X, Y = q.Y, Z = q.Z, W = q.W;
    const float len2 = store(((W * W + Z * Z) + Y * Y) + X * X);
    const double s = (len2 == 0.0f) ? 1.0 : 2.0 / static_cast<double>(len2);

    const double sX = s * X;  // stays in a register
    const float ys = store(s * Y);
    const float zs = store(s * Z);
    const float wx = store(sX * W);
    const float wy = store(ys * W);
    const float wz = store(zs * W);
    const double xx = sX * X;
    const double xy = ys * X;
    const double xz = zs * X;
    const float yy = store(ys * Y);
    const float yz = store(zs * Y);
    const double zz = zs * Z;

    RotationTerms t{};
    t.m00 = store(1.0 - (yy + zz));
    t.m01 = store(xy - wz);
    t.m02 = store(xz + wy);
    t.m10 = store(xy + wz);
    t.m11 = store(1.0 - (zz + xx));
    t.m12 = store(yz - wx);
    t.m20 = store(xz - wy);
    t.m21 = store(yz + wx);
    t.m22 = store(1.0 - (xx + yy));
    return t;
}

} // namespace

// ---- math tables -----------------------------------------------------------

const std::array<float, math::kCosineTableSize + 1>& math::CosineTable() noexcept {
    // Math_InitializeCosineLookup @ 0x00A0DB60: fild(2j) * pi/1024 (float), fcos, fstp.
    static const auto table = [] {
        std::array<float, kCosineTableSize + 1> t{};
        for (int j = 0; j < kCosineTableSize; ++j) {
            t[static_cast<std::size_t>(j)] =
                store(std::cos(static_cast<double>(2 * j) * static_cast<double>(kPiOver1024)));
        }
        t[kCosineTableSize] = t[0];
        return t;
    }();
    return table;
}

double math::TableCos(float x) noexcept { return tableLookup(x, 0); }
double math::TableSin(float x) noexcept { return tableLookup(x, 256); }

// ---- CQuaternion -------------------------------------------------------------

CQuaternion CQuaternion::operator*(const CQuaternion& p) const noexcept {
    const double qX = X, qY = Y, qZ = Z, qW = W;
    const double pX = p.X, pY = p.Y, pZ = p.Z, pW = p.W;
    CQuaternion r;
    r.W = store(((qW * pW - pX * qX) - qY * pY) - qZ * pZ);
    r.X = store(((qW * pX + qX * pW) + qY * pZ) - qZ * pY);
    r.Y = store(((pY * qW + qY * pW) + qZ * pX) - pZ * qX);
    r.Z = store(((pZ * qW + pY * qX) + qZ * pW) - qY * pX);
    return r;
}

CQuaternion& CQuaternion::operator*=(const CQuaternion& rhs) noexcept {
    *this = *this * rhs;
    return *this;
}

bool CQuaternion::Equals(const CQuaternion& rhs) const noexcept {
    return x87::less(std::fabs(static_cast<double>(X) - rhs.X), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(Y) - rhs.Y), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(Z) - rhs.Z), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(W) - rhs.W), kEpsilon);
}

bool CQuaternion::IsIdentity() const noexcept {
    return x87::less(std::fabs(static_cast<double>(X)), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(Y)), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(Z)), kEpsilon) &&
           x87::less(std::fabs(static_cast<double>(W) - 1.0), kEpsilon);
}

void CQuaternion::SetFromAxisAngle(const C3DVector& axis, float angle) noexcept {
    // Half angle in table steps: angle * 0.5 * 1024 (retail 0x0122F59C, 0x01230010).
    const float x = store(static_cast<double>(angle) * 0.5 * 1024.0);
    const double s = math::TableSin(x);
    X = store(s * axis.X);
    Y = store(s * axis.Y);
    Z = store(s * axis.Z);
    W = store(math::TableCos(x));
}

// ---- C3DKeyframe ---------------------------------------------------------------

void C3DKeyframe::ToMatrix(CMatrix3x4& out, bool applyScaling) const noexcept {
    const RotationTerms t = rotationTerms(Rotation);
    out.E11 = t.m00;
    out.E12 = t.m01;
    out.E13 = t.m02;
    out.E21 = t.m10;
    out.E22 = t.m11;
    out.E23 = t.m12;
    out.E31 = t.m20;
    out.E32 = t.m21;
    out.E33 = t.m22;
    out.E41 = Position.X;
    out.E42 = Position.Y;
    out.E43 = Position.Z;
    if (applyScaling) {
        const double sx = Scaling.X, sy = Scaling.Y, sz = Scaling.Z;
        out.E11 = store(t.m00 * sx);
        out.E12 = store(t.m01 * sx);
        out.E13 = store(t.m02 * sx);
        out.E21 = store(t.m10 * sy);
        out.E22 = store(t.m11 * sy);
        out.E23 = store(t.m12 * sy);
        out.E31 = store(t.m20 * sz);
        out.E32 = store(t.m21 * sz);
        out.E33 = store(t.m22 * sz);
    }
}

void C3DKeyframe::ToMatrix(CMatrix4x4& out) const noexcept {
    const RotationTerms t = rotationTerms(Rotation);
    out.M = {t.m00, t.m01, t.m02, 0.0f,  //
             t.m10, t.m11, t.m12, 0.0f,  //
             t.m20, t.m21, t.m22, 0.0f,  //
             Position.X, Position.Y, Position.Z, 1.0f};
}

// ---- CPreTransposedBoneMatrix -------------------------------------------------

void CPreTransposedBoneMatrix::Transform(const C4DVector& in, C4DVector& out, float w) const noexcept {
    const double x = in.X, y = in.Y, z = in.Z, wd = w;
    const auto& D = Data;
    const float rx = store(((z * D[0][2] + y * D[0][1]) + wd * D[0][3]) + x * D[0][0]);
    const float ry = store(((z * D[1][2] + y * D[1][1]) + x * D[1][0]) + wd * D[1][3]);
    const float rz = store(((z * D[2][2] + y * D[2][1]) + x * D[2][0]) + wd * D[2][3]);
    out.X = rx;
    out.Y = ry;
    out.Z = rz;
    out.W = w;
}

void CPreTransposedBoneMatrix::TransformInPlace(C4DVector& v, float w) const noexcept {
    const C4DVector in = v;
    Transform(in, v, w);
}

} // namespace fable
