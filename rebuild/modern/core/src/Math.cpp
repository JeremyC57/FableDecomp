// Bit-exact portable reconstruction of retail Fable.exe math routines.
// Comments name the retail address; `store()` marks every retail `fstp DWORD`.

#include "fable/core/Math.hpp"

#include "fable/core/X87.hpp"

#include <bit>
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

const std::array<std::uint32_t, 128>& math::InvSqrtTable() noexcept {
    // First loop of 0x00A0DB60: f = float with bits ((i | 0x1F80) << 17), i.e. the
    // 128 buckets (exponent parity + 6 mantissa bits) of [0.5, 2); entry = rounded
    // top mantissa bits of 1/sqrt(f). The retail store of 0xFF to 0x013CE658 then
    // overrides bucket 64 (f = 1.0, whose rounded mantissa would wrap to 0).
    static const auto table = [] {
        std::array<std::uint32_t, 128> t{};
        for (std::uint32_t i = 0; i < 128; ++i) {
            const float f = std::bit_cast<float>((i | 0x1F80U) << 17U);
            const float r = store(1.0 / std::sqrt(static_cast<double>(f)));
            t[i] = ((std::bit_cast<std::uint32_t>(r) + 0x2000U) >> 15U) & 0xFFU;
        }
        t[64] = 0xFF;
        return t;
    }();
    return table;
}

float math::InvSqrtEstimate(float x) noexcept {
    const std::uint32_t u = std::bit_cast<std::uint32_t>(x);
    const std::uint32_t exponent = (u >> 23U) & 0xFFU;
    const std::uint32_t bucket = (u >> 17U) & 0x7FU;
    const std::uint32_t bits = (((0x17CU - exponent) << 22U) & 0xFF800000U) | (InvSqrtTable()[bucket] << 15U);
    return std::bit_cast<float>(bits);
}

double math::FastInvSqrt(float x) noexcept {
    // y = (3 - y0*y0*x) * y0 * 0.5   (retail constants 0x0122DED4 = 3, 0x0122F59C = 0.5)
    const double y0 = InvSqrtEstimate(x);
    return ((3.0 - (y0 * y0) * x) * y0) * 0.5;
}

double math::TableCos(float x) noexcept { return tableLookup(x, 0); }
double math::TableSin(float x) noexcept { return tableLookup(x, 256); }

// ---- C2DVector / C3DVector ---------------------------------------------------------

void C2DVector::Normalise() noexcept {
    const double inv = 1.0 / std::sqrt(static_cast<double>(Y) * Y + static_cast<double>(X) * X);
    X = store(inv * X);
    Y = store(inv * Y);
}

C2DVector& C2DVector::FastNormalise() noexcept {
    const float n = store(static_cast<double>(Y) * Y + static_cast<double>(X) * X);
    const double inv = math::FastInvSqrt(n);
    X = store(inv * X);
    Y = store(inv * Y);
    return *this;
}

void C3DVector::Normalise() noexcept {
    const double len = std::sqrt((static_cast<double>(Z) * Z + static_cast<double>(Y) * Y) + static_cast<double>(X) * X);
    const double inv = 1.0 / len;
    X = store(inv * X);
    Y = store(inv * Y);
    Z = store(inv * Z);
}

double C3DVector::NormaliseAndGetLength() noexcept {
    const double len = std::sqrt((static_cast<double>(Z) * Z + static_cast<double>(Y) * Y) + static_cast<double>(X) * X);
    const double inv = 1.0 / len;
    X = store(inv * X);
    Y = store(inv * Y);
    Z = store(inv * Z);
    return len;
}

void C3DVector::Rotate(const C3DVector& axis, float angle) noexcept {
    CMatrix3x4 m;
    m.InitialiseRotation(axis, angle);
    const double x = X, y = Y, z = Z;
    X = store((m.E31 * z + m.E21 * y) + m.E11 * x);
    Y = store((m.E32 * z + m.E22 * y) + m.E12 * x);
    Z = store((m.E33 * z + m.E23 * y) + m.E13 * x);
}

// ---- CMatrix3x4 ----------------------------------------------------------------

namespace {

using Mat12 = std::array<float, 12>;

Mat12 elements(const CMatrix3x4& m) noexcept { return std::bit_cast<Mat12>(m); }
CMatrix3x4 fromElements(const Mat12& e) noexcept { return std::bit_cast<CMatrix3x4>(e); }

} // namespace

void CMatrix3x4::InitialiseSkewedSymmetric(const C3DVector& v) noexcept {
    *this = fromElements({0.0f, -v.Z, v.Y,  //
                          v.Z, 0.0f, -v.X,  //
                          -v.Y, v.X, 0.0f,  //
                          0.0f, 0.0f, 0.0f});
}

