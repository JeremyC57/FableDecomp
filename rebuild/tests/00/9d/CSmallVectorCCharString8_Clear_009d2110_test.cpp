// behaviour test for 009d2110: CSmallVector<CCharString,8>::Clear on a host copy of the shared model
#include <cstdio>
struct SStr { const char* p; SStr() : p(0) { } SStr& operator=(const SStr& o) { p = o.p; return *this; } };
static SStr Mk(int i) { static const char* t[8] = { "a", "b", "c", "d", "e", "f", "g", "h" }; SStr s; s.p = t[i]; return s; }
static int Eq(const SStr& s, int i) { return s.p == Mk(i).p; }
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
template <int N> struct CSmallVectorSize;
template <> struct CSmallVectorSize<8> { typedef signed char Type; };     // CSmallVector<T,8>: schar Size
template <> struct CSmallVectorSize<32> { typedef int Type; };            // CSmallVector<T,32>: int Size
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    typename CSmallVectorSize<N>::Type Size;              // +0x04
    long GetSize() const { return Size; }
    void resize(long n, const T& fill);
    void push_back(const T& value);
    T* erase(T* where);
    void Clear();
    void Allocate(unsigned long n);
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::Clear()
{
    delete[] Start;
    Start = 0;
    Size = 0;
}
template <class T, int N> void CSmallVector<T, N>::Allocate(unsigned long n)
{
    Clear();
    Start = new T[n];
    Size = n;
}
template <class T, int N> void CSmallVector<T, N>::Copy(const CSmallVector& other)
{
    Allocate(other.Size);
    for (unsigned long i = 0; i < other.Size; ++i)
        Start[i] = other.Start[i];
}
template <class T, int N> void CSmallVector<T, N>::resize(long n, const T& fill)
{
    if (n > 0)
    {
        T* next = new T[n];
        for (int i = 0; i < Min(n, GetSize()); ++i)
            next[i] = Start[i];
        int extra = n - Size;
        if (extra <= 0x7fffffff)
        {
            if (extra > 0)
                for (int j = 0; j < n - Size; ++j)
                    next[Size + j] = fill;
            Clear();
            Start = next;
            Size = n;
        }
    }
    else
        Clear();
}
template <class T, int N> void CSmallVector<T, N>::push_back(const T& value)
{
    resize(Size + 1, T());
    Start[Size - 1] = value;
}
template <class T, int N> T* CSmallVector<T, N>::erase(T* where)
{
    T* next = new T[Size - 1];
    int index = where - Start;
    int i;
    for (i = 0; i < index; ++i)
        next[i] = Start[i];
    for (i = index + 1; i < Size; ++i)
        next[i - 1] = Start[i];
    int size = Size;
    delete[] Start;
    Size = size - 1;
    Start = next;
    return index > Size ? Start + Size : Start + index;
}
typedef SStr T;
static const int N = 8;
int main()
{
    int ok = 1;
    CSmallVector<T, N> v; v.Start = 0; v.Size = 0;
    v.Allocate(3);
    v.Clear();
    if (v.Size != 0 || v.Start != 0) ok = 0;
    v.Clear();                                            // clearing an empty vector is harmless
    if (v.Size != 0 || v.Start != 0) ok = 0;
    if (!ok) { std::printf("009d2110_TEST FAIL\n"); return 1; }
    std::printf("009d2110_TEST PASS\n");
    return 0;
}