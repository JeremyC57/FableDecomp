// std::vector<CCardPositionsDef,std::allocator<CCardPositionsDef> >::_M_insert_overflow(CCardPositionsDef* position, const CCardPositionsDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path for the card-position defs: allocate size + max(size, n) slots, copy-construct the prefix, the n copies of x and (unless at the end) the suffix, then release the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9FA3, 228 bytes, `ret 14h` (thiscall: position, x, the tag, fillLength, atEnd).
// Identity: single caller 0x004F9F56, the overflow branch of vector<CCardPositionsDef>::_M_fill_insert. Stride 0x48 = CCardPositionsDef
//   (vtable 0x012439DC; its copy constructor runs CParentDefClassBase(const CParentDefClassBase&) @ 0x0046EE98 then
//   the Offset vector copy constructor @ 0x004EBE1A). Catalog label "CActiveFile_OnReadFinished" is propagated.
// Callees: malloc @ 0x00BFEA0E; STLport `_Construct<CCardPositionsDef, CCardPositionsDef>` @ 0x004EBDF1 (`__fastcall`, the placement null
//   test plus the inlined copy constructor; manifest "_Cons_val" is propagated); vector::_M_clear @ 0x004EBD8D (manifest
//   "CActiveFile::OnReadFinished" is propagated: __destroy_aux of [start, finish) then free).
// Model: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)`, the same body as the sibling 0x004EDD13
//   (CPrizeScoreDef) and the landed 0x00472D79, with `_Construct` out of line (a /Gr algorithm instance) and
//   __uninitialized_copy / __uninitialized_fill_n, allocate and max inline; size() is an `idiv` by 0x48.
// Layout: the engine header CParentDefClassBase.h carries the Ego_r donor size 0x3D (SubDefInfoMap 0x14 there);
//   retail is 0x39 (the CCardPositionsDef members start at +0x3C, sizeof 0x48), so a local base model stands in.
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`; `and dword ptr [ebp-8],0` for the null block).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CParentDefClassBase {                         // local model, retail 0x39 bytes (vtable, CDefClassBase, InstantiationName, SubDefInfoMap)
public:
    unsigned char _body[0x39];
};

class C2DVector {
public:
    float X;
    float Y;
};

class CCardPositionsDef : public CParentDefClassBase {   // Ego_r.pdb, retail sizeof 0x48
public:
    struct COffsetVector {                          // std::vector<C2DVector> (STLport: start, finish, end of storage)
        C2DVector* _M_start;
        C2DVector* _M_finish;
        C2DVector* _M_end_of_storage;
    } Offset;                                       // +0x3C
};

namespace std {
struct __false_type { __false_type() { } };

template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value);   // 0x004EBDF1

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

template <class T> class vector {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void _M_clear();                                // 0x004EBD8D
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = allocate(length);
    T* newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}

template void vector<CCardPositionsDef>::_M_insert_overflow(CCardPositionsDef* position, const CCardPositionsDef& x,
    const __false_type&, unsigned int fillLength, bool atEnd);
}