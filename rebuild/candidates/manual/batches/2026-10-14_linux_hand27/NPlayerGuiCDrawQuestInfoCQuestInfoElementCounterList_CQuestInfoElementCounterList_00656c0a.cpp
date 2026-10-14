#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList::CQuestInfoElementCounterList(long first, long count, float height) -- base element(height), the list range stored, the cursor zeroed, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656C0A, length 46 bytes.
// Callees: the quest-info element base constructor @ 0x00656795 (one float argument, pushed as a copy: `fld [esp+0Ch] ; push ecx ; fstp [esp]`).
// Vtable 0x0125A504 = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounterList (rtti-vtables.tsv); the manifest labels
// "CCrimeStatBar::CCrimeStatBar" (this body) and "CVsyncCallbackTimerInfo" (the base) are propagated.
// Shape: the size-optimised TU zeroes the middle word with `and dword ptr [esi+20h],0`; the two long arguments are
// copied around it. No PDB layout for the element classes; member names are descriptive.
namespace NPlayerGui {

struct CQuestInfoElement {
    virtual void v00();
    unsigned char _body[0x1c - 4];             // +0x04 .. +0x1c: the element base state
    CQuestInfoElement(float height);           // 0x00656795
};

struct CQuestInfoElementCounterList : CQuestInfoElement {   // vtable 0x0125A504
    virtual void v00();
    long m_firstCounter;                       // +0x1c
    long m_cursor;                             // +0x20
    long m_counterCount;                       // +0x24
    CQuestInfoElementCounterList(long firstCounter, long counterCount, float height);
};

CQuestInfoElementCounterList::CQuestInfoElementCounterList(long firstCounter, long counterCount, float height)
    : CQuestInfoElement(height), m_firstCounter(firstCounter), m_cursor(0), m_counterCount(counterCount)
{
}

}
