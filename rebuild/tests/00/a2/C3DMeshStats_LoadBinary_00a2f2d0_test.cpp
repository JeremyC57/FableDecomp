// 00a2f2d0 behaviour test: C3DMeshStats::LoadBinary (match.cpp body, operator[] as *(begin() + n)),
// host stand-ins over a byte buffer. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

class CMovableResourceMemoryPool { public: int Id; };
class CCharString { public: std::string S; };
class C3DVector { public: float X, Y, Z; };

static int g_overflowCalls = 0;
class CMemoryDataInputStream { public:
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    std::vector<unsigned char> Overflow;   // bytes served once the chunk runs out
    size_t OverflowPos;
    virtual ~CMemoryDataInputStream() { }
    virtual void Read(void* buffer, unsigned long size) { ReadInline(buffer, (long)size); }
    virtual CCharString ReadNullTerminatedString()
    {
        CCharString s;
        unsigned char c;
        for (;;) { ReadInline(&c, 1); if (!c) break; s.S += (char)c; }
        return s;
    }
    C3DVector Read3DVector() { C3DVector v; v.X = ReadFloat(); v.Y = ReadFloat(); v.Z = ReadFloat(); return v; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++g_overflowCalls;
        for (long i = 0; i < size; ++i) {
            if (m_remaining > 0) { buffer[i] = *m_pData++; --m_remaining; }
            else buffer[i] = Overflow[OverflowPos++];
        }
        m_position += size;
    }
    static bool IsValidPosition(i64 position) { return position >= 0 && position <= 0x7fffffff; }
    void ReadInline(void* buffer, long size)
    {
        i64 newPosition = (i64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    bool ReadBool() { unsigned char value; ReadInline(&value, sizeof(value)); return value != 0; }
    unsigned long ReadULong() { unsigned long value = 0; ReadInline(&value, 4); return value; }
    float ReadFloat() { float value; ReadInline(&value, sizeof(value)); return value; }
    unsigned short ReadUShort() { unsigned short value; ReadInline(&value, sizeof(value)); return value; }
};

class C3DBoundingSphere { public: C3DVector Centre; float Radius; };
class C3DBoundingBox { public:
    C3DVector Min; C3DVector Max;
    void Set(const C3DVector& mn, const C3DVector& mx) { Min = mn; Max = mx; }
};
class C3DHelperPoint { public: unsigned char _data[0x14]; };
class C3DDummyObject { public: unsigned char _data[0x38]; };
static int g_volumeFailAt = -1, g_volumeLoads = 0;
class C3DMeshVolume { public:
    unsigned short Tag;
    CMovableResourceMemoryPool* Pool;
    bool LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool)
    { Tag = in->ReadUShort(); Pool = pool; return g_volumeLoads++ != g_volumeFailAt; }
};
class C3DGeneratorObject { public:
    unsigned char Kind;
    void LoadBinary(CMemoryDataInputStream* in) { unsigned char k; in->Read(&k, 1); Kind = k; }
};
template <class T> struct CPreAllocVectorMap {
    T* Data;
    unsigned long Count;
    bool Sorted;
};
struct CMeshVolumeEntry { unsigned long Key; C3DMeshVolume Value; };
struct CMeshVolumeMap {
    std::vector<CMeshVolumeEntry> V;
    CMeshVolumeEntry* _M_start;
    bool Sorted;
    unsigned long size() const { return (unsigned long)V.size(); }
    CMeshVolumeEntry* begin() { return _M_start; }
    CMeshVolumeEntry& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n) { V.resize(n); _M_start = n ? &V[0] : 0; }
};
struct CGeneratorArray {
    std::vector<C3DGeneratorObject> V;
    C3DGeneratorObject* _M_start;
    unsigned long size() const { return (unsigned long)V.size(); }
    void resize(unsigned long n) { V.resize(n); _M_start = n ? &V[0] : 0; }
};

