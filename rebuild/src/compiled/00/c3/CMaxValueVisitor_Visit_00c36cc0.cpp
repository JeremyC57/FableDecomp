// CMaxValueVisitor::Visit(void* item) -- raise the running maximum (+4) to the item's value when the value is larger
// Leaf: ?Visit@CMaxValueVisitor@@UAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36CC0, 40 bytes, thiscall `ret 4`.
// Identity: slot 0 of the visitor vtable 0x012B51E8; the landed constructor 0x00C36DA0 and CVoiceSet::GetMaxValue
//   0x00C37000 model this class and name this slot (`if (item->GetValue() > m_max) m_max = item->GetValue()`).
//   Descriptive (libpk, no PDB/RTTI).
// Callee: the item's value getter 0x00C361E0 (`return +0x164`), declared with its landed spelling ?M@C00c361e0@@QAEMXZ.
// Shape: the getter is called twice (compare, then store); `value > m_max` is `fcomp [esi+4] ; test ah,41h ; jne`.
struct C00c361e0 {
    float M();                              // 0x00C361E0
};

struct IValueVisitor {                      // vtable 0x012B4160
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~IValueVisitor() { }            // slot 1
};

struct CMaxValueVisitor : IValueVisitor {   // vtable 0x012B51E8
    float m_max;                            // +0x04
    virtual void Visit(void* item);
};

void CMaxValueVisitor::Visit(void* item)
{
    C00c361e0* source = (C00c361e0*)item;
    if (source->M() > m_max)
        m_max = source->M();
}