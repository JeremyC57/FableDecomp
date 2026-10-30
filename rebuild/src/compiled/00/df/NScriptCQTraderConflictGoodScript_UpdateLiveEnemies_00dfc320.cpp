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
class CScriptThing : public CBaseClass { public:        // vtable 0x01238C8C, 12 bytes
    virtual const CCharString& GetName() const;         // slot 1 (+0x04)
    virtual CCharString GetDefName() const;             // slot 2 (+0x08)
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c)
    virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013();
    virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033();
    virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043();
    virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053();
    virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073();
    virtual void t074(); virtual void t075(); virtual void t076(); virtual void t077();
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
protected:
    virtual bool IsEqualTo(const CScriptThing& other) const;   // slot 78 (+0x138)
};
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
class CGameScriptInterfaceBase { public:          // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007(); virtual void g008(); virtual void g009();
    virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015(); virtual void g016(); virtual void g017(); virtual void g018(); virtual void g019();
    virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023(); virtual void g024(); virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029();
    virtual void g030(); virtual void g031(); virtual void g032(); virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039();
    virtual void g040(); virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047(); virtual void g048(); virtual void g049();
    virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055(); virtual void g056(); virtual void g057(); virtual void g058(); virtual void g059();
    virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063(); virtual void g064(); virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void g071(); virtual void g072(); virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078(); virtual void g079(); virtual void g080();
    virtual void g081(); virtual void g082(); virtual void g083();
    virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing>& things) const;   // slot 84 (+0x150)
    virtual void g085(); virtual void g086(); virtual void g087(); virtual void g088(); virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094();
    virtual void g095(); virtual void g096(); virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102(); virtual void g103(); virtual void g104();
    virtual void g105(); virtual void g106(); virtual void g107(); virtual void g108(); virtual void g109(); virtual void g110(); virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114();
    virtual void g115(); virtual void g116(); virtual void g117(); virtual void g118(); virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124();
    virtual void g125(); virtual void g126(); virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132(); virtual void g133(); virtual void g134();
    virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140(); virtual void g141(); virtual void g142(); virtual void g143(); virtual void g144();
    virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148(); virtual void g149(); virtual void g150(); virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154();
    virtual void g155(); virtual void g156(); virtual void g157(); virtual void g158(); virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164();
    virtual void g165(); virtual void g166(); virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172(); virtual void g173(); virtual void g174();
    virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180(); virtual void g181(); virtual void g182(); virtual void g183(); virtual void g184();
    virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188(); virtual void g189(); virtual void g190(); virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194();
    virtual void g195(); virtual void g196(); virtual void g197(); virtual void g198(); virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204();
    virtual void g205(); virtual void g206(); virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212(); virtual void g213(); virtual void g214();
    virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220(); virtual void g221(); virtual void g222(); virtual void g223(); virtual void g224();
    virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228(); virtual void g229(); virtual void g230();
    virtual bool GetFollowingEntityList(const CScriptThing& leader, std::vector<CScriptThing>& followers) const;   // slot 231 (+0x39c)
};
class CGameScriptInterface : public CGameScriptInterfaceBase {
public:
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
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