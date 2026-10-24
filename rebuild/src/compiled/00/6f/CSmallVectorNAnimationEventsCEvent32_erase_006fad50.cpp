// CSmallVector<NAnimationEvents::CEvent,32>::erase(NAnimationEvents::CEvent* where) -- remove one event: copy every other event into a fresh Size-1 array, free the old one, return the slot after the gap
// Leaf: ?erase@
// Retail: Fable.exe @ 0x006FAD50, 144 bytes, `ret 4`. FableWin.pdb ?erase@?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@QAEPAVCEvent@NAnimationEvents@@PAV23@@Z.
// Layout: Ego_r.pdb CSmallVector<T,32> = Start (T*, +0) and Size (+4, int); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `new T[Size-1]` over CEvent's empty inline ctor is a bare operator new[]; Size is saved in a local before the delete[]
//   (retail's `mov ebp,[esi+4]` ahead of the call, `lea eax,[ebp-1]` after it) and the result clamps the index to the new Size.
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
template class CSmallVector<NAnimationEvents::CEvent, 32>;