// CTCInventoryStats::GetTimeSpentInGame(unsigned long frames, const NGameText::CDataBank* bank) -- formats the play time (frames at 15 per second) as "<Time spent>: <h> <hour(s)> <m> <mins>" from the game text bank
// Leaf: ?GetTimeSpentInGame@CTCInventoryStats@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CCC91, 538 bytes (0x21A, no padding), static __fastcall `ret 4` (hidden return
//   pointer in ecx, frames in edx, bank on the stack). Four callers (0x0047F268, 0x00595FFD, 0x005CE6F7, 0x005D2CED).
// Identity: Ego_r.pdb ?GetTimeSpentInGame@CTCInventoryStats@@SI?AVCWideString@@KPBVCDataBank@NGameText@@@Z; the
//   catalog's bare "GetTimeSpentInGame" lacks the class.
// Callees: CWideString(const wchar_t*) 0x0099B6B0, CWideString() 0x0099AED0, CWideString(const CWideString&) 0x0099B720,
//   ~CWideString 0x0099B510, CWideString::operator= 0x0099B7D0, CWideString::operator+= 0x0099B8D0, operator+(const
//   CWideString&, const CWideString&) 0x0099BE70 (__fastcall, hidden return in ecx), GFIntToWideString 0x00405F00
//   (__fastcall, hidden return in ecx), CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0. Strings: L" " 0x0122D820, L": " 0x01236A50, L"\n" 0x01231728,
//   "TEXT_GUI_STATS_LABEL_HOUR" 0x012549F8, "..._HOURS" 0x012549DC, "..._MINS" 0x012549C0, "..._TIMESPENT" 0x012549A0.
// Globals: the main game component 0x013B86A0 (PTextBank, a CCountedPointer<NGameText::CDataBank>, at +0x14 -- as
//   the landed CMainGameComponent::Render 0x00417001), the default bank.
// Shape: /Os frame-pointer TU, no EH. Retail builds an unused L"\n" string. The singular/plural hour label is an
//   if/else with a named CCharString in each branch (VC7.1 merges the identical tails, so only the pushed string
//   differs; as a temporary the label swaps frame slots with the GetTextBySymbol result); the minutes and time-spent
//   label CCharStrings are named in a block around the one long concatenation (both built before the expression,
//   both destroyed before the return copy); the dead `bank` argument slot holds temporaries.
#pragma optimize("s", on)
#pragma optimize("y", off)

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    ~CCharString();                                                                    // 0x0099EAE0
};

class CWideString {
public:
    void* PStringData;
    CWideString();                                                                     // 0x0099AED0
    CWideString(const unsigned short* text);                                           // 0x0099B6B0
    CWideString(const CWideString& other);                                             // 0x0099B720
    ~CWideString();                                                                    // 0x0099B510
    const CWideString& operator=(const CWideString& other);                            // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);                           // 0x0099B8D0
};

CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70
CWideString __fastcall GFIntToWideString(long value);                                 // 0x00405F00

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;                // 0x009C95E0
};
}

template <class T> class CCountedPointer {
public:
    T*    Data;                                                                        // +0x00
    void* PInfo;                                                                       // +0x04
    T* Get() const { return Data; }
};

class CMainGameComponent {
public:
    unsigned char                         _pad_0x00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;                                   // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;                                      // 0x013B86A0

class CTCInventoryStats {
public:
    static CWideString __fastcall GetTimeSpentInGame(unsigned long frames, const NGameText::CDataBank* bank);
};

CWideString __fastcall CTCInventoryStats::GetTimeSpentInGame(unsigned long frames, const NGameText::CDataBank* bank)
{
    CWideString space(L" ");
    CWideString colon(L": ");
    CWideString newLine(L"\n");
    CWideString text;
    if (bank == 0)
        bank = g_pMainGameComponent->PTextBank.Get();
    CWideString hoursText;
    unsigned long seconds = frames / 15;
    unsigned long hours = seconds / 3600;
    unsigned long secondsInHour = seconds % 3600;
    hoursText = GFIntToWideString(hours) + space;
    if (hours == 1) {
        CCharString hourLabel("TEXT_GUI_STATS_LABEL_HOUR");
        hoursText += bank->GetTextBySymbol(hourLabel);
    }
    else {
        CCharString hoursLabel("TEXT_GUI_STATS_LABEL_HOURS");
        hoursText += bank->GetTextBySymbol(hoursLabel);
    }
    {
        CCharString minsLabel("TEXT_GUI_STATS_LABEL_MINS");
        CCharString timeSpentLabel("TEXT_GUI_STATS_LABEL_TIMESPENT");
        text = bank->GetTextBySymbol(timeSpentLabel) + colon + hoursText + space +
               GFIntToWideString(secondsInHour / 60) + space + bank->GetTextBySymbol(minsLabel);
    }
    return text;
}