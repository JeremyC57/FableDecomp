// CV_AmbushScamScript::ResetRandomSpeechTime() -- restart the random-speech timer (+0x84) with the def's maximum minus a random amount below the def's range
// Leaf: ?ResetRandomSpeechTime@CV_AmbushScamScript@@
// Retail: Fable.exe @ 0x00E1FEB0, 60 bytes (+ int3 padding). Called from eleven places in its script (0x00E1FD9C..).
// Callees: rand @ 0x00BFEB16, CGameScriptInterface::SetTimer (slot 89, +0x164) through the interface global @ 0x0143E8F8.
// Data: the script def pointer @ 0x0143E90C (CDefPointer<CScriptDef>): +0x9c8 the timer maximum, +0x9cc the random range
// (descriptive names; the retail CScriptDef offsets do not line up with Ego_r.pdb here).
// Shape: the def pointer is read into a local BEFORE rand() (kept in edi for the `idiv`), while the maximum is read
// through the global again after the call, and the difference is a named local (that keeps the `mov eax,[g] ;
// mov eax,[eax+9c8h]` reload in eax; computing it inside the call argument hoists the interface fetch into ebx).
extern "C" int __cdecl rand();                           // 0x00BFEB16

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0x9c8];
    long RandomSpeechTimeMax;                            // +0x9c8
    long RandomSpeechTimeRange;                          // +0x9cc
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"

struct CV_AmbushScamScript {
    unsigned char _base[0x84];
    CTimer RandomSpeechTimer;                            // +0x84
    void ResetRandomSpeechTime();
};

void CV_AmbushScamScript::ResetRandomSpeechTime()
{
    CScriptDef* def = pGlobalScriptDef;
    long time = pGlobalScriptDef->RandomSpeechTimeMax - rand() % def->RandomSpeechTimeRange;
    g_pGameScriptInterface->SetTimer(RandomSpeechTimer.TimerIndex, time);
}