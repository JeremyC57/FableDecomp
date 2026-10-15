// CEngineInternalPrimitiveBase::RemoveFromList() -- unlinks the primitive from its scene-grid cell (persistent primitives only) and from the render list, handing its mask-group head role to the next primitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FE60, length 116 bytes. No callees.
// Layout (PDB names, engine/CEngineInternalPrimitiveBase.h): GridCell +0x20 (CEngineSceneGridCell, primitive count +0x34), flag byte
// +0x34 (bit 1 = NonPersistent), NextPrimitive/RefPrimitive +0x38/+0x3C, NextLayerMask/RefLayerMask +0x40/+0x44 (Ref = the address of
// the pointer that points at us). Modelled locally: the header keeps the bitfield group opaque and types the Ref members as void*.
// Shape: the zero needed by the four closing stores is held in esi from the start, so every null test is `cmp reg,esi`; the
// mask-group hand-over (`RefLayerMask` set) re-reads every member from `this` after each store (no locals); the inner test
// `NextLayerMask && NextLayerMask != NextPrimitive` gives `cmp eax,esi ; je ; mov edx,[ecx+38h] ; cmp eax,edx`.
struct CEngineSceneGridCell {
    unsigned char _pad[0x34];
    long          NoPrimitives;                       // +0x34
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    CEngineSceneGridCell*          GridCell;          // +0x20
    unsigned char _pad24[0xc];
    unsigned long                  RenderLayerMask;   // +0x30
    unsigned char                  Enable : 1;        // +0x34
    unsigned char                  NonPersistent : 1;
    unsigned char                  OverrideFadeDistance : 1;
    unsigned char                  PostRenderEnable : 1;
    unsigned char _pad35[3];
    CEngineInternalPrimitiveBase*  NextPrimitive;     // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;      // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;     // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;      // +0x44
    void RemoveFromList();
};

void CEngineInternalPrimitiveBase::RemoveFromList()
{
    if (!NonPersistent && GridCell) {
        --GridCell->NoPrimitives;
        GridCell = 0;
    }
    if (RefPrimitive)
        *RefPrimitive = NextPrimitive;
    if (NextPrimitive)
        NextPrimitive->RefPrimitive = RefPrimitive;
    if (RefLayerMask) {
        *RefLayerMask = NextPrimitive;
        if (NextPrimitive) {
            NextPrimitive->RefLayerMask = RefLayerMask;
            if (NextLayerMask && NextLayerMask != NextPrimitive) {
                NextPrimitive->NextLayerMask = NextLayerMask;
                NextLayerMask->RefLayerMask = &NextPrimitive->NextLayerMask;
            }
        }
    }
    NextPrimitive = 0;
    RefPrimitive = 0;
    NextLayerMask = 0;
    RefLayerMask = 0;
}