// CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game) -- cut-scene script ctor: script base, interface/data pair, vtable 0x012F53C0, the empty FirePoint/Fires vectors and OakValeFlag map, and the seven v1..v7 scripted-thing handles
// Leaf: ??0CCS_OakValeRevisitedScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE8100, length 266 bytes (`ret 8`); the only caller is its factory
// thunk at 0x00EE8BFB.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, malloc @ 0x00BFEA0E (the map's 0x18-byte header),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x7).
// Layout (retail = PDB -0x14 for the base, 12-byte STLport containers): FirePoint +0x48, Fires +0x54
// (vector<CScriptThing>), OakValeFlag +0x60 (map<CCharString,bool>), v1..v7 +0x6c..+0xcc (vtable 0x0127094C,
// 0x10 bytes each); size 0xdc.
// Shape: the landed CV_ChickenKickingScript (0x00E62830) / CQ_HeroSoulsBriarScript (0x00D80C30) model; the two
// vectors are STLport `_Vector_base` + `_STLP_alloc_proxy` (six zero words from the one ebx), the map header is
// zeroed, malloc'd and self-linked by `_M_empty_initialize`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
struct _Rb_tree_node_base {
    char                _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template <unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map/_Rb_tree (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
class CScriptThing;
struct CMapNodeCharStringBool;
struct CCS_OakValeRevisitedScript : CQuestScriptObject {   // vtable 0x012F53C0
    CRetailVector<CScriptThing*> FirePoint;        // +0x48 (vector<CScriptThing>, 12 bytes)
    CRetailVector<CScriptThing*> Fires;            // +0x54
    CRetailMap<0x18> OakValeFlag;                  // +0x60 map<CCharString,bool>
    CScriptGameResourceObjectScriptedThingBase v1; // +0x6c
    CScriptGameResourceObjectScriptedThingBase v2; // +0x7c
    CScriptGameResourceObjectScriptedThingBase v3; // +0x8c
    CScriptGameResourceObjectScriptedThingBase v4; // +0x9c
    CScriptGameResourceObjectScriptedThingBase v5; // +0xac
    CScriptGameResourceObjectScriptedThingBase v6; // +0xbc
    CScriptGameResourceObjectScriptedThingBase v7; // +0xcc
    virtual void v00();
    CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game);
};
CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}