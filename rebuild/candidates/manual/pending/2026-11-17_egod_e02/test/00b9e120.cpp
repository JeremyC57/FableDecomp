// Behaviour test for CEngineInternalPrimitive2DStaticMesh::Initialise (0x00B9E120): view_z = the 2D layer's view distance; the world
// position is the clip box centre at view_z taken from screen to view space ((x - centre) / half-width * z, (centre - y) / half-height * z)
// and through the camera's inverse view transform to world space; Scale = the smaller half extent of the clip box, converted to a view
// distance at view_z (* OneOverHalfWindowWidth * z / HomogenousViewScaleX) and divided by the bounding-sphere radius; Initialised set.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
struct M { float E[4][3]; V Transform(const V& p) const {
    V r = { p.X * E[0][0] + p.Y * E[1][0] + p.Z * E[2][0] + E[3][0],
            p.X * E[0][1] + p.Y * E[1][1] + p.Z * E[2][1] + E[3][1],
            p.X * E[0][2] + p.Y * E[1][2] + p.Z * E[2][2] + E[3][2] }; return r; } };
struct Camera { float OneOverHalfWindowWidth, OneOverHalfWindowHeight, WindowCenterX, WindowCenterY, HomogenousViewScaleX; M InverseViewTransform; };
struct Box { float TLX, TLY, BRX, BRY; };
struct Mesh {
    bool Initialised; Box ClipBox; int Layer; float Radius; V WorldPos; float Scale;
    void Initialise(const Camera& cam, float layer_distance)
    {
        const float view_z = layer_distance;
        const float half_clip_box_width = (ClipBox.BRX - ClipBox.TLX) * 0.5f;
        const float half_clip_box_height = (ClipBox.BRY - ClipBox.TLY) * 0.5f;
        const float half_clip_square_width = half_clip_box_width < half_clip_box_height ? half_clip_box_width : half_clip_box_height;
        V p = { ClipBox.TLX + half_clip_box_width, ClipBox.TLY + half_clip_box_height, view_z };
        p.X = (p.X - cam.WindowCenterX) * cam.OneOverHalfWindowWidth * p.Z;
        p.Y = (cam.WindowCenterY - p.Y) * cam.OneOverHalfWindowHeight * p.Z;
        WorldPos = cam.InverseViewTransform.Transform(p);
        Scale = half_clip_square_width;
        Scale = cam.OneOverHalfWindowWidth * Scale * view_z / cam.HomogenousViewScaleX;
        Initialised = true;
        Scale /= Radius;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static int fail(int n) { std::printf("00b9e120_TEST FAIL %d\n", n); return 1; }

int main()
{
    Camera cam = { 1.0f / 320.0f, 1.0f / 240.0f, 320.0f, 240.0f, 2.0f, { { { 0, 1, 0 }, { -1, 0, 0 }, { 0, 0, 1 }, { 10, 20, 30 } } } };
    Mesh m = { false, { 100, 40, 300, 440 }, 0, 4.0f, { 0, 0, 0 }, 0 };   // 200 wide, 400 high: width is smaller
    m.Initialise(cam, 8.0f);
    // centre (200, 240): view X = (200-320)/320*8 = -3, Y = (240-240)/240*8 = 0, Z = 8; world = +(10,20,30)
    if (!m.Initialised || !near(m.WorldPos.X, 10.0f) || !near(m.WorldPos.Y, 17.0f) || !near(m.WorldPos.Z, 38.0f)) return fail(1);
    if (!near(m.Scale, 100.0f / 320.0f * 8.0f / 2.0f / 4.0f)) return fail(2);
    Mesh n = { false, { 0, 0, 640, 100 }, 0, 2.0f, { 0, 0, 0 }, 0 };     // height is smaller
    n.Initialise(cam, 4.0f);
    if (!near(n.Scale, 50.0f / 320.0f * 4.0f / 2.0f / 2.0f)) return fail(3);
    if (!near(n.WorldPos.X, 10.0f - (240.0f - 50.0f) / 240.0f * 4.0f) || !near(n.WorldPos.Y, 20.0f) || !near(n.WorldPos.Z, 34.0f)) return fail(4);
    std::printf("00b9e120_TEST PASS\n");
    return 0;
}
