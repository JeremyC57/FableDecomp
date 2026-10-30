// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale) -- a quest-info timer element: the base element with the scale, the watched timer's index and the sprite graphic
// Leaf: ??0CQuestInfoElementTimer
// Identity (naming wave 11): ??0CQuestInfoElementTimer@CDrawQuestInfo@NPlayerGui@@QAE@ABVCTimer@@JM@Z (Ego_r.pdb 0x00690EEF,
//   FableWin.pdb 0x020EEE7D): nested in CDrawQuestInfo, the timer by const reference (same code as the pointer).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A4E9, length 44 bytes (`ret 0Ch`, unaligned: a /Os module).
// Caller: the landed CDrawQuestInfo::AddTimer 0x0064F93F (`new CQuestInfoElementTimer(timer, graphic, scale)`, 0x24 bytes).
// Label correction: the manifest's `_Pair_base<CCharString,unsigned long>` is propagated; the body stores the
//   NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer vtable 0x0125A458 (rtti-vtables.tsv) after the base constructor.
// Callee: CQuestInfoElement::CQuestInfoElement(float scale) @ 0x00656795 (catalog "CVsyncCallbackTimerInfo", propagated).
// Layout: as the landed AddTimer / CQuestInfoElementBar (0x0064A450) models: retail CQuestInfoElement is 0x1c bytes
//   (vptr, ID, ScriptID, Scale, a 12-byte STLport vector); Ego_r.pdb CQuestInfoElementTimer TimerIndex +0x20 / Graphic +0x24
//   sit 4 bytes lower in retail (+0x1c / +0x20); CTimer is one long (TimerIndex).
// Shape: the float argument is re-pushed through the x87 (`fld ; push ecx ; fstp [esp]`). Graphic is a plain initialiser
//   and TimerIndex a BODY assignment: the initialiser store is scheduled above the vtable store and the CTimer read (which
//   may alias the object) follows it; with both in the initialiser list VC keeps declaration order, TimerIndex first and
//   both stores below the vtable store.
#pragma optimize("s", on)

#define FABLE_RETAIL_NSCRIPT
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
namespace NPlayerGui {


struct CEnginePrimitiveHandleVector {             // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
};

class CDrawQuestInfo {
public:
    class CQuestInfoElement {                      // retail 0x1c bytes
    public:
        virtual void v00();
        long  ID;                                  // +0x04
        long  ScriptID;                            // +0x08
        float Scale;                               // +0x0c
        CEnginePrimitiveHandleVector PrimitiveHandles; // +0x10
        CQuestInfoElement(float scale);            // 0x00656795 ??0CQuestInfoElement@CDrawQuestInfo@NPlayerGui@@QAE@M@Z (FableWin.pdb)
    };
    class CQuestInfoElementTimer : public CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    public:
        virtual void v00();
        long TimerIndex;                           // +0x1c
        long Graphic;                              // +0x20
        CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale);
    };
};

CDrawQuestInfo::CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer& timer, long graphic, float scale)
    : CQuestInfoElement(scale), Graphic(graphic)
{
    TimerIndex = timer.TimerIndex;
}

}