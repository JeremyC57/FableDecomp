// std::vector<CEnvironmentThemeDef>::_M_insert_overflow(CEnvironmentThemeDef* position, const CEnvironmentThemeDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate size + max(size, n) slots, copy-construct the prefix, the n copies of x and (unless at the end) the suffix, then release the old block
// Leaf: ?_M_insert_overflow@?$vector@UCEnvironmentThemeDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477169, 244 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd).
// Identity: stride 0xC4 with the element copy constructor 0x0047318D (landed); the old block is released through
//   vector::_M_clear 0x00473023 (landed as CEnvironmentThemeDefVector::clear (chunk 241): destroy [start, finish) then free). Single caller 0x0047715A
//   (push_back / insert). The catalog label "CEnvironmentThemeDef_vector_Reallocate" is behavioural; the shape is the landed
//   vector<CDialogueLayerDef>::_M_insert_overflow 0x00472D79 with a 0xC4-byte element (signed `idiv` count).
// Callees: malloc 0x00BFEA0E, CEnvironmentThemeDef(const CEnvironmentThemeDef&) 0x0047318D, vector::_M_clear 0x00473023.
// Model: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` with allocate, max (const references: the old
//   size homed in [ebp-4], which then holds the new start), __uninitialized_copy / __uninitialized_fill_n and the
//   null-checked placement `_Construct` inlined; _M_clear stays out of line. CEnvironmentThemeDef (vtable 0x01236244, retail sizeof 0xC4)
//   is local: the 0x3C-byte CParentDefClassBase then TimeOfDay +0x3C .. WaterReflectionOffset +0xC0 (Ego_r.pdb, 4 bytes smaller in retail).
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`; `and dword ptr [ebp-4],0` for the null block).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CParentDefClassBase {
    unsigned char _body[0x3c - 4];                  // +0x04
    virtual ~CParentDefClassBase();
};

struct CEnvironmentThemeDef : CParentDefClassBase {                 // vtable 0x01236244, retail sizeof 0xC4
    unsigned long Fields[(0xc4 - 0x3c) / 4];        // +0x3c
    CEnvironmentThemeDef(const CEnvironmentThemeDef& other);                          // 0x0047318D
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

template <class T> class vector {
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void _M_clear();                                // 0x00473023
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

template void vector<CEnvironmentThemeDef>::_M_insert_overflow(CEnvironmentThemeDef* position,
    const CEnvironmentThemeDef& x, const __false_type&, unsigned int fillLength, bool atEnd);
}