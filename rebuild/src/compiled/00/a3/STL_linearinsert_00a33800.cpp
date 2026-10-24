// _STL::__linear_insert(pair<unsigned long,C3DDummyObject>* first, pair<unsigned long,C3DDummyObject>* last, pair<unsigned long,C3DDummyObject> val, CKeyPairCompareLess comp) -- insertion-sort step: a value smaller than the first element shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33800, 84 bytes, `ret 3Ch` (__fastcall: first in ecx, last in edx, the
// 0x38-byte value and the empty comparator on the stack).
// Callees: 0x00A314E0 _STL::__copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*)
// (__fastcall, 3 stack arguments; also called from the three other sort helpers at 0x00A325CC/0x00A32D4C/0x00A3319C),
// 0x00A31320 _STL::__unguarded_linear_insert(last, val, comp) (__fastcall).
// Identity: the manifest's `CPersistContext_TransferableVector<CReverbEnvironmentEntry>::Resize` is propagated (no code
// references this body; it is the out-of-line COMDAT of an insertion-sort helper). The element is 0x38 bytes =
// pair<unsigned long, C3DDummyObject> (Ego_r.pdb C3DDummyObject 0x34: CMatrix3x4 Transform, long BoneIndex; the
// copy_backward callee is catalogued for pair<.., C3DDummyObject>), ordered by the unsigned key (`cmp ; jae`).
// Shape: copy_backward is inlined down to __copy_backward with the iterator-category tag as a 1-byte temporary homed
// in the dead comparator slot (`lea eax,[esp+48h]`, user-constructed tag) and a null distance pointer; `*first = val`
// is a 14-dword `rep movsd` (implicit operator=); in the else arm the by-value argument copy of `val` is memberwise --
// the key already loaded for the compare (`mov [eax],ecx`) then 13 dwords -- so pair has STLport's user copy
// constructor `first(p.first), second(p.second)` (the implicit one block-copies 14 dwords and loads the key in eax).
namespace _STL {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                                // 0x34 bytes
    CMatrix3x4 Transform;                              // +0x00
    long       BoneIndex;                              // +0x30
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DDummyObject> TEntry;    // 0x38 bytes

struct CKeyPairCompareLess {                           // CKeyPairCompareLess<unsigned long, C3DDummyObject>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

TEntry* __fastcall __copy_backward(TEntry* first, TEntry* last, TEntry* result, const random_access_iterator_tag& tag, int* distance);   // 0x00A314E0
void __fastcall __unguarded_linear_insert(TEntry* last, TEntry val, CKeyPairCompareLess comp);                                           // 0x00A31320

inline TEntry* copy_backward(TEntry* first, TEntry* last, TEntry* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(TEntry* first, TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}