// NScript::CQ_TraderConflictEvilScript::UpdateLiveEnemies() -- rebuild the quest's AllCreatures list: every creature except the hero, minus the demon door face, the bandit followers and fighters, and anything following the hero
// Leaf: ?UpdateLiveEnemies@CQ_TraderConflictEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF9B80, 639 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb donor order ... CEntityScriptBinding<CQ_TraderConflictEvilScript>::CreateActiveEntityScript
//   0x00E386D0, UpdateLiveEnemies 0x00E388C0, Main 0x00E38C30 = retail CreateActiveEntityScript 0x00DF99F0 (landed), this
//   row, then PlayCutscene 0x00DF9E00 (landed; Ego_r inlines it); FableWin.pdb ?UpdateLiveEnemies@CQ_TraderConflictEvilScript@NScript@@QAEXXZ
//   (void, no arguments: plain `ret`). Called three times from the script's Main (0x00DF6494, 0x00DF6BEC, 0x00DF7296).
//   The catalog's "CScriptBase_FilterCombatantsByType" is a placeholder.
// Callees: vector<CScriptThing>::erase(first, last) 0x008AC9D0 (the inline clear()) and erase(position) 0x00CD3152,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing::GetDefName (slot 2, by value) / GetName (slot 1),
//   CBasicString<char>::Compare 0x00411570, ~CCharString 0x0099EAE0, the pimpl's CScriptThing::IsEqualTo (slot 78, inline
//   CScriptThing::operator==), free 0x00BFEA14 (the followers vector); through the quest interface (+0x40):
//   GetHero (slot 70), GetAllCreaturesExcludingHero (slot 84: the one-argument vector getter of the FableWin name set,
//   `ret 4` at 0x008A89D0), GetFollowingEntityList (slot 231).
// Strings: "CREATURE_DEMON_DOOR_FACE_01" 0x01259B50, "TC_BanditFollower" 0x012DE864, "TC_BanditFighter" 0x012DE850,
//   "" 0x0122D70E.
// Layout: AllCreatures +0x48 (Ego_r.pdb +0x5C; retail = Ego_r - 0x14 before the 12-byte STLport vector).
// Shape: the GetDefName temporary needs its own `if` (inside one || chain it becomes a conditional temporary with a frame
//   flag); the two GetName tests share the next arm and VC merges the three erase arms. CScriptThing::operator== goes
//   through CCountedPointer's operator-> / operator* (direct `.Data` access folds the +4 into the load displacement
//   instead of the `mov ebx,4` induction offset). Every statement re-checks IsActiveThreadTerminating, as the landed
//   WatchTimeLimit sibling.
#include <string.h>

extern "C" void __cdecl free(void* block);               // 0x00BFEA14

#pragma pack(push, 1)
template <class T>
class CBasicString {                                      // 13 bytes (pack 1)
public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
};
class CCharStringData {
public:
    CBasicString<char> Data;
    long               NoRefs;
};
#pragma pack(pop)
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    CCharStringData* PStringData;
    ~CCharString();                                       // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }

namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __position);                   // 0x00CD3152 (CScriptThing)
    iterator erase(iterator __first, iterator __last);     // 0x008AC9D0 (CScriptThing)
    void clear() { erase(begin(), end()); }
};
}   // namespace std

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_TraderConflictEvilScript : public CScriptBase {
public:
    CGameScriptInterface*      Game;                      // +0x40
    void*                      Data;                      // +0x44
    std::vector<CScriptThing>  AllCreatures;              // +0x48
    void UpdateLiveEnemies();
};
}   // namespace NScript

void NScript::CQ_TraderConflictEvilScript::UpdateLiveEnemies()
{
    AllCreatures.clear();
    Game->GetAllCreaturesExcludingHero(AllCreatures);
    std::vector<CScriptThing>::iterator it = AllCreatures.begin();
    while (it != AllCreatures.end()) {
        if (IsActiveThreadTerminating())
            return;
        if (it->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") {
            if (IsActiveThreadTerminating())
                return;
            AllCreatures.erase(it);
        }
        else if (it->GetName() == "TC_BanditFollower" || it->GetName() == "TC_BanditFighter") {
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
    std::vector<CScriptThing> followers;
    Game->GetFollowingEntityList(Game->GetHero(), followers);
    for (unsigned long i = 0; i < followers.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        for (it = AllCreatures.begin(); it != AllCreatures.end(); ++it) {
            if (followers[i] == *it) {
                AllCreatures.erase(it);
                break;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
}