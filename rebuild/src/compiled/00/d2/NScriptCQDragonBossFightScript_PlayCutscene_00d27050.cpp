// NScript::CQ_DragonBossFightScript::PlayCutscene(CCharString name) -- play a dragon-fight cut-scene with the hero: take the hero under scripted control, cast him as HERO, then run the named cut-scene (with the script's cs_flags) inside a paused movie sequence with the movie camera fixed
// Leaf: ?PlayCutscene@CQ_DragonBossFightScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D27050, 489 bytes, thiscall `ret 4` (the by-value name is destroyed by the callee).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?PlayCutscene@CQ_DragonBossFightScript@NScript@@QAEXVCCharString@@@Z; all five
//   callers (0x00D26348, 0x00D2662A, 0x00D2666B, 0x00D266ED, 0x00D26887) lie in CQ_DragonBossFightScript::DoMission 0x00D26190
//   (the next function, JackTaunts, starts at 0x00D26A50); the flags map passed to RunCutsceneMacro is this+0x84 = Ego_r
//   CQ_DragonBossFightScript::cs_flags (0xA0 - 0x14 for CScriptBase - 2 x 4 for the two Dinkumware vectors before it).
//   Catalog "DoMission_Setup" is wrong.
// Callees: CBaseObject ctor / dtor / operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCountedPointer<
//   CScriptGameResourceObjectScriptedThingBase>::operator= 0x008AB1E0 (Ego_r.pdb ??4?$CCountedPointer@VCScriptGameResourceObject
//   ScriptedThingBase@@@@QAEAAV0@ABV0@@Z; out of line here), CCharString 0x0099EBF0 / 0x0099EAE0, CRT malloc 0x00BFEA0E / free
//   0x00BFEA14 (the cast map's header node), operator delete 0x00BFE9BC, the cast map's operator[] 0x00CD3D2E and
//   _Rb_tree::_M_erase 0x00CD344A, CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer 0x006E7AB0,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall); CGameScriptInterfaceBase slots 8 StartScriptingEntity, 70 GetHero,
//   370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities (ghidra_out/cgsi_slots_aligned.tsv).
//   Literals "HERO" 0x01255174, "" 0x0122D70E (paired with retail by relocation, rel.py).
// Model: the landed CQ_HeroSoulsScript::PlayCutscene 0x00D7AA30 (family J035), with &cs_flags as RunCutsceneMacro's flags map and
//   the hero's PImp copy in `cast["HERO"] = hero` going through the out-of-line CCountedPointer operator= (declared as an explicit
//   specialization, as the model does for the movie PImp dtor); with the inline operator= the body grows to 547 bytes.
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40; the master-data argument is the one Ego_r.pdb shows for
//   CLevelScriptBase, probable for this script), then the Ego_r CQ_DragonBossFightScript members at Ego_r - 0x1C (DragonState
//   +0x48 .. cs_flags +0x84); the two vectors are opaque (unused here).
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
#include "engine/CBaseClass.h"                         // ~CBaseClass 0x0099A2E0
class CScriptThing;
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);   // 0x008AB1E0 (out of line in retail)

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

// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDataBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_DragonBossFightScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EDragonState { };
    EDragonState  DragonState;                          // +0x48
    unsigned char MinionSpawnPoints[0x0c];              // +0x4c std::vector<CScriptThing>
    unsigned char SummonerSpawnPoints[0x0c];            // +0x58 std::vector<CScriptThing>
    long          NextSummonerSpawnPoint;               // +0x64
    long          TargetNumMinions;                     // +0x68
    long          TargetNumSummoners;                   // +0x6c
    long          NumMinions;                           // +0x70
    long          NumSummoners;                         // +0x74
    long          NumFlyBysBetweenSummonerSpawns;       // +0x78
    long          NumFlyBysSinceLastSummonerSpawn;      // +0x7c
    bool          MinionSpawningEnabled;                // +0x80
    bool          SummonerSpawningEnabled;              // +0x81
    std::map<CCharString, bool> cs_flags;               // +0x84
    void PlayCutscene(CCharString name);
};
void CQ_DragonBossFightScript::PlayCutscene(CCharString name)
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
    RunCutsceneMacro(name, cast, &cs_flags, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
}
}