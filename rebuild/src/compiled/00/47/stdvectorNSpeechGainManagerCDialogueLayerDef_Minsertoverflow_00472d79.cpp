// std::vector<NSpeechGainManager::CDialogueLayerDef>::_M_insert_overflow(CDialogueLayerDef* position, const CDialogueLayerDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate size + max(size, n) slots, copy-construct the prefix, the n copies of x and (unless at the end) the suffix, then release the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00472D79, 223 bytes, `ret 14h` (thiscall: position, x, the tag, fillLength, atEnd).
// Identity: the catalog's "DialogueLayer_AllocCopyInsert" is behavioural. Stride 0x40, element copy constructor
//   CDialogueLayerDef::CDialogueLayerDef(const CDialogueLayerDef&) @ 0x004663E7 (landed under NSpeechGainManager), the
//   old block released through vector::_M_clear @ 0x0046638B (landed as "CActiveFile::OnReadFinished": __destroy_aux of
//   [start, finish) then free); same element as the landed vector<NSpeechGainManager::CDialogueLayerDef>::operator=
//   @ 0x00466281 and the erase @ 0x00472BDD. Single caller 0x00472C2A (insert / push_back).
// Callees: malloc @ 0x00BFEA0E, CDialogueLayerDef copy constructor @ 0x004663E7, vector::_M_clear @ 0x0046638B.
// Model: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` (exceptions off: no unwind) with allocate, max,
//   __uninitialized_copy / __uninitialized_fill_n and _Construct inlined (placement new: the null test, then the
//   out-of-line copy constructor); _M_clear stays out of line (it destroys before freeing). `max` takes const references,
//   so the old size is homed in [ebp-4] and that slot then holds the new start; the length lives in [ebp-8].
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`; `and dword ptr [ebp-4],0` for the null block).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace NSpeechGainManager {
class CDialogueLayerDef { public:                          // retail sizeof 0x40, polymorphic
    unsigned long Data[15];                         // +0x04
    CDialogueLayerDef(const CDialogueLayerDef& other);   // 0x004663E7
    virtual ~CDialogueLayerDef();
};
}

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
    void _M_clear();                                // 0x0046638B
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

template void vector<NSpeechGainManager::CDialogueLayerDef>::_M_insert_overflow(NSpeechGainManager::CDialogueLayerDef* position,
    const NSpeechGainManager::CDialogueLayerDef& x, const __false_type&, unsigned int fillLength, bool atEnd);
}