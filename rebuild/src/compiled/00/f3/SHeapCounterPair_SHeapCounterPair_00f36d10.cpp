// SHeapCounterPair::SHeapCounterPair() -- a two-word counter record zeroed at construction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36D10, length 16 bytes. No callers and no vtable slot; it sits between the
// CDynamicObjectHeapBase bodies (split from catalogue row 00f36ca0 via the skipped-row ledger, chunk 907), so it is the
// constructor of a small record of that module. The class name is descriptive (no PDB symbol resolves this address).
// Shape: `mov eax,ecx` first (the constructor returns `this`), then two immediate zero stores through eax.
struct SHeapCounterPair {
    long m_first;                                        // +0x00
    long m_second;                                       // +0x04
    SHeapCounterPair();
};

SHeapCounterPair::SHeapCounterPair()
{
    m_first = 0;
    m_second = 0;
}