// _global::__insertion_sort(PairCharStringVector* first, PairCharStringVector* last, CKeyPairCompareLess comp) -- SGI insertion sort over (name, C3DVector) pairs ordered by name: linear-insert each element after the first into the sorted prefix
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077ECB0, length 96 bytes, `ret 4` (the empty comparator on the stack;
//   first/last in ecx/edx -- the SGI algorithm TUs are /Gr). Callers: the final insertion pass of the sort at 0x0077F050 /
//   0x0077F06E / 0x0077F48D / 0x0077F4A7.
// Identity: the element is 16 bytes = { CCharString +0 ; three words +4..+0xC }; this TU's heap/sort helpers carry the manifest
//   names `_Push_heap/_Adjust_heap/_Sort_heap<pair<CCharString,C3DVector>*,...,CKeyPairCompareLess<CCharString,C3DVector>>`,
//   so the element is pair<CCharString, C3DVector>. The row's `_Move_backward<pair<unsigned long,CDefClassInfo>*,...>` label
//   is propagated (wrong function and wrong element).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, __linear_insert @ 0x0077E960 (fastcall first/last,
//   then the 16-byte value and the comparator on the stack, `ret 14h`).
// Shape: STLport
//     if (__first == __last) return;
//     for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i) __linear_insert(__first, __i, _Tp(*__i), __comp);
//   the by-value element is built in the argument area by STLport's explicit pair copy constructor (CCharString by call,
//   C3DVector by its memberwise copy constructor through ebx = i + 0xC); `first` is spilled to the `push ecx` slot.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class C3DVector {                                       // 12 bytes
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    float Z;                                            // +0x08
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct PairCharStringVector {                           // std::pair<CCharString, C3DVector>, 16 bytes
    CCharString first;                                  // +0x00
    C3DVector   second;                                 // +0x04
    PairCharStringVector(const PairCharStringVector& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<CCharString, C3DVector>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __linear_insert(PairCharStringVector* first, PairCharStringVector* last, PairCharStringVector val,
                                CKeyPairCompareLess comp);   // 0x0077E960

void __fastcall __insertion_sort(PairCharStringVector* first, PairCharStringVector* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (PairCharStringVector* i = first + 1; i != last; ++i)
        __linear_insert(first, i, PairCharStringVector(*i), comp);
}