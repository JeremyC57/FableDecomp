// CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, own vtable, the empty Briar Rose entity handle, the empty cut-scene argument map and the two imposter timers
// Leaf: ??0CQ_HeroSoulsBriarScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80C30, length 135 bytes (padded to 144), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (catalog label "CHeroMorphDef" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (the handle's base), malloc @ 0x00BFEA0E (the map's
// 0x18-byte header), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the global @ 0x0143E8F8 (x2).
// Vtable 0x012D41BC (slot 3 = Init 0x00D80CC0). Layout (retail; PDB offsets -0x14, the retail STLport map is 12 bytes):
// PlayedBriarCS +0x48, seh_BriarRose (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C) +0x4c,
// csargs map +0x5c, BriarRoseImposterTauntTimer +0x68, BriarRoseImposterHereTimer +0x6c, KickOffQuestStartScreen +0x70.
// Shape (landed 0x00D2F6B0 / 0x00E299F0 models): the argument words before the vtable store come from the inline
// `__declspec(novtable)` intermediate base; the handle's counted-pointer words come from an embedded {Data, Info}
// member object (one `xor ebx,ebx` shared with the map's zero stores); the timers' inline ctors register them.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
namespace NScript {
class CQ_HeroSoulsBriarScript : public CQuestScriptObject {
public:   // vtable 0x012D41BC
    bool             PlayedBriarCS;                // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;   // +0x4c
    CRetailMap<0x18> csargs;                       // +0x5c
    CTimer           BriarRoseImposterTauntTimer;  // +0x68
    CTimer           BriarRoseImposterHereTimer;   // +0x6c
    bool             KickOffQuestStartScreen;      // +0x70
    CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_HeroSoulsBriarScript;
NScript::CQ_HeroSoulsBriarScript::CQ_HeroSoulsBriarScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}