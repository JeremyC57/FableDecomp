// std::vector<CInputEvent>::erase(CInputEvent* first, CInputEvent* last) -- STLport range erase: assign the tail [last, end) down onto first (13-dword block copies), run the (empty, out-of-line) destructor over the vacated tail, pull _M_finish back and return first
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F67E0, length 112 bytes, thiscall, `ret 8`.
// Callees: CInputEvent::~CInputEvent @ 0x00A03B30 (an empty body folded by /OPT:ICF; the landed _Destroy_Record34 0x009F6720
//   calls the same address).
// Callers: CInputManager (ClearEvents 0x009F5540 = `clear()` = erase(begin(), end()), 0x009F539E/0x009F53AE, 0x009F54E0,
//   0x009F585B/0x009F586D, 0x009F5C42/0x009F5C51) and 0x009F6AB7. The chunk label "EventContainer_Erase" is descriptive.
// Shape: STLport 4.x `erase(first, last) { pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss()); _Destroy(i, _M_finish);
//   _M_finish = i; return first; }` with __copy_ptrs/__copy inlined (the counted, strength-reduced `rep movsd` loop of the
//   out-of-line twin 0x009F63B0) and the destroy walk inlined (`mov ecx,esi ; call` per element, end cached in edi).
namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

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

template <class InputIter, class OutputIter>
__forceinline OutputIter __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter> __forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last);
};

template <class T> T* vector<T>::erase(T* first, T* last)
{
    T* i = __copy_ptrs(last, this->_M_finish, first, __false_type());
    _Destroy(i, this->_M_finish);
    this->_M_finish = i;
    return first;
}

}

class CInputEvent {                                   // 0x34 bytes
public:
    long m_words[13];
    CInputEvent(const CInputEvent& other);            // 0x009E4850
    ~CInputEvent();                                   // 0x00A03B30 (empty, ICF-folded)
};

template class std::vector<CInputEvent>;