CMatrix3x4 CMatrix3x4::operator*(const CMatrix3x4& rhs) const noexcept {
    const Mat12 a = elements(*this);
    const Mat12 b = elements(rhs);
    auto A = [&](int i) { return static_cast<double>(a[static_cast<std::size_t>(i)]); };
    auto B = [&](int i) { return static_cast<double>(b[static_cast<std::size_t>(i)]); };
    return fromElements({
        store((A(0) * B(0) + A(1) * B(3)) + A(2) * B(6)),
        store((A(2) * B(7) + A(0) * B(1)) + A(1) * B(4)),
        store((A(2) * B(8) + A(0) * B(2)) + A(1) * B(5)),
        store((A(3) * B(0) + A(4) * B(3)) + A(5) * B(6)),
        store((A(5) * B(7) + A(3) * B(1)) + A(4) * B(4)),
        store((A(5) * B(8) + A(3) * B(2)) + A(4) * B(5)),
        store((A(6) * B(0) + A(7) * B(3)) + A(8) * B(6)),
        store((A(6) * B(1) + A(7) * B(4)) + A(8) * B(7)),
        store((A(8) * B(8) + A(6) * B(2)) + A(7) * B(5)),
        store(((A(9) * B(0) + A(10) * B(3)) + A(11) * B(6)) + B(9)),
        store(((A(9) * B(1) + A(10) * B(4)) + A(11) * B(7)) + B(10)),
        store(((A(11) * B(8) + A(9) * B(2)) + A(10) * B(5)) + B(11)),
    });
}

CMatrix3x4& CMatrix3x4::operator*=(const CMatrix3x4& rhs) noexcept {
    const Mat12 a = elements(*this);
    const Mat12 b = elements(rhs);
    auto A = [&](int i) { return static_cast<double>(a[static_cast<std::size_t>(i)]); };
    auto B = [&](int i) { return static_cast<double>(b[static_cast<std::size_t>(i)]); };
    *this = fromElements({
        store((A(2) * B(6) + A(1) * B(3)) + A(0) * B(0)),
        store((A(0) * B(1) + A(2) * B(7)) + A(1) * B(4)),
        store((A(0) * B(2) + A(2) * B(8)) + A(1) * B(5)),
        store((A(5) * B(6) + A(4) * B(3)) + A(3) * B(0)),
        store((A(3) * B(1) + A(5) * B(7)) + A(4) * B(4)),
        store((A(3) * B(2) + A(5) * B(8)) + A(4) * B(5)),
        store((A(8) * B(6) + A(7) * B(3)) + A(6) * B(0)),
        store((A(6) * B(1) + A(8) * B(7)) + A(7) * B(4)),
        store((A(6) * B(2) + A(8) * B(8)) + A(7) * B(5)),
        store(((A(11) * B(6) + A(10) * B(3)) + A(9) * B(0)) + B(9)),
        store(((A(9) * B(1) + A(11) * B(7)) + A(10) * B(4)) + B(10)),
        store(((A(9) * B(2) + A(11) * B(8)) + A(10) * B(5)) + B(11)),
    });
    return *this;
}

bool CMatrix3x4::IsIdentity() const noexcept {
    const Mat12 a = elements(*this);
    auto A = [&](int i) { return static_cast<double>(a[static_cast<std::size_t>(i)]); };
    const double limit = store(kEpsilon * kEpsilon);
    // `test ah,0x41; je fail`: only a strictly greater (ordered) result fails.
    auto withinLimit = [limit](double sumSquares) { return !(sumSquares > limit); };
    const double d0 = A(0) - 1.0;
    const double d4 = A(4) - 1.0;
    const double d8 = A(8) - 1.0;
    return withinLimit((A(2) * A(2) + A(1) * A(1)) + d0 * d0) &&
           withinLimit((A(5) * A(5) + d4 * d4) + A(3) * A(3)) &&
           withinLimit((d8 * d8 + A(7) * A(7)) + A(6) * A(6)) &&
           withinLimit((A(11) * A(11) + A(10) * A(10)) + A(9) * A(9));
}

bool CMatrix3x4::Equals(const CMatrix3x4& rhs) const noexcept {
    const Mat12 a = elements(*this);
    const Mat12 b = elements(rhs);
    Mat12 t{};
    for (std::size_t i = 0; i < 12; ++i) {
        t[i] = store(static_cast<double>(b[i]) - a[i]);
    }
    t[0] = store((static_cast<double>(b[0]) + 1.0) - a[0]);         // kept in a register
    t[4] = store(static_cast<double>(store(b[4] + 1.0)) - a[4]);   // stored first
    t[8] = store(static_cast<double>(store(b[8] + 1.0)) - a[8]);
    return fromElements(t).IsIdentity();
}

