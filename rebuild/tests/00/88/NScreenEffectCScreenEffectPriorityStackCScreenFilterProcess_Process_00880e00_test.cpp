// Behaviour test for 0x00880E00 NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess::Process
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
static CScreenFilterParams Interpolate(const CScreenFilterParams& from, const CScreenFilterParams& to, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    CScreenFilterParams r;
    r.Alpha = from.Alpha + (to.Alpha - from.Alpha) * t;
    r.Saturation = from.Saturation + (to.Saturation - from.Saturation) * t;
    r.Contrast = from.Contrast + (to.Contrast - from.Contrast) * t;
    r.Brightness = from.Brightness + (to.Brightness - from.Brightness) * t;
    r.Tint.R = from.Tint.R + (to.Tint.R - from.Tint.R) * t;
    r.Tint.G = from.Tint.G + (to.Tint.G - from.Tint.G) * t;
    r.Tint.B = from.Tint.B + (to.Tint.B - from.Tint.B) * t;
    r.Tint.A = from.Tint.A + (to.Tint.A - from.Tint.A) * t;
    return r;
}
struct CScreenFilterEntry { long id; CScreenFilterParams Params; };
struct CScreenFilterProcess {
    CScreenEffectPriorityStack* TheStack;
    void Process(float weight, const CScreenFilterEntry* entry)
    {
        TheStack->CurrentScreenFilterParam = Interpolate(TheStack->CurrentScreenFilterParam, entry->Params, weight);
    }
};
int main()
{
    CScreenEffectPriorityStack stack;
    CScreenFilterParams& cur = stack.CurrentScreenFilterParam;
    cur.Alpha = 0.0f; cur.Saturation = 1.0f; cur.Contrast = 0.0f; cur.Brightness = 0.0f;
    cur.Tint.R = 1.0f; cur.Tint.G = 1.0f; cur.Tint.B = 1.0f; cur.Tint.A = 1.0f;
    CScreenFilterEntry e; e.id = 1;
    e.Params.Alpha = 1.0f; e.Params.Saturation = 0.0f; e.Params.Contrast = 0.5f; e.Params.Brightness = 0.25f;
    e.Params.Tint.R = 0.0f; e.Params.Tint.G = 0.5f; e.Params.Tint.B = 1.0f; e.Params.Tint.A = 1.0f;
    CScreenFilterProcess p; p.TheStack = &stack;
    p.Process(0.5f, &e);
    int ok = cur.Alpha == 0.5f && cur.Saturation == 0.5f && cur.Contrast == 0.25f && cur.Brightness == 0.125f
        && cur.Tint.R == 0.5f && cur.Tint.G == 0.75f && cur.Tint.B == 1.0f;
    p.Process(2.0f, &e);
    ok = ok && cur.Alpha == 1.0f && cur.Saturation == 0.0f;
    if (!ok) { std::printf("00880e00_TEST FAIL\n"); return 1; }
    std::printf("00880e00_TEST PASS\n");
    return 0;
}