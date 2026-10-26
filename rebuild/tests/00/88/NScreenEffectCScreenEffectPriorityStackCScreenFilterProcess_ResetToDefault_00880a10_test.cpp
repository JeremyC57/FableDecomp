// Behaviour test for 0x00880A10 NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess::ResetToDefault
#include <cstdio>
class CRGBFloatColour { public: float R, G, B, A; };
class C3DVector { public: float X, Y, Z; void Initialise(float x, float y, float z) { X = x; Y = y; Z = z; } };
struct CScreenFilterParams { float Alpha, Saturation, Contrast, Brightness; CRGBFloatColour Tint; };
struct CRadialBlurParams { CRGBFloatColour ThresholdColour, IntensityColour; float Length, Fade, Range, Twist; };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; long AreaTrackPos, BlurTrackPos; };
struct CRadialBlurEntry { long id; CRadialBlurParams Params; CRadialBlurPositionParams PosParams; };
struct node { node* _M_next; node* _M_prev; CRadialBlurEntry _M_data; };
struct CRadialBlurList {
    node* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CRadialBlurEntry& front() { return _M_node->_M_next->_M_data; }
};
struct CScreenEffectPriorityStack {
    CRadialBlurList RadialParamStack[3];
    CRadialBlurParams CurrentRadialBlurParams;
    CRadialBlurPositionParams CurrentRadialBlurPosParams;
    CRGBFloatColour CurrentDisplacementParam;
    CScreenFilterParams CurrentScreenFilterParam;
};
static void Fill(void* p, unsigned long n) { unsigned char* b = (unsigned char*)p; for (unsigned long i = 0; i < n; ++i) b[i] = 0x41; }
struct CScreenFilterProcess {
    CScreenEffectPriorityStack* TheStack;
    void ResetToDefault()
    {
        CScreenFilterParams& params = TheStack->CurrentScreenFilterParam;
        params.Alpha = params.Contrast = params.Brightness = 0.0f;
        params.Saturation = 1.0f;
        params.Tint.R = 1.0f; params.Tint.G = 1.0f; params.Tint.B = 1.0f; params.Tint.A = 1.0f;
    }
};
int main()
{
    CScreenEffectPriorityStack stack;
    Fill(&stack, sizeof(stack));
    CScreenFilterProcess p; p.TheStack = &stack;
    p.ResetToDefault();
    CScreenFilterParams& f = stack.CurrentScreenFilterParam;
    int ok = f.Alpha == 0.0f && f.Saturation == 1.0f && f.Contrast == 0.0f && f.Brightness == 0.0f
        && f.Tint.R == 1.0f && f.Tint.G == 1.0f && f.Tint.B == 1.0f && f.Tint.A == 1.0f
        && stack.CurrentDisplacementParam.R != 0.0f;
    if (!ok) { std::printf("00880a10_TEST FAIL\n"); return 1; }
    std::printf("00880a10_TEST PASS\n");
    return 0;
}