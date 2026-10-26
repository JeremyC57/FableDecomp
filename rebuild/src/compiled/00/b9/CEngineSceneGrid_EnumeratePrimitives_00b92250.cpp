// CEngineSceneGrid::EnumeratePrimitives(unsigned long, unsigned long, CEnginePrimitiveEnumerator*) -- hand every primitive of a grid cell to the enumerator, then recurse into the four finer cells (2x+j, 2y+i) on LevelDown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92250, length 141 bytes.
// Callees: virtual slot 1 of the enumerator (the primitive), itself @ 0x00B92250 (on LevelDown, re-read for every call).
// Layout: the PDB CEngineSceneGrid / CEngineSceneGridCell (as in GetCellTotalPrimitiveCount @ 0x00B91D20); primitives are
// chained through +0x38 from the cell's FirstPrimitive (+0x04); no engine header exists for these classes.
// Shape: a missing cell skips everything; the primitive walk is a top-tested while (esi); the enumerator stays in ebp, so
// VC7.1 reuses its dead stack slot for the outer index; the row `y*2 + i` is formed once per outer iteration (edi).
class CEngineInternalPrimitiveBase;

class CEngineSceneGridCell { public:                      // 0x48 bytes (PDB layout, matches retail)
    CEngineSceneGridCell*         Parent;          // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;  // +0x04
    float                         BoundingBox[6];  // +0x08 C3DBoundingBox
    float                         DrawDistance;    // +0x20
    unsigned char                 GroupPrimitiveHandle[0x10];   // +0x24 CEnginePrimitiveHandle[2]
    long                          LocalCount;      // +0x34
    unsigned long                 TotalCount;      // +0x38
    unsigned long                 RenderMask;      // +0x3c
    unsigned long                 OccludedLayers;  // +0x40
    unsigned char                 BoundingBoxValid : 1;     // +0x44 bit 0
    unsigned char                 TotalCountValid : 1;      //        bit 1
    unsigned char                 RenderMaskValid : 1;      //        bit 2
    unsigned char                 WasRendered : 1;          //        bit 3
    unsigned char                 HasOcclusionResults : 1;  //        bit 4
    unsigned char                 OccludedFrames;  // +0x45
    unsigned char                 PreloadFrames;   // +0x46
};

template <class T> class CArray { public:                 // retail CArray: data pointer first
    T*   m_pData;
    long m_size;
    long m_capacity;
};

class CEngineSceneGrid { public:                          // 0x24 bytes (PDB layout, matches retail)
    unsigned long                 _vfptr_or_pad;   // +0x00
    unsigned long                 Level;           // +0x04
    CEngineSceneGrid*             LevelUp;         // +0x08
    CEngineSceneGrid*             LevelDown;       // +0x0c
    unsigned long                 Size;            // +0x10
    CArray<CEngineSceneGridCell*> GridCells;       // +0x14
    void EnumeratePrimitives(unsigned long x, unsigned long y, struct CEnginePrimitiveEnumerator* enumerator);
};

class CEngineInternalPrimitiveBase { public:
    unsigned char                 _pad[0x38];
    CEngineInternalPrimitiveBase* m_pNextInCell;   // +0x38
};

struct CEnginePrimitiveEnumerator {
    virtual void v00();
    virtual void Visit(CEngineInternalPrimitiveBase* primitive);   // slot 1
};

void CEngineSceneGrid::EnumeratePrimitives(unsigned long x, unsigned long y, CEnginePrimitiveEnumerator* enumerator)
{
    CEngineSceneGridCell* cell = GridCells.m_pData[y * Size + x];
    if (cell) {
        for (CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive; primitive; primitive = primitive->m_pNextInCell)
            enumerator->Visit(primitive);
        if (LevelDown) {
            for (unsigned long i = 0; i < 2; ++i)
                for (unsigned long j = 0; j < 2; ++j)
                    LevelDown->EnumeratePrimitives(x * 2 + j, y * 2 + i, enumerator);
        }
    }
}