void CMatrix3x4::Orthonormalise() noexcept {
    const double A0 = E11, A1 = E12, A2 = E13, A3 = E21, A4 = E22, A5 = E23;

    // Row 0 normalised; X and Y keep register precision, Z is stored.
    const float n0 = store((A2 * A2 + A1 * A1) + A0 * A0);
    const double y = math::FastInvSqrt(n0);
    const float yf = store(y);
    const double a0n = A0 * y;
    const double a1n = static_cast<double>(yf) * A1;
    const double a2n = store(static_cast<double>(yf) * A2);

    // Row 2 = normalise(row0 x row1).
    const double c0 = store(A5 * a1n - A4 * a2n);
    const double c1 = store(A3 * a2n - A5 * a0n);
    const double c2 = store(A4 * a0n - A3 * a1n);
    const float n1 = store((c2 * c2 + c1 * c1) + c0 * c0);
    const double y1 = math::FastInvSqrt(n1);
    const float c0n = store(c0 * y1);
    const float c1n = store(c1 * y1);
    const float c2n = store(y1 * c2);

    // Row 1 = normalise(row2 x row0).
    const double d0 = store(c1n * a2n - c2n * a1n);
    const double d1 = store(c2n * a0n - c0n * a2n);
    const double d2 = store(c0n * a1n - c1n * a0n);
    const float n2 = store((d2 * d2 + d1 * d1) + d0 * d0);
    const double y2 = math::FastInvSqrt(n2);

    E11 = store(a0n);
    E12 = store(a1n);
    E13 = static_cast<float>(a2n);
    E21 = store(d0 * y2);
    E22 = store(d1 * y2);
    E23 = store(y2 * d2);
    E31 = c0n;
    E32 = c1n;
    E33 = c2n;
    E41 = E42 = E43 = 0.0f;
}

C3DVector CMatrix3x4::operator*(const C3DVector& v) const noexcept {
    const double x = v.X, y = v.Y, z = v.Z;
    return {store((E12 * y + E13 * z) + x * E11),  //
            store((E22 * y + E21 * x) + E23 * z),  //
            store((E32 * y + E31 * x) + E33 * z)};
}

void CMatrix3x4::InitialiseRotation(const C3DVector& axis, float angle) noexcept {
    const double theta = static_cast<double>(angle) * 6.2831854820251465;  // retail double 0x0124F2B8
    const double c = std::cos(theta);
    const double s = std::sin(theta);
    const double X = axis.X, Y = axis.Y, Z = axis.Z;
    const double xx = X * X;  // kept in a register
    const double yy = store(Y * Y);
    const double zz = store(Z * Z);
    const double xy = store(Y * X);
    const double xz = store(Z * X);
    const double yz = store(Z * Y);
    const double sx = store(s * X);
    const double sy = store(s * Y);
    const double sz = store(s * Z);

    E11 = store((xx + c) - xx * c);
    const double t = xy - xy * c;
    E21 = store(sz + t);
    const double u = xz - xz * c;
    E31 = store(u - sy);
    E12 = store(t - sz);
    E22 = store((yy + c) - yy * c);
    const double v = yz - yz * c;
    const double vf = store(v);
    E32 = store(v + sx);
    E13 = store(u + sy);
    E23 = store(vf - sx);
    E33 = store((zz + c) - zz * c);
    E41 = E42 = E43 = 0.0f;
}

// ---- CPlane -----------------------------------------------------------------

bool CPlane::Initialise(const C3DVector& normal, float distance) noexcept {
    Normal = normal;
    Distance = distance;
    return true;
}

bool CPlane::Initialise(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2) noexcept {
    C3DVector e1{store(static_cast<double>(p1.X) - p0.X), store(static_cast<double>(p1.Y) - p0.Y),
                 store(static_cast<double>(p1.Z) - p0.Z)};
    C3DVector e2{store(static_cast<double>(p2.X) - p0.X), store(static_cast<double>(p2.Y) - p0.Y),
                 store(static_cast<double>(p2.Z) - p0.Z)};
    e1.Normalise();
    e2.Normalise();
    Normal.X = store(static_cast<double>(e2.Z) * e1.Y - static_cast<double>(e1.Z) * e2.Y);
    Normal.Y = store(static_cast<double>(e1.Z) * e2.X - static_cast<double>(e2.Z) * e1.X);
    Normal.Z = store(static_cast<double>(e2.Y) * e1.X - static_cast<double>(e1.Y) * e2.X);
    Normal.Normalise();
    Distance = store((static_cast<double>(Normal.Z) * p0.Z + static_cast<double>(Normal.Y) * p0.Y) +
                     static_cast<double>(p0.X) * Normal.X);
    return true;
}

