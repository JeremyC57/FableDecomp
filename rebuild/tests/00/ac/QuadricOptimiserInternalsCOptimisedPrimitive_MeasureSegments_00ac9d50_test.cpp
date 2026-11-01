// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::MeasureSegments (0x00AC9D50): per segment, the
// bounding box of every vertex position of the faces painted with that segment; SegmentSizes gets the box diagonal
// length times Parameters.Scaling, resized to SegmentCount (a segment with no faces measures the FLT_MIN-FLT_MAX box).
#include <cstdio>
#include <cmath>
#include <vector>
struct Vec { float X, Y, Z; Vec() { } Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    Vec operator-(const Vec& o) const { return Vec(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)std::sqrt(Z * Z + Y * Y + X * X); } };
struct Vertex { Vec Position; };
struct Face { long VertexIndices[3]; long Segment; };
struct Primitive {
    std::vector<Vertex> Vertices; std::vector<Face> Faces; long SegmentCount; std::vector<float> SegmentSizes; float Scaling;
    void MeasureVertex(Vec& p, const Face& face, long vertex, std::vector<Vec>& mins, std::vector<Vec>& maxs)
    {
        p = Vertices[face.VertexIndices[vertex]].Position;
        Vec& mn = mins[face.Segment];
        if (mn.X > p.X) mn.X = p.X; if (mn.Y > p.Y) mn.Y = p.Y; if (mn.Z > p.Z) mn.Z = p.Z;
        Vec& mx = maxs[face.Segment];
        if (mx.X < p.X) mx.X = p.X; if (mx.Y < p.Y) mx.Y = p.Y; if (mx.Z < p.Z) mx.Z = p.Z;
    }
    void MeasureSegments()
    {
        long faceCount = (long)Faces.size();
        std::vector<Vec> mins(SegmentCount, Vec(3.402823466e+38F, 3.402823466e+38F, 3.402823466e+38F));
        std::vector<Vec> maxs(SegmentCount, Vec(1.175494351e-38F, 1.175494351e-38F, 1.175494351e-38F));
        for (long i = 0; i < faceCount; ++i) {
            Face& face = Faces[i]; Vec p;
            MeasureVertex(p, face, 0, mins, maxs); MeasureVertex(p, face, 1, mins, maxs); MeasureVertex(p, face, 2, mins, maxs);
        }
        SegmentSizes.resize(SegmentCount);
        for (long i = 0; i < SegmentCount; ++i) SegmentSizes[i] = (maxs[i] - mins[i]).GetLength() * Scaling;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (std::fabs(b) + 1.0f); }
int main()
{
    Primitive prim; prim.Scaling = 2.0f; prim.SegmentCount = 3;
    Vertex v; const float pts[5][3] = { { 0, 0, 0 }, { 1, 2, 2 }, { -1, 0, 0 }, { 0, 3, 4 }, { 5, 5, 5 } };
    for (int i = 0; i < 5; ++i) { v.Position = Vec(pts[i][0], pts[i][1], pts[i][2]); prim.Vertices.push_back(v); }
    Face f;
    f.VertexIndices[0] = 0; f.VertexIndices[1] = 1; f.VertexIndices[2] = 2; f.Segment = 0; prim.Faces.push_back(f);
    f.VertexIndices[0] = 0; f.VertexIndices[1] = 3; f.VertexIndices[2] = 2; f.Segment = 0; prim.Faces.push_back(f);
    f.VertexIndices[0] = 4; f.VertexIndices[1] = 4; f.VertexIndices[2] = 4; f.Segment = 2; prim.Faces.push_back(f);
    prim.SegmentSizes.resize(7, -1.0f);                 // shrinks to 3
    prim.MeasureSegments();
    if (prim.SegmentSizes.size() != 3) { std::printf("00ac9d50_TEST FAIL size\n"); return 1; }
    // segment 0: box (-1,0,0)..(1,3,4): diagonal sqrt(4+9+16) = sqrt(29), scaled by 2
    if (!Near(prim.SegmentSizes[0], 2.0f * (float)std::sqrt(29.0))) { std::printf("00ac9d50_TEST FAIL seg0 %g\n", prim.SegmentSizes[0]); return 1; }
    // segment 1: untouched: (FLT_MIN - FLT_MAX) diagonal = FLT_MAX * sqrt(3) overflows to +inf in float, times 2 stays +inf
    if (!(prim.SegmentSizes[1] > 3.0e38f)) { std::printf("00ac9d50_TEST FAIL seg1 %g\n", prim.SegmentSizes[1]); return 1; }
    // segment 2: a degenerate face at (5,5,5): zero box
    if (!Near(prim.SegmentSizes[2], 0.0f)) { std::printf("00ac9d50_TEST FAIL seg2 %g\n", prim.SegmentSizes[2]); return 1; }
    prim.SegmentCount = 4; prim.MeasureSegments();         // grows to 4 (segment 3 empty)
    if (prim.SegmentSizes.size() != 4 || !Near(prim.SegmentSizes[0], 2.0f * (float)std::sqrt(29.0))) { std::printf("00ac9d50_TEST FAIL grow\n"); return 1; }
    std::printf("00ac9d50_TEST PASS\n");
    return 0;
}