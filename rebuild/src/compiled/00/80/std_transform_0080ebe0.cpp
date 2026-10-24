// std::transform(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, std::back_insert_iterator<std::vector<CThing*> > result, CGetThingFromIntelligentPointer opr) -- STLport transform of an intelligent-pointer array into a vector of raw thing pointers through a back inserter, returning the inserter
// Leaf: ??$transform@
// Retail: Fable.exe @ 0x0080EBE0, 107 bytes (+ int3 padding), `ret 0Ch`: __fastcall (the algorithm TUs are /Gr) returning
//   the 4-byte back_insert_iterator through the hidden pointer in ecx (it has a user constructor), first in edx; last,
//   the inserter and the empty functor (a pushed byte) on the stack.
// Caller: CTCDExplosion::UpdateExplosiveAdditionalImpulse 0x0080C5D0. Callees: CBaseIntelligentPointer::GetPItem
//   0x00A01B50, vector<CThing*>::_M_insert_overflow 0x00436AC0 (stand-in body below).
// Identity: Ego_r.pdb `??$_Transform@PAV?$CIntelligentPointer@VCThing@@@@V?$back_insert_iterator@V?$vector@PAVCThing@@...
//   VCGetThingFromIntelligentPointer@?A0x41ace303@@` (the CTCDExplosion TU's anonymous-namespace functor; FableWin.pdb has
//   the same functor in the TLC TU). Label: catalogued `std_vector_CopyToIntelligentPointer` (descriptive, direction wrong).
// Shape: STLport `for (; first != last; ++first, ++result) *result = opr(*first); return result;` -- the inserter's
//   container pointer is held in esi and its argument slot reused for the pushed value AND the overflow's __true_type tag
//   (same-TU auto_inline(off)/noinline overflow body that never touches its tag -- GOTCHA 1276).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

namespace std {
struct __true_type { __true_type() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                             // +0x00
    T* _M_finish;                                            // +0x04
    T* _M_end_of_storage;                                    // +0x08
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line STLport growth path (GOTCHAS: it reads x only through its reference and never
// touches the tag, so the caller homes the tag temporary on a dead argument slot as retail does).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&,
                                                        unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        *new_finish = *p;
    for (unsigned int n = fill_len; n > 0; --n, ++new_finish)
        *new_finish = x;
    if (!atend)
        for (T* q = position; q != _M_finish; ++q, ++new_finish)
            *new_finish = *q;
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage = new_start + len;
}
#pragma auto_inline()

template <class Container> class back_insert_iterator {
protected:
    Container* container;
public:
    explicit back_insert_iterator(Container& x) : container(&x) { }
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
};
}
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObject* PData;                                      // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
namespace {
class CGetThingFromIntelligentPointer {                      // empty functor (anonymous namespace of the TU)
public:
    CThing* operator()(const CIntelligentPointer<CThing>& p) const { return p.GetPItem(); }
};
}
namespace std {
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter __fastcall transform(InputIter first, InputIter last, OutputIter result, UnaryOperation opr)
{
    for (; first != last; ++first, ++result)
        *result = opr(*first);
    return result;
}
template back_insert_iterator<vector<CThing* > > __fastcall transform(CIntelligentPointer<CThing>*,
    CIntelligentPointer<CThing>*, back_insert_iterator<vector<CThing* > >, CGetThingFromIntelligentPointer);
}