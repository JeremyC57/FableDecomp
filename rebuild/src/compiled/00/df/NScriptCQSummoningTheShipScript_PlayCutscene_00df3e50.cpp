// NScript::CQ_SummoningTheShipScript::PlayCutscene(CCharString name) -- play a cut-scene with the hero: take the hero under scripted control, cast him as HERO, then run the named cut-scene inside a paused movie sequence with the movie camera fixed
// Leaf: ?PlayCutscene@CQ_SummoningTheShipScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF3E50, 541 bytes (+ int3 padding), thiscall `ret 4` (the by-value name is destroyed by the callee).
// Identity (proven): NScript::CQ_SummoningTheShipScript::PlayCutscene(CCharString); evidence: called from 0x00DF36E0, which resolve to Ego_r.pdb NScript::CQ_SummoningTheShipScript: 0x00DF36E0 is spawned (address taken at 0x00DF1655) by 0x00DF13A0 is Main (slot 2) of the quest-script vtable 0x012DE10C (Main registers STS_BriarRose, SummonerAttacker, SummonerMinion); `this` holds the script interface at +0x40 (CLevelScriptBase::Game); the Ego_r/FableWin.pdb name sets hold ?PlayCutscene@CQ_SummoningTheShipScript@NScript@@QAEXVCCharString@@@Z
// Label: the manifest's "DoMission_HeroUIInitialize" is propagated / a placeholder (family J035: six PlayCutscene bodies of quest scripts that cast only the hero).
// Callees: CBaseObject ctor / dtor / operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0 (??0CBaseObject@@IAE@XZ, ??1CBaseObject@@MAE@XZ,
//   ??4CBaseObject@@IAEAAV0@ABV0@@Z: the vtable 0x0129A7D8 those store has RTTI .?AVCBaseObject@@, the RTTI base of
//   CScriptGameResourceObjectBase), CCharString 0x0099EBF0 / 0x0099EAE0, CRT malloc 0x00BFEA0E / free 0x00BFEA14 (the cast map's
//   header node), operator delete 0x00BFE9BC, the cast map's operator[] 0x00CD3D2E and _Rb_tree::_M_erase 0x00CD344A,
//   CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   CGameScriptInterfaceBase slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera,
//   379 PauseAllNonScriptedEntities (ghidra_out/cgsi_slots_aligned.tsv). Literals "HERO" 0x01255174, "" 0x0122D70E.
// Model: the landed CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0 without the ROSE cast and the csargs map, re-typed to the PDB
//   (CBaseObject access, CCountedPointer<T> PImp members, the real STLport 4.6 map<CCharString, CScriptGameResourceObjectScriptedThingBase>
//   whose RunCutsceneMacro / operator[] / _Rb_tree spellings the PDB name sets hold).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EScriptAIPriority { };
class CCPPointerInfo {
public:
    long RefCount;                                      // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer() : Data(0), Info(0) { }
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        ReleaseData();
        Data = 0;
        Info = 0;
    }
};
class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing;
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                 // vtable 0x0129A7D8
protected:
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
public:
    CBaseObjectPointer* IntelligentPointer;             // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(__a), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                            // +0x04
    _Compare  _M_key_compare;                           // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static bool& _S_color(_Link_type __x) { return (bool&)(__x->_M_color); }
private:
    void _M_erase(_Link_type __x);                      // 0x00CD344A
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = false;      // _S_rb_tree_red: tells the header from the root
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _Base(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Alloc allocator_type;
private:
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
public:
    map() : _M_t(_Compare(), allocator_type()) { }
    _Tp& operator[](const key_type& __k);               // 0x00CD3D2E
};
}   // namespace std

void __fastcall RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,
                                 std::map<CCharString, bool>* flags, std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D

