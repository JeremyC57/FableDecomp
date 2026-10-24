// CTCFadeOutAndIn::UpdateFadingThings() -- drop dead things from the fade list, then add the things carried by the parent and by every fading thing
// Leaf: ?UpdateFadingThings@CTCFadeOutAndIn@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C3E80, 184 bytes (+ int3 padding), thiscall `ret`.
// Callers: one site, 0x006C3F65 (CTCFadeOutAndIn::FrameUpdate 0x006C3F40, landed as `RemoveDeadFadingThings`).
// Callees: STLport find_if(.., CIsThingDead, const random_access_iterator_tag&) @ 0x006C4270 and
//   remove_copy_if(.., CIsThingDead) @ 0x006C4210 (both __fastcall `ret 8`, this chunk's row 37),
//   vector<CIntelligentPointer<CThing>>::erase(first, last) @ 0x004AC0F0,
//   CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList(const CThing&) @ 0x006C3CA0,
//   vector<CIntelligentPointer<CThing>> copy constructor @ 0x0062ED40 (landed), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//   free @ 0x00BFEA14 (the STLport malloc allocator); the element destructor runs through vtable slot 0 with 0.
// Layout (Ego_r.pdb CTCFadeOutAndIn, 0x28): ParentThing +4 (CTCBase), FadingThings +0x18 (12-byte STLport vector of
//   8-byte CIntelligentPointer<CThing>).
// Shape: `if (!empty()) erase(remove_if(begin(), end(), CIsThingDead()), end());` (STLport remove_if inline: find_if, the
//   `first == last` early result, remove_copy_if from first + 1; the empty predicate and tag temporaries share the frame),
//   then the parent's carried things, then a local copy of the list walked by index (the trip count hoisted into a
//   down-counter) -- the copy's destructor inline: virtual element destructors, `if (start) free(start)`. The vector copy
//   constructor and its _Vector_base(n, a) need same-TU `auto_inline(off)` bodies: declared-only, the local copy's
//   address escapes and every loop re-reads its bounds (203v184); CIsThingDead needs a user constructor (a POD
//   `CIsThingDead()` zero-stores its byte).
// Label correction: the catalog `Region_RemoveDeadThings` is descriptive; Ego_r.pdb names CTCFadeOutAndIn::UpdateFadingThings.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CBaseObject;
class CBaseObjectPointer;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CIsThingDead {
public:
    CIsThingDead() { }
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> void __fastcall _Construct(_Tp* __p, const _Tp& __val);                  // 0x004AA030

template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred,
                              const random_access_iterator_tag&);                               // 0x006C4270

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return find_if(__first, __last, __pred, random_access_iterator_tag());
}

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result,
                                      _Predicate __pred);                                       // 0x006C4210

template <class _ForwardIter, class _Predicate>
inline _ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    __first = find_if(__first, __last, __pred);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy_if(++__next, __last, __first, __pred);
    }
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a);  // 0x0062ED90
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    vector(const vector& __x);                          // 0x0062ED40
    ~vector()
    {
        for (_Tp* __p = this->_M_start; __p != this->_M_finish; ++__p)
            _Destroy(__p);
    }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last);  // 0x004AC0F0
};

// The two constructors that see the local copy's address, same-TU and out of line (0x0062ED90 / 0x0062ED40):
// with their bodies visible VC7.1 proves the local vector never escapes and keeps its bounds in registers.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
_Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc& __a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
{
    _M_start = _M_end_of_storage.allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
{
    _Tp* __cur = this->_M_start;
    for (const _Tp* __first = __x._M_start; __first != __x._M_finish; ++__first, ++__cur)
        _Construct(__cur, *__first);
    this->_M_finish = __cur;
}
#pragma auto_inline(on)

}

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};

class CTCFadeOutAndIn : public CTCBase {
public:
    bool FadingOut;                                     // +0x0C
    bool FadingIn;                                      // +0x0D
    float Alpha;                                        // +0x10
    long FramesLeftForFade;                             // +0x14
    std::vector<CIntelligentPointer<CThing> > FadingThings;   // +0x18
    void AddCarriedThingsOfThingToFadeList(const CThing& thing);   // 0x006C3CA0
    void UpdateFadingThings();
};

void CTCFadeOutAndIn::UpdateFadingThings()
{
    if (!FadingThings.empty())
        FadingThings.erase(std::remove_if(FadingThings.begin(), FadingThings.end(), CIsThingDead()), FadingThings.end());
    AddCarriedThingsOfThingToFadeList(*ParentThing);
    std::vector<CIntelligentPointer<CThing> > things(FadingThings);
    for (unsigned int i = 0; i < things.size(); ++i)
        AddCarriedThingsOfThingToFadeList(*things[i].GetPItem());
}