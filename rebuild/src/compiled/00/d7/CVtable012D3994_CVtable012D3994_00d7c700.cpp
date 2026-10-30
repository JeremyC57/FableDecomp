// CVtable_012D3994::CVtable_012D3994(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x58, three zeroed words at +0x5c..+0x64
// Leaf: ??0CVtable_012D3994
// Retail: Fable.exe @ 0x00D7C700, 62 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Label note: the catalog/manifest name "CV_AmbushScamScript::CV_AmbushScamScript" is propagated over several
// quest-script constructors; this one stores vtable 0x012D3994 (no RTTI locator: quest-script module), so it is
// named CVtable_012D3994 like the landed family (0x00E68F70).
// Shape (landed 0x00E68F70 model): the two arguments are stored BEFORE the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); the timer member's inline
// constructor registers the timer (CTimer() : Index(RegisterTimer())); the three zero words are init-list members.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CVtable_012D3994 : CScriptObject {            // vtable 0x012D3994
    unsigned char _pad48[0x58 - 0x48];
    CTimer m_timer;                                  // +0x58
    long m_value5c;                                  // +0x5c
    long m_value60;                                  // +0x60
    long m_value64;                                  // +0x64
    virtual void v00();
    CVtable_012D3994(void* owner, CGameScriptInterface* game);
};

CVtable_012D3994::CVtable_012D3994(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game), m_value5c(0), m_value60(0), m_value64(0)
{
}