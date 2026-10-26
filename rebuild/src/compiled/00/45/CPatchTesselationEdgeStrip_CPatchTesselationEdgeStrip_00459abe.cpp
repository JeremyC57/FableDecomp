// CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip() -- frees the three heap blocks at +0x54/+0x48/+0x3c (each only when non-null), then tail-jumps to the base destructor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00459ABE, 53 bytes. Pragma: optimize("s") (`pop ecx` cleanup after each free).
// Callees: free @ 0x00BFEA14 (CRT), base destructor @ 0x00430300 (manifest label "CTCInventoryItem::OnDie" is a
// propagated name over a byte-identical body; it is the out-of-line destructor of the 0x3c-byte base).
// Shape: the "CPatchTesselationEdgeStrip" dtor label is itself propagated across byte-identical landscape
// patch-strip destructors. The three owned blocks sit 12 bytes apart (+0x3c, +0x48, +0x54: the first word of
// three 12-byte array records); they are released in reverse declaration order, then `mov ecx,esi ; pop esi ;
// jmp` is the base destructor called as the last statement of the destructor (sibling call).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

struct CPatchStripBase {
    unsigned char m_baseData[0x3c];
    ~CPatchStripBase();                             // 0x00430300
};

class CPatchTesselationEdgeStrip : public CPatchStripBase { public:
    void*         m_pIndicesA;                      // +0x3c
    unsigned char m_recordA[0x08];                  // +0x40
    void*         m_pIndicesB;                      // +0x48
    unsigned char m_recordB[0x08];                  // +0x4c
    void*         m_pIndicesC;                      // +0x54
    unsigned char m_recordC[0x08];                  // +0x58
    ~CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip()
{
    if (m_pIndicesC != 0) free(m_pIndicesC);
    if (m_pIndicesB != 0) free(m_pIndicesB);
    if (m_pIndicesA != 0) free(m_pIndicesA);
}