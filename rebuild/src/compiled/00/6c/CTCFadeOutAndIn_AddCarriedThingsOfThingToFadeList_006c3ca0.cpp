// CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList(const CThing& thing) -- add every live thing the thing carries to the fade list unless an intelligent pointer to it is already there
// Leaf: ?AddCarriedThingsOfThingToFadeList@CTCFadeOutAndIn@@AAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C3CA0, 322 bytes, 8-aligned ebp frame, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb donor order of the CTCFadeOutAndIn TU -- ctor (retail 0x006C3C30), AddCarriedThingsOfThingToFadeList
//   (this row), AddThingToFade (0x006C3DF0, landed), UpdateFadingThings (0x006C3E80, landed) -- and the two call sites in
//   UpdateFadingThings (0x006C3ECB with the parent thing, 0x006C3EFA per fading thing), which the landed body already
//   binds to this symbol. The catalog label "COptimisedPrimitive_RemapAndFilter" is propagated.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CTCCarrying::GetAllCarriedThings(vector<CThing*>&) const @ 0x006A2220 (interface 0x46 = CTCCarrying: its
//   GetInterfaceType 0x004E74C5 `push 46h`), CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), STLport find(.., const random_access_iterator_tag&) @ 0x006C4060,
//   vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CTCFadeOutAndIn (FadingThings +0x18); retail CThing UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0.
// Shape: the AddThingToFade test folded into one && condition with the live test, so the find temporary is
//   conditionally destroyed through a frame flag word (`or [esp+14h],1`); the carried list is a local STLport vector
//   whose destructor frees the buffer.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

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
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
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
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    void push_back(const _Tp& __x);                     // 0x004AD520
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    bool _Unchecked_test(unsigned int __pos) const { return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != 0; }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
    const value_type* LowerBound(const K& key) const;   // 0x004365B0
};

class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                    // CClass / CBaseObject / CBaseClass part, UniqueID (+0x18)
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      Dead : 1;                        // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> const T* PeekTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        return (const T*)((it == map->Data.end() || type < it->first) ? map->Data.end() : it)->second;
    }
};

class CTCCarrying {
public:
    void GetAllCarriedThings(std::vector<CThing*>& things) const;   // 0x006A2220
};

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
private:
    void AddCarriedThingsOfThingToFadeList(const CThing& thing);
};

void CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList(const CThing& thing)
{
    if (thing.HasTC((ETCInterfaceType)0x46)) {
        const CTCCarrying* carrying = thing.PeekTC<CTCCarrying>((ETCInterfaceType)0x46);
        std::vector<CThing*> things;
        carrying->GetAllCarriedThings(things);
        for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
            CThing* carried = *it;
            if (carried && !carried->Dead &&
                std::find(FadingThings.begin(), FadingThings.end(), CIntelligentPointer<CThing>(carried)) == FadingThings.end())
                FadingThings.push_back(CIntelligentPointer<CThing>(carried));
        }
    }
}