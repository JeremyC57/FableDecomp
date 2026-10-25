// CVoiceSet::GetMaxValue() -- walks the collection at +0x10 with a stack visitor that keeps the largest item value (from 0) and returns it
// Leaf: GetMaxValue
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37000, 74 bytes, __fastcall, no arguments, result in st0. Descriptive name
//   (no PDB covers this library): the catalog's `IntelligentPointer_BuildAndDispatchFloat` is a description.
// Visitor: vtable 0x012B51E8, 8 bytes {vtable, max}; slot 0 Visit 0x00C36CC0 (`if (item->GetValue() > m_max) m_max =
//   item->GetValue()`, value getter 0x00C361E0); constructor 0x00C36DA0 (landed as
//   CBaseIntelligentPointer::CBaseIntelligentPointer; max = 0); abstract base vtable 0x012B4160, whose destructor
//   0x00C37430 is the EH state-0 unwind action. The collection's slot 10 calls the visitor for every item.
// Shape: `this` stays in edx across the constructor call because the constructor's body is in this translation unit
//   (it touches only eax/ecx); a same-TU noinline stand-in (not under `auto_inline(off)`, which would keep the base's
//   implicit constructor out of line) gives VC7.1 the same knowledge.
struct IValueVisitor {                      // vtable 0x012B4160
    virtual void Visit(void* item) = 0;     // slot 0
    virtual ~IValueVisitor() { }            // slot 1
};

struct CMaxValueVisitor : IValueVisitor {   // vtable 0x012B51E8
    float m_max;                            // +0x04
    CMaxValueVisitor();                     // 0x00C36DA0
    virtual void Visit(void* item);         // 0x00C36CC0
};

struct IItemCollection {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void ForEach(IValueVisitor* visitor);  // slot 10
};

struct CVoiceSet {
    unsigned char _pad00[0x10];
    IItemCollection m_items;                // +0x10
    float GetMaxValue();
};

__declspec(noinline) CMaxValueVisitor::CMaxValueVisitor()
    : m_max(0)
{
}

float CVoiceSet::GetMaxValue()
{
    CMaxValueVisitor visitor;
    m_items.ForEach(&visitor);
    return visitor.m_max;
}