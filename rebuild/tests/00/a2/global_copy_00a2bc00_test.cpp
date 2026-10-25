// Behaviour test for __copy<CPhysicsMeshInfo*> (0x00A2BC00): forward assignment of a range keeps each destination's
// dynamic type, copies Bank/Index, swaps the counted mesh pointer (old pointee released and deleted at zero, new one
// addref'd, self-shared pointer untouched), returns result + n, and an empty range returns result unchanged.
#include <cstdio>

class CMeshDataBank { public: int Id; };

class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry() {}
    virtual int Kind() const { return 0; }
    CMeshDataBank* Bank;
    unsigned long  Index;
};

static int g_deleted = 0;

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            CIVCountedPointeeBase* old = Object;
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {
public:
    class CMeshData;
    virtual int Kind() const { return 1; }
    CIVCountedPointer<CMeshData> MeshData;
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

int main()
{
    int ok = 1;
    CMeshDataBank b1, b2; b1.Id = 1; b2.Id = 2;
    CIVCountedPointeeBase m1, m2, m3;
    m1.IVRefCount = 0; m2.IVRefCount = 0; m3.IVRefCount = 0;
    CPhysicsMeshInfo src[3], dst[4];
    src[0].Bank = &b1; src[0].Index = 10; src[0].MeshData = CIVCountedPointer<CPhysicsMeshInfo::CMeshData>(); src[0].MeshData.Object = &m1; m1.IVRefCount = 1;
    src[1].Bank = &b2; src[1].Index = 11; src[1].MeshData.Object = &m2; m2.IVRefCount = 1;
    src[2].Bank = &b1; src[2].Index = 12; src[2].MeshData.Object = 0;
    for (int i = 0; i < 4; ++i) { dst[i].Bank = 0; dst[i].Index = 99; }
    dst[0].MeshData.Object = &m3; m3.IVRefCount = 1;   // released to zero -> Delete
    dst[1].MeshData.Object = &m2; m2.IVRefCount = 2;   // already shares src[1]'s pointee -> untouched
    dst[2].MeshData.Object = &m1; m1.IVRefCount = 2;   // dropped to 1, src[0] still holds it
    dst[3].MeshData.Object = 0;
    int* dist = 0;
    CPhysicsMeshInfo* r = std::__copy(src, src + 3, dst, std::random_access_iterator_tag(), dist);
    if (r != dst + 3) ok = 0;
    for (int i = 0; i < 3; ++i)
        if (dst[i].Kind() != 1 || dst[i].Bank != src[i].Bank || dst[i].Index != src[i].Index || dst[i].MeshData.Object != src[i].MeshData.Object) ok = 0;
    if (g_deleted != 1 || m3.IVRefCount != 0) ok = 0;
    if (m1.IVRefCount != 2 || m2.IVRefCount != 2) ok = 0;
    if (dst[3].Index != 99 || dst[3].Bank != 0) ok = 0;
    CPhysicsMeshInfo* e = std::__copy(src, src, dst + 1, std::random_access_iterator_tag(), dist);
    if (e != dst + 1 || g_deleted != 1) ok = 0;

    if (!ok) { std::printf("00a2bc00_TEST FAIL\n"); return 1; }
    std::printf("00a2bc00_TEST PASS\n");
    return 0;
}