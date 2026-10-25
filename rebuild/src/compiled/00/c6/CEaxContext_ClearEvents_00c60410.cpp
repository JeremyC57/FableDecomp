// CEaxContext::ClearEvents() -- free the EAX context's queued notifications: unlink each event from the +0x3c4 queue (next pointer +0x94) and return it to the library allocator (CPKObject::operator delete 0x00C3C370)
// Leaf: ?ClearEvents@CEaxContext@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60410, 55 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CEventChain_00c60410::DeleteAll = CEaxContext::ClearEvents"): agent442's
//   landed ~CEaxContext 0x00C60930 and CEaxSource::Set 0x00C61B40 call this as ClearEvents (rebuild/oracles/
//   reloc_bindings.tsv binds ?ClearEvents@CEaxContext@@QAEXXZ to 0x00C60410); the queue holds the 0x98-byte SEaxEvent
//   records CEaxContext::InvalidateAll 0x00C60A30 allocates through CPKObject::operator new 0x00C3C360 and appends at
//   +0x3c4 (next +0x94); 0x00C3C370 is the matching CPKObject::operator delete. The CEventChain_00c60410 name and the
//   chunk label "CDrawGuildSeal::ClearMessages" are propagated; descriptive names (libpk: no PDB/RTTI).
// Shape: the head is unlinked BEFORE the call (`mov [esi+3C4h],ecx` between the push and the call) and re-read for the
//   loop test; the loop head is aligned with a `lea ecx,[ecx]` nop.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject();
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

struct SEaxEvent {                                  // 0x98-byte queued notification
    unsigned long m_flags;                          // +0x00
    unsigned long m_type;                           // +0x04 2 = environment changed
    unsigned long m_environment;                    // +0x08
    unsigned char _pad0c[0x88];
    SEaxEvent*    m_pNext;                          // +0x94
};

class CEaxContext : public CPKObject {              // vtable 0x012B9340
public:
    unsigned char    _pad004[0x3c0];                // +0x004
    SEaxEvent*       m_pEvents;                     // +0x3c4 queued notifications
    void ClearEvents();
};

void CEaxContext::ClearEvents()
{
    while (m_pEvents != 0) {
        SEaxEvent* ev = m_pEvents;
        m_pEvents = ev->m_pNext;
        CPKObject::operator delete(ev);
    }
}
