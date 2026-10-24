// std::vector<CThingMapCell>::operator=(const vector& x) -- STLport vector assignment of 4-byte thing-map cells: reallocate when x outgrows the capacity, else assign over the live cells and copy-construct the tail; returns *this
// Leaf: ??4?$vector@VCThingMapCell@NThingMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085DFD0, length 222 bytes (+2 int3), thiscall `ret 4`.
// Full name: std::vector<NThingMap::CThingMapCell>::operator=(const std::vector<NThingMap::CThingMapCell>&). Evidence: its
//   callers are the landed fill / copy / copy_backward helpers over 12-byte elements (0x0085E410, 0x0085E470, 0x0085E4C0),
//   used by the vector<vector<CThingMapCell> > insert/resize bodies (0x0085E6C0, 0x0085EBB0) next to the
//   NThingMap::CThingMap scalar deleting destructor (0x0085ECF0); Ego_r.pdb CThingMap::Cells is
//   vector<vector<NThingMap::CThingMapCell> > and CThingMapCell is 4 bytes (CMapwhoNode* PFirstNode).
// Label correction: catalog "Vector_Reallocate" is behavioural.
// Callees: _M_allocate_and_copy(n, first, last) @ 0x0085DCA0 (thiscall), the out-of-line `__fastcall`
//   __uninitialized_copy(first, last, result, const __false_type&) @ 0x0085D820, free @ 0x00BFEA14 (the inline _M_clear).
// Model: STLport 4.x `vector::operator=` verbatim (as the landed 0x0045BAAA CRGBColour row) in the speed TU (no pragma):
//   `__xlen > capacity()` reallocates, `size() >= __xlen` assigns with the counted random-access `__copy` (no destroy: the
//   cell's destructor is trivial), else copies size() cells and constructs the rest through `__uninitialized_copy`.
// Shape: the type-traits tag is a named `__false_type` local (homed in the dead argument slot, no store); the value-
//   initialised temporary `__false_type()` adds a `mov byte ptr [esp+1Ch],0` before the call.
extern "C" void __cdecl free(void* block);

namespace NThingMap {
class CMapwhoNode;
class CThingMapCell {                                   // Ego_r.pdb, 4 bytes
public:
    CMapwhoNode* PFirstNode;                            // +0x00
};
}

namespace std {
struct random_access_iterator_tag { };
struct __false_type { };

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&);   // 0x0085D820

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class T> class vector {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last);   // 0x0085DCA0
    void _M_clear()
    {
        if (_M_start)
            free(_M_start);
    }
    vector& operator=(const vector& x);
};

template <class T> vector<T>& vector<T>::operator=(const vector<T>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        }
        else if (size() >= xlen) {
            __copy((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
        }
        else {
            __copy((const_pointer)x._M_start, (const_pointer)x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __false_type isPOD;
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish, isPOD);
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}

template class vector<NThingMap::CThingMapCell>;
}