// NScript::CQ_WaspBossScript::WatchForCutscene() -- script thread: wait for the queen hornet to attack, then cast the hero (HERO) in CS_WASPBOSS_QUEEN inside a paused movie sequence, turn the hero towards the queen, reset the camera and mark the cut-scene finished
// Leaf: ?WatchForCutscene@CQ_WaspBossScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E12330, 586 bytes, thiscall, plain `ret`.
// Identity (proven): the landed CQ_WaspBossScript::Main 0x00E0EA40 spawns this body (function pointer at 0x00E0F17C)
//   under the thread name "WatchForCutscene"; Ego_r.pdb / FableWin.pdb ?WatchForCutscene@CQ_WaspBossScript@NScript@@QAEXXZ
//   (void, no arguments). The catalog's "CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene" is a
//   placeholder.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, CBaseObject ctor / operator= / dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430 (all protected, Ego_r.pdb),
//   malloc 0x00BFEA0E (the map header node), operator delete 0x00BFE9BC (CCPPointerInfo), map<CCharString,
//   CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E and ~map 0x00CDBFB0 (Ego_r.pdb spellings),
//   RunCutsceneMacro 0x00CBFB7D (__fastcall, Ego_r.pdb), CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer
//   0x006E7AB0 (FableWin.pdb), ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0, ~CScriptThing 0x004AA840; the quest
//   interface (+0x40) slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName,
//   370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 416 CameraDefault, 475 EntitySetFacingAngleTowardsThing,
//   512 EntitySetCutsceneBehaviour.
// Strings: "QueenHornet" 0x012E0198, "HERO" 0x01255174, "" 0x0122D70E, "CS_WASPBOSS_QUEEN" 0x012E05A0.
// Layout (retail = Ego_r.pdb - 0x14, as the landed WaspIntro 0x00E12F20): QueenHornetAttacks +0x4b (PDB +0x5f),
//   CutsceneFinished +0x4c (PDB +0x60).
// Shape: the landed WaspIntro sibling, with the cast map spelled as the real STLport 4.6 std::map (inline constructor:
//   malloc'd header node; out-of-line operator[] and destructor).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { };                             // Ego_r.pdb (enumerators not needed)
enum ECutsceneBehaviour { };                            // Ego_r.pdb (enumerators not needed)

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer();                                 // 0x006E7AB0 (CScriptGameResourceObjectMovieBase)
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
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Tp> class allocator {
public:
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    size_t   _M_node_count;                             // +0x04
    _Compare _M_key_compare;                            // +0x08
private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    map() : _M_t(_Compare(), _Alloc()) { }
    ~map();                                             // 0x00CDBFB0 (CCharString -> CScriptGameResourceObjectScriptedThingBase)
    _Tp& operator[](const key_type& __k);               // 0x00CD3D2E (CCharString -> CScriptGameResourceObjectScriptedThingBase)
    _Rep_type _M_t;
};
}   // namespace std

void __fastcall RunCutsceneMacro(const CCharString& name,
                                 std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,
                                 std::map<CCharString, bool>* flags, std::map<CCharString, CCharString>* args,
                                 bool a5, bool a6);     // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_WaspBossScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[3];
    bool                  QueenHornetAttacks;           // +0x4b (PDB +0x5f)
    bool                  CutsceneFinished;             // +0x4c (PDB +0x60)
    void WatchForCutscene();
};

void CQ_WaspBossScript::WatchForCutscene()
{
    while (!QueenHornetAttacks) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing queen = GSI->GetThingWithScriptName("QueenHornet");
    GSI->EntitySetCutsceneBehaviour(queen, (ECutsceneBehaviour)2);
    CScriptGameResourceObjectScriptedThingBase hero;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    CGameScriptInterface* game = GSI;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_WASPBOSS_QUEEN", cast, 0, 0, false, true);
    GSI->EntitySetFacingAngleTowardsThing(GSI->GetHero(), queen, true);
    GSI->CameraDefault();
    CutsceneFinished = true;
    game->PauseAllNonScriptedEntities(false);
}
}   // namespace NScript