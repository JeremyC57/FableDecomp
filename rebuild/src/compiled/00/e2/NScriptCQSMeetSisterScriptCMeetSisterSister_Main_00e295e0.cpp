// NScript::CQS_MeetSisterScript::CMeetSisterSister::Main() -- Arena exit, Theresa (the sister): take her (Me) under scripted control (priority 4; frame-waits until it succeeds), then the hero; cast them as "Hero" and "Sister", play CS_ARENA_LEAVE_THERESA_02 inside a paused movie sequence, remove her from the world, complete and later deactivate the active quest and fade the screen in
// Leaf: ?Main@CMeetSisterSister@CQS_MeetSisterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E295E0, 940 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): slot 1 of the entity-script vtable 0x012E224C, the vtable the landed
//   NScript::CQS_MeetSisterScript::CMeetSisterSister::Alloc 0x00E29040 stores into the 0x1C-byte object it news (the
//   "MeetSisterSister" binding of CQS_MeetSisterScript::Main); Ego_r.pdb names it
//   ?Main@CMeetSisterSister@CQS_MeetSisterScript@NScript@@EAEXXZ (private virtual; donor size 0x3B0 vs retail 0x3AC, the
//   donor's next symbol is CQS_MeetSisterScript::Alloc as retail's 0x00E299F0 neighbourhood). Its sibling
//   CMeetSisterMessenger::Main 0x00E292C0 (slot 1 of vtable 0x012E2230) plays CS_ARENA_LEAVE_THERESA_01.
// Label: the manifest's "CScriptGameResourceObjectScriptedThingBase::HandleSisterArenaDialogue" is descriptive/invented: `this`
//   is the entity script (Game +4, Me +8), the ScriptedThingBase objects are its locals.
// Callees: CBaseObject ctor / dtor / operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CScriptGameResourceObjectScriptedThingBase
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / implicit dtor 0x007E74D0 (out-of-line copy at all exits but the first),
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, malloc 0x00BFEA0E (the cast
//   map's header node), operator delete 0x00BFE9BC, map operator[] 0x00CD3D2E, map dtor 0x00CDBFB0,
//   CCountedPointer<CScriptGameResourceObjectMovieBase> dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall).
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 108 RemoveThing,
//   281 DeactivateQuestLater, 288 SetQuestAsCompleted, 370 StartMovieSequence, 374 FadeScreenIn,
//   379 PauseAllNonScriptedEntities, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "Hero" 0x012C4570, "Sister" 0x012CC9E8, "" 0x0122D70E, "CS_ARENA_LEAVE_THERESA_02" 0x012E22AC.
// Model: the landed NScript::CQ_HeroSoulsArenaScript::PlayCutscene 0x00D7FC70 (CBaseObject access, CCountedPointer<T> PImp
//   members, the real STLport 4.6 map<CCharString, CScriptGameResourceObjectScriptedThingBase>).
// Shape: the hero object, the cast and the movie live in an inner block: retail destroys the movie (inline), the cast (map dtor
//   call) and the hero (0x007E74D0) before RemoveThing, the sister object only at the function end.
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
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
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
class CScriptThingImp;
class CScriptThing : public CBaseClass {               // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // 0x004AA840
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
};
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
    {                                                   // implicit copy-assignment, expanded (0x008ABD10 is its COMDAT)
        CScriptGameResourceObjectBase::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
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

// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"

class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    virtual void Main();                                // slot 1
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_MeetSisterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMeetSisterSister : public CEntityScriptBase {    // 0x1C bytes, vtable 0x012E224C
    public:
        CQS_MeetSisterScript*   ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    private:
        virtual void Main();                            // slot 1
    };
};

void CQS_MeetSisterScript::CMeetSisterSister::Main()
{
    CScriptGameResourceObjectScriptedThingBase sister;
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        if (sister.IsValid())
            sister.Release();
        while (!Game->StartScriptingEntity(Me, sister, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["Hero"] = hero;
        cast["Sister"] = sister;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_ARENA_LEAVE_THERESA_02", cast, 0, 0, false, true);
        game->PauseAllNonScriptedEntities(false);
    }
    Game->RemoveThing(Me, false, true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->FadeScreenIn();
}
}   // namespace NScript