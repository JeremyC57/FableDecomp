// self-contained copy of the CPhysicsMeshObject model
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCharString { const char* m_pData; };
struct C3DVectorVector { C3DVector* first; C3DVector* last; C3DVector* end_of_storage; };
static int g_meshDestroyed = 0;
struct CPhysicsMesh {
    long m_refCount; bool m_navInfo; const char* m_volume; C3DVector m_points[2];
    virtual void v00() { }
    virtual void DeleteThis() { ++g_meshDestroyed; }
    bool HasContainmentVolume(const CCharString& name) const { return std::strcmp(name.m_pData, m_volume) == 0; }
    bool HasNavigationLineListInfo() const { return m_navInfo; }
    void GetRigidBodyBoundingPoints(C3DVectorVector& points) const { points.first[0] = m_points[0]; points.first[1] = m_points[1]; points.last = points.first + 2; }
};
template <class T> struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            T* object = m_pObject;
            if (object) { if (--object->m_refCount == 0) object->DeleteThis(); m_pObject = 0; }
            m_pObject = other.m_pObject;
            if (m_pObject) ++m_pObject->m_refCount;
        }
        return *this;
    }
    ~CIVCountedPointer() { T* object = m_pObject; if (object) { if (--object->m_refCount == 0) object->DeleteThis(); m_pObject = 0; } }
    T* operator->() const { return m_pObject; }
};
struct CScaleElements {
    float m_scale;
    CScaleElements(float scale) : m_scale(scale) { }
};
static CScaleElements ForEach_ScaleElements(C3DVector* first, C3DVector* last, CScaleElements f)
{ for (; first != last; ++first) { first->X *= f.m_scale; first->Y *= f.m_scale; first->Z *= f.m_scale; } return f; }
static int g_baseDestroyed = 0;
struct CBaseClass { virtual void v00() { } CBaseClass() { } ~CBaseClass() { ++g_baseDestroyed; } };
struct CPhysicsMeshObject : CBaseClass {
    CIVCountedPointer<CPhysicsMesh> Mesh;
    C3DVector m_position; CRightHandedSet m_orientation;
    float Scale; float OO_Scale;
    CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale)
    { Mesh = mesh; SetPositionMatrix(position, orientation, scale); }
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& orientation, float scale)
    { m_position = position; m_orientation = orientation; Scale = scale; OO_Scale = 1.0f / scale; }
    ~CPhysicsMeshObject() { }
    void GetRigidBodyBoundingPoints(C3DVectorVector& points) const
    {
        { CIVCountedPointer<CPhysicsMesh> mesh(Mesh); mesh->GetRigidBodyBoundingPoints(points); }
        ForEach_ScaleElements(points.first, points.last, CScaleElements(Scale));
    }
    bool HasContainmentVolume(const CCharString& name) const { CIVCountedPointer<CPhysicsMesh> mesh(Mesh); return mesh->HasContainmentVolume(name); }
    bool HasNavigationLineListInfo() const { CIVCountedPointer<CPhysicsMesh> mesh(Mesh); return mesh->HasNavigationLineListInfo(); }
};
static CPhysicsMesh* NewMesh() { CPhysicsMesh* m = new CPhysicsMesh; m->m_refCount = 0; m->m_navInfo = true; m->m_volume = "inside";
    m->m_points[0].X = 1.0f; m->m_points[0].Y = 2.0f; m->m_points[0].Z = 3.0f; m->m_points[1].X = -1.0f; m->m_points[1].Y = 0.0f; m->m_points[1].Z = 4.0f; return m; }
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    CPhysicsMesh* mesh = NewMesh();
    CIVCountedPointer<CPhysicsMesh> holder; holder.m_pObject = mesh; mesh->m_refCount = 1;
    CRightHandedSet rh; rh.Up = V(0, 0, 1); rh.Forward = V(0, 1, 0);
    {
        CPhysicsMeshObject o(holder, V(1, 2, 3), rh, 2.0f);
        if (mesh->m_refCount != 2 || o.Mesh.m_pObject != mesh || o.Scale != 2.0f || o.m_position.Y != 2.0f) { std::printf("00a12cc0_TEST FAIL\n"); return 1; }
    }
    if (mesh->m_refCount != 1 || g_meshDestroyed != 0) { std::printf("00a12cc0_TEST FAIL\n"); return 1; }
    std::printf("00a12cc0_TEST PASS\n");
    return 0;
}