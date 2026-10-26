// CEngineSceneGridCell::InvalidateAll()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91710, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  L: mov al,[ecx+44h] ; test al,7 ; je R ; and al,0F8h ; mov [ecx+44h],al ; mov ecx,[ecx] ; test ecx,ecx ; jne L ; R: ret
// PDB CEngineSceneGridCell: +0x00 CEngineSceneGridCell* Parent; +0x44 bitfield byte with
// bit0 BoundingBoxValid, bit1 TotalCountValid, bit2 RenderMaskValid, bit3 WasRendered,
// bit4 HasOcclusionResults. Walks up the parent chain clearing the three cached-validity bits
// and stops early at the first ancestor that is already fully invalid (its ancestors must be too).

class CEngineSceneGridCell { public:
    CEngineSceneGridCell* Parent;               // +0x00 (PDB)
    unsigned char         _pad_0x04[0x40];      // +0x04 bounding box, counts, masks (PDB)
    unsigned char         Flags;                // +0x44 (PDB bitfield, see above)
    void InvalidateAll();
};

#define CELL_VALID_MASK 0x07   // BoundingBoxValid | TotalCountValid | RenderMaskValid

void CEngineSceneGridCell::InvalidateAll()
{
    CEngineSceneGridCell* cell = this;
    do {
        unsigned char flags = cell->Flags;
        if (!(flags & CELL_VALID_MASK))
            return;                                   // already invalid: so are the ancestors
        cell->Flags = (unsigned char)(flags & ~CELL_VALID_MASK);
        cell = cell->Parent;
    } while (cell);
}