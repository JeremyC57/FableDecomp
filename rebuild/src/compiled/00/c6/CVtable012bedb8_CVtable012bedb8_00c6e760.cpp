// CVtable_012bedb8::CVtable_012bedb8() -- constructor of the gain-ramp mixer effect (vtable 0x012BEDB8): the critical-section-owning base (0x00C6E6A0), own vtable, then the current / previous ramp pointers aimed at the two 4-byte gain records it owns
// Leaf: ??0CVtable_012bedb8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E760, 30 bytes (+ int3 padding).
// Callers: 0x00C550B3, 0x00C55113. Callees: CVtable_012beda4::CVtable_012beda4 0x00C6E6A0 (landed; 0x28 bytes).
// Label: the manifest's "CConsoleCommandParameters_Constructor" is propagated; vtable 0x012BEDB8 has no RTTI (libpk
//   mixer module), so the class is named by vtable like its landed siblings CVtable_012bed90 (0x00C6D9D0) and
//   CVtable_012bed7c (0x00C6CA20); its destructor 0x00C6E780 restores the vtable and tail-jumps to 0x00C6E630, and its
//   per-block Process (slot 2, 0x00C6E8F0) ramps the gain record behind +0x30 down / up through 0x00C6E820 / 0x00C6E790
//   and then swaps the +0x30 / +0x34 pointers.
// Shape: the records have no constructor, so only the two pointer members are initialised (init-list order); VC7.1
//   schedules the vtable store between the two pointer stores, as retail.
struct CVtable_012beda4 {                              // vtable 0x012BEDA4, 0x28 bytes
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, count, flag
    CVtable_012beda4();                                // 0x00C6E6A0
};
struct SGainRamp {                                     // 4 bytes: a Q15 gain and its per-sample step
    unsigned short m_gain;                             // +0x00
    short          m_step;                             // +0x02
};
struct CVtable_012bedb8 : CVtable_012beda4 {           // vtable 0x012BEDB8
    SGainRamp  m_first;                                // +0x28
    SGainRamp  m_second;                               // +0x2c
    SGainRamp* m_pCurrent;                             // +0x30
    SGainRamp* m_pPrevious;                            // +0x34
    virtual void v00();
    CVtable_012bedb8();
};
CVtable_012bedb8::CVtable_012bedb8()
    : m_pCurrent(&m_first), m_pPrevious(&m_second)
{
}