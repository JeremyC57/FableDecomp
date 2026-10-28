// CArray<C3DVector>::DeallocateMemory() -- releases the storage of an array of 3D vectors.
// Leaf: ?DeallocateMemory@?$CArray@VC3DVector@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71F40, length 90 bytes.
// Callees: STLport __copy<C3DVector*, C3DVector*, int> 0x0074CA20 (__fastcall: first in ecx, last in edx, result + the
//          iterator category tag + the Distance* on the stack), free 0x00BFEA14.
// Identity: CArray<C3DVector> (FableWin.pdb has the class): the only caller, CArray<C3DVector>::DeallocateUnusedMemory
//   0x00A725D0, runs on CPhysicsMesh::TotalPoints (`add ecx,3Ch` in GetMeshDataFromFileData; retail layout with 0x10-byte
//   CVectorMaps). The manifest label (CArray<std::pair<unsigned_long,...>>) is propagated.
// Retail CArray<T> (the engine's VC6-era vector: {first, last, end_of_storage}, 12 bytes). DeallocateMemory()
// releases the storage: when there is any capacity it erases every element (erase(begin(), end()) is
// `last = copy(end, end, begin)` plus the destruction of what the copy left behind), detaches the block, zeroes
// the three pointers, destroys the detached range and frees the block (the null test is explicit in the source).
// The copy is dispatched as `__copy(end, end, begin, random_access_iterator_tag(), (long*)0)`: the empty tag is a
// one-byte temporary the compiler homes in the `push ecx` frame slot (`lea edx,[esp+0Fh]`) and the null Distance*
// is the `push 0`. capacity() on a 12-byte stride is the reciprocal multiply; the test folds to `add ; je`.
// Store order: the detach is written through a reference (`Exchange(slot, 0)`); a store through a `T*&` is opaque
// to VC7.1's alias analysis, so the following member load is not hoisted above it and the preceding `last =`
// store is not eliminated as dead (plain stores let the compiler reorder/elide both).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class C3DVector { public: float X, Y, Z; };

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x0074CA20 <C3DVector*, C3DVector*, int>
}

template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void erase(T* f, T* l) { last = std::__copy(l, last, f, std::random_access_iterator_tag(), (int*)0); }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<C3DVector>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(first, last);
        C3DVector* p = Exchange(first, (C3DVector*)0);
        Exchange(last, (C3DVector*)0);
        end_of_storage = 0;
        if (p)
            free(p);
    }
}