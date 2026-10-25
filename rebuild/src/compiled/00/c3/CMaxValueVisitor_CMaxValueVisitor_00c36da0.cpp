// CMaxValueVisitor::CMaxValueVisitor() -- constructs the stack visitor CVoiceSet::GetMaxValue walks its items with: vtable 0x012B51E8, the running maximum at +4 starting from 0.0f
// Leaf: ??0CMaxValueVisitor@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36DA0, 16 bytes, __thiscall `ret`, returns `this`:
//   mov eax,ecx ; mov dword ptr [eax],12B51E8h ; mov dword ptr [eax+4],0 ; ret
// Naming (sweep1, 2026-10-25): was landed as CBaseIntelligentPointer::CBaseIntelligentPointer (a propagated manifest label;
//   its `m_pItem = 0` is the float maximum); FINDINGS chunk 437 ("0xC36DA0 = visitor ctor, m_pItem = 0 is float max = 0").
//   Its only caller is the landed CVoiceSet::GetMaxValue 0x00C37000 (call 0x00C3701D), whose source models this exact
//   class: CMaxValueVisitor : IValueVisitor (vtable 0x012B4160: pure Visit, inline virtual destructor), vtable 0x012B51E8
//   slot 0 Visit 0x00C36CC0 (`if (item->GetValue() > m_max) m_max = ...`), and returns m_max in st0. The 0.0f store is
//   the integer immediate 0. Descriptive names (no PDB covers libpk).
struct IValueVisitor {                      // vtable 0x012B4160
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~IValueVisitor() { }            // slot 1
};

struct CMaxValueVisitor : IValueVisitor {   // vtable 0x012B51E8
    float m_max;                            // +0x04
    CMaxValueVisitor();
    virtual void Visit(void* item);         // 0x00C36CC0
};

CMaxValueVisitor::CMaxValueVisitor()
    : m_max(0)
{
}
