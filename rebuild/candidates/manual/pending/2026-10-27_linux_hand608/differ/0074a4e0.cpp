// CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature) -- drop every open generation position the creature cannot navigate to from the generator (swap with the last, pop)
// Grade: DIFFER(234v234) -- names now PDB-exact (GetPos, PeekTC<const CTCCreatureNavigation>, const IsAbleToNavigateFromPosToPos); residue: the creature is read into eax (`lea ebx,[eax+44h]`, retail ebx `add ebx,44h`) and the index lives in its own frame slot (retail: the dead creature argument slot, zeroed after the PeekTC). Declaring the index after the PeekTC puts it in the creature slot as retail but merges its zero with the offset register (`xor ebx,ebx ; cmp edi,ebx ; mov [i],ebx`, 232 bytes). Tried: index declared after PeekTC (while and for), single count variable, for-init index.
// Leaf: ?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074A4E0, 234 bytes (+6 int3), __thiscall `ret 4`.
// Callees: CThing::GetPos @ 0x004C73D0 (the generator's parent), CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) const @ 0x004365B0 (the inline PeekTC on the
//   creature, interface 7 = CTCCreatureNavigation), CTCCreatureNavigation::IsAbleToNavigateFromPosToPos const @
//   0x006CB620 (tolerance 0).
// Label: the manifest's "VertexBuffer_FilterNavigableVertices" is descriptive; Ego_r.pdb / FableWin.pdb name
//   `?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z`. Layout: OpenPositions (STLport vector<C3DVector>)
//   at +0x60 as engine/CTCCreatureGenerator.h (retail container widths), ParentThing +0x04 (CTCBase).
// Shape: `count = OpenPositions.size(); if (count > 0) { pos = GetPos(); creature.PeekTC(&nav) (nav left unset when the
//   bit is clear: retail never stores it on that path); i = 0; while (i < count) if (!nav->IsAble...(pos,
//   OpenPositions[i], 0)) { --count; OpenPositions[i] = OpenPositions[count]; pop_back(); } else ++i; }`.
#include <stddef.h>
class C3DVector {
public:
    float X, Y, Z;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef size_t size_type;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void pop_back() { --this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> { };
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    const_iterator LowerBound(const K& key) const;            // 0x004365B0 for the TC map (the const overload)
};
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    const C3DVector& GetPos() const;                                      // 0x004C73D0
    template <class T> __forceinline bool PeekTC(T** out) const
    {
        if (UsingComponents._M_getword(T::INTERFACE_TYPE) & UsingComponents._S_maskbit(T::INTERFACE_TYPE)) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };

class CTCCreatureNavigation {
public:
    enum { INTERFACE_TYPE = 7 };
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620
};

class CTCCreatureGenerator {
public:
    virtual void v00();
protected:
    void DoNavCheck(const CThingCreatureBase& creature);
    CThing*                ParentThing;                       // +0x04 (CTCBase)
    unsigned char          _pad08[0x60 - 0x08];
    std::vector<C3DVector> OpenPositions;                     // +0x60
};

void CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature)
{
    long numPositions = OpenPositions.size();
    long i = 0;
    if (numPositions > 0) {
        C3DVector position = ParentThing->GetPos();
        const CTCCreatureNavigation* navigation;
        creature.PeekTC(&navigation);
        long count = numPositions;
        while (i < count) {
            if (!navigation->IsAbleToNavigateFromPosToPos(position, OpenPositions[i], 0.0f)) {
                --count;
                OpenPositions[i] = OpenPositions[count];
                OpenPositions.pop_back();
            }
            else
                ++i;
        }
    }
}
