// NScript::CV_SickChildScript::PlayCutscene(CCharString name, CCharString arg) -- play one of the Sick Child quest's cut-scenes: take the hero under scripted control, pass `arg` as the cut-scene's $ARG1 unless it is "NULL", cast the quest's witch handle as WITCH, the mother as MUM and the hero as HERO, then run the named cut-scene macro with the movie camera fixed
// Leaf: ?PlayCutscene@CV_SickChildScript@NScript@@QAEXVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECE460, 908 bytes, thiscall `ret 8` (both by-value strings are destroyed by the callee).
// Identity: Ego_r.pdb / FableWin.pdb ?PlayCutscene@CV_SickChildScript@NScript@@QAEXVCCharString@@0@Z (two by-value
//   CCharString = `ret 8`); the cast entries read this+0x48 / this+0x58, Ego_r.pdb CV_SickChildScript::seh_Mother +0x5C /
//   seh_Witch +0x6C less the 0x14 retail script-base shrink, cast as "MUM" / "WITCH"; its four callers 0x00ECDBDB,
//   0x00ECE311, 0x00ECE9B4, 0x00ECF337 sit in the CV_SickChildScript functions around it. The manifest label
//   "Script_InitializeWitchCharacter" is wrong.
// Callees: CBaseObject ctor / dtor / operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString 0x0099EBF0 / 0x0099EAE0 /
//   operator= 0x0099EFB0, _stricmp 0x00BFEBA8 (the inline CCharString::operator!=), CRT malloc 0x00BFEA0E / free 0x00BFEA14
//   (map headers), operator delete 0x00BFE9BC, map<CCharString, CCharString>::operator[] 0x009AC700 and its _M_erase
//   0x009AA830, map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E and its _M_erase
//   0x00CD344A, RunCutsceneMacro 0x00CBFB7D (__fastcall); CGameScriptInterfaceBase slots 8 StartScriptingEntity,
//   70 GetHero, 371 FixMovieSequenceCamera (ghidra_out/cgsi_slots_aligned.tsv).
// Data: "NULL" 0x012393E4, "$ARG1" 0x012DD73C, "WITCH" 0x012F2610, "MUM" 0x012CC4F0, "HERO" 0x01255174, "" 0x0129AAF4.
// Model: the landed CQ_HeroSoulsNostroScript::PlayCutscene 0x00D89AD0 (real STLport 4.6 maps, CBaseObject access, PImp
//   members) plus the local cut-scene argument map std::map<CCharString, CCharString> that RunCutsceneMacro takes.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8 (MSVCR71 import)
class CCharStringData {
public:
    char* PData;                                        // +0x00 (CBasicString<char>::PData)
};
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    bool operator!=(const CCharString& other) const { return _stricmp(GetAsCharArray(), other.GetAsCharArray()) != 0; }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing;
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CScriptGameResourceObjectBase::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};

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
    _Tp& operator[](const key_type& __k);               // 0x00CD3D2E (cast map), 0x009AC700 (argument map)
};
}   // namespace std

void __fastcall RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,
                                 std::map<CCharString, bool>* flags, std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_SickChildScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    CScriptGameResourceObjectScriptedThingBase seh_Mother;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Witch;    // +0x58
    void PlayCutscene(CCharString name, CCharString arg);
};
void CV_SickChildScript::PlayCutscene(CCharString name, CCharString arg)
{
    std::map<CCharString, CCharString> csargs;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    if (arg != "NULL")
        csargs["$ARG1"] = arg;
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["WITCH"] = seh_Witch;
    cast["MUM"] = seh_Mother;
    cast["HERO"] = hero;
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
}
}