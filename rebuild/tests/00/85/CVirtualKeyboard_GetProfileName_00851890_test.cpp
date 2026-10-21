#include <cstdio>
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

void CEditBox::_v000() {}
void CEditBox::_v001() {}
void CEditBox::_v002() {}
void CEditBox::_v003() {}
void CEditBox::_v004() {}
void CEditBox::_v005() {}
void CEditBox::_v006() {}
void CEditBox::_v007() {}
void CEditBox::_v008() {}
void CEditBox::_v009() {}
void CEditBox::_v010() {}
void CEditBox::_v011() {}
void CEditBox::_v012() {}
void CEditBox::_v013() {}
void CEditBox::_v014() {}
void CEditBox::_v015() {}
void CEditBox::_v016() {}
void CEditBox::_v017() {}
void CEditBox::_v018() {}
void CEditBox::_v019() {}
void CEditBox::_v020() {}
void CEditBox::_v021() {}
void CEditBox::_v022() {}
void CEditBox::_v023() {}
void CEditBox::_v024() {}
void CEditBox::_v025() {}
void CEditBox::_v026() {}
void CEditBox::_v027() {}
void CEditBox::_v028() {}
void CEditBox::_v029() {}
void CEditBox::_v030() {}
void CEditBox::_v031() {}
void CEditBox::_v032() {}
void CEditBox::_v033() {}
void CEditBox::_v034() {}
void CEditBox::_v035() {}
void CEditBox::_v036() {}
void CEditBox::_v037() {}
void CEditBox::_v038() {}
void CEditBox::_v039() {}
void CEditBox::_v040() {}
void CEditBox::_v041() {}
void CEditBox::_v042() {}
void CEditBox::_v043() {}
void CEditBox::_v044() {}
void CEditBox::_v045() {}
void CEditBox::_v046() {}
void CEditBox::_v047() {}
void CEditBox::_v048() {}
void CEditBox::_v049() {}
void CEditBox::_v050() {}
void CEditBox::_v051() {}
void CEditBox::_v052() {}
void CEditBox::_v053() {}
void CEditBox::_v054() {}
void CEditBox::_v055() {}
void CEditBox::_v056() {}
void CEditBox::_v057() {}
void CEditBox::_v058() {}
void CEditBox::_v059() {}
void CEditBox::_v060() {}
void CEditBox::_v061() {}
void CEditBox::_v062() {}
void CEditBox::_v063() {}
void CEditBox::_v064() {}
void CEditBox::_v065() {}
void CEditBox::_v066() {}
void CEditBox::_v067() {}
void CEditBox::_v068() {}
void CEditBox::_v069() {}
void CEditBox::_v070() {}
void CEditBox::_v071() {}
void CEditBox::_v072() {}
void CEditBox::_v073() {}
void CEditBox::_v074() {}
void CEditBox::_v075() {}
void CEditBox::_v076() {}
void CEditBox::_v077() {}
void CEditBox::_v078() {}
void CEditBox::_v079() {}
void CEditBox::_v080() {}
void CEditBox::_v081() {}
void CEditBox::_v082() {}
void CEditBox::_v083() {}
void CEditBox::_v084() {}
void CEditBox::_v085() {}
void CEditBox::_v086() {}
void CEditBox::_v087() {}
void CEditBox::_v088() {}
void CEditBox::_v089() {}
void CEditBox::_v090() {}
void CEditBox::_v091() {}
void CEditBox::_v092() {}
void CEditBox::_v093() {}
void CEditBox::_v094() {}
void CEditBox::_v095() {}
void CEditBox::_v096() {}
void CEditBox::_v097() {}
void CEditBox::_v098() {}
void CEditBox::_v099() {}
void CEditBox::_v100() {}
void CEditBox::_v101() {}
void CEditBox::_v102() {}
void CEditBox::_v103() {}
void CEditBox::_v104() {}
void CEditBox::_v105() {}
void CEditBox::_v106() {}
void CEditBox::_v107() {}
void CEditBox::_v108() {}
void CEditBox::_v109() {}
void CEditBox::_v110() {}
void CEditBox::_v111() {}
void CEditBox::_v112() {}
void CEditBox::_v113() {}
void CEditBox::_v114() {}
void CEditBox::_v115() {}
void CEditBox::_v116() {}
void CEditBox::_v117() {}
void CEditBox::_v118() {}
void CEditBox::_v119() {}
void CEditBox::_v120() {}
void CEditBox::_v121() {}
void CEditBox::_v122() {}
void CEditBox::_v123() {}
void CEditBox::_v124() {}
void CEditBox::_v125() {}
void CEditBox::_v126() {}
void CEditBox::_v127() {}
void CEditBox::_v128() {}
void CEditBox::_v129() {}
void CEditBox::_v130() {}
void CEditBox::_v131() {}
void CEditBox::_v132() {}
void CEditBox::_v133() {}
void CEditBox::_v134() {}
void CEditBox::_v135() {}
void CEditBox::_v136() {}
void CEditBox::_v137() {}
void CEditBox::_v138() {}
void CEditBox::_v139() {}
void CEditBox::_v140() {}
void CEditBox::_v141() {}
class CWideStringData { public: wchar_t Buf[64]; long Len; };
static CWideStringData g_pool[128]; static int g_used, g_live;
static CWideStringData* NewData(const wchar_t* s, long n)
{ CWideStringData* d = &g_pool[g_used++]; for (long i = 0; i < n; ++i) d->Buf[i] = s[i]; d->Buf[n] = 0; d->Len = n; ++g_live; return d; }
CWideString::CWideString(const CWideString& s) { PStringData = NewData(s.PStringData->Buf, s.PStringData->Len); }
CWideString::~CWideString() { --g_live; }
long CWideString::GetLength() const { return PStringData->Len; }
CWideString::operator const wchar_t*() const { return PStringData->Buf; }
CWideString CWideString::GetSubString(long start, long end) const
{ CWideString r(*this); --g_live; r.PStringData = NewData(PStringData->Buf + start, end - start); return r; }
static CWideString* g_src;
CWideString CEditBox::GetText() const { return *g_src; }
static bool Check(const wchar_t* in, const wchar_t* want)
{
    long n = 0; while (in[n]) ++n;
    CWideStringData d; for (long i = 0; i <= n; ++i) d.Buf[i] = in[i]; d.Len = n;
    CWideString* s = (CWideString*)&d; CWideStringData* pd = &d; g_src = (CWideString*)&pd;
    (void)s;
    static CEditBox box; static unsigned char raw[sizeof(CVirtualKeyboard)];
    CVirtualKeyboard_Methods& k = *(CVirtualKeyboard_Methods*)raw; k.PEntryBox = &box;
    int live0 = g_live; bool ok;
    {
        CWideString r = k.GetProfileName();
        long m = 0; while (want[m]) ++m;
        ok = r.GetLength() == m;
        for (long i = 0; ok && i < m; ++i) ok = ((const wchar_t*)r)[i] == want[i];
    }
    return ok && g_live == live0;
}
int main()
{
    bool ok = Check(L"  ab c  ", L"ab c") && Check(L"abc", L"abc") && Check(L"   ", L"") && Check(L"", L"") && Check(L" x", L"x");
    std::printf(ok ? "00851890_TEST PASS\n" : "00851890_TEST FAIL\n");
    return ok ? 0 : 1;
}