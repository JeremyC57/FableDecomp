// Behaviour test for C3DMeshFileTrianglesChunk::GetChunkInfo @ 0x00ABBE90: sprintf "Number of triangles: %u"
// (count = number of 6-byte index triples in Triangles at +0x2c) into a 128-byte buffer and return
// CCharString(text, -1). Checks several triangles, none, and a large count (the divide by 6). The chunk is laid over
// zeroed storage (no default constructor) and the method is called non-virtually.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CCharString { public:
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

class C3DVector  { public: float X, Y, Z; };                   // trivial default constructor (retail leaves it uninitialised)
class CMatrix3x4 { public:                                     // 48 bytes: three axis vectors and the position, copied with rep movsd
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                  // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

template<class T> class CArray { public:                       // retail CArray<T>: begin/end/capacity, no allocator state
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) {}
    unsigned long Size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long index) const { return m_begin[index]; }
};

class CChunkedFileSaver;
class CChunkedFileLoader;
class CDataOutputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
    CMatrix3x4 ReadMatrix3x4();                         // 0x00995340 (by value through the hidden pointer)
};

// CChunkedFileChunk (0x10: vptr, ChunkType[5], Parent) is the base of the X* chunks.
class CChunkedFileChunk { public:
    virtual void v0();
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
};

// CChunkedFileChunk (0x10) -> CChunkedFileSuperChunk -> C3DMeshFileChunk (0x2c in retail).
struct C3DMeshFileChunk {
    virtual ~C3DMeshFileChunk();                                 // slot 0 (scalar deleting dtor)
    virtual void v1();
    virtual CCharString GetChunkInfo() const;                    // slot 2
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // slot 7, 0x00AA8830 (CChunkedFileSuperChunk)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0 (CChunkedFileSuperChunk)
    C3DMeshFileChunk(const char* chunkType);                     // 0x00AA8360
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// The chunk factory that owns CreateNewInstance(): a `new T` registered through virtual slot 10.
// (rtti-vtables.tsv lists CreateNewInstance at slot 9 of the chunk's own vtable: the prototype chunk
// creates its sibling and registers it through slot 10; both models give the same bytes.)
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* chunk);   // slot 10 (+0x28)
};
// ------------------------------------------------------------------------------------------------

extern "C" int sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

struct C3DMeshFileTriangle { unsigned short Index[3]; };          // 6 bytes
class C3DMeshFileTrianglesChunk : public C3DMeshFileChunk { public:
    CArray<C3DMeshFileTriangle> Triangles;   // +0x2c (PDB +0x34)
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileTrianglesChunk::GetChunkInfo() const
{
    char text[128];
    sprintf(text, "Number of triangles: %u", Triangles.Size());
    return CCharString(text, -1);
}

// ---- instrumented stubs ----
static int g_textCalls = 0;
static long g_lenArg = 0;
static char* dupText(const char* text, size_t n) { char* p = (char*)std::malloc(n + 1); std::memcpy(p, text, n); p[n] = 0; return p; }
CCharString::CCharString() : m_pStringData(0) { }
CCharString::CCharString(const char* text, long length)
{ ++g_textCalls; g_lenArg = length; m_pStringData = dupText(text, length < 0 ? std::strlen(text) : (size_t)length); }
CCharString::CCharString(const CCharString& other)
{ m_pStringData = other.m_pStringData ? dupText((const char*)other.m_pStringData, std::strlen((const char*)other.m_pStringData)) : 0; }
CCharString::~CCharString() { std::free(m_pStringData); }
static bool is(const CCharString& s, const char* text) { return s.m_pStringData && std::strcmp((const char*)s.m_pStringData, text) == 0; }

int main()
{
    static C3DMeshFileTriangle tris[200000];
    static union { double align; unsigned char raw[sizeof(C3DMeshFileTrianglesChunk)]; } storage;
    std::memset(storage.raw, 0, sizeof(storage.raw));
    C3DMeshFileTrianglesChunk& chunk = *reinterpret_cast<C3DMeshFileTrianglesChunk*>(storage.raw);
    CHECK((unsigned char*)&chunk.Triangles - storage.raw == 0x2c);
    chunk.Triangles.m_begin = tris; chunk.Triangles.m_end = tris + 7; chunk.Triangles.m_capacity = tris + 7;

    g_textCalls = 0;
    { CCharString r = chunk.C3DMeshFileTrianglesChunk::GetChunkInfo(); CHECK(g_textCalls == 1 && g_lenArg == -1 && is(r, "Number of triangles: 7")); }

    g_textCalls = 0; chunk.Triangles.m_end = tris;
    { CCharString r = chunk.C3DMeshFileTrianglesChunk::GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Number of triangles: 0")); }

    g_textCalls = 0; chunk.Triangles.m_end = tris + 123457;
    { CCharString r = chunk.C3DMeshFileTrianglesChunk::GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Number of triangles: 123457")); }

    if (g_fail) { std::printf("00abbe90_TEST FAIL\n"); return 1; }
    std::printf("00abbe90_TEST PASS\n");
    return 0;
}