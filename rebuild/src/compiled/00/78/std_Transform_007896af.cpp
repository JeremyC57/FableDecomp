// std::_Transform(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, back_insert_iterator<vector<const CThing*> > dest, CGetThingFromIntelligentPointer func) -- push the thing behind every (intelligent pointer, value) pair of [first, last) onto the destination vector, return the advanced back inserter
// Leaf: _Transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007896AF, length 55 bytes (unaligned: a size-optimised TU), `ret 0Ch`.
// Callers: CTCMeleeTrap::PerformPossibleTrapDamage @ 0x0078891D (call 0x00788993: `ecx` = the returned inserter's slot,
//   `edx` = the pair vector's begin [this+0xC], then the end [this+0x10], the back inserter (a local vector's address) and
//   the zeroed one-byte functor are pushed).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, vector<const CThing*>::push_back(const CThing* const&) @ 0x0049C450.
// Identity: Ego_r.pdb names `std::_Transform<pair<CIntelligentPointer<CThing>,long>*, back_insert_iterator<vector<const
//   CThing*> >, `anonymous namespace'::CGetThingFromIntelligentPointer>` as `__fastcall` (`YI`), returning the inserter.
// Shape: Dinkumware `for (; _First != _Last; ++_First, ++_Dest) *_Dest = _Func(*_First); return (_Dest);` in a /Gr TU
//   compiled for size with frame pointers (`#pragma optimize("s", on)` + `optimize("y", off)`): `push ebp ; mov ebp,esp`,
//   the hidden result pointer in ecx (kept in edi), first in edx, the loop entered through a `jmp` to its bottom test.
//   The functor's result is an rvalue bound to push_back's `const T&`, so VC homes it in the dead `dest` argument slot
//   (`mov [ebp+0Ch],eax ; lea eax,[ebp+0Ch]`) once the inserter's container pointer lives in ebx. back_insert_iterator
//   needs its user constructor: without it the 4-byte inserter is returned in eax and first/last move to ecx/edx (50 v 55).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void* PData;                               // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> class vector {
public:
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    void push_back(const T& value);            // 0x0049C450 for vector<const CThing*>
};
template <class C> class back_insert_iterator {
public:
    C* container;
    explicit back_insert_iterator(C& cont) : container(&cont) { }
    back_insert_iterator& operator=(const typename C::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};
template <class InIt, class OutIt, class Fn1>
OutIt __fastcall _Transform(InIt _First, InIt _Last, OutIt _Dest, Fn1 _Func)
{
    for (; _First != _Last; ++_First, ++_Dest)
        *_Dest = _Func(*_First);
    return (_Dest);
}
}

template <> class std::vector<const CThing*> {
public:
    typedef const CThing* value_type;
    const CThing** _Myfirst;
    const CThing** _Mylast;
    const CThing** _Myend;
    void push_back(const CThing* const& value);     // 0x0049C450
};

namespace {
struct CGetThingFromIntelligentPointer {
    const CThing* operator()(const std::pair<CIntelligentPointer<CThing>, long>& entry) const { return entry.first.GetPItem(); }
};
}

template std::back_insert_iterator<std::vector<const CThing*> > __fastcall std::_Transform(std::pair<CIntelligentPointer<CThing>, long>*, std::pair<CIntelligentPointer<CThing>, long>*, std::back_insert_iterator<std::vector<const CThing*> >, CGetThingFromIntelligentPointer);