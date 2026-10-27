// std::pair::pair(const first_type& a, const second_type& b) -- (pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> >) STLport member-wise constructor: copy the key word, copy the intrusive counted pointer and add a reference
// Leaf: ??0?$pair@UCUnprovenKey_005f82bc@@V?$CIVCountedPointer@VCUnprovenPointee_005f82bc@@@@@std@@QAE@ABUCUnprovenKey_005f82bc@@ABV?$CIVCountedPointer@VCUnprovenPointee_005f82bc@@@@@Z
// Retail: Fable.exe @ 0x005F82BC, 29 bytes, thiscall `ret 8` (&a, &b); /Os TU (packed address).
// Identity (descriptive types): the STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}` shape over a
// 4-byte key and a CIVCountedPointer (Ego_r.pdb: one `CIVCountedPointeeBase* Object`; the pointee's IVRefCount at +4 is
// bumped by the inline copy constructor). Its only caller is the unreferenced make_pair 0x005F89DC (`push b ; mov ecx,result
// ; push a ; call`), so neither type can be proven: CUnprovenKey_005f82bc / CUnprovenPointee_005f82bc.
// Callees: none.
// Shape: `mov eax,ecx` first, the key copied, then the pointer copy stored before its null test (`test edx,edx ; mov
// [eax+4],edx ; je`) and `inc dword ptr [edx+4]`.
#pragma optimize("s", on)
struct CUnprovenKey_005f82bc {
    long Word;
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    unsigned long IVRefCount;                               // +0x04
};

class CUnprovenPointee_005f82bc : public CIVCountedPointeeBase {
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                          // +0x00
    CIVCountedPointer(const CIVCountedPointer<T>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

template struct std::pair<CUnprovenKey_005f82bc, CIVCountedPointer<CUnprovenPointee_005f82bc> >;