static bool g_allocOk = true;
class C3DMeshStats { public:
    CMovableResourceMemoryPool* MemoryPool;
    CCharString Name;
    C3DBoundingSphere BoundingSphere;
    C3DBoundingBox BoundingBox;
    CPreAllocVectorMap<C3DHelperPoint> HelperPoints;
    CPreAllocVectorMap<C3DDummyObject> DummyObjects;
    CMeshVolumeMap MeshVolumes;
    CGeneratorArray MeshGenerators;
    char* PackedNames;
    unsigned short PackedNamesSize;
    bool AnimatedFlag;
    std::vector<C3DHelperPoint> HelperStore;
    std::vector<C3DDummyObject> DummyStore;
    std::vector<char> NameStore;
    bool AllocateBuffers(unsigned long helpers, unsigned long dummies, unsigned long namesSize)
    {
        if (!g_allocOk) return false;
        HelperStore.resize(helpers + 1); DummyStore.resize(dummies + 1); NameStore.resize(namesSize + 1);
        HelperPoints.Data = &HelperStore[0]; HelperPoints.Count = helpers; HelperPoints.Sorted = true;
        DummyObjects.Data = &DummyStore[0]; DummyObjects.Count = dummies; DummyObjects.Sorted = true;
        PackedNames = &NameStore[0];
        return true;
    }
    bool LoadBinary(CMemoryDataInputStream* in);
};

bool C3DMeshStats::LoadBinary(CMemoryDataInputStream* in)
{
    float u0 = 0.0f;
    float u1 = 0.0f;
    float u2 = 0.0f;
    float u3 = 0.0f;
    float u4 = 0.0f;
    float u5 = 0.0f;
    float u6 = 0.0f;
    float u7 = 0.0f;
    Name = in->ReadNullTerminatedString();
    AnimatedFlag = in->ReadBool();
    BoundingSphere.Centre = in->Read3DVector();
    BoundingSphere.Radius = in->ReadFloat();
    C3DVector boxMin = in->Read3DVector();
    C3DVector boxMax = in->Read3DVector();
    BoundingBox.Set(boxMin, boxMax);
    unsigned long numHelpers = in->ReadUShort();
    unsigned long numDummies = in->ReadUShort();
    PackedNamesSize = in->ReadUShort();
    unsigned long numVolumes = in->ReadUShort();
    unsigned long numGenerators = in->ReadUShort();
    if (!AllocateBuffers(numHelpers, numDummies, PackedNamesSize))
        return false;
    in->Read(HelperPoints.Data, HelperPoints.Count * sizeof(C3DHelperPoint));
    HelperPoints.Sorted = false;
    in->Read(DummyObjects.Data, DummyObjects.Count * sizeof(C3DDummyObject));
    HelperPoints.Sorted = false;
    in->Read(PackedNames, PackedNamesSize);
    MeshVolumes.resize(numVolumes);
    for (unsigned long i = 0; i < MeshVolumes.size(); ++i) {
        MeshVolumes[i].Key = in->ReadULong();
        if (!MeshVolumes[i].Value.LoadBinary(in, MemoryPool))
            return false;
    }
    MeshVolumes.Sorted = false;
    MeshGenerators.resize(numGenerators);
    for (unsigned long j = 0; j < MeshGenerators.size(); ++j)
        MeshGenerators._M_start[j].LoadBinary(in);
    return true;
}

// little-endian writers
static void put(std::vector<unsigned char>& b, const void* p, size_t n) { const unsigned char* c = (const unsigned char*)p; for (size_t i = 0; i < n; ++i) b.push_back(c[i]); }
static void u16(std::vector<unsigned char>& b, unsigned v) { b.push_back((unsigned char)(v & 0xff)); b.push_back((unsigned char)(v >> 8)); }
static void u32(std::vector<unsigned char>& b, unsigned long v) { for (int i = 0; i < 4; ++i) b.push_back((unsigned char)((v >> (8 * i)) & 0xff)); }
static void f32(std::vector<unsigned char>& b, float f) { put(b, &f, 4); }

static std::vector<unsigned char> build()
{
    std::vector<unsigned char> b;
    put(b, "mesh_a", 7);                    // name + NUL
    b.push_back(1);                         // animated
    f32(b, 1.0f); f32(b, 2.0f); f32(b, 3.0f); f32(b, 4.5f);          // sphere
    f32(b, -1.0f); f32(b, -2.0f); f32(b, -3.0f);                       // box min
    f32(b, 5.0f); f32(b, 6.0f); f32(b, 7.0f);                          // box max
    u16(b, 2); u16(b, 1); u16(b, 5); u16(b, 3); u16(b, 2);             // helpers, dummies, names, volumes, generators
    for (int i = 0; i < 2 * 0x14; ++i) b.push_back((unsigned char)(0x10 + i));
    for (int i = 0; i < 0x38; ++i) b.push_back((unsigned char)(0x80 + i));
    put(b, "ab\0cd", 5);
    for (unsigned k = 0; k < 3; ++k) { u32(b, 0x1000 + k); u16(b, 0x700 + k); }
    b.push_back(0x21); b.push_back(0x22);
    return b;
}
static void openStream(CMemoryDataInputStream& in, const std::vector<unsigned char>& b, long chunk)
{
    in.m_position = 0; in.StreamSize = (long)b.size(); in.m_pData = &b[0]; in.SourceChunkStreamPos = 0;
    in.m_remaining = chunk < (long)b.size() ? chunk : (long)b.size();
    in.Overflow.assign(b.begin() + in.m_remaining, b.end()); in.OverflowPos = 0;
}

