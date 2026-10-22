// CTCClock::vfunc_12(const CTCClock*) -- copies the other clock's def pointer: the old def released (virtual destroy at zero) and cleared, the new one stored and add-ref'd, nothing done when both already hold the same def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007327B0, length 57 bytes (+ int3 padding). Vtable 0x01267044
// (CTCClock) slot 12.
// Callees: none (the def's slot-1 destroy is a virtual call).
// Layout = Ego_r.pdb CTCClock (0x14 bytes): CTCBase 0xc, PDef +0xc (CDefPointer<CClockDef const>, one
// word), LastTimeUpdated +0x10.
// Shape: `PDef = other->PDef` through the inlined CDefPointer::operator= of the landed 0x0083E0D0 family
// (compare first, release through `dec [ecx+4] ; jne ; call [vt+4]`, the null store, then the source
// re-read, stored and add-ref'd with `inc [eax+4]`).
struct CRefCounted {                        // second base: the count sits at +4 behind the vptr
    long m_count;
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                 // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void Release()                          // inlined into CDefPointer::operator=
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CDefPointer {                        // CDefPointer<CClockDef const>
    CDefBase* m_p;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

struct CTCClock {                           // vtable 0x01267044
    void*         __vftable;                // +0x00
    unsigned char _pad04[8];                // +0x04 CTCBase
    CDefPointer   PDef;                     // +0x0c
    float         LastTimeUpdated;          // +0x10
    void Copy(const CTCClock* other);
};

void CTCClock::Copy(const CTCClock* other)
{
    PDef = other->PDef;
}