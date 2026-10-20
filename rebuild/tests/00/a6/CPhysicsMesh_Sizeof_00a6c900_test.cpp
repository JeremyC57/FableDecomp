#include <stdio.h>
// Behaviour test for CPhysicsMesh::Sizeof (0x00A6C900): 252 fixed bytes plus the sized/reserved containers plus the
// root segment's own Sizeof when there is one.
struct C3DTriangle2 { short Index[3]; };
struct C3DVector { float X, Y, Z; };
struct CArrayOfLines { void* a; void* b; void* c; };
struct CHelperPointPair { unsigned char _d[20]; };
struct CDummyObjectPair { unsigned char _d[56]; };
struct CMeshVolumePair { unsigned char _d[16]; };
template <class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    long capacity() const { return _M_end_of_storage - _M_start; }
    void Set(long used, long reserved) { _M_start = (T*)0x1000; _M_finish = _M_start + used; _M_end_of_storage = _M_start + reserved; }
};
template <class T> struct CVectorMap { CArray<T> Data; long Compare; };
struct CIPhysicsFacesSegmentBase {
    virtual void v00() {}
    virtual long Sizeof() const { return 1000; }
};
template <class T> struct CCountedPointer {
    T* Data;
    T* operator->() const { return Data; }
    bool IsValid() const { return Data != 0; }
};
class CPhysicsMesh {
public:
    long m_04; unsigned long ReferenceCount; bool NavigationLineListInfo;
    CVectorMap<CHelperPointPair>  HelperPoints;
    CVectorMap<CDummyObjectPair>  DummyObjects;
    CArray<C3DTriangle2>          TotalFaceIndices;
    CArray<C3DVector>             TotalPoints;
    CArray<CArrayOfLines>         NavigationLines;
    CVectorMap<CMeshVolumePair>   NavigationContainmentVolumes;
    CVectorMap<CMeshVolumePair>   ContainmentVolumes;
    unsigned char                 _pad74[0xB4 - 0x74];
    CCountedPointer<const CIPhysicsFacesSegmentBase> RootSegment;
    long Sizeof() const;
};
long CPhysicsMesh::Sizeof() const
{
    long size = 252
              + DummyObjects.Data.size() * sizeof(CDummyObjectPair)
              + HelperPoints.Data.size() * sizeof(CHelperPointPair)
              + TotalFaceIndices.capacity() * sizeof(C3DTriangle2)
              + TotalPoints.capacity() * sizeof(C3DVector)
              + NavigationLines.capacity() * sizeof(CArrayOfLines)
              + ContainmentVolumes.Data.capacity() * sizeof(CMeshVolumePair)
              + NavigationContainmentVolumes.Data.capacity() * sizeof(CMeshVolumePair);
    if (RootSegment.IsValid())
        size += RootSegment->Sizeof();
    return size;
}
int main()
{
    int ok = 1;
    CPhysicsMesh m;
    m.HelperPoints.Data.Set(2, 4); m.DummyObjects.Data.Set(3, 3);
    m.TotalFaceIndices.Set(1, 10); m.TotalPoints.Set(2, 5); m.NavigationLines.Set(0, 2);
    m.NavigationContainmentVolumes.Data.Set(1, 3); m.ContainmentVolumes.Data.Set(0, 1);
    m.RootSegment.Data = 0;
    long expected = 252 + 3 * 56 + 2 * 20 + 10 * 6 + 5 * 12 + 2 * 12 + 1 * 16 + 3 * 16;
    if (m.Sizeof() != expected) ok = 0;
    CIPhysicsFacesSegmentBase root; m.RootSegment.Data = &root;
    if (m.Sizeof() != expected + 1000) ok = 0;
    m.HelperPoints.Data.Set(0, 0); m.DummyObjects.Data.Set(0, 0); m.TotalFaceIndices.Set(0, 0); m.TotalPoints.Set(0, 0);
    m.NavigationLines.Set(0, 0); m.NavigationContainmentVolumes.Data.Set(0, 0); m.ContainmentVolumes.Data.Set(0, 0); m.RootSegment.Data = 0;
    if (m.Sizeof() != 252) ok = 0;
    if (ok) { printf("00a6c900_TEST PASS\n"); return 0; }
    printf("00a6c900_TEST FAIL\n"); return 1;
}