// NUISystem::CText::ForceWidth(float width) -- rebuilds the text value from its own characters, keeping only the leading run whose accumulated glyph widths stay within the given width, and sets it back through the virtual setter
// Leaf: ForceWidth
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FB10, 173 bytes (+3 int3). `ret 4`.
// Callees: CWideString::CWideString() 0x0099AED0, CWideString::GetLength() 0x0099B220, CWideString::GetAsCharArray()
//          0x0099AF10, GetCharWidthW(unsigned short) 0x0054F4B0 (`__fastcall`, unsigned result: the
//          `fild` + 2^32 fix-up @ 0x0122DCB4), CWideString::operator+=(unsigned short) 0x0099B9A0, own slot 144 (+0x240)
//          SetValue(const CWideString&, bool, bool), ~CWideString 0x0099B510.
// Shape: the local string is built first (frame +4), the accumulator (frame +0) zeroed and the index (esi) cleared; the
//        loop re-reads the character pointer and the length through the out-of-line calls each pass (`i != GetLength()`,
//        tested once before the loop), the width is added into the accumulator and stored (`fst`) before the `>` test
//        that breaks out; the character is appended only when it still fits.
// Layout: retail CDraggableInto 0x15C: Value (CWideString) +0x15C (see the landed ctor 0x0054F5C0).
class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    const unsigned short* GetAsCharArray() const;                 // 0x0099AF10
    const CWideString& operator+=(unsigned short c);              // 0x0099B9A0
    unsigned short operator[](unsigned long i) const { return GetAsCharArray()[i]; }   // inline: the character is a temporary before the register argument
public:
    long GetLength() const;   // 0x0099B220
};
unsigned long __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0 (`mov si,cx`)
namespace NUISystem {
class CText {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetValue(const CWideString& value, bool a, bool b);   // slot 144 (+0x240) 0x0054F410
    unsigned char _pad04[0x15c - 0x04];
    CWideString   Value;                                    // +0x15C
    void ForceWidth(float width);
};
}
void NUISystem::CText::ForceWidth(float width)
{
    CWideString fitted;
    float accumulated = 0.0f;
    for (unsigned long i = 0; i != Value.GetLength(); ++i) {
        accumulated += (float)GetCharWidthW(Value[i]);
        if (accumulated > width)
            break;
        fitted += Value[i];
    }
    SetValue(fitted, true, true);
}