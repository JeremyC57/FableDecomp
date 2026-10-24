// NUISystem::CComponent::ChangeColourDelta(const CRGBColour& delta, float time, bool flags) -- fade to the current colour plus a per-channel delta
// Grade: DIFFER(105v104) -- retail builds the sum in place in the push-ecx frame (byte copy of Colour B/A, R/G summed directly); this form copies the operator+ result once more (dword copy + shr extracts)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005305A0, 104 bytes. Vtable 0x0122F5D4 slot 39 (CSprite, from CComponent);
// calls ChangeColour (vslot 38, row 0x0052EC60) like the landed ChangeZoomDelta 0x0052EC20 calls ChangeZoom.
// Leaf: ChangeColourDelta
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour& operator+=(const CRGBColour& o) { R += o.R; G += o.G; B += o.B; A += o.A; return *this; }
    CRGBColour operator+(const CRGBColour& o) const { CRGBColour c = *this; c += o; return c; }
};
struct CComponent_Retail {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool flags);        // slot 38
    virtual void ChangeColourDelta(const CRGBColour& delta, float time, bool flags);    // slot 39
    unsigned char _pad_0x04[0x80];          // +0x04
    CRGBColour    Colour;                   // +0x84
};
void CComponent_Retail::ChangeColourDelta(const CRGBColour& delta, float time, bool flags)
{
    ChangeColour(Colour + delta, time, flags);
}
