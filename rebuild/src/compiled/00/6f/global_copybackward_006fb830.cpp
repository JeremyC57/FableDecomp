// _global::__copy_backward(PairULongEventVector* first, PairULongEventVector* last, PairULongEventVector* result, const random_access_iterator_tag&, long*) -- SGI random-access copy_backward over (unsigned long, CSmallVector<NAnimationEvents::CEvent,32>) pairs: assign each element from the back, return the new start of the destination
// Leaf: __copy_backward
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FB830, length 101 bytes (cut 112 with padding), `ret 0Ch` (result, the iterator-tag reference and the
//   distance-type pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). The row's
//   `_Move_backward<pair<unsigned long,CSmallVector<CEvent,32>>*,...>` label is right on the element type; the function is
//   STLport's __copy_backward.
// Layout: the element is 12 bytes (`imul 0x2AAAAAAB ; sar edx,1`): pair = { unsigned long first +0 ; CSmallVector second +4 },
//   CSmallVector<NAnimationEvents::CEvent,32> (Ego_r.pdb) = { CEvent* Start +0 ; int Size +4 }.
// Callee: CSmallVector<NAnimationEvents::CEvent,32>::Copy @ 0x006FB5C0 (thiscall, `ret 4`: free, reallocate, copy 8-byte events).
// Shape: STLport
//     for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;
//   with the pair's implicit assignment inlined: the key word, then the small vector's inline operator= (`if (&other != this)
//   Copy(other);`, the `cmp edi,ebx ; je` self-assignment test: the source address is the left operand). The trip count lives in the dead `result` argument slot.
namespace NAnimationEvents {
class CEvent;
}

template <class T, int N> class CSmallVector {         // Ego_r.pdb, 8 bytes
public:
    T*  Start;                                          // +0x00
    int Size;                                           // +0x04
    void Copy(const CSmallVector& other);               // 0x006FB5C0
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};

struct PairULongEventVector {                           // std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >, 12 bytes
    unsigned long                                first;  // +0x00
    CSmallVector<NAnimationEvents::CEvent, 32>   second; // +0x04
};

struct random_access_iterator_tag { };

PairULongEventVector* __fastcall __copy_backward(PairULongEventVector* first, PairULongEventVector* last,
                                                  PairULongEventVector* result, const random_access_iterator_tag&, long*)
{
    for (long n = long(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}