#include <cstdio>
#include <cstring>
// Behaviour copy of C3DMeshPartitionBuilderElement::BuildOutput (0x00AC2A80).
struct V { float X, Y, Z; };
struct Box { V Min, Max; };
struct Sphere { V Center; float Radius; };
struct Tri { unsigned short Index[3]; };
struct Vec {
    Tri* b; Tri* f; Tri* e; int overflows;
    Vec() : b(0), f(0), e(0), overflows(0) {}
    unsigned long size() const { return (unsigned long)(f - b); }
    void overflow(const Tri& x) { ++overflows; unsigned long n = size(); Tri* nb = new Tri[n * 2 + 1];
        if (n) std::memcpy(nb, b, n * sizeof(Tri)); delete[] b; b = nb; f = nb + n; e = nb + n * 2 + 1; *f++ = x; }
    void push_back(const Tri& x) { if (f != e) { *f = x; ++f; } else overflow(x); }
};
struct OutE {
    Box BoundingBox; long StartTriangle, TriangleCount; OutE* SubElements[2]; Sphere BoundingSphere;
    OutE* BuildSubElement(long i) { OutE* n = new OutE(); std::memset(n, 0, sizeof(OutE)); SubElements[i] = n; return n; }
};
struct B {
    long StartTriangle, TriangleCount; Box BoundingBox; Sphere BoundingSphere; Vec Triangles; B* SubElements[2];
    void BuildOutput(OutE* out, Vec& tris) {
        out->BoundingBox = BoundingBox; out->BoundingSphere = BoundingSphere;
        out->StartTriangle = StartTriangle; out->TriangleCount = TriangleCount;
        if (SubElements[0]) { SubElements[0]->BuildOutput(out->BuildSubElement(0), tris); SubElements[1]->BuildOutput(out->BuildSubElement(1), tris); }
        else for (unsigned long i = 0; i < Triangles.size(); ++i) tris.push_back(Triangles.b[i]);
    }
};
int main() {
    B root, l, r; std::memset(&root.BoundingBox, 0, sizeof(Box)); root.BoundingBox.Max.X = 5; root.BoundingSphere.Radius = 7;
    root.StartTriangle = 0; root.TriangleCount = 3; root.SubElements[0] = &l; root.SubElements[1] = &r;
    l = root; r = root; l.SubElements[0] = l.SubElements[1] = 0; r.SubElements[0] = r.SubElements[1] = 0;
    Tri t1 = {{1, 2, 3}}, t2 = {{4, 5, 6}}, t3 = {{7, 8, 9}};
    l.Triangles.push_back(t1); l.Triangles.push_back(t2); r.Triangles.push_back(t3); l.StartTriangle = 0; r.StartTriangle = 2;
    Vec out; OutE o; std::memset(&o, 0, sizeof(o)); root.BuildOutput(&o, out);
    if (out.size() != 3 || out.b[0].Index[0] != 1 || out.b[1].Index[2] != 6 || out.b[2].Index[1] != 8 || !o.SubElements[0] || !o.SubElements[1]
        || o.BoundingBox.Max.X != 5 || o.BoundingSphere.Radius != 7 || o.SubElements[1]->StartTriangle != 2 || o.SubElements[0]->SubElements[0] != 0) {
        std::printf("00ac2a80_TEST FAIL\n"); return 1; }
    std::printf("00ac2a80_TEST PASS\n"); return 0;
}
