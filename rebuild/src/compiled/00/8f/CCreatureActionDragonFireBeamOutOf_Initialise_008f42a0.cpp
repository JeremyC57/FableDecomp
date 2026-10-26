// CCreatureAction_DragonFireBeamOutOf::Initialise() -- set the combat animation FIRE_BEAM_OUT_OF, run the CCombatActionBase initialisation, then remove creature mode 0x4B through the owner's mode manager when it has one
// Leaf: ?Initialise@CCreatureAction_DragonFireBeamOutOf@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F42A0, 117 bytes (+ int3 padding to 128), thiscall, plain `ret` (virtual).
// Identity (proven): CCreatureAction_DragonFireBeamOutOf::Initialise() (Ego_r.pdb / FableWin.pdb ?Initialise@CCreatureAction_DragonFireBeamOutOf@@UAEXXZ); evidence: slot 12 of the CCreatureAction_DragonFireBeamOutOf vtable 0x01286EE4 (RTTI; bases CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass); string 0x012884DC "FIRE_BEAM_OUT_OF"
// Label: the manifest's "CCreatureAction_DragonFireBeamOutOf::Initialise" (doubled class) names this row's own class; family J019: six "out of" combat
//   actions that differ in the animation string, the mode removed and (BriarRose) the base whose Initialise runs.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z) and ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0 (Ego_r.pdb; the catalog also lists SetPDef / SetName /
//   SetAnimName stand-ins there), CCombatActionBase::Initialise() 0x00857520 (catalog-bound, Ego_r.pdb ?Initialise@CCombatActionBase@@UAEXXZ),
//   CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r.pdb), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound
//   0x0040F020 (as J018), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0 (Ego_r.pdb spelling, global class).
// Shape: the animation name is a block-scoped local (ctor / call / dtor before the base call); the qualified base Initialise; CThing::GetTC
//   (interface 0x31 = CTCCreatureModeManager: bit 17 of UsingComponents word 1, `test [eax+24h], 20000h`) with its result
//   tested. Layout: CCreatureActionBase PCreature +0x08 (Ego_r.pdb, CIntelligentPointer<CThingCreatureBase>); retail
//   CCombatActionBase is 0x114 bytes. Local model: engine/CCreatureActionBase.h carries the Ego_r layout (0xB4) and a
//   data-member vptr.
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


class CCharStringData;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharStringData* PStringData;
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CThingCreatureBase : public CThing { };
class CTCCreatureModeManager {
public:
    enum { INTERFACE_TYPE = 0x31 };
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CCreatureActionBase {                                  // 0xA8 bytes in retail (vptr first)
public:
    virtual ~CCreatureActionBase();
    virtual void Initialise();                               // retail slot 12
    unsigned char _pad04[4];                                 // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _body10[0xA8 - 0x10];
};
class CCombatActionBase : public CCreatureActionBase {      // 0x114 bytes in retail
public:
    virtual void Initialise();                               // 0x00857520
    void SetCombatAnimation(const CCharString& name);        // 0x008574D0
    unsigned char _bodyA8[0x114 - 0xA8];
};

class CCreatureAction_DragonFireBeamOutOf : public CCombatActionBase {                           // RTTI: CCreatureAction_DragonFireBeamOutOf <- CCombatActionBase <- CCreatureActionBase <- CBaseObject <- CBaseClass
public:
    virtual void Initialise();
};

void CCreatureAction_DragonFireBeamOutOf::Initialise()
{
    {
        CCharString name("FIRE_BEAM_OUT_OF");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature->GetTC(&modeManager))
        modeManager->RemoveMode(0x4B);
}