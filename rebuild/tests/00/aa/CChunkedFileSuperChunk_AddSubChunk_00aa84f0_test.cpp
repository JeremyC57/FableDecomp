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

class CChunkedFileSuperChunk;
class CChunkedFileChunk {
public:
    CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk() : Parent(0) { }
    virtual ~CChunkedFileChunk() { }
    void SetParent(CChunkedFileSuperChunk* parent) { Parent = parent; }
};
typedef CCountedPointer<CChunkedFileChunk> TChunkPtr;
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    TVec<TChunkPtr> SubChunks;
    TVec<TChunkPtr> RegisteredSubChunkTypes;
    unsigned long   CurrentChunkIndex;
    CChunkedFileSuperChunk() : CurrentChunkIndex(0) { }
    // 0x00AA84F0: adopt the chunk and append it
    void AddSubChunk(const TChunkPtr& chunk)
    {
        chunk->SetParent(this);
        SubChunks.push_back(chunk);
    }
    // 0x00AA87E0: append a prototype chunk type
    void RegisterSubChunkType(const TChunkPtr& chunk)
    {
        RegisteredSubChunkTypes.push_back(chunk);
    }
};

int main()
{
    {
        TChunkPtr a(new CChunkedFileChunk);
        TChunkPtr b(new CChunkedFileChunk);
        TChunkPtr c(new CChunkedFileChunk);
        {
            CChunkedFileSuperChunk s;
            s.AddSubChunk(a);
            s.AddSubChunk(b);
            s.AddSubChunk(c);
            Check(s.SubChunks.size() == 3 && s.SubChunks.capacity() == 4);
            Check(s.SubChunks[0].Data == a.Data && s.SubChunks[2].Data == c.Data);
            Check(a.Count() == 2 && b.Count() == 2 && c.Count() == 2);
            Check(a->Parent == &s && c->Parent == &s);
            Check(s.RegisteredSubChunkTypes.size() == 0);
        }
        Check(a.Count() == 1 && g_deleted == 0);
    }
    Check(g_deleted == 3 && g_blocks == 0);

    if (g_fail) {
        std::printf("00aa84f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00aa84f0_TEST PASS\n");
    return 0;
}