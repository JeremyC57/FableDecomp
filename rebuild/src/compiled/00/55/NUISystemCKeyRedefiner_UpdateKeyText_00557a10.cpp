// NUISystem::CKeyRedefiner::UpdateKeyText() -- show the bound key: when the action has a controller binding and it has a name, the upper-cased key name in white, otherwise the text tag TEXT_GUI_KEY_UNDEFINED in red
// Leaf: UpdateKeyText
// Retail: Fable.exe (TLC, VC7.1) @ 0x00557A10, 219 bytes; called from CancelSelection (0x00557BD0, landed).
// Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, CWideString() @ 0x0099AED0, GetActionName(const
//   CActionInputControl&, CWideString&, bool) @ 0x00446720 (`__fastcall`), CWideString::operator const wchar_t* @
//   0x0099AF10, _wcsupr @ 0x00F3FB8A (result unused: in place), CWideString::operator= @ 0x0099B7D0,
//   CWideString::~CWideString @ 0x0099B510, the key text's slots 144 (+0x240, SetText(text, isTag, bool)) and 13 (+0x34,
//   SetColour). Literal L"TEXT_GUI_KEY_UNDEFINED" @ 0x0124ABCC.
// Layout: Ego_r.pdb NUISystem::CKeyRedefiner with the retail offsets (the CSprite part 0x30 below the PDB, the 4-byte
//   STLport list): PKeyText +0x198, Definition (CActionInputControl) +0x1a4 (ControllerType +0x1a8).
// Shape: the text, the tag flag and the colour are function-scope locals (the flag set before the text ctor runs, the
//   red colour built in place by the R,G,B,A-assigning ctor); the key name lives in the `if` block; the white colour is
//   a temporary assigned over the local (VC builds it in the local's own slot, hence the self copy).
class CWideString { public:
    void* m_pStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const unsigned short* text);       // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    operator unsigned short*() const;              // 0x0099AF10
};
extern "C" unsigned short* __cdecl _wcsupr(unsigned short* text);   // 0x00F3FB8A
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CActionInputControl { public:                       // 28 bytes
    long  GameAction;                              // +0x00
    long  ControllerType;                          // +0x04
    long  KeyboardKey;                             // +0x08
    long  XboxButton;                              // +0x0c
    long  MouseButton;                             // +0x10
    float ControlDirection[2];                     // +0x14
};
bool __fastcall GetActionName(const CActionInputControl& control, CWideString& name, bool shortName);   // 0x00446720

namespace NUISystem {
class CText { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012();
    virtual void SetColour(const CRGBColour& colour);   // slot 13 (+0x34)
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool isTag, bool b);   // slot 144 (+0x240)
};
class CKeyRedefiner { public:
    virtual void v00();
    unsigned char       _sprite04[0x194];          // +0x04 the CSprite / control bases
    CText*              PKeyText;                  // +0x198
    void*               Actions;                   // +0x19c list<EGameAction> (sentinel)
    long                SubType;                   // +0x1a0
    CActionInputControl Definition;                // +0x1a4
    void UpdateKeyText();
};

void CKeyRedefiner::UpdateKeyText()
{
    bool isTag = true;
    CWideString text((const unsigned short*)L"TEXT_GUI_KEY_UNDEFINED");
    CRGBColour colour(0xff, 0, 0, 0xff);
    if (Definition.ControllerType) {
        CWideString keyName;
        if (GetActionName(Definition, keyName, false)) {
            isTag = false;
            _wcsupr(keyName);
            text = keyName;
            colour = CRGBColour(0xff, 0xff, 0xff, 0xff);
        }
    }
    if (PKeyText) {
        PKeyText->SetText(text, isTag, true);
        PKeyText->SetColour(colour);
    }
}
}