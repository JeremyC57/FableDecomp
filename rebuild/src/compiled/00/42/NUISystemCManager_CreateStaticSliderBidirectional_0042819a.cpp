// NUISystem::CManager::CreateStaticSliderBidirectional(const CWideString& text, float value, const C2DCoordI& position)
// Leaf: CreateStaticSliderBidirectional
// Retail: Fable.exe @ 0x0042819A, 762 bytes, `ret 0Ch`; /Os + optimize("y", off) TU (as the landed CreateLevelSlider 0x00427DC9).
// Layout: SComponentState / CStateMap / CComponent slots from the landed CreateLevelSlider model.
#pragma optimize("s", on)
#pragma optimize("y", off)

class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC50 (a user copy: by-value arguments are built in place)
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    struct SData { long Refs; long Length; };
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : ((SData*)m_pStringData)->Length; }
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690 (hidden result in ecx)
class CWideString { public:
    void* m_pStringData;
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall LongToWideString(long value);   // 0x00405F00 (hidden result in ecx, value in edx)
class C2DCoordI { public:
    long X, Y;
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() {}
    CRGBFloatColour(float r, float g, float b, float a = 1.0f) : R(r), G(g), B(b), A(a) {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    __forceinline CRGBColour(const CRGBFloatColour& c)
    {
        R = (unsigned char)(long)(c.R * 255.0f);
        G = (unsigned char)(long)(c.G * 255.0f);
        B = (unsigned char)(long)(c.B * 255.0f);
        A = (unsigned char)(long)(c.A * 255.0f);
    }
};

namespace NUISystem {

struct C2DVector {
    float X;
    float Y;
};
struct SComponentState {
    unsigned char _pad00[8];
    C2DVector     Position;                 // +0x08
    C2DVector     Size;                     // +0x10
    CRGBColour    Colour;                   // +0x18
};
struct CStateMap {                          // map<unsigned long, SComponentState> (STLport, 12 bytes)
    unsigned char _body[12];
    SComponentState& operator[](const unsigned long& key);   // 0x0042D5B1
};

class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0xc)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual CStateMap& GetStateMap();                          // slot 82 (+0x148)
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
    virtual void SetTextNarrow(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};

class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, long parent);   // 0x0041DB1D
    CComponent* CreateStaticSliderBidirectional(const CWideString& text, float value, const C2DCoordI& position);
};

CComponent* CManager::CreateStaticSliderBidirectional(const CWideString& text, float value, const C2DCoordI& position)
{
    CComponent* slider;
    {
        CCharString name("UI_STATIC_SLIDER_BIDIRECTIONAL");
        slider = CreateComponent(name, 0);
    }
    CStateMap& states = slider->GetStateMap();
    states[0].Position.X = (float)position.X;
    states[0].Position.Y = (float)position.Y;
    states[1].Position.X = (float)position.X;
    states[1].Position.Y = (float)position.Y;
    states[3].Position.X = (float)position.X;
    states[3].Position.Y = (float)position.Y;
    float scale = (value + 100.0f) * 0.005f;
    CRGBFloatColour colour;
    colour = CRGBFloatColour(0.2f, 0.2f, 0.2f, 1.0f);
    CComponent* caption = slider->FindChild(CCharString("UI_STATIC_SLIDER_BIDIRECTIONAL_TEXT"));
    if (caption)
        caption->SetText(text, true, true);
    CComponent* button = slider->FindChild(CCharString("UI_STATIC_SLIDER_BIDIRECTIONAL_BUTTON"));
    CComponent* sprite = slider->FindChild(CCharString("UI_STATIC_SLIDER_BIDIRECTIONAL_BUTTON_SPRITE"));
    if (button) {
        CRGBColour tint(colour);
        sprite->GetStateMap()[0].Colour = tint;
        CStateMap& buttonStates = button->GetStateMap();
        buttonStates[1].Position.X = scale * 100.0f;
        buttonStates[3].Position.X = scale * 100.0f;
        CComponent* number = button->FindChild(CCharString("UI_STATIC_SLIDER_BIDIRECTIONAL_BUTTON_TEXT"));
        CCharString str = GFIntToCharString((long)value);
        if (value > 0.0f)
            str = "+" + str;
        if (number) {
            number->SetTextNarrow(str, true, true);
            float offset = 0.0f;
            unsigned long length = str.GetLength();
            if (length == 1)
                offset = 5.0f;
            if (length == 3)
                offset = -5.0f;
            if (length == 4)
                offset = -10.0f;
            number->GetStateMap()[0].Position.X += offset;
        }
    }
    return slider;
}

}