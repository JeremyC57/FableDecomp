// CTCDCameraPoint::Persist(CPersistContext* context, bool flag) -- unless the context is in mode 1 or 3, transfer the camera point's CutInto / CutOutOf / TestAngleBeforeActivation flags (default false) and its PointType (default 0)
// Leaf: Persist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E88F0, length 138 bytes (`ret 8`). Vtable 0x0123EC3C (CTCDCameraPoint) slot 1, the
// persist hook (catalog "vfunc_1").
// Callees: CPersistContext::Transfer<bool>(name, value, default) @ 0x004045C0 (x3), CPersistContext::Transfer<long> @ 0x00410BE0.
// Strings: "CutInto" 0x01269C70, "CutOutOf" 0x01269C64, "TestAngleBeforeActivation" 0x01269C48, "PointType" 0x01270C20.
// Layout: Ego_r.pdb CTCDCameraPoint (0x14): CutInto +0xC, CutOutOf +0xD, TestAngleBeforeActivation +0xE, Type +0x10
// (ECameraPointType). The context's mode word sits at +0x18.
// Shape: each `false` / `0` default is a fresh temporary bound to the const reference and homed in the dead first-argument
// slot (the context is already in edi); the enum goes through a long local transferred and then stored.
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0; long 0x00410BE0
};
class CTCDCameraPoint { public:
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);  // slot 1
    void* Owner;                                // +0x04
    unsigned char _pad08[4];
    bool CutInto;                               // +0x0c
    bool CutOutOf;                              // +0x0d
    bool TestAngleBeforeActivation;             // +0x0e
    long Type;                                  // +0x10 ECameraPointType
};

void CTCDCameraPoint::Persist(CPersistContext* context, bool flag)
{
    if (context->Mode != 1 && context->Mode != 3) {
        context->Transfer("CutInto", CutInto, false);
        context->Transfer("CutOutOf", CutOutOf, false);
        context->Transfer("TestAngleBeforeActivation", TestAngleBeforeActivation, false);
        long type;
        context->Transfer<long>("PointType", type, 0);
        Type = type;
    }
}