class CGameScriptInterfaceBase {                        // the interface CGameScriptInterface (vtable 0x01260F0C) implements
public:
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007();
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015(); virtual void g016();
    virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023(); virtual void g024();
    virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031(); virtual void g032();
    virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039(); virtual void g040();
    virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047(); virtual void g048();
    virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055(); virtual void g056();
    virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063(); virtual void g064();
    virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069();
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
    virtual void g071(); virtual void g072(); virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078();
    virtual void g079(); virtual void g080(); virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084(); virtual void g085(); virtual void g086();
    virtual void g087(); virtual void g088(); virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094();
    virtual void g095(); virtual void g096(); virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102();
    virtual void g103(); virtual void g104(); virtual void g105(); virtual void g106(); virtual void g107(); virtual void g108(); virtual void g109(); virtual void g110();
    virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114(); virtual void g115(); virtual void g116(); virtual void g117(); virtual void g118();
    virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124(); virtual void g125(); virtual void g126();
    virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132(); virtual void g133(); virtual void g134();
    virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140(); virtual void g141(); virtual void g142();
    virtual void g143(); virtual void g144(); virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148(); virtual void g149(); virtual void g150();
    virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154(); virtual void g155(); virtual void g156(); virtual void g157(); virtual void g158();
    virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164(); virtual void g165(); virtual void g166();
    virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172(); virtual void g173(); virtual void g174();
    virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180(); virtual void g181(); virtual void g182();
    virtual void g183(); virtual void g184(); virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188(); virtual void g189(); virtual void g190();
    virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195(); virtual void g196(); virtual void g197(); virtual void g198();
    virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204(); virtual void g205(); virtual void g206();
    virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212(); virtual void g213(); virtual void g214();
    virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220(); virtual void g221(); virtual void g222();
    virtual void g223(); virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228(); virtual void g229(); virtual void g230();
    virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235(); virtual void g236(); virtual void g237(); virtual void g238();
    virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244(); virtual void g245(); virtual void g246();
    virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252(); virtual void g253(); virtual void g254();
    virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260(); virtual void g261(); virtual void g262();
    virtual void g263(); virtual void g264(); virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268(); virtual void g269(); virtual void g270();
    virtual void g271(); virtual void g272(); virtual void g273(); virtual void g274(); virtual void g275(); virtual void g276(); virtual void g277(); virtual void g278();
    virtual void g279(); virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284(); virtual void g285(); virtual void g286();
    virtual void g287(); virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292(); virtual void g293(); virtual void g294();
    virtual void g295(); virtual void g296(); virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300(); virtual void g301(); virtual void g302();
    virtual void g303(); virtual void g304(); virtual void g305(); virtual void g306(); virtual void g307(); virtual void g308(); virtual void g309(); virtual void g310();
    virtual void g311(); virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316(); virtual void g317(); virtual void g318();
    virtual void g319(); virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324(); virtual void g325(); virtual void g326();
    virtual void g327(); virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332(); virtual void g333(); virtual void g334();
    virtual void g335(); virtual void g336(); virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340(); virtual void g341(); virtual void g342();
    virtual void g343(); virtual void g344(); virtual void g345(); virtual void g346(); virtual void g347(); virtual void g348(); virtual void g349(); virtual void g350();
    virtual void g351(); virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356(); virtual void g357(); virtual void g358();
    virtual void g359(); virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364(); virtual void g365(); virtual void g366();
    virtual void g367(); virtual void g368(); virtual void g369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void FixMovieSequenceCamera(bool fix) const;   // slot 371 (+0x5cc)
    virtual void g372(); virtual void g373(); virtual void g374(); virtual void g375(); virtual void g376(); virtual void g377(); virtual void g378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
};
class CScriptDataBase;
class CScriptBase {
public:
    virtual ~CScriptBase();
    unsigned char _base[0x40 - 4];                      // Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread, RuntimeScriptedEntityPersist
};
template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SummoningTheShipScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void PlayCutscene(CCharString name);
};
void CQ_SummoningTheShipScript::PlayCutscene(CCharString name)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterfaceBase* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
}
}