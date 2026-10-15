// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter::CQuestInfoElementCounter(long count, long total, float height) -- base element(height), the count and total stored, the two display values zeroed, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656BA2, length 50 bytes; a size-optimised TU
// (`and dword ptr [..],0` zero stores): `#pragma optimize("s", on)`.
// Callees: the quest-info element base constructor @ 0x00656795 (one float argument, pushed as a copy:
// `fld [esp+0Ch] ; push ecx ; fstp [esp]`; manifest label "CVsyncCallbackTimerInfo" is propagated).
// Identity: vtable 0x0125A4F0 = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter (rtti-vtables.tsv);
// the manifest label "CTCAtmosPlayer::CTCAtmosPlayer" is propagated. The sibling constructor
// 0x00656BD8 stores the same class with the other member pair zeroed. No PDB layout exists for the
// element classes; member names are descriptive.
// Shape (as the landed sibling 0x00656C0A): the zero initialisers are scheduled before the argument
// copies, all before the vtable store.
#pragma optimize("s", on)
namespace NPlayerGui {

struct CQuestInfoElement {
    virtual void v00();
    unsigned char _body[0x1c - 4];                          // +0x04 .. +0x1c: the element base state
    CQuestInfoElement(float height);                        // 0x00656795
};

struct CQuestInfoElementCounter : CQuestInfoElement {       // vtable 0x0125A4F0
    virtual void v00();
    long m_count;                                           // +0x1c
    long m_total;                                           // +0x20
    long m_displayedCount;                                  // +0x24
    long m_displayedTotal;                                  // +0x28
    CQuestInfoElementCounter(long count, long total, float height);
};

CQuestInfoElementCounter::CQuestInfoElementCounter(long count, long total, float height)
    : CQuestInfoElement(height), m_count(count), m_total(total), m_displayedCount(0), m_displayedTotal(0)
{
}

}