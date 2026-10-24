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
struct CCPPointerInfo {                                 // the shared count block
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

struct CNavConnectorNode { int id; };
typedef CCountedPointer<CNavConnectorNode> TNodePtr;
struct SConnPair {                                      // pair<long, CCountedPointer<CNavConnectorNode> >
    long     first;
    TNodePtr second;
    SConnPair(long k, const TNodePtr& p) : first(k), second(p) { }
};
class CConnectorMap : public TVec<SConnPair> {          // CVectorMap<long, CCountedPointer<CNavConnectorNode>, ...>
public:
    bool Compare;
    bool Dirty;
    void clear()                                        // erase(begin(), end())
    {
        SConnPair* i = start;
        for (SConnPair* s = finish; s != finish; ++s, ++i)
            *i = *s;
        Destroy(i, finish);
        finish = i;
    }
    // the row: drop every entry, then swap with an empty vector so the storage is released
    void DeallocateMemory()
    {
        clear();
        TVec<SConnPair> empty;
        swap(empty);
        Dirty = false;
    }
};

int main()
{
    {
        CNavConnectorNode* shared = new CNavConnectorNode;
        TNodePtr keep(shared);
        CConnectorMap m;
        m.Dirty = true;
        m.push_back(SConnPair(3, TNodePtr(new CNavConnectorNode)));
        m.push_back(SConnPair(1, keep));
        m.push_back(SConnPair(2, TNodePtr(new CNavConnectorNode)));
        Check(m.size() == 3 && keep.Count() == 2 && g_blocks == 1);
        m.DeallocateMemory();
        Check(m.start == 0 && m.finish == 0 && m.eos == 0);
        Check(!m.Dirty);
        Check(g_blocks == 0);
        Check(g_deleted == 2);
        Check(keep.Count() == 1 && keep.Data == shared);
        m.DeallocateMemory();
        Check(m.start == 0 && g_blocks == 0);
    }
    Check(g_deleted == 3);

    if (g_fail) {
        std::printf("00a83fd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a83fd0_TEST PASS\n");
    return 0;
}