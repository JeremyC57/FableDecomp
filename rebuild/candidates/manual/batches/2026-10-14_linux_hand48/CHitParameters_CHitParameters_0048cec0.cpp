// CHitParameters::~CHitParameters() -- destroy the intelligent pointer at +0x38 (0x00A01C10), release the counted pointer at +0x30, destroy the intelligent pointer at +0x28, then the base destructor (0x00A0D3D0, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048CEC0, length 51 bytes.
// The base label "~CAInputProcess" is propagated. Shape: no vtable store (the class is not polymorphic here), members last-to-first.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};
struct CBaseIntelligentPointer {                   // 8 bytes
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CHitParametersBase_00a0d3d0 {               // 0x28 bytes
    unsigned char _body[0x28];
    ~CHitParametersBase_00a0d3d0();                // 0x00A0D3D0
};
struct CHitParameters : CHitParametersBase_00a0d3d0 {
    CBaseIntelligentPointer m_pointer28;           // +0x28
    CIVCountedPointer       m_counted30;           // +0x30
    unsigned char           _pad34[4];
    CBaseIntelligentPointer m_pointer38;           // +0x38
    ~CHitParameters();
};

CHitParameters::~CHitParameters()
{
}
