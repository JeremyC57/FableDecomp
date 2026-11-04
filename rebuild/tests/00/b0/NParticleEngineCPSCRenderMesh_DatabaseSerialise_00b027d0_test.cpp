// Behaviour test for NParticleEngine::CPSCRenderMesh::DatabaseSerialise (0x00B027D0): a standalone model of the load path -- an old
// record with StartMeshAlpha builds white start / mid / end colours carrying the transferred alphas (mid / end default 255) and turns
// the Use*Colour flags on; a packed float (steps over a range) is unpacked, rounded to two places, transferred and packed back with
// round-to-nearest, so a value with no record keeps the packed default and a loaded value lands on the nearest step.
#include <cstdio>
#include <cmath>
static float RoundNear(float v) { return std::floor(v * 100.0f + 0.5f) / 100.0f; }
static float Unpack(unsigned long packed, float steps, float lo, float hi) { return RoundNear(packed * (1.0f / steps) * (hi - lo) + lo); }
static unsigned long Pack(float v, float lo, float hi, float steps) { return (unsigned long)std::floor((RoundNear(v) - lo) * (1.0f / (hi - lo)) * steps + 0.5f); }
struct Record { bool hasStartAlpha; long startAlpha; bool hasMidAlpha; long midAlpha; bool hasWidth; float width; };
struct Mesh {
    unsigned long StartColour, MidColour, EndColour; bool UseStartColour, UseMidColour, UseEndColour; unsigned long TrailWidth;
    void Load(const Record& r) {
        long alpha = r.hasStartAlpha ? r.startAlpha : -1;
        if (alpha != -1) {
            StartColour = 0x00FFFFFFul | ((unsigned long)alpha << 24); UseStartColour = true;
            alpha = r.hasMidAlpha ? r.midAlpha : 255; UseMidColour = true; MidColour = 0x00FFFFFFul | ((unsigned long)alpha << 24);
            alpha = 255; EndColour = 0x00FFFFFFul | ((unsigned long)alpha << 24); UseEndColour = true;
        }
        float value = Unpack(TrailWidth, 1023.0f, 0.0f, 10.0f);
        if (r.hasWidth) value = r.width;
        TrailWidth = Pack(value, 0.0f, 10.0f, 1023.0f);
    }
};
int main()
{
    Mesh m = { 0, 0, 0, false, false, false, 51 };
    Record r = { true, 128, false, 0, false, 0.0f };
    m.Load(r);
    bool ok = m.StartColour == 0x80FFFFFF && m.MidColour == 0xFFFFFFFF && m.EndColour == 0xFFFFFFFF && m.UseStartColour && m.UseMidColour
        && m.UseEndColour && m.TrailWidth == 51;
    Mesh n = { 1, 2, 3, false, false, false, 51 };
    Record q = { false, 0, false, 0, true, 2.5f };
    n.Load(q);
    ok = ok && n.StartColour == 1 && !n.UseStartColour && n.TrailWidth == 256;
    printf(ok ? "00b027d0_TEST PASS\n" : "00b027d0_TEST FAIL\n");
    return ok ? 0 : 1;
}