// CThingMarker::DrawIsDrawable() -- drawable when the CThing base test passes AND (the display engine's slot-9 flag is set OR the marker has a graphic-appearance component)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D100, length 52 bytes (vtable 0x01245524 slot 27).
// Callees: CThing::DrawIsDrawable() @ 0x004C7790 (non-virtual), GetDisplayEngine() @ 0x004C7A70, CDisplayEngine virtual slot 9 (+0x24).
// Shape: the early `return false` gives `xor al,al`, the `||` result in the success arm is a full
// register 0/1 (`xor eax,eax` / `mov eax,1`); PTCGraphicAppearance is CThing's +0x64.
class CDisplayEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool IsDrawingEnabled();                 // slot 9 (+0x24)
};
struct CTCGraphicAppearance;
class CThing { public:
    virtual void v00();
    unsigned char         _pad04[0x60];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70
    virtual bool DrawIsDrawable();                   // 0x004C7790 (called non-virtually)
};
class CThingMarker : public CThing { public:
    virtual bool DrawIsDrawable();
};

bool CThingMarker::DrawIsDrawable()
{
    if (CThing::DrawIsDrawable())
        return GetDisplayEngine()->IsDrawingEnabled() || PTCGraphicAppearance != 0;
    return false;
}