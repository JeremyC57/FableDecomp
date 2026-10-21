#include <cstdio>
#include <vector>
// behaviour test: CTriangleStripifier2::GenerateTriangleListAndStrip flattens the triangles to an index list, runs the
// index-list overload, and rebuilds the output triangle list (resized to the input count) from the reordered indices;
// an empty input clears both outputs
struct C3DTriangle2 { unsigned short VertexIndex[3]; C3DTriangle2() { } };
struct CTriangleArray {
    std::vector<C3DTriangle2> V;
    unsigned long size() const { return (unsigned long)V.size(); }
    C3DTriangle2& operator[](unsigned long n) { return V[n]; }
    const C3DTriangle2& operator[](unsigned long n) const { return V[n]; }
    void resize(unsigned long n) { V.resize(n); }
    void clear() { V.clear(); }
};
struct CIndexArray {
    std::vector<unsigned short> V;
    CIndexArray() { }
    explicit CIndexArray(unsigned long n) : V(n) { }
    unsigned short& operator[](unsigned long n) { return V[n]; }
    void clear() { V.clear(); }
};
class CTriangleStripifier2 {
public:
    int Calls;
    void GenerateTriangleListAndStrip(const CTriangleArray& triangles, CTriangleArray& triangleList, CIndexArray& strip);
    void GenerateTriangleListAndStrip(CIndexArray& triangles, CIndexArray& triangleList, CIndexArray& strip)
    {
        ++Calls;                                         // stand-in: reverse the triangle order, strip = first index
        unsigned long n = (unsigned long)triangles.V.size() / 3;
        for (unsigned long t = n; t-- > 0;)
            for (int k = 0; k < 3; ++k)
                triangleList.V.push_back(triangles.V[t * 3 + k]);
        strip.V.push_back(triangles.V[0]);
    }
};
void CTriangleStripifier2::GenerateTriangleListAndStrip(const CTriangleArray& triangles, CTriangleArray& triangleList,
                                                        CIndexArray& strip)
{
    if (triangles.size() == 0) {
        triangleList.clear();
        strip.clear();
        return;
    }
    CIndexArray indices(triangles.size() * 3);
    CIndexArray listIndices;
    unsigned short* index = &indices[0];
    for (unsigned long i = 0; i < triangles.size(); ++i) {
        index[i * 3] = triangles[i].VertexIndex[0];
        index[i * 3 + 1] = triangles[i].VertexIndex[1];
        index[i * 3 + 2] = triangles[i].VertexIndex[2];
    }
    GenerateTriangleListAndStrip(indices, listIndices, strip);
    triangleList.resize(triangles.size());
    const unsigned short* listIndex = &listIndices[0];
    for (unsigned long i = 0; i < triangles.size(); ++i) {
        triangleList[i].VertexIndex[0] = listIndex[i * 3];
        triangleList[i].VertexIndex[1] = listIndex[i * 3 + 1];
        triangleList[i].VertexIndex[2] = listIndex[i * 3 + 2];
    }
}
int main()
{
    CTriangleStripifier2 s;
    s.Calls = 0;
    CTriangleArray in, out;
    for (int t = 0; t < 3; ++t) {
        C3DTriangle2 tri;
        for (int k = 0; k < 3; ++k) tri.VertexIndex[k] = (unsigned short)(t * 10 + k);
        in.V.push_back(tri);
    }
    for (int t = 0; t < 5; ++t) out.V.push_back(in.V[0]);   // stale contents get resized away
    CIndexArray strip;
    s.GenerateTriangleListAndStrip(in, out, strip);
    bool ok = s.Calls == 1 && out.size() == 3 && strip.V.size() == 1 && strip.V[0] == 0;
    for (int t = 0; ok && t < 3; ++t)
        for (int k = 0; k < 3; ++k)
            ok = ok && out.V[t].VertexIndex[k] == (unsigned short)((2 - t) * 10 + k);
    CTriangleArray empty;
    s.GenerateTriangleListAndStrip(empty, out, strip);
    ok = ok && s.Calls == 1 && out.size() == 0 && strip.V.empty();
    if (!ok) { std::printf("00f3f3a0_TEST FAIL\n"); return 1; }
    std::printf("00f3f3a0_TEST PASS\n");
    return 0;
}
