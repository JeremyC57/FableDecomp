// CThing::IsFrameUpdated() -- a thing is updated this frame when it has no base def, its def says always frame-update, it was rendered this frame, or a frame update is being forced
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7BF0, 74 bytes. CThing vtable 0x01239ABC slot 44.
// Callees: CThing virtual slot 27 (+0x6c, DrawIsDrawable) through the inlined IsRenderedThisFrame (0x004C78F0).
// Shape: PBaseDef retail +0x70, def +0x41 bit 1 AlwaysFrameUpdate (`test byte ptr [eax+41h],2`); the inlined
// IsRenderedThisFrame keeps its long-returning appearance query (`movzx eax,cl ; test eax,eax`); ForceFrameUpdate
// is bit 3 of +0x92. One `||` chain, full-register 0/1 result.
struct CTCGraphicAppearance {
    unsigned char _pad00[0x58];
    unsigned char UpdatePrimitiveReasons;            // +0x58 (PDB +0x70)
    unsigned char SelfIlluminating : 1;              // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;   // bit 1
    unsigned char ResendPrimsEveryRenderFrame : 1;   // bit 2
    long WasRendered() const
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
};

struct CThingBaseDef {
    unsigned char _pad00[0x41];
    unsigned char AvailableInEditor : 1;             // +0x41 bit 0
    unsigned char AlwaysFrameUpdate : 1;             // bit 1
};

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool DrawIsDrawable();                   // slot 27 (+0x6c)
    unsigned char          _pad04[0x64 - 0x04];
    CTCGraphicAppearance*  PTCGraphicAppearance;     // +0x64 (PDB +0x74)
    unsigned char          _pad68[0x70 - 0x68];
    CThingBaseDef*         PBaseDef;                 // +0x70 CDefPointer<CThingBaseDef const> (PDB +0x80)
    unsigned char          _pad74[0x92 - 0x74];
    unsigned char          DeletedOnOwnerLevelOutOfScope : 1; // +0x92 bit 0 (PDB +0xa2)
    unsigned char          Drawable : 1;             // bit 1
    unsigned char          Dying : 1;                // bit 2
    unsigned char          ForceFrameUpdate : 1;     // bit 3
    bool IsRenderedThisFrame()
    {
        return DrawIsDrawable() && PTCGraphicAppearance && PTCGraphicAppearance->WasRendered();
    }
    bool IsFrameUpdated();
};

bool CThing::IsFrameUpdated()
{
    return !PBaseDef || PBaseDef->AlwaysFrameUpdate || IsRenderedThisFrame() || ForceFrameUpdate;
}