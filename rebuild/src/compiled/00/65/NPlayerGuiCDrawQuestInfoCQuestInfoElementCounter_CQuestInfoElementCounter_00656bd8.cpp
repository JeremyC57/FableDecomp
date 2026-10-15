// NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter::CQuestInfoElementCounter(unsigned long total, unsigned long displayedTotal, float height) -- base element(height), the count pair zeroed, the two totals stored, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656BD8, length 50 bytes; a size-optimised TU
// (`and dword ptr [..],0` zero stores): `#pragma optimize("s", on)`.
// Callees: the quest-info element base constructor @ 0x00656795 (one float argument, pushed as a copy).
// Identity: vtable 0x0125A4F0 = NPlayerGui::CDrawQuestInfo::CQuestInfoElementCounter (rtti-vtables.tsv),
// the second constructor of the class (0x00656BA2 is the first); the manifest label
// "CTCAtmosPlayer::CTCAtmosPlayer" is propagated. The two constructors differ only in which member pair
// receives the arguments (+0x20/+0x28 here, +0x1c/+0x20 there); the parameter types of this overload
// are not visible in the bytes (both are 4-byte values copied through eax) and are descriptive.
// Shape: the zero initialisers are scheduled before the argument copies, all before the vtable store.
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
    unsigned long m_total;                                  // +0x20
    long m_displayedCount;                                  // +0x24
    unsigned long m_displayedTotal;                         // +0x28
    CQuestInfoElementCounter(unsigned long total, unsigned long displayedTotal, float height);
};

CQuestInfoElementCounter::CQuestInfoElementCounter(unsigned long total, unsigned long displayedTotal, float height)
    : CQuestInfoElement(height), m_count(0), m_total(total), m_displayedCount(0), m_displayedTotal(displayedTotal)
{
}

}