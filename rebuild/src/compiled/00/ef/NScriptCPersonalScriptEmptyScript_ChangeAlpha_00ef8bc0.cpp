// NScript::CPersonalScript_EmptyScript::ChangeAlpha(CScriptThing thing) -- script loop that keeps pulsing a thing's alpha between 0 and 1: the step is -0.5 per second at the constant frame rate, reversed whenever the alpha leaves [0, 1], applied every script frame until the thread is terminating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF8BC0, 254 bytes, `ret 0Ch` (the 12-byte CScriptThing by value). Called from 0x00EF8A69.
//   PDB: ?ChangeAlpha@CPersonalScript_EmptyScript@NScript@@QAEXVCScriptThing@@@Z.
// Callees: CGameScriptInterface slots 654 (+0xA38) GetConstantFPS, 510 (+0x7F8) EntitySetAlpha(const CScriptThing&, float,
//   bool) and 7 (+0x1C) NewScriptFrame (the interface pointer at +0x40; ghidra_out/cgsi_vtable_slots.tsv lists these 4 rows
//   high, at 658 and 514), CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC.
// Constants: 0x01238798 = -0.5f, 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Shape: the by-value CScriptThing parameter is destroyed by the callee (inline ~CScriptThing: vtable re-store, counted
//   release, zeroes, ~CBaseClass); `while (!terminating)` rotated with the entry test first; the step reversal is shared
//   by both clamp arms; `alpha += step` keeps the sum on the x87 stack for the `< 0.0f` test.
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:
    unsigned char         _pad00[0x40];
    CGameScriptInterface* GSI;                     // +0x40 (the quest script's interface pointer)
    bool IsActiveThreadTerminating();              // 0x00CB7940
};

namespace NScript {
class CPersonalScript_EmptyScript : public CScriptBase { public:
    void ChangeAlpha(CScriptThing thing);
};

void CPersonalScript_EmptyScript::ChangeAlpha(CScriptThing thing)
{
    float alpha = 1.0f;
    float step = -0.5f / (float)GSI->GetConstantFPS();
    while (!IsActiveThreadTerminating()) {
        alpha += step;
        if (alpha < 0.0f) {
            alpha = 0.0f;
            step = -step;
        }
        else if (alpha > 1.0f) {
            alpha = 1.0f;
            step = -step;
        }
        GSI->EntitySetAlpha(thing, alpha, false);
        GSI->NewScriptFrame();
    }
}
}