// CSmallVector<NAnimationEvents::CEvent,32>::push_back(const NAnimationEvents::CEvent& value) -- append an event: resize to Size+1 with a default event, then copy the value into the last slot
// Leaf: ?push_back@
// Retail: Fable.exe @ 0x006FAEC0, 64 bytes, `ret 4`. FableWin.pdb ?push_back@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@QAEXABVCEvent@NAnimationEvents@@@Z;
//   resize is the out-of-line 0x006FADE0.
// Layout: Ego_r.pdb CSmallVector<T,32> = Start (T*, +0) and Size (+4, int); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `T()` is the uninitialised 8-byte frame temporary (`sub esp,8`) of CEvent's empty inline ctor.
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
    void resize(long n, const T& fill);                   // 0x006FADE0 (out of line)
    void push_back(const T& value);
    T* erase(T* where);
    void Clear();
    void Allocate(unsigned long n);
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::push_back(const T& value)
{
    resize(Size + 1, T());
    Start[Size - 1] = value;
}
template class CSmallVector<NAnimationEvents::CEvent, 32>;