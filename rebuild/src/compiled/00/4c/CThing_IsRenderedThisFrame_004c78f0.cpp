// CThing::IsRenderedThisFrame() -- a drawable thing with a graphic appearance counts as rendered when the appearance has primitive-update reasons pending or resends its primitives every render frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C78F0, 50 bytes. Callees: CThing virtual slot 27 (+0x6c, DrawIsDrawable).
// Shape: PTCGraphicAppearance is retail +0x64 (PDB +0x74). On the appearance (retail 0x18 bytes narrower than the
// PDB) +0x58 is the UpdatePrimitiveReasons byte and +0x59 bit 2 ResendPrimsEveryRenderFrame. The inline
// appearance query returns a LONG (`movzx eax,cl ; test eax,eax`): 1 when no reason is pending but the
// resend flag is set, else the reasons byte; the caller's `&&` chain gives the full-register 0/1 result.
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

class CThing { public:
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
    bool IsRenderedThisFrame();
};

bool CThing::IsRenderedThisFrame()
{
    return DrawIsDrawable() && PTCGraphicAppearance && PTCGraphicAppearance->WasRendered();
}