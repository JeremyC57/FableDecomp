// behaviour test for 00463ae0: CSmallVector<CThingComponentSet::CEntry,8>::Copy on a host copy of the shared model
#include <cstdio>
struct SDefString { long TablePos; SDefString() : TablePos(-1) { } };
struct SEntry { SDefString Name; unsigned long DriverType; bool Override; SEntry() : DriverType(0), Override(false) { } };
static SEntry Mk(int i) { SEntry e; e.Name.TablePos = i; e.DriverType = 10 + i; e.Override = (i & 1) != 0; return e; }
static int Eq(const SEntry& e, int i) { return e.Name.TablePos == i && e.DriverType == (unsigned long)(10 + i) && e.Override == ((i & 1) != 0); }
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
typedef SEntry T;
static const int N = 8;
int main()
{
    int ok = 1;
    CSmallVector<T, N> a; a.Start = 0; a.Size = 0;
    a.Allocate(3); a.Start[0] = Mk(2); a.Start[1] = Mk(4); a.Start[2] = Mk(6);
    CSmallVector<T, N> b; b.Start = 0; b.Size = 0;
    b.Allocate(1);
    b.Copy(a);
    if (b.Size != 3 || b.Start == a.Start || !Eq(b.Start[0], 2) || !Eq(b.Start[1], 4) || !Eq(b.Start[2], 6)) ok = 0;
    CSmallVector<T, N> z; z.Start = 0; z.Size = 0;
    b.Copy(z);
    if (b.Size != 0) ok = 0;
    if (!ok) { std::printf("00463ae0_TEST FAIL\n"); return 1; }
    std::printf("00463ae0_TEST PASS\n");
    return 0;
}