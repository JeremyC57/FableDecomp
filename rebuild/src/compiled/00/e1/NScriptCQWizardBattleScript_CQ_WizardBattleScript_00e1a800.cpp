// NScript::CQ_WizardBattleScript::CQ_WizardBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script ctor: script base, interface/data pair, own vtable and three empty CScriptThing members (Maze, TopOfLighthouse, MiddleOfLighthouse)
// Leaf: ??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1A800, length 106 bytes (padded to 112), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated), CBase::CBase @ 0x0099A2D0
// (each CScriptThing's base, x3).
// Vtable 0x012E0F54 (slot 3 = Init 0x00E1A870, landed). Layout (retail; PDB offsets -0x14): the flags +0x48..+0x4f,
// MazePhase +0x50, NumberOfScreamersAlive +0x54, MazeConversation +0x58, Maze +0x5c, TopOfLighthouse +0x68,
// MiddleOfLighthouse +0x74 (CScriptThing, vtable 0x01238C8C, 12 bytes).
// Shape (landed 0x00D2F6B0 model): argument words before the vtable store from the inline `__declspec(novtable)`
// intermediate base; each CScriptThing's base ctor runs on `lea edi,[esi+N]`, its vtable is stored and its counted-pointer
// words are zeroed from one `xor ebx,ebx` by the inline ctor of an embedded {Data, Info} member object.
// Naming wave 35: identity level-script ctor: called by the Alloc 00e1d9f0 on the new block, stores script vtable 012e0f54; PDB ??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_WizardBattleScript proven by its Main 00e1a990 entity registrations; was CQ_WizardBattleScript::CQ_WizardBattleScript (namespace spelling) (wave3/labels_alloc.tsv); PDB ??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
//   Was `??0CQ_WizardBattleScript@@QAE@PAXPAVCGameScriptInterface@@@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
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
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
namespace NScript {
class CQ_WizardBattleScript : public CQuestScriptObject {
public:   // vtable 0x012E0F54
    bool         VillagersTurnOnHero;              // +0x48
    bool         VillagersChased;                  // +0x49
    bool         FightersStart;                    // +0x4a
    bool         MissionSucceeded;                 // +0x4b
    bool         MissionFailed;                    // +0x4c
    bool         MissionOver;                      // +0x4d
    bool         QuestStartScreenShown;            // +0x4e
    bool         MazeDefeated;                     // +0x4f
    long         MazePhase;                        // +0x50
    long         NumberOfScreamersAlive;           // +0x54
    long         MazeConversation;                 // +0x58
    CScriptThing Maze;                             // +0x5c
    CScriptThing TopOfLighthouse;                  // +0x68
    CScriptThing MiddleOfLighthouse;               // +0x74
    CQ_WizardBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_WizardBattleScript();    // ??1CQ_WizardBattleScript@NScript@@UAE@XZ
};
}
using NScript::CQ_WizardBattleScript;
NScript::CQ_WizardBattleScript::CQ_WizardBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CQuestScriptObject((void*)data, (CGameScriptInterface*)&game)
{
}