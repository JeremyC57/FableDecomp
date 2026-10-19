// NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess::Process(float weight, const CStackEntry* entry) -- blend the stack's current displacement colour towards the entry's colour by `weight`
// Leaf: ?Process@CDisplacementProcess@CScreenEffectPriorityStack@NScreenEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880D50, length 174 bytes (+ int3 padding), `ret 8`. No callees;
// the 1.0f constant @ 0x0122DED8.
// Layout: CDisplacementProcess {vptr, Owner +4} (PDB; vtable 0x01278014, built in the landed
// CScreenEffectPriorityStack::Process 0x00880E50). The owner's CurrentDisplacementParam (CRGBFloatColour) sits at retail
// +0x84 (PDB +0xbc: the three-list arrays and the sets are narrower in retail); the entry is a
// CDisplacementMonochromeEffectColourEntry (CStackEntry 0x10 bytes + CRGBFloatColour Colour +0x10).
// Shape: `1 - weight` in a named float local (computed first and kept on the FPU), then `current * remaining +
// colour * weight` with inline by-value colour operators that return through the 4-float constructor (a named result
// local adds memberwise copies): the right product is formed first (R kept on the FPU, G/B/A spilled), then the left
// one; the sum goes through a frame temporary and is copied into the member with integer moves.
namespace NScreenEffect {

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float scale) const { return CRGBFloatColour(R * scale, G * scale, B * scale, A * scale); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};

class CScreenEffectPriorityStack {
public:
    struct CStackEntry {
        void* __vftable;                           // +0x00
        long curdurationcount;                     // +0x04
        long durationcount;                        // +0x08
        bool fadingout;                            // +0x0c
    };
    struct CDisplacementMonochromeEffectColourEntry : CStackEntry {
        CRGBFloatColour Colour;                    // +0x10
    };
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void Unused() = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CDisplacementProcess : public ICustomProcessInterface {
    public:
        CDisplacementProcess(CScreenEffectPriorityStack* owner) : Owner(owner) {}
        virtual ~CDisplacementProcess() {}
        virtual void ResetToDefault();
        virtual void Unused();
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* Owner;         // +0x04
    };

    unsigned char _pad[0x84];
    CRGBFloatColour CurrentDisplacementParam;      // +0x84
};

void CScreenEffectPriorityStack::CDisplacementProcess::Process(float weight, const CStackEntry* entry)
{
    const CDisplacementMonochromeEffectColourEntry* colourEntry = static_cast<const CDisplacementMonochromeEffectColourEntry*>(entry);
    float remaining = 1.0f - weight;
    Owner->CurrentDisplacementParam = Owner->CurrentDisplacementParam * remaining + colourEntry->Colour * weight;
}

}
