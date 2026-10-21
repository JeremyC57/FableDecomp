// NUISystem::CManager::CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel, const CRGBColour& colour) -- build a "UI_LEVEL_SLIDER" component at a position: its caption, its level number and a fill sprite scaled to level/maxLevel and tinted with the colour
// Leaf: CreateLevelSlider
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00427DC9, length 358 bytes (unaligned: a size-optimised frame-pointer TU), `ret 14h`.
// Ego_r.pdb: `?CreateLevelSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBColour@@@Z`.
// Pragmas: #pragma optimize("s", on) + #pragma optimize("y", off) (ebp frame closed with `leave`, as the landed 0x00647457).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CManager::CreateComponent(const CCharString&, long)
//   @ 0x0041DB1D, the component's virtual slots 82 GetStateMap (+0x148), 3 FindChild(CCharString by value) (+0xc) and 144
//   SetText(const CWideString&, bool, bool) (+0x240), map<unsigned long, SComponentState>::operator[] @ 0x0042D5B1 (as the landed
//   CComponentContainer::Initialise 0x005461D0), the `__fastcall` CWideString-from-long helper @ 0x00405F00 (manifest "CGuiControlNumber::SetSLongValue"
//   is a twin), ~CWideString @ 0x0099B510.
// Strings: "UI_LEVEL_SLIDER" @ 0x012309D8, "UI_LEVEL_SLIDER_TEXT" @ 0x012309C0, "UI_LEVEL_SLIDER_NUMBER" @ 0x012309A8,
//   "UI_LEVEL_SLIDER_FILL_SPRITE" @ 0x0123098C; the fill width 32.0f @ 0x01230894.
// Layout: SComponentState (landed model): Position +8, Size +0x10, Colour +0x18.
// Shape: each state-0 lookup takes a fresh zero key; the fill fraction is 1 for a zero maximum, else level / maxLevel (fild/fidiv);
//   the tint is a named CRGBColour(R, G, B, 0xdc) built before the key (an unnamed temporary stores the key first); the slider name
//   is a named CCharString in its own block (built before any push, destroyed right after CreateComponent); the number text
//   comes from a `__fastcall` CWideString-by-value helper (hidden result in ecx, the long in edx) built in the dead text slot.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC50 (a user copy: by-value arguments are built in place)
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* m_pStringData;
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall LongToWideString(long value);   // 0x00405F00 (hidden result in ecx, value in edx)
struct C2DCoordI {
    long X, Y;
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
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
};

class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, long parent);   // 0x0041DB1D
    CComponent* CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel, const CRGBColour& colour);
};

CComponent* CManager::CreateLevelSlider(const CWideString& text, const C2DCoordI& position, long level, long maxLevel, const CRGBColour& colour)
{
    CComponent* slider;
    {
        CCharString name("UI_LEVEL_SLIDER");
        slider = CreateComponent(name, 0);
    }
    CStateMap& states = slider->GetStateMap();
    states[0].Position.X = (float)position.X;
    states[0].Position.Y = (float)position.Y;
    CComponent* caption = slider->FindChild(CCharString("UI_LEVEL_SLIDER_TEXT"));
    if (caption)
        caption->SetText(text, true, true);
    CComponent* number = slider->FindChild(CCharString("UI_LEVEL_SLIDER_NUMBER"));
    if (number)
        number->SetText(LongToWideString(level), true, true);
    CComponent* fill = slider->FindChild(CCharString("UI_LEVEL_SLIDER_FILL_SPRITE"));
    if (fill) {
        CStateMap& fillStates = fill->GetStateMap();
        float fraction = maxLevel == 0 ? 1.0f : (float)level / maxLevel;
        fillStates[0].Size.X = fraction * 32.0f;
        CRGBColour tint(colour.R, colour.G, colour.B, 0xdc);
        fillStates[0].Colour = tint;
    }
    return slider;
}

}