bool CPlane::InitialiseFromPointAndNormal(const C3DVector& point, const C3DVector& normal) noexcept {
    Normal = normal;
    Normal.Normalise();
    Distance = store((static_cast<double>(point.Z) * Normal.Z + static_cast<double>(point.Y) * Normal.Y) +
                     static_cast<double>(point.X) * Normal.X);
    return true;
}

namespace {

double planeDotDir(const C3DVector& n, const C3DVector& dir) noexcept {
    return (static_cast<double>(dir.Y) * n.Y + static_cast<double>(dir.X) * n.X) + static_cast<double>(n.Z) * dir.Z;
}

double planeDotPoint(const C3DVector& n, const C3DVector& p) noexcept {
    return (static_cast<double>(p.Z) * n.Z + static_cast<double>(p.X) * n.X) + static_cast<double>(p.Y) * n.Y;
}

void pointAlong(const C3DVector& point, const C3DVector& dir, double t, C3DVector& out) noexcept {
    const double tx = t * dir.X;  // kept in a register
    const double ty = store(t * dir.Y);
    const double tz = store(t * dir.Z);
    out = {store(tx + point.X), store(ty + point.Y), store(tz + point.Z)};
}

} // namespace

bool CPlane::GetIntersectionWithLine(const C3DVector& point, const C3DVector& dir, C3DVector& out) const noexcept {
    const double denom = planeDotDir(Normal, dir);
    if (denom == 0.0) {
        return false;
    }
    const double t = (static_cast<double>(Distance) - planeDotPoint(Normal, point)) / denom;
    pointAlong(point, dir, t, out);
    return true;
}

bool CPlane::GetIntersectionWithLineOffset(const C3DVector& point, const C3DVector& dir, float offset,
                                           C3DVector& out) const noexcept {
    const double denom = planeDotDir(Normal, dir);
    if (denom == 0.0) {
        return false;
    }
    const double inv = 1.0 / denom;
    const double t = ((static_cast<double>(Distance) - planeDotPoint(Normal, point)) * inv - inv * offset) - kEpsilon;
    pointAlong(point, dir, t, out);
    return true;
}

bool CPlane::IsWithinDistance(const C3DVector& point, float maxDistance, float& distance) const noexcept {
    const double d = std::fabs(((static_cast<double>(point.Z) * Normal.Z + static_cast<double>(point.Y) * Normal.Y) +
                                static_cast<double>(point.X) * Normal.X) -
                               Distance);
    distance = store(d);
    return d < maxDistance;
}

bool CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c, C3DVector& out1,
                                         C3DVector& out2) const noexcept {
    const double N0 = Normal.X, N1 = Normal.Y, N2 = Normal.Z, D = Distance;
    const float da = store(((a.Y * N1 + N2 * a.Z) + a.X * N0) - D);
    const float db = store(((b.X * N0 + b.Y * N1) + b.Z * N2) - D);
    const float dc = store(((c.Z * N2 + c.X * N0) + c.Y * N1) - D);
    auto sameSide = [](float p, float q) { return (p > 0.0f && q > 0.0f) || (p < 0.0f && q < 0.0f); };
    auto edge = [](const C3DVector& to, const C3DVector& from) {
        return C3DVector{store(static_cast<double>(to.X) - from.X), store(static_cast<double>(to.Y) - from.Y),
                         store(static_cast<double>(to.Z) - from.Z)};
    };
    const C3DVector* base = nullptr;
    C3DVector e1, e2;
    if (sameSide(da, db)) {
        base = &c, e1 = edge(a, c), e2 = edge(b, c);
    } else if (sameSide(da, dc)) {
        base = &b, e1 = edge(a, b), e2 = edge(c, b);
    } else if (sameSide(db, dc)) {
        base = &a, e1 = edge(b, a), e2 = edge(c, a);
    } else {
        return false;
    }
    const C3DVector origin = *base;
    GetIntersectionWithLine(origin, e1, out1);
    GetIntersectionWithLine(origin, e2, out2);
    return true;
}

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

void CPreTransposedBoneMatrix::ScaleRows(const C3DVector& s) noexcept {
    // The retail SSE path (flag 0x013D2880) multiplies whole rows and restores
    // column 3; single float products round identically on either path.
    const float scale[3] = {s.X, s.Y, s.Z};
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            Data[r][c] = store(static_cast<double>(scale[r]) * Data[r][c]);
        }
    }
}

void CPreTransposedBoneMatrix::ScaleColumns(const C3DVector& s) noexcept {
    const float scale[3] = {s.X, s.Y, s.Z};
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            Data[r][c] = store(static_cast<double>(scale[c]) * Data[r][c]);
        }
    }
}

} // namespace fable
