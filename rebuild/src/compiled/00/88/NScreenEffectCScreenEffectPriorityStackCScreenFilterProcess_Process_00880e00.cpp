// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess::Process(float weight, const CStackEntry* entry) -- move the stack's current screen filter towards the entry's filter by `weight`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880E00, 65 bytes (row padded to 80), `ret 8`. Virtual slot 3 of
// CScreenFilterProcess's vtable 0x01278028; FableWin.pdb lists ?Process@CScreenFilterProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXMPBUCStackEntry@23@@Z.
// Callees: CScreenFilterParams::Interpolate(const CScreenFilterParams& to, float t) const @ 0x00882FE0 (PDB name LerpTo; thiscall,
// hidden result pointer, clamps t to [0,1]; declared exactly as in the landed GetRenderFrameInterpolatedScreenFilterParameters
// 0x008859F0 so the address keeps one symbol; manifest label "InterpolateRadialBlurParams" is propagated).
// Layout: CurrentScreenFilterParam at retail +0xac of the stack (see the CScreenFilterProcess::ResetToDefault row 0x00880A10);
// the entry is a CScreenFilterEntry (CStackEntry 0x10 bytes, then its CScreenFilterParams at +0x10).
// Shape: the call result assigned straight to the member: the 0x20-byte POD (implicit copy) moves as an 8-dword `rep movsd`
// from the pointer the call returns in eax into the re-read member address.
// Leaf: ?Process@CScreenFilterProcess
class CRGBFloatColour { public:
    float R, G, B, A;
};

struct CScreenFilterParams {                       // CEnvironmentSpellEffectInterface::CScreenFilterParams (32 bytes)
    float           Alpha;                         // +0x00
    float           Saturation;                    // +0x04
    float           Contrast;                      // +0x08
    float           Brightness;                    // +0x0c
    CRGBFloatColour Tint;                          // +0x10
    CScreenFilterParams Interpolate(const CScreenFilterParams& to, float t) const;   // 0x00882FE0
};

namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry {
        void* __vftable;                           // +0x00
        long curdurationcount;                     // +0x04
        long durationcount;                        // +0x08
        bool fadingout;                            // +0x0c
    };
    struct CScreenFilterEntry : CStackEntry {
        CScreenFilterParams Params;                // +0x10
    };
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void OnErase(CStackEntry* entry) = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CScreenFilterProcess : public ICustomProcessInterface {
    public:
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };

    void*               __vftable;                 // +0x00
    unsigned char       _stacks04[0xac - 0x04];    // radial/displacement stacks and keys, ScreenFilterParamStack, ScreenFilterKeys
    CScreenFilterParams CurrentScreenFilterParam;  // +0xac
};

void CScreenEffectPriorityStack::CScreenFilterProcess::Process(float weight, const CStackEntry* entry)
{
    const CScreenFilterEntry* filterEntry = static_cast<const CScreenFilterEntry*>(entry);
    TheStack->CurrentScreenFilterParam = TheStack->CurrentScreenFilterParam.Interpolate(filterEntry->Params, weight);
}
}