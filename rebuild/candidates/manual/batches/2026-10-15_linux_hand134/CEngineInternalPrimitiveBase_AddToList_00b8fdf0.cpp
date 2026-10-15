// CEngineInternalPrimitiveBase::AddToList(CEngineInternalPrimitiveBase** list) -- links the primitive into a render list grouped by layer mask: behind the first entry with the same mask, else at the head as a new mask group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8FDF0, length 106 bytes. No callees.
// Layout (PDB names, engine/CEngineInternalPrimitiveBase.h): RenderLayerMask +0x30, NextPrimitive +0x38 / RefPrimitive +0x3C (the
// primitive chain, Ref = address of the pointer that points at us), NextLayerMask +0x40 / RefLayerMask +0x44 (the chain of mask-group
// heads). The header types Ref* as void* and keeps no methods, so the class is modelled locally with the real pointer types.
// Shape: the mask is read into a local first (`mov esi,[ecx+30h]` before the argument); the walk variable is declared in the `for`
// and the head is re-read as `*list` afterwards (CSE'd with the walk's first load: the head rides in edx, the walker in eax -- a
// `first` local declared before the loop swaps them); the found path inserts after the group head and leaves the mask links null.
class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    unsigned char _pad04[0x2c];
    unsigned long                  RenderLayerMask;   // +0x30
    unsigned char                  _pad34[4];
    CEngineInternalPrimitiveBase*  NextPrimitive;     // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;      // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;     // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;      // +0x44
    void AddToList(CEngineInternalPrimitiveBase** list);
};

void CEngineInternalPrimitiveBase::AddToList(CEngineInternalPrimitiveBase** list)
{
    unsigned long mask = RenderLayerMask;
    for (CEngineInternalPrimitiveBase* p = *list; p; p = p->NextLayerMask) {
        if (p->RenderLayerMask == mask) {
            NextPrimitive = p->NextPrimitive;
            RefPrimitive = &p->NextPrimitive;
            p->NextPrimitive = this;
            if (NextPrimitive)
                NextPrimitive->RefPrimitive = &NextPrimitive;
            NextLayerMask = 0;
            RefLayerMask = 0;
            return;
        }
    }
    CEngineInternalPrimitiveBase* first = *list;
    NextPrimitive = first;
    NextLayerMask = first;
    if (first) {
        first->RefPrimitive = &NextPrimitive;
        first->RefLayerMask = &NextLayerMask;
    }
    RefPrimitive = list;
    RefLayerMask = list;
    *list = this;
}
