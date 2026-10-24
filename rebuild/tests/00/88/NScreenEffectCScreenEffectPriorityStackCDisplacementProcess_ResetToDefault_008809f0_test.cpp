// Behaviour test for 0x008809F0 NScreenEffect::CScreenEffectPriorityStack::CDisplacementProcess::ResetToDefault
#include <cstdio>
struct CRGBFloatColour { float R, G, B, A; };
struct C3DVector { float X, Y, Z; void Initialise(float x, float y, float z) { X = x; Y = y; Z = z; } };
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
struct CDisplacementProcess {
    CScreenEffectPriorityStack* TheStack;
    void ResetToDefault()
    {
        CRGBFloatColour& colour = TheStack->CurrentDisplacementParam;
        colour.R = 0.0f; colour.G = 0.0f; colour.B = 0.0f; colour.A = 0.0f;
    }
};
int main()
{
    CScreenEffectPriorityStack stack;
    Fill(&stack, sizeof(stack));
    CDisplacementProcess p; p.TheStack = &stack;
    p.ResetToDefault();
    CRGBFloatColour& c = stack.CurrentDisplacementParam;
    int ok = c.R == 0.0f && c.G == 0.0f && c.B == 0.0f && c.A == 0.0f && stack.CurrentScreenFilterParam.Alpha != 0.0f;
    if (!ok) { std::printf("008809f0_TEST FAIL\n"); return 1; }
    std::printf("008809f0_TEST PASS\n");
    return 0;
}