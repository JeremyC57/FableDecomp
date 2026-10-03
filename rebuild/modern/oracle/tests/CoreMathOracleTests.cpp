// Bit-exact differential tests: fable_core math vs retail Fable.exe.

#include "OracleTestSupport.hpp"

#include "fable/core/Math.hpp"

#include <bit>
#include <array>
#include <cmath>
#include <cstring>

namespace fable::oracle::test {
namespace {

constexpr int kRounds = 2000;

bool sameBits(float a, float b) {
    if (std::isnan(a) || std::isnan(b)) {
        return std::isnan(a) && std::isnan(b);  // NaN payloads differ across CPUs
    }
    return std::bit_cast<std::uint32_t>(a) == std::bit_cast<std::uint32_t>(b);
}

template <typename T>
bool sameFloats(const T& a, const T& b) {
    static_assert(sizeof(T) % 4 == 0);
    float fa[sizeof(T) / 4];
    float fb[sizeof(T) / 4];
    std::memcpy(fa, &a, sizeof(T));
    std::memcpy(fb, &b, sizeof(T));
    for (std::size_t i = 0; i < sizeof(T) / 4; ++i) {
        if (!sameBits(fa[i], fb[i])) {
            std::cerr << "    float " << i << ": port " << fa[i] << " (0x" << std::hex
                      << std::bit_cast<std::uint32_t>(fa[i]) << ") retail " << fb[i] << " (0x"
                      << std::bit_cast<std::uint32_t>(fb[i]) << std::dec << ")\n";
            return false;
        }
    }
    return true;
}

// Value with a random exponent in [2^-20, 2^20]: mixing magnitudes makes
// intermediate rounding (and so operation order) visible in the result.
float wideMagnitude() {
    const float mantissa = randomFloat(1.0f, 2.0f);
    const int exponent = static_cast<int>(randomU32() % 41) - 20;
    return ((randomU32() & 1U) != 0 ? -1.0f : 1.0f) * std::ldexp(mantissa, exponent);
}

// +-2^-20, +-1, +-2^20: products cancel exactly, exposing operation order.
float cancelling() {
    static constexpr float kScales[] = {0x1p-20f, 1.0f, 0x1p20f};
    return ((randomU32() & 1U) != 0 ? -1.0f : 1.0f) * kScales[randomU32() % 3];
}

// A quarter of composite samples are built entirely from cancelling values.
bool cancellingMode() { return randomU32() % 4 == 0; }

// Mix of ordinary values, exact edge cases, wide magnitudes and extremes.
float sample(float range = 4.0f) {
    switch (randomU32() % 16) {
    case 6:
    case 7:
    case 8:
    case 9: return wideMagnitude();
    case 10:
    case 11: return cancelling();
    case 0: return 0.0f;
    case 1: return -0.0f;
    case 2: return 1.0f;
    case 3: return -1.0f;
    case 4: return randomFloat(-1.0e-3f, 1.0e-3f);
    case 5: return randomFloat(-1.0e4f, 1.0e4f);
    default: return randomFloat(-range, range);
    }
}

CQuaternion randomQuat() {
    if (cancellingMode()) {
        return {cancelling(), cancelling(), cancelling(), cancelling()};
    }
    return {sample(), sample(), sample(), sample()};
}
C3DVector randomVec3() {
    if (cancellingMode()) {
        return {cancelling(), cancelling(), cancelling()};
    }
    return {sample(), sample(), sample()};
}
C4DVector randomVec4() {
    if (cancellingMode()) {
        return {cancelling(), cancelling(), cancelling(), cancelling()};
    }
    return {sample(), sample(), sample(), sample()};
}

template <typename T>
std::uint32_t place(RetailOracle& o, const T& v) {
    const auto at = o.alloc(sizeof(T) + 16);
    o.put(at, v);
    return at;
}

void fillRetailCosineTable(RetailOracle& o) {
    // Math_InitializeCosineLookup @ 0x00A0DB60 up to its first CRT acos call
    // (0x00A0DBF8 is just after the wrap-around store at 0x00A0DBF3).
    o.runUntil(0x00A0DB60, 0x00A0DBF8);  // through the T[1024] = T[0] copy
}

void cosineTable(RetailOracle& o) {
    fillRetailCosineTable(o);
    const auto& table = math::CosineTable();
    for (std::size_t j = 0; j < table.size(); ++j) {
        const auto retail = o.get<float>(0x013CD550 + static_cast<std::uint32_t>(j) * 4);
        if (!sameBits(table[j], retail)) {
            std::cerr << "    cosine table entry " << j << '\n';
            CHECK(false);
            return;
        }
    }
}

void quatMultiply(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        const auto a = randomQuat(), b = randomQuat();
        const auto pa = place(o, a), pb = place(o, b), pr = o.alloc(16);
        const std::uint32_t args[] = {pr, pb};
        o.call(0x00A88B60, CallingConvention::Thiscall, args, pa);
        if (!sameFloats(a * b, o.get<CQuaternion>(pr))) {
            CHECK(false);
            return;
        }
    }
}

void quatMultiplyAssign(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        auto a = randomQuat();
        const auto b = randomQuat();
        const auto pa = place(o, a), pb = place(o, b);
        const std::uint32_t args[] = {pb};
        const auto r = o.call(0x00A88C10, CallingConvention::Thiscall, args, pa);
        CHECK_EQ(r.eax, pa);
        a *= b;
        if (!sameFloats(a, o.get<CQuaternion>(pa))) {
            CHECK(false);
            return;
        }
    }
}

