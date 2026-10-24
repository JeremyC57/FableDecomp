// CSmallVector<NAnimationEvents::CEvent,32>::resize(long n, const NAnimationEvents::CEvent& fill) -- grow or shrink the event vector to n entries (copy the kept prefix, fill new slots, free the old block)
// Leaf: ?resize@
// Retail: Fable.exe @ 0x006FADE0, 224 bytes, `ret 8`. FableWin.pdb ?resize@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@QAEXJABVCEvent@NAnimationEvents@@@Z.
//   Callees: operator new[]/delete[], the `vector constructor iterator` 0x004038C0 over the empty CEvent ctor 0x006F8AA0.
// Layout: Ego_r.pdb CSmallVector<T,32> = Start (T*, +0) and Size (+4, int); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: Min(n, GetSize()) -- GetSize() returns the int Size as a long by value, which is retail's spilled frame copy of Size compared
//   through a reference select; `Min(n, Size)` binds the member directly and loses the copy.
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
    CEvent() { }                                          // empty (retail keeps an out-of-line copy at 0x006F8AA0)
};
}
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
template class CSmallVector<NAnimationEvents::CEvent, 32>;