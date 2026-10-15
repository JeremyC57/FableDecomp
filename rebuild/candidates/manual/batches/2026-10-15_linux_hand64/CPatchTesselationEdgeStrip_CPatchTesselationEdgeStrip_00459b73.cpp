// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- frees the four heap blocks at +0x60/+0x54/+0x48/+0x3c (each only when non-null), then tail-jumps to the base destructor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00459B73, 67 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: free @ 0x00BFEA14 (CRT), base destructor @ 0x00430300 (manifest label "CTCInventoryItem::OnDie" is a
// propagated name over a byte-identical body; it is the out-of-line destructor of the 0x3c-byte base).
// Shape: same family as 0x00459ABE with a fourth 12-byte array record: the owned blocks (+0x3c, +0x48, +0x54,
// +0x60) are released last-to-first, then the base destructor is the sibling-call `jmp` with `this` in ecx.
// The "CPatchTesselationEdgeStrip" dtor label is propagated across these byte-identical landscape strip bodies.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

struct CPatchStripBase {
    unsigned char m_baseData[0x3c];
    ~CPatchStripBase();                             // 0x00430300
};

struct CPatchTesselationEdgeStrip : CPatchStripBase {
    void*         m_pIndicesA;                      // +0x3c
    unsigned char m_recordA[0x08];
    void*         m_pIndicesB;                      // +0x48
    unsigned char m_recordB[0x08];
    void*         m_pIndicesC;                      // +0x54
    unsigned char m_recordC[0x08];
    void*         m_pIndicesD;                      // +0x60
    unsigned char m_recordD[0x08];
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
    if (m_pIndicesD != 0) free(m_pIndicesD);
    if (m_pIndicesC != 0) free(m_pIndicesC);
    if (m_pIndicesB != 0) free(m_pIndicesB);
    if (m_pIndicesA != 0) free(m_pIndicesA);
}
