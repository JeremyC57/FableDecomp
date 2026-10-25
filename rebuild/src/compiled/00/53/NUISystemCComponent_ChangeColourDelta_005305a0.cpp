// NUISystem::CComponent::ChangeColourDelta(const CRGBColour& delta, float time, bool flags) -- fade to the current colour plus a per-channel delta
// Leaf: ?ChangeColourDelta@CComponent@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005305A0, 104 bytes. Vtable 0x0122F5D4 slot 39 (CSprite, inherited from CComponent);
//   calls ChangeColour (vslot 38, +0x98, row 0x0052EC60) like the landed ChangeZoomDelta 0x0052EC20 calls ChangeZoom.
// Identity: Ego_r.pdb `?ChangeColourDelta@CComponent@NUISystem@@UAEXABVCRGBColour@@M_N@Z` (the object symbol here).
// Layout: CRGBColour = the union's channel view B +0, G +1, R +2, A +3 (struct_layouts_egor.tsv CRGBColour_u_0_s_0);
//   CComponent::Colour at retail +0x84.
// Shape (agent902, pass 2): one colour local in the push-ecx slot -- `CRGBColour colour = Colour; colour += delta;
//   ChangeColour(colour, time, flags);` -- with the landed CRGBColour copy constructor (`Set(other.R, other.G, other.B,
//   other.A)`, Set storing R, G, B, A). VC7.1 then forwards the R/G copies into the sums and stores/reloads B and A, as
//   retail. An `operator+` temporary (agent901: implicit dword copy + shr extracts, 105v104) or a memberwise
//   B,G,R,A copy constructor (107 / 99) build the sum differently.
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    CRGBColour& operator+=(const CRGBColour& o) { R += o.R; G += o.G; B += o.B; A += o.A; return *this; }
};

namespace NUISystem {

class CComponent {
public:
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
    virtual void ChangeColour(const CRGBColour& colour, float time, bool flags);        // slot 38 (+0x98)
    virtual void ChangeColourDelta(const CRGBColour& delta, float time, bool flags);    // slot 39
    unsigned char _pad_0x04[0x80];          // +0x04
    CRGBColour    Colour;                   // +0x84
};

void CComponent::ChangeColourDelta(const CRGBColour& delta, float time, bool flags)
{
    CRGBColour colour = Colour;
    colour += delta;
    ChangeColour(colour, time, flags);
}

}