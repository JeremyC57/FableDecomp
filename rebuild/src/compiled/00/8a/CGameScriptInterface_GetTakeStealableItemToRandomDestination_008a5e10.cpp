// CGameScriptInterface::GetTakeStealableItemToRandomDestination(CScriptThing thing) -- script query: false when the script thing resolves no thing, else the thing's CTCAIScratchpad::TakeStealableItemToRandomDestination flag (read through CThing::GetTC<CTCAIScratchpad>); the by-value CScriptThing is destroyed inline
// Leaf: ?GetTakeStealableItemToRandomDestination@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A5E10, 240 bytes, thiscall, `ret 0Ch` (the 12-byte CScriptThing by value).
// Identity (proven): CGameScriptInterface::GetTakeStealableItemToRandomDestination(CScriptThing); evidence: CGameScriptInterface vtable slot 827 (ghidra_out/cgsi_slots_aligned.tsv) holds this body as ?GetTakeStealableItemToRandomDestination@CGameScriptInterface@@UAE_NVCScriptThing@@@Z, which FableWin.pdb and Ego_r.pdb hold; it reads the bool at +0x6A of interface 0xAA, Ego_r.pdb CTCAIScratchpad::TakeStealableItemToRandomDestination (the PDB getter ?GetTakeStealableItemToRandomDestination@CTCAIScratchpad@@QBE_NXZ; the landed GetActionFrequency 0x008A3D20 reads ActionFrequency +0x40 of the same 0xAA)
// Label: the manifest's mangled "?GetTakeStealableItemToRandomDestination@CGameScriptInterface@@UAE_NVCScriptThing@@@Z" is this row's own CGSI slot name (family J018: six
//   CTCAIScratchpad flag getters differing only in the field offset; retail has no identical-code folding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020
//   (Ego_r.pdb names it; the catalog's bindings there are stand-ins), CBaseClass::~CBaseClass() 0x0099A2E0 (the CScriptThing
//   destructor's base call), operator delete 0x00BFE9BC; the pointee's virtual GetThing (slot 11, +0x2C).
// Shape: `if (!thing.GetThing()) return false; CTCAIScratchpad* tc; thing.GetThing()->GetTC(&tc); return tc->GetTakeStealableItemToRandomDestination();`
//   -- CScriptThing::GetThing forwards to the counted pimpl; CThing::GetTC<T>(T**) (Ego_r.pdb
//   ??$GetTC@VCTCAIScratchpad@@@CThing@@QAE_NPAPAVCTCAIScratchpad@@@Z) tests UsingComponents through the STLport bitset
//   reference proxy (`test ch, 4`) and looks the type up through a map pointer (LowerBound on a key copy, the end / greater
//   check against the constant); its result is ignored, so a missing component reads the unset local (retail's stale load).
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44; CScriptThing = vptr
//   (vtable 0x01238C8C, RTTI .?AVCScriptThing@@) + CCountedPointer<CScriptThing> PImp; CTCAIScratchpad has no STL member
//   before +0x7C, so its Ego_r.pdb offsets are retail's (TakeStealableItemToRandomDestination +0x6A). The engine CScriptThing / CGameScriptInterface
//   headers model the vptr as a data member, so both are local here (no CThing / CTCAIScratchpad header exists).
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

class C3DVector { public: float X, Y, Z; };
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
enum EScriptingStateGroups { EScriptingStateGroups_Force32 = 0x7fffffff };
class CTCAIScratchpad {                                      // Ego_r.pdb members (= retail offsets up to +0x7C)
public:
    enum { INTERFACE_TYPE = 0xAA };
    void*                 __vfptr_and_CTCBase[3];             // +0x00 CTCBase (vptr, ParentThing, flags)
    unsigned long         ThankingPhrase;                     // +0x0C
    unsigned long         IgnoringPhrase;                     // +0x10
    C3DVector             WanderCentrePoint;                  // +0x14
    float                 WanderMinDistance;                  // +0x20
    float                 WanderMaxDistance;                  // +0x24
    long                  GossipCounter;                      // +0x28
    long                  MaxGossipPhrase;                    // +0x2C
    unsigned long         WarningPhrase;                      // +0x30
    unsigned long         BeerRequestPhrase;                  // +0x34
    EScriptingStateGroups ScriptingStateGroup;                // +0x38
    float                 MaxHeroReactionDistance;            // +0x3C
    long                  ActionFrequency;                    // +0x40
    float                 ActionFrequencyVariation;           // +0x44
    CCharString           Action;                             // +0x48
    bool                  FaceHeroForAction;                  // +0x4C
    CCharString           TargetName;                         // +0x50
    float                 FollowDistance;                     // +0x54
    bool                  AttackHeroOnSight;                  // +0x58
    long                  TimeToSpendHarassingHero;           // +0x5C
    float                 CombatNearbyEnemyFleeingBreakOffRange;   // +0x60
    float                 CombatNearbyBreakOffRange;          // +0x64
    bool                  StealStealableItems;                // +0x68
    bool                  RecoverStealableItems;              // +0x69
    bool                  TakeStealableItemToRandomDestination;    // +0x6A
    bool                  KillSelfAndStealableItemAfterReachingDestination;   // +0x6B
    bool                  AllowedToFollow;                    // +0x6C
    CCharString           TableName;                          // +0x70
    CCharString           SeatName;                           // +0x74
    bool                  DisableHeadLooking;                 // +0x78
    bool GetTakeStealableItemToRandomDestination() const { return TakeStealableItemToRandomDestination; }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo {                                       // 12 bytes (Ego_r.pdb)
public:
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04 (retail passes the pointee in ecx)
    void* Data;                                              // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass {                    // vtable 0x01238C8C
public:
    virtual ~CScriptThing() { }
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CThing* GetThing() const { return PImp.Data == 0 ? 0 : PImp.Data->GetThing(); }   // slot 11 (+0x2C)
    CCountedPointer<CScriptThing> PImp;                      // +0x04
};

class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    virtual bool GetTakeStealableItemToRandomDestination(CScriptThing thing);
};

bool CGameScriptInterface::GetTakeStealableItemToRandomDestination(CScriptThing thing)
{
    if (!thing.GetThing())
        return false;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->GetTakeStealableItemToRandomDestination();
}