void quatEqualsAndIdentity(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        auto a = randomQuat();
        auto b = a;
        if (i % 2 == 0) {  // perturb around the 1e-4 tolerance
            b.X += randomFloat(-2e-4f, 2e-4f);
            b.W += randomFloat(-2e-4f, 2e-4f);
        }
        if (i % 3 == 0) {
            a = {randomFloat(-2e-4f, 2e-4f), randomFloat(-2e-4f, 2e-4f), 0.0f, 1.0f + randomFloat(-2e-4f, 2e-4f)};
        }
        const auto pa = place(o, a), pb = place(o, b);
        const std::uint32_t args[] = {pb};
        const auto eq = o.call(0x00A88C50, CallingConvention::Thiscall, args, pa);
        CHECK_EQ(eq.eax & 0xFFU, static_cast<std::uint32_t>(a.Equals(b)));
        const auto id = o.call(0x00A88CB0, CallingConvention::Thiscall, {}, pa);
        CHECK_EQ(id.eax & 0xFFU, static_cast<std::uint32_t>(a.IsIdentity()));
    }
}

void quatFromAxisAngle(RetailOracle& o) {
    fillRetailCosineTable(o);
    for (int i = 0; i < kRounds; ++i) {
        const auto axis = randomVec3();
        const float angle = (i % 4 == 0) ? sample(1000.0f) : randomFloat(-2.0f, 2.0f);
        CQuaternion port;
        port.SetFromAxisAngle(axis, angle);
        const auto pq = o.alloc(16), pv = place(o, axis);
        const std::uint32_t args[] = {pv, RetailOracle::bits(angle)};
        o.call(0x00A88D10, CallingConvention::Thiscall, args, pq);
        if (!sameFloats(port, o.get<CQuaternion>(pq))) {
            std::cerr << "    angle " << angle << '\n';
            CHECK(false);
            return;
        }
    }
}

void keyframeToMatrix(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        C3DKeyframe k{randomQuat(), randomVec4(), randomVec4()};
        if (i % 50 == 0) {
            k.Rotation = {0, 0, 0, 0};  // zero-length branch
        }
        const bool scale = (i % 2) != 0;
        CMatrix3x4 port;
        k.ToMatrix(port, scale);
        const auto pk = place(o, k), pm = o.alloc(48);
        const std::uint32_t args[] = {pm, scale ? 1U : 0U};
        o.call(0x00A52450, CallingConvention::Thiscall, args, pk);
        if (!sameFloats(port, o.get<CMatrix3x4>(pm))) {
            CHECK(false);
            return;
        }

        CMatrix4x4 port4;
        k.ToMatrix(port4);
        const auto pm4 = o.alloc(64);
        const std::uint32_t args4[] = {pm4, 0U};
        o.call(0x00AA39A0, CallingConvention::Thiscall, args4, pk);
        if (!sameFloats(port4, o.get<CMatrix4x4>(pm4))) {
            CHECK(false);
            return;
        }
    }
}

