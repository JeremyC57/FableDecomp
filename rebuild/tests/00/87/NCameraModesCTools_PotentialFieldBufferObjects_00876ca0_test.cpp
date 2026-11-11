// Behaviour test for 0x00876CA0 NCameraModes::CTools::PotentialFieldBufferObjects: the row's body over test doubles (a list
// of objects with positions and mesh extents, the step and the four sample offsets); checks the empty list (zero gradient)
// and a two-object field against a direct evaluation of the forward differences.
#include <cstdio>
#include <cmath>
#include <list>
namespace field_test {
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetMagnitude() const { return (float)std::sqrt(X * X + Z * Z + Y * Y); }
};
struct CThing { C3DVector pos; float extent; const C3DVector& GetPos() const { return pos; } };
struct CThingGameObject : CThing { };
float g_PotentialFieldSampleStep = 0.1f;
C3DVector g_PotentialFieldSampleOffsets[4] = { C3DVector(0, 0, 0), C3DVector(1, 0, 0), C3DVector(0, 1, 0), C3DVector(0, 0, 1) };
struct CTools {
    float GetMeshExtent(const CThing& thing) const { return thing.extent; }
    float PotentialField(const C3DVector& pos, const C3DVector& source, float scale, float r_prime) const
    {
        return (scale * 0.1f) / ((pos - source).GetMagnitude() - r_prime);
    }
    C3DVector PotentialFieldBufferObjects(const C3DVector& field_is_at, const std::list<const CThingGameObject*>& objects) const;
};
// --- the row's body ---
C3DVector CTools::PotentialFieldBufferObjects(const C3DVector& field_is_at, const std::list<const CThingGameObject*>& objects) const
{
    float field_at_pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (std::list<const CThingGameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        for (long counter = 0; counter < 4; ++counter) {
            C3DVector pos_to_check = field_is_at + g_PotentialFieldSampleOffsets[counter] * g_PotentialFieldSampleStep;
            float extent = GetMeshExtent(**it);
            C3DVector object_pos = (*it)->GetPos();
            field_at_pos[counter] += PotentialField(pos_to_check, object_pos, extent, (float)std::sqrt(extent));
        }
    }
    return C3DVector((field_at_pos[1] - field_at_pos[0]) / g_PotentialFieldSampleStep,
                     (field_at_pos[2] - field_at_pos[0]) / g_PotentialFieldSampleStep,
                     (field_at_pos[3] - field_at_pos[0]) / g_PotentialFieldSampleStep);
}
static double field(double px, double py, double pz, const CThing& t)
{
    double dx = px - t.pos.X, dy = py - t.pos.Y, dz = pz - t.pos.Z;
    return (t.extent * 0.1) / (std::sqrt(dx * dx + dy * dy + dz * dz) - std::sqrt((double)t.extent));
}
static bool near(double a, double b) { return std::fabs(a - b) <= 1e-3 * (1.0 + std::fabs(b)); }
}
int main()
{
    using namespace field_test;
    CTools tools;
    std::list<const CThingGameObject*> objects;
    C3DVector at(1.0f, 2.0f, 3.0f);
    C3DVector g = tools.PotentialFieldBufferObjects(at, objects);
    bool ok = g.X == 0.0f && g.Y == 0.0f && g.Z == 0.0f;
    CThingGameObject a, b;
    a.pos = C3DVector(5.0f, 2.0f, 3.0f); a.extent = 4.0f;
    b.pos = C3DVector(1.0f, -6.0f, 0.0f); b.extent = 9.0f;
    objects.push_back(&a);
    objects.push_back(&b);
    g = tools.PotentialFieldBufferObjects(at, objects);
    double f0 = field(1, 2, 3, a) + field(1, 2, 3, b);
    double fx = field(1.1, 2, 3, a) + field(1.1, 2, 3, b);
    double fy = field(1, 2.1, 3, a) + field(1, 2.1, 3, b);
    double fz = field(1, 2, 3.1, a) + field(1, 2, 3.1, b);
    ok = ok && near(g.X, (fx - f0) / 0.1) && near(g.Y, (fy - f0) / 0.1) && near(g.Z, (fz - f0) / 0.1);
    ok = ok && g.X > 0.0 && g.Y < 0.0;   // the field grows towards a (on +X) and away from b (on -Y)
    std::printf(ok ? "00876ca0_TEST PASS\n" : "00876ca0_TEST FAIL\n");
    return ok ? 0 : 1;
}