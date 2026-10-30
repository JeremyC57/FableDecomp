// CGameScriptInterface::GetAllowedToFollow(CScriptThing thing) -- script query: true when the script thing resolves no thing, else the thing's CTCAIScratchpad::AllowedToFollow flag (read through CThing::GetTC<CTCAIScratchpad>); the by-value CScriptThing is destroyed inline
// Leaf: ?GetAllowedToFollow@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A6390, 240 bytes, thiscall, `ret 0Ch` (the 12-byte CScriptThing by value).
// Identity (proven): CGameScriptInterface vtable 0x01260F0C slot 833; Ego_r.pdb ?GetAllowedToFollow@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
//   (donor 0x008E4920) and the manifest label agree; it reads the bool at +0x6C of interface 0xAA = Ego_r.pdb
//   CTCAIScratchpad::AllowedToFollow (no STL member before +0x7C, so the PDB offset is retail's).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020, CBaseClass::~CBaseClass() 0x0099A2E0, operator
//   delete 0x00BFE9BC; the pointee's virtual GetThing (slot 11, +0x2C).
// Shape (family K032, representative 0x008A60D0): the landed J018 getter CGameScriptInterface::GetDisableHeadLooking 0x008A6BF0
//   with `return true` for an unresolved thing (`mov al,1`) and a direct member read (no CTCAIScratchpad getter of this
//   flag is in the PDB name sets). Header debt: engine/CScriptThing.h / CGameScriptInterface.h model the vptr as data.
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
    bool                  IsPushableByHero;                   // +0x79
    bool                  LookForFiniteTime;                  // +0x7A
    bool                  AvoidRegionExits;                   // +0x7B
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

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::GetAllowedToFollow(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->AllowedToFollow;
}