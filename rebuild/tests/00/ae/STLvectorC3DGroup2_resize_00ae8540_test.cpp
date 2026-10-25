// behaviour test for 0x00AE8540 std::vector<C3DGroup2>::resize(unsigned int n): erase the tail when shrinking, else append default groups (bone -1, no blends)
#include <cstdio>
#include <cstdlib>
#include <new>
static int gLive = 0;                                    // live heap blocks
static void* Alloc(unsigned int n) { if (n == 0) return 0; ++gLive; return std::malloc(n); }
static void Free(void* p) { if (p) { --gLive; std::free(p); } }
struct SGroup {                                          // C3DGroup2 model: bone index + an owned blend array
    long BoneIndex; long* Blends; unsigned int NoBlends;
    SGroup() : BoneIndex(-1), Blends(0), NoBlends(0) { }
    SGroup(long bone, unsigned int n) : BoneIndex(bone), Blends((long*)Alloc(n * sizeof(long))), NoBlends(n)
    { for (unsigned int i = 0; i < n; ++i) Blends[i] = bone * 10 + (long)i; }
    SGroup(const SGroup& o) : BoneIndex(o.BoneIndex), Blends((long*)Alloc(o.NoBlends * sizeof(long))), NoBlends(o.NoBlends)
    { for (unsigned int i = 0; i < NoBlends; ++i) Blends[i] = o.Blends[i]; }
    SGroup& operator=(const SGroup& o)
    {
        if (this != &o) { Free(Blends); BoneIndex = o.BoneIndex; NoBlends = o.NoBlends; Blends = (long*)Alloc(NoBlends * sizeof(long));
            for (unsigned int i = 0; i < NoBlends; ++i) Blends[i] = o.Blends[i]; }
        return *this;
    }
    ~SGroup() { Free(Blends); }
    bool Check(long bone, unsigned int n) const
    {
        if (BoneIndex != bone || NoBlends != n) return false;
        for (unsigned int i = 0; i < n; ++i) if (Blends[i] != bone * 10 + (long)i) return false;
        return true;
    }
};
template <class T> struct TVec {                         // STLport vector model
    T* Start; T* Finish; T* EndOfStorage;
    TVec() : Start(0), Finish(0), EndOfStorage(0) { }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    unsigned int capacity() const { return (unsigned int)(EndOfStorage - Start); }
    static T* UninitCopy(T* first, T* last, T* result) { for (; first != last; ++first, ++result) new (result) T(*first); return result; }
    static T* UninitFillN(T* first, unsigned int n, const T& x) { for (; n > 0; --n, ++first) new (first) T(x); return first; }
    void Clear() { for (T* p = Start; p != Finish; ++p) p->~T(); Free(Start); }
    void Set(T* s, T* f, T* e) { Start = s; Finish = f; EndOfStorage = e; }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            unsigned int old = size();
            T* tmp;
            if (Start) { tmp = (T*)Alloc(n * sizeof(T)); UninitCopy(Start, Finish, tmp); Clear(); }
            else tmp = (T*)Alloc(n * sizeof(T));
            Set(tmp, tmp + old, tmp + n);
        }
    }
    void InsertOverflow(T* pos, const T& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old > fill ? old : fill);
        T* ns = (T*)Alloc(len * sizeof(T));
        T* nf = UninitCopy(Start, pos, ns);
        if (fill == 1) { new (nf) T(x); ++nf; } else nf = UninitFillN(nf, fill, x);
        if (!atend) nf = UninitCopy(pos, Finish, nf);
        Clear();
        Set(ns, nf, ns + len);
    }
    void push_back(const T& x) { if (Finish != EndOfStorage) { new (Finish) T(x); ++Finish; } else InsertOverflow(Finish, x, 1, true); }
    void erase(T* first, T* last)
    {
        T* d = first;
        for (T* s = last; s != Finish; ++s, ++d) *d = *s;
        for (T* p = d; p != Finish; ++p) p->~T();
        Finish = d;
    }
    void insert(T* pos, unsigned int n, const T& x)
    {
        if (n == 0) return;
        if (pos == Finish && (unsigned int)(EndOfStorage - Finish) >= n) { Finish = UninitFillN(Finish, n, x); return; }
        InsertOverflow(pos, x, n, pos == Finish);
    }
    void resize(unsigned int n, const T& x) { if (n < size()) erase(Start + n, Finish); else insert(Finish, n - size(), x); }
    void resize(unsigned int n) { resize(n, T()); }
};
int main()
{

    {
        TVec<SGroup> v;
        v.push_back(SGroup(4, 2)); v.push_back(SGroup(6, 1));
        v.resize(5);
        if (v.size() != 5 || !v.Start[0].Check(4, 2) || !v.Start[1].Check(6, 1)) { std::printf("00ae8540_TEST FAIL\n"); return 1; }
        for (int i = 2; i < 5; ++i) if (v.Start[i].BoneIndex != -1 || v.Start[i].NoBlends != 0) { std::printf("00ae8540_TEST FAIL\n"); return 1; }
        v.resize(1);
        if (v.size() != 1 || !v.Start[0].Check(4, 2)) { std::printf("00ae8540_TEST FAIL\n"); return 1; }
        v.resize(1);
        if (v.size() != 1) { std::printf("00ae8540_TEST FAIL\n"); return 1; }
        v.Clear();
    }
    if (gLive != 0) { std::printf("00ae8540_TEST FAIL\n"); return 1; }
    std::printf("00ae8540_TEST PASS\n");
    return 0;
}