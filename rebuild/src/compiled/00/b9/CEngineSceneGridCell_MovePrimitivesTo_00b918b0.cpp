// CEngineSceneGridCell::MovePrimitivesTo(CEngineSceneGridCell*) -- move every primitive of this cell into another cell (unlink, re-point, link, count, invalidate the destination's caches), then invalidate this cell's caches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B918B0, length 108 bytes (+ int3 padding).
// Callees: CEngineInternalPrimitiveBase::RemoveFromList @ 0x00B8FE60 and AddToList(list head) @ 0x00B8FDF0 (on the primitive),
// CEngineSceneGridCell::InvalidateAll @ 0x00B91710 (on the parent).
// Layout: the PDB CEngineSceneGridCell (Parent +0x00, FirstPrimitive +0x04, LocalCount +0x34, the validity bit-field byte
// +0x44: bits 0-2 = BoundingBoxValid / TotalCountValid / RenderMaskValid); primitives chain through +0x38 and point back at
// their cell at +0x20. No engine header exists for these classes. Models reused from the landed CEngineSceneGridCell::AddPrimitive
// @ 0x00B91870 and InvalidateAll @ 0x00B91710.
// Shape: the successor is read (ebx) before the primitive is unlinked; the destination's inline AddPrimitive stores the cell
// pointer between the list-head push and the call, then increments the count interleaved with the flag test; the inline
// Invalidate (`test al,7 ; and al,0F8h`, parent null test between the and and the store) runs per primitive on the
// destination and once on this cell.
class CEngineSceneGridCell;

class CEngineInternalPrimitiveBase { public:
    unsigned char                 _pad[0x20];
    CEngineSceneGridCell*         m_pCell;         // +0x20
    unsigned char                 _pad24[0x14];
    CEngineInternalPrimitiveBase* m_pNextInCell;   // +0x38
    void RemoveFromList();                         // 0x00B8FE60
    void AddToList(CEngineInternalPrimitiveBase** head);   // 0x00B8FDF0
};

class CEngineSceneGridCell { public:
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    unsigned char                 _pad08[0x2c];    // +0x08 bounding box, draw distance, group handles
    long                          LocalCount;      // +0x34
    unsigned char                 _pad38[0xc];     // +0x38 total count, render mask, occluded layers
    unsigned char                 Flags;           // +0x44 (bits 0-2: the cached-value validity bits)
    void InvalidateAll();                          // 0x00B91710
    void Invalidate()
    {
        if (Flags & 7) {
            Flags &= ~7;
            if (Parent)
                Parent->InvalidateAll();
        }
    }
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive)
    {
        primitive->m_pCell = this;
        primitive->AddToList(&FirstPrimitive);
        ++LocalCount;
        Invalidate();
    }
    void MovePrimitivesTo(CEngineSceneGridCell* dest);
};

void CEngineSceneGridCell::MovePrimitivesTo(CEngineSceneGridCell* dest)
{
    CEngineInternalPrimitiveBase* primitive = FirstPrimitive;
    while (primitive) {
        CEngineInternalPrimitiveBase* next = primitive->m_pNextInCell;
        primitive->RemoveFromList();
        dest->AddPrimitive(primitive);
        primitive = next;
    }
    Invalidate();
}