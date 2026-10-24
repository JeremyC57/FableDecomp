// NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess::ResetToDefault() -- zero the stack's current displacement colour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008809F0, 22 bytes (row padded to 32). Virtual slot 1 of CDisplacementProcess's
// vtable 0x01278014; FableWin.pdb lists ?ResetToDefault@CDisplacementProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXXZ.
// Layout: retail CScreenEffectPriorityStack = PDB layout with 4-byte STLport lists and 12-byte sets: RadialParamStack[3] +0x04,
// RadialBlurKeys +0x10, CurrentRadialBlurParams +0x1c, CurrentRadialBlurPosParams +0x4c, DisplacementParamStack[3] +0x6c,
// DisplacementKeys +0x78, CurrentDisplacementParam +0x84 (as the landed CDisplacementProcess::Process 0x00880D50),
// ScreenFilterParamStack[3] +0x94, ScreenFilterKeys +0xa0, CurrentScreenFilterParam +0xac. Process objects: {vptr, TheStack +4}.
// Shape: component stores through a reference to the member (`add eax,84h` once), R,G,B,A from one zero register; assigning
// a CRGBFloatColour(0,0,0,0) temporary instead builds it in a 16-byte frame and copies it word by word.
namespace NScreenEffect {

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};

class CScreenEffectPriorityStack {
public:
    struct CStackEntry;
    struct CScreenFilterParams {                   // CEnvironmentSpellEffectInterface::CScreenFilterParams (32 bytes)
        float           Alpha;                     // +0x00
        float           Saturation;                // +0x04
        float           Contrast;                  // +0x08
        float           Brightness;                // +0x0c
        CRGBFloatColour Tint;                      // +0x10
    };
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void OnErase(CStackEntry* entry) = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CDisplacementProcess : public ICustomProcessInterface {
    public:
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };
    class CScreenFilterProcess : public ICustomProcessInterface {
    public:
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };

    void*               __vftable;                 // +0x00
    unsigned char       _radial04[0x84 - 0x04];    // RadialParamStack[3], RadialBlurKeys, CurrentRadialBlurParams/PosParams, DisplacementParamStack[3], DisplacementKeys
    CRGBFloatColour     CurrentDisplacementParam;  // +0x84
    unsigned char       _filter94[0xac - 0x94];    // ScreenFilterParamStack[3], ScreenFilterKeys
    CScreenFilterParams CurrentScreenFilterParam;  // +0xac
};

void CScreenEffectPriorityStack::CDisplacementProcess::ResetToDefault()
{
    CRGBFloatColour& colour = TheStack->CurrentDisplacementParam;
    colour.R = 0.0f;
    colour.G = 0.0f;
    colour.B = 0.0f;
    colour.A = 0.0f;
}

}