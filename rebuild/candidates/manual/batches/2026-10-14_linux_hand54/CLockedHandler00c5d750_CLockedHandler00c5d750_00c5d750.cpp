// CLockedHandler_00c5d750::CLockedHandler_00c5d750() -- this class's vtable (0x012B8FCC, no RTTI), the critical-section member at +0x1c constructed inline (its vtable 0x012B8FC8, InitializeCriticalSection on its section at +0x20), then the four dwords +4..+0x10 cleared in the body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D750, length 48 bytes.
// The chunk label is propagated; named by address. Shape: `this` in esi, the member constructed before the zero stores (so they are body assignments), `ret`.
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4

struct CCriticalSectionObject_012b8fc8 {           // vtable 0x012B8FC8 (its deleting destructor is landed as 0x00C5D5B0)
    virtual void v00();
    unsigned char m_section[24];                   // +0x04: a CRITICAL_SECTION
    CCriticalSectionObject_012b8fc8() { InitializeCriticalSection(m_section); }
};

struct CLockedHandler_00c5d750 {                   // vtable 0x012B8FCC
    virtual void v00();
    long                            m_4;           // +0x04
    long                            m_8;           // +0x08
    long                            m_c;           // +0x0c
    long                            m_10;          // +0x10
    unsigned char                   _pad14[8];
    CCriticalSectionObject_012b8fc8 m_lock;        // +0x1c
    CLockedHandler_00c5d750();
};

CLockedHandler_00c5d750::CLockedHandler_00c5d750()
    : m_lock()
{
    m_8 = 0;
    m_4 = 0;
    m_c = 0;
    m_10 = 0;
}
