// CSmallVector<NAnimationEvents::CEvent,32>::Allocate(unsigned long n, const CEvent& value) -- clear, new[] n events, fill each with value
// Leaf: ?Allocate@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FAFE0, 87 bytes, `ret 8`.
// Label correction: catalog "AnimEventArray_Reallocate" -> CSmallVector<NAnimationEvents::CEvent,32>::Allocate.
// Callees: operator delete[] 0x00BFEB1C, operator new[] 0x00BFEB22.
// Shape: Clear() (delete[], zero both members), new[] n events, then a fill loop over an END-pointer local (`end =
//   Start + n` computed once; a `Start + Size` bound re-reads it).
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
    CEvent() { }
};
}
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    int Size;                                             // +0x04
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    void Allocate(unsigned long n, const T& value);
};
template <class T, int N> void CSmallVector<T, N>::Allocate(unsigned long n, const T& value)
{
    Clear();
    Start = new T[n];
    Size = n;
    T* end = Start + n;
    for (T* p = Start; p != end; ++p)
        *p = value;
}
template class CSmallVector<NAnimationEvents::CEvent, 32>;