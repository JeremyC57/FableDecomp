// NScript::CQ_AwakeningTheOracleScript::PlayCutscene(CCharString name, bool inMovieSequence) -- play an Awakening-the-Oracle cut-scene: take the hero under scripted control and cast him as HERO in cs_chars, fix the movie camera, run the named cut-scene with cs_args (inside a paused movie sequence when inMovieSequence), then release the camera and clear cs_args and cs_chars
// Leaf: ?PlayCutscene@CQ_AwakeningTheOracleScript@NScript@@QAEXVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFF9A0, 443 bytes, thiscall `ret 8` (the by-value name is destroyed by the callee), no EH frame.
// Identity (proven): Ego_r.pdb (egor_pdb_names 0x00D00670) and FableWin.pdb spell
//   ?PlayCutscene@CQ_AwakeningTheOracleScript@NScript@@QAEXVCCharString@@_N@Z, which reloc_bindings already binds to this address;
//   the six callers (0x00CFB7B1 .. 0x00D004AB) lie in the quest's Main and its entity scripts; the two maps this body passes and
//   clears are this+0x80 / +0x8C = Ego_r cs_args 152 / cs_chars 168 less 0x14 (CScriptBase) and 4 (the Dinkumware vector
//   DiggingPoints, 16 -> 12 bytes in STLport). Catalog "Main_InitializeHeroMorphComponents" is wrong.
// Callees: CBaseObject ctor / dtor / operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCountedPointer<
//   CScriptGameResourceObjectScriptedThingBase>::operator= 0x008AB1E0 (out of line), CCharString 0x0099EBF0 / 0x0099EAE0,
//   operator delete 0x00BFE9BC, cs_chars operator[] 0x00CD3D2E and clear 0x00CD3699, cs_args clear 0x009AACE0,
//   CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall,
//   PDB spelling); CGameScriptInterfaceBase slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence,
//   371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities. Literals "HERO" 0x01255174, "" 0x0122D70E.
// Model: the landed CQ_DragonBossFightScript::PlayCutscene 0x00D27050 TU (family J035; STLport 4.6 map, out-of-line PImp
//   operator=) with the CQS_ScytheInfoScript::PlayCutscene 0x00E2A900 body (member cast map, optional movie sequence, both maps
//   cleared); map::clear is out of line for both map types, as the landed 0x00D7C470 declares it for cs_args. The PImp
//   pointers come from the shared engine/CCountedPointer.h (the DragonBossFight TU still carries a local copy).
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40; master-data argument as the family models it), then the Ego_r
//   members at Ego_r - 0x14 up to DiggingPoints (opaque 12-byte vector), cs_args +0x80, cs_chars +0x8C.
// Shape: the movie-sequence name is the implicit "" temporary (homed in the dead bool argument slot); Pause/unpause go through
//   one named copy of Game.
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
#include "engine/CCountedPointer.h"
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
    _Tp& operator[](const key_type& __k);               // 0x00CD3D2E for <CCharString, CScriptGameResourceObjectScriptedThingBase>
    void clear();                                       // 0x009AACE0 <CCharString, CCharString>, 0x00CD3699 <CCharString, CScriptGameResourceObjectScriptedThingBase>
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
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool          MissionSucceeded;                     // +0x48
    bool          PlayedOpeningCS;                      // +0x49
    bool          PlayedLostBayCS;                      // +0x4a
    bool          PlayedNecroCS;                        // +0x4b
    long          TabletsCollected;                     // +0x4c
    bool          SpawnedFinalSpotsStuff;               // +0x50
    long          PositionTaken;                        // +0x54
    bool          GivenDiggingSpotsRewards;             // +0x58
    long          GraveNumbersGivenOut;                 // +0x5c
    long          FinalSummonersAlive;                  // +0x60
    bool          DisplayedBadTabletMessage;            // +0x64
    bool          PlayedOpeningNecroCS;                 // +0x65
    long          TotalSpawnedCreaturesAlive;           // +0x68
    bool          AutosavedInNW3;                       // +0x6c
    bool          GotY;                                 // +0x6d
    bool          GotM;                                 // +0x6e
    bool          GotC;                                 // +0x6f
    bool          GotA;                                 // +0x70
    unsigned char DiggingPoints[0x0c];                  // +0x74 std::vector<CScriptThing>
    std::map<CCharString, CCharString> cs_args;         // +0x80
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cs_chars;   // +0x8c
    void PlayCutscene(CCharString name, bool inMovieSequence);
};
void CQ_AwakeningTheOracleScript::PlayCutscene(CCharString name, bool inMovieSequence)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    cs_chars["HERO"] = hero;
    Game->FixMovieSequenceCamera(true);
    if (inMovieSequence) {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro(name, cs_chars, 0, &cs_args, false, true);
        game->PauseAllNonScriptedEntities(false);
    } else
        RunCutsceneMacro(name, cs_chars, 0, &cs_args, false, true);
    Game->FixMovieSequenceCamera(false);
    cs_args.clear();
    cs_chars.clear();
}
}   // namespace NScript