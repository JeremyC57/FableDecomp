// Grade: DIFFER(305v305) -- scheduling only: the spill of fabs(step) to [ebp-4] lands before the first argument-slot CCharString setup (push ecx; mov ecx,esp; push -1) instead of after it. Fixed on the way: declared CCharString copy ctor (argument built in place), the test written on the unnamed expression so the quotient stays on the x87 stack (fld st(1)), and `if (!Play)` first for the retail block layout
// CSliderAnimator::Update() -- advance the slider animation: the step is |(EndValue - StartValue) / Overall| (a step of exactly -1 stops the animation), the fraction `a` grows by 1 / (15 * step) while playing and clamps to 1 (finishing it), otherwise it sits at 1 when finished or 0; the interpolated value is written as "<value><parenthesised suffix>" to the UI_LEVEL_SLIDER_NUMBER text and the UI_LEVEL_SLIDER_FILL_SPRITE is scaled to value / MaxValue * 32 wide
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056EB48, length 305 bytes (`ret`, frame pointer + `leave`). FableWin.pdb
// CSliderAnimator is 28 bytes {CComponent* pSlider, float StartValue, EndValue, MaxValue, a, Overall, bool Play, Finished}.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 built in the argument slot, the slider's child lookup (virtual slot 3,
// +0x0c, CCharString by value), ExtractParenthesis @ 0x0056EAD6 (same TU), _ftol @ 0x00BFEA70, IntToWideString @ 0x00405F00
// and operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (both __fastcall, hidden result in ecx), CText slot 144
// (+0x240) set-text, ~CWideString @ 0x0099B510, the fill sprite's slot 11 (+0x2c) taking a C2DVector by reference.
// TU: `#pragma optimize("s", on)` + `("y", off)` like the landed sibling ExtractParenthesis (ebp frame, `leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
struct CCharStringData { const char* m_pData; unsigned long m_length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30 (declared so the by-value argument is built in place)
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);                                   // 0x00405F00
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

namespace NUISystem {
struct CComponent {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual CComponent* GetChild(CCharString name);            // slot 3 (+0x0c)
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void SetScale(const C2DVector& scale);             // slot 11 (+0x2c)
};
struct CText : CComponent {
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
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
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
    float speed = (float)fabs((EndValue - StartValue) / Overall);
    NUISystem::CComponent* fill = pSlider->GetChild(CCharString("UI_LEVEL_SLIDER_FILL_SPRITE"));
    NUISystem::CText* number = (NUISystem::CText*)pSlider->GetChild(CCharString("UI_LEVEL_SLIDER_NUMBER"));
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
    number->SetText(IntToWideString(value) + suffix, true, true);
    fill->SetScale(C2DVector(value / MaxValue * 32.0f, 1.0f));
}
