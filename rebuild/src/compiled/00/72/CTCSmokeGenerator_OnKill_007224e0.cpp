// CTCSmokeGenerator::OnKill() -- kill every live smoke thing the generator spawned
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007224E0, length 92 bytes; vtable 0x01265D74 (CTCSmokeGenerator) slot 14,
// the CTCBase OnKill slot (identity as the landed CTCCarryable/CTCBoastingArea slot-14 overrides).
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (three times per element), CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CTCSmokeGenerator with the retail 12-byte vector: GenerationPositions first/last +0x10/+0x14,
// PositionsCreated +0x1c; a CGenerationPos is 0x14 bytes, its intelligent pointer at +0x0c (a 12-byte position first).
// The thing's flag byte +0x91 bit 0 marks it as already killed.
// Shape: `last` re-read every iteration; the loop keeps the element and its pointer member in two strength-reduced
// registers.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                                  // +0x91, bit 0: killed
    void Kill(bool immediately);                            // 0x004C9B80
};

struct CBaseIntelligentPointer {
    void*   m_vtable;
    void*   m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};

struct CGenerationPos {                                     // CTCSmokeGenerator::CGenerationPos, 0x14 bytes
    float                   Position[3];                    // +0x00
    CBaseIntelligentPointer PThing;                         // +0x0c
};

class CTCSmokeGenerator { public:
    void*           __vftable;                              // +0x00
    CThing*         ParentThing;                            // +0x04
    bool            ToBeRemoved;                            // +0x08
    float           GenerationDistance;                     // +0x0c
    CGenerationPos* m_pFirst;                               // +0x10 GenerationPositions
    CGenerationPos* m_pLast;                                // +0x14
    CGenerationPos* m_pEnd;                                 // +0x18
    bool            PositionsCreated;                       // +0x1c
    void OnKill();
};

void CTCSmokeGenerator::OnKill()
{
    if (PositionsCreated) {
        for (CGenerationPos* pos = m_pFirst; pos != m_pLast; ++pos) {
            if (pos->PThing.GetPItem() && !(pos->PThing.GetPItem()->m_flags & 1))
                pos->PThing.GetPItem()->Kill(true);
        }
    }
}