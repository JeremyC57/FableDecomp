A="rebuild/src/compiled/00/9c/CompareDataOffset_operator_009cc640.cpp"
B="rebuild/src/compiled/00/9d/std_Pushheap_009d3040.cpp"
C="rebuild/src/compiled/00/9d/global_Adjustheap_009d3750.cpp"
EDITS=[
(A,"    unsigned long operator[](unsigned long index);   // 0x00A629F0","    unsigned long operator[](unsigned long index) const;   // 0x00A629F0 (Ego_r / FableWin ??ACPackedUIntArray@@QBEKK@Z)"),
(B,"""struct CPackedUIntArray {                                 // Ego_r.pdb 16 bytes
    unsigned long* PackedInts;                            // +0x00
    unsigned long  Size;                                  // +0x04
    unsigned char  Bits;                                  // +0x08
    unsigned long  Bias;                                  // +0x0C
    unsigned long operator[](unsigned long index);        // 0x00A629F0
};
""",""),
(B,"namespace N009d3040 {\n","""struct CPackedUIntArray {                                 // Ego_r.pdb 16 bytes (global class: Ego_r / FableWin ??ACPackedUIntArray@@QBEKK@Z)
    unsigned long* PackedInts;                            // +0x00
    unsigned long  Size;                                  // +0x04
    unsigned char  Bits;                                  // +0x08
    unsigned long  Bias;                                  // +0x0C
    unsigned long operator[](unsigned long index) const;  // 0x00A629F0
};
namespace N009d3040 {
"""),
(C,"""struct CPackedUIntArray {                          // 16 bytes
    unsigned long* PackedInts;                     // +0x00
    unsigned long  Size;                           // +0x04 (0: not packed, use RuntimeData)
    unsigned char  Bits;                           // +0x08
    unsigned long  Bias;                           // +0x0C
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return Size != 0; }
};
""",""),
(C,"namespace FableHeap_009d3750 {\n","""struct CPackedUIntArray {                          // 16 bytes (global class: Ego_r / FableWin ??ACPackedUIntArray@@QBEKK@Z)
    unsigned long* PackedInts;                     // +0x00
    unsigned long  Size;                           // +0x04 (0: not packed, use RuntimeData)
    unsigned char  Bits;                           // +0x08
    unsigned long  Bias;                           // +0x0C
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return Size != 0; }
};
namespace FableHeap_009d3750 {
"""),
]
