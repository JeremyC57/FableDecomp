// Behaviour test for NHitLocations::CPhysicalPrimitiveMesh::GetFirstIntersectionWithTriangle (0x00815770).
#include <cstdio>
struct CThing { int id; };
struct C3DVector { float X, Y, Z; };
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; const T& operator[](long i) const { return _M_start[i]; } };
class CPhysicalPrimitiveMesh {
public:
    int calls, hitOn; const C3DVector* edges[3][2]; float radii[3];
    virtual bool GetFirstIntersectionWithLine(CThing*, const C3DVector& start, const C3DVector& end, C3DVector& position, C3DVector& normal, float radius) const
    {
        CPhysicalPrimitiveMesh* self = const_cast<CPhysicalPrimitiveMesh*>(this);
        self->edges[calls][0] = &start; self->edges[calls][1] = &end; self->radii[calls] = radius;
        bool hit = (++self->calls == hitOn);
        position.X = 10.0f * calls; position.Y = 0; position.Z = 0; normal.X = 0; normal.Y = 1; normal.Z = 0;
        return hit;
    }
    bool GetFirstIntersectionWithTriangle(CThing* thing, const vector<C3DVector>& triangle, C3DVector& position, C3DVector& normal, float radius) const
    {
        C3DVector hitPosition;
        C3DVector hitNormal;
        for (long i = 0; i != 3; ++i) {
            if (GetFirstIntersectionWithLine(thing, triangle[(i + 2) % 3], triangle[(i + 3) % 3], hitPosition, hitNormal, radius)) {
                position = hitPosition;
                normal = hitNormal;
                return true;
            }
        }
        return false;
    }
    virtual ~CPhysicalPrimitiveMesh() {}
};
int main()
{
    C3DVector v[3]; vector<C3DVector> tri; tri._M_start = v; tri._M_finish = v + 3; tri._M_end_of_storage = v + 3;
    CPhysicalPrimitiveMesh m; m.calls = 0; m.hitOn = 2; CThing t;
    C3DVector pos = { -1, -1, -1 }, nrm = { -1, -1, -1 };
    if (!m.GetFirstIntersectionWithTriangle(&t, tri, pos, nrm, 0.5f) || m.calls != 2 || pos.X != 20.0f || nrm.Y != 1.0f) { std::printf("00815770_TEST FAIL\n"); return 1; }
    if (m.edges[0][0] != &v[2] || m.edges[0][1] != &v[0] || m.edges[1][0] != &v[0] || m.edges[1][1] != &v[1] || m.radii[1] != 0.5f) { std::printf("00815770_TEST FAIL\n"); return 1; }
    m.calls = 0; m.hitOn = 9; pos.X = -1;
    if (m.GetFirstIntersectionWithTriangle(&t, tri, pos, nrm, 0.5f) || m.calls != 3 || pos.X != -1.0f || m.edges[2][0] != &v[1] || m.edges[2][1] != &v[2]) { std::printf("00815770_TEST FAIL\n"); return 1; }
    std::printf("00815770_TEST PASS\n");
    return 0;
}