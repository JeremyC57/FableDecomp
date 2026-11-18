// CConsole::NextCommandGuess() -- step the command guess: unless the front line still equals (case-insensitively) the lower-cased user entry, advance GuessPos through the registered inputs and show the next input whose name starts with the user entry, else leave guess mode
// Leaf: ?NextCommandGuess@CConsole@@AAEXXZ
// Naming (agentNF31, 2026-11-12): CCharString::ToLower (0x0099ED10) respelled GetAsLowerCase: Ego_r.pdb 0x00AFA690 + FableWin.pdb
//   0x02F61580 ?GetAsLowerCase@CCharString@@QBE?AV1@XZ (fable_string.h declares it at 0x0099ED10).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB0D0, 327 bytes (336 with padding).
// Callees: CCharString copy ctor 0x0099EC30, ToLower 0x0099ED10 (by value), dtor 0x0099EAE0, operator= 0x0099EFB0;
//          _stricmp 0x00BFEBA8, strncmp 0x00BFEAF8; the map iterator's operator++ 0x009EE470 (manifest
//          BinaryTree_FindSuccessor); CConsole::SetGuessMode 0x009EAFF0; CConsoleCommandLine(const
//          CCharString&) 0x009EEA10 and CBaseClass::~CBaseClass 0x0099A2E0; the input's virtual slot 2 (+0x08,
//          GetFormattedName, CCharString by value).
//
// Shape: both command lines hand out their entry by value (`GetEntry()`), so the user entry is copied before
// ToLower and the front entry is copied for the case-insensitive test, an inline CCharString member (the copy is
// built before either GetData; the bool is `== 0`, `neg ; sbb ; inc`). GetData
// is the inline `p ? p->PData : ""` (empty literal 0x0129AAF4); strncmp's lower-entry data and length share one
// null test. The new front line is assigned straight from the input's formatted name: the implicit conversion
// through CConsoleCommandLine(const CCharString&) builds the line temporary that the implicit operator= reads
// with `lea ecx,[esp+18h]` (an explicit `CConsoleCommandLine(...)` temporary reuses the ctor's eax instead); the
// line (string dtor, then the base dtor, no vtable re-store) and the name die at the end of the statement.
// Layout: the retail CConsole offsets of the landed rows (0x009EA660, 0x009EB260): FrontCommandLine +0x1c,
// BackCommandLine +0x2c, RegisteredConsoleInputs +0x40, GuessPos +0x68.
// NF87 (2026-11-17): moved onto the CConsole model of NF85's landed 0x009EAFF0 SetGuessMode row (CConsole has no shared header):
//   the private members Ego_r.pdb + FableWin.pdb spell (?NextCommandGuess@CConsole@@AAEXXZ, ?SetGuessMode@CConsole@@AAEXXZ), the
//   STLport map<CCharString, CCountedPointer<CConsoleInputBase> > (its iterator's out-of-line operator++ 0x009EE470 under the STLport
//   spelling, TREE_INC_OUTOFLINE) and fable_string.h's EqualsCaseInsensitive / StartsWith (the inline _stricmp / strncmp tests).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CConsole;
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase();                                                  // slot 0
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console) = 0;   // slot 1
    virtual CCharString GetFormattedName() const = 0;                              // slot 2 (+8)
    CCharString Name;                                                              // +0x04
};
class CConsoleCommandLine : public CBaseClass {
    friend class CConsole;
public:
    CConsoleCommandLine(const CCharString& text);           // 0x009EEA10
private:
    CCharString GetString() const { return CurrentEntry; }
    CCharString CurrentEntry;                               // +0x04
    bool        UserEntry;                                  // +0x08
    long        CursorPosition;                             // +0x0c
};

class CConsole {
public:
    virtual ~CConsole();
private:
    void SetGuessMode();                                    // 0x009EAFF0
    void NextCommandGuess();
    unsigned char _pad04[0x1c - 4];                         // KeyBindingMap +0x04, StringBindingMap +0x10
    CConsoleCommandLine FrontCommandLine;                   // +0x1c
    CConsoleCommandLine BackCommandLine;                    // +0x2c
    CCharString HelpBarString;                              // +0x3c
    std::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;   // +0x40
    unsigned char _pad4c[0x65 - 0x4c];                      // CommandLineHistory .. MovingIntoHistory
    bool GuessMode;                                         // +0x65
    std::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator GuessPos;      // +0x68
};

void CConsole::NextCommandGuess()
{
    CCharString userEntry = BackCommandLine.GetString().GetAsLowerCase();
    bool unchanged = FrontCommandLine.GetString().EqualsCaseInsensitive(userEntry);
    if (unchanged)
        return;
    ++GuessPos;
    if (GuessPos != RegisteredConsoleInputs.end() && (*GuessPos).first.StartsWith(userEntry)) {
        FrontCommandLine = (*GuessPos).second->GetFormattedName();
    } else {
        SetGuessMode();
    }
}