void boneMatrixTransform(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        CPreTransposedBoneMatrix m;
        for (auto& row : m.Data) {
            for (auto& v : row) {
                v = (i % 4 == 0) ? cancelling() : sample();
            }
        }
        const auto in = randomVec4();
        const float w = sample();
        C4DVector port;
        m.Transform(in, port, w);
        const auto pm = place(o, m), pin = place(o, in), pout = o.alloc(16);
        const std::uint32_t args[] = {pin, pout, RetailOracle::bits(w)};
        o.call(0x00987BF0, CallingConvention::Thiscall, args, pm);
        if (!sameFloats(port, o.get<C4DVector>(pout))) {
            CHECK(false);
            return;
        }

        C4DVector inplace = in;
        m.TransformInPlace(inplace, w);
        const auto pv = place(o, in);
        const std::uint32_t args2[] = {pv, RetailOracle::bits(w)};
        o.call(0x00ADDFE0, CallingConvention::Thiscall, args2, pm);
        if (!sameFloats(inplace, o.get<C4DVector>(pv))) {
            CHECK(false);
            return;
        }
    }
}

CMatrix3x4 randomMatrix() {
    std::array<float, 12> e{};
    const bool cancel = cancellingMode();
    for (auto& v : e) {
        v = cancel ? cancelling() : sample();
    }
    return std::bit_cast<CMatrix3x4>(e);
}

CMatrix3x4 nearIdentity(float spread) {
    std::array<float, 12> e{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    for (auto& v : e) {
        if (randomU32() % 3 == 0) {
            v += randomFloat(-spread, spread);
        }
    }
    return std::bit_cast<CMatrix3x4>(e);
}

void invSqrtTable(RetailOracle& o) {
    fillRetailCosineTable(o);
    const auto& table = math::InvSqrtTable();
    for (std::size_t i = 0; i < table.size(); ++i) {
        CHECK_EQ(table[i], o.get<std::uint32_t>(0x013CE558 + static_cast<std::uint32_t>(i) * 4));
    }
}

void matrixSkew(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        const auto v = randomVec3();
        CMatrix3x4 port = randomMatrix();
        port.InitialiseSkewedSymmetric(v);
        const auto pm = place(o, randomMatrix()), pv = place(o, v);
        const std::uint32_t args[] = {pv};
        o.call(0x00A55D80, CallingConvention::Thiscall, args, pm);
        if (!sameFloats(port, o.get<CMatrix3x4>(pm))) {
            CHECK(false);
            return;
        }
    }
}

void matrixMultiply(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        const auto a = randomMatrix(), b = randomMatrix();
        const auto pa = place(o, a), pb = place(o, b), pr = o.alloc(48);
        const std::uint32_t args[] = {pr, pb};
        o.call(0x00A55DF0, CallingConvention::Thiscall, args, pa);
        if (!sameFloats(a * b, o.get<CMatrix3x4>(pr))) {
            CHECK(false);
            return;
        }
        CMatrix3x4 c = a;
        c *= b;
        o.call(0x00C1CF20, CallingConvention::Fastcall, {}, pa, pb);  // this=ECX, rhs=EDX
        if (!sameFloats(c, o.get<CMatrix3x4>(pa))) {
            CHECK(false);
            return;
        }
    }
}

void matrixIdentityEquals(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        const auto a = (i % 4 == 0) ? randomMatrix() : nearIdentity(2e-4f);
        auto b = a;
        if (i % 2 == 0) {
            std::array<float, 12> e = std::bit_cast<std::array<float, 12>>(b);
            e[randomU32() % 12] += randomFloat(-2e-4f, 2e-4f);
            b = std::bit_cast<CMatrix3x4>(e);
        }
        const auto pa = place(o, a), pb = place(o, b);
        const auto id = o.call(0x00A560A0, CallingConvention::Thiscall, {}, pa);
        CHECK_EQ(id.eax & 0xFFU, static_cast<std::uint32_t>(a.IsIdentity()));
        const std::uint32_t args[] = {pb};
        const auto eq = o.call(0x00A56180, CallingConvention::Thiscall, args, pa);
        CHECK_EQ(eq.eax & 0xFFU, static_cast<std::uint32_t>(a.Equals(b)));
    }
}

