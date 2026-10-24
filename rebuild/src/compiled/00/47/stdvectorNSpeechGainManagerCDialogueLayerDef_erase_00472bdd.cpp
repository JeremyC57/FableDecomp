// std::vector<NSpeechGainManager::CDialogueLayerDef>::erase(iterator first, iterator last) -- STLport: assign the tail [last, finish) down over first, destroy the leftover elements, pull finish back, return first
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU noinline __destroy_aux template body (tag unnamed, never touched) homes the empty __false_type tag over the live first slot
// Leaf: ?erase@?$vector@UCDialogueLayerDef@NSpeechGainManager@@@std@@QAEPAUCDialogueLayerDef@NSpeechGainManager@@PAU34@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00472BDD, 77 bytes, `ret 8` (thiscall: first, last; the iterator is returned in eax).
// Identity: the catalog's "Vector_ShiftDialogueLayersRight" is behavioural. Stride 0x40 with the element assignment
//   CDialogueLayerDef::operator= @ 0x004663AE (landed as NSpeechGainManager::CDialogueLayerDef::operator=) and the
//   per-element virtual destructor loop @ 0x00450A14: the same element as the landed
//   vector<NSpeechGainManager::CDialogueLayerDef>::operator= @ 0x00466281. Single caller 0x00472B9D.
// Callees: CDialogueLayerDef::operator= @ 0x004663AE, __destroy_aux(first, last, const __false_type&) @ 0x00450A14
//   (catalog "vector<CIntelligentPointer<CThing const>>::_Destroy" is propagated: it calls the virtual destructor,
//   slot 0 with flag 0, every 0x40 bytes).
// Model: STLport 4.x `erase(first, last) { pointer i = __copy_ptrs(last, _M_finish, first, _IsPODType()); _Destroy(i,
//   _M_finish); _M_finish = i; return first; }` with __copy_ptrs/__copy and _Destroy/__destroy inlined down to
//   __destroy_aux, whose empty `_Trivial_destructor()` tag is homed in the dead argument slot's top byte
//   (`lea eax,[ebp+0Bh]`); the copy count reuses the `last` slot. CDialogueLayerDef is local (no engine header).
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace NSpeechGainManager {
struct CDialogueLayerDef {                          // retail sizeof 0x40, polymorphic
    unsigned long Data[15];                         // +0x04
    virtual ~CDialogueLayerDef();
    CDialogueLayerDef& operator=(const CDialogueLayerDef& other);   // 0x004663AE
};
}

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { };

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

// Same-TU stand-in for __destroy_aux @ 0x00450A14 (template body, tag unnamed and never touched): VC7.1 then homes the
// empty tag over the live `first` slot as retail does (GOTCHAS: empty-tag rule).
template <class ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        (&*first)->NSpeechGainManager::CDialogueLayerDef::~CDialogueLayerDef();
}

template <class ForwardIter, class T>
__forceinline void __destroy(ForwardIter first, ForwardIter last, T*)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> __forceinline T* __value_type(const T*) { return (T*)0; }   // _STLP_VALUE_TYPE for pointers

template <class ForwardIter>
__forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy(first, last, __value_type(first));
}

template <class T> class vector {
public:
    typedef T* iterator;
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08
    iterator erase(iterator first, iterator last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, __false_type());
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};

template vector<NSpeechGainManager::CDialogueLayerDef>::iterator
    vector<NSpeechGainManager::CDialogueLayerDef>::erase(iterator first, iterator last);
}