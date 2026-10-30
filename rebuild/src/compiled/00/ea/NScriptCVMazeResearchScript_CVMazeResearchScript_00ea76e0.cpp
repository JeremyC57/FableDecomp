// CV_MazeResearchScript::CV_MazeResearchScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012EE444, the empty Sword script thing and the empty cut-scene flag map
// Leaf: ??0CV_MazeResearchScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA76E0, length 101 bytes (padded to 112), `ret 8`.
// Catalog label "CV_MurderTwistFinalScript" is wrong: the factory 0x00EA8690 (this ctor) is registered at 0x00CD8F62
// under "S_VMR" / "V_MazeResearch", and the PDB CV_MazeResearchScript layout fits the stores.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseClass::CBaseClass @ 0x0099A2D0 (the CScriptThing root; CScriptThing
// vtable 0x01238C8C), malloc @ 0x00BFEA0E (the map's 0x18-byte header).
// Layout (retail = Ego_r.pdb - 0x14): SwordTaken +0x48, BookRead +0x49, Sword +0x4c, cs_flags +0x58; size 0x64.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
class CTimer { public:                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
template <class T> class CCountedPointer { public:                     // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CCPPointerInfo;
struct CCountedPointer_CScriptThing {              // CScriptThing::PImp
    CScriptThingImp* Data;                         // +0
    CCPPointerInfo*  Info;                         // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    CCountedPointer_CScriptThing PImp;             // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};
namespace NScript {
class CV_MazeResearchScript : public CQuestScriptObject {
public:   // vtable 0x012EE444
    bool SwordTaken;                               // +0x48
    bool BookRead;                                 // +0x49
    CScriptThing Sword;                            // +0x4c
    CRetailMap<0x18> cs_flags;                     // +0x58
    virtual void v00();
    CV_MazeResearchScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_MazeResearchScript;
NScript::CV_MazeResearchScript::CV_MazeResearchScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}