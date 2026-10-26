// self-contained copy of the CPhysicsMeshObject model
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CCharString { public: const char* m_pData; };
struct C3DVectorVector { C3DVector* first; C3DVector* last; C3DVector* end_of_storage; };
static int g_meshDestroyed = 0;
class CPhysicsMesh { public:
    long m_refCount; bool m_navInfo; const char* m_volume; C3DVector m_points[2];
    virtual void v00() { }
    virtual void DeleteThis() { ++g_meshDestroyed; }
    bool HasContainmentVolume(const CCharString& name) const { return std::strcmp(name.m_pData, m_volume) == 0; }
    bool HasNavigationLineListInfo() const { return m_navInfo; }
    void GetRigidBodyBoundingPoints(C3DVectorVector& points) const { points.first[0] = m_points[0]; points.first[1] = m_points[1]; points.last = points.first + 2; }
};
template <class T> class CIVCountedPointer { public:
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
class CBaseClass { public: virtual void v00() { } CBaseClass() { } ~CBaseClass() { ++g_baseDestroyed; } };
class CPhysicsMeshObject : public CBaseClass { public:
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
        CPhysicsMeshObject* o = new CPhysicsMeshObject(holder, V(1, 2, 3), rh, 2.0f);
        holder.~CIVCountedPointer<CPhysicsMesh>(); holder.m_pObject = 0;
        if (mesh->m_refCount != 1 || g_meshDestroyed != 0) { std::printf("00a127d0_TEST FAIL\n"); return 1; }
        delete o;
        if (g_meshDestroyed != 1 || g_baseDestroyed != 1) { std::printf("00a127d0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a127d0_TEST PASS\n");
    return 0;
}