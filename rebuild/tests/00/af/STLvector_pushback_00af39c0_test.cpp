// behaviour test for 0x00AF39C0 _STL::vector<pair<CCharString, CRegisteredPSCInfo> >::push_back: copy-construct at the finish pointer, or grow through _M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int gLive = 0;
static void* Alloc(unsigned int n) { if (n == 0) return 0; ++gLive; return std::malloc(n); }
static void Free(void* p) { if (p) { --gLive; std::free(p); } }
struct SStrData { int RefCount; char Text[16]; };
struct CCharString {                                     // shared, reference-counted string model
    SStrData* Data;
    CCharString(const char* s) : Data((SStrData*)Alloc(sizeof(SStrData))) { Data->RefCount = 1; std::strcpy(Data->Text, s); }
    CCharString(const CCharString& o) : Data(o.Data) { ++Data->RefCount; }
    CCharString& operator=(const CCharString& o) { ++o.Data->RefCount; Release(); Data = o.Data; return *this; }
    ~CCharString() { Release(); }
    void Release() { if (--Data->RefCount == 0) Free(Data); }
    bool Is(const char* s) const { return std::strcmp(Data->Text, s) == 0; }
};
struct SInfo { void* AllocFunc; int ComponentType; CCharString GUIName; SInfo(int t, const char* n) : AllocFunc(0), ComponentType(t), GUIName(n) { } };
struct SPSCPair { CCharString first; SInfo second; SPSCPair(const char* k, int t, const char* n) : first(k), second(t, n) { } };
template <class T> struct TVec {
    T* Start; T* Finish; T* EndOfStorage;
    TVec() : Start(0), Finish(0), EndOfStorage(0) { }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    static T* UninitCopy(T* first, T* last, T* result) { for (; first != last; ++first, ++result) new (result) T(*first); return result; }
    static T* UninitFillN(T* first, unsigned int n, const T& x) { for (; n > 0; --n, ++first) new (first) T(x); return first; }
    void Clear() { for (T* p = Start; p != Finish; ++p) p->~T(); Free(Start); }
    void InsertOverflow(T* pos, const T& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old > fill ? old : fill);
        T* ns = (T*)Alloc(len * sizeof(T));
        T* nf = UninitCopy(Start, pos, ns);
        if (fill == 1) { new (nf) T(x); ++nf; } else nf = UninitFillN(nf, fill, x);
        if (!atend) nf = UninitCopy(pos, Finish, nf);
        Clear();
        Start = ns; Finish = nf; EndOfStorage = ns + len;
    }
    void push_back(const T& x) { if (Finish != EndOfStorage) { new (Finish) T(x); ++Finish; } else InsertOverflow(Finish, x, 1, true); }
};
int main()
{

    {
        TVec<SPSCPair> v;
        const char* keys[5] = { "Wind", "Colour", "Size", "Spin", "Alpha" };
        for (int i = 0; i < 5; ++i) v.push_back(SPSCPair(keys[i], i, keys[i]));
        if (v.size() != 5 || v.EndOfStorage - v.Start != 8) { std::printf("00af39c0_TEST FAIL\n"); return 1; }
        for (int i = 0; i < 5; ++i) if (!v.Start[i].first.Is(keys[i]) || v.Start[i].second.ComponentType != i || v.Start[i].first.Data->RefCount != 1) { std::printf("00af39c0_TEST FAIL\n"); return 1; }
        v.Clear();
    }
    if (gLive != 0) { std::printf("00af39c0_TEST FAIL\n"); return 1; }
    std::printf("00af39c0_TEST PASS\n");
    return 0;
}