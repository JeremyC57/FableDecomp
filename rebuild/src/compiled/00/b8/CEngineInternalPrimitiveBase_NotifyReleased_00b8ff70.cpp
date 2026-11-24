// Leaf: ?NotifyReleased@CEngineInternalPrimitiveBase@@MAEXABVCEnginePrimitiveHandle@@@Z
// CEngineInternalPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) -- invalidate the scene-grid cell this primitive sits in (unless bit 1 of the flag byte says it is detached), mark the grid level as none and unlink from the primitive list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FF70, length 36 bytes.
// Callees: CEngineSceneGridCell::InvalidateAll @ 0x00B91710 (on GridCell at +0x20), RemoveFromList @ 0x00B8FE60 (on `this`). Shape: the raw flag test
// `test byte ptr [esi+34h],2`, the null test of the cell, and the `-1` level byte stored between `mov ecx,esi` and the last call. Layout from
// engine/CEngineInternalPrimitiveBase.h (PDB): GridCell +0x20, GridPos_Level +0x24, PostRenderEnable (flag group) +0x34. The handle argument is unused.
class CEnginePrimitiveHandle;

class CEngineSceneGridCell { friend class CEngineInternalPrimitiveBase; public:
    private: void InvalidateAll(); public:                          // 0x00B91710
};

class CEngineInternalPrimitiveBase { public:
    unsigned char _pad[0x1c];
    CEngineSceneGridCell* GridCell;                // +0x20
    signed char           GridPos_Level;           // +0x24
    unsigned char         _pad25[0x0f];
    unsigned char         PostRenderEnable;        // +0x34: bit 1 = detached from the grid
    void RemoveFromList();                         // 0x00B8FE60
    protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);
    public:
};

void CEngineInternalPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle& handle)
{
    if (!(PostRenderEnable & 2)) {
        if (GridCell != 0)
            GridCell->InvalidateAll();
    }
    GridPos_Level = -1;
    RemoveFromList();
}