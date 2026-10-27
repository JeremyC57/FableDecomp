// std::swap(pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >&, pair<...>&) -- swap two extra-event map entries through a temporary copy
// Leaf: ??$swap@U?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@std@@@std@@YIXAAU?$pair@KV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@@0@0@Z
// Retail: Fable.exe @ 0x006FB8F0, 102 bytes, __fastcall (a in ecx, b in edx), `ret`.
// Identity (probable): STLport 4.6 `swap(a, b) { _Tp tmp = a; a = b; b = tmp; }` over the 12-byte element of
//   NAnimationEvents::CManager::AnimToExtraEventsMap (CVectorMap<unsigned long, CSmallVector<CEvent,32>>): the pair
//   copy builds the temporary (first, then CSmallVector's copy ctor = zero Start/Size + Copy), the pair assignments
//   copy `first` and run CSmallVector::operator= (`if (this != &other) Copy(other)`) through the landed
//   CSmallVector<NAnimationEvents::CEvent,32>::Copy 0x006FB5C0, and the temporary's ~CSmallVector is `delete[] Start`
//   (operator delete[] 0x00BFEB1C). Its only reference is the tail `jmp` of the iter_swap at 0x006FBD00.
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};
}
template <class T, int N> class CSmallVector {
public:
    T*  Start;                                            // +0x00
    int Size;                                             // +0x04 (int for N = 32)
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector() { delete[] Start; }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Copy(const CSmallVector& other);                 // 0x006FB5C0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap(pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >&,
                              pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> >&);
}