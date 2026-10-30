// CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable, six zeroed counters, the empty map and two timers
// Leaf: ??0CQ_NewOakValeIntroScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D25410, length 153 bytes (padded to 160), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated), malloc @ 0x00BFEA0E
// (the map's 0x18-byte header), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8 (x2).
// Vtable 0x012CB70C (slot 3 = Init 0x00D254B0). The retail class does not follow the Ego_r PDB layout of
// CQ_NewOakValeIntroScript past +0x4c (it has a 12-byte map at +0x84 and timers at +0x90/+0x94), so the members are
// named descriptively: six zero-initialised words +0x4c..+0x60, the map +0x84, two timers +0x90/+0x94.
// Shape (landed 0x00E299F0 model): argument words before the vtable store from the inline `__declspec(novtable)`
// intermediate base; the six zero words come from the inline ctor of an embedded member object (one `xor ebx,ebx`
// after the vtable store, shared with the map; plain init-list zeros hoist it above the store); the map is STLport's
// _Rb_tree; the timers' inline ctors register them.
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
struct CZeroWords {                                // six counters zeroed by their own inline ctor (an init-list zero on the
    long Word[6];                                  // script itself hoists the zero register above the vtable store)
    CZeroWords() { Word[0] = 0; Word[1] = 0; Word[2] = 0; Word[3] = 0; Word[4] = 0; Word[5] = 0; }
};
struct CQ_NewOakValeIntroScript : CQuestScriptObject {   // vtable 0x012CB70C
    unsigned char    _pad48[4];                    // +0x48 (VictimShake)
    CZeroWords       m_counters;                   // +0x4c..+0x63
    unsigned char    _pad64[0x84 - 0x64];          // +0x64
    CRetailMap<0x18> m_map;                        // +0x84
    CTimer           m_timer90;                    // +0x90
    CTimer           m_timer94;                    // +0x94
    CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game);
};
CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}