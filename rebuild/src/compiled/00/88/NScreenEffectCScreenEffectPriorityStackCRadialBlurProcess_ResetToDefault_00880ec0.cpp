// NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::ResetToDefault() -- zero the stack's current radial-blur parameters and both centres, then take the position parameters of the top entry of the highest-priority non-empty radial-blur list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880EC0, 138 bytes (row padded to 144). Virtual slot 1 of CRadialBlurProcess's
// vtable 0x0127803C; FableWin.pdb lists ?ResetToDefault@CRadialBlurProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXXZ.
// No callees.
// Layout: retail CScreenEffectPriorityStack (PDB layout with 4-byte STLport lists and 12-byte sets): RadialParamStack[3] +0x04,
// RadialBlurKeys +0x10, CurrentRadialBlurParams +0x1c, CurrentRadialBlurPosParams +0x4c; a list node holds next/prev then the
// CRadialBlurEntry, so the entry's PosParams sit at node +0x48.
// Shape: every store goes through `TheStack->` (re-read after each store: VC7.1 assumes the float stores may alias the member);
// the two colours through the inline CRGBFloatColour::Initialise(r,g,b,a) (stores folded into the displacement, then a dead
// `add eax,1Ch`), the two centres through the inline C3DVector::Initialise(x,y,z) (address formed first); assigning C3DVector
// temporaries leaves dead frame stores. The search loop reuses the zero register as the index; the found list's front entry's
// PosParams is a 32-byte POD copied with `rep movsd`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Initialise(float x, float y, float z) { X = x; Y = y; Z = z; }
};
struct CRGBFloatColour {
    float R, G, B, A;
    void Initialise(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};
namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CRadialBlurParams {                     // CEnvironmentSpellEffectInterface::CRadialBlurParams (0x30 bytes)
        CRGBFloatColour ThresholdColour;           // +0x00
        CRGBFloatColour IntensityColour;           // +0x10
        float           Length;                    // +0x20
        float           Fade;                      // +0x24
        float           Range;                     // +0x28
        float           Twist;                     // +0x2c
    };
    struct CRadialBlurPositionParams {             // CEnvironmentSpellEffectInterface::CRadialBlurPositionParams (0x20 bytes)
        C3DVector AreaCenter;                      // +0x00
        C3DVector BlurCenter;                      // +0x0c
        long      AreaTrackPos;                    // +0x18 ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION
        long      BlurTrackPos;                    // +0x1c
    };
    struct CStackEntry {                           // 0x10 bytes
        void* __vftable;                           // +0x00
        long  curdurationcount;                    // +0x04
        long  durationcount;                       // +0x08
        bool  fadingout;                           // +0x0c
    };
    struct CRadialBlurEntry : CStackEntry {
        CRadialBlurParams         Params;          // +0x10
        CRadialBlurPositionParams PosParams;       // +0x40
    };
    struct CRadialBlurList {                       // STLport list<CRadialBlurEntry>: 4 bytes (the header node pointer)
        struct node { node* _M_next; node* _M_prev; CRadialBlurEntry _M_data; };
        node* _M_node;
        bool empty() const { return _M_node->_M_next == _M_node; }
        CRadialBlurEntry& front() { return _M_node->_M_next->_M_data; }
    };
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void OnErase(CStackEntry* entry) = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CRadialBlurProcess : public ICustomProcessInterface {
    public:
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };
    void*                     __vftable;                   // +0x00
    CRadialBlurList           RadialParamStack[3];         // +0x04
    unsigned char             RadialBlurKeys[12];          // +0x10 set<CStackEntry*> (retail 12 bytes)
    CRadialBlurParams         CurrentRadialBlurParams;     // +0x1c
    CRadialBlurPositionParams CurrentRadialBlurPosParams;  // +0x4c
};
void CScreenEffectPriorityStack::CRadialBlurProcess::ResetToDefault()
{
    TheStack->CurrentRadialBlurParams.ThresholdColour.Initialise(0.0f, 0.0f, 0.0f, 0.0f);
    TheStack->CurrentRadialBlurParams.IntensityColour.Initialise(0.0f, 0.0f, 0.0f, 0.0f);
    TheStack->CurrentRadialBlurParams.Length = 0.0f;
    TheStack->CurrentRadialBlurParams.Fade = 0.0f;
    TheStack->CurrentRadialBlurParams.Range = 0.0f;
    TheStack->CurrentRadialBlurParams.Twist = 0.0f;
    TheStack->CurrentRadialBlurPosParams.AreaCenter.Initialise(0.0f, 0.0f, 0.0f);
    TheStack->CurrentRadialBlurPosParams.BlurCenter.Initialise(0.0f, 0.0f, 0.0f);
    for (long i = 0; i < 3; ++i) {
        if (!TheStack->RadialParamStack[i].empty()) {
            TheStack->CurrentRadialBlurPosParams = TheStack->RadialParamStack[i].front().PosParams;
            return;
        }
    }
}
}