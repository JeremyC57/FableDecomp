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
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077();
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
protected:
    virtual bool IsEqualTo(const CScriptThing& other) const;   // slot 78 (+0x138)
public:
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};

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

class CGameScriptInterface {                             // vtable 0x01260F0C
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();                      // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing>& creatures) const;   // slot 84 (+0x150)
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164();
    virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188();
    virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196();
    virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212();
    virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228();
    virtual void v229(); virtual void v230();
    virtual bool GetFollowingEntityList(const CScriptThing& thing, std::vector<CScriptThing>& followers) const;   // slot 231 (+0x39c)
};
class CScriptBase {                                      // retail 0x40 bytes
public:
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                     // 0x00CB7940
};
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