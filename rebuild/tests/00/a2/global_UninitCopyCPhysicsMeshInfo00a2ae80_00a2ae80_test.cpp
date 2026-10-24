// Behaviour test for 00a2ae80: placement-copy 16-byte CPhysicsMeshInfo entries (bank + index of the CMeshDataBankEntry base, the intrusive mesh-data pointer add-ref'd, the derived vptr)
#include <cstdio>
#include <new>

struct CMeshDataBank { int Id; };
struct CIVCountedPointeeBase {
    unsigned int IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    virtual ~CIVCountedPointeeBase() { }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};
class CMeshDataBankEntry {
public:
    CMeshDataBankEntry() : Bank(0), Index(0) { }
    virtual ~CMeshDataBankEntry() { }
    virtual int Kind() const { return 1; }
    CMeshDataBank* Bank;
    unsigned int Index;
};
class CPhysicsMeshInfo : public CMeshDataBankEntry {
public:
    struct CMeshData : CIVCountedPointeeBase { };
    CPhysicsMeshInfo() { }
    virtual ~CPhysicsMeshInfo() { }
    virtual int Kind() const { return 2; }
    CIVCountedPointer<CMeshData> MeshData;
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CPhysicsMeshInfo* UninitCopy(const CPhysicsMeshInfo* first, const CPhysicsMeshInfo* last, CPhysicsMeshInfo* result, const FalseType&)
{
    CPhysicsMeshInfo* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    CMeshDataBank bank = { 7 };
    CPhysicsMeshInfo::CMeshData mesh;
    mesh.IVRefCount = 1;
    CPhysicsMeshInfo src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Bank = &bank;
        src[i].Index = 100 + i;
        src[i].MeshData.Object = (i == 1) ? 0 : &mesh;
    }
    void* raw = ::operator new(3 * sizeof(CPhysicsMeshInfo));
    CPhysicsMeshInfo* dst = static_cast<CPhysicsMeshInfo*>(raw);
    CPhysicsMeshInfo* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && mesh.IVRefCount == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Kind() == 2 && dst[i].Bank == &bank && dst[i].Index == 100u + i && dst[i].MeshData.Object == src[i].MeshData.Object;
    for (int i = 0; i < 3; ++i)
        dst[i].~CPhysicsMeshInfo();
    ::operator delete(raw);
    if (!ok) { std::printf("00a2ae80_TEST FAIL\n"); return 1; }
    std::printf("00a2ae80_TEST PASS\n");
    return 0;
}