// CQuestInfoElement::CQuestInfoElement(long) -- store the vtable, zero the element's words and keep the argument at +0x0c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656795, length 35 bytes. No callees.
// Leaf: ??0CQuestInfoElement
// Vtable 0x0125A2B4 is NPlayerGui::CDrawQuestInfo::CQuestInfoElement (rtti-vtables.tsv); the nine callers are the
// CQuestInfoElementBar constructors (0x0064A450, 0x00656B1B, ...). The catalog label "CVsyncCallbackTimerInfo" is propagated.
// Shape: argument loaded into edx first, `mov eax,ecx ; xor ecx,ecx`, vtable, then members in order: a plain initialiser list.
struct CQuestInfoElement {
    virtual void v00();
    long m_a;          // +0x04
    long m_b;          // +0x08
    long m_value;      // +0x0c (the constructor argument)
    long m_d;          // +0x10
    long m_e;          // +0x14
    long m_f;          // +0x18
    CQuestInfoElement(long value);
};

CQuestInfoElement::CQuestInfoElement(long value) : m_a(0), m_b(0), m_value(value), m_d(0), m_e(0), m_f(0)
{
}