void matrixOrthonormalise(RetailOracle& o) {
    fillRetailCosineTable(o);  // also fills the inverse-sqrt table
    for (int i = 0; i < kRounds; ++i) {
        auto m = (i % 3 == 0) ? nearIdentity(0.5f) : randomMatrix();
        const auto pm = place(o, m);
        o.call(0x00A56270, CallingConvention::Thiscall, {}, pm);
        m.Orthonormalise();
        if (!sameFloats(m, o.get<CMatrix3x4>(pm))) {
            CHECK(false);
            return;
        }
    }
}

void matrixTimesVector(RetailOracle& o) {
    for (int i = 0; i < kRounds; ++i) {
        const auto m = randomMatrix();
        const auto v = randomVec3();
        const auto pm = place(o, m), pv = place(o, v), pr = o.alloc(12);
        const std::uint32_t args[] = {pr, pv};
        o.call(0x00A56530, CallingConvention::Thiscall, args, pm);
        if (!sameFloats(m * v, o.get<C3DVector>(pr))) {
            CHECK(false);
            return;
        }
    }
}

void boneMatrixScale(RetailOracle& o) {
    for (const std::uint8_t sse : {0, 1}) {
        o.put<std::uint8_t>(0x013D2880, sse);  // retail CPU-feature flag selecting the SSE path
        for (int i = 0; i < kRounds; ++i) {
            CPreTransposedBoneMatrix m;
            for (auto& row : m.Data) {
                for (auto& v : row) {
                    v = sample();
                }
            }
            const auto s = randomVec3();
            const auto pm = o.alloc(48, 16), pv = place(o, s);
            o.put(pm, m);
            const std::uint32_t args[] = {pv};
            auto rows = m;
            rows.ScaleRows(s);
            o.call(0x00A9D480, CallingConvention::Thiscall, args, pm);
            if (!sameFloats(rows, o.get<CPreTransposedBoneMatrix>(pm))) {
                std::cerr << "    ScaleRows sse=" << int{sse} << '\n';
                CHECK(false);
                return;
            }
            auto cols = rows;
            cols.ScaleColumns(s);
            o.call(0x00A9D580, CallingConvention::Thiscall, args, pm);
            if (!sameFloats(cols, o.get<CPreTransposedBoneMatrix>(pm))) {
                std::cerr << "    ScaleColumns sse=" << int{sse} << '\n';
                CHECK(false);
                return;
            }
        }
    }
}

} // namespace

void registerCoreMathTests(TestList& tests) {
    tests.push_back({"math.InvSqrtTable", invSqrtTable});
    tests.push_back({"CMatrix3x4::InitialiseSkewedSymmetric 0x00A55D80", matrixSkew});
    tests.push_back({"CMatrix3x4::operator*/*= 0x00A55DF0/0x00C1CF20", matrixMultiply});
    tests.push_back({"CMatrix3x4::IsIdentity/Equals 0x00A560A0/0x00A56180", matrixIdentityEquals});
    tests.push_back({"CMatrix3x4::Orthonormalise 0x00A56270", matrixOrthonormalise});
    tests.push_back({"CMatrix3x4::operator*(C3DVector) 0x00A56530", matrixTimesVector});
    tests.push_back({"CPreTransposedBoneMatrix::ScaleRows/Columns 0x00A9D480/0x00A9D580", boneMatrixScale});
    tests.push_back({"math.CosineTable", cosineTable});
    tests.push_back({"CQuaternion::operator* 0x00A88B60", quatMultiply});
    tests.push_back({"CQuaternion::operator*= 0x00A88C10", quatMultiplyAssign});
    tests.push_back({"CQuaternion::Equals/IsIdentity 0x00A88C50/0x00A88CB0", quatEqualsAndIdentity});
    tests.push_back({"CQuaternion::SetFromAxisAngle 0x00A88D10", quatFromAxisAngle});
    tests.push_back({"C3DKeyframe::ToMatrix 0x00A52450/0x00AA39A0", keyframeToMatrix});
    tests.push_back({"CPreTransposedBoneMatrix::Transform 0x00987BF0/0x00ADDFE0", boneMatrixTransform});
}

} // namespace fable::oracle::test
