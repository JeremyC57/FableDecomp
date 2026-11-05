// NScript::CV_MazeResearchScript::OnPersist(CPersistContext& context) -- saves/loads SwordTaken and BookRead by name (default false)
// Leaf: ?OnPersist@CV_MazeResearchScript@NScript@@UAEXAAVCPersistContext@@@Z
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA80A0, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012ee444 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ea7800 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CV_MazeResearchScript; PDB ?OnPersist@CV_MazeResearchScript@NScript@@; refs=1; was _unattributed::PersistTransfer2_00ea80a0.
//   ?OnPersist@CV_MazeResearchScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ea80a0::PersistTransfer2_00ea80a0(CPersistContext* context) -- persists the "SwordTaken" and "BookRead" byte flags at +0x48/+0x49 Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA80A0, oracle length 67 b... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00EA76E0 (rebuild/src/compiled/00/ea/NScriptCVMazeResearchScript_CVMazeResearchScript_00ea76e0.cpp); the PDB OnPersist declaration added; its bool SwordTaken, bool BookRead. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
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
#include "engine/CCountedPointer.h"
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
struct CCountedPointer_CScriptThing {              // CScriptThing::PImp
    CScriptThingImp* Data;                         // +0
    CCPPointerInfo*  Info;                         // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript {
class CV_MazeResearchScript : public CQuestScriptObject {
public:   // vtable 0x012EE444
    bool SwordTaken;                               // +0x48
    bool BookRead;                                 // +0x49
    CScriptThing Sword;                            // +0x4c
    CRetailMap<0x18> cs_flags;                     // +0x58
    virtual void v00();
    CV_MazeResearchScript(void* data, CGameScriptInterface* game);
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CV_MazeResearchScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
} // namespace NScript
using NScript::CV_MazeResearchScript;
NScript::CV_MazeResearchScript::CV_MazeResearchScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}

void NScript::CV_MazeResearchScript::OnPersist(CPersistContext& context)
{
    context.Transfer("SwordTaken", SwordTaken, false);
    context.Transfer("BookRead", BookRead, false);
}
