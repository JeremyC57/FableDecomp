// std::vector<CEngineLocalDetailObjectDef>::_M_insert_overflow(CEngineLocalDetailObjectDef* position, const CEngineLocalDetailObjectDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate size + max(size, n) slots, copy-construct the prefix, the n copies of x and (unless at the end) the suffix, then release the old block
// Leaf: ?_M_insert_overflow@?$vector@VCEngineLocalDetailObjectDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047800E, 244 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd).
// Identity: stride 0x88 with the element copy constructor 0x0047756C (landed); the old block is released through
//   vector::_M_clear 0x00477BFC (renamed in sweep2 from "CActiveFile::OnReadFinished" / "StdList_Dtor": destroy [start, finish) then free). Single caller 0x00478002
//   (push_back / insert). The catalog label "CEngineLocalDetailObjectDef_vector_Reallocate" is behavioural; the shape is the landed
//   vector<CDialogueLayerDef>::_M_insert_overflow 0x00472D79 with a 0x88-byte element (signed `idiv` count).
// Callees: malloc 0x00BFEA0E, CEngineLocalDetailObjectDef(const CEngineLocalDetailObjectDef&) 0x0047756C, vector::_M_clear 0x00477BFC.
// Model: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` with allocate, max (const references: the old
//   size homed in [ebp-4], which then holds the new start), __uninitialized_copy / __uninitialized_fill_n and the
//   null-checked placement `_Construct` inlined; _M_clear stays out of line. CEngineLocalDetailObjectDef (vtable 0x0123634C, retail sizeof 0x88)
//   is a local class (PDB `V`): the 0x3C-byte CParentDefClassBase then the detail-object values and flag bits +0x3C .. +0x88
//   (Ego_r.pdb, 4 bytes smaller in retail). The vector is the real STLport 4.6 vector<_Tp, allocator<_Tp> > over
//   _Vector_base / _STLP_alloc_proxy, so _M_clear binds the family's one symbol (0x00477AB7 operator= binds the same).
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`; `and dword ptr [ebp-4],0` for the null block).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CParentDefClassBase {                       // retail 0x3C (engine/CParentDefClassBase.h is the Ego_r 0x40 layout)
    unsigned char _body[0x3c - 4];                  // +0x04
    virtual ~CParentDefClassBase();
};

class CEngineLocalDetailObjectDef : public CParentDefClassBase {          // vtable 0x0123634C, retail sizeof 0x88
public:
    unsigned long Fields[(0x88 - 0x3c) / 4];        // +0x3c
    CEngineLocalDetailObjectDef(const CEngineLocalDetailObjectDef& other);                          // 0x0047756C
};

namespace std {
struct __false_type { __false_type() { } };

template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class T, class _Alloc> class _Vector_base {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    _STLP_alloc_proxy<T*, _Alloc> _M_end_of_storage; // +0x08
};

template <class T, class _Alloc = allocator<T> > class vector : public _Vector_base<T, _Alloc> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void _M_clear();                                // 0x00477BFC
    __forceinline void _M_set(T* s, T* f, T* e) { this->_M_start = s; this->_M_finish = f; this->_M_end_of_storage._M_data = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};

template <class T, class _Alloc>
void vector<T, _Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = this->_M_end_of_storage.allocate(length);
    T* newFinish = __uninitialized_copy(this->_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, this->_M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}

template void vector<CEngineLocalDetailObjectDef>::_M_insert_overflow(CEngineLocalDetailObjectDef* position,
    const CEngineLocalDetailObjectDef& x, const __false_type&, unsigned int fillLength, bool atEnd);
}