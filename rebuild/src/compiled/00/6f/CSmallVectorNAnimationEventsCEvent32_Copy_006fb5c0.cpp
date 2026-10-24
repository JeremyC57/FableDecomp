// CSmallVector<NAnimationEvents::CEvent,32>::Copy(const CSmallVector<NAnimationEvents::CEvent,32>& other) -- make this a copy of `other`: Allocate(other.Size) (inlined) then element-wise event copy
// Leaf: ?Copy@
// Retail: Fable.exe @ 0x006FB5C0, 96 bytes, `ret 4`. FableWin.pdb ?Copy@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@IAEXABV1@@Z (protected).
// Layout: Ego_r.pdb CSmallVector<T,32> = Start (T*, +0) and Size (+4, int); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: Allocate and Clear are inline here (CEvent's empty ctor needs no loop); `unsigned long i < other.Size` gives the `jb`.
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
template class CSmallVector<NAnimationEvents::CEvent, 32>;