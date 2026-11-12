// Behaviour test for N3DPrimitive::CDeviceIndependent::AddTriangleList(const C3DMeshFileTrianglesChunk&) @ 0x00A90900:
// a copy of the model (counted list, counted strip sub-chunk, growable arrays) and the same statement sequence.
#include <cstdio>
#include <cstring>
#include <cstdlib>
template <class T> class CArray {
public:
    T* Start; T* Finish; T* End;
    CArray() : Start(0), Finish(0), End(0) { }
    ~CArray() { for (T* p = Start; p != Finish; ++p) p->~T(); std::free(Start); }
    unsigned long size() const { return (unsigned long)(Finish - Start); }
    void reserve_more(unsigned long need) {
        unsigned long old = size(); if ((unsigned long)(End - Start) >= need) return;
        unsigned long cap = need > old * 2 ? need : old * 2;
        T* mem = (T*)std::malloc(cap * sizeof(T));
        for (unsigned long i = 0; i < old; ++i) { new (mem + i) T(Start[i]); Start[i].~T(); }
        std::free(Start); Start = mem; Finish = mem + old; End = mem + cap;
    }
    void resize(unsigned long n, const T& x) {
        if (n < size()) { for (T* p = Start + n; p != Finish; ++p) p->~T(); Finish = Start + n; }
        else { reserve_more(n); while (size() < n) { new (Finish) T(x); ++Finish; } }
    }
    void resize(unsigned long n) { resize(n, T()); }
    void push_back(const T& x) { reserve_more(size() + 1); new (Finish) T(x); ++Finish; }
    T& operator[](unsigned long i) { return Start[i]; }
    const T& operator[](unsigned long i) const { return Start[i]; }
private:
    void* operator new(size_t, void* p);
};
inline void* operator new(size_t, void* p) { return p; }
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
static int g_deletedLists = 0, g_deletedStrips = 0;
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { ++g_deletedLists; delete this; }
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(T* object) : Object(object) { }                 // adopts: no add-ref
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    ~CIVCountedPointer() { if (Object) { if (Object->Release() == 0) Object->Delete(); Object = 0; } }
    T* operator->() const { return static_cast<T*>(Object); }
    CIVCountedPointeeBase* Object;
};
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Info = 0; Data = 0; }
    void ShareData(T* data, CCPPointerInfo* info) { if (Info != info) { ReleaseData(); Data = data; Info = info; if (info) ++info->RefCount; } }
    template <class U> void SetStaticCast(const CCountedPointer<U>& o) { ShareData(static_cast<T*>(o.Data), o.Info); }
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};
class CChunkedFileChunk { public: const char* Type; CChunkedFileChunk(const char* t) : Type(t) { } virtual ~CChunkedFileChunk() { } };
class C3DMeshFileXTriangleStripChunk : public CChunkedFileChunk {
public:
    C3DMeshFileXTriangleStripChunk() : CChunkedFileChunk("XTST") { }
    static const char* GetStaticChunkType() { return "XTST"; }
    unsigned long GetCount() const { return StripVertices.size(); }
    unsigned long GetStripVertex(unsigned long i) const { return StripVertices[i]; }
    CArray<unsigned short> StripVertices;
};
static void DeleteStrip(void* p) { ++g_deletedStrips; delete (C3DMeshFileXTriangleStripChunk*)p; }
class C3DMeshFileTrianglesChunk {
public:
    CCountedPointer<CChunkedFileChunk> Sub;      // the one sub-chunk of this model (may be empty)
    CArray<C3DTriangle2> TriangleList;
    const C3DTriangle2* GetList() const { return TriangleList.Start; }
    unsigned long GetCount() const { return TriangleList.size(); }
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const {
        if (Sub.Data && std::strcmp(Sub.Data->Type, type) == 0) return Sub;
        return CCountedPointer<CChunkedFileChunk>();
    }
};
class CTriangleList : public CIVCountedPointeeBase {
public:
    void ResizeTriangleList(unsigned long n) { Triangles.resize(n); }
    void SetTriangle(unsigned long i, const C3DTriangle2& t) { Triangles[i] = t; }
    void ResizeStrip(unsigned long n) { StripIndices.resize(n); }
    void SetStripIndex(unsigned long i, unsigned short v) { StripIndices[i] = v; }
    CArray<C3DTriangle2> Triangles;
    CArray<unsigned short> StripIndices;
};
class CDeviceIndependent {
public:
    void AddTriangleList(const CIVCountedPointer<CTriangleList>& triangle_list) { TriangleLists.push_back(triangle_list); }
    void AddTriangleList(const C3DMeshFileTrianglesChunk& chunk)
    {
        CIVCountedPointer<CTriangleList> triangle_list(new CTriangleList);
        const C3DTriangle2* triangles = chunk.GetList();
        unsigned long count = chunk.GetCount();
        triangle_list->ResizeTriangleList(count);
        for (unsigned long i = 0; i < count; ++i)
            triangle_list->SetTriangle(i, triangles[i]);
        CCountedPointer<C3DMeshFileXTriangleStripChunk> strip;
        strip.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXTriangleStripChunk::GetStaticChunkType()));
        if (strip.Get()) {
            unsigned long strip_count = strip->GetCount();
            triangle_list->ResizeStrip(strip_count);
            for (unsigned long j = 0; j < strip_count; ++j)
                triangle_list->SetStripIndex(j, (unsigned short)strip->GetStripVertex(j));
        }
        AddTriangleList(triangle_list);
    }
    CArray<CIVCountedPointer<CTriangleList> > TriangleLists;
};
static int Fail(int code) { std::printf("00a90900_TEST FAIL (%d)\n", code); return 1; }
int main()
{
    {
        CDeviceIndependent device;
        // chunk A: four triangles and a five-vertex strip sub-chunk
        C3DMeshFileTrianglesChunk a;
        for (unsigned short t = 0; t < 4; ++t) { C3DTriangle2 tri; tri.VertexIndex[0] = t; tri.VertexIndex[1] = (unsigned short)(t + 100); tri.VertexIndex[2] = (unsigned short)(t * 7 + 3); a.TriangleList.push_back(tri); }
        C3DMeshFileXTriangleStripChunk* stripChunk = new C3DMeshFileXTriangleStripChunk;
        for (unsigned short v = 0; v < 5; ++v) stripChunk->StripVertices.push_back((unsigned short)(v * 11 + 1));
        CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteStrip; info->Data = stripChunk;
        a.Sub.Data = stripChunk; a.Sub.Info = info;
        // chunk B: two triangles, no sub-chunk
        C3DMeshFileTrianglesChunk b;
        for (unsigned short t = 0; t < 2; ++t) { C3DTriangle2 tri; tri.VertexIndex[0] = (unsigned short)(t + 50); tri.VertexIndex[1] = 9; tri.VertexIndex[2] = (unsigned short)(60 - t); b.TriangleList.push_back(tri); }
        // chunk C: empty, sub-chunk of another type
        C3DMeshFileTrianglesChunk c;
        CChunkedFileChunk other("XBSP");
        CCPPointerInfo otherInfo; otherInfo.RefCount = 5; otherInfo.DeleteFunc = 0; otherInfo.Data = &other;
        c.Sub.Data = &other; c.Sub.Info = &otherInfo;

        device.AddTriangleList(a);
        device.AddTriangleList(b);
        device.AddTriangleList(c);
        if (device.TriangleLists.size() != 3) return Fail(1);
        CTriangleList* la = device.TriangleLists[0].operator->();
        CTriangleList* lb = device.TriangleLists[1].operator->();
        CTriangleList* lc = device.TriangleLists[2].operator->();
        // each list is owned by the array alone once the local pointer is gone
        if (la->IVRefCount != 1 || lb->IVRefCount != 1 || lc->IVRefCount != 1) return Fail(2);
        if (la->Triangles.size() != 4 || lb->Triangles.size() != 2 || lc->Triangles.size() != 0) return Fail(3);
        for (unsigned long i = 0; i < 4; ++i)
            if (std::memcmp(&la->Triangles[i], &a.TriangleList[i], sizeof(C3DTriangle2)) != 0) return Fail(4);
        for (unsigned long i = 0; i < 2; ++i)
            if (std::memcmp(&lb->Triangles[i], &b.TriangleList[i], sizeof(C3DTriangle2)) != 0) return Fail(5);
        if (la->StripIndices.size() != 5) return Fail(6);
        for (unsigned long j = 0; j < 5; ++j)
            if (la->StripIndices[j] != (unsigned short)(j * 11 + 1)) return Fail(7);
        if (lb->StripIndices.size() != 0 || lc->StripIndices.size() != 0) return Fail(8);
        // the strip sub-chunk's count is back where it was, nothing deleted yet
        if (info->RefCount != 1 || otherInfo.RefCount != 5 || g_deletedStrips != 0 || g_deletedLists != 0) return Fail(9);
        c.Sub.Data = 0; c.Sub.Info = 0;      // the stack-owned count block is not released by the model
    }
    // the device's array released the three lists; chunk A released its strip
    if (g_deletedLists != 3 || g_deletedStrips != 1) return Fail(10);
    std::printf("00a90900_TEST PASS\n");
    return 0;
}