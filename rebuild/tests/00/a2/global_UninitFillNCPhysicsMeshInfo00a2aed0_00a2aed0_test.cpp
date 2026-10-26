#include <cstdio>
#include <new>

// Model of 0x00A2AED0: placement-copy `count` CPhysicsMeshInfo records (base CMeshDataBankEntry {Bank, Index},
// own CIVCountedPointer MeshData whose pointee keeps IVRefCount), returning the advanced pointer.
class CMeshDataBank { public: int Id; };
class CMeshDataBankEntry { public:
    virtual int Kind() const { return 0xCD28; }
    CMeshDataBank* Bank;
    unsigned long Index;
    CMeshDataBankEntry(CMeshDataBank* b, unsigned long i) : Bank(b), Index(i) { }
};
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    explicit CIVCountedPointer(CIVCountedPointeeBase* o) : Object(o) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};
class CPhysicsMeshInfo : public CMeshDataBankEntry { public:
    virtual int Kind() const { return 0xCD50; }
    CIVCountedPointer MeshData;
    CPhysicsMeshInfo(CMeshDataBank* b, unsigned long i, CIVCountedPointeeBase* m) : CMeshDataBankEntry(b, i), MeshData(m) { }
};
struct CRetailAllocator { };

static CPhysicsMeshInfo* UninitFillN(CPhysicsMeshInfo* first, unsigned long count, const CPhysicsMeshInfo& value, CRetailAllocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CPhysicsMeshInfo(value);
    return first;
}

static double g_storage[128];

static int Fail()
{
    std::printf("00a2aed0_TEST FAIL\n");
    return 1;
}

int main()
{
    CMeshDataBank bank = { 3 };
    CIVCountedPointeeBase mesh;
    CPhysicsMeshInfo value(&bank, 17, &mesh);
    CPhysicsMeshInfo* buf = (CPhysicsMeshInfo*)g_storage;
    CPhysicsMeshInfo* end = UninitFillN(buf, 4, value, CRetailAllocator());
    if (end != buf + 4 || mesh.IVRefCount != 5)
        return Fail();
    for (int i = 0; i < 4; ++i) {
        const CMeshDataBankEntry& entry = buf[i];
        if (buf[i].Bank != &bank || buf[i].Index != 17 || buf[i].MeshData.Object != &mesh || entry.Kind() != 0xCD50)
            return Fail();
    }
    if (UninitFillN(buf, 0, value, CRetailAllocator()) != buf || mesh.IVRefCount != 5)
        return Fail();
    CPhysicsMeshInfo empty(&bank, 2, 0);
    if (UninitFillN(buf + 6, 1, empty, CRetailAllocator()) != buf + 7 || buf[6].MeshData.Object != 0 || buf[6].Index != 2)
        return Fail();
    std::printf("00a2aed0_TEST PASS\n");
    return 0;
}