// CTCDayOrNightOnlySupport::GetSectionNameSuffix(const CCharString& name) -- name + "DayOnly" / "NightOnly" by DayNightOrBoth, else an empty string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702AC0, length 74 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0; operator+(const CCharString&, const CCharString&)
// @ 0x0099F570 (`__fastcall`: hidden result in ecx, left in edx, right on the stack -- as in the landed
// C3DMeshFile*Chunk sources). Globals: the static CCharStrings "DayOnly" @ 0x013BAC18 and "NightOnly"
// @ 0x013BAC1C (their `$E` initialisers at 0x0121E5C0 / 0x0121E5E0 construct them from the literals
// 0x01264DAC / 0x01264DB4); the empty literal @ 0x0122D70E.
// PDB CTCDayOrNightOnlySupport (0x10 bytes): DayNightOrBoth (EDayNightOrBoth) +0x0c.
// Shape: `ret 8` = hidden result + one argument; the two `dec eax ; je` tests are a two-case switch
// (case 1 = day only, case 2 = night only) with the default (empty string) laid out first; the result
// pointer is hoisted into esi/ecx above the dispatch.
struct CCharString {
    void* m_pStringData;                                    // CCharStringData*
    CCharString(const char* text, long length);             // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

extern CCharString g_DayOnlySuffix;                         // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;                       // 0x013BAC1C "NightOnly"

struct CTCDayOrNightOnlySupport {
    virtual void v00();
    unsigned char _base[0x8];                               // CTCBase: ParentThing +4, flags +8/+9
    long          DayNightOrBoth;                           // +0x0c EDayNightOrBoth
    CCharString GetSectionNameSuffix(const CCharString& name) const;
};

CCharString CTCDayOrNightOnlySupport::GetSectionNameSuffix(const CCharString& name) const
{
    switch (DayNightOrBoth) {
    case 1:
        return name + g_DayOnlySuffix;
    case 2:
        return name + g_NightOnlySuffix;
    }
    return CCharString("", -1);
}
