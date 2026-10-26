// CCombatSequence_WillEnflame::OnStart() -- when the sequence's creature has a will-power indicator component, restart its recharge with Reset(-1.0f) (a negative time means the def's recharge time)
// Leaf: ?OnStart@CCombatSequence_WillEnflame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909730, 71 bytes (+ int3 padding to 80), thiscall, plain `ret` (virtual).
// Identity (proven): CCombatSequence_WillEnflame::OnStart() (Ego_r.pdb ?OnStart@CCombatSequence_WillEnflame@@UAEXXZ); evidence: slot 17 of the CCombatSequence_WillEnflame vtable 0x0127A25C (RTTI; base CCombatSequenceBase)
// Label: the manifest's "CCombatSequence_WillEnflame::OnStart" (doubled class) names this row's own class; family J020: six byte-identical
//   will-power combat sequences (retail has no identical-code folding, so each address is one class's OnStart).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020 (as J018), CTCAICreatureWillPowerIndicator::
//   Reset(float) 0x007E0E70 (?Reset@CTCAICreatureWillPowerIndicator@@QAEXM@Z, Ego_r.pdb / FableWin.pdb; unbound in the catalog,
//   the landed label agrees).
// Shape: CThing::GetTC (interface 0xCE = CTCAICreatureWillPowerIndicator, the landed GetInterfaceType 0x007E1140: bit 14 of
//   UsingComponents word 6, `test ch, 40h`) with its result tested. Layout: CCombatSequenceBase (Ego_r.pdb members; retail
//   0x28 bytes with the 12-byte STLport vector) Creature +0x1C. Local model: the generated engine/CCombatSequence_WillEnflame.h has the Ego_r
//   size (0x2C) and a data-member vptr.
#include <stddef.h>
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        size_t _M_bpos;
        reference(bitset<_Nb>& __b, size_t __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](size_t __pos) { return reference(*this, __pos); }
    bool operator[](size_t __pos) const { return _Unchecked_test(__pos); }
    bool _Unchecked_test(size_t __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    iterator LowerBound(const K& key);                        // 0x0040F020 for the TC map
};

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed (CTCAIScratchpad is 0xAA)
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};


class CThingCreatureBase : public CThing { };
class CTCAICreatureWillPowerIndicator {
public:
    enum { INTERFACE_TYPE = 0xCE };
    void Reset(float time);                                  // 0x007E0E70
};
class CCombatAction;
class CCombatSequenceBase {                                  // retail 0x28 bytes
public:
    virtual ~CCombatSequenceBase();
    virtual void OnStart();                                  // retail slot 17
    bool                Running;                             // +0x04
    long                NoAvailableActions;                  // +0x08
    long                CurrentActionIndex;                  // +0x0C
    std::vector<CCombatAction, std::allocator<CCombatAction> > CombatActions;   // +0x10
    CThingCreatureBase* Creature;                            // +0x1C
    CThingCreatureBase* Target;                              // +0x20
    bool                CreateCalled;                        // +0x24
};
class CCombatSequence_WillEnflame : public CCombatSequenceBase {                // RTTI: CCombatSequence_WillEnflame <- CCombatSequenceBase
public:
    virtual void OnStart();
};

void CCombatSequence_WillEnflame::OnStart()
{
    CTCAICreatureWillPowerIndicator* indicator;
    if (Creature->GetTC(&indicator))
        indicator->Reset(-1.0f);
}