// CQ_HeroSoulsMotherScript::CQ_HeroSoulsMotherScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable, the empty cut-scene argument map and the super-screamer timer
// Leaf: ??0CQ_HeroSoulsMotherScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D83400, length 103 bytes (padded to 112), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated), malloc @ 0x00BFEA0E
// (the map's 0x18-byte header node), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8.
// Vtable 0x012D45F0 (slot 3 = Init 0x00D83470). Layout (retail; PDB offsets -0x14, the retail STLport map is 12 bytes):
// PlayedMotherCS +0x48, csargs map<CCharString,CCharString> +0x4c, ScreamersAlive +0x58, BarToFill +0x5c, BarID +0x60,
// QuestStartScreened +0x64, GotSuperScreamer +0x65, NewSuperScreamerTimer +0x68.
// Shape (landed 0x00E299F0 / 0x00D7C700 models): the two arguments are stored BEFORE the vtable store by the inline
// `__declspec(novtable)` intermediate base; the map is STLport's _Rb_tree (header nulled, allocated, count zeroed, then
// _M_empty_initialize re-reading the header); the timer member's inline ctor registers it. The plain members have no
// initialiser (Init sets them).
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
};
} // namespace NScript
using NScript::CQ_HeroSoulsMotherScript;
NScript::CQ_HeroSoulsMotherScript::CQ_HeroSoulsMotherScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}