// CUIComponentBase::CUIComponentBase(CUIOwner* owner, const SUIRect& rect) -- display-element base constructor: owner at +4, own vtable, the 20-byte rect copied to +8, the two state words (1, 0), then the element registers itself at the end of the owner's element array (+0x2C)
// Leaf: ??0CUIComponentBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EAA0, 102 bytes (`ret 8`). Descriptive name (no PDB covers this library): the
//   manifest's "NUISystem_CDisplayElement_Constructor" is close but propagated. Vtable 0x012B3404 (no RTTI). Called by
//   the landed derived constructors 0x00C1D880, 0x00C1E050 and 0x00C1E110 (which model it as (owner, rect)); the
//   destructor 0x00C1EB10 removes the element again through the pointer-array Remove 0x00C1EBF0.
// Callees: the element array's Grow @ 0x00C1EB30 (thiscall, slot count).
// Shape: the inline pointer-array Add over `lea edi,[eax+2Ch]` (count read before data, `inc dword ptr`); the rect is
//   an init-list copy through `lea ecx,[esi+8]`.
struct SUIRect { long m_words[5]; };

struct CUIComponentBase;

template <class T>
struct CPtrArray {
    T* m_data;                              // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    void Grow(unsigned long slots);         // 0x00C1EB30
    void Add(T value)
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

struct CUIOwner {
    unsigned char _pad[0x2c];
    CPtrArray<CUIComponentBase*> m_elements;    // +0x2C
};

struct CUIComponentBase {                   // vtable 0x012B3404
    virtual void v00();
    CUIOwner* m_pOwner;                     // +0x04
    SUIRect m_rect;                         // +0x08
    unsigned short m_state;                 // +0x1C
    unsigned short m_flags;                 // +0x1E
    CUIComponentBase(CUIOwner* owner, const SUIRect& rect);
};

CUIComponentBase::CUIComponentBase(CUIOwner* owner, const SUIRect& rect)
    : m_pOwner(owner), m_rect(rect), m_state(1), m_flags(0)
{
    owner->m_elements.Add(this);
}