// CThing::DrawPreparePrimitivesForRendering(CRenderContext* context) -- for a drawable thing that is not in limbo, let the thing's driver component prepare its primitives; true when the thing is drawn
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7540, 50 bytes. CThing vtable 0x01239ABC slot 36.
// Callees: CThing virtual slot 27 (+0x6c, DrawIsDrawable 0x004C7790), CTCDriverBase virtual slot 11 (+0x2c).
// Shape: InLimbo is bit 5 of the +0x91 flag byte (PDB +0xa1; `test byte ptr [esi+91h],20h`); PTCDriver is
// retail +0x5c (PDB +0x6c), null-tested before the call through an inline accessor
// (the direct member form loads it into eax and copies it to ecx). Two `xor al,al` false exits share one tail.
struct CRenderContext;

class CTCDriverBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual void DrawPreparePrimitivesForRendering(CRenderContext* context); // slot 11 (+0x2c)
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
    unsigned char   _pad04[0x5c - 0x04];
    CTCDriverBase*  PTCDriver;                       // +0x5c (PDB +0x6c)
    unsigned char   _pad60[0x91 - 0x60];
    unsigned char   Dead : 1;                        // +0x91 bit 0 (PDB +0xa1)
    unsigned char   CreatedByOwnerThingCreation : 1; // bit 1
    unsigned char   IncludeInThingScans : 1;         // bit 2
    unsigned char   Activated : 1;                   // bit 3
    unsigned char   GloballyLoaded : 1;              // bit 4
    unsigned char   InLimbo : 1;                     // bit 5
    CTCDriverBase* GetTCDriver() const { return PTCDriver; }
    bool DrawPreparePrimitivesForRendering(CRenderContext* context);
};

bool CThing::DrawPreparePrimitivesForRendering(CRenderContext* context)
{
    if (InLimbo || !DrawIsDrawable())
        return false;
    if (CTCDriverBase* driver = GetTCDriver())
        driver->DrawPreparePrimitivesForRendering(context);
    return true;
}