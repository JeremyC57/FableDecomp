// CTCHeroMorph::GetTattooThings(vector<CDefPointer<const CThingBaseDef>>& things) const -- refill the caller's vector with the hero's tattoo thing defs
// Leaf: ?GetTattooThings@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071DE20, length 96 bytes. __fastcall, `ret 4`. Ego_r.pdb:
//         `?GetTattooThings@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@...@std@@@Z`.
// Callees: std::__copy<CDefPointer*> @ 0x0071FA60 (the erase), std::__destroy_aux<CDefPointer*> @ 0x005B26E9,
//          vector<CDefPointer<const CThingBaseDef> >::reserve @ 0x00721240,
//          std::__copy<const CDefPointer*, back_insert_iterator<...> > @ 0x00721F70 (returns the iterator through ecx).
// Identity: the manifest label "Tattoo_ApplyChanges" is wrong. `this` reads the vector at +0x1C (CTCHeroMorph::Tattoos,
//   Ego_r.pdb +0x2C: the two 12-byte lists before it are 4 bytes in retail -- the member the landed GetTattoos 0x0071DD50
//   reads); out-of-TU callers 0x005AD361 / 0x006D1691; it sits between GetTattoos 0x0071DD50 and UpdateTextureMorphs
//   0x0071DE80 in the CTCHeroMorph block.
// Shape: `things.clear(); things.reserve(Tattoos.size()); copy(Tattoos.begin(), Tattoos.end(), back_inserter(things));`
//   with STLport's clear = erase(begin(), end()) (`__copy_ptrs` -> the out-of-line counted __copy, `_Destroy` ->
//   out-of-line __destroy_aux) and copy -> __copy_aux -> the out-of-line random-access __copy; every empty tag and the
//   returned back_insert_iterator share the dead `things` argument slot. The __copy instances are a same-TU stand-in body
//   under auto_inline(off): with a declaration only VC7.1 gives the returned iterator its own frame word (`push ecx`).
struct __false_type { __false_type() {} };

class CThingBaseDef;
template <class T> class CDefPointer {                  // retail 4 bytes: the counted def pointer
public:
    T* Object;
    CDefPointer(const CDefPointer<T>& other);
    ~CDefPointer();
    CDefPointer<T>& operator=(const CDefPointer<T>& other);
};

namespace std {
struct output_iterator_tag {};
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };

template <class _Category, class _Tp, class _Distance, class _Pointer, class _Reference>
struct iterator {};

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x005B26E9
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

// Same-TU stand-in body for the two __copy instances (0x0071FA60 / 0x00721F70, later in this TU): VC7.1 uses what it
// learns from it.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void reserve(unsigned int __n);                     // 0x00721240
    void push_back(const _Tp& __x);
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

template <class _Container>
class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void> {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __val)
    {
        container->push_back(__val);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};
template <class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x) { return back_insert_iterator<_Container>(__x); }
}

class CThing;
class CTCHeroMorph {
public:
    virtual ~CTCHeroMorph();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x1C - 0x08];                  // CTCBase tail, PMeshMorphSet, the two particle-emitter lists
    std::vector<CDefPointer<const CThingBaseDef> > Tattoos;   // +0x1C (Ego_r.pdb +0x2C)
    void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >& things) const;
};

void CTCHeroMorph::GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >& things) const
{
    things.clear();
    things.reserve(Tattoos.size());
    std::copy(Tattoos.begin(), Tattoos.end(), std::back_inserter(things));
}