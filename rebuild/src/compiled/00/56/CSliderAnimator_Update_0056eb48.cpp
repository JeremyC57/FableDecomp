// CSliderAnimator::Update() -- advance the slider animation: the step is |(EndValue - StartValue) / Overall| (a step of exactly -1 stops the animation), the fraction `a` grows by 1 / (15 * step) while playing and clamps to 1 (finishing it), otherwise it sits at 1 when finished or 0; the interpolated value is written as "<value><parenthesised suffix>" to the UI_LEVEL_SLIDER_NUMBER text and the UI_LEVEL_SLIDER_FILL_SPRITE is zoomed to value / MaxValue * 32 wide
// Leaf: ?Update@CSliderAnimator@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056EB48, length 305 bytes (`ret`, frame pointer + `leave`). FableWin.pdb
// CSliderAnimator (class) is 28 bytes {CComponent* pSlider, float StartValue, EndValue, MaxValue, a, Overall, bool Play, Finished};
// FableWin.pdb ?Update@CSliderAnimator@@QAEXXZ; caller CTCInventoryExperience::GlobalUpdate 0x0056FC5D (landed).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 built in the argument slot; NUISystem::CComponent::FindChild(CCharString)
// (virtual slot 3, +0x0c; vtables 0x0124608C / CText 0x01249CCC slot 3 = 0x00530780, landed); CSliderAnimator::ExtractParenthesis
// @ 0x0056EAD6 (same TU, landed); _ftol @ 0x00BFEA70; GFIntToWideString(long) @ 0x00405F00 (FableWin.pdb ?GFIntToWideString@@YI?AV
// CWideString@@J@Z) and operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (both __fastcall, hidden result in ecx);
// NUISystem::CText::SetValue(const CWideString&, bool, bool) (slot 144, +0x240 = 0x0054F410, landed); ~CWideString @ 0x0099B510;
// NUISystem::CComponent::SetZoom(const C2DVector&) (slot 11, +0x2c = 0x0052E750, landed). The other slots are unnamed placeholders.
// TU: `#pragma optimize("s", on)` + `("y", off)` like the landed sibling ExtractParenthesis (ebp frame, `leave`).
// Shape (what matched): the step is `float speed = fabs(quotient)` through <math.h>'s C++ `float fabs(float)` overload (fabsf ->
//   (float)fabs((double))): the fstp of the step then lands after `push ecx ; mov ecx,esp ; push -1` of the first by-value
//   CCharString argument, as retail. A hand-declared `(float)fabs(double)` intrinsic stores it before the argument setup. The
//   declared CCharString copy constructor makes VC build the by-value argument in place; the -1 test is written on the unnamed
//   quotient so it stays on the x87 stack (fld st(1)); `if (!Play)` first gives retail's block layout.
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <math.h>
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30 (declared so the by-value argument is built in place)
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
CWideString __fastcall GFIntToWideString(long value);                                 // 0x00405F00
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

namespace NUISystem {
class CComponent { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0x0c), 0x00530780
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void SetZoom(const C2DVector& zoom);               // slot 11 (+0x2c), 0x0052E750
};
class CText : public CComponent { public:
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
               virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
               virtual void v##n##8(); virtual void v##n##9();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    V10(02) V10(03) V10(04) V10(05) V10(06) V10(07) V10(08) V10(09) V10(10) V10(11) V10(12) V10(13)
#undef V10
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void SetValue(const CWideString& text, bool lookUpTag, bool keepSize);   // slot 144 (+0x240), 0x0054F410
};
}

class CSliderAnimator {
public:
    NUISystem::CComponent* pSlider;   // +0x00
    float StartValue;                 // +0x04
    float EndValue;                   // +0x08
    float MaxValue;                   // +0x0c
    float a;                          // +0x10
    float Overall;                    // +0x14
    bool  Play;                       // +0x18
    bool  Finished;                   // +0x19
    CWideString ExtractParenthesis(NUISystem::CText* text);   // 0x0056EAD6
    void Update();
};


void CSliderAnimator::Update()
{
    if ((EndValue - StartValue) / Overall == -1.0f) {
        Play = false;
        Finished = false;
        return;
    }
    float speed = fabs((EndValue - StartValue) / Overall);
    NUISystem::CComponent* fill = pSlider->FindChild(CCharString("UI_LEVEL_SLIDER_FILL_SPRITE"));
    NUISystem::CText* number = (NUISystem::CText*)pSlider->FindChild(CCharString("UI_LEVEL_SLIDER_NUMBER"));
    if (!Play)
        a = Finished ? 1.0f : 0.0f;
    else {
        a += 1.0f / (speed * 15.0f);
        if (a > 1.0f) {
            Play = false;
            Finished = true;
            a = 1.0f;
        }
    }
    long value = (long)((EndValue - StartValue) * a + StartValue);
    CWideString suffix = ExtractParenthesis(number);
    number->SetValue(GFIntToWideString(value) + suffix, true, true);
    fill->SetZoom(C2DVector(value / MaxValue * 32.0f, 1.0f));
}