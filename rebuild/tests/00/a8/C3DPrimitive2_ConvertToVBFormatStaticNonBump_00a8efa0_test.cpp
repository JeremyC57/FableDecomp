// Behaviour test for C3DPrimitive2::ConvertToVBFormatStaticNonBump 0x00A8EFA0: every C3DVertex2 of the device-independent
// data is written to the CTVertexNormTex1 buffer (position, normal, U1, V1), remainder past a multiple of four included.
#include <cstdio>

class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class C3DVertex2 { public: C3DVector Position, Normal; float U, V;
    const C3DVector GetPosition() const { return Position; } const C3DVector GetNormal() const { return Normal; }
    float GetU() const { return U; } float GetV() const { return V; } };
class CTVertexNormTex1Base { public: float X, Y, Z, NormalX, NormalY, NormalZ, U1, V1; };
template <class T> class CGenericVertex : public T { public:
    void SetPosition(const C3DVector& p) { this->X = p.X; this->Y = p.Y; this->Z = p.Z; }
    void SetNormal(const C3DVector& p) { this->NormalX = p.X; this->NormalY = p.Y; this->NormalZ = p.Z; }
    void SetU1(float u) { this->U1 = u; } void SetV1(float v) { this->V1 = v; } };
class CTVertexNormTex1 : public CGenericVertex<CTVertexNormTex1Base> { };
struct CDeviceIndependent { C3DVertex2* first; C3DVertex2* last;
    unsigned long GetVertexCount() const { return (unsigned long)(last - first); }
    const C3DVertex2& PeekVertex(unsigned long i) const { return first[i]; } };
class C3DPrimitive2 { public: CDeviceIndependent* Data;
    void ConvertToVBFormatStaticNonBump(CTVertexNormTex1* out) {
        unsigned long count = Data->GetVertexCount();
        for (unsigned long i = 0; i < count; ++i) {
            const C3DVertex2& src = Data->PeekVertex(i);
            CTVertexNormTex1& vertex = out[i];
            vertex.SetPosition(src.GetPosition()); vertex.SetNormal(src.GetNormal()); vertex.SetU1(src.GetU()); vertex.SetV1(src.GetV());
        }
    } };

int main()
{
    const int n = 7;                      // 4 unrolled + 3 remainder
    C3DVertex2 src[n]; CTVertexNormTex1 out[n + 1];
    for (int i = 0; i < n; ++i) {
        src[i].Position.X = 1.f + i; src[i].Position.Y = 2.f + i; src[i].Position.Z = 3.f + i;
        src[i].Normal.X = -1.f - i; src[i].Normal.Y = -2.f - i; src[i].Normal.Z = -3.f - i;
        src[i].U = 0.25f * i; src[i].V = 0.5f * i;
    }
    for (int i = 0; i <= n; ++i) out[i].X = out[i].Y = out[i].Z = out[i].NormalX = out[i].NormalY = out[i].NormalZ = out[i].U1 = out[i].V1 = 99.f;
    CDeviceIndependent data = { src, src + n };
    C3DPrimitive2 prim; prim.Data = &data;
    prim.ConvertToVBFormatStaticNonBump(out);
    for (int i = 0; i < n; ++i) {
        if (out[i].X != 1.f + i || out[i].Y != 2.f + i || out[i].Z != 3.f + i ||
            out[i].NormalX != -1.f - i || out[i].NormalY != -2.f - i || out[i].NormalZ != -3.f - i ||
            out[i].U1 != 0.25f * i || out[i].V1 != 0.5f * i) { std::printf("00a8efa0_TEST FAIL\n"); return 1; }
    }
    if (out[n].X != 99.f) { std::printf("00a8efa0_TEST FAIL\n"); return 1; }
    data.last = src;                      // empty: nothing written
    out[0].X = 5.f; prim.ConvertToVBFormatStaticNonBump(out);
    if (out[0].X != 5.f) { std::printf("00a8efa0_TEST FAIL\n"); return 1; }
    std::printf("00a8efa0_TEST PASS\n");
    return 0;
}