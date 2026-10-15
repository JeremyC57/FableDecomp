// SVtableRecord_00c3c8a0::SVtableRecord_00c3c8a0() -- store the vtable, zero two words, set the index at +0x0c to -1, zero +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C8A0, length 27 bytes (+ int3 padding). No callees.
// Leaf: ??0SVtableRecord_00c3c8a0
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated; callers are SExpSpendUndo (0x00C3D7D0) and
// TeleportData (0x00C4D000) constructors, four instances each. Vtable 0x012B586C has no RTTI row (descriptive name).
// Shape: `xor ecx,ecx` before the vtable store, then the members in order with -1 as an immediate: a plain initialiser list.
struct SVtableRecord_00c3c8a0 {
    virtual void v00();
    long m_a;          // +0x04
    long m_b;          // +0x08
    long m_index;      // +0x0c (-1 = none)
    long m_d;          // +0x10
    SVtableRecord_00c3c8a0();
};

SVtableRecord_00c3c8a0::SVtableRecord_00c3c8a0() : m_a(0), m_b(0), m_index(-1), m_d(0)
{
}
