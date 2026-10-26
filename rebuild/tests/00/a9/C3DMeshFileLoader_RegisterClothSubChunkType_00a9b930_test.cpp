#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_deleted = 0;
class CCPPointerInfo { public:                                 // the shared count block
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    explicit CCountedPointer(T* data) : Data(0), Info(0) { Reset(data); }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { Release(); }
    static void __fastcall DeleteData(void* data) { ++g_deleted; delete (T*)data; }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Reset(T* data)
    {
        Release();
        if (data) {
            Info = new CCPPointerInfo;
            Info->RefCount = 1;
            Info->DeleteFunc = &CCountedPointer<T>::DeleteData;
            Info->Data = data;
            Data = data;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    long Count() const { return Info ? Info->RefCount : 0; }
    T* operator->() const { return Data; }
};

static int g_blocks = 0;
template <class T> class TVec {                         // STLport vector<T>: start / finish / end of storage
public:
    T* start;
    T* finish;
    T* eos;
    TVec() : start(0), finish(0), eos(0) { }
    ~TVec() { Destroy(start, finish); Free(start); }
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    T& operator[](unsigned int i) { return start[i]; }
    const T& operator[](unsigned int i) const { return start[i]; }
    static void Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
    static void Free(T* p) { if (p) { --g_blocks; free(p); } }
    void InsertOverflow(T* pos, const T& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        T* newStart = (T*)malloc(len * sizeof(T));
        ++g_blocks;
        T* newFinish = newStart;
        for (T* p = start; p != pos; ++p, ++newFinish)
            new (newFinish) T(*p);
        for (unsigned int n = fillLen; n > 0; --n, ++newFinish)
            new (newFinish) T(x);
        if (!atEnd)
            for (T* q = pos; q != finish; ++q, ++newFinish)
                new (newFinish) T(*q);
        Destroy(start, finish);
        Free(start);
        start = newStart;
        finish = newFinish;
        eos = newStart + len;
    }
    void push_back(const T& x)
    {
        if (finish != eos) {
            new (finish) T(x);
            ++finish;
        } else
            InsertOverflow(finish, x, 1, true);
    }
    void swap(TVec& o)
    {
        T* t = start; start = o.start; o.start = t;
        t = finish; finish = o.finish; o.finish = t;
        t = eos; eos = o.eos; o.eos = t;
    }
private:
    TVec(const TVec&);
    TVec& operator=(const TVec&);
};

static int g_seq = 0;
class CChunkedFileSuperChunk;
class CChunkedFileChunk {
public:
    int                     Kind;
    CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk(int kind) : Kind(kind), Parent(0) { }
    virtual ~CChunkedFileChunk() { }
    virtual CChunkedFileSuperChunk* GetAsSuperChunk() { return 0; }
    virtual CChunkedFileChunk* CreateNewInstance() const { return new CChunkedFileChunk(Kind); }
};
typedef CCountedPointer<CChunkedFileChunk> TChunkPtr;
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    TVec<TChunkPtr> RegisteredSubChunkTypes;
    int             Order;
    CChunkedFileSuperChunk(int kind) : CChunkedFileChunk(kind), Order(0) { }
    CChunkedFileSuperChunk* GetAsSuperChunk() { return this; }
    void RegisterSubChunkType(const TChunkPtr& chunk) { Order = ++g_seq; RegisteredSubChunkTypes.push_back(chunk); }
};
class C3DMeshFileLoader {
public:
    TChunkPtr PrimitiveClothChunkType;
    TChunkPtr SubMeshClothChunkType;
    TChunkPtr AddChunkType(const CChunkedFileChunk& chunkType, CChunkedFileSuperChunk* parent)   // 0x00A9AD00
    {
        TChunkPtr chunk;
        chunk.Reset(chunkType.CreateNewInstance());
        parent->RegisterSubChunkType(chunk);
        return chunk;
    }
    // the row: register the cloth prototype under the sub-mesh cloth chunk, then the primitive cloth chunk
    void RegisterClothSubChunkType(const CChunkedFileChunk& chunkType)
    {
        AddChunkType(chunkType, SubMeshClothChunkType->GetAsSuperChunk());
        AddChunkType(chunkType, PrimitiveClothChunkType->GetAsSuperChunk());
    }
};

int main()
{
    {
        C3DMeshFileLoader loader;
        CChunkedFileSuperChunk* prim = new CChunkedFileSuperChunk(10);
        CChunkedFileSuperChunk* sub = new CChunkedFileSuperChunk(11);
        loader.PrimitiveClothChunkType.Reset(prim);
        loader.SubMeshClothChunkType.Reset(sub);
        CChunkedFileChunk proto(5);
        loader.RegisterClothSubChunkType(proto);
        Check(sub->Order == 1 && prim->Order == 2);
        Check(sub->RegisteredSubChunkTypes.size() == 1 && prim->RegisteredSubChunkTypes.size() == 1);
        Check(sub->RegisteredSubChunkTypes[0].Count() == 1 && prim->RegisteredSubChunkTypes[0].Count() == 1);
        Check(sub->RegisteredSubChunkTypes[0].Data->Kind == 5 && prim->RegisteredSubChunkTypes[0].Data->Kind == 5);
        Check(sub->RegisteredSubChunkTypes[0].Data != prim->RegisteredSubChunkTypes[0].Data);
        Check(g_deleted == 0);
    }
    Check(g_deleted == 4 && g_blocks == 0);

    if (g_fail) {
        std::printf("00a9b930_TEST FAIL\n");
        return 1;
    }
    std::printf("00a9b930_TEST PASS\n");
    return 0;
}