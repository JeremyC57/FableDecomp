// _STL::__unguarded_linear_insert(pair<unsigned long, C3DDummyObject>* last, pair<unsigned long, C3DDummyObject> val, CKeyPairCompareLess comp) -- insertion-sort inner step without a lower bound check for the dummy-object table: shift every larger entry up one slot and drop the value into the gap
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A31320, 55 bytes, `ret 3Ch` (__fastcall: last in ecx, the 0x38-byte value
//   and the empty comparator on the stack).
// Identity: called from the landed _STL::__linear_insert 0x00A33800 (which names it); the catalog's
//   "_Copy_impl<CDegradableInfo*,CDegradableInfo*>" is propagated. Element: pair<unsigned long, C3DDummyObject> (0x38
//   bytes; Ego_r.pdb C3DDummyObject = CMatrix3x4 Transform + long BoneIndex), ordered by the unsigned key (`jb`).
// Shape: SGI `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` -- the
//   key of val stays in edx, every assignment is the implicit 14-dword `rep movsd` (last kept in edi, the source in esi),
//   the final one copies from val's argument slot (`lea esi,[esp+0Ch]`).
namespace _STL {

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

void __fastcall __unguarded_linear_insert(TEntry* last, TEntry val, CKeyPairCompareLess comp)
{
    TEntry* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

}