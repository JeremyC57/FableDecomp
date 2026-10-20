// CVtable_012b4f74::SetupEventNotification(const SNotificationDesc*) -- adopts a 28-byte notification descriptor, owning a freshly allocated (or reused) buffer when the caller supplies none
// Leaf: SetupEventNotification
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C4E0, 166 bytes (+ int3 padding), `ret 4`. Label correction: the
// catalog files this row under CTCHeroStats (propagated); it is slot 2 of the no-RTTI library vtable 0x012B4F74 (the
// one the base ctor 0x00C3C3B0 stores; it zeroes this descriptor and the flags), in the 0x00C3xxxx library module, so
// the class is named after its vtable. The member name is the catalog's; the real one is unknown.
// Callees: the class's operator new @ 0x00C3C360 and operator delete @ 0x00C3C370 (__cdecl statics over two CRT
// imports; the same pair serves `new` of the derived 0x40-byte object in 0x00C3CD00).
// Shape: a caller-supplied buffer releases any owned one and clears the "owned" bit (2); without one the owned buffer
// is reused when the size matches, else freed and reallocated (-5 on failure); both owning paths share the
// `flags | 2` tail. An OLD descriptor mode of 0x80000000 sets flag 0x80, any other clears bit 31; then the descriptor is copied
// (`rep movsd`, 7 dwords) and the buffer word overwritten with the chosen buffer.
// Layout: descriptor copy +0x10 (its size word lands at +0x1C, its buffer at +0x28), flags +0x2C.

struct SNotificationDesc {                           // 28 bytes
    unsigned long Mode;                              // +0x00 (0x80000000 = special)
    unsigned long Reserved04;                        // +0x04
    unsigned long Reserved08;                        // +0x08
    unsigned long Size;                              // +0x0C
    unsigned long Reserved10;                        // +0x10
    unsigned long Reserved14;                        // +0x14
    void*         Buffer;                            // +0x18
};

class CVtable_012b4f74 {
public:
    virtual void v00();
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* block);       // 0x00C3C370
    unsigned char     _pad_0x04[0x0C];
    SNotificationDesc m_desc;                        // +0x10
    unsigned long     m_flags;                       // +0x2C
    long SetupEventNotification(const SNotificationDesc* desc);
};

long CVtable_012b4f74::SetupEventNotification(const SNotificationDesc* desc)
{
    void* buffer = desc->Buffer;
    if (buffer == 0) {
        if ((m_flags & 2) && desc->Size == m_desc.Size) {
            buffer = m_desc.Buffer;
        } else {
            if (m_flags & 2)
                operator delete(m_desc.Buffer);
            buffer = operator new(desc->Size);
            if (buffer == 0)
                return -5;
        }
        m_flags |= 2;
    } else {
        if (m_flags & 2)
            operator delete(m_desc.Buffer);
        m_flags &= ~2;
    }
    if (m_desc.Mode == 0x80000000)
        m_flags |= 0x80;
    else
        m_flags &= 0x7FFFFFFF;
    m_desc = *desc;
    m_desc.Buffer = buffer;
    return 0;
}
