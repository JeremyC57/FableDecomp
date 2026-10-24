// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale) -- a quest-info timer element: the base element with the scale, the watched timer's index and the sprite graphic
// Leaf: ??0CQuestInfoElementTimer
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

namespace NPlayerGui {

struct CTimer {                                    // Ego_r.pdb: one long
    long TimerIndex;                               // +0x00
};

struct CEnginePrimitiveHandleVector {             // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
};

struct CQuestInfoElement {                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes)
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    CEnginePrimitiveHandleVector PrimitiveHandles; // +0x10
    CQuestInfoElement(float scale);                // 0x00656795
};

struct CQuestInfoElementTimer : CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    virtual void v00();
    long TimerIndex;                               // +0x1c
    long Graphic;                                  // +0x20
    CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale);
};

CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale)
    : CQuestInfoElement(scale), Graphic(graphic)
{
    TimerIndex = timer->TimerIndex;
}

}