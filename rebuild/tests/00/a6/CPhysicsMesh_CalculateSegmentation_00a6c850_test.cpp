#include <cstdio>
class C3DTriangle2 { public: unsigned short m_index[3]; };
class C3DVector { public: float x, y, z; };
template <class T> class CArray { public:
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
};
class CIPhysicsFacesSegmentBase { public:
    unsigned long m_faces;
    static CIPhysicsFacesSegmentBase* Create(const CArray<C3DTriangle2>& t, const CArray<C3DVector>& v)
    { static CIPhysicsFacesSegmentBase s; s.m_faces = t.size() * 1000 + v.size(); return &s; }
};
template <class T> class CCountedPointer { public:
    T* m_pObject; int m_resets;
    void Reset(T* object) { m_pObject = object; ++m_resets; }
};
class CPhysicsMesh { public:
    CArray<C3DTriangle2> TotalFaceIndices; CArray<C3DVector> TotalPoints;
    CCountedPointer<CIPhysicsFacesSegmentBase> RootSegment;
    void CalculateSegmentation()
    {
        if (TotalFaceIndices.size() > 60)
            RootSegment.Reset(CIPhysicsFacesSegmentBase::Create(TotalFaceIndices, TotalPoints));
    }
};
int main()
{
    static C3DTriangle2 tris[100]; static C3DVector verts[10];
    CPhysicsMesh m; m.RootSegment.m_pObject = 0; m.RootSegment.m_resets = 0;
    m.TotalPoints.m_pFirst = verts; m.TotalPoints.m_pLast = verts + 7; m.TotalPoints.m_pEnd = verts + 10;
    m.TotalFaceIndices.m_pFirst = tris; m.TotalFaceIndices.m_pLast = tris + 60; m.TotalFaceIndices.m_pEnd = tris + 100;
    m.CalculateSegmentation();
    if (m.RootSegment.m_resets != 0 || m.RootSegment.m_pObject != 0) { std::printf("00a6c850_TEST FAIL\n"); return 1; }
    m.TotalFaceIndices.m_pLast = tris + 61;
    m.CalculateSegmentation();
    if (m.RootSegment.m_resets != 1 || m.RootSegment.m_pObject == 0 || m.RootSegment.m_pObject->m_faces != 61007) { std::printf("00a6c850_TEST FAIL\n"); return 1; }
    std::printf("00a6c850_TEST PASS\n");
    return 0;
}