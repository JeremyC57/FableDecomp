// NScript::CQ_TraderConflictGoodScript::UpdateLiveEnemies() -- rebuild AllCreatures: fetch every creature except the hero, drop the chickens (CREATURE_NEW_CHICKEN_04 def) and the quest's own people (TraderToRescue, BodyGuard, RingFighter, FisticuffsMember, Tyler), then drop every entity following the hero
// Leaf: ?UpdateLiveEnemies@CQ_TraderConflictGoodScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFC320, 777 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb 0x00E3C790 / FableWin.pdb 0x038B7740 ?UpdateLiveEnemies@CQ_TraderConflictGoodScript@NScript@@QAEXXZ; Ego_r donor
//   order WatchForKilledPeople 0x00E3C6E0 -> UpdateLiveEnemies -> WatchForHittingEnemies 0x00E3CCB0 -> WatchForRegionTransitions
//   0x00E3CFC0 is retail's 0x00DFC290 -> 0x00DFC320 -> 0x00DFC630 -> 0x00DFC920; its callers are exactly those class members
//   (WatchForHittingEnemies 0x00DFC8C1 / 0x00DFC8FB, WatchForRegionTransitions 0x00DFC97D, CBanditExtra::Main 0x00DFCBF2,
//   AttackPeople 0x00DFD609); the body rebuilds the PDB member AllCreatures (Ego_r +0x5C -> retail +0x48). The catalog label
//   "CScriptBase_ProcessNPCConversations" is wrong.
// Callees: std::vector<CScriptThing>::erase(first, last) 0x008AC9D0 / erase(pos) 0x00CD3152 / ~vector 0x008AC970,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBasicString<char>::Compare 0x00411570 (inline CCharString::operator==),
//   ~CCharString 0x0099EAE0; CScriptThing slots 1 GetName / 2 GetDefName / 78 IsEqualTo (inline operator==, FableWin.pdb
//   ?IsEqualTo@CScriptThing@@MBE_NABV1@@Z on the implementation objects); CGameScriptInterface slots 70 GetHero,
//   84 GetAllCreaturesExcludingHero, 231 GetFollowingEntityList (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "CREATURE_NEW_CHICKEN_04" 0x012DEE90, "TraderToRescue" 0x012DEC58, "BodyGuard" 0x012CB3F0, "RingFighter" 0x012DEE84,
//   "FisticuffsMember" 0x012DEE70, "Tyler" 0x012DEE68, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, PMasterData +0x44, AllCreatures +0x48 (STLport vector<CScriptThing>, 12 bytes).
// Shape: one `||` condition over the inline CCharString::operator== (strcmp intrinsic on the null arm, Compare otherwise); the
//   erase arm leaves the iterator in place; followers are matched with a nested search and `break`.
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector();                                          // 0x008AC970 for CScriptThing
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __position);                // 0x00CD3152 for CScriptThing
    iterator erase(iterator __first, iterator __last);  // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};
}
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    std::vector<CScriptThing> AllCreatures;             // +0x48
    void UpdateLiveEnemies();
};

void CQ_TraderConflictGoodScript::UpdateLiveEnemies()
{
    AllCreatures.clear();
    Game->GetAllCreaturesExcludingHero(AllCreatures);
    std::vector<CScriptThing>::iterator it = AllCreatures.begin();
    while (it != AllCreatures.end()) {
        if (IsActiveThreadTerminating())
            return;
        if (it->GetDefName() == "CREATURE_NEW_CHICKEN_04") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "TraderToRescue") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "BodyGuard") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "RingFighter") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "FisticuffsMember") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "Tyler") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            ++it;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> followers;
    Game->GetFollowingEntityList(Game->GetHero(), followers);
    for (unsigned int i = 0; i < followers.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        for (std::vector<CScriptThing>::iterator it2 = AllCreatures.begin(); it2 != AllCreatures.end(); ++it2) {
            if (followers[i] == *it2) {
                AllCreatures.erase(it2);
                break;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript