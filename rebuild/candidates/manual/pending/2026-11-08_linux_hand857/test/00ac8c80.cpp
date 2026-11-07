// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::ComputeVertexCost (0x00AC8C80): a model of the row --
// the vertex normal is the normalised sum of its faces' normals; each face's U/V tangents are projected into that plane
// (length kept); the cost is (max over U,V of |range of tangents| / max tangent length)^2 times the texture weight.
#include <stdio.h>
#include <math.h>
#include <set>
#include <vector>
struct V3 { float X, Y, Z; V3(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {} };
static V3 sub(V3 a, V3 b) { return V3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V3 cross(V3 a, V3 b) { return V3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static float dot(V3 a, V3 b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static float mag(V3 a) { return sqrtf(dot(a, a)); }
static V3 norm(V3 a) { float m = mag(a); return m > 1e-5f ? V3(a.X / m, a.Y / m, a.Z / m) : a; }
struct Face { unsigned long Idx[3]; V3 U, V; };
struct Vertex { V3 Position; float Cost; std::set<unsigned long> Faces; };
struct Prim {
    std::vector<Vertex> Vertices; std::vector<Face> Faces; float Weight, Penalty;
    V3 FaceNormal(const Face& f) const {
        V3 n = cross(sub(Vertices[f.Idx[1]].Position, Vertices[f.Idx[0]].Position), sub(Vertices[f.Idx[2]].Position, Vertices[f.Idx[0]].Position));
        return dot(n, n) > 1e-10f ? norm(n) : n;
    }
    V3 MakeOrthogonal(V3 v, V3 n) const {
        float m = mag(v); if (fabsf(m) <= 1e-5f) return v;
        float d = dot(v, n); V3 r(v.X - n.X * d, v.Y - n.Y * d, v.Z - n.Z * d);
        float s = dot(r, r); if (s > 1e-10f) { float k = m / sqrtf(s); r = V3(r.X * k, r.Y * k, r.Z * k); }
        return r;
    }
    void ComputeVertexCost(Vertex& vx) {
        float distortion = 0.0f;
        if (!vx.Faces.empty()) {
            V3 n;
            for (std::set<unsigned long>::iterator it = vx.Faces.begin(); it != vx.Faces.end(); ++it) { V3 f = FaceNormal(Faces[*it]); n = V3(n.X + f.X, n.Y + f.Y, n.Z + f.Z); }
            if (dot(n, n) > 1e-10f) n = norm(n);
            float ul = 0, vl = 0; V3 umin(1e30f, 1e30f, 1e30f), umax(-1e30f, -1e30f, -1e30f), vmin = umin, vmax = umax;
            for (std::set<unsigned long>::iterator it = vx.Faces.begin(); it != vx.Faces.end(); ++it) {
                V3 u = MakeOrthogonal(Faces[*it].U, n), v = MakeOrthogonal(Faces[*it].V, n);
                umin = V3(u.X < umin.X ? u.X : umin.X, u.Y < umin.Y ? u.Y : umin.Y, u.Z < umin.Z ? u.Z : umin.Z);
                umax = V3(u.X > umax.X ? u.X : umax.X, u.Y > umax.Y ? u.Y : umax.Y, u.Z > umax.Z ? u.Z : umax.Z);
                vmin = V3(v.X < vmin.X ? v.X : vmin.X, v.Y < vmin.Y ? v.Y : vmin.Y, v.Z < vmin.Z ? v.Z : vmin.Z);
                vmax = V3(v.X > vmax.X ? v.X : vmax.X, v.Y > vmax.Y ? v.Y : vmax.Y, v.Z > vmax.Z ? v.Z : vmax.Z);
                if (mag(u) > ul) ul = mag(u); if (mag(v) > vl) vl = mag(v);
            }
            if (fabsf(ul) > 1e-5f && mag(sub(umax, umin)) / ul > distortion) distortion = mag(sub(umax, umin)) / ul;
            if (fabsf(vl) > 1e-5f && mag(sub(vmax, vmin)) / vl > distortion) distortion = mag(sub(vmax, vmin)) / vl;
        }
        float squared = distortion * distortion;
        if (squared > 1.0f) vx.Cost = Penalty;   // retail stores the penalty, then overwrites it
        vx.Cost = squared * Weight;
    }
};
int main()
{
    Prim p; p.Weight = 2.0f; p.Penalty = 100.0f;
    Vertex a; a.Position = V3(0, 0, 0); Vertex b; b.Position = V3(1, 0, 0); Vertex c; c.Position = V3(0, 1, 0); Vertex d; d.Position = V3(-1, 0, 0);
    p.Vertices.push_back(a); p.Vertices.push_back(b); p.Vertices.push_back(c); p.Vertices.push_back(d);
    Face f0 = { {0, 1, 2}, V3(1, 0, 0), V3(0, 1, 0) };
    Face f1 = { {0, 2, 3}, V3(1, 0, 0), V3(0, 1, 0) };
    p.Faces.push_back(f0); p.Faces.push_back(f1);
    Vertex& v0 = p.Vertices[0]; v0.Faces.insert(0); v0.Faces.insert(1); v0.Cost = -1;
    p.ComputeVertexCost(v0);                       // identical tangents: no distortion
    int ok = v0.Cost == 0.0f;
    p.Faces[1].U = V3(0, 1, 0);                    // U swings 90 degrees: range sqrt(2), length 1
    p.ComputeVertexCost(v0);
    ok = ok && fabsf(v0.Cost - 4.0f) < 1e-4f;      // (sqrt 2)^2 * weight 2
    Vertex& v1 = p.Vertices[1]; v1.Cost = 7; p.ComputeVertexCost(v1);
    ok = ok && v1.Cost == 0.0f;                    // a vertex with no faces costs 0
    printf(ok ? "00ac8c80_TEST PASS\n" : "00ac8c80_TEST FAIL\n");
    return ok ? 0 : 1;
}
