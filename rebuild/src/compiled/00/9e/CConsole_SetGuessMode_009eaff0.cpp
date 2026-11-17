// CConsole::SetGuessMode() -- enter command-guess mode: look the lower-cased current line up in the registered console inputs and, when the found input's name starts with it, take that input's formatted name as the guessed command line
// Leaf: ?SetGuessMode@CConsole@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EAFF0, 221 bytes (224 with padding).
// Identity: Ego_r.pdb and FableWin.pdb ?SetGuessMode@CConsole@@AAEXXZ (private; the old row said public QAE).
// Callees: CCharString copy ctor 0x0099EC30, CCharString::GetAsLowerCase 0x0099ED10 (result by hidden pointer; PDB ?GetAsLowerCase@CCharString@@QBE?AV1@XZ), ~CCharString
//   0x0099EAE0, RegisteredConsoleInputs' _M_lower_bound 0x009EEC10 (STLport map<CCharString, CCountedPointer<CConsoleInputBase> >, NF82),
//   strncmp 0x00BFEAF8, the input's virtual slot 2 GetFormattedName (+8, its text by value), CConsoleCommandLine::CConsoleCommandLine(const
//   CCharString&) 0x009EEA10, CCharString::operator= 0x0099EFB0, CBaseClass::~CBaseClass 0x0099A2E0 (the command line's base); NullChar 0x0129AAF4.
// Layout (Ego_r.pdb CConsole members at the retail offsets, STLport maps 12 bytes): FrontCommandLine +0x1c, BackCommandLine +0x2c
//   (CConsoleCommandLine: vptr, CurrentEntry +4, UserEntry +8, CursorPosition +0xc), HelpBarString +0x3c, RegisteredConsoleInputs +0x40,
//   GuessMode +0x65, GuessPos +0x68; map node key +0x10, value +0x14. CConsoleInputBase (Ego_r.pdb: vptr, Name +4) slots from the retail
//   vtables (0x0122E5B0 / CConsoleCommandBase 0x0122E638): 0 the destructor, 1 ProcessCommandLine, 2 GetFormattedName (const).
// Shape: the lower-cased line comes from the by-value CConsoleCommandLine::GetString (Ego_r.pdb ?GetString@CConsoleCommandLine@@ABE?AVCCharString@@XZ,
//   inline: the CurrentEntry copy is built before the hidden-result address, then destroyed right after the GetAsLowerCase call); the
//   name test is the inline CCharString::StartsWith (strncmp on the two GetAsCharArray texts and the prefix length); the guess goes
//   through the old row's forced-inline SetGuessLine(const CCharString&), so the GetFormattedName temporary outlives the named
//   CConsoleCommandLine (its text, ~CBaseClass, then the name) while the ctor still takes the returned pointer. NF85 tried the one-
//   expression `FrontCommandLine = CConsoleCommandLine(name)` (the temporary is addressed through the ctor's eax: 220 v 221) and a
//   reference-bound name with a named command line (the argument re-addressed: 225 v 221).
// NF85 (q11068): moved off the row-local CCharString / Dinkumware command-map (_Lbound stand-in) models onto fable_string.h and
//   stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound, as the
//   landed sibling CConsole::AddInput 0x009EC5E0 models the map); CConsole / CConsoleInputBase / CConsoleCommandLine have no shared header.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
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
    void SetGuessMode();
    unsigned char _pad04[0x1c - 4];                         // KeyBindingMap +0x04, StringBindingMap +0x10
    CConsoleCommandLine FrontCommandLine;                   // +0x1c
    CConsoleCommandLine BackCommandLine;                    // +0x2c
    CCharString HelpBarString;                              // +0x3c
    std::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;   // +0x40
    unsigned char _pad4c[0x65 - 0x4c];                      // CommandLineHistory .. MovingIntoHistory
    bool GuessMode;                                         // +0x65
    std::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator GuessPos;      // +0x68
    // Kept from the landed row (descriptive name; no PDB member): the guess line built as a named command line from the name, so the
    // name temporary outlives it and the copy reads the named object from the frame.
    FABLE_FORCEINLINE void SetGuessLine(const CCharString& text)
    {
        CConsoleCommandLine guess(text);
        FrontCommandLine = guess;
    }
};

void CConsole::SetGuessMode()
{
    GuessMode = true;
    CCharString lowered = BackCommandLine.GetString().GetAsLowerCase();
    GuessPos = RegisteredConsoleInputs.lower_bound(lowered);
    if (GuessPos != RegisteredConsoleInputs.end() && (*GuessPos).first.StartsWith(lowered))
        SetGuessLine((*GuessPos).second->GetFormattedName());
}
