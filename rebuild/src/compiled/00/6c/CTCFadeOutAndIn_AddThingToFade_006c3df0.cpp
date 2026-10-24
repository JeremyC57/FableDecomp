// CTCFadeOutAndIn::AddThingToFade(CThing& thing) -- add the thing to the fade list unless an intelligent pointer to it is already there
// Leaf: ?AddThingToFade@CTCFadeOutAndIn@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C3DF0, 142 bytes (+ int3 padding), thiscall `ret 4`.
// Callers: one site, 0x004A3D84.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem(CBaseObject*) @ 0x00A01B90,
//   ~CBaseIntelligentPointer() @ 0x00A01C10 (all landed); CIntelligentPointer<CThing> vtable @ 0x01238C6C;
//   STLport find(CIntelligentPointer<CThing>*, .., const CIntelligentPointer<CThing>&, const random_access_iterator_tag&)
//   @ 0x006C4060 (landed, __fastcall); vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520.
// Layout (Ego_r.pdb CTCFadeOutAndIn, 0x28): FadingThings at +0x18, a 12-byte STLport vector of 8-byte CIntelligentPointer<CThing>.
// Shape: `if (find(begin, end, CIntelligentPointer<CThing>(&thing)) == end) FadingThings.push_back(CIntelligentPointer<CThing>(&thing));`
//   -- each pointer is a frame temporary (base ctor, vtable store, SetPItem), the find temporary dies before the branch
//   (`sete bl`), the random-access tag temporary takes the dead `thing` argument slot, and `end()` is read once.
// Label correction: the catalog `Region_AddPointerIfNew` is descriptive; Ego_r.pdb names CTCFadeOutAndIn::AddThingToFade.
class CBaseObject;
class CBaseObjectPointer;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                  const random_access_iterator_tag&);                          // 0x006C4060

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return find(__first, __last, __val, random_access_iterator_tag());
}

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const _Tp& __x);                     // 0x004AD520
};

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
    void AddThingToFade(CThing& thing);
};

void CTCFadeOutAndIn::AddThingToFade(CThing& thing)
{
    if (std::find(FadingThings.begin(), FadingThings.end(), CIntelligentPointer<CThing>(&thing)) == FadingThings.end())
        FadingThings.push_back(CIntelligentPointer<CThing>(&thing));
}