// NScript::CQ_HeroSoulsMotherScript::OnPersist(CPersistContext& context) -- saves/loads PlayedMotherCS by name (default false)
// Leaf: ?OnPersist@CQ_HeroSoulsMotherScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D835A0, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012d45f0 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d835d0 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CQ_HeroSoulsMotherScript; PDB ?OnPersist@CQ_HeroSoulsMotherScript@NScript@@; refs=1; was _unattributed::PersistTransfer_PlayedMotherCS_00d835a0.
//   ?OnPersist@CQ_HeroSoulsMotherScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d835a0::PersistTransfer_PlayedMotherCS_00d835a0(CPersistContext* context) -- persists the "PlayedMotherCS" flag at +0x48 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D835A0, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00D83400 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsMotherScript_CQHeroSoulsMotherScript_00d83400.cpp); the PDB OnPersist declaration added; its bool PlayedMotherCS. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
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
namespace NScript {
class CQ_HeroSoulsMotherScript : public CQuestScriptObject {
public:   // vtable 0x012D45F0
    bool             PlayedMotherCS;               // +0x48
    CRetailMap<0x18> csargs;                       // +0x4c
    long             ScreamersAlive;               // +0x58
    float            BarToFill;                    // +0x5c
    long             BarID;                        // +0x60
    bool             QuestStartScreened;           // +0x64
    bool             GotSuperScreamer;             // +0x65
    CTimer           NewSuperScreamerTimer;        // +0x68
    CQ_HeroSoulsMotherScript(void* data, CGameScriptInterface* game);
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_HeroSoulsMotherScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
} // namespace NScript
using NScript::CQ_HeroSoulsMotherScript;
NScript::CQ_HeroSoulsMotherScript::CQ_HeroSoulsMotherScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}

void NScript::CQ_HeroSoulsMotherScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PlayedMotherCS", PlayedMotherCS, false);
}
