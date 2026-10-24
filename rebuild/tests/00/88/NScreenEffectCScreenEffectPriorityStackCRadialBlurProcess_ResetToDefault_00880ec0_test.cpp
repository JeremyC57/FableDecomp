// Behaviour test for 0x00880EC0 NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::ResetToDefault
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
struct CRadialBlurProcess {
    CScreenEffectPriorityStack* TheStack;
    void ResetToDefault()
    {
        CRadialBlurParams& p = TheStack->CurrentRadialBlurParams;
        p.ThresholdColour.R = 0.0f; p.ThresholdColour.G = 0.0f; p.ThresholdColour.B = 0.0f; p.ThresholdColour.A = 0.0f;
        p.IntensityColour.R = 0.0f; p.IntensityColour.G = 0.0f; p.IntensityColour.B = 0.0f; p.IntensityColour.A = 0.0f;
        p.Length = 0.0f; p.Fade = 0.0f; p.Range = 0.0f; p.Twist = 0.0f;
        TheStack->CurrentRadialBlurPosParams.AreaCenter.Initialise(0.0f, 0.0f, 0.0f);
        TheStack->CurrentRadialBlurPosParams.BlurCenter.Initialise(0.0f, 0.0f, 0.0f);
        for (long i = 0; i < 3; ++i) {
            if (!TheStack->RadialParamStack[i].empty()) {
                TheStack->CurrentRadialBlurPosParams = TheStack->RadialParamStack[i].front().PosParams;
                return;
            }
        }
    }
};
int main()
{
    CScreenEffectPriorityStack stack;
    Fill(&stack, sizeof(stack));
    node heads[3];
    node entry;
    long k;
    for (k = 0; k < 3; ++k) { heads[k]._M_next = &heads[k]; heads[k]._M_prev = &heads[k]; stack.RadialParamStack[k]._M_node = &heads[k]; }
    stack.CurrentRadialBlurPosParams.AreaTrackPos = 7;
    CRadialBlurProcess p; p.TheStack = &stack;
    p.ResetToDefault();
    int ok = stack.CurrentRadialBlurParams.Twist == 0.0f && stack.CurrentRadialBlurParams.ThresholdColour.A == 0.0f
        && stack.CurrentRadialBlurPosParams.AreaCenter.X == 0.0f && stack.CurrentRadialBlurPosParams.BlurCenter.Z == 0.0f
        && stack.CurrentRadialBlurPosParams.AreaTrackPos == 7;
    entry._M_data.PosParams.AreaCenter.Initialise(1.0f, 2.0f, 3.0f);
    entry._M_data.PosParams.BlurCenter.Initialise(4.0f, 5.0f, 6.0f);
    entry._M_data.PosParams.AreaTrackPos = 2; entry._M_data.PosParams.BlurTrackPos = 3;
    heads[1]._M_next = &entry; entry._M_next = &heads[1];
    p.ResetToDefault();
    ok = ok && stack.CurrentRadialBlurPosParams.AreaCenter.Y == 2.0f && stack.CurrentRadialBlurPosParams.BlurCenter.Z == 6.0f
        && stack.CurrentRadialBlurPosParams.BlurTrackPos == 3 && stack.CurrentRadialBlurParams.Length == 0.0f;
    if (!ok) { std::printf("00880ec0_TEST FAIL\n"); return 1; }
    std::printf("00880ec0_TEST PASS\n");
    return 0;
}