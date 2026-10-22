struct CEngineInternalPrimitiveBase
{
    char pad0[0x20];
    void* GridCell;      // +0x20
    char pad1[0x14];
    CEngineInternalPrimitiveBase* NextPrimitive; // +0x38
    void* RefPrimitive;      // +0x3c
    void* NextLayerMask;      // +0x40
    void* RefLayerMask;      // +0x44
};

void __fastcall ClearPrimitiveList(CEngineInternalPrimitiveBase* self)
{
    CEngineInternalPrimitiveBase* p = self;
    while (p != 0)
    {
        CEngineInternalPrimitiveBase* next = p->NextPrimitive;
        p->GridCell = 0;
        p->NextPrimitive = 0;
        p->RefPrimitive = 0;
        p->NextLayerMask = 0;
        p->RefLayerMask = 0;
        p = next;
    }
}