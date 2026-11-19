#include <stdio.h>
#include <math.h>
class C3DVector { public: float X, Y, Z;
    C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& p) { X = p.X; Y = p.Y; Z = p.Z; } };
class C2DVector { public: float X, Y;
    C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector& AccurateNormalise() { float m = sqrtf(X * X + Y * Y); if (m > 0.0f) { X /= m; Y /= m; } return *this; }
    float Dot(const C2DVector& vector) const { return X * vector.X + Y * vector.Y; } };
enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION { LTMD_TOP, LTMD_EAST, LTMD_WEST, LTMD_NORTH, LTMD_SOUTH };
class CEngineLandscapeMeshBuilder {
    static C3DVector MappingDirNormals[];
public:
    float GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION dir, const C3DVector& normal);
};
C3DVector CEngineLandscapeMeshBuilder::MappingDirNormals[5] = {
    C3DVector(0, 0, 1), C3DVector(1, 0, 0), C3DVector(-1, 0, 0), C3DVector(0, 1, 0), C3DVector(0, -1, 0) };
static const float maps[5][2] = { {0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

float CEngineLandscapeMeshBuilder::GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION dir, const C3DVector& normal)
{
    C3DVector map_normal = MappingDirNormals[dir];
    float vert_angle = asin(normal.Z);
    float base_blend = (vert_angle * 0.63661975f - 0.5f) * 4.0f;
    if (base_blend < 0.0f)
        base_blend = 0.0f;
    else if (base_blend > 1.0f)
        base_blend = 1.0f;
    if (dir == 0)
        return base_blend;
    if (base_blend == 1.0f)
        return 0.0f;
    C2DVector normal2d(normal);
    normal2d.AccurateNormalise();
    float dot = normal2d.Dot(C2DVector(map_normal));
    float blend = 1.0f - (acos(dot) * 0.63661975f - 0.25f) * 2.0f;
    if (blend < 0.0f)
        blend = 0.0f;
    else if (blend > 1.0f)
        blend = 1.0f;
    if (base_blend == 0.0f)
        return blend;
    if (blend == 0.0f)
        return 0.0f;
    return (1.0f - base_blend) * blend;
}

static double clamp01(double v) { return v < 0 ? 0 : (v > 1 ? 1 : v); }
static double expect(int dir, double x, double y, double z)
{
    const double k = 2.0 / 3.14159265358979;
    double base = clamp01((asin(z) * k - 0.5) * 4.0);
    if (dir == 0) return base;
    if (base == 1.0) return 0.0;
    double m = sqrt(x * x + y * y), nx = m > 0 ? x / m : x, ny = m > 0 ? y / m : y;
    double d = nx * maps[dir][0] + ny * maps[dir][1];
    if (d > 1) d = 1; if (d < -1) d = -1;
    double blend = clamp01(1.0 - (acos(d) * k - 0.25) * 2.0);
    if (base == 0.0) return blend;
    if (blend == 0.0) return 0.0;
    return (1.0 - base) * blend;
}
int main()
{
    CEngineLandscapeMeshBuilder b;
    static const float n[][3] = { {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}, {0.6f, 0.0f, 0.8f}, {0.0f, -0.6f, 0.8f},
        {0.8f, 0.36f, 0.48f}, {-0.5f, 0.5f, 0.7071068f}, {0.3f, -0.9f, 0.3162278f}, {0.7f, 0.7f, 0.141f}, {0.42f, 0.42f, 0.8044f} };
    int checks = 0, zero = 0, one = 0, mid = 0;
    for (unsigned i = 0; i < sizeof(n) / sizeof(n[0]); ++i)
        for (int d = 0; d < 5; ++d) {
            float got = b.GetMappingDirectionBlend((LANDSCAPE_TEXTURE_MAPPING_DIRECTION)d, C3DVector(n[i][0], n[i][1], n[i][2]));
            double want = expect(d, n[i][0], n[i][1], n[i][2]);
            if (fabs(got - want) > 2e-4) { printf("FAIL dir %d normal %u: got %f want %f\n", d, i, got, want); return 1; }
            ++checks; if (want == 0) ++zero; else if (want == 1) ++one; else ++mid;
        }
    if (checks != 55 || zero < 5 || one < 2 || mid < 5) { printf("FAIL coverage %d %d %d\n", zero, one, mid); return 2; }
    puts("00be2b70_TEST PASS");
    return 0;
}