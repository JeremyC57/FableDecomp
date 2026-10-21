#include <cstdio>
// Behaviour copy of C3DMeshPartitionElement::WriteToFile (0x00AC27A0): pre-order tree write with bool markers.
struct Out {
    float f[200]; long l[200]; int b[200]; int nf, nl, nb; int order[600]; int n;
    Out() : nf(0), nl(0), nb(0), n(0) {}
    void WriteFloat(float v) { f[nf++] = v; order[n++] = 0; }
    void WriteLong(long v) { l[nl++] = v; order[n++] = 1; }
    void WriteBool(bool v) { b[nb++] = v ? 1 : 0; order[n++] = 2; }
};
struct V { float X, Y, Z; V() {} V(const V& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Sphere { V Center; float Radius; V GetCenter() const { return Center; } float GetRadius() const { return Radius; } };
struct E {
    V Min, Max; long StartTriangle, TriangleCount; E* SubElements[2]; Sphere BoundingSphere;
    void WriteToFile(Out& out) const {
        out.WriteFloat(Min.X); out.WriteFloat(Min.Y); out.WriteFloat(Min.Z);
        out.WriteFloat(Max.X); out.WriteFloat(Max.Y); out.WriteFloat(Max.Z);
        out.WriteFloat(BoundingSphere.GetCenter().X); out.WriteFloat(BoundingSphere.GetCenter().Y);
        out.WriteFloat(BoundingSphere.GetCenter().Z); out.WriteFloat(BoundingSphere.GetRadius());
        out.WriteLong(StartTriangle); out.WriteLong(TriangleCount);
        if (SubElements[0]) { out.WriteBool(true); SubElements[0]->WriteToFile(out); SubElements[1]->WriteToFile(out); }
        else out.WriteBool(false);
    }
};
static void init(E& e, long s) {
    e.Min.X = e.Min.Y = e.Min.Z = 0; e.Max.X = e.Max.Y = e.Max.Z = 1; e.BoundingSphere.Center.X = (float)s;
    e.BoundingSphere.Center.Y = 2; e.BoundingSphere.Center.Z = 3; e.BoundingSphere.Radius = 4;
    e.StartTriangle = s; e.TriangleCount = s * 10; e.SubElements[0] = e.SubElements[1] = 0;
}
int main() {
    E root, a, b; init(root, 1); init(a, 2); init(b, 3); root.SubElements[0] = &a; root.SubElements[1] = &b;
    Out o; root.WriteToFile(o);
    // root: 10 floats, 2 longs, true; a: ..., false; b: ..., false
    if (o.nf != 30 || o.nl != 6 || o.nb != 3 || o.b[0] != 1 || o.b[1] != 0 || o.b[2] != 0 || o.l[2] != 2 || o.l[4] != 3
        || o.f[6] != 1.0f || o.f[16] != 2.0f || o.f[26] != 3.0f || o.f[9] != 4.0f || o.order[12] != 2) {
        std::printf("00ac27a0_TEST FAIL\n"); return 1; }
    std::printf("00ac27a0_TEST PASS\n"); return 0;
}
