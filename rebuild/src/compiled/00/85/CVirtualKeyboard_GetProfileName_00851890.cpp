// CVirtualKeyboard::GetProfileName() -- the entry box text with leading and trailing spaces trimmed
// Leaf: ?GetProfileName@CVirtualKeyboard@@QAE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x00851890, 137 bytes, `ret 4` (hidden return buffer); callers 0x00599F39, 0x0085193E.
// Body: text = PEntryBox->GetText() (CEditBox slot 142, 0x00540CF0: copy of the CWideString at +0x164); first = 0,
// last = GetLength() - 1 (0x0099B220); skip leading L' ' through operator const wchar_t* (0x0099AF10) while first <
// GetLength() (re-read each pass); trim trailing L' ' while last > first; return GetSubString(first, last + 1)
// (0x0099BDB0, half-open range) and destroy the temporary (~CWideString 0x0099B510).
// Layout: engine/CVirtualKeyboard.h (PEntryBox +0x0c).
// Opt: /O2 (16-aligned entry; the 2-byte `mov edi,edi` before the first loop is the compiler's loop-top pad).
#include "engine/CVirtualKeyboard.h"
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& s);                   // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    long GetLength() const;                              // 0x0099B220
    operator const wchar_t*() const;                     // 0x0099AF10
    CWideString GetSubString(long start, long end) const; // 0x0099BDB0
};
struct CEditBox {
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055(); virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080(); virtual void _v081(); virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087(); virtual void _v088(); virtual void _v089(); virtual void _v090(); virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095(); virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099(); virtual void _v100(); virtual void _v101(); virtual void _v102(); virtual void _v103(); virtual void _v104(); virtual void _v105(); virtual void _v106(); virtual void _v107(); virtual void _v108(); virtual void _v109(); virtual void _v110(); virtual void _v111(); virtual void _v112(); virtual void _v113(); virtual void _v114(); virtual void _v115(); virtual void _v116(); virtual void _v117(); virtual void _v118(); virtual void _v119(); virtual void _v120(); virtual void _v121(); virtual void _v122(); virtual void _v123(); virtual void _v124(); virtual void _v125(); virtual void _v126(); virtual void _v127(); virtual void _v128(); virtual void _v129(); virtual void _v130(); virtual void _v131(); virtual void _v132(); virtual void _v133(); virtual void _v134(); virtual void _v135(); virtual void _v136(); virtual void _v137(); virtual void _v138(); virtual void _v139(); virtual void _v140(); virtual void _v141();
    virtual CWideString GetText() const;                 // slot 142 (+0x238), 0x00540CF0
};
struct CVirtualKeyboard_Methods : CVirtualKeyboard {
    CWideString GetProfileName();
};
CWideString CVirtualKeyboard_Methods::GetProfileName()
{
    CWideString text = PEntryBox->GetText();
    long first = 0;
    long last = text.GetLength() - 1;
    while (first < text.GetLength() && ((const wchar_t*)text)[first] == L' ')
        ++first;
    while (last > first && ((const wchar_t*)text)[last] == L' ')
        --last;
    return text.GetSubString(first, last + 1);
}