int main()
{
    CMovableResourceMemoryPool pool = { 9 };
    std::vector<unsigned char> data = build();

    // whole stream in one chunk: every field lands, both loops run, Sorted flags cleared
    {
        CMemoryDataInputStream in; openStream(in, data, 100000); g_overflowCalls = 0; g_volumeLoads = 0; g_volumeFailAt = -1; g_allocOk = true;
        C3DMeshStats m; m.MemoryPool = &pool; m.MeshVolumes.Sorted = true; m.AnimatedFlag = false;
        CHECK(m.LoadBinary(&in));
        CHECK(m.Name.S == "mesh_a" && m.AnimatedFlag);
        CHECK(m.BoundingSphere.Centre.X == 1.0f && m.BoundingSphere.Centre.Z == 3.0f && m.BoundingSphere.Radius == 4.5f);
        CHECK(m.BoundingBox.Min.Y == -2.0f && m.BoundingBox.Max.Z == 7.0f);
        CHECK(m.HelperPoints.Count == 2 && m.HelperPoints.Data[1]._data[0] == 0x10 + 0x14 && !m.HelperPoints.Sorted);
        CHECK(m.DummyObjects.Count == 1 && m.DummyObjects.Data[0]._data[0x37] == 0x80 + 0x37);
        CHECK(m.DummyObjects.Sorted);          // the retail body clears HelperPoints.Sorted twice, never DummyObjects
        CHECK(m.PackedNamesSize == 5 && m.PackedNames[3] == 'c');
        CHECK(m.MeshVolumes.size() == 3 && !m.MeshVolumes.Sorted);
        for (unsigned k = 0; k < 3; ++k) {
            CHECK(m.MeshVolumes[k].Key == 0x1000 + k);
            CHECK(m.MeshVolumes[k].Value.Tag == 0x700 + k && m.MeshVolumes[k].Value.Pool == &pool);
        }
        CHECK(m.MeshGenerators.size() == 2 && m.MeshGenerators.V[0].Kind == 0x21 && m.MeshGenerators.V[1].Kind == 0x22);
        CHECK(in.m_position == data.size() && g_overflowCalls == 0);
    }

    // a small chunk forces the overflow path; the result is the same
    {
        CMemoryDataInputStream in; openStream(in, data, 20); g_overflowCalls = 0; g_volumeLoads = 0; g_volumeFailAt = -1; g_allocOk = true;
        C3DMeshStats m; m.MemoryPool = &pool; m.MeshVolumes.Sorted = true;
        CHECK(m.LoadBinary(&in));
        CHECK(g_overflowCalls > 0);
        CHECK(m.BoundingBox.Max.X == 5.0f && m.MeshVolumes[2].Key == 0x1002 && m.MeshGenerators.V[1].Kind == 0x22);
    }

    // a mesh volume failing stops the load: Sorted stays set, generators untouched
    {
        CMemoryDataInputStream in; openStream(in, data, 100000); g_volumeLoads = 0; g_volumeFailAt = 1; g_allocOk = true;
        C3DMeshStats m; m.MemoryPool = &pool; m.MeshVolumes.Sorted = true;
        CHECK(!m.LoadBinary(&in));
        CHECK(m.MeshVolumes.Sorted && m.MeshGenerators.size() == 0 && g_volumeLoads == 2);
    }

    // AllocateBuffers failing returns false straight after the header
    {
        CMemoryDataInputStream in; openStream(in, data, 100000); g_volumeLoads = 0; g_volumeFailAt = -1; g_allocOk = false;
        C3DMeshStats m; m.MemoryPool = &pool; m.MeshVolumes.Sorted = true; m.PackedNamesSize = 0;
        CHECK(!m.LoadBinary(&in));
        CHECK(m.PackedNamesSize == 5 && m.MeshVolumes.size() == 0 && g_volumeLoads == 0);
    }

    if (!g_fail) printf("00a2f2d0_TEST PASS\n");
    return